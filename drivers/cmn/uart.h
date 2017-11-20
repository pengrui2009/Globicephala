/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : uart.h
 @brief  : This file provider the function of the driver of uart.
 @author : pengrui
 @history:
           2016-08-30    pengrui      Created file
           2017-10-24    wangxianwen  Optimize the structure.

           ...
******************************************************************************/

#ifndef _UART_H_
#define _UART_H_

#include <stdio.h>
#include <stdint.h>


/* Com port verify mode definition. */
typedef enum _comport_verify_em
{
    COMPORT_VERIFY_NO   = 0,  /* No verify. */
    COMPORT_VERIFY_EVEN = 1,  /* Even verify. */
    COMPORT_VERIFY_ODD  = 2   /* Odd verify. */ 
    
}comport_verify_em, *comport_verify_em_ptr;


/* Com port buffer type definition. */
typedef enum _comport_buff_em
{
    COMPORT_BUFF_ALL = 0,  /* Read buffer and write buffer. */
    COMPORT_BUFF_RD  = 1,  /* Read buffer only. */
    COMPORT_BUFF_WR  = 2   /* Write buffer only. */
    
}comport_buff_em, *comport_buff_em_ptr;


/* Com port hardware flow control mode definition. */
typedef enum _comport_flowctl_em
{
    COMPORT_RTSCTS_DISABLE = 0,  /* Disable hardware flow control. */
    COMPORT_RTSCTS_ENABLE  = 1,  /* Enable hardware flow control. */   
    
}comport_flowctl_em, *comport_flowctl_em_ptr;


/* Uart config parameters. */
typedef struct _uart_config_st
{
    /* Verify mode. */
    comport_verify_em    verify;

    /* Data bit. */
    uint8_t                 ndata;
    
    /* Stop bit. */
    uint8_t                 nstop;

    /* Timeout mode. Unit 100ms, 0x00 Block forever, 0xFF No block forever. */
    uint8_t               timeout;

    /* Baudrate. */
    uint32_t              baud;

    /* Hardware flow control. */
    comport_flowctl_em    rtscts;

} uart_config_st, *uart_config_st_ptr;


/******************************************************************************
*    Function: comport_config
*    Descriptions: config the specific uart fd device.
*    Paramters:
            fd                -    the file descriptor.
            cfg_ptr            -    the config paramters pointer.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int comport_config (int fd, uart_config_st_ptr cfg_ptr);

/******************************************************************************
*    Function: comport_send
*    Descriptions: Send data to the specific uart device.
*    Paramters:
            fd                -    the file descriptor.
            buff_ptr        -   buffer head address for data storage.
            count           -   the data count that user want to send.
*    Return:
            >= 0            -    the data count that have received.
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int comport_send (int fd, uint8_t *buff_ptr, uint32_t count);

/******************************************************************************
*    Function: comport_receive
*    Descriptions: Receive data from the specific uart device.
*    Paramters:
            fd                -    the file descriptor.
            buff_ptr        -   buffer head address for data storage.
            count           -   the data count that user want to receive.
*    Return:
            >= 0            -    the data count that have received.
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int comport_receive (int fd, uint8_t *buff_ptr, uint32_t count);

/******************************************************************************
*    Function: comport_flush
*    Descriptions: Flush the specific uart buffer.
*    Paramters:
            fd                -    the file descriptor.
            mode            -   buffer mode.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int comport_flush (int fd, comport_buff_em mode);

/******************************************************************************
*    Function: comport_init
*    Descriptions: open the specific uart device.
*    Paramters:
            fd_ptr          -   the pointer that hold uart file descriptor.
            name_ptr        -    the device character pointer.
            cfg_ptr            -    the config paramters pointer.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int comport_init (int *fd_ptr, const char *name_ptr, uart_config_st_ptr cfg_ptr);

/******************************************************************************
*    Function: comport_deinit
*    Descriptions: Deinit the uart fd device.
*    Paramters:
            fd                -    the file descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int comport_deinit(int fd);


#endif
