/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : drv_ltev.h
 @brief  : This file contain LTE-V module driver fucntions.
 @author : pengrui
 @history:
           2017-05-30    pengrui      Created file
           2017-06-09    wangxianwen  Optimization file structure.
           ...
******************************************************************************/

#ifndef CV_DRV_LTEV_H_
#define CV_DRV_LTEV_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include "error.h"


/* Ltev data transmit length range. */
#define LTEV_DATALENTH_MIN    (0)
#define LTEV_DATALENTH_MAX    (3000)


/* Ltev data infor structure. */
typedef struct _ltev_infor_st
{   
    uint8_t             ueid[4];  /* UE ID. */
    uint8_t             psid[4];  /* PS ID. */

    int8_t                   pw;  /* Send power. */
    uint8_t                 pri;  /* Priority. */
    uint8_t                  pi;  /* Period indicator. */
    uint8_t                  ci;  /* Channel indicator. */
    
}ltev_infor_st, * ltev_infor_st_ptr;

#define LTEV_INFOR_ST_LEN    (sizeof(ltev_infor_st))


/* Ltev configuration in device mode. */
typedef struct _ltev_device_cfg_st
{
    /* Interface name. */
    char     if_name[16];
    
}ltev_device_cfg_st, ltev_device_cfg_st_ptr;

#define LTEV_DEVICE_CFG_ST_LEN    (ltev_device_cfg_st)


/* Ltev configuration in socket mode. */
typedef struct _ltev_socket_cfg_st
{
    /* Interface name. */
    char     if_name[16];

    /* Host port and remote port. */
    uint16_t   host_port;
    uint16_t remote_port;

    /* Socket checksum configure. */
    uint8_t     checksum;
    
}ltev_socket_cfg_st, ltev_socket_cfg_st_ptr;

#define LTEV_SOCKET_CFG_ST_LEN    (ltev_socket_cfg_st)


/* Ltev active mode enumerate. */
typedef enum _ltev_mode_em
{
    LTEV_MODE_DEVICE  = 1,  /* Ltev as a device in project. */
    LTEV_MODE_SOCKET  = 2   /* Ltev as a socket connection in project. */
    
}ltev_mode_em, *ltev_mode_em_ptr;

#define LTEV_MODE_EM_LEN    (sizeof(ltev_mode_em))


/* Ltev configure structure. */
typedef struct _ltev_config_st
{  
    /* Device mode. */
    ltev_mode_em         dev_mode;

    /* Configuration in device and socket mode. */
    ltev_device_cfg_st device_cfg;
    ltev_socket_cfg_st socket_cfg;
    
}ltev_config_st, * ltev_config_st_ptr;

#define LTEV_CONFIG_ST_LEN    (sizeof(ltev_config_st))


/* Ltev file structure. */
typedef struct _drv_ltev_st
{   
    /* Device configuration. */
    ltev_config_st       config;

    /* Application file descriptor. */
    int                  fd_app;
    /* Control file descriptor. */
    int                 fd_ctrl; 

    /* Local network device ip address. */
    union
    {
        uint8_t         byte[4];
        uint32_t           word;  
        
    }local_ip;
    
}drv_ltev_st, * drv_ltev_st_ptr;

#define DRV_LTEV_ST_LEN    (sizeof(drv_ltev_st))



/* Ltev command that set send power structure. */
typedef struct _ltev_cmd_set_sdpower_st
{
    /* Node id. */
    uint8_t  node_id;

    /* Send power. */
    int16_t sd_power;
    
}ltev_cmd_set_sdpower_st, *ltev_cmd_set_sdpower_st_ptr;

#define LTEV_CMD_SET_SDPOWER_ST    (sizeof(ltev_cmd_set_sdpower_st))

/* Node id range. */
#define LTEV_CMD_SSDPOWER_NODEID_MIN     (LTEV_CSETSDPOWER_D6ID_MIN)
#define LTEV_CMD_SSDPOWER_NODEID_MAX     (LTEV_CSETSDPOWER_D6ID_MAX)

/* Send power range. */
#define LTEV_CMD_SSDPOWER_SDPOWER_MIN    (LTEV_CSETSDPOWER_D9SDPOWER_MIN)
#define LTEV_CMD_SSDPOWER_SDPOWER_MAX    (LTEV_CSETSDPOWER_D9SDPOWER_MAX)


