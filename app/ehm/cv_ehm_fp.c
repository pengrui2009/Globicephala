/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_ehm_fp.c
 @brief  : This file realizes the frame process
 @author : fred
 @history:
           2016-1-20    fred    Created file
           ...
******************************************************************************/
#include "cv_osal.h"
#define OSAL_MODULE_DEBUG
#define OSAL_MODULE_DEBUG_LEVEL OSAL_DEBUG_INFO
#define MODULE_NAME "ehm-fp"
#include "cv_osal_dbg.h"

#include "cv_ehm.h"
#include "cv_cms_def.h"

/*****************************************************************************
 * declaration of variables and functions                                    *
*****************************************************************************/
extern ehm_envar_t *p_ehm_envar;

/*****************************************************************************
 * implementation of functions                                               *
*****************************************************************************/
/*****************************************************************************
 @funcname: ehm_main_proc
 @brief   : ehm module main handle process
 @param   : ehm_envar_t * p_ehm
 @param   : sys_msg_t *p_msg
 @return  :
*****************************************************************************/
ehm_txbuf_t * ehm_fp_get_txbuf(ehm_envar_t *p_ehm)
{
    ehm_txbuf_t *txbuf = NULL;
    list_head_t *p_txbuf_free_list = &p_ehm->txbuf_free_list;

    osal_enter_critical();
    if (!list_empty(p_txbuf_free_list)) {
        txbuf = list_first_entry(p_txbuf_free_list, ehm_txbuf_t, list);
        list_del(&txbuf->list);
        txbuf->data_ptr = txbuf->buffer + EHM_BUF_HEAD_RESERVA_LEN;
        txbuf->data_len = 0;
        memset(&(txbuf->info), 0x00, sizeof(ehm_txinfo_t));
    }		
    osal_leave_critical();

    return txbuf;   
}
/*****************************************************************************
 @funcname: ehm_main_proc
 @brief   : ehm module main handle process
 @param   : ehm_envar_t * p_ehm
 @param   : sys_msg_t *p_msg
 @return  :
*****************************************************************************/
ehm_txbuf_t *ehm_get_txbuf(void)
{
    return ehm_fp_get_txbuf(p_ehm_envar);
}
/*****************************************************************************
 @funcname: ehm_main_proc
 @brief   : ehm module main handle process
 @param   : ehm_envar_t * p_ehm
 @param   : sys_msg_t *p_msg
 @return  :
*****************************************************************************/
ehm_rxbuf_t * ehm_fp_get_rxbuf(ehm_envar_t *p_ehm)
{
    ehm_rxbuf_t *rxbuf = NULL;
    list_head_t *p_rxbuf_free_list = &p_ehm->rxbuf_free_list;

    osal_enter_critical();
    if (!list_empty(p_rxbuf_free_list)) {
        rxbuf = list_first_entry(p_rxbuf_free_list, ehm_rxbuf_t, list);
        list_del(&rxbuf->list);
        rxbuf->data_ptr = rxbuf->buffer + EHM_BUF_HEAD_RESERVA_LEN;
        rxbuf->data_len = 0;
        memset(&(rxbuf->info),0x00,sizeof(ehm_rxinfo_t));
    }
    osal_leave_critical();

    return rxbuf;
}
/*****************************************************************************
 @funcname: ehm_main_proc
 @brief   : ehm module main handle process
 @param   : ehm_envar_t * p_ehm
 @param   : sys_msg_t *p_msg
 @return  :
*****************************************************************************/
ehm_txbuf_t *ehm_get_rxbuf(void)
{
    return ehm_fp_get_rxbuf(p_ehm_envar);
}
