/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : shzone.c
 @brief  : This file contain share data module driver fucntions.
 @author : pengrui
 @history:
           2017-9-28    pengrui      Created file
           2017-10-19   wangxianwen  Optimize the structure.
           ...
******************************************************************************/

#include "shzone.h"


/******************************************************************************
*    Function: shzone_thread_entry
*    Descriptions: Thread that handle the emergency data.
*    Paramters:
            param_ptr       -    the file descriptor.
*    Return:
            = 0                -    succeed.
            < 0                -    failed
*    Comments: 
******************************************************************************/
static void *shzone_thread_entry(shzone_st_ptr shzone_ptr)
{
    int                  ret = 0;
    long             msgtype = 0; /* Receive all the message. */
    shzone_msg_st shzone_msg = { 0, 0 };
    uint32_t      msg_length = 0;


    /* Error detection. */
    if(shzone_ptr == NULL)
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

START_RCV:
    
    /* Reset messgae length and receive new mesage. */
    msgtype = 0;
    msg_length = sizeof(shzone_msg);
    
    ret = osal_msg_recv(shzone_ptr->msg_id, &msgtype, &shzone_msg, &msg_length, OSAL_WAIT_FOREVER);
    if(ret != ERR_OK)
    {
        printf("[%s %d]: Receive message error. \n", __FUNCTION__, __LINE__);
    }
    else
    {
        /* Call the user handle function. */
        shzone_ptr->config.cback_ptr(shzone_msg.addr, shzone_msg.data_len);
    }
    
    goto START_RCV;
    
ERR_EXIT:

    return (void *)NULL;
}


