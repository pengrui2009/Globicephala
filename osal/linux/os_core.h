#ifndef _OS_CORE_H_
#define _OS_CORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <dlfcn.h>
#include <getopt.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <linux/socket.h>
#include <asm/ioctl.h>
#include <errno.h>
#include <syslog.h>
#include <mqueue.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/uio.h>


//#include <netpacket/packet.h>
#include <linux/filter.h>
#include <net/if.h>

/* linux */
#include <endian.h>
#include <byteswap.h>

#include "error.h"

#define OSAL_NO_WAIT            0x0000
#define OSAL_WAIT_FOREVER    0xFFFF

#define OSAL_NOBLOCK            0x0
#define OSAL_BLOCK              0xFFFFFFFF


#define MSGBUF_SIZE        256

//typedef long long        int64_t;
//typedef unsigned long long uint64_t;

typedef pthread_t             osal_task_t;
typedef pthread_mutex_t       osal_mutex_t;
typedef pthread_spinlock_t    osal_spinlock_t;
typedef int                   osal_msg_t;                
typedef sem_t                 osal_sem_t;
typedef int                   osal_sema_t;
typedef mqd_t                 osal_mqd_t;
typedef timer_t               osal_timer_t;
typedef int                   osal_shm_t;

#define  os_calloc(cnt, len) calloc(cnt, len)
#define  os_malloc(len)      malloc(len)
#define  os_free(ptr)        free(ptr)
#define  os_printf           printf

typedef long os_time_t;

struct os_time {
    os_time_t sec;
    os_time_t usec;
};

struct os_reltime {
    os_time_t sec;
    os_time_t usec;
};

struct msg_buf
{
    long msgtype;
    char msgdata[MSGBUF_SIZE];
};

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
int os_mutex_create(pthread_mutex_t *mutex_ptr);


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
int  os_mutex_lock(pthread_mutex_t * mutex_ptr);

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
int os_mutex_trylock(pthread_mutex_t * mutex_ptr);


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
int os_mutex_unlock(pthread_mutex_t * mutex_ptr);


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
int os_mutex_destroy(pthread_mutex_t * mutex_ptr);


/******************************************************************************
*    Function: os_sema_open
*    Descriptions: open and init the named posix semaphore.
*    Paramters:
            sem                       - the handle fd of semaphore(returned)
            name                      - the key name of semaphore 
            init_count                - the init count of semaphore
*    Return:
            = 0                       -    success
            < 0                       -    failed
*    Comments: 
******************************************************************************/
int os_sem_open(sem_t **sem, const char *name, int init_cnt);


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
int os_sem_close(sem_t *sem_ptr);


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
int os_sem_unlink(const char *name);


/******************************************************************************
*    Function: os_sem_init
*    Descriptions: open and init the unnamed posix semaphore.
*    Paramters:
            sem                - the handle fd of semaphore(returned)
            pshared            - 0:the semaphore can used only in the thread 1:the semaphore can used int diff process
            init_cnt           - the init count of semaphore
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_sem_init(sem_t * sem, int pshared, int init_cnt);

/******************************************************************************
*    Function: os_sem_destroy
*    Descriptions: destory the unnamed posix semaphore.
*    Paramters:
            sem                - the handle fd of semaphore
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_sem_destroy(sem_t *sem);

/******************************************************************************
*    Function: os_sem_wait
*    Descriptions: read the posix semaphore.
*    Paramters:
            sem                - the handle fd of semaphore(returned)
            timeout            - 0: no wait 0xFFFF: wait forever other:wait timeout ms
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_sem_wait(sem_t* sem, uint32_t timeout);

/******************************************************************************
*    Function: os_sem_post
*    Descriptions: post the posix semaphore.
*    Paramters:
            sem                - the handle fd of semaphore(returned)
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int  os_sem_post(sem_t *sem);

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
int os_sema_open(int *p_semfd, const char* name, int key_id, int init_count);


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
int os_sema_lock(int semfd);

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
int os_sema_unlock(int semfd);

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
int os_sema_close(int semfd);

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
int os_spinlock_init(pthread_spinlock_t  *spinlock_ptr, int pshared);

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
int os_spinlock_lock(pthread_spinlock_t *spinlock_ptr);

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
int os_spinlock_trylock(pthread_spinlock_t *spinlock_ptr);

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
int os_spinlock_unlock(pthread_spinlock_t *spinlock_ptr);

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
int os_spinlock_destroy(pthread_spinlock_t *spinlock_ptr);

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
uint64_t os_tick_get(void);

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
int os_get_time(struct os_time *t);

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
int os_get_reltime(struct os_reltime *t);

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
int os_set_reltime(struct os_reltime *t);


/******************************************************************************
*    Function: os_delay
*    Descriptions: delay usec.
*    Paramters:
            usec                - the count of delay time(uint us)
*    Return:
            void
*    Comments: 
******************************************************************************/
void os_delay(int usec);

/******************************************************************************
*    Function: os_sleep
*    Descriptions: sleep msec ms.
*    Paramters:
            msec               - the time of sleep(uint ms)
*    Return:
            void
*    Comments: 
******************************************************************************/
void os_sleep(int msec);

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
int os_current_thread(void);

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
                          );


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
int os_queue_delete(mqd_t queue_id);


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
int os_queue_recv(mqd_t queue_id, void *data, uint32_t size, uint32_t *size_copied);


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
int os_queue_send(mqd_t queue_id, void *data, unsigned int size, uint32_t priority);


/******************************************************************************
*    Function: os_mq_timedrecv
*    Descriptions: recv msg from queue.
*    Paramters:
            id                     - the handle fd of queue
            buf                    - the buf of the queue
            msg_len                - the max recv len need recv
            msg_len                - the len of buf received
            timeout                - wait time
*    Return:
            = 0                    - the length of recv data
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_mq_timedrecv(mqd_t id, void *buf, uint32_t max_len, uint32_t *msg_len, uint32_t timeout);


/******************************************************************************
*    Function: os_mq_timedsend
*    Descriptions: send the msg to queue.
*    Paramters:
            qid                    - the handle fd of posix queue
            data                   - the data we need send
            msg_len                - the length of data
            priority               - the priority of this queue
            timeout                - wait time
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int os_mq_timedsend(mqd_t qid, void *data, uint32_t msg_len, uint32_t priority, uint32_t timeout);


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
int os_msg_init(int *id_ptr, const char * key_name_ptr, int key_id);


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
int os_msg_snd(int fd, long type, const void* data_ptr, size_t data_len, uint16_t timeout);


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
            -ERR_NOMSG              - there is no such type msg int the msg queue 
            < 0                     - failed
*    Comments: 
******************************************************************************/
int os_msg_rcv(int fd, long *type, void* data_ptr, size_t *data_len, uint16_t timeout);


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
int os_msg_deinit(int fd);


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
int os_shm_init(int* id_ptr, void** baseaddr_ptr, int size, const char* key_name_ptr, int key_id);


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
int os_shm_deinit(int shmid, void* shmaddr);


#endif

