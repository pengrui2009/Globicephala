/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : os_timer.c
 @brief  : timer 
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           ...
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/prctl.h>

#include "os_timer.h"

/* Number of threads used.  */
#define THREAD_MAXNODES    16

/* The limit is not published if we are compiled with kernel timer support.
   But we still compiled in this implementation with its limit unless built
   to require the kernel support.  */
#ifndef TIMER_MAX
#define TIMER_MAX 256
#endif

#ifndef DELAYTIMER_MAX
#define DELAYTIMER_MAX    2147483647
#endif

#ifndef __set_errno
#define __set_errno(e) (errno = (e))
#endif

/* Double linked list.  */
struct list_links
{
    struct list_links *next;
    struct list_links *prev;
};

/* Forward declaration.  */
struct timer_node;

/* Definitions for an internal thread of the POSIX timer implementation.  */
struct thread_node
{
    struct list_links links;
    pthread_attr_t attr;
    pthread_t id;
    unsigned int exists;
    struct list_links timer_queue;
    pthread_cond_t cond;
    struct timer_node *current_timer;
    pthread_t captured;
    clockid_t clock_id;
};

/* Internal representation of a timer.  */
struct timer_node
{
    struct list_links links;
    struct sigevent event;
    clockid_t clock;
    int type;
    int millisec;
    struct itimerspec value;
    struct timespec expirytime;
    pthread_attr_t attr;
    unsigned int abstime;
    unsigned int armed;
    enum 
    {
        TIMER_FREE, TIMER_INUSE, TIMER_DELETED
    }inuse;
    struct thread_node *thread;
    pid_t creator_pid;
    int refcount;
    int overrun_count;
};

/* Return pointer to timer structure corresponding to ID.  */
#define timer_id2ptr(timerid) ((struct timer_node *) (void *) timerid)
#define timer_ptr2id(timerid) ((timer_t) (void *) timerid)

/* Array containing the descriptors for the used threads.  */
static struct thread_node thread_array[THREAD_MAXNODES];

/* Static array with the structures for all the timers.  */
struct timer_node __timer_array[TIMER_MAX];

/* Global lock to protect operation on the lists.  */
pthread_mutex_t __timer_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Variable to protext initialization.  */
pthread_once_t __timer_init_once_control = PTHREAD_ONCE_INIT;

/* Nonzero if initialization of timer implementation failed.  */
int __timer_init_failed;

/* Node for the thread used to deliver signals.  */
struct thread_node __timer_signal_thread_rclk;

/* Lists to keep free and used timers and threads.  */
struct list_links timer_free_list;
struct list_links thread_free_list;
struct list_links thread_active_list;

static inline void zero_timespec(struct timespec * ts)
{
    ts->tv_sec = 0;
    ts->tv_nsec = 0;
}

static inline void zero_itimerspec(struct itimerspec * its)
{
    zero_timespec(&its->it_value);
    zero_timespec(&its->it_interval);
}

static inline void ms2ts(int ms, struct timespec * ts)
{
    ts->tv_sec = ms / 1000;
    ts->tv_nsec = (ms % 1000) * 1000000;
}

/* List handling functions.  */
static inline void list_init (struct list_links *list)
{
    list->next = list->prev = list;
}

static inline void list_append (struct list_links *list, struct list_links *newp)
{
    newp->prev = list->prev;
    newp->next = list;
    list->prev->next = newp;
    list->prev = newp;
}

static inline void list_insbefore (struct list_links *list, struct list_links *newp)
{
    list_append (list, newp);
}

/*
 * Like list_unlink_ip, except that calling it on a node that
 * is already unlinked is disastrous rather than a noop.
 */

static inline void list_unlink (struct list_links *list)
{
    struct list_links *lnext = list->next, *lprev = list->prev;

    lnext->prev = lprev;
    lprev->next = lnext;
}

static inline struct list_links *list_first (struct list_links *list)
{
    return list->next;
}

static inline struct list_links *list_null (struct list_links *list)
{
    return list;
}

static inline struct list_links *list_next (struct list_links *list)
{
    return list->next;
}

static inline int list_isempty (struct list_links *list)
{
    return list->next == list;
}

/* Timespec helper routines.  */
static inline int __attribute ((always_inline)) timespec_compare (const struct timespec *left, const struct timespec *right)
{
    if (left->tv_sec < right->tv_sec)
        return -1;

    if (left->tv_sec > right->tv_sec)
        return 1;

    if (left->tv_nsec < right->tv_nsec)
        return -1;
	
    if (left->tv_nsec > right->tv_nsec)
        return 1;

    return 0;
}

static inline void
timespec_add (struct timespec *sum, const struct timespec *left, const struct timespec *right)
{
    sum->tv_sec = left->tv_sec + right->tv_sec;
    sum->tv_nsec = left->tv_nsec + right->tv_nsec;

    if (sum->tv_nsec >= 1000000000)
    {
        ++sum->tv_sec;
        sum->tv_nsec -= 1000000000;
    }
}