/******************************************************************************
*    Function: shzone_read
*    Descriptions: Read data from share memory zone.
*    Paramters:
            fd                -    the file descriptor.
            addr            -   Address for data to read.
            buff_ptr        -   buffer head address for data storage.
            data_len        -   the data length that user want to read.
*    Return:
            >= 0            -    the data count that have readed.
            < 0                -    failed
*    Comments: 
******************************************************************************/
int shzone_read(int fd, uint32_t addr, void *buff_ptr, uint16_t data_len)
{
    int                  ret = 0;
    shzone_st_ptr shzone_ptr = (shzone_st_ptr)fd;


    /* Error detection. */
    if((shzone_ptr == NULL) || (shzone_ptr->config.shm_size <= addr) || (buff_ptr == NULL))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Get semaphone. */
    ret = osal_sema_lock(shzone_ptr->sem_id);
    if(ret != ERR_OK)
    {
        printf("[%s %d]: Get semaphone error. \n", __FUNCTION__, __LINE__);

        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    /* Copy data into share zone. */
    if(shzone_ptr->config.shm_size < (addr + data_len))
    {
        data_len = shzone_ptr->config.shm_size - addr;
    }
    memcpy(buff_ptr, shzone_ptr->shm_addr_ptr + addr, data_len);

    /* Release semaphone. */
    ret = osal_sema_unlock(shzone_ptr->sem_id);
    if(ret != ERR_OK)
    {
        printf("[%s %d]: Get semaphone error. \n", __FUNCTION__, __LINE__);

        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    /* Return the data length that have been writed. */
    ret = data_len;

ERR_EXIT:

    return ret;
}


/******************************************************************************
*    Function: shzone_write
*    Descriptions: Write data to share memory zone.
*    Paramters:
            fd                -    the file descriptor.
            type            -   Data type for buffer.
            addr            -   Address for data to write.
            buff_ptr        -   buffer head address for data storage.
            data_len        -   the data length that user want to write.
*    Return:
            >= 0            -    the data count that have writed.
            < 0                -    failed
*    Comments: 
******************************************************************************/
int shzone_write(int fd, shzone_datatype_em type, uint32_t addr, void *buff_ptr, uint16_t data_len)
{
    int                  ret = 0;
    shzone_st_ptr shzone_ptr = (shzone_st_ptr)fd;
    shzone_msg_st shzone_msg = { 0, 0 };
    

    /* Error detection. */
    if((shzone_ptr == NULL) || (shzone_ptr->config.shm_size <= addr) || (buff_ptr == NULL))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Get semaphone. */
    ret = osal_sema_lock(shzone_ptr->sem_id);
    if(ret != ERR_OK)
    {
        printf("[%s %d]: Get semaphone error. \n", __FUNCTION__, __LINE__);

        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    /* Copy data into share zone. */
    if(shzone_ptr->config.shm_size < (addr + data_len))
    {
        data_len = shzone_ptr->config.shm_size - addr;
    }
    memcpy(shzone_ptr->shm_addr_ptr + addr, buff_ptr, data_len);

    /* Release semaphone. */
    ret = osal_sema_unlock(shzone_ptr->sem_id);
    if(ret != ERR_OK)
    {
        printf("[%s %d]: Get semaphone error. \n", __FUNCTION__, __LINE__);

        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    /* Send message when data type is emergency. */
    if(type == SHZONE_DATATYPE_EMERGENCY)
    {
        shzone_msg.addr = addr;
        shzone_msg.data_len = data_len;
        
        ret = osal_msg_send(shzone_ptr->msg_id, 0, &shzone_msg, sizeof(shzone_msg), OSAL_NO_WAIT);
        if(ret != ERR_OK)
        {
            printf("[%s %d]: Send message queue error. \n", __FUNCTION__, __LINE__);

            ret = -ERR_SYS;
            goto ERR_EXIT;
        }
    }

    /* Return the data length that have been writed. */
    ret = data_len;
    
ERR_EXIT:

    return ret;
}


/******************************************************************************
*    Function: shzone_init
*    Descriptions: open or create the share zone structure.
*    Paramters:
            fd_ptr          -   the pointer that hold net file descriptor.
            cfg_ptr            -    the config paramters pointer.
*    Return:
            > 0                -    file descriptor
            < 0                -    failed
*    Comments: 
******************************************************************************/
int shzone_init(int *fd_ptr, shzone_config_st_ptr cfg_ptr)
{
    int                  ret = 0;
    shzone_st_ptr shzone_ptr = NULL;

    
    /* Error detection. */
    if((fd_ptr == NULL) || (cfg_ptr == NULL) || (cfg_ptr->name_ptr == NULL) || (cfg_ptr->shm_size == 0))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Calloc new memory for share zone structure. */
    if((shzone_ptr = os_calloc(1, sizeof(*shzone_ptr))) == NULL)
    {
        printf("[%s %d]: Allocate memory for shzone driver error. \n", __FUNCTION__, __LINE__);

        ret = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    /* Init share memory. */
    ret = osal_shm_init(&(shzone_ptr->shm_id), (void**)&(shzone_ptr->shm_addr_ptr), cfg_ptr->shm_size, cfg_ptr->name_ptr, cfg_ptr->id);
    if(ret != ERR_OK)
    {
        printf("[%s %d]: Init share memory error. \n", __FUNCTION__, __LINE__);
        goto ERR_EXIT;
    }

    /* Init share memory's semaphone. */
    ret = osal_sema_open(&(shzone_ptr->sem_id), cfg_ptr->name_ptr, cfg_ptr->id, 1);
    if(ret != ERR_OK)
    {
        printf("[%s %d]: Init samaphone error. \n", __FUNCTION__, __LINE__);
        goto ERR_EXIT;
    }

    /* Init message queue. */
    ret = osal_msg_open(&(shzone_ptr->msg_id), cfg_ptr->name_ptr, cfg_ptr->id);
    if(ret != ERR_OK)
    {
        printf("[%s %d]: Init message queue error. \n", __FUNCTION__, __LINE__);
        goto ERR_EXIT;
    }

    /* Set configuration. */
    memcpy(&(shzone_ptr->config), cfg_ptr, sizeof(shzone_ptr->config));

    /* Create task for callback function process. */
    ret = osal_task_create(&(shzone_ptr->cback_task), "shzone_task", (void* (*)(void*))shzone_thread_entry, shzone_ptr, SHZONE_TASK_STACKSIZE, THZONE_TASK_PRIORITY);
    if(ret != ERR_OK)
    {
        printf("[%s %d]: Create task error. \n", __FUNCTION__, __LINE__);
        goto ERR_EXIT;
    }

    /* return the file descriptor. */
    * fd_ptr = (int)shzone_ptr;
    ret = ERR_OK;

    return ret;
    
ERR_EXIT:

    /* Deinit the shzone driver when error. */
    if(shzone_ptr != NULL)
    {
        shzone_deinit((int)shzone_ptr);
    }
    return ret;
}


/******************************************************************************
*    Function: shzone_deinit
*    Descriptions: close share memory driver.
*    Paramters:
            fd                -    the file descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
int shzone_deinit(int fd)
{
    int                  ret = 0;
    shzone_st_ptr shzone_ptr = (shzone_st_ptr)fd;


    /* Error detection. */
    if(shzone_ptr == NULL)
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Deinit share memory. */
    if(shzone_ptr->shm_id != 0)
    {
        ret = osal_shm_deinit(shzone_ptr->shm_id, shzone_ptr->shm_addr_ptr);
        if(ret != ERR_OK)
        {
            printf("[%s %d]: Deinit share memory error. \n", __FUNCTION__, __LINE__);
            goto ERR_EXIT;
        }
    }

    /* Deinit semaphone. */
    if(shzone_ptr->sem_id != 0)
    {
        ret = osal_sema_close(shzone_ptr->sem_id);
        if(ret != ERR_OK)
        {
            printf("[%s %d]: Deinit semophone error. \n", __FUNCTION__, __LINE__);
            goto ERR_EXIT;
        }
    }

    /* Deinit message queue. */
    if(shzone_ptr->msg_id != 0)
    {
        ret = osal_msg_close(shzone_ptr->msg_id);
        if(ret != ERR_OK)
        {
            printf("[%s %d]: Deinit message queue error. \n", __FUNCTION__, __LINE__);
            goto ERR_EXIT;
        }
    }

    /* Delete task. */
    if(shzone_ptr->cback_task != 0)
    {
        ret = osal_task_del(shzone_ptr->cback_task);
        if(ret != ERR_OK)
        {
            printf("[%s %d]: Delete task error. \n", __FUNCTION__, __LINE__);
            goto ERR_EXIT;
        }
    }

    /* Free the driver memory. */
    os_free(shzone_ptr);
    ret = ERR_OK;

ERR_EXIT:

    return ret;
}


