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


#ifndef __OSAL_PTHREAD_H__
#define __OSAL_PTHREAD_H__

#include "os_core.h"
#include "osal_cmn.h"
#include "os_pthread.h"

#ifdef  __cplusplus
extern "C" {
#endif

/******************************************************************************
*    Function: osal_task_create
*    Descriptions: create the pthread task.
*    Paramters:
            taskfd_ptr                 - the handle of the task.
            name                       - the name of the task
            start_routine              - the handle function of the thread
            arg                        - the arg of the handle function
            stack_size                 - the stack size of the pthread task
            prio                       - the prio of the pthread task 0: normal thread
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_task_create(osal_task_t *taskfd_ptr, const char * name, void *(*start_routine)(void *), void * arg, int stack_size, int prio);

/******************************************************************************
*    Function: osal_task_create_joinable
*    Descriptions: create the joinable pthread task.
*    Paramters:
            taskfd_ptr                 - the handle of the task.
            name                       - the name of the task
            prio                       - the prio of the pthread task 0: normal thread
            stack_size                 - the stack size of the pthread task
            start_routine              - the handle function of the thread
            arg                        - the arg of the handle function
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_task_create_joinable(osal_task_t *taskfd_ptr, const char * name, int prio, int stack_size, void * (*start_routine)(void *), void * arg);

/******************************************************************************
*    Function: osal_task_del
*    Descriptions: delete the task.
*    Paramters:
            task                       - the handle of the task.
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_task_del(osal_task_t task);

/******************************************************************************
*    Function: osal_blockallsigs
*    Descriptions: block all the signal.
*    Paramters:
            void                       -
*    Return:
            void                       - 
*    Comments: 
******************************************************************************/
void osal_blockallsigs(void);

/******************************************************************************
*    Function: osal_unblocksig
*    Descriptions: unblock the signal.
*    Paramters:
            sig                        - the no of the signal.
*    Return:
            void                       - 
*    Comments: 
******************************************************************************/
void osal_unblocksig(int sig);

#ifdef  __cplusplus
}
#endif

#endif