static inline void timespec_sub (struct timespec *diff, const struct timespec *left, const struct timespec *right)
{
    diff->tv_sec = left->tv_sec - right->tv_sec;
    diff->tv_nsec = left->tv_nsec - right->tv_nsec;

    if (diff->tv_nsec < 0)
    {
        --diff->tv_sec;
        diff->tv_nsec += 1000000000;
    }
}


/* We need one of the list functions in the other modules.  */
static inline void list_unlink_ip (struct list_links *list)
{
    struct list_links *lnext = list->next, *lprev = list->prev;

    lnext->prev = lprev;
    lprev->next = lnext;

    /* The suffix ip means idempotent; list_unlink_ip can be called
     * two or more times on the same node.
     */

    list->next = list;
    list->prev = list;
}

/* Functions build on top of the list functions.  */
static inline struct thread_node *thread_links2ptr (struct list_links *list)
{
    return (struct thread_node *) ((char *) list - offsetof (struct thread_node, links));
}

static inline struct timer_node *timer_links2ptr (struct list_links *list)
{
    return (struct timer_node *) ((char *) list - offsetof (struct timer_node, links));
}


/* Initialize a newly allocated thread structure.  */
static void thread_init (struct thread_node *thread, const pthread_attr_t *attr, clockid_t clock_id)
{
    if (attr != NULL)
        thread->attr = *attr;
    else
    {
        pthread_attr_init (&thread->attr);
        pthread_attr_setdetachstate (&thread->attr, PTHREAD_CREATE_DETACHED);
    }

    thread->exists = 0;
    list_init (&thread->timer_queue);
    pthread_cond_init (&thread->cond, 0);
    thread->current_timer = 0;
    thread->captured = pthread_self ();
    thread->clock_id = clock_id;
}


/* Initialize the global lists, and acquire global resources.  Error
   reporting is done by storing a non-zero value to the global variable
   timer_init_failed.  */
static void init_module (void)
{
    int i;

    list_init (&timer_free_list);
    list_init (&thread_free_list);
    list_init (&thread_active_list);

    for (i = 0; i < TIMER_MAX; ++i)
    {
        list_append (&timer_free_list, &__timer_array[i].links);
        __timer_array[i].inuse = TIMER_FREE;
    }

    for (i = 0; i < THREAD_MAXNODES; ++i)
        list_append (&thread_free_list, &thread_array[i].links);

    thread_init (&__timer_signal_thread_rclk, 0, CLOCK_REALTIME);
}


/* This is a handler executed in a child process after a fork()
   occurs.  It reinitializes the module, resetting all of the data
   structures to their initial state.  The mutex is initialized in
   case it was locked in the parent process.  */
static void reinit_after_fork (void)
{
    init_module ();
    pthread_mutex_init (&__timer_mutex, 0);
}


/* Called once form pthread_once in timer_init. This initializes the
   module and ensures that reinit_after_fork will be executed in any
   child process.  */
static void __timer_init_once (void)
{
    init_module ();
    pthread_atfork (0, 0, reinit_after_fork);
}


/* Deinitialize a thread that is about to be deallocated.  */
static void thread_deinit (struct thread_node *thread)
{
    assert (list_isempty (&thread->timer_queue));
    pthread_cond_destroy (&thread->cond);
}


/* Allocate a thread structure from the global free list.  Global
   mutex lock must be held by caller.  The thread is moved to
   the active list. */
static struct thread_node *__timer_thread_alloc (const pthread_attr_t *desired_attr, clockid_t clock_id)
{
    struct list_links *node = list_first (&thread_free_list);

    if (node != list_null (&thread_free_list))
    {
        struct thread_node *thread = thread_links2ptr (node);
        list_unlink (node);
        thread_init (thread, desired_attr, clock_id);
        list_append (&thread_active_list, node);
        return thread;
    }

    return 0;
}


/* Return a thread structure to the global free list.  Global lock
   must be held by caller.  */
static void __timer_thread_dealloc (struct thread_node *thread)
{
    thread_deinit (thread);
    list_unlink (&thread->links);
    list_append (&thread_free_list, &thread->links);
}


/* Each of our threads which terminates executes this cleanup
   handler. We never terminate threads ourselves; if a thread gets here
   it means that the evil application has killed it.  If the thread has
   timers, these require servicing and so we must hire a replacement
   thread right away.  We must also unblock another thread that may
   have been waiting for this thread to finish servicing a timer (see
   timer_delete()).  */
static int __timer_thread_start(struct thread_node * thread);

static void thread_cleanup (void *val)
{
    if (val != NULL)
    {
        struct thread_node *thread = val;

        /* How did the signal thread get killed?  */
        assert (thread != &__timer_signal_thread_rclk);

        pthread_mutex_lock (&__timer_mutex);

        thread->exists = 0;

        /* We are no longer processing a timer event.  */
        thread->current_timer = 0;

        if (list_isempty (&thread->timer_queue))
            __timer_thread_dealloc (thread);
        else
            (void) __timer_thread_start (thread);
 
        pthread_mutex_unlock (&__timer_mutex);

        /* Unblock potentially blocked timer_delete().  */
        pthread_cond_broadcast (&thread->cond);
    }
}

