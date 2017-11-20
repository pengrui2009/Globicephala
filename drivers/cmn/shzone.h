/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : shzone.h
 @brief  : This file contain share zone module driver fucntions.
 @author : pengrui
 @history:
           2017-9-28    pengrui      Created file
           2017-10-30   wangxianwen  Optimize the structure.
           ...
******************************************************************************/
#ifndef __DRV_SHZONE_H__
#define __DRV_SHZONE_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "os_core.h"
#include "osal_sema.h"
#include "osal_shm.h"
#include "osal_msg.h"
#include "osal_thread.h"
#include "error.h"

#define STR_EMER_MSG            ("EMER_MSG")
#define MAX_MSG_BUF_SIZE        16


/* Data type for share zone data. */
typedef enum _shzone_datatype_em
{
    SHZONE_DATATYPE_NORMAL    = 0,
    SHZONE_DATATYPE_EMERGENCY = 1

}shzone_datatype_em, *shzone_datatype_em_ptr;


/* Share zone's message structure. */
typedef struct _shzone_msg_st
{
    /* Valid data address and length. */
    uint32_t     addr;
    uint16_t data_len;
    
}shzone_msg_st, *shzone_msg_st_ptr;


/* Callback function definition. */
typedef void (*func_ptr)(uint32_t addr, uint16_t data_len);

/* shzone configuration structure. */
typedef struct _shzone_config_st
{
    /* Share zone module name and id. */
    char       * name_ptr;
    int                id;

    /* The size of shm zone. */
    unsigned int shm_size;

    /* shzone emergency data callback func. */
    func_ptr    cback_ptr; 
    
}shzone_config_st, *shzone_config_st_ptr;


/* Share zone file structure. */
typedef struct _shzone_st
{   
    /* Configure for share zone. */
    shzone_config_st config;

    /* The base address and id for share memory. */
    uint8_t   *shm_addr_ptr;
    osal_shm_t       shm_id;

    /* Semaphore id and message id for share memory. */
    osal_sema_t      sem_id;
    osal_msg_t       msg_id;

    /* Task that handle callback  */
    osal_task_t  cback_task;
    
}shzone_st, * shzone_st_ptr;


/* Task stack and priority for cback_task. */
#define SHZONE_TASK_STACKSIZE  (4096)                 /*shzone task stack size. */
#define THZONE_TASK_PRIORITY   (TK_PRIO_DEFAULT)      /*shzone task priority. */


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
extern int shzone_read(int fd, uint32_t addr, void *buff_ptr, uint16_t data_len);

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
extern int shzone_write(int fd, shzone_datatype_em type, uint32_t addr, void *buff_ptr, uint16_t data_len);

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
extern int shzone_init(int *fd_ptr, shzone_config_st_ptr cfg_ptr);

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
extern int shzone_deinit(int fd);


#endif

