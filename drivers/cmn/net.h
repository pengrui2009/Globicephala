/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : net.h
 @brief  : This file contain NET module driver fucntions.
 @author : pengrui
 @history:
           2016-08-30    pengrui      Created file
           2017-10-26    wangxianwen  Optimization file structure.
           ...
******************************************************************************/

#ifndef CV_DRV_NET_H_
#define CV_DRV_NET_H_

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include "error.h"

#define NET_BLOCK           0x0
#define NET_NOBLOCK         0xFFFF

/* Net configure structure. */
typedef struct _net_config_st
{   
    //0:disable 1:enable
    int checksum;
    /* Host port and remote port. */
    uint16_t   host_port;
    uint16_t remote_port;
    
}net_config_st, * net_config_st_ptr;

#define NET_CONFIG_ST_LEN    (sizeof(net_config_st))

/* Net file structure. */
typedef struct _net_st
{   
    /* Configuration. */
    net_config_st config;
    
    /* Net file descriptor. */
    int               fd;
    
    /* Local network device ip address. */
    union
    {
        uint8_t  byte[4];
        uint32_t    word;  
    }local_ip;
    
}net_st, * net_st_ptr;

#define NET_ST_LEN    (sizeof(net_st))


/******************************************************************************
*    Function: net_config
*    Descriptions: config the specific net fd driver.
*    Paramters:
            fd                -    the file descriptor.
            cfg_ptr            -    the config paramters pointer.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int net_config(int fd, net_config_st_ptr cfg_ptr);

/******************************************************************************
*    Function: net_send
*    Descriptions: Send data to the specific net driver.
*    Paramters:
            fd                - the file descriptor.
            ipaddr            - the ipaddr of we send to
            flag              - send flag:BLOCK or NOBLOCK
            buff_ptr          - buffer head address for data storage.
            data_len          - the data length that user want to send.
*    Return:
            >= 0              - the data count that have sended.
            < 0               - failed
*    Comments: 
******************************************************************************/
extern int net_send(int fd, uint16_t flag, uint8_t *ipaddr, uint8_t *buff_ptr, uint16_t data_len);

/******************************************************************************
*    Function: net_receive
*    Descriptions: Receive data from the specific net driver.
*    Paramters:
            fd                   - the file descriptor.
            ipaddr               - the ipaddr of remote
            flag                 - the flag of recv:BLOCK or NOBLOCK
            buff_ptr             - buffer head address for data storage.
            data_len             - the data count that user want to receive.
*    Return:
            >= 0                 - the data count that have received.
            < 0                  - failed
*    Comments: 
******************************************************************************/
extern int net_receive(int fd, uint16_t flag, uint8_t *ipaddr, uint8_t *buff_ptr, uint16_t data_len);

/******************************************************************************
*    Function: net_init
*    Descriptions: open the specific net device.
*    Paramters:
            fd_ptr          -   the pointer that hold net file descriptor.
            name_ptr        -    the device character pointer.
            cfg_ptr            -    the config paramters pointer.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int net_init(int *fd_ptr, const char *name_ptr, net_config_st_ptr cfg_ptr);

/******************************************************************************
*    Function: net_deinit
*    Descriptions: Deinit the net fd device.
*    Paramters:
            fd                -    the file descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int net_deinit(int fd);

#endif