/* Handle a timer which is supposed to go off now.  */
static void thread_expire_timer(struct thread_node * self, struct timer_node * timer)
{
    self->current_timer = timer; /* Lets timer_delete know timer is running. */

    pthread_mutex_unlock(&__timer_mutex);

    if (SIGEV_THREAD == timer->event.sigev_notify) 
    {
        ((void (*)(void *)) timer->event.sigev_notify_function)((void *) timer->event.sigev_value.sival_ptr);
    }

    /* Notice that the timer may be has been deleted in the timer expire handler,
       so DO NOT dereference the timer pointer anymore. */

    pthread_mutex_lock(&__timer_mutex);

    self->current_timer = 0;

    pthread_cond_broadcast(&self->cond);
}

/* Enqueue a timer in wakeup order in the thread's timer queue.
   Returns 1 if the timer was inserted at the head of the queue,
   causing the queue's next wakeup time to change. */
static int __timer_thread_queue_timer (struct thread_node *thread, struct timer_node *insert)
{
    struct list_links *iter;
    int athead = 1;

    for (iter = list_first (&thread->timer_queue); iter != list_null (&thread->timer_queue); iter = list_next (iter))
    {
        struct timer_node *timer = timer_links2ptr (iter);

        if (timespec_compare (&insert->expirytime, &timer->expirytime) < 0)
            break;
        athead = 0;
    }

    list_insbefore (iter, &insert->links);
    return athead;
}

/* Thread function; executed by each timer thread. The job of this
   function is to wait on the thread's timer queue and expire the
   timers in chronological order as close to their scheduled time as
   possible.  */
static void * thread_func(void * arg)
{
    int result = 0;
    int policy = 0;
    struct thread_node * self = arg;
    struct list_links * first;
    struct timer_node * timer;
    struct sched_param sched = {.sched_priority = 0};
    struct timespec now, abstime;

    pthread_attr_getschedpolicy(&self->attr, &policy);
    pthread_attr_getschedparam(&self->attr, &sched);
    sched_setscheduler(0, policy, &sched);
    prctl(PR_SET_NAME, (unsigned long) "tk_timer");

    /* Register cleanup handler, in case rogue application terminates
       this thread.  (This cannot happen to __timer_signal_thread, which
       doesn't invoke application callbacks). */

    pthread_cleanup_push(thread_cleanup, self);

    result = pthread_mutex_lock(&__timer_mutex);
    if(result)
    {
        result = -ERR_SYS;
        fprintf(stderr, "pthread_mutex_lock() error: %s\r\n", strerror(errno));
        goto ERR_EXIT;
    }

    for (;;)
    {
        timer = NULL;

        /* While the timer queue is not empty, inspect the first node.  */
        first = list_first(&self->timer_queue);
        if (first != list_null(&self->timer_queue))
        {
            timer = timer_links2ptr(first);

            /* This assumes that the elements of the list of one thread
               are all for the same clock.  */
            clock_gettime(timer->clock, &now);

            for (;;)
            {
                /* If the timer is due or overdue, remove it from the queue.
                   If it's a periodic timer, re-compute its new time and
                   requeue it.  Either way, perform the timer expiry. */
                if (timespec_compare(&now, &timer->expirytime) < 0)
                {
                    break;
                }

                list_unlink_ip(first);

                if ((0 != timer->value.it_interval.tv_sec) || (0 != timer->value.it_interval.tv_nsec)) 
                {
                    timer->overrun_count = 0;
                    timespec_add(&timer->expirytime, &timer->expirytime, &timer->value.it_interval);
                    while (timespec_compare(&timer->expirytime, &now) < 0)
					{
                        timespec_add(&timer->expirytime, &timer->expirytime, &timer->value.it_interval);
                        if (timer->overrun_count < DELAYTIMER_MAX) 
                        {
                            ++timer->overrun_count;
                        }
                    }
                    __timer_thread_queue_timer(self, timer);
                }

                thread_expire_timer(self, timer);

                /* DO NOT dereference the timer pointer anymore, because it
                   may be has been deleted in the timer expire handler. */
                timer = NULL;

                first = list_first(&self->timer_queue);
                if (first == list_null(&self->timer_queue))
                {
                    break;
                }

                timer = timer_links2ptr(first);
            }
        }

        /* If the queue is not empty, wait until the expiry time of the
           first node.  Otherwise wait indefinitely.  Insertions at the
           head of the queue must wake up the thread by broadcasting
           this condition variable.  */
        if (timer != NULL)
        {
            if (CLOCK_MONOTONIC == timer->clock) 
            {
                clock_gettime(CLOCK_MONOTONIC, &now);
                clock_gettime(CLOCK_REALTIME, &abstime);
                timespec_sub(&now, &timer->expirytime, &now);
                timespec_add(&abstime, &abstime, &now);
                pthread_cond_timedwait(&self->cond, &__timer_mutex, &abstime);
            } else {
                pthread_cond_timedwait(&self->cond, &__timer_mutex, &timer->expirytime);
            }
        } else {
            pthread_cond_wait(&self->cond, &__timer_mutex);
        }
    }

    pthread_mutex_unlock(&__timer_mutex);
    pthread_cleanup_pop(1);

ERR_EXIT:

    return NULL;
}

