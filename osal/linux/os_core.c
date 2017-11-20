/*****************************************************************************
 copyright(c) beijing carsmart technology co., ltd.
 all rights reserved.
 
 @file   : os_core.c
 @brief  : os api 
 @author : wanglei
 @history:
           2015-7-30    wanglei    created file
           2017-10-23   pengrui    add the msg, sema, named sem and the comments
           ...
******************************************************************************/

#include "os_core.h"

/******************************************************************************
*    Function: os_mutex_create
*    Descriptions: open the mutex.
*    Paramters:
            mutex_ptr                  - the handle of the mutex.
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int os_mutex_create(pthread_mutex_t *mutex_ptr)
{
    int ret = 0;
    pthread_mutexattr_t mutex_attr ;

    if(mutex_ptr == NULL)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    ret = pthread_mutexattr_init(&mutex_attr);
    if (ret) 
    {
        fprintf(stderr, "pthread_mutexattr_init(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    /* dnot shared between processes */
    ret = pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_PRIVATE);
    if (ret) 
    {
        fprintf(stderr, "pthread_mutexattr_setpshared(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }
    // the mutex is normal mutex
    ret = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
    if (ret) 
    {
        fprintf(stderr, "pthread_mutexattr_setptype(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    ret = pthread_mutex_init(mutex_ptr, &mutex_attr);
    if (ret) 
    {
        fprintf(stderr, "pthread_mutex_init(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    return ret;
ERR_EXIT:
    
    pthread_mutexattr_destroy(&mutex_attr);  
    
    return ret;
}

/******************************************************************************
*    Function: os_mutex_lock
*    Descriptions: lock the mutex.
*    Paramters:
            mutex_ptr                - the handle of the mutex.
*    Return:
            = 0                      - success
            < 0                      - failed
*    Comments: 
******************************************************************************/
int  os_mutex_lock(pthread_mutex_t * mutex_ptr)
{
    int ret = 0;

    if(NULL == mutex_ptr)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    ret = pthread_mutex_lock(mutex_ptr);
    if (ret) 
    {
        fprintf(stderr, "pthread_mutex_lock(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

ERR_EXIT:

    return ret;
}

/******************************************************************************
*    Function: os_mutex_trylock
*    Descriptions: try lock the mutex.
*    Paramters:
            mutex_ptr                - the handle of the mutex.
*    Return:
            = 0                      - success
            < 0                      - failed
*    Comments: 
******************************************************************************/
int os_mutex_trylock(pthread_mutex_t * mutex_ptr)
{
    int ret = 0;
    
    if (mutex_ptr == NULL)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    ret = pthread_mutex_trylock(mutex_ptr);
    if (ret) 
    {
        fprintf(stderr, "pthread_mutex_trylock(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }
    
ERR_EXIT:

    return ret; 
}

/******************************************************************************
*    Function: os_mutex_unlock
*    Descriptions: unlock the mutex.
*    Paramters:
            mutex_ptr                - the handle of the mutex.
*    Return:
            = 0                      - success
            < 0                      - failed
*    Comments: 
******************************************************************************/
int os_mutex_unlock(pthread_mutex_t * mutex_ptr)
{
    int ret = 0;

    if (mutex_ptr == NULL)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    ret = pthread_mutex_unlock(mutex_ptr);
    if (0 != ret) 
    {
        fprintf(stderr, "pthread_mutex_unlock(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

ERR_EXIT:
    
    return ret;
}

/******************************************************************************
*    Function: os_mutex_destroy
*    Descriptions: close the mutex.
*    Paramters:
            mutex_ptr                - the handle of the mutex.
*    Return:
            = 0                      - success
            < 0                      - failed
*    Comments: 
******************************************************************************/
int os_mutex_destroy(pthread_mutex_t * mutex_ptr)
{
    int ret = 0;

    if (mutex_ptr == NULL)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    ret = pthread_mutex_destroy(mutex_ptr);
    if (ret) 
    {
        fprintf(stderr, "pthread_mutex_destroy(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
    }

ERR_EXIT:
    
    return ret;
}
#define SEM_NAME_LEN    20
/******************************************************************************
*    Function: os_sema_open
*    Descriptions: open and init the named posix semaphore.
*    Paramters:
            sem_ptr                   - the handle fd of semaphore(returned)
            name                      - the key name of semaphore 
            init_count                - the init count of semaphore
*    Return:
            = 0                       -    success
            < 0                       -    failed
*    Comments: 
******************************************************************************/
int os_sem_open(sem_t **sem_ptr, const char *name, int init_cnt)
{
    int result = 0;
	char sem_name[SEM_NAME_LEN] ={0};

    if(name == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

	snprintf(sem_name,SEM_NAME_LEN,"/%s",name);

    *sem_ptr = sem_open(sem_name, O_CREAT, S_IRWXU | S_IRWXG, init_cnt);

    if(SEM_FAILED == *sem_ptr)
    {
       (void) fprintf(stderr, "sem_open(): %s\r\n", strerror(errno));
       result = -ERR_SYS;
    }

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: os_sem_close
*    Descriptions: close the named posix semaphore.
*    Paramters:
            sem_ptr                - the handle fd of semaphore
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_sem_close(sem_t *sem_ptr)
{
    int result = 0;

    if(sem_ptr == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    result = sem_close(sem_ptr);
    if(result)
    {
        (void) fprintf(stderr, "sem_close(): %s\r\n", strerror(errno));
    }

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: os_sem_unlink
*    Descriptions: delete the named posix semaphore.
*    Paramters:
            name                  - the name of semaphore
*    Return:
            = 0                   - success
            < 0                   - failed
*    Comments: 
******************************************************************************/
int os_sem_unlink(const char *name)
{
    int result = 0;

    if(name == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    result= sem_unlink(name);

    if(result)
    {
        (void) fprintf(stderr, "sem_unlink(): %s\r\n", strerror(errno));
    }

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: os_sem_init
*    Descriptions: open and init the unnamed posix semaphore.
*    Paramters:
            sem_ptr            - the handle fd of semaphore(returned)
            pshared            - 0:the semaphore can used only in the thread 1:the semaphore can used int diff process
            init_cnt           - the init count of semaphore
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_sem_init(sem_t * sem_ptr, int pshared, int init_cnt)
{
    int result = 0;

    if(sem_ptr == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    result = sem_init(sem_ptr , pshared , init_cnt);
    if (0 != result) 
    {
        (void) fprintf(stderr, "sem_init(): %s\r\n", strerror(errno));
    }

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: os_sem_destroy
*    Descriptions: destory the unnamed posix semaphore.
*    Paramters:
            sem_ptr            - the handle fd of semaphore
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_sem_destroy(sem_t *sem_ptr)
{
    int result = 0;

    if(sem_ptr == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    result = sem_destroy(sem_ptr);
    if (0 != result) 
    {
        (void) fprintf(stderr, "sem_destroy(): %s\r\n", strerror(errno));
    }

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: os_sem_wait
*    Descriptions: read the posix semaphore.
*    Paramters:
            sem_ptr            - the handle fd of semaphore(returned)
            timeout            - 0: no wait 0xFFFF: wait forever other:wait timeout ms
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_sem_wait(sem_t* sem_ptr, uint32_t timeout)
{
    int result = 0;
    
    if(sem_ptr == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    if(timeout == OSAL_NOBLOCK)
    {
        result = sem_trywait(sem_ptr);
		if(result)
		{
		    (void) fprintf(stderr, "sem_trywait(): %s\r\n", strerror(errno));
		    result = -ERR_SYS;
		    goto ERR_EXIT;
		}
    }
    else if(timeout == OSAL_BLOCK)
    {
        result = sem_wait(sem_ptr);
		if(result)
		{
		    (void) fprintf(stderr, "sem_wait(): %s\r\n", strerror(errno));
		    result = -ERR_SYS;
		    goto ERR_EXIT;
		}

    }
    else
    {
        struct timeval current_time;
        struct timespec ts;

        gettimeofday(&current_time, 0);
        ts.tv_sec = current_time.tv_sec;
        ts.tv_sec += timeout/1000;
        if((timeout%1000)*1000 + current_time.tv_usec >= 1000000)
        {
            ts.tv_sec += 1;
            ts.tv_nsec = ((timeout%1000)*1000 + current_time.tv_usec)%1000000;
            ts.tv_nsec *= 1000;
        }
        else
        {
            ts.tv_nsec = (timeout%1000)*1000 + current_time.tv_usec;
            ts.tv_nsec *= 1000;
        }

        while ((result = sem_timedwait(sem_ptr, &ts)) == -1 && errno == EINTR)
        {
            continue;
        }

        if(result)
        {
            (void) fprintf(stderr, "sem_timedwait(): %s\r\n", strerror(errno));
		    result = -ERR_SYS;
		    goto ERR_EXIT;
        }
    }

ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    Function: os_sem_post
*    Descriptions: post the posix semaphore.
*    Paramters:
            sem_ptr            - the handle fd of semaphore(returned)
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int  os_sem_post(sem_t *sem_ptr)
{
    int ret = 0;
    
    if (sem_ptr == NULL) 
    {
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    ret = sem_post(sem_ptr);
    if (ret) 
    {
        (void) fprintf(stderr, "sem_post(): %s\r\n", strerror(errno));
        ret = -ERR_SYS;
    }

ERR_EXIT:
    
    return ret;
}

/******************************************************************************
*    Function: os_sema_open
*    Descriptions: open and init the systemV semaphore.
*    Paramters:
            p_semfd            - the handle fd of semaphore(returned)
            name               - the key name of semaphore 
            key_id             - the key id of semaphore
            init_count         - the init count of semaphore
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_sema_open(int *p_semfd, const char* name, int key_id, int init_count)
{
    int result = 0;
    key_t semkey;
    
    union semun {
        int              val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
    }semopts;

    if((p_semfd == NULL) || (name == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    semkey = ftok(name, key_id);
    if((key_t)-1 == semkey)
    {
        (void) fprintf(stderr, "ftok(): %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    result = semget(semkey, 1, 0666|IPC_CREAT);
    if(-1 == result)
    {
        (void) fprintf(stderr, "semget(): %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    *p_semfd = result;

    semopts.val = init_count;
    result = semctl(*p_semfd, 0, SETVAL, semopts);
    if(-1 == result)
    {
        (void) fprintf(stderr, "semctl(): %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    result = 0;
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: os_sema_lock
*    Descriptions: set systemV semaphore lock.
*    Paramters:
            semfd              - the handle fd of semaphore
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_sema_lock(int semfd)
{
    int result = 0;
    struct sembuf buf = {0, -1, 0};

    result = semop(semfd, &buf, 1);
    if(-1 == result)
    {
        (void) fprintf(stderr, "semop(): %s\r\n", strerror(errno));
        result = -ERR_SYS;
    }
    
    return result;
}

/******************************************************************************
*    Function: os_sema_unlock
*    Descriptions: set systemV semaphore unlock.
*    Paramters:
            semfd              - the handle fd of semaphore
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_sema_unlock(int semfd)
{
    int result = 0;
    struct sembuf buf = {0, 1, 0};

    result = semop(semfd, &buf, 1);
    if(-1 == result)
    {
        (void) fprintf(stderr, "semop(): %s\r\n", strerror(errno));
        result = -ERR_SYS;
    }

    return result;
}

/******************************************************************************
*    Function: os_sema_close
*    Descriptions: close systemV semaphore.
*    Paramters:
            semfd              - the handle fd of semaphore
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
int os_sema_close(int semfd)
{
    int result = 0;

    result = semctl(semfd, 0, IPC_RMID);
    if(-1 == result)
    {
        (void) fprintf(stderr, "semctl(): %s\r\n", strerror(errno));
        result = -ERR_SYS;
    }
    
    return result;
}

/******************************************************************************
*    Function: os_spinlock_init
*    Descriptions: init the spinlock.
*    Paramters:
            spinlock_ptr               - the handle of spinlock
            pshared                    - the type of the spinlock
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int os_spinlock_init(pthread_spinlock_t  *spinlock_ptr, int pshared)
{
    int ret = -1;
    
    if(NULL == spinlock_ptr)
    {
        ret = -ERR_INVAL;
		goto ERR_EXIT;
    }

    ret = pthread_spin_init(spinlock_ptr, pshared);
    if(ret)
    {
        (void) fprintf(stderr, "pthread_spin_init(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
    }

ERR_EXIT:

    return ret;
}

/******************************************************************************
*    Function: os_spinlock_lock
*    Descriptions: lock the spinlock.
*    Paramters:
            spinlock_ptr               - the handle of spinlock
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int os_spinlock_lock(pthread_spinlock_t *spinlock_ptr)
{
    int ret = 0;

    if (NULL == spinlock_ptr)
    {
        ret = -ERR_INVAL;
		goto ERR_EXIT;
    }
	
    ret = pthread_spin_lock(spinlock_ptr);
	if(ret)
    {
        (void) fprintf(stderr, "pthread_spin_lock(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
		goto ERR_EXIT;
    }

ERR_EXIT:

    return ret;
}

/******************************************************************************
*    Function: os_spinlock_trylock
*    Descriptions: try lock the spinlock.
*    Paramters:
            spinlock_ptr               - the handle of spinlock
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int os_spinlock_trylock(pthread_spinlock_t *spinlock_ptr)
{
    int ret = 0;
	
    if (NULL == spinlock_ptr)
    {
        ret = -ERR_INVAL;
		goto ERR_EXIT;
    }

	ret = pthread_spin_trylock(spinlock_ptr);
	if(ret)
    {
        (void) fprintf(stderr, "pthread_spin_trylock(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
		goto ERR_EXIT;
    }

ERR_EXIT:
	
    return ret;
}

/******************************************************************************
*    Function: os_spinlock_unlock
*    Descriptions: unlock the spinlock.
*    Paramters:
            spinlock_ptr               - the handle of spinlock
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int os_spinlock_unlock(pthread_spinlock_t *spinlock_ptr)
{
    int ret = 0;

    if (NULL == spinlock_ptr)
	{
        ret = -ERR_INVAL;
        goto ERR_EXIT;
	}

    ret = pthread_spin_unlock(spinlock_ptr);
    if(ret)
	{
        (void) fprintf(stderr, "pthread_spin_unlock(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

ERR_EXIT:

    return ret;

}

/******************************************************************************
*    Function: os_spinlock_destroy
*    Descriptions: close the spinlock.
*    Paramters:
            spinlock_ptr               - the handle fd of spinlock
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int os_spinlock_destroy(pthread_spinlock_t *spinlock_ptr)
{
    int ret = 0;

    if (NULL == spinlock_ptr)
    {
        ret = -ERR_INVAL;
	    goto ERR_EXIT;
	}
	
    ret = pthread_spin_destroy(spinlock_ptr);
    if(ret)
	{
        (void) fprintf(stderr, "pthread_spin_destroy(): %s\r\n", strerror(ret));
        ret = -ERR_SYS;
        goto ERR_EXIT;
	}

ERR_EXIT:

    return ret;
}

/******************************************************************************
*    Function: os_tick_get
*    Descriptions: get the system tick.
*    Paramters:
            void               - 
*    Return:
            > 0                - the ticks of the systime
            < 0                - failed
*    Comments: 
******************************************************************************/
uint64_t os_tick_get(void)
{
    clock_t ticks = 0;
    
    ticks = times(NULL);
    
    return (unsigned long long) ticks;
}

/******************************************************************************
*    Function: os_get_time
*    Descriptions: get the time of systime .
*    Paramters:
            t                  - the sec and usec of the systime
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_get_time(struct os_time *t)
{
    int ret = 0;
    struct timeval tv;

    ret = gettimeofday(&tv, NULL);
    if(ret)
    {
        ret = -ERR_SYS;
		(void) fprintf(stderr, "gettimeofday(): %s\r\n", strerror(errno));
		goto ERR_EXIT;
    }
	
    t->sec = tv.tv_sec;
    t->usec = tv.tv_usec;

ERR_EXIT:

    return ret;
}

/******************************************************************************
*    Function: os_get_reltime
*    Descriptions: get the real time.
*    Paramters:
            t                  - the time we get from system(return)
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_get_reltime(struct os_reltime *t)
{
    int ret = 0;
    struct timespec ts = { 0 };

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    if (ret == 0) 
    {
        t->sec = ts.tv_sec;
        t->usec = ts.tv_nsec / 1000;
        return 0;
    }else{
        ret = -ERR_SYS;
	    (void) fprintf(stderr, "clock_gettime(): %s\r\n", strerror(errno));
        return ret;
    }
}

/******************************************************************************
*    Function: os_set_reltime
*    Descriptions: set the systime.
*    Paramters:
            t                  - the data of the time set
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_set_reltime(struct os_reltime *t)
{
    int result = 0;
    struct timespec ts = { 0 };


    ts.tv_sec = t->sec;
    ts.tv_nsec= t->usec * 1000ul;
    
    result = clock_settime(CLOCK_REALTIME, &ts);
    if(result)
    {
        result = -ERR_SYS;
	    (void) fprintf(stderr, "clock_settime(): %s\r\n", strerror(errno));
    }
	
    return result;
}

/******************************************************************************
*    Function: usleep_ex
*    Descriptions: usleep usec.
*    Paramters:
            usecs                - the count the usleep(uint usec)
*    Return:
            void
*    Comments: 
******************************************************************************/
static void usleep_ex(unsigned usecs)
{
    struct timeval t;

    t.tv_sec = usecs/1000000;
    t.tv_usec = usecs%1000000;
    (void)select(1, NULL, NULL, NULL, &t);
}

/******************************************************************************
*    Function: os_delay
*    Descriptions: delay usec.
*    Paramters:
            usec                - the count of delay time(uint us)
*    Return:
            void
*    Comments: 
******************************************************************************/
void os_delay(int usec)
{
    usleep_ex(usec);

    return;
}

/******************************************************************************
*    Function: os_sleep
*    Descriptions: sleep msec ms.
*    Paramters:
            msec               - the time of sleep(uint ms)
*    Return:
            void
*    Comments: 
******************************************************************************/
void os_sleep(int msec)
{
    msec = (msec <=10)?1:(msec-10);
    usleep_ex(msec*1000);
    return;
}

/******************************************************************************
*    Function: os_current_thread
*    Descriptions: get the current thread pid.
*    Paramters:
            void               - 
*    Return:
            > 0                - current pid
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_current_thread(void)
{
    pid_t pid = 0;

    pid = syscall(SYS_gettid);

    return (int)pid;
}

#define MQ_NAME_LEN 32
/******************************************************************************
*    Function: os_queue_create
*    Descriptions: creat posix queue msg.
*    Paramters:
            queue_id               - the handle fd of posix queue(returned)
            queue_name             - the name of posix queue
            queue_depth            - Max. # of messages on queue
            data_size              - Max data size of each queue
            flags                  - 0:BLOCK 1:NOBLOCK
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_queue_create(mqd_t *queue_id, const char *queue_name, unsigned int queue_depth, 
                          unsigned int data_size, unsigned int flags
                          )
{
    int result = 0;
    struct mq_attr attrs;
    char  mq_name[MQ_NAME_LEN+1] = {0};
    mqd_t id = -1;

    if((queue_id == NULL) || (queue_name == NULL))
    {
		result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    snprintf(mq_name,MQ_NAME_LEN,"/%s",queue_name);
    
    attrs.mq_flags = flags;     
    attrs.mq_maxmsg = queue_depth; 
    attrs.mq_msgsize = data_size; 

    id = mq_open(mq_name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, &attrs);
    if(id == -1)
	{
        (void) fprintf(stderr, "mq_open(): %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;;
    }

    *queue_id = id;

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: os_queue_delete
*    Descriptions: delete posix queue msg.
*    Paramters:
            queue_id               - the handle fd of posix queue
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_queue_delete(mqd_t queue_id)
{
    int result = 0;

    result = mq_close(queue_id);
    if(result)
    {
        (void) fprintf(stderr, "mq_close(): %s\r\n", strerror(errno));
        result = -ERR_SYS;
    }

    return result;
}

/******************************************************************************
*    Function: os_queue_recv
*    Descriptions: recv posix queue msg.
*    Paramters:
            queue_id               - the handle fd of posix queue
            data                   - the data of recv from queue
            size                   - the size of recv data from queue
            size_copied            - the copied size from queue
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_queue_recv(mqd_t queue_id, void *data, uint32_t size, uint32_t *size_copied)
{
    int len = 0 ;
    int result = 0;

    if(data == NULL || size_copied == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    len = mq_receive(queue_id, data, size, NULL);
    if(len < 0)
    {
        (void) fprintf(stderr, "mq_receive(): %s\r\n", strerror(errno));
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    *size_copied = len;
    result = 0;

ERR_EXIT:

   return result;
}

/******************************************************************************
*    Function: os_queue_send
*    Descriptions: send msg to posix queue msg.
*    Paramters:
            queue_id               - the handle fd of posix queue 
            data                   - the data send to posix queue
            size                   - the size of the data send to queue
            priority               - the priority of the queue
*    Return:
            = 0                    -    succes
            < 0                    -    failed
*    Comments: 
******************************************************************************/
int os_queue_send(mqd_t queue_id, void *data, unsigned int size, uint32_t priority)
{
    int result = 0;

    if(data == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    result = mq_send(queue_id, data, size, priority);  
    if(result)
    {
        (void) fprintf(stderr, "mq_send(): %s\r\n", strerror(errno)); 
	    result = -ERR_SYS;
        goto ERR_EXIT;
    }

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: os_millisec_2_timespec
*    Descriptions: get the current time and convert to timeval.
*    Paramters:
            timeout             - the handle fd of posix queue(returned)
            ts                  - the name of posix queue
*    Return:
            void                - no return    
*    Comments: 
******************************************************************************/
static inline void os_millisec_2_timespec(uint32_t timeout, struct timespec* ts)
{
    struct timeval current_time;
    if(ts == NULL)
    {
        return;
    }
#if 1
    gettimeofday(&current_time, 0);
    ts->tv_sec = current_time.tv_sec;
    
    ts->tv_sec += timeout/1000;
    if((timeout%1000)*1000 + current_time.tv_usec >= 1000000)
    {
        ts->tv_sec += 1;
        ts->tv_nsec = ((timeout%1000)*1000 + current_time.tv_usec)%1000000;
        ts->tv_nsec *= 1000;
    }else{
        ts->tv_nsec = (timeout%1000)*1000 + current_time.tv_usec;
        ts->tv_nsec *= 1000;
    }
#else
    ts->tv_sec = 0;
    ts->tv_nsec = timeout*1000*1000;
#endif
}

/******************************************************************************
*    Function: os_mq_timedrecv
*    Descriptions: recv msg from queue.
*    Paramters:
            id                     - the handle fd of queue
            buf                    - the buf of the queue
            msg_len                - the max recv len need recv
            msg_len                - the len of buf received
            timeout                - wait time(uint ms)
*    Return:
            = 0                    - the length of recv data
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_mq_timedrecv(mqd_t id, void *buf, uint32_t max_len, uint32_t *msg_len, uint32_t timeout)
{
    struct timespec ts;
    int result = 0;

    if((NULL == buf) || (NULL == msg_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    os_millisec_2_timespec(timeout,&ts);

    while ((result = mq_timedreceive(id, buf, max_len, 0, &ts)) < 0 && errno == EINTR)
    {
        continue;
    }

    if (result < 0) 
    {
        (void) fprintf(stderr, "mq_timedreceive(): %s\r\n", strerror(errno)); 
        result = -ERR_SYS;
    } 

    *msg_len = result;
    result = 0;

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: os_mq_timedsend
*    Descriptions: send the msg to queue.
*    Paramters:
            qid                    - the handle fd of posix queue
            data                   - the data we need send
            msg_len                - the length of data
            priority               - the priority of this queue
            timeout                - wait time(uint ms)
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_mq_timedsend(mqd_t qid, void *data, uint32_t msg_len, uint32_t priority, uint32_t timeout)
{
    struct timespec ts;
    int result = 0;

    if(NULL == data)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    os_millisec_2_timespec(timeout, &ts);    
#if 0
    while ((ret = mq_timedsend(qid, data, msg_len, priority, &ts)) < 0 && errno == EINTR)
    {
        continue;
    }
#else
    result = mq_timedsend(qid, data, msg_len, priority, &ts);
#endif

    if (result < 0) 
    {
        (void) fprintf(stderr, "mq_timedsend(): %s\r\n", strerror(errno));
        result = -ERR_SYS;
    }
    
ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    Function: os_msg_init
*    Descriptions: init the system XSI msg.
*    Paramters:
            id_ptr                 - the handle fd of the XSI msg(returned)
            key_name_ptr           - the name of key
            key_id                 - the id of key
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_msg_init(int *id_ptr, const char * key_name_ptr, int key_id)
{
    int result = 0;
    key_t msgkey;

    if((NULL == id_ptr) || (NULL == key_name_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* get key */
    msgkey = ftok(key_name_ptr, key_id);
    if(msgkey == (key_t)-1)
    {
        (void) fprintf(stderr, "ftok() error: %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    result = msgget(msgkey, 0666|IPC_CREAT);
    if(-1 == result)//|IPC_EXCL
    {
		(void) fprintf(stderr, "msgget() error: %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    *id_ptr = result;

	result = 0;
ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    Function: os_msg_snd
*    Descriptions: send msg to the XSI msg.
*    Paramters:
            fd                     - the handle fd of the XSI msg
            type                   - the msg type we send
            data_ptr               - the msg data we send
            data_len               - the length of msg data
            timeout                - wait time 0:BLOCK 1:NOBLOCK
*    Return:
            = 0                    - success
            -ERR_FULLMSG           - msg queue is full
            < 0                    - failed 
*    Comments: 
******************************************************************************/
int os_msg_snd(int fd, long type, const void* data_ptr, size_t data_len, uint16_t timeout)
{
    int ret = 0;
    struct msg_buf msg = {0};

    if(0 >= type)
    {
        ret = -ERR_INVAL;
        fprintf(stderr, "msgtype must large than zero\n");
        goto ERR_EXIT;
    }

    if(NULL == data_ptr)
    {
        ret = - ERR_INVAL;
        goto ERR_EXIT;
    }

    if(MSGBUF_SIZE < data_len)
    {
        ret = -ERR_INVAL;
		fprintf(stderr, "msglen is large than the msgsize(%d)\n", MSGBUF_SIZE);
		data_len = MSGBUF_SIZE;
    }
    
    msg.msgtype = type;
    memcpy(msg.msgdata, data_ptr, data_len);

    if(OSAL_NO_WAIT == timeout)
    {
        ret = msgsnd(fd, (void *)&msg, data_len, IPC_NOWAIT);
        if(-1 == ret)
        {
            if(EAGAIN != errno)
            {
                fprintf(stderr, "msgsnd() error: %s\r\n", strerror(errno));
                ret = -ERR_SYS;
                goto ERR_EXIT;
            }else{
                ret = -ERR_FULLMSG;
                goto ERR_EXIT;
            }
        }
    }else if(OSAL_WAIT_FOREVER == timeout){
        ret = msgsnd(fd, (void *)&msg, data_len, 0);
		if(-1 == ret)
        {
            fprintf(stderr, "msgsnd() error: %s\r\n", strerror(errno));
            ret = -ERR_SYS;
            goto ERR_EXIT;
        }
    }
        
ERR_EXIT:
    
    return ret;
}

/******************************************************************************
*    Function: os_msg_rcv
*    Descriptions: recv msg from the XSI msg.
*    Paramters:
            fd                      - the handle fd of XSI msg
            type                    - the msg type recv from XSI msg, 0: recv all
            data_ptr                - the recv data from msg
            data_len                - the length the data that we need recv from msg
            timeout                 - wait time: 0:BLOCK 1:NOBLOCK
*    Return:
            = 0                     - success
            -ERR_NOMSG              - there is no the type of the msg int the msg queue 
            < 0                     - failed
*    Comments: 
******************************************************************************/
int os_msg_rcv(int fd, long *type, void* data_ptr, size_t *data_len, uint16_t timeout)
{
    int ret = 0;
    struct msg_buf msg = {0};

    if(NULL == data_ptr)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    msg.msgtype = *type;

    if(OSAL_NO_WAIT== timeout)
    {
        ret = msgrcv(fd, (void *)&msg, *data_len, *type, IPC_NOWAIT);
        if(-1 == ret)
        {
            if(ENOMSG != errno)
            {
                fprintf(stderr, "msgrcv() error: %s\r\n", strerror(errno));
                ret = -ERR_SYS;
                goto ERR_EXIT;
            }else{
                ret = -ERR_NOMSG;
                goto ERR_EXIT;
            }
        }

        *data_len = ret;
    }else if(OSAL_WAIT_FOREVER == timeout){
        ret = msgrcv(fd, (void *)&msg, *data_len, *type, 0);
        if(-1 == ret)
        {
            fprintf(stderr, "msgrcv() error: %s\r\n", strerror(errno));
            ret = -ERR_SYS;
            goto ERR_EXIT;
        }
    }

    *type = msg.msgtype;
    memcpy(data_ptr, msg.msgdata, ret);
    *data_len = ret;

    ret = 0;
    
ERR_EXIT:
    
    return ret;
}

/******************************************************************************
*    Function: os_msg_deinit
*    Descriptions: close the XSI msg.
*    Paramters:
            fd                     - the handle fd of systemV msg
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_msg_deinit(int fd)
{
    int ret = 0;
    
    if(msgctl(fd, IPC_RMID, 0) != 0)
    {
        fprintf(stderr, "msgctl() error: %s\r\n", strerror(errno));
        ret = -ERR_SYS;
        
    }
    
    return ret;
}

/******************************************************************************
*    Function: os_shm_init
*    Descriptions: create the share memory and init.
*    Paramters:
            id_ptr                 - the handle fd of the share memory(returned)
            baseaddr_ptr           - the base address of the share memory
            size                   - the size of share memroy
            key_name_ptr           - the name of key
            key_id                 - the id of key
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_shm_init(int* id_ptr, void** baseaddr_ptr, int size, const char* key_name_ptr, int key_id)
{
    int result = 0;
    key_t shmkey;
    
    if((id_ptr == NULL) || (key_name_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* get key */
    if((shmkey = ftok(key_name_ptr, key_id)) == (key_t)-1)
    {
        (void) fprintf(stderr, "ftok() error: %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    result = shmget(shmkey,size, 0666|IPC_CREAT);
    if(-1 == result)//|IPC_EXCL
    {
        (void) fprintf(stderr, "shmget() error: %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    *id_ptr = result;

    *baseaddr_ptr = shmat(*id_ptr,(void*)0,0);
    if((void*)-1 == *baseaddr_ptr)
    {
        (void) fprintf(stderr, "shmat() error: %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    result = 0;
    
ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    Function: os_shm_deinit
*    Descriptions: close share memory.
*    Paramters:
            shmid               - the share memory id of the share memory
            shmaddr              - the share memory address of share memory
*    Return:
            = 0                    -    success
            < 0                    -    failed
*    Comments: 
******************************************************************************/
int os_shm_deinit(int shmid, void* shmaddr)
{
    int result = 0; 

    if(shmaddr == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    result = shmdt(shmaddr);
    if(-1 == result)  
    {  
        (void) fprintf(stderr, "shmdt() error: %s\r\n", strerror(errno));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }  

    result = shmctl(shmid, IPC_RMID, 0);
    if(-1 == result)  
    {  
        (void) fprintf(stderr, "shmctl() error: %s\r\n", strerror(errno));
        result = -ERR_SYS;  
        goto ERR_EXIT;
    }

ERR_EXIT:
    
    return result;
}


