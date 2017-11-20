/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : os_pthread.h
 @brief  : os_pthread.h header file
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           ...
******************************************************************************/


#ifndef __OS_PTHREAD_H__
#define __OS_PTHREAD_H__

#include <pthread.h>
#include <stdint.h>
#ifdef  __cplusplus
extern "C" {
#endif

#define TK_CLS_LOW              (10)
#define TK_CLS_BELOW_NORMAL     (20)
#define TK_CLS_NORMAL           (30)
#define TK_CLS_ABOVE_NORMAL     (40)
#define TK_CLS_HIGH             (48)

#define TK_PRIO_LOW             (-2)
#define TK_PRIO_NORMAL          (0)
#define TK_PRIO_HIGH            (2)

#define TK_PRIO(cls, prio)      ((cls) + (prio))
#define TK_PRIO_DEFAULT         (TK_PRIO(TK_CLS_NORMAL, TK_PRIO_NORMAL))

#define TK_NAME_PREFIX          "tk_"

#define THREAD_MODE_NORMAL             0            //普通线程
#define THREAD_MODE_REALTIME        1            //实时线程
/*************************************************
  API
*************************************************/


/******************************************************************************
*     Function: thread_cancel
*     Descriptions: cancel the pthread through the pid.
*     Paramters:
            tid                - the pid of the thread
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
******************************************************************************/
int thread_cancel(pthread_t tid);


/******************************************************************************
*     Function: thread_setpriority_base
*     Descriptions: set the pthread priority.
*     Paramters:
            tid                - the pid of the thread
            prio               - the priority of the thread
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
******************************************************************************/
int thread_setpriority(pthread_t tid, uint8_t prio);


/******************************************************************************
*    Function: thread_create_base
*    Descriptions: create the thread.
*    Paramters:
            tid                - the pid of the thread
            function           - the handle function
            arg                - the arguments of the handle function
            mode               - the mode of the thread
            prio               - the priority of the thread
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/ 
int thread_create_base(pthread_t *tid, void *(*function)(void *), void *arg, uint8_t mode, uint8_t prio);

/******************************************************************************
*     Function: thread_setpriority_base
*     Descriptions: set the pthread priority.
*     Paramters:
            tid                - the pid of the thread
            prio               - the priority of the thread
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
******************************************************************************/
int thread_setpriority_base(pthread_t tid, uint8_t prio);

/******************************************************************************
*    Function: thread_gettid_base
*    Descriptions: get the pthread id.
*    Paramters:
            void               - 
*    Return:
            > 0                - the pid of the thread
            < 0                - failed
*    Comments: 
******************************************************************************/
int thread_gettid_base(void);

/******************************************************************************
*    Function: os_blockallsigs
*    Descriptions: block all the signal.
*    Paramters:
            void               - 
*    Return:
            void               - 
*    Comments: 
******************************************************************************/
void os_blockallsigs(void);


/******************************************************************************
*    Function: os_unblocksig
*    Descriptions: unblock the signal.
*    Paramters:
            sig                - the no of the signal
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
void os_unblocksig(int sig);


/******************************************************************************
*    Function: os_pthread_create
*    Descriptions: create the thread.
*    Paramters:
            thread             - the handle of the thread
            name               - the name of the thread
            prio               - the prio of the thread
            stack_size         - the stack size of the pthread
            start_routine      - the handle function of the thread
            arg                - the arg of the handle function
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_pthread_create(pthread_t * thread, const char * name, int prio, int stack_size, 
             void * (*start_routine)(void *), void * arg);



/******************************************************************************
*    Function: os_pthread_create_joinable
*    Descriptions: create the joinable thread .
*    Paramters:
            thread             - the handle of the thread
            name               - the name of the thread
            prio               - the prio of the thread
            stack_size         - the stack size of the pthread
            start_routine      - the handle function of the thread
            arg                - the arg of the handle function
*    Return:
            = 0                - success
            < 0                - failed
*    Comments: 
******************************************************************************/
int os_pthread_create_joinable(pthread_t * thread, const char * name, int prio, int stack_size, 
            void * (*start_routine)(void *), void * arg);


#ifdef  __cplusplus
}
#endif


#endif
