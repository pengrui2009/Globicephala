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



/*****************************************************************************
 * declaration of variables and functions                                    *
*****************************************************************************/


/*****************************************************************************
 * implementation of functions                                               *
*****************************************************************************/
#ifndef __CV_EHM_MAIN_H__
#define __CV_EHM_MAIN_H__
#include <stdint.h>
#include "list.h"
#include "cv_osal.h"
#include "cv_vam.h"
#include "cv_rcp.h"
#include "app_msg_format.h"
//#include "arm_math.h"

#define EHM_TX_LEN   256
#define EHM_RX_LEN   256

#define EHM_SRC_V2X		0
#define EHM_SRC_HOST	1

#define EHM_BUF_NUM      5

#define EHM_BUF_HEAD_RESERVA_LEN       (MSG_VEHICLE_ALERT_ST_LEN)   //according to uart  

#define EHM_BUF_TRAIL_RESERVA_LEN       5   //according to uart 

#define EHM_BUF_NODE_INFO_LEN   (EHM_TX_LEN-EHM_BUF_HEAD_RESERVA_LEN-EHM_BUF_TRAIL_RESERVA_LEN)

#define EHM_BUF_SUMMARY_NODE_NUM   (EHM_BUF_NODE_INFO_LEN/NODE_SUMMARY_INFOR_ST_LEN)

#define EHM_BUF_DETAIL_NODE_NUM   (EHM_BUF_NODE_INFO_LEN/NODE_DETAIL_INFOR_ST_LEN)
#define EHM_BUF_DETAIL_NODE_SUMMARY_NUM   (((EHM_BUF_NODE_INFO_LEN-NODE_DETAIL_INFOR_ST_LEN)/NODE_SUMMARY_INFOR_ST_LEN)+1)



#define EHM_TXBUF_DATA_PTR(txbuf)  (txbuf->data_ptr)
#define EHM_TXBUF_INFO_PTR(txbuf)  (&(txbuf->info))

#define EHM_RXBUF_DATA_PTR(rxbuf)  (rxbuf->data_ptr)
#define EHM_RXBUF_INFO_PTR(rxbuf)  (&(rxbuf->info))
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

typedef enum _V2X_MSG_TYPE {
	VX2_RESERVED ,
	V2X_NB_NODE_INFO,
	V2X_BASIC_VEHICLE_STATUS,
	V2X_FULL_VEHICLE_STATUS,
	V2X_VEHICLE_STATIC_INFO,
	V2X_LC_VEHICLE_ALERT_SET,
	V2X_NB_VEHICLE_ALERT,
	V2X_ROADSIZE_ALERT,
}V2X_MSG_TYPE_E;
/* ehm param config */
typedef struct _ehm_config {

    NODE_TYPE_E report_node_type;
    NODE_INFOR_TYPE_E node_info_type;
    
} ehm_config_t;

/************************** ehm param *********************/



/* ehm module txbuf strtct follow by wnet module */
typedef struct _ehm_txinfo {

    EHM_DATA_TYPE_E data_type;
    uint8_t f_node_num;
    uint32_t data_systime;
    
}ehm_txinfo_t;



/* ehm module txbuf strtct follow by wnet module */
typedef struct _ehm_txbuf {
    /**
     * DO NOT MODIFY IT
     */
    list_head_t list;

    ehm_txinfo_t info;
    /**
     * END
     */
    uint32_t flag;   /* buffer's status */
    
    uint8_t *data_ptr;
    int32_t data_len;

    uint8_t buffer[EHM_TX_LEN];
}ehm_txbuf_t;

/* ehm module rxbuf strtct follow by wnet module */
typedef struct _ehm_rxinfo {

    EHM_RECV_TYPE_E recv_type;
//    uint8_t f_node_num;
    uint32_t data_systime;

}ehm_rxinfo_t;

/* ehm module rxbuf strtct follow by wnet module */
typedef struct _ehm_rxbuf {
    /**
     * DO NOT MODIFY IT
     */
    list_head_t list;

    ehm_rxinfo_t info;
    /**
     * END
     */

    uint32_t flag;   /* buffer's status */
    
    uint8_t *data_ptr;
    int32_t data_len;

    uint8_t buffer[EHM_RX_LEN];
}ehm_rxbuf_t;

typedef struct _comport_config{
	uint8_t		verify;			//校验方式
	uint8_t		ndata;			//数据位位数
	uint8_t		nstop;			//停止位位数
	uint8_t		timeout;		//超时时间（单位100ms，为0时永久阻塞，0xff不阻塞）
	uint32_t	baud;			//波特率
	uint8_t		rtscts;			//是否使用rtscts流控信号线
} comport_config_t;

typedef struct _ehm_envar {

    ehm_config_t *working_param;

    
    osal_queue_t * queue_ehm_main;
    osal_task_t   * task_ehm_main;

    osal_sem_t       * sem_ehm_tx;
    osal_task_t     * task_ehm_tx;

    osal_sem_t       * sem_ehm_rx;
    osal_task_t     * task_ehm_rx;

    
    list_head_t    txbuf_free_list;
    list_head_t txbuf_waiting_list;
    
    list_head_t    rxbuf_free_list;
    list_head_t rxbuf_waiting_list;


    ehm_txbuf_t txbuf[EHM_BUF_NUM];
    ehm_rxbuf_t rxbuf[EHM_BUF_NUM];

}ehm_envar_t;



void ehm_init(void);
int inform_ehm_caculate_done(void);
ehm_txbuf_t *ehm_get_txbuf(void);


#endif


