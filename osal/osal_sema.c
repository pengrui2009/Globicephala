/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_sema.c
 @brief  : system v Semaphore api
 @author : pengrui
 @history:
           2017-10-18    pengrui    Created file
           ...
******************************************************************************/
#include "osal_sema.h"

/******************************************************************************
*    Function: osal_sema_open
*    Descriptions: open semaphore.
*    Paramters:
            sema_ptr               - the handle of the sema
            name                   - the name of key.
            key_id                 - the id of key.
            init_count             - the init value of the sem.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_sema_open(osal_sema_t *sema_ptr, const char *name, int key_id, int init_count)
{
    int result = 0;

    result = os_sema_open(sema_ptr, name, key_id, init_count);
    if(result)
    {
        goto ERR_EXIT;
    }
    
ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    Function: osal_sema_close
*    Descriptions: close semaphore.
*    Paramters:
            semfd                  - the handle of sem.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_sema_close(osal_sema_t semfd)
{
    int result = 0;
    
    result = os_sema_close(semfd);
    
    return result;
}

/******************************************************************************
*    Function: osal_sema_lock
*    Descriptions: lock the sem.
*    Paramters:
            semfd                 - the handle fd of the sem.
*    Return:
            =0                    - succes
            <0                    - failed
*    Comments: 
******************************************************************************/
int osal_sema_lock(osal_sema_t semfd)
{
    int result = 0;
    
    result = os_sema_lock(semfd);

    return result;
}

/******************************************************************************
*    Function: osal_sema_unlock
*    Descriptions: unlock the sem.
*    Paramters:
            semfd                 - the handle fd of the sem.
*    Return:
            =0                    - success
            <0                    - failed
*    Comments: 
******************************************************************************/
int osal_sema_unlock(osal_sema_t semfd)
{
    int result = 0;
    
    result = os_sema_unlock(semfd);
    
    return result;
}