/* Ltev command enumberaiton. */
typedef enum _ltev_cmd_em
{
    /* Ltev command: set send power. */
    LTEV_CMD_SET_SDPOWER = 0
    
}ltev_cmd_em, *ltev_cmd_em_ptr;

#define LTEV_CMD_EM_LEN    (sizeof(ltev_cmd_em))


/* Ltev command data union. */
typedef union _ltev_cmddata_un
{
    /* Command data for: LTEV_CMD_SET_SDPOWER. */
    ltev_cmd_set_sdpower_st sdpower;

}ltev_cmddata_un, *ltev_cmddata_un_ptr;

#define LTEV_CMDDATA_UN_LEN    (sizeof(ltev_cmddata_un))



/* Save all the compiler settings and off the optimization. */
#pragma pack(push)
#pragma pack(1)


/* Ltev data frame header structure. */
typedef struct _ltev_dheader_st
{
    uint8_t  ueid[4];  /* UE ID. */
    uint8_t  psid[4];  /* PS ID. */

    int8_t        pw;  /* Send power. */
    uint8_t      pri;  /* Priority. */
    uint8_t       pi;  /* Period indicator. */
    uint8_t       ci;  /* Channel indicator. */

    uint8_t length_l;  /* Payload length low byte. Unit: byte. */
    uint8_t length_h;  /* Payload length high byte. Unit: byte. */

    uint8_t       tm;  /* Test mode byte. */
    uint8_t       ts;  /* Test mode byte. */
    
}ltev_dheader_st, * ltev_dheader_st_ptr;

#define LTEV_DHEADER_ST_LEN    (sizeof(ltev_dheader_st))

/* Ltev data header pw range. */
#define LTEV_DHEADER_PW_MIN    (-30)
#define LTEV_DHEADER_PW_MAX    (30)

/* Ltev data header pri range. */
#define LTEV_DHEADER_PRI_MIN   (0)
#define LTEV_DHEADER_PRI_MAX   (7)

/* Ltev data header pi range. */
#define LTEV_DHEADER_PI_MIN    (0)
#define LTEV_DHEADER_PI_MAX    (10)
#define LTEV_DHEADER_PI_ONSHOT (0)

/* Ltev data header ci range. */
#define LTEV_DHEADER_CI_MIN    (0)
#define LTEV_DHEADER_CI_MAX    (1)

/* Ltev data header tm, ts data. */
#define LTEV_DHEADER_TM_DATA   (0)
#define LTEV_DHEADER_TS_DATA   (0)


/* Ltev control frame header structure. */
typedef struct _ltev_cheader_st
{
    /* Payload length. Unit: byte. */
    uint8_t length_l;
    uint8_t length_h;

    /* Fixed data 2. */
    uint8_t   d2_fix[2];

    /* Fixed data 3. */
    uint8_t   d3_fix[4];

    /* Reserved data. */
    uint8_t reserved[4];
    
}ltev_cheader_st, * ltev_cheader_st_ptr;

#define LTEV_CHEADER_ST_LEN    (sizeof(ltev_cheader_st))

/* Ltev control header d2_fix data content. */
#define LTEV_CHEADER_D2FIX_DATA0   (0xE1)
#define LTEV_CHEADER_D2FIX_DATA1   (0x00)

/* Ltev control header d3_fix data content. */
#define LTEV_CHEADER_D3FIX_DATA0   (0xD3)
#define LTEV_CHEADER_D3FIX_DATA1   (0x07)
#define LTEV_CHEADER_D3FIX_DATA2   (0x00)
#define LTEV_CHEADER_D3FIX_DATA3   (0x00)

/* Ltev control header reserved data content. */
#define LTEV_CHEADER_RESERV_DATA0  (0x55)
#define LTEV_CHEADER_RESERV_DATA1  (0xAA)
#define LTEV_CHEADER_RESERV_DATA2  (0x55)
#define LTEV_CHEADER_RESERV_DATA3  (0xAA)


