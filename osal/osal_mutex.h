/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_mutex.h
 @brief  : osal_mutex.c header file
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           2017-10-24   pengrui    modify the api and add the comments
           ...
******************************************************************************/


#ifndef _OSAL_MUTEX_H_
#define _OSAL_MUTEX_H_

#include "os_core.h"
#include "osal_cmn.h"

/******************************************************************************
*    Function: osal_mutex_create
*    Descriptions: open the mutex.
*    Paramters:
            mutex_ptr                - the handle of the mutex.
*    Return:
            = 0                      - success
            < 0                      - failed
*    Comments: 
******************************************************************************/
int osal_mutex_create(osal_mutex_t *mutex_ptr);

/******************************************************************************
*    Function: osal_mutex_delete
*    Descriptions: close the mutex.
*    Paramters:
            mutex_ptr              - the handle of the mutex.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_mutex_delete(osal_mutex_t *mutex_ptr);


/******************************************************************************
*    Function: osal_mutex_lock
*    Descriptions: lock the mutex.
*    Paramters:
            mutex_ptr              - the handle of the mutex.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_mutex_lock(osal_mutex_t *mutex_ptr);

/******************************************************************************
*    Function: osal_mutex_unlock
*    Descriptions: unlock the mutez.
*    Paramters:
            mutex_ptr              - the handle of the mutex.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_mutex_unlock(osal_mutex_t *mutex_ptr);


/******************************************************************************
*    Function: osal_mutex_trylock
*    Descriptions: try lock the mutex, if can't lock return error.
*    Paramters:
            mutex_ptr              - the handle of the mutex.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_mutex_trylock(osal_mutex_t *mutex_ptr);

#endif /* _OSAL_MUTEX_H_*/