/* Start a thread and associate it with the given thread node.  Global
   lock must be held by caller.  */
static int __timer_thread_start (struct thread_node *thread)
{
    int retval = 1;

    assert (!thread->exists);
    thread->exists = 1;

    retval = pthread_create(&thread->id, &thread->attr, thread_func, thread);
    if(0 != retval)
    {
        thread->exists = 0;
        fprintf(stderr, "pthread_create() error:%s\r\n", strerror(retval));
		retval = -ERR_SYS;
    }

    return retval;
}

static void __timer_thread_wakeup (struct thread_node *thread)
{
    pthread_cond_broadcast (&thread->cond);
}


/* Compare two pthread_attr_t thread attributes for exact equality.
   Returns 1 if they are equal, otherwise zero if they are not equal
   or contain illegal values.  This version is NPTL-specific for
   performance reason.  One could use the access functions to get the
   values of all the fields of the attribute structure.  */
static int thread_attr_compare (const pthread_attr_t *left, const pthread_attr_t *right)
{
    int lpolicy = 0, rpolicy = 0;
    struct sched_param lsched = {.sched_priority = 0}, rsched = {.sched_priority = 0};

    (void) pthread_attr_getschedpolicy(left, &lpolicy);
    (void) pthread_attr_getschedpolicy(right, &rpolicy);

    (void) pthread_attr_getschedparam(left, &lsched);
    (void) pthread_attr_getschedparam(right, &rsched);

    if ((lpolicy == rpolicy) && (lsched.sched_priority == rsched.sched_priority)) 
    {
        return 1;
    }

    return 0;
}


/* Search the list of active threads and find one which has matching
   attributes.  Global mutex lock must be held by caller.  */
static struct thread_node *__timer_thread_find_matching (const pthread_attr_t *desired_attr, clockid_t desired_clock_id)
{
    struct list_links *iter = list_first (&thread_active_list);

    while (iter != list_null (&thread_active_list))
    {
        struct thread_node *candidate = thread_links2ptr (iter);

        if (thread_attr_compare (desired_attr, &candidate->attr) && desired_clock_id == candidate->clock_id)
            return candidate;

        iter = list_next (iter);
    }

    return NULL;
}


/* Grab a free timer structure from the global free list.  The global
   lock must be held by the caller.  */
static struct timer_node * __timer_alloc (void)
{
    struct list_links *node = list_first (&timer_free_list);

    if (node != list_null (&timer_free_list))
    {
        struct timer_node *timer = timer_links2ptr (node);
        list_unlink_ip (node);
        timer->inuse = TIMER_INUSE;
        timer->refcount = 1;
        return timer;
    }

    return NULL;
}


/* Return a timer structure to the global free list.  The global lock
   must be held by the caller.  */
static void __timer_dealloc (struct timer_node *timer)
{
    assert (timer->refcount == 0);
    list_unlink_ip (&timer->links);
    timer->thread = NULL;    /* Break association between timer and thread.  */
    timer->inuse = TIMER_FREE;
    list_append (&timer_free_list, &timer->links);
}


/* Thread cancellation handler which unlocks a mutex.  */
static void __timer_mutex_cancel_handler (void *arg)
{
    pthread_mutex_unlock (arg);
}

/* Check whether timer is valid; global mutex must be held. */
static inline int timer_valid (struct timer_node *timer)
{
    return timer && timer->inuse == TIMER_INUSE;
}

static inline void timer_addref (struct timer_node *timer)
{
    timer->refcount++;
}

static inline void timer_delref (struct timer_node *timer)
{
    if (--timer->refcount == 0)
        __timer_dealloc (timer);
}

