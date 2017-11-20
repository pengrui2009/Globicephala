/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_shm.c
 @brief  : test the shm api
 @author : pengrui
 @history:
           2017-10-24    pengrui    Created file
           ...
******************************************************************************/
#include "osal_shm.h"

int main()
{
    int ret = 0;
    uint8_t shm_buf[20] = {0};
    void *shmaddr_ptr= NULL;
    int shm_size = 100;
    osal_shm_t shmfd;

    ret = osal_shm_init(&shmfd, &shmaddr_ptr, shm_size, ".", 'A');
    if(ret)
    {
        fprintf(stderr, "osal_shm_init() error\n");
    }

    memcpy(shmaddr_ptr, shm_buf, 20);

    return 0;
}