/* Ltev control frame that set send power structure. */
typedef struct _ltev_cframe_set_sdpower_st
{
    /* Control frame header. */
    ltev_cheader_st ctrl_header;

    /* Fixed data 5. */
    uint8_t           d5_fix[2];

    /* Node id. */
    uint8_t               d6_id;
    
    /* Fixed data 7. */
    uint8_t           d7_fix[3];

    /* Fixed data 8. */
    uint8_t           d8_fix[2];

    /* Send power. Unit: dBm. */
    uint8_t        d9_sdpower_l;
    uint8_t        d9_sdpower_h;

    /* Fixed data 10. */
    uint8_t          d10_fix[2];
    
    /* Fixed data 11. */
    uint8_t          d11_fix[2];
    
    /* Fixed data 12. */
    uint8_t          d12_fix[2];
    
}ltev_cframe_set_sdpower_st, * ltev_cframe_set_sdpower_st_ptr;

#define LTEV_CFRAME_SET_SDPOWER_ST_LEN    (sizeof(ltev_cframe_set_sdpower_st))

/* Ltev control frame that set send power d5_fix data content. */
#define LTEV_CSETSDPOWER_D5FIX_DATA0   (0x00)
#define LTEV_CSETSDPOWER_D5FIX_DATA1   (0x00)

/* Ltev control frame that set send power d6_id data range. */
#define LTEV_CSETSDPOWER_D6ID_MIN      (0x00)
#define LTEV_CSETSDPOWER_D6ID_MAX      (0xFE)

/* Ltev control frame that set send power d7_fix data content. */
#define LTEV_CSETSDPOWER_D7FIX_DATA0   (0x00)
#define LTEV_CSETSDPOWER_D7FIX_DATA1   (0x00)
#define LTEV_CSETSDPOWER_D7FIX_DATA2   (0x00)

/* Ltev control frame that set send power d8_fix data content. */
#define LTEV_CSETSDPOWER_D8FIX_DATA0   (0x60)
#define LTEV_CSETSDPOWER_D8FIX_DATA1   (0x9F)

/* Ltev control frame that set send power d9_sdpower data content. */
#define LTEV_CSETSDPOWER_D9SDPOWER_MIN (-20)
#define LTEV_CSETSDPOWER_D9SDPOWER_MAX (20)

/* Ltev control frame that set send power d10_fix data content. */
#define LTEV_CSETSDPOWER_D10FIX_DATA0  (0x61)
#define LTEV_CSETSDPOWER_D10FIX_DATA1  (0x00)

/* Ltev control frame that set send power d11_fix data content. */
#define LTEV_CSETSDPOWER_D11FIX_DATA0  (0x01)
#define LTEV_CSETSDPOWER_D11FIX_DATA1  (0x00)

/* Ltev control frame that set send power d12_fix data content. */
#define LTEV_CSETSDPOWER_D12FIX_DATA0  (0xF4)
#define LTEV_CSETSDPOWER_D12FIX_DATA1  (0x01)


/* restore all compiler settings in stacks. */
#pragma pack(pop)


/******************************************************************************
*    Function: drv_ltev_send
*    Descriptions: Send data to the specific ltev driver.
*    Paramters:
            fd                -    the file descriptor.
            infor_ptr       -   data information.
            buff_ptr        -   buffer head address for data storage.
            data_len        -   the data length that user want to send.
*    Return:
            >= 0            -    the data count that have sended.
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int drv_ltev_send(int fd, ltev_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t data_len);

/******************************************************************************
*    Function: drv_ltev_receive
*    Descriptions: Receive data from the specific ltev driver.
*    Paramters:
            fd                -    the file descriptor.
            infor_ptr       -   data information.
            buff_ptr        -   buffer head address for data storage.
            data_len        -   the data count that user want to receive.
*    Return:
            >= 0            -    the data count that have received.
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int drv_ltev_receive(int fd, ltev_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t data_len);

/******************************************************************************
*    Function: drv_ltev_init
*    Descriptions: open the specific ltev device.
*    Paramters:
            fd_ptr          -   the pointer that hold file descriptor.
            cfg_ptr            -    the config paramters pointer.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int drv_ltev_init(int *fd_ptr, ltev_config_st_ptr cfg_ptr);

/******************************************************************************
*    Function: drv_ltev_deinit
*    Descriptions: Deinit the ltev fd device.
*    Paramters:
            fd                -    the file descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int drv_ltev_deinit(int fd);

#endif 