/* Create new per-process timer using CLOCK.  */
static int __timer_create (clockid_t clock_id,struct sigevent *evp,timer_t *timerid)
{
    int retval = 0;
    struct timer_node *newtimer = NULL;
    struct thread_node *thread = NULL;

    if (0
#if defined _POSIX_CPUTIME && _POSIX_CPUTIME >= 0
       || clock_id == CLOCK_PROCESS_CPUTIME_ID
#endif
#if defined _POSIX_THREAD_CPUTIME && _POSIX_THREAD_CPUTIME >= 0
       || clock_id == CLOCK_THREAD_CPUTIME_ID
#endif
      )
    {
        /* We don't allow timers for CPU clocks.  At least not in the moment.  */
        __set_errno (ENOTSUP);
	    retval = -ERR_INVAL;
        goto ERR_EXIT;
    }

    retval = pthread_once(&__timer_init_once_control, __timer_init_once);
    if (__timer_init_failed)
    {
        __set_errno (ENOMEM);
        retval = -ERR_SYS;
		goto ERR_EXIT;
    }

    retval = pthread_mutex_lock (&__timer_mutex);
    if(retval)
    {
        retval = -ERR_SYS;
		fprintf(stderr, "pthread_mutex_lock() error:%s\r\n", strerror(errno));
		goto ERR_EXIT;
    }

    newtimer = __timer_alloc ();
    if (__builtin_expect (newtimer == NULL, 0))
    {
        __set_errno (EAGAIN);
        retval = -ERR_NOMEM;
        fprintf(stderr, "__timer_alloc() error\r\n");
        goto unlock_bail;
    }

    if (evp != NULL)
    {
        newtimer->event = *evp;
    }else{
        newtimer->event.sigev_notify = SIGEV_SIGNAL;
        newtimer->event.sigev_signo = SIGALRM;
        newtimer->event.sigev_value.sival_ptr = (void *) timer_ptr2id (newtimer);
        newtimer->event.sigev_notify_function = 0;
    }

    newtimer->event.sigev_notify_attributes = &newtimer->attr;
    newtimer->creator_pid = getpid ();

    switch (__builtin_expect (newtimer->event.sigev_notify, SIGEV_SIGNAL))
    {
    case SIGEV_NONE:
    case SIGEV_SIGNAL:
      /*We have a global thread for delivering timed signals.If it is not running, try to start it up.  */
        thread = &__timer_signal_thread_rclk;
        if (! thread->exists)
        {
            retval = __timer_thread_start (thread);
            if (__builtin_expect (retval, 1) < 0)
            {
                __set_errno (EAGAIN);
                goto unlock_bail;
            }
        }
        break;

    case SIGEV_THREAD:
        /* Copy over thread attributes or set up default ones.  */
        if (evp->sigev_notify_attributes)
            newtimer->attr = *(pthread_attr_t *) evp->sigev_notify_attributes;
        else
            pthread_attr_init (&newtimer->attr);

        /* Ensure thread attributes call for deatched thread.  */
        pthread_attr_setdetachstate (&newtimer->attr, PTHREAD_CREATE_DETACHED);

        /* Try to find existing thread having the right attributes.  */
        thread = __timer_thread_find_matching (&newtimer->attr, clock_id);

        /* If no existing thread has these attributes, try to allocate one.  */
        if (thread == NULL)
            thread = __timer_thread_alloc (&newtimer->attr, clock_id);

        /* Out of luck; no threads are available.  */
        if (__builtin_expect (thread == NULL, 0))
        {
            __set_errno (EAGAIN);
            retval = -ERR_NOMEM;
			fprintf(stderr, "__timer_thread_alloc() error\r\n");
            goto unlock_bail;
        }

        /* If the thread is not running already, try to start it.  */
		retval = __timer_thread_start(thread);
        if (!thread->exists && (retval < 0))
        {
            __set_errno (EAGAIN);
			fprintf(stderr, "__timer_thread_start() error\r\n");
            goto unlock_bail;
        }
        break;

default:
        __set_errno (EINVAL);
        retval = -ERR_INVAL;
        goto unlock_bail;
    }

    newtimer->clock = clock_id;
    newtimer->abstime = 0;
    newtimer->armed = 0;
    newtimer->thread = thread;

    *timerid = timer_ptr2id (newtimer);
    retval = 0;

    if (__builtin_expect (retval, 0) == -1)
    {
unlock_bail:
        if (thread != NULL)
            __timer_thread_dealloc (thread);
        if (newtimer != NULL)
        {
            timer_delref (newtimer);
            __timer_dealloc (newtimer);
        }
    }

    retval = pthread_mutex_unlock (&__timer_mutex);
    if(retval)
    {
        retval = -ERR_SYS;
        fprintf(stderr, "pthread_mutex_unlock() error:%s\r\n", strerror(errno));
    }
ERR_EXIT:

	__timer_init_failed = 0;
    return retval;
}

static int __timer_delete(timer_t timerid)
{
    struct thread_node * thread;
    struct timer_node * timer;
    int retval = -1;

    retval = pthread_mutex_lock(&__timer_mutex);
    if(retval)
    {
        retval = -ERR_SYS;
		fprintf(stderr, "pthread_mutex_unlock() error:%s\r\n", strerror(errno));
		goto ERR_EXIT;
    }

    timer = timer_id2ptr (timerid);
    if (!timer_valid(timer))
    {
        /* Invalid timer ID or the timer is not in use. */
        __set_errno (EINVAL);
    }else{
        if (timer->armed && (NULL != timer->thread)) 
        {
            thread = timer->thread;

            /* If thread is cancelled while waiting for handler to terminate,
               the mutex is unlocked and timer_delete is aborted. */
            pthread_cleanup_push(__timer_mutex_cancel_handler, &__timer_mutex);

            /* If timer is currently being serviced, wait for it to finish. */
            while ((thread->current_timer == timer) && (thread->id != pthread_self()))
            {
                pthread_cond_wait(&thread->cond, &__timer_mutex);
            }

            pthread_cleanup_pop(0);
        }

        /* Remove timer from whatever queue it may be on and deallocate it. */
        timer->inuse = TIMER_DELETED;
        list_unlink_ip(&timer->links);
        timer_delref(timer);

        retval = 0;
    }

    retval = pthread_mutex_unlock(&__timer_mutex);
    if(retval)
    {
        retval = -ERR_SYS;
		fprintf(stderr, "pthread_mutex_unlock() error:%s\r\n", strerror(errno));
		goto ERR_EXIT;
    }

ERR_EXIT:

    return retval;
}

