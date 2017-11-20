/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_sema.h
 @brief  : system v Semaphore api
 @author : pengrui
 @history:
           2017-10-18    pengrui    Created file
           ...
******************************************************************************/
#ifndef _OSAL_SEMA_H_
#define	_OSAL_SEMA_H_

#include "os_core.h"
#include "osal_cmn.h"

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
int osal_sema_open(osal_sema_t *sema_ptr, const char *name, int key_id, int init_count);

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
int osal_sema_close(osal_sema_t semfd);


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
int osal_sema_lock(osal_sema_t semfd);


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
int osal_sema_unlock(osal_sema_t semfd);


#endif /*_OSAL_SEMA_H_*/
