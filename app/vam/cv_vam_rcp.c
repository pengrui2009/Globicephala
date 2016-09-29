/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_vam_rcp.c
 @brief  : this file realize the vehicle Remote Communicat Protocol
 @author : wangyifeng
 @history:
           2014-6-17    wangyifeng    Created file
           2014-7-29    wanglei       modified file: process evam msg 
           ...
******************************************************************************/
#include "cv_osal.h"
#define OSAL_MODULE_DEBUG
#define OSAL_MODULE_DEBUG_LEVEL OSAL_DEBUG_INFO
#define MODULE_NAME "rcp"
#include "cv_vam_api.h"
#include "cv_vam.h"
#include "cv_cms_def.h"
#include "cv_wnet.h"
#include "J2735.h"
#include "app_msg_format.h"


#include "MessageModuleSet.h"
#include "cv_msg_bsm.h"
#include "cv_msg_rsa.h"

#include "DSRCmsgIDEnum.h"


float rcp_dbg_distance = 0;



int rcp_mda_process(uint8_t msg_hops, uint8_t msg_count, uint8_t *p_temp_id, uint8_t *p_forward_id, uint8_t *data, uint32_t datalen)
{
    mda_msg_info_t src;
    mda_envar_t * p_mda;
    int ret;

    p_mda = &p_cms_envar->mda;
    src.left_hops = msg_hops;
    src.msg_count = msg_count;
    memcpy(src.temorary_id, p_temp_id, RCP_TEMP_ID_LEN);
    memcpy(src.forward_id, p_forward_id, RCP_TEMP_ID_LEN);
    
    ret = mda_handle(p_mda, &src, NULL, data, datalen);
    return ret;
}


