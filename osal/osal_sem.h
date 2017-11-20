/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_sem.h
 @brief  : osal_sem.c header file
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           2017-10-24   pengrui    Add the comments
           ...
******************************************************************************/

#ifndef _OSAL_SEM_H_
#define _OSAL_SEM_H_

#include "os_core.h"
#include "osal_cmn.h"

/******************************************************************************
*    Function: osal_sem_open
*    Descriptions: open named semaphore.
*    Paramters:
            sem_ptr              - the handle of the sem.
            name                 - the name of the sem, such as /semtest
            count                - the init value of sem.
*    Return:
            = 0                  - success
            < 0                  - failed
*    Comments: 
******************************************************************************/
int osal_sem_open(osal_sem_t **sem_ptr, const char* name, uint32_t count);

/******************************************************************************
*    Function: osal_sem_close
*    Descriptions: close the named sem.
*    Paramters:
            name                   - the name of the sem
            sem_ptr                - the handle of the sem
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_sem_close(const char *name, osal_sem_t *sem_ptr);


/******************************************************************************
*    Function: osal_sem_create
*    Descriptions: open the memory-shared sem.
*    Paramters:
            sem_ptr               - the handle of the sem
            count                 - the init value of sem 
*    Return:
            = 0                   - success
            < 0                   - failed
*    Comments: 
******************************************************************************/
int osal_sem_create(osal_sem_t *sem_ptr, uint32_t count);


/******************************************************************************
*    Function: osal_sem_delete
*    Descriptions: close the memory-shared sem.
*    Paramters:
            sem_ptr                - the handle of the sem
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_sem_delete(osal_sem_t *sem_ptr);


/******************************************************************************
*    Function: osal_sem_take
*    Descriptions: take the sem, sem count decrement one.
*    Paramters:
            sem_ptr                    - the handle of the sem
            wait_time                  - the msg type
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_sem_take(osal_sem_t *sem_ptr, uint32_t wait_time);

/******************************************************************************
*    Function: osal_sem_release
*    Descriptions: release the sem.
*    Paramters:
            sem_ptr                   - the handle of the msg
*    Return:
            = 0                       - success
            < 0                       - failed
*    Comments: 
******************************************************************************/
int osal_sem_release(osal_sem_t *sem_ptr);


#endif

