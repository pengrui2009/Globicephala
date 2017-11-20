/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_spinlock.h
 @brief  : osal_spinlock.c header file
 @author : wanglei
 @history:
           2015-8-14    wanglei    Created file
           ...
******************************************************************************/

#ifndef _OSAL_SPINLOCK_H_
#define _OSAL_SPINLOCK_H_

#include "os_core.h"
#include "osal_cmn.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum
{
    SPINLOCK_CMN = 0,  /*spin_lock*/
    SPINLOCK_BH  = 1,  /*spin_lock_bh*/
    SPINLOCK_IRQ = 2,  /*spin_lock_irq*/
    SPINLOCK_SMP = 3,  /*spin_lock_irqsave*/
} OSAL_SPINLOCK_LEVEL;

/******************************************************************************
*    Function: osal_spinlock_init
*    Descriptions: init the spinlock.
*    Paramters:
            spinlock_id_ptr            - the handle of the spinlock.
            level                      - the level of the spinlock
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_spinlock_init(osal_spinlock_t *spinlock_ptr, OSAL_SPINLOCK_LEVEL level);

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
int osal_spinlock_lock(osal_spinlock_t *spinlockfd);


/******************************************************************************
*    Function: osal_spinlock_unlock
*    Descriptions: unlock the spinlock.
*    Paramters:
            spinlockfd                - the handle of the spinlock.
*    Return:
            = 0                       - success
            < 0                       - failed
*    Comments: 
******************************************************************************/
int osal_spinlock_unlock(osal_spinlock_t *spinlockfd);

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
int osal_spinlock_destroy(osal_spinlock_t *spinlockfd);

#ifdef  __cplusplus
}
#endif

#endif