/* Parse bsm message from others. */
int rcp_parse_bsm(vam_envar_t *p_vam, wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{
    bsm_msg_st_ptr msg_ptr = NULL;
    int             result = -1;


    if((msg_ptr = calloc(1, sizeof(*msg_ptr))) != NULL)
    {
        result = bsm_analyse_msg(msg_ptr, databuf, datalen);
        free(msg_ptr);    
    }
    
    return result;
}


int rcp_parse_eva(vam_envar_t *p_vam, wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{

#if 0

    vam_sta_node_t *p_sta;
    rcp_msg_emergency_vehicle_alert_t *p_evam;
    uint16_t alert_mask;

    if (datalen < sizeof(rcp_msg_emergency_vehicle_alert_t)){
        return -1;
    }

    p_evam = (rcp_msg_emergency_vehicle_alert_t *)databuf;

    if (0 == memcmp(p_evam->temporary_id, p_vam->local.pid, RCP_TEMP_ID_LEN)){
        return 0;
    }
    
//    rcp_mda_process(p_evam->msg_id.hops, p_evam->rsa.msg_count, 
//                     p_evam->temporary_id, p_evam->forward_id, databuf, datalen);


    //TBD
    alert_mask = decode_itiscode(p_evam->rsa.typeEvent, p_evam->rsa.description);
    //rt_kprintf("recv evam: alert_mask = 0x%04x\r\n", alert_mask);

    p_sta = vam_find_sta(p_vam, p_evam->temporary_id);
    if(p_sta != NULL)
    {
    	p_sta->s.dsecond = cv_ntohs(p_evam->rsa.time_stamp);
        p_sta->s.time = osal_get_systemtime();

        p_sta->s.pos.longitude = decode_longitude(p_evam->rsa.position.lon);
        p_sta->s.pos.latitude = decode_latitude(p_evam->rsa.position.lat);
        p_sta->s.pos.elevation = decode_elevation(p_evam->rsa.position.elev);

        p_sta->s.dir = decode_angle(p_evam->rsa.position.heading);
        p_sta->s.speed = decode_absolute_velocity(p_evam->rsa.position.speed.speed);
#if 0
        p_sta->s.acce_set.longitudinal = decode_acceleration(p_evam->motion.acce.lon);
        p_sta->s.acce_set.lateral = decode_acceleration(p_evam->motion.acce.lat);
        p_sta->s.acce_set.vertical = decode_vertical_acceleration(p_evam->motion.acce.vert);
        p_sta->s.acce_set.yaw_rate = decode_yawrate(p_evam->motion.acce.yaw);
#endif
        p_sta->s.alert_mask = alert_mask;

        /* inform the app layer once */
        if(p_vam->evt_handler[VAM_EVT_EVA_UPDATE] != NULL)
        {
            (p_vam->evt_handler[VAM_EVT_EVA_UPDATE])(&p_sta->s);
        }
    }
#endif
    
    return 0;
}


int rcp_parse_rsa(vam_envar_t *p_vam, wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{
    rsa_msg_st_ptr msg_ptr = NULL;
    int             result = -1;


    if((msg_ptr = calloc(1, sizeof(*msg_ptr))) != NULL)
    {
        result = rsa_analyse_msg(msg_ptr, databuf, datalen);
        free(msg_ptr);    
    }
    
    return result;    
}


int rcp_parse_msg(vam_envar_t *p_vam, wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{    
    MessageModuleSet_t  *mms_ptr = NULL;
    asn_dec_rval_t   decode_rval = { 0 };
    asn_codec_ctx_t   decode_ctx = { 0 };


    /* Decode into message module set for message parse. */
    decode_rval = uper_decode(&decode_ctx, &asn_DEF_MessageModuleSet,(void **)&mms_ptr, databuf, datalen, 0, 0);
    if(decode_rval.code != RC_OK)
    {
        printf("\n uper_decode() for MessageModuleSet_t is faild. \n ");
        return -1;
    }
    else
    {
        switch(mms_ptr->dsrcMsgId)
        {
            case DSRCmsgIDEnum_basicSafetyMessage:  {
                rcp_parse_bsm(p_vam, rxinfo, databuf, datalen);    break;  } 
                
            case DSRCmsgIDEnum_emergencyVehicleAlert: {
                rcp_parse_eva(p_vam, rxinfo, databuf, datalen);    break;  } 
                
            case DSRCmsgIDEnum_roadSideAlert:  {
                rcp_parse_rsa(p_vam, rxinfo, databuf, datalen);    break;  }
                
            default:              {
                                                                   break;  }   
        }
        
        return 0;
    }
}


/*****************************************************************************
 @funcname: vam_rcp_recv
 @brief   : RCP receive data frame from network layer
 @param   : wnet_rxinfo_t *rxinfo  
 @param   : uint8_t *databuf      
 @param   : uint32_t datalen      
 @return  : 
*****************************************************************************/
int vam_rcp_recv(wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{
    vam_envar_t *p_vam = &p_cms_envar->vam;


    //vam_add_event_queue(p_vam, VAM_MSG_RCPRX, datalen, (uint32_t)databuf, rxinfo);
    rcp_parse_msg(p_vam, rxinfo, databuf, datalen);
    return 0;
}



/* Init bsm message structure. */
void bsm_init_msg(bsm_msg_st_ptr msg_ptr, vam_stastatus_t_ptr vam_ptr)
{
    static uint8_t msg_count = 0;


    /* msgCnt. */
    msg_ptr->coreData.msgCnt = msg_count ++;
    if(127 < msg_count)
    {
        msg_count = 0;
    }

    /* id. */
    memcpy(msg_ptr->coreData.id, vam_ptr->pid, sizeof(msg_ptr->coreData.id));

    /* secMark. */
    msg_ptr->coreData.secMark = vam_ptr->dsecond;

    /* Position. */
    msg_ptr->coreData.latitude = vam_ptr->pos.latitude;
    msg_ptr->coreData.longitude = vam_ptr->pos.longitude;
    msg_ptr->coreData.elevation = vam_ptr->pos.elevation;

    /* accuracy. */
    msg_ptr->coreData.accuracy.semi_major_accu = vam_ptr->pos_accuracy.semi_major_accu;
    msg_ptr->coreData.accuracy.semi_minor_accu = vam_ptr->pos_accuracy.semi_minor_accu;
    msg_ptr->coreData.accuracy.semi_major_orientation = vam_ptr->pos_accuracy.semi_major_orientation;

    msg_ptr->coreData.trans = 


    



    msg_ptr->coreData.angle = 47.512;
}


/* Init rsa message structure. */
void eva_init_msg(bsm_msg_st_ptr msg_ptr, vam_stastatus_t_ptr vam_ptr)
{
    
}


/* Init rsa message structure. */
void rsa_init_msg(rsa_msg_st_ptr msg_ptr, vam_stastatus_t_ptr vam_ptr)
{
    
}










/* Send bsm message to others. */
int rcp_send_bsm(vam_envar_t *p_vam)
{
    int                       result = 0;
    wnet_txbuf_t              *txbuf = NULL;
    bsm_msg_st_ptr           msg_ptr = NULL;
    uint16_t               valid_bit = 0;


    /* Get tx buffer from memory. */
    if((txbuf = wnet_get_txbuf()) == NULL) 
    {
        return -1;
    }

    /* Allocate room for message and clear the memory. */
    if((msg_ptr = calloc(1, sizeof(*msg_ptr))) == NULL)
    {
        return -1;
    }


    /* Set bsm optional setting. */
    if(p_vam->flag & VAM_FLAG_TX_BSM_ALERT)
    {
        msg_ptr->partIIExt.opt.vecSafetyExt = MSG_OPTIONAL_YES;
        msg_ptr->partIIExt.vecSafetyExt.opt.events = MSG_OPTIONAL_YES;
    }
    

    /* Build bsm message. */
    result = bsm_build_msg(msg_ptr, txbuf->data_ptr, sizeof(txbuf->buffer) - (txbuf->data_ptr - txbuf->buffer), &valid_bit);
    if(result == 0)
    {
        txbuf->data_len = (valid_bit+7)/8;
    
        /* Set tx information structure. */
        memcpy(txbuf->info.dest.dsmp.addr, "\xFF\xFF\xFF\xFF\xFF\xFF", MACADDR_LENGTH);
        txbuf->info.dest.dsmp.aid = 0x00000020;
        txbuf->info.protocol = WNET_TRANS_PROT_DSMP;
        txbuf->info.encryption = WNET_TRANS_ENCRYPT_NONE;
        txbuf->info.prority = WNET_TRANS_RRORITY_NORMAL;
        txbuf->info.timestamp = osal_get_systimestamp();

        /* Send bsm message. */
        result = wnet_send(&(txbuf->info), (uint8_t *)txbuf->data_ptr, txbuf->data_len);
    }

    free(msg_ptr);
    
    wnet_release_txbuf(txbuf);

    return result;
}


int rcp_send_eva(vam_envar_t *p_vam)
{
    return 0;
}


int rcp_send_rsa(vam_envar_t *p_vam)
{
    int               result = 0;
    rsa_msg_st_ptr   msg_ptr = NULL;
    
    wnet_txbuf_t      *txbuf = NULL;
    uint16_t       valid_bit = 0;


    /* Get tx buffer from memory. */
    if((txbuf = wnet_get_txbuf()) == NULL) 
    {
        osal_printf("get txbuf failed line%d", __LINE__);
        return -1;
    }
  
    if((msg_ptr = calloc(1, sizeof(*msg_ptr))) == NULL)
    {
        return -1;
    }

    /* Build rsa message. */
    result = rsa_build_msg(msg_ptr, txbuf->data_ptr, sizeof(txbuf->buffer) - (txbuf->data_ptr - txbuf->buffer), &valid_bit);
    if(result == 0)
    {
        txbuf->data_len = (valid_bit+7)/8;
    
        /* Set tx information structure. */
        memcpy(txbuf->info.dest.dsmp.addr, "\xFF\xFF\xFF\xFF\xFF\xFF", MACADDR_LENGTH);
        txbuf->info.dest.dsmp.aid = 0x00000020;
        txbuf->info.protocol = WNET_TRANS_PROT_DSMP;
        txbuf->info.encryption = WNET_TRANS_ENCRYPT_NONE;
        txbuf->info.prority = WNET_TRANS_RRORITY_EMERGENCY;
        txbuf->info.timestamp = osal_get_systimestamp();

        /* Send bsm message. */
        result = wnet_send(&(txbuf->info), (uint8_t *)txbuf->data_ptr, txbuf->data_len);
    }

    wnet_release_txbuf(txbuf);
    
    return result;
    
}


int rcp_send_forward_msg(wnet_txbuf_t *txbuf)
{

#if 0
    wnet_txinfo_t *txinfo;
//    rcp_msgid_t *p_msgid;
  //  rcp_msg_basic_safty_t *p_bsm;
//    rcp_msg_emergency_vehicle_alert_t *p_evam;

    vam_envar_t *p_vam = &p_cms_envar->vam;
    
    txinfo = WNET_TXBUF_INFO_PTR(txbuf);
    memset(txinfo, 0, sizeof(wnet_txinfo_t));
    memcpy(txinfo->dest.dsmp.addr, "\xFF\xFF\xFF\xFF\xFF\xFF", MACADDR_LENGTH);
    txinfo->dest.dsmp.aid = 0x00000020;
    txinfo->protocol = WNET_TRANS_PROT_DSMP;
    txinfo->encryption = WNET_TRANS_ENCRYPT_NONE;
    txinfo->prority = WNET_TRANS_RRORITY_NORMAL;//WNET_TRANS_RRORITY_EMERGENCY;
    txinfo->timestamp = osal_get_systimestamp();

    /* modify the forward_id of msgdata */
//    p_msgid = (rcp_msgid_t *)(WNET_TXBUF_DATA_PTR(txbuf));
    if (DSRCmsgIDEnum_basicSafetyMessage == p_msgid->id){
 //       p_bsm = (rcp_msg_basic_safty_t *)WNET_TXBUF_DATA_PTR(txbuf);
 //       memcpy(p_bsm->forward_id, p_vam->local.pid, RCP_TEMP_ID_LEN);
    }
    else if(DSRCmsgIDEnum_emergencyVehicleAlert == p_msgid->id){
//        p_evam = (rcp_msg_emergency_vehicle_alert_t *)WNET_TXBUF_DATA_PTR(txbuf);
//        memcpy(p_evam->forward_id, p_vam->local.pid, RCP_TEMP_ID_LEN);    
    }
    else {
        return -1;
    }
 
 
    return wnet_send(txinfo, WNET_TXBUF_DATA_PTR(txbuf), txbuf->data_len);
#endif

    return 0;
 
}


wnet_txbuf_t *rcp_create_forward_msg(uint8_t left_hops, uint8_t *pdata, uint32_t length)
{

//    rcp_msgid_t *p_msg;
    wnet_txbuf_t *txbuf = NULL;

//    p_msg = (rcp_msgid_t *)pdata;
   // p_msg->hops = left_hops;

    txbuf = wnet_get_txbuf();
    if (!txbuf) {
        return NULL;
    }

    memcpy(WNET_TXBUF_DATA_PTR(txbuf), pdata, length);
    txbuf->data_len = length;
    
    return txbuf;

    
}



