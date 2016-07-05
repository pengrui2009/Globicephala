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

//#define RSU_TEST

/**
    prority of all the tasks in system 
*/
#define RT_SYS_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 15)
#define RT_VAM_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 22)
#define RT_VSA_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 23)
#define RT_MDA_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 19)
#define RT_GPS_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 21)
#define RT_HI_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 25)
#define RT_WNETTX_THREAD_PRIORITY   TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
#define RT_WNETRX_THREAD_PRIORITY   TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
#define EHM_MAIN_THREAD_PRIORITY	TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
#define EHM_RX_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)
#define EHM_TX_THREAD_PRIORITY		TK_PRIO_DEFAULT//(TK_PRIO_DEFAULT - 20)

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


    VSA_MSG_BASE = 0x0300,
    VSA_MSG_MANUAL_BC,   
    VSA_MSG_EEBL_BC,
    VSA_MSG_AUTO_BC,
    
    VSA_MSG_CFCW_ALARM,
    VSA_MSG_CRCW_ALARM,
    VSA_MSG_OPPOSITE_ALARM,
    VSA_MSG_SIDE_ALARM,

    VSA_MSG_ACC_RC,
    VSA_MSG_EEBL_RC,
    VSA_MSG_X_RC,
    VSA_MSG_XX_RC,
    VSA_MSG_XXX_RC,

    EHM_MSG_BASE = 0x0400,
    EHM_MSG_VSA_RECV_DONE,
    EHM_MSG_VSA_SEND_DATA,
    EHM_MSG_XXX
};

enum HI_OUT_TYPE{
    HI_OUT_NONE = 0,
    HI_OUT_SYS_INIT,
    HI_OUT_BSM_UPDATE,
    HI_OUT_BSM_NONE,
    HI_OUT_GPS_CAPTURED,
    HI_OUT_GPS_LOST,
    HI_OUT_CRD_ALERT,
    HI_OUT_CRD_CANCEL,
    HI_OUT_CRD_REAR_ALERT,
    HI_OUT_CRD_REAR_CANCEL,
    HI_OUT_VBD_ALERT,
    HI_OUT_VBD_CANCEL,
    HI_OUT_VBD_STATUS,
    HI_OUT_VBD_STOP,
    HI_OUT_EBD_ALERT,
    HI_OUT_EBD_CANCEL,
    HI_OUT_EBD_STATUS,
    HI_OUT_EBD_STOP,
    HI_OUT_CANCEL_ALERT,
};

enum HI_IN_TYPE{
    HI_IN_NONE = 0,
    HI_IN_KEY_PRESSED,
    HI_IN_KEY_RELEASE,
};


/**
    misc definitions 
*/
#define OS_TICK_PER_SECOND  (100)
#define MS_TO_TICK(n)     (n)
#define SECOND_TO_TICK(n) ((n)*1000)


/*****************************************************************************
 * declaration of structs                                                    *
*****************************************************************************/

/**
    structure of system global message 
*/
typedef struct _sys_msg{
    uint16_t id;
    uint16_t len;
    uint32_t argc; 
    void    *argv;
}sys_msg_t;

/**
    structure of system configure parameters 
*/
typedef struct _cfg_param{

	/*********************ID******************/	
    uint8_t pid[RCP_TEMP_ID_LEN];  // ID 

    /******************** VAM *********************/
    vam_config_t vam;

    /******************** VSA *********************/
    vsa_config_t vsa;
    
    /*********************WNET*************************/
    wnet_config_t wnet;
	
    /******************** DBG *********************/
    uint8_t print_xxx;  /* 0 - disable, 1 - enable */

                 
}cfg_param_t;

/** 
    structure of system manager module's environment variable 
*/
typedef struct _sys_envar{
    /* working_param */
    vsa_config_t working_param;

    uint32_t status;
    uint32_t hi_timer_cnt;

    uint32_t led_priority;
    uint16_t led_blink_duration;
    uint16_t led_blink_period;
    uint16_t led_blink_cnt;
	uint8_t voc_flag;

    /* os related */
    osal_task_t *task_sys_mng;
    osal_queue_t *queue_sys_mng;

}sys_envar_t;


/**
    structure of system global environment variable 
*/
typedef struct _cms_global{
    vam_envar_t vam;
    vsa_envar_t vsa;
    wnet_envar_t wnet;

    sys_envar_t sys;
    mda_envar_t mda;
}cms_global_t;

/*****************************************************************************
 * declare of global functions and variables                                 *
*****************************************************************************/
extern cms_global_t cms_envar, *p_cms_envar;
extern cfg_param_t cms_param, *p_cms_param;

osal_status_t sys_add_event_queue(sys_envar_t *p_sys, 
                             uint16_t msg_id, 
                             uint16_t msg_len, 
                             uint32_t msg_argc,
                             void    *msg_argv);

#endif /* __CV_CMS_DEF_H__ */

