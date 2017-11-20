/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_shm.c
 @brief  : the function file of shm.
 @author : pengrui
 @history:
           2017-10-19    pengrui    Created file
           ...
******************************************************************************/
#include "osal_shm.h"

/******************************************************************************
*    Function: osal_shm_init
*    Descriptions: open and init the shm.
*    Paramters:
            shmfd_ptr              - the handle fd of share memory
            shmaddr_ptr            - the base address of share memory
            size                   - the size of share memory zone
            key_name_ptr           - the name of key
            key_id                 - the id of key
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_shm_init(osal_shm_t *shmfd_ptr, void **shmaddr_ptr,int size,const char * key_name_ptr,int key_id)
{
    int ret = 0;
    
    ret = os_shm_init(shmfd_ptr, (void *)shmaddr_ptr, size, key_name_ptr, key_id);
    if(ret)
    {
        goto ERR_EXIT;
    }

ERR_EXIT:

    return ret;
}

/******************************************************************************
*    Function: osal_shm_deinit
*    Descriptions: close the share memory.
*    Paramters:
            fd                     - the handle fd of the share memory
            shmaddr_ptr            - the base address of share memory
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_shm_deinit(osal_shm_t fd, void *shmaddr_ptr)
{
    int ret = 0;
    
    ret = os_shm_deinit(fd, shmaddr_ptr);

    return ret;
}
