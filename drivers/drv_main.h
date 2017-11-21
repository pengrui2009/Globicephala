/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : drv_main.h
 @brief  : This file contain all the modules driver fucntions.
 @author : wangxianwen
 @history:
           2017-11-21    wangxianwen  Create files.
           ...
******************************************************************************/

#ifndef _DRV_MAIN_H_
#define _DRV_MAIN_H_

#include "drv_audio.h"
#include "drv_barometer.h"
#include "drv_ltev.h"


/* Driver enable configure. */
#define DRV_ENABLE_AUDIO     0
#define DRV_ENABLE_BAROMETER 0
#define DRV_ENABLE_LTEV      1


/* Ltev configure. */
#if(DRV_ENABLE_AUDIO != 0)

/* audio_config_st:  */
#define DRV_AUDIO_CFG  { 0 }

#endif

/* Barometer configure. */
#if(DRV_ENABLE_BAROMETER != 0)

/* barometer_config_st:  */
#define DRV_BAROMETER_CFG  { 0 }

#endif

/* Ltev configure. */
#if(DRV_ENABLE_LTEV != 0)

/* ltev_config_st:        ltev_mode_em,  device_cfg,      socket_cfg. */
#define DRV_LTEV_CFG  { LTEV_MODE_SOCKET, {"lm30"}, {"eth0", 2000, 2001, 1} }

#endif


/* Driver main group structure. */
typedef struct _drv_main_st
{
#if(DRV_ENABLE_AUDIO != 0)
    /* Audio file descriptor. */
    int       audio_fd;
#endif

#if(DRV_ENABLE_BAROMETER != 0)
    /* Barometer file descriptor. */
    int   barometer_fd;
#endif

#if(DRV_ENABLE_LTEV != 0)
    /* Ltev file descriptor. */
    int        ltev_fd;
#endif

}drv_main_st, *drv_main_st_ptr;

#define DRV_MAIN_ST_LEN    (sizeof(drv_main_st))


/******************************************************************************
*    Function: drv_main_init
*    Descriptions: Main routine that open the device group.
*    Paramters:
            drv_ptr_ptr        -    the pointer that hold driver point descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int drv_main_init(drv_main_st_ptr *drv_ptr_ptr);

/******************************************************************************
*    Function: drv_main_deinit
*    Descriptions: Main routine that close the device group.
*    Paramters:
            drv_ptr            -    the pointer that hold driver descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int drv_main_deinit(drv_main_st_ptr drv_ptr);

#endif

