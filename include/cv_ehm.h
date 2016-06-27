/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_ehm.h
 @brief  : ehm module header file
 @author : fred
 @history:
           2016-1-11    fred    Created file
           ...
******************************************************************************/


#ifndef __CV_EHM_MAIN_H__
#define __CV_EHM_MAIN_H__
#include <stdint.h>
#include "list.h"
#include "cv_osal.h"
#include "cv_vam.h"
#include "cv_rcp.h"
#include "app_msg_format.h"
#include "uart.h"


#define EHM_TX_LEN   256
#define EHM_RX_LEN   256

#define EHM_SRC_V2X		0
#define EHM_SRC_HOST	1


/* Reserved data head and tail in uart format. */
#define EHM_BUF_HEAD_RESERVED_LEN       (UART_MSG_HEADER_ST_LEN) 
#define EHM_BUF_TAIL_RESERED_LEN        2

/* Valid data in buffer. */
#define EHM_BUF_VALID_DATA_LEN          (EHM_TX_LEN - EHM_BUF_HEAD_RESERVED_LEN - EHM_BUF_TAIL_RESERED_LEN)



#define ANGLE(g) ((g)/PI*180.0f) 






/************************** ehm param *********************/

/* report node type enum */
typedef enum _NODE_TYPE {

    ALERT_NODE = 1,
    ALL_NODE
 
}NODE_TYPE_E;


/* neighbour node information type */
typedef enum _NODE_INFOR_TYPE {

    SUMMARY_INFO=1,
    DETAIL_INFO

}NODE_INFOR_TYPE_E;


/* ehm enum */
typedef enum _EHM_DATA_TYPE {

    ALERL_NODE_SUMMARY = 1,
    COMMON_NODE_SUMMARY =2,
    COMMON_NODE_DETAIL =3
} EHM_DATA_TYPE_E;

typedef enum _EHM_RECV_TYPE
{
	UART_RECV_TYPE = 1,
	ETH_RECV_TYPE = 2,

}EHM_RECV_TYPE_E;

typedef enum _V2X_MSG_TYPE 
{
	V2X_RESERVED ,
	V2X_NB_NODE_INFO,
	V2X_BASIC_VEHICLE_STATUS,
	V2X_FULL_VEHICLE_STATUS,
	V2X_VEHICLE_STATIC_INFO,
	V2X_LC_VEHICLE_ALERT_SET,
	V2X_NB_VEHICLE_ALERT,
	V2X_ROADSIZE_ALERT,
}V2X_MSG_TYPE_E;

/************************** ehm param *********************/



/* ehm module txbuf strtct follow by wnet module */
typedef struct _ehm_txinfo {

    EHM_DATA_TYPE_E data_type;
    uint8_t f_node_num;
    uint32_t data_systime;
    
}ehm_txinfo_t;



/* ehm module txbuf strtct follow by wnet module */
typedef struct _ehm_txbuf 
{
    /* Caution: Do not modify it.*/
    list_head_t list;
    ehm_txinfo_t info;
    /* END */
    
    uint32_t flag;   /* buffer's status */
    
    uint8_t *data_ptr;
    int32_t data_len;

    uint8_t buffer[EHM_TX_LEN];
}ehm_txbuf_t;



/* ehm buffer structure. */
typedef struct _ehm_buffer_st 
{
    /* Data buffer. */
    uint8_t  buffer[1024];

    /* Current effective data address and length. */
    uint8_t    * data_ptr;
    int32_t      data_len;
    
}ehm_buffer_st, *ehm_buffer_st_ptr;

#define EHM_BUFFER_ST_LEN    sizeof(ehm_buffer_st)


/* External host manage parameter config structure. */
typedef struct _ehm_config_st 
{
    EHM_RECV_TYPE_E        recv_type;
    comport_config_t  comport_config;


    
    NODE_TYPE_E     report_node_type;
    NODE_INFOR_TYPE_E node_info_type;
    
} ehm_config_st, * ehm_config_st_ptr;

#define EHM_CONFIG_ST_LEN    sizeof(ehm_config_st)


/* External host manage environment structure. */
typedef struct _ehm_envar_st 
{
    /* Pointer to ehm module's configurration structure. */
    ehm_config_st_ptr config_ptr;


    osal_queue_t * queue_main;
    osal_task_t   * task_main;

    osal_sem_t       * sem_tx;
    osal_task_t     * task_tx;

    osal_sem_t       * sem_rx;

    
    osal_task_t     * task_rx;
    ehm_buffer_st   buffer_rx;



}ehm_envar_st, * ehm_envar_st_ptr;

#define EHM_ENVAR_ST_LEN    sizeof(ehm_envar_st)



int inform_ehm_caculate_done(void);


#endif


