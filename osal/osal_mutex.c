/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_mutex.c
 @brief  : pthread mutex api
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           2017-10-24   pengrui    modify the api and add the comments
           ...
******************************************************************************/
#include "osal_mutex.h"

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
int osal_mutex_create(osal_mutex_t *mutex_ptr)
{
    int ret = 0; 

    ret = os_mutex_create(mutex_ptr);
    if (ret)
    {    
        goto ERR_EXIT;
    }

ERR_EXIT:
 
    return ret;
}

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
int osal_mutex_delete(osal_mutex_t *mutex_ptr)
{
    int ret = 0;
    
    ret = os_mutex_destroy(mutex_ptr);
    
    return ret; 
}

/******************************************************************************
*    Function: osal_mutex_lock
*    Descriptions: lock the mutex.
*    Paramters:
            mutex_ptr              - the handle of the mutex.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: the mutex_ptr paramter must be pointer, because we will update mutex 
******************************************************************************/
int osal_mutex_lock(osal_mutex_t *mutex_ptr)
{
    int ret = 0;
    
    ret = os_mutex_lock(mutex_ptr); 

    return ret;
}

/******************************************************************************
*    Function: osal_mutex_unlock
*    Descriptions: unlock the mutez.
*    Paramters:
            mutex_ptr              - the handle of the mutex.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: the mutex_ptr paramter must be pointer, because we will update mutex 
******************************************************************************/
int osal_mutex_unlock(osal_mutex_t *mutex_ptr)
{   
    int ret = 0;
    
    ret = os_mutex_unlock(mutex_ptr);
    
    return ret; 
}

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
int osal_mutex_trylock(osal_mutex_t *mutex_ptr)
{
    int ret = 0;

    ret = os_mutex_trylock(mutex_ptr);

    return ret; 
}