#if (0)
/* Get expiration overrun for timer TIMERID.  */
static int __timer_getoverrun (timerid)
{
    struct timer_node *timer;
    int retval = -1;

    pthread_mutex_lock (&__timer_mutex);

    if (! timer_valid (timer = timer_id2ptr (timerid)))
        __set_errno (EINVAL);
    else
        retval = timer->overrun_count;

    pthread_mutex_unlock (&__timer_mutex);

    return retval;
}

/* Get current value of timer TIMERID and store it in VLAUE.  */
static int __timer_gettime (timerid, value)
     timer_t timerid;
     struct itimerspec *value;
{
    struct timer_node *timer;
    struct timespec now, expiry;
    int retval = -1, armed = 0, valid;
    clock_t clock = 0;

    pthread_mutex_lock (&__timer_mutex);

    timer = timer_id2ptr (timerid);
    valid = timer_valid (timer);

    if (valid) 
    {
        armed = timer->armed;
        expiry = timer->expirytime;
        clock = timer->clock;
        value->it_interval = timer->value.it_interval;
    }

    pthread_mutex_unlock (&__timer_mutex);

    if (valid)
    {
        if (armed)
        {
            clock_gettime (clock, &now);
            if (timespec_compare (&now, &expiry) < 0)
            {
                timespec_sub (&value->it_value, &expiry, &now);
            }else{
                value->it_value.tv_sec = 0;
                value->it_value.tv_nsec = 0;
            }
        }else{
            value->it_value.tv_sec = 0;
            value->it_value.tv_nsec = 0;
        }

        retval = 0;
    }else{
        __set_errno (EINVAL);
    }
    return retval;
}
#endif


static int __timer_settime (timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *ovalue)
{
    struct timer_node *timer;
    struct thread_node *thread = NULL;
    struct timespec now;
    int have_now = 0, need_wakeup = 0;
    int retval = 0;

    if(NULL == value)
    {
        retval = -ERR_SYS;
        goto ERR_EXIT;
    }

    timer = timer_id2ptr (timerid);
    if (timer == NULL)
    {
        __set_errno (EINVAL);
	    retval = -ERR_SYS;
        goto bail;
    }

    if (value->it_interval.tv_nsec < 0
      || value->it_interval.tv_nsec >= 1000000000
      || value->it_value.tv_nsec < 0
      || value->it_value.tv_nsec >= 1000000000)
    {
        __set_errno (EINVAL);
	    retval = -ERR_SYS;
        goto bail;
    }

    /* Will need to know current time since this is a relative timer;
     might as well make the system call outside of the lock now! */

    if ((flags & TIMER_ABSTIME) == 0)
    {
        retval = clock_gettime (timer->clock, &now);
        if(retval)
        {
            retval = -ERR_SYS;
			fprintf(stderr, "clock_gettime() error:%s\r\n", strerror(errno));
            goto bail;
        }
        have_now = 1;
    }

    retval = pthread_mutex_lock (&__timer_mutex);
	if(retval)
    {
        fprintf(stderr, "pthread_mutex_lock() error:%s\r\n", strerror(errno));
        retval = -ERR_SYS;
	    goto bail;
    }
    timer_addref (timer);

    /* One final check of timer validity; this one is possible only
     until we have the mutex, because it accesses the inuse flag. */

	retval = timer_valid(timer);
    if (!retval )
    {
        __set_errno (EINVAL);
	    retval = -ERR_INVAL;
        goto unlock_bail;
    }

    if (ovalue != NULL)
    {
        ovalue->it_interval = timer->value.it_interval;

        if (timer->armed)
        {
            if (!have_now)
            {
                retval = pthread_mutex_unlock (&__timer_mutex);
                if(retval)
                {
                    fprintf(stderr, "pthread_mutex_unlock() error:%s\r\n", strerror(errno));
                    retval = -ERR_SYS;
                    goto bail;
                }

                retval = clock_gettime (timer->clock, &now);
                if(retval)
                {
                    fprintf(stderr, "clock_gettime() error:%s\r\n", strerror(errno));
                    retval = -ERR_SYS;
                    goto bail;
                }

                have_now = 1;
                retval = pthread_mutex_lock (&__timer_mutex);
                if(retval)
                {
                    fprintf(stderr, "pthread_mutex_lock() error:%s\r\n", strerror(errno));
                    retval = -ERR_SYS;
                    goto bail;
                }
				
                timer_addref (timer);
            }

            timespec_sub (&ovalue->it_value, &timer->expirytime, &now);
        }else{
            ovalue->it_value.tv_sec = 0;
            ovalue->it_value.tv_nsec = 0;
        }
    }

    timer->value = *value;

    list_unlink_ip (&timer->links);
    timer->armed = 0;

    thread = timer->thread;

    /* A value of { 0, 0 } causes the timer to be stopped. */
    if ((value->it_value.tv_sec != 0) || __builtin_expect (value->it_value.tv_nsec != 0, 1))
    {
        if ((flags & TIMER_ABSTIME) != 0)
        {
            /* The user specified the expiration time.  */
            timer->expirytime = value->it_value;
        }else{
            timespec_add (&timer->expirytime, &now, &value->it_value);
        }
        /* Only need to wake up the thread if timer is inserted at the head of the queue. */
        if (thread != NULL)
        {
             need_wakeup = __timer_thread_queue_timer (thread, timer);
        }
        timer->armed = 1;
    }

    retval = 0;

unlock_bail:
    timer_delref (timer);

    retval = pthread_mutex_unlock (&__timer_mutex);
	if(retval)
	{
		fprintf(stderr, "pthread_mutex_unlock() error:%s\r\n", strerror(errno));
		retval = -ERR_SYS;
	}

bail:
    if (thread != NULL && need_wakeup)
       __timer_thread_wakeup (thread);

ERR_EXIT:

    return retval;
}

