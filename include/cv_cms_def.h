/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_cms_def.h
 @brief  : This file include the cms global definition 
 @author : wangyifeng
 @history:
           2014-6-16    wangyifeng    Created file
           ...
******************************************************************************/
#ifndef __CV_CMS_DEF_H__
#define __CV_CMS_DEF_H__

#include "cv_wnet.h"
#include "cv_vam.h"
#include "cv_vsa.h"
#include "cv_mda.h"
/*****************************************************************************
 * declaration of variables and functions                                    *
*****************************************************************************/


/**
    prority of all the tasks in system 
*/
//#define RT_SYS_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 15)
//#define RT_VAM_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 22)
//#define RT_VSA_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 23)
//#define RT_MDA_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 19)
//#define RT_GPS_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 21)
//#define RT_HI_THREAD_PRIORITY			TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 25)
//#define RT_WNETTX_THREAD_PRIORITY   		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
//#define RT_WNETRX_THREAD_PRIORITY   		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
//#define EHM_MAIN_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
//#define EHM_RX_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
//#define EHM_TX_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)

#define RT_SYS_THREAD_PRIORITY		86//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 15)
#define RT_VAM_THREAD_PRIORITY		78//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 22)
#define RT_VSA_THREAD_PRIORITY		77//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 23)
#define RT_MDA_THREAD_PRIORITY		81//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 19)
#define RT_GPS_THREAD_PRIORITY		79//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 21)
#define RT_HI_THREAD_PRIORITY		75//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 25)
#define RT_WNETTX_THREAD_PRIORITY   	80//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
#define RT_WNETRX_THREAD_PRIORITY   	80//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
#define EHM_MAIN_THREAD_PRIORITY	76//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
#define EHM_RX_THREAD_PRIORITY		70//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
#define EHM_TX_THREAD_PRIORITY		70//TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)

#define DEF_THREAD_STACK_SIZE   (1024*8192)

#define RT_SYS_THREAD_STACK_SIZE   DEF_THREAD_STACK_SIZE//(1024*8)
#define RT_GPS_THREAD_STACK_SIZE   DEF_THREAD_STACK_SIZE//(1024*8)
#define RT_MEMS_THREAD_STACK_SIZE  DEF_THREAD_STACK_SIZE//(1024*8)
#define RT_VAM_THREAD_STACK_SIZE   DEF_THREAD_STACK_SIZE//(1024*8)
#define RT_VSA_THREAD_STACK_SIZE   DEF_THREAD_STACK_SIZE//(1024*8)
#define RT_HI_THREAD_STACK_SIZE    DEF_THREAD_STACK_SIZE//(1024*8)
#define EHM_MAIN_THREAD_STACK_SIZE DEF_THREAD_STACK_SIZE//(1024*8)
#define EHM_RX_THREAD_STACK_SIZE   DEF_THREAD_STACK_SIZE//(1024*8)
#define	EHM_TX_THREAD_STACK_SIZE   DEF_THREAD_STACK_SIZE//(1024*8)

/**
    size of all the queue in system 
*/
#define SYS_QUEUE_SIZE 16
#define VAM_QUEUE_SIZE 20
#define VSA_QUEUE_SIZE 16
#define WNET_QUEUE_SIZE 20
#define EHM_QUEUE_SIZE (6)

/* per message len */
#define QUEUE_MSG_SIZE 256
#define VAM_MQ_MSG_SIZE 128
#define VSA_MQ_MSG_SIZE 128
#define SYS_MQ_MSG_SIZE 128
#define WNET_MQ_MSG_SIZE 2048
#define EHM_MQ_MSG_SIZE	128

enum SYSTEM_MSG_TYPE
{
    SYS_MSG_BASE = 0x0000,
    SYS_MSG_KEY_PRESSED,
    SYS_MSG_KEY_RELEASED,

    VAM_MSG_BASE = 0x0200,
    VAM_MSG_START,
    VAM_MSG_STOP,
    VAM_MSG_RCPTX,
    VAM_MSG_RCPRX,
    VAM_MSG_NEIGH_TIMEOUT,

    EHM_MSG_BASE = 0x0400,
    EHM_MSG_VSA_RECV_DONE,
    EHM_MSG_VSA_SEND_DATA,
    EHM_MSG_XXX
};


/**misc definitions */
#define OS_TICK_PER_SECOND  (100)
#define MS_TO_TICK(n)     (n)
#define SECOND_TO_TICK(n) ((n)*1000)


/*串口配置参数*/
typedef struct _comport_config
{
	uint8_t		verify;			//校验方式
	uint8_t		ndata;			//数据位位数
	uint8_t		nstop;			//停止位位数
	uint8_t		timeout;		//超时时间（单位100ms，为0时永久阻塞，0xff不阻塞）
	uint32_t	baud;			//波特率
	uint8_t		rtscts;			//是否使用rtscts流控信号线

} comport_config_t;

/* structure of system global message. */
typedef struct _sys_msg_t
{
    uint16_t   id;
    uint16_t  len;
    uint32_t argc; 
    void    *argv;
    
}sys_msg_t;


/*Structure of system configure parameters. */
typedef struct _cfg_param_t
{
    uint8_t pid[RCP_TEMP_ID_LEN];

    vam_config_t   vam;
    vsa_config_t   vsa;
    
    wnet_config_t wnet;
	
    uint8_t  print_xxx;  /* 0 - disable, 1 - enable */
            
}cfg_param_t, *cfg_param_t_ptr;

#define CFG_PARAM_T_LEN    (sizeof(cfg_param_t))


/* Structure of system global environment variable. */
typedef struct _cms_global_t
{
    vam_envar_t   vam;
    vsa_envar_t   vsa;
    wnet_envar_t wnet;

    mda_envar_t   mda;
    
}cms_global_t, * cms_global_t_ptr;

#define CMS_GLOBAL_T_LEN    (sizeof(cms_global_t))



extern cms_global_t cms_envar, *p_cms_envar;
extern cfg_param_t cms_param, *p_cms_param;

#endif

