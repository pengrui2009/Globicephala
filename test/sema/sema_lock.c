/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : sem_send.c
 @brief  : test the named sema api
 @author : pengrui
 @history:
           2017-10-24   pengrui    create file
           ...
******************************************************************************/
#include "osal_sema.h"

int main()
{
    int ret = 0;
    osal_sema_t semafd;
    int init_count = 1;

    ret = osal_sema_open(&semafd, ".", 'A', init_count);
    if(ret)
    {
        fprintf(stderr, "osal_sema_open error\n");
        goto ERR_EXIT;
    }

    ret =osal_sema_lock(semafd);
    if(ret)
    {
        fprintf(stderr, "osal_sema_open error\n");
        goto ERR_EXIT;
    }

    ret =osal_sema_lock(semafd);
    if(ret)
    {
        fprintf(stderr, "osal_sema_open error\n");
        goto ERR_EXIT;
    }

ERR_EXIT:

    return ret;
}

