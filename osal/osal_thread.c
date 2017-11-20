/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_pthread.c
 @brief  : pthread os adapt layer
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           2017-10-24   pengrui    Add the comments
           ...
******************************************************************************/

#include "osal_thread.h"
#include <limits.h>

/******************************************************************************
*    Function: osal_blockallsigs
*    Descriptions: block all the signal.
*    Paramters:
            void                       -
*    Return:
            void                       - 
*    Comments: 
******************************************************************************/
void osal_blockallsigs(void)
{
    os_blockallsigs();
}

/******************************************************************************
*    Function: osal_unblocksig
*    Descriptions: unblock the signal.
*    Paramters:
            sig                        - the no of the signal.
*    Return:
            void                       - 
*    Comments: 
******************************************************************************/
void osal_unblocksig(int sig)
{
    os_unblocksig(sig);
}

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
int osal_task_create(osal_task_t *taskfd_ptr, const char * name, void *(*start_routine)(void *), void * arg, int stack_size, int prio)
{
    int ret = 0;
    
    ret = os_pthread_create(taskfd_ptr, name, prio, stack_size, start_routine, arg);
	
    return ret;
}

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
int osal_task_create_joinable(osal_task_t *taskfd_ptr, const char * name, int prio, int stack_size, void * (*start_routine)(void *), void * arg)
{
    int ret;

    ret = os_pthread_create_joinable(taskfd_ptr, name, prio, stack_size, start_routine, arg);
    
    return ret;
}

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
int osal_task_del(osal_task_t task)
{
	
    return 0; 
}