/******************************************************************************
*     Function: os_timer_add
*     Descriptions: add the time.
*     Paramters:
            prio               - the priority of the timer
                                 the different priority of the timer was serviced by the different process
                                 the same priority timer was serviced by the same process
                                 the max support timer with different priority is 16
                                 the max support timer is 256
            millisec           - the time of timeout (uint:ms)
            flag               - the flag of the timer
                                 TIMER_ONESHOT     :the timer is once ack timer when we created
                                 TIMER_INTERVAL    :the timer is period ack timer when we created
                                 TIMER_STOPPED     :the timer is stoped when we created
            handler            - the handler function of the timer
            arg                - the arguments of the handler fucntion
            timerid            - the handle id of timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
      1. the timer would not be deleted after it was timeout.
      2. the handler function of the timer can be delete by itself.
      3. when we fail to create the timer, the memory info of the timer will not be modified.
      4. the timer id can modify by os_timer_add or os_timer_del function.
      5. after we use os_timer_del function, we should set the time id zero.
      6. before we create the timer, we had better check the timerid exist or not.
*     Example:
      1. ceate one timer with period 1 second:
        timer_t timerid;
        rc_timer_add(TIMER_PRIO_NORMAL, 1000, 0, handler, NULL, &timerid);
      2. ceate one timer with period 1 second and stop after it was created:
      timer_t timerid;
      rc_timer_add(TIMER_PRIO_NORMAL, 1000, TIMER_STOPPED, handler, NULL, &timerid);
      3. create one timer with period 1 second and ack one time
      timer_t timerid;
      rc_timer_add(TIMER_PRIO_NORMAL, 1000, TIMER_ONESHOT, handler, NULL, &timerid);
      4. create one timer with one shot and stop status
      timer_t timerid;
      rc_timer_add(TIMER_PRIO_NORMAL, 1000, TIMER_ONESHOT | TIMER_STOPPED, handler, NULL, &timerid);
******************************************************************************/
int os_timer_add(int prio, int millisec, int flag, void (*handler)(void *), void * arg, timer_t * timerid)
{
    int result = 0;
    timer_t tid = 0;
    struct timer_node * timer;
    struct sigevent sev;
    struct itimerspec its;
    pthread_attr_t attr;
    struct sched_param sched;

    if ((millisec <= 0) || (NULL == handler) || (NULL == timerid))
    {
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    result = pthread_attr_init(&attr);
    if(result)
    {
		(void) fprintf(stderr, "pthread_attr_init(): %s\r\n", strerror(result));
		result = -ERR_SYS;
        goto ERR_EXIT;
    }

    result = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if(result)
    {
		(void) fprintf(stderr, "pthread_attr_init(): %s\r\n", strerror(result));
		result = -ERR_SYS;
        goto ERR_EXIT;
    }

    sched.sched_priority = prio;
    result = pthread_attr_setschedparam(&attr, &sched);
    if(result)
    {
		(void) fprintf(stderr, "pthread_attr_init(): %s\r\n", strerror(result));
		result = -ERR_SYS;
        goto ERR_EXIT;
    }

    (void) memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = (void (*)(sigval_t)) handler;
    sev.sigev_notify_attributes = &attr;
    sev.sigev_value.sival_ptr = arg;

    result = __timer_create(CLOCK_MONOTONIC, &sev, &tid);
    if (0 != result)
    {
        goto ERR_EXIT;
    }

    result = pthread_mutex_lock(&__timer_mutex);
	if(result)
    {
		(void) fprintf(stderr, "pthread_mutex_lock(): %s\r\n", strerror(errno));
		result = -ERR_SYS;
        goto ERR_EXIT;
    }
    timer = timer_id2ptr(tid);
	
	result = timer_valid(timer);
    if (!result) 
    {
        int ret = 0;
        ret = pthread_mutex_unlock(&__timer_mutex);
        if(ret)
        {
		    (void) fprintf(stderr, "pthread_mutex_unlock(): %s\r\n", strerror(errno));
		    result = -ERR_SYS;
            goto ERR_EXIT;
        }
		goto ERR_EXIT;
    }
	
    timer->type = (flag & TIMER_ONESHOT) ? TIMER_ONESHOT : TIMER_INTERVAL;
    timer->millisec = millisec;
    result = pthread_mutex_unlock(&__timer_mutex);
    if(result)
    {
		(void) fprintf(stderr, "pthread_mutex_lock(): %s\r\n", strerror(errno));
		result = -ERR_SYS;
        goto ERR_EXIT;
    }
	
    if (flag & TIMER_STOPPED) {
        its.it_value.tv_sec = 0;
        its.it_value.tv_nsec = 0;
    } else {
        ms2ts(millisec, &its.it_value);
    }

    if (flag & TIMER_ONESHOT) {
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
    } else {
        its.it_interval.tv_sec = its.it_value.tv_sec;
        its.it_interval.tv_nsec = its.it_value.tv_nsec;
    }

    result = __timer_settime(tid, flag, &its, NULL);
    if (0 != result) 
    {
        goto ERR_EXIT;
    }

    *timerid = tid;

ERR_EXIT:

    return result;
}

/******************************************************************************
*     Function: os_timer_del
*     Descriptions: delete the time.
*     Paramters:
            timerid            - the handle of the timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     Before we delete the timer, we must set the timerid 0
******************************************************************************/
int os_timer_del(timer_t timerid)
{
    return __timer_delete(timerid);
}

/******************************************************************************
*     Function: os_timer_start
*     Descriptions: start the time.
*     Paramters:
            timerid            - the handle of the timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     if the timer was started, then it will reset the timer,like os_timer_reset
     if the timer was timeout, then it will call the function and restart the time count.
******************************************************************************/
int os_timer_start(timer_t timerid)
{
    int result = 0;
    struct timer_node * timer;
    struct itimerspec its;

    result = pthread_mutex_lock(&__timer_mutex);
    if(result)
    {
        result = -ERR_SYS;
	    (void) fprintf(stderr, "pthread_mutex_lock(): %s\r\n", strerror(errno));
        goto ERR_EXIT;
    }
	
    timer = timer_id2ptr(timerid);

    result = timer_valid(timer);
	if (!result) 
    {
        int ret = 0;
        ret = pthread_mutex_unlock(&__timer_mutex);
	    if(ret)
        {
            result = -ERR_SYS;
	        (void) fprintf(stderr, "pthread_mutex_unlock(): %s\r\n", strerror(errno));
            goto ERR_EXIT;
        }
        
	    goto ERR_EXIT;
    }

    ms2ts(timer->millisec, &its.it_value);
    if (TIMER_ONESHOT == timer->type) 
    {
        zero_timespec(&its.it_interval);
    } else {
        its.it_interval.tv_sec = its.it_value.tv_sec;
        its.it_interval.tv_nsec = its.it_value.tv_nsec;
    }

    result = pthread_mutex_unlock(&__timer_mutex);
    if(result)
    {
        result = -ERR_SYS;
	    (void) fprintf(stderr, "pthread_mutex_unlock(): %s\r\n", strerror(errno));
        goto ERR_EXIT;
    }
	
    result = __timer_settime(timerid, 0, &its, NULL);

ERR_EXIT:
	
    return result;
}


/******************************************************************************
*     Function: os_timer_stop
*     Descriptions: stop the time.
*     Paramters:
            timerid             - the handle of the timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     we can stop the timer many times at the same moment
******************************************************************************/
int os_timer_stop(timer_t timerid)
{
    int result = 0;
    struct itimerspec its;

    zero_itimerspec(&its);

    result = __timer_settime(timerid, 0, &its, NULL);

    return result;	
}

/******************************************************************************
*     Function: os_timer_reset
*     Descriptions: reset the time.
*     Paramters:
            timerid             - the handle of the timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     it has the same effect with the function of os_timer_start
******************************************************************************/
int os_timer_reset(timer_t timerid)
{
    return os_timer_start(timerid);
}

/******************************************************************************
*     Function: os_timer_settime
*     Descriptions: modify the time period of timer.
*     Paramters:
            timerid             - the handle of the timer
            millisec           - the msec time of the time
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     Before we modify the timer, we had better used os_timer_stop stop the timer;
     After we modify the timer, we can take it effect after os_timer_start
     Example: 
      int ret = 0;
      ret |= os_timer_stop(timerid);
      ret |= os_timer_settime(timerid, 2000);
      ret |= os_timer_start(timerid);
******************************************************************************/
int os_timer_settime(timer_t timerid, int millisec)
{
    int result = 0;
    struct timer_node * timer = NULL;

    if (millisec <= 0) 
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    result = pthread_mutex_lock(&__timer_mutex);
    if(result)
    {
        result = -ERR_SYS;
        (void) fprintf(stderr, "pthread_mutex_lock(): %s\r\n", strerror(errno));
		goto ERR_EXIT;
    }

    timer = timer_id2ptr(timerid);

    result = timer_valid(timer);
    if (!result) 
    {
        int ret = 0;
        ret = pthread_mutex_unlock(&__timer_mutex);
	    if(ret)
        {
            result = -ERR_SYS;
            (void) fprintf(stderr, "pthread_mutex_lock(): %s\r\n", strerror(errno));
		    goto ERR_EXIT;
        }
        
        goto ERR_EXIT;
    }
    timer->millisec = millisec;

    result = pthread_mutex_unlock(&__timer_mutex);
    if(result)
    {
        result = -ERR_SYS;
        (void) fprintf(stderr, "pthread_mutex_lock(): %s\r\n", strerror(errno));
		goto ERR_EXIT;
    }
ERR_EXIT:

    return result;
}


