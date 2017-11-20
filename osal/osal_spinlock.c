/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_spinlock.c
 @brief  : spinlock 
 @author : wanglei
 @history:
           2015-12-9    wanglei    Created file
           2017-10-24   pengrui    Add the comments
           ...
******************************************************************************/
#include "osal_spinlock.h"

/******************************************************************************
*    Function: osal_spinlock_init
*    Descriptions: init the spinlock.
*    Paramters:
            level                      - the level of the spinlock
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_spinlock_init(osal_spinlock_t *spinlock_ptr, OSAL_SPINLOCK_LEVEL level)
{
    int ret = 0;

    if(spinlock_ptr == NULL)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    ret = os_spinlock_init(spinlock_ptr, level);
    if(ret)
    {
        goto ERR_EXIT;
    }
    
ERR_EXIT:

    return ret;
}

/******************************************************************************
*    Function: osal_spinlock_lock
*    Descriptions: lock the spinlock.
*    Paramters:
            spinlockfd                 - the handle of the spinlock.
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_spinlock_lock(osal_spinlock_t *spinlockfd)
{
    int result = 0;
    
    result = os_spinlock_lock(spinlockfd);
    
    return result;
}

/******************************************************************************
*    Function: osal_spinlock_unlock
*    Descriptions: unlock the spinlock.
*    Paramters:
            spinlockfd             - the handle of the spinlock.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_spinlock_unlock(osal_spinlock_t *spinlockfd)
{
    int result = 0;
    
    result = os_spinlock_unlock(spinlockfd);
    
    return result;
}

/******************************************************************************
*    Function: osal_spinlock_destroy
*    Descriptions: close the spinlock.
*    Paramters:
            spinlockfd                 - the handle of the spinlock_ptr.
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_spinlock_destroy(osal_spinlock_t *spinlockfd)
{
    int result = 0;
    
    result = os_spinlock_destroy(spinlockfd);
    
    return result;
}


