/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : sem_test.c
 @brief  : test the sem api
 @author : pengrui
 @history:
           2015-8-25    wanglei    Created file
           2017-10-24   pengrui    Add the comments
           ...
******************************************************************************/
#include "osal_sem.h"



int main(void)
{
    int result = 0;
    osal_sem_t sem;

    result = osal_sem_create(&sem, 0);
    if(result)
    {
        fprintf(stderr, "osal_sem_create() error\n");
        goto ERR_EXIT;
    }
    
    result = osal_sem_take(&sem, OSAL_WAIT_FOREVER);
    if(result)
    {
        fprintf(stderr, "osal_sem_take() error\n");
        goto ERR_EXIT;
    }    

    result = osal_sem_take(&sem, OSAL_WAIT_FOREVER);
    if(result)
    {
        fprintf(stderr, "osal_sem_take() error\n");
        goto ERR_EXIT;
    }

    result = osal_sem_release(&sem);
    if(result)
    {
        fprintf(stderr, "osal_sem_release() error\n");
        goto ERR_EXIT;
    }

    result = osal_sem_delete(&sem);
    if(result)
    {
        fprintf(stderr, "osal_sem_delete() error\n");
        goto ERR_EXIT;
    }

ERR_EXIT:

    return result;
}

