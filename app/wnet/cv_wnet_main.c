/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_wnet_main.c
 @brief  : this file realize wireless network managment
 @author : wangyifeng
 @history:
           2014-6-17    wangyifeng    Created file
           2014-7-29    wanglei       modified file: process evam msg 
           ...
******************************************************************************/
#include "cv_osal.h"
#define OSAL_MODULE_DEBUG
#define OSAL_MODULE_DEBUG_LEVEL OSAL_DEBUG_INFO
#define MODULE_NAME "wnet"
#include "cv_osal_dbg.h"

#include "cv_vam.h"
#include "cv_cms_def.h"
#include "cv_wnet.h"
#include "cv_drv_vnet.h"

/*****************************************************************************
 * declaration of variables and functions                                    *
*****************************************************************************/
wnet_envar_t *p_wnet_envar = NULL;

/*****************************************************************************
 * implementation of functions                                               *
*****************************************************************************/
void * wnet_tx_thread_entry(void *parameter)
{
    int err;
    wnet_envar_t *p_wnet = (wnet_envar_t *)parameter;
    uint8_t data[WNET_MQ_MSG_SIZE];
    uint32_t len = 0;

    OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_TRACE, "%s: ---->\n", __FUNCTION__);
   
	while(1){
	    memset(data, 0, WNET_MQ_MSG_SIZE);      
	    err = osal_queue_recv(p_wnet->queue_frame_tx, data, &len, OSAL_WAITING_FOREVER);
        if (err == OSAL_STATUS_SUCCESS){
            //fp_send(p_wnet);
        }
        else{
            OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_ERROR, "%s:failed to take semaphore(%d)\n", \
                                __FUNCTION__, err);
        }
	}
}


uint8_t RxDataBuffer[WNET_MQ_MSG_SIZE] = { 0 };

void * wnet_rx_thread_entry(void *parameter)
{
    int err;
    int max_sock_fd;

    int sock_wifi_fd = 0;
    int sock_simu_fd = 0;
    sock_wifi_fd = g_wifi_envar.monitor_sock;
#ifdef VNET_SIMU_OPEN
    sock_simu_fd = g_simu_envar.monitor_sock;
#endif
    fd_set fd_read;
//    wnet_envar_t *p_wnet = (wnet_envar_t *)parameter;
    uint32_t len = 0;
    wnet_rxinfo_t rxinfo;
    OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_TRACE, "%s: ---->\n", __FUNCTION__);
    while(1){
		FD_ZERO(&fd_read);
		FD_SET(sock_wifi_fd,&fd_read);
#ifdef VNET_SIMU_OPEN
		FD_SET(sock_simu_fd,&fd_read);
#endif
		max_sock_fd = (sock_wifi_fd > sock_simu_fd)?sock_wifi_fd:sock_simu_fd;

	    err = select(max_sock_fd+1,&fd_read,NULL,NULL,NULL);//select 轮询查找网口数据状态
	    if(err>0)
	    {
	    	 memset(RxDataBuffer, 0, WNET_MQ_MSG_SIZE);
	    	 memset(&rxinfo, 0, sizeof(wnet_rxinfo_t));
			if(FD_ISSET(sock_wifi_fd,&fd_read))
			{
				err = drv_vnet_recv(&rxinfo, RxDataBuffer, &len);
				if (err == OSAL_STATUS_SUCCESS && len > 0){
					wnet_recv(&rxinfo, RxDataBuffer, len);
				}else{
					//OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_ERROR, "%s:failed to recv frame(err = %d,len = %d)\n", __FUNCTION__, err,len);
				}
			}
#ifdef VNET_SIMU_OPEN
			 memset(RxDataBuffer, 0, WNET_MQ_MSG_SIZE);
			 memset(&rxinfo, 0, sizeof(wnet_rxinfo_t));
			if(FD_ISSET(sock_simu_fd,&fd_read))
			{
				err = drv_simu_recv(&rxinfo,RxDataBuffer, &len);
				if(err == OSAL_STATUS_SUCCESS && len > 0)
				{
					wnet_recv(&rxinfo,RxDataBuffer,len);
					//vam_rcp_recv(&rxinfo, RxDataBuffer, len);
				}else{
						OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_ERROR, "%s:failed to recv frame(err = %d,len = %d)\n", __FUNCTION__, err,len);
				}
			}
#endif
	    }
     }
    return NULL;
}

extern void test_comm(void);
extern int cv_drv_wifi_init(void);

void timer_stat_callback(void* parameter)
{
//    wnet_envar_t *p_wnet = p_wnet_envar;
    //test_comm();
}

void wnet_init(void)
{
    int ret;
    ret = 0;
    wnet_envar_t *p_wnet;
    p_wnet_envar = &p_cms_envar->wnet;
    p_wnet = p_wnet_envar;

    memset(p_wnet, 0, sizeof(wnet_envar_t));
    memcpy(&p_wnet->working_param, &p_cms_param->wnet, sizeof(wnet_config_t));

#if 0
    /* Initialize the txbuf */
    for(i = 0;i< TXBUF_NUM;i++){
        memset(&p_wnet->txbuf[i], 0, sizeof(wnet_txbuf_t));
    }

    /* Initialize the rxbuf */
    for(i = 0;i< RXBUF_NUM;i++){
        memset(&p_wnet->rxbuf[i], 0, sizeof(wnet_rxbuf_t));
    }
#endif

    ret = drv_vnet_init(p_wnet);
    if(ret < 0)
    {
    	osal_printf("drv_vnet_init error:%d\n",ret);
    	return;
    }
#ifdef VNET_SIMU_OPEN
    ret = drv_simu_init();
	if(ret < 0)
	{
		osal_printf("drv_simu_init error:%d\n",ret);
		return;
	}
#endif
    /* os object for wnet */
#if 0
    p_wnet->queue_frame_rx = osal_queue_create("vnet_frame_rx", WNET_QUEUE_SIZE, WNET_MQ_MSG_SIZE);
    osal_assert(p_wnet->queue_frame_rx != NULL);

    p_wnet->queue_frame_tx = osal_queue_create("vnet_frame_tx", WNET_QUEUE_SIZE, WNET_MQ_MSG_SIZE);
    osal_assert(p_wnet->queue_frame_tx != NULL);



    p_wnet->task_wnet_tx = osal_task_create("wntx",
                           wnet_tx_thread_entry, p_wnet,
                           DEF_THREAD_STACK_SIZE, RT_WNETTX_THREAD_PRIORITY);
    osal_assert(p_wnet->task_wnet_tx != NULL);
#endif
/*    p_wnet->timer_stat = osal_timer_create("tm-stat", timer_stat_callback, p_wnet,\
        100, TIMER_INTERVAL , TIMER_PRIO_NORMAL);
   osal_assert(p_wnet->timer_stat != NULL);
*/
    p_wnet->task_wnet_rx = osal_task_create("wnrx",
                           wnet_rx_thread_entry, p_wnet,
                           DEF_THREAD_STACK_SIZE, RT_WNETRX_THREAD_PRIORITY);

    osal_assert(p_wnet->task_wnet_rx != NULL);
    
}

void wnet_deinit(void)
{
    drv_vnet_deinit();
}
