/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_sys_manager.c
 @brief  : this file include the system manage functions
 @author : wangyifeng
 @history:
           2014-6-20    wangyifeng    Created file
           ...
******************************************************************************/

#include "cv_osal.h"

#define OSAL_MODULE_DEBUG
#define OSAL_MODULE_DEBUG_LEVEL OSAL_DEBUG_INFO
#define MODULE_NAME "sysc"
#include "cv_osal_dbg.h"
OSAL_DEBUG_ENTRY_DEFINE(sysc);

#include "cv_vam.h"
#include "cv_cms_def.h"
#include "cv_vsa.h"


#define HUMAN_ITERFACE_DEFAULT         (500)
#define HUMAN_ITERFACE_VOC             (3000)
#define HUMAN_ITERFACE_GPS_VOC         (5000)
#define BREATH_CYCLE                   300 


#define AUDIO_START_ADDRESS     58 /* Offset relative to audio file header size */

/*****************************************************************************
 * declaration of variables and functions                                    *
*****************************************************************************/
extern int param_set(uint8_t param, int32_t value);

/*****************************************************************************
 * implementation of functions                                               *
*****************************************************************************/


osal_status_t sys_add_event_queue(sys_envar_t *p_sys, 
                             uint16_t msg_id, 
                             uint16_t msg_len, 
                             uint32_t msg_argc,
                             void    *msg_argv)
{
    int err = OSAL_STATUS_NOMEM;
    sys_msg_t *p_msg;
    uint32_t len = sizeof(sys_msg_t);
    p_msg = (sys_msg_t *)osal_malloc(len);
    if (p_msg) {
        p_msg->id = msg_id;
        p_msg->len = msg_len;
        p_msg->argc = msg_argc;
        p_msg->argv = msg_argv;
        err = osal_queue_send(p_sys->queue_sys_mng, p_msg, len, 0, 0);
    }

    if (err != OSAL_STATUS_SUCCESS) {
        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_WARN, "%s: failed=[%d], msg=%04x\n",\
                           __FUNCTION__, err, msg_id);
        osal_free(p_msg);
    }

    return err;
}


void sys_manage_proc(sys_envar_t *p_sys, sys_msg_t *p_msg)
{
    switch(p_msg->id){
    case SYS_MSG_INITED:
        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_TRACE, "%s: initialize complete\n", __FUNCTION__);
        vam_start();
        vsa_start();

        break;
        
    default:
        break;
    }
}

void * sysc_thread_entry(void *parameter)
{
    int err;
    sys_msg_t *p_msg;
    sys_envar_t *p_sys = (sys_envar_t *)parameter;

    uint32_t len = 0;
    uint8_t buf[SYS_MQ_MSG_SIZE];
    p_msg = (sys_msg_t *)buf;

    while(1){
        memset(buf, 0, SYS_MQ_MSG_SIZE);        
        err = osal_queue_recv(p_sys->queue_sys_mng, buf, &len, OSAL_WAITING_FOREVER);
        if (err == OSAL_STATUS_SUCCESS){
            sys_manage_proc(p_sys, p_msg);
        }
        else{
            OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_ERROR, "%s: osal_queue_recv error [%d]\n", __FUNCTION__, err);
        }
    }
}


void sys_init(void)
{
    sys_envar_t *p_sys = &p_cms_envar->sys;

    
    /* object for sys */
    p_sys->queue_sys_mng = osal_queue_create("q-sys", SYS_QUEUE_SIZE, SYS_MQ_MSG_SIZE);
    osal_assert(p_sys->queue_sys_mng != NULL);

    p_sys->task_sys_mng = osal_task_create("t-sys",
                           sysc_thread_entry, p_sys,
                           RT_SYS_THREAD_STACK_SIZE, RT_SYS_THREAD_PRIORITY);
    osal_assert(p_sys->task_sys_mng != NULL);

    OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_INFO, "module initial\n");
}

