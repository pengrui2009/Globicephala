/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : spinlock_test.c
 @brief  : test the spinlock api
 @author : pengrui
 @history:
           2017-10-24    pengrui    Created file
           ...
******************************************************************************/
#include "osal_spinlock.h"

int main()
{
    int ret =0;
    osal_spinlock_t spinlockfd;
    
    ret = osal_spinlock_init(&spinlockfd, SPINLOCK_CMN);
    if(ret)
    {
        fprintf(stderr, "osal_spinlock_init() \n");
        goto ERR_EXIT;
    }

    ret = osal_spinlock_lock(&spinlockfd);
    if(ret)
    {
        fprintf(stderr, "osal_spin_lock() \n");
        goto ERR_EXIT;
    }

    ret = osal_spinlock_lock(&spinlockfd);
    if(ret)
    {
        fprintf(stderr, "osal_spin_lock() \n");
        goto ERR_EXIT;
    }

    ret = osal_spinlock_unlock(&spinlockfd);
    if(ret)
    {
        fprintf(stderr, "osal_spin_unlock() \n");
        goto ERR_EXIT;
    }

    
    ret = osal_spinlock_destroy(&spinlockfd);
    if(ret)
    {
        fprintf(stderr, "osal_spin_unlock() \n");
        goto ERR_EXIT;
    }
ERR_EXIT:

    return ret;
}

