/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : sem_recv.c
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
    int init_count = 0;

    ret = osal_sema_open(&semafd, ".", 'A', init_count);
    if(ret)
    {
        fprintf(stderr, "osal_sema_open error\n");
        goto ERR_EXIT;
    }

    ret =osal_sema_unlock(semafd);
    if(ret)
    {
        fprintf(stderr, "osal_sema_open error\n");
        goto ERR_EXIT;
    }


ERR_EXIT:

    return ret;
}

