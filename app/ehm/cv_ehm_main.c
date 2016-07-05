/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_ehm_main.c
 @brief  : this file include function for extern host manager module
 @author : fred
 @history:
           2016-1-11    fred    Created file
           ...
******************************************************************************/
#include "cv_osal.h"
#define OSAL_MODULE_DEBUG
#define OSAL_MODULE_DEBUG_LEVEL OSAL_DEBUG_INFO
#define MODULE_NAME "ehm"
#include "cv_osal_dbg.h"

//#include "components.h"
#include "cv_cms_def.h"
#include "cv_vsa.h"
#include "cv_vam.h"
//#include "cv_drv_file.h"
#include "cv_ehm.h"
//#include "drv_wnet.h"
#include "cv_rcp.h"
#include "J2735.h"
/*****************************************************************************
 * declaration of variables and functions                                    *
*****************************************************************************/

/* need add to global param  later*/
ehm_config_st ehm_config = 
{ 
    UART_RECV_TYPE,
        
    { COMPORT_VERIFY_NO, 8, 1, 0, 115200, COMPORT_RTSCTS_DISABLE },
    
    V2X_NB_NODE_SUMMRAY_INFO|V2X_NB_NODE_DETAIL_INFO|V2X_BASIC_VEHICLE_STATUS|V2X_NB_VEHICLE_ALERT|V2X_ROADSIZE_ALERT
};

ehm_envar_st   ehm_envar = { &ehm_config, 0 };

/*****************************************************************************
 @funcname: ehm_add_event_queue
 @brief   : ehm module main queue event add function
 @param   : vam_envar_t *p_vam  
 @param   : uint16_t msg_id     
 @param   : uint16_t msg_len    
 @param   : uint32_t msg_argc   
 @param   : void    *msg_argv   
 @return  : 
*****************************************************************************/
int ehm_add_event_queue(ehm_envar_st *p_ehm, uint16_t msg_id, uint16_t msg_len, uint32_t msg_argc, void *msg_argv)
{
    int       err = OSAL_STATUS_NOMEM;
    sys_msg_t msg = { msg_id, msg_len, msg_argc, msg_argv };


    /* Send message to tx task. */
    err = osal_queue_send(p_ehm->queue_tx, &msg, sizeof(msg), 0, OSAL_NO_WAIT);
    if(err != OSAL_STATUS_SUCCESS) 
    {
        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_WARN, "%s: failed = [%d], msg = %04x. \n", __FUNCTION__, err, msg_id);
    }

    return err;
}


/* Message group.----------------------------------------------------------- */


ehm_buffer_st * ehm_get_txbuf(ehm_envar_st * p_ehm)
{
	ehm_buffer_st *txbuf = &p_ehm->buffer_tx;
	memset(txbuf->buffer, 0x0, sizeof(txbuf->buffer));
	txbuf->data_len = 0;
	if(p_ehm->config_ptr->recv_type == UART_RECV_TYPE)
	{
		txbuf->data_ptr = txbuf->buffer + UART_MSG_HEADER_ST_LEN;
		//txbuf->data_len = UART_MSG_HEADER_ST_LEN + 2;//2byte CRC
	}else{
		txbuf->data_ptr = txbuf->buffer;
	}

	return txbuf;
}

/*****************************************************************************
 @funcname: nb_node_summary_infor
 @brief   : neighbour node summary infor message.
 @param   : 
 @param   : 
 @return  :
*****************************************************************************/
static int8_t encode_nb_node_summary_infor(ehm_envar_st * p_ehm, vam_envar_t *p_vam, vsa_envar_t *p_vsa)
{
	ehm_buffer_st * txbuf = NULL;
    frame_msg_header_st_ptr          msg_head_ptr = NULL;
    msg_vehicle_nb_status_st_ptr      nb_node_ptr = NULL;
    nb_node_summary_infor_st_ptr node_summary_ptr = NULL;

    vam_sta_node_t *p_sta = NULL;
    vsa_info_t vsa_position = { { 0 }, 0 };

    vsa_crd_node_t *p_alert_node = NULL;


    /* Get tx buffer from ehm tx buffer list. */
    txbuf = ehm_get_txbuf(p_ehm);
//    if (txbuf == NULL)
//    {
//        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_INFO, "%s can not get ehm tx buf",__FUNCTION__);
//        return -1;
//    }
    
    /* Initial message header. */
    msg_head_ptr = (frame_msg_header_st_ptr)txbuf->data_ptr;
    msg_head_ptr->mark = 0x0E;
    msg_head_ptr->src = 0;
    msg_head_ptr->reserved1 = 0;
    msg_head_ptr->reserved2 = 0;
    msg_head_ptr->type = MSGTYPE_V2X_APPLY;

    /* Initial message body 1. */
    nb_node_ptr = (msg_vehicle_nb_status_st_ptr)(txbuf->data_ptr + FRAME_MSG_HEADER_ST_LEN);
    nb_node_ptr->msg_id = MSGID_NBNODE_INFO;
    nb_node_ptr->system_time = cv_ntohl(osal_get_systemtime());
    nb_node_ptr->nodenumber = 0;
    nb_node_ptr->node_infor_type = NODE_INFOR_TYPE_SUMMARY;

    /* Set data length. */
    txbuf->data_len += FRAME_MSG_HEADER_ST_LEN + MSG_VEHICLE_NB_STATUS_ST_LEN;

    /* Initial message body 2. */ 
    node_summary_ptr = (nb_node_summary_infor_st_ptr)(txbuf->data_ptr + FRAME_MSG_HEADER_ST_LEN + MSG_VEHICLE_NB_STATUS_ST_LEN);

    osal_sem_take(p_vam->sem_sta, OSAL_WAITING_FOREVER);

    list_for_each_entry(p_sta, vam_sta_node_t, &p_vam->neighbour_list, list)
    {
        /* pid */
        memcpy(node_summary_ptr->node_id, p_sta->s.pid, RCP_TEMP_ID_LEN);

        /* relative position data */
        if (vsa_position_get(p_sta->s.pid, &vsa_position) == 0) 
        {
            if( (vsa_position.vsa_location == SLICE0_000_0T022_5) || (vsa_position.vsa_location == SLICE1_022_5T045_0) 
             || (vsa_position.vsa_location == SLICE2_045_0T067_5) || (vsa_position.vsa_location == SLICE3_067_5T090_0) 
             || (vsa_position.vsa_location == SLICE12_270_0T292_5)|| (vsa_position.vsa_location == SLICE13_292_5T315_0) 
             || (vsa_position.vsa_location == SLICE14_315_0T337_5)|| (vsa_position.vsa_location == SLICE15_337_5T360_0) )
            {
                node_summary_ptr->longitudinal_dis = - vsa_position.v_offset;
            }
            else
            {
                node_summary_ptr->longitudinal_dis = vsa_position.v_offset;
            }

            if( (vsa_position.vsa_location == SLICE0_000_0T022_5) || (vsa_position.vsa_location == SLICE1_022_5T045_0) 
             || (vsa_position.vsa_location == SLICE2_045_0T067_5) || (vsa_position.vsa_location == SLICE3_067_5T090_0) 
             || (vsa_position.vsa_location == SLICE4_090_0T112_5)|| (vsa_position.vsa_location == SLICE5_112_5T135_0) 
             || (vsa_position.vsa_location == SLICE6_135_0T157_5)|| (vsa_position.vsa_location == SLICE7_157_5T180_0) )
            {
                node_summary_ptr->lateral_dis = vsa_position.h_offset;
            }
            else
            {
                node_summary_ptr->lateral_dis = - vsa_position.h_offset;
            }
               
            node_summary_ptr->longitudinal_dis = cv_ntohs(node_summary_ptr->longitudinal_dis);
            node_summary_ptr->lateral_dis = cv_ntohs(node_summary_ptr->lateral_dis);
        }
        else 
        {
            node_summary_ptr->lateral_dis = 0;
            node_summary_ptr->longitudinal_dis = 0;
        }

        /* angle. */
        node_summary_ptr->angle = encode_angle(vsa_position.relative_dir);
        node_summary_ptr->velocity = encode_relative_velocity(vsa_position.relative_speed);
        node_summary_ptr->signalstrength = 0;
        node_summary_ptr->losstolerance = 0;

        /* alert flag. */
        if(list_empty(&p_vsa->crd_list))
        {
            /* Set alert flag to none when crd list empty. */
            node_summary_ptr->alert_flag.alert_word = 0;
        }
        else
        {
            list_for_each_entry(p_alert_node, vsa_crd_node_t, &p_vsa->crd_list, list) 
            {
                if(memcmp(node_summary_ptr->node_id, p_alert_node->pid, RCP_TEMP_ID_LEN) == 0)
                {   
                    /* Set alert flag when find the specific node and cancel the loop.  */
                	//待完善代码，告警标识不对
                    node_summary_ptr->alert_flag.alert_word = cv_ntohl(p_alert_node->alert_flag);
                    break;
                }
                else
                {
                    /* Set alert flag to none when no specific node. */
                    node_summary_ptr->alert_flag.alert_word = 0;
                }
            }
        }       
        
        /* Update data length and node number. */
        txbuf->data_len += NB_NODE_SUMMARY_INFOR_ST_LEN;
        nb_node_ptr->nodenumber ++;
        
        node_summary_ptr ++;

        /* Stop the loop when no enough room for node infor. */
        if((EHM_BUF_VALID_DATA_LEN / NB_NODE_SUMMARY_INFOR_ST_LEN) <= nb_node_ptr->nodenumber)
        {
            break;
        }
    }  
    
    osal_sem_release(p_vam->sem_sta);
    return 0;    
    
}


/*****************************************************************************
 @funcname: nb_node_detail_infor
 @brief   : neighbour node detail infor message.
 @param   : 
 @param   : 
 @return  :
*****************************************************************************/
static int8_t encode_nb_node_detail_infor(ehm_envar_st * p_ehm, vam_envar_t *p_vam, vsa_envar_t *p_vsa)
{
	ehm_buffer_st * txbuf = NULL;
        
    frame_msg_header_st_ptr          msg_head_ptr = NULL;
    msg_vehicle_nb_status_st_ptr      nb_node_ptr = NULL;
    nb_node_detail_infor_st_ptr   node_detail_ptr = NULL;

    vam_sta_node_t *p_sta = NULL;
    vsa_crd_node_t *p_alert_node = NULL;

    /* Get tx buffer from ehm tx buffer list. */
    txbuf = ehm_get_txbuf(p_ehm);
    if (txbuf == NULL) 
    {
        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_INFO, "%s can not get ehm tx buf",__FUNCTION__);
        return -1;
    }
    
    /* Initial message header. */
    msg_head_ptr = (frame_msg_header_st_ptr)txbuf->data_ptr;
    msg_head_ptr->mark = 0x0E;
    msg_head_ptr->src = 0;
    msg_head_ptr->reserved1 = 0;
    msg_head_ptr->reserved2 = 0;
    msg_head_ptr->type = MSGTYPE_V2X_APPLY;

    /* Initial message body 1. */
    nb_node_ptr = (msg_vehicle_nb_status_st_ptr)(txbuf->data_ptr + FRAME_MSG_HEADER_ST_LEN);
    nb_node_ptr->msg_id = MSGID_NBNODE_INFO;
    nb_node_ptr->system_time = cv_ntohl(osal_get_systemtime());
    nb_node_ptr->nodenumber = 0;
    nb_node_ptr->node_infor_type = NODE_INFOR_TYPE_DETAIL;

    /* Set data length. */
    txbuf->data_len += FRAME_MSG_HEADER_ST_LEN + MSG_VEHICLE_NB_STATUS_ST_LEN;

    /* Initial message body 2. */ 
    node_detail_ptr = (nb_node_detail_infor_st_ptr)(txbuf->data_ptr + FRAME_MSG_HEADER_ST_LEN + MSG_VEHICLE_NB_STATUS_ST_LEN);

    osal_sem_take(p_vam->sem_sta, OSAL_WAITING_FOREVER);

    list_for_each_entry(p_sta, vam_sta_node_t, &p_vam->neighbour_list, list)
    {
        /* pid */
        memcpy(node_detail_ptr->node_id, p_sta->s.pid, RCP_TEMP_ID_LEN);

        /* position 3d. */
        node_detail_ptr->pos3d.latitude = encode_latitude(p_sta->s.pos.lat);
        node_detail_ptr->pos3d.longitude = encode_longitude(p_sta->s.pos.lon);
        node_detail_ptr->pos3d.elevation = encode_elevation(p_sta->s.pos.elev);

        /* position accuracy. */
        node_detail_ptr->posaccu.semimajoraxisAccu = encode_semimajor_axis_accuracy(p_sta->s.pos.accu.semi_major_accu);
        node_detail_ptr->posaccu.semiminoraxisAccu = encode_semiminor_axis_accuracy(p_sta->s.pos.accu.semi_minor_accu);
        node_detail_ptr->posaccu.semimajorAxisOrien = encode_semimajor_axis_orientation(p_sta->s.pos.accu.semi_major_orientation);

        /* velocity. */
        node_detail_ptr->velocity = encode_absolute_velocity(p_sta->s.speed);

        /* angle. */
        node_detail_ptr->angle = encode_angle(p_sta->s.dir);

        /* steer wheel angle. */
        node_detail_ptr->steer_wheel_angle = encode_steer_wheel_angle(p_sta->s.steer_wheel_angle);

        /* acceleration set 4 way. */  
        node_detail_ptr->acc.lonacce = encode_acceleration(p_sta->s.acce.lon);
        node_detail_ptr->acc.latacce = encode_acceleration(p_sta->s.acce.lat);
        node_detail_ptr->acc.veracce = encode_vertical_acceleration(p_sta->s.acce.vert);
        node_detail_ptr->acc.yawrate = encode_yawrate(p_sta->s.acce.yaw);

        /* brake. */
        node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.reserved = p_sta->s.braksta.wheel_brakes.wheel_brake_bit.reserved;
		node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.rightrear = p_sta->s.braksta.wheel_brakes.wheel_brake_bit.rightrear;
		node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.rightfront = p_sta->s.braksta.wheel_brakes.wheel_brake_bit.rightfront;
		node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.leftrear = p_sta->s.braksta.wheel_brakes.wheel_brake_bit.leftrear;
		node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.leftfront = p_sta->s.braksta.wheel_brakes.wheel_brake_bit.leftfront;

		node_detail_ptr->brake.traction = p_sta->s.braksta.traction;
		node_detail_ptr->brake.abs = p_sta->s.braksta.abs;
		node_detail_ptr->brake.scs = p_sta->s.braksta.scs;
		node_detail_ptr->brake.brakeboost = p_sta->s.braksta.brakeboost;
		node_detail_ptr->brake.auxbrakes = p_sta->s.braksta.auxbrakes;

        /* external light. */
        node_detail_ptr->exterlight.lowbeamheadlight = 0;
        node_detail_ptr->exterlight.highbeamheadlight = 0;
        node_detail_ptr->exterlight.leftturnsignallight = 0;
        node_detail_ptr->exterlight.rightturnsignallight = 0;
        node_detail_ptr->exterlight.hazardsignallight = 0;
        node_detail_ptr->exterlight.automaticlight = 0;
        node_detail_ptr->exterlight.daytimerunninglight = 0;
        node_detail_ptr->exterlight.foglighton = 0;
        node_detail_ptr->exterlight.parkinglight = 0;
        node_detail_ptr->exterlight.reserved = 0;

        /* alert flag. */
	    if(list_empty(&p_vsa->crd_list))
	    {
		   /* Set alert flag to none when crd list empty. */
	    	node_detail_ptr->alert_flag.alert_word = 0;
	    }
	    else
	    {
		   list_for_each_entry(p_alert_node, vsa_crd_node_t, &p_vsa->crd_list, list)
		   {
			   if(memcmp(node_detail_ptr->node_id, p_alert_node->pid, RCP_TEMP_ID_LEN) == 0)
			   {
				   /* Set alert flag when find the specific node and cancel the loop.  */
				//待完善代码，告警标识不对
				   node_detail_ptr->alert_flag.alert_word = cv_ntohl(p_alert_node->alert_flag);
				   break;
			   }
			   else
			   {
				   /* Set alert flag to none when no specific node. */
				   node_detail_ptr->alert_flag.alert_word = 0;
			   }
		   }
	    }
        /* Update data length and node number. */
        txbuf->data_len += NB_NODE_DETAIL_INFOR_ST_LEN;
        nb_node_ptr->nodenumber ++;
        
        node_detail_ptr ++;

        /* Stop the loop when no enough room for node infor. */
        if((EHM_BUF_VALID_DATA_LEN / NB_NODE_DETAIL_INFOR_ST_LEN) <= nb_node_ptr->nodenumber)
        {
            break;
        }
    }  
    
    osal_sem_release(p_vam->sem_sta);

    return 0;    
    
}


/*****************************************************************************
 @funcname: encode_basic_vehicle_status
 @brief   : basic vehicle status message.
 @param   : 
 @param   : 
 @return  :
*****************************************************************************/
static int8_t encode_basic_vehicle_status(ehm_envar_st * p_ehm)
{
	ehm_buffer_st * txbuf = NULL;
        
    frame_msg_header_st_ptr       msg_head_ptr = NULL;
    msg_vehicle_basic_status_st_ptr status_ptr = NULL;
    vam_stastatus_t               local_status = { { 0 }, 0 };
    

    /* Get tx buffer from ehm tx buffer list. */
    txbuf = ehm_get_txbuf(p_ehm);
    if (txbuf == NULL) 
    {
        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_INFO, "%s can not get ehm tx buf",__FUNCTION__);
        return -1;
    }
    
    /* Initial message header. */
    msg_head_ptr = (frame_msg_header_st_ptr)txbuf->data_ptr;
    msg_head_ptr->mark = 0x0E;
    msg_head_ptr->src = 0;
    msg_head_ptr->reserved1 = 0;
    msg_head_ptr->reserved2 = 0;
    msg_head_ptr->type = MSGTYPE_V2X_APPLY;

    /* Initial message body. */
    vam_get_local_current_status(&local_status);
    status_ptr = (msg_vehicle_basic_status_st_ptr)(txbuf->data_ptr + FRAME_MSG_HEADER_ST_LEN);

    /* message id. */
    status_ptr->msg_id = MSGID_BASIC_VEHICLE_STATUS;

    /* node id. */
    memcpy(status_ptr->node_id, local_status.pid, RCP_TEMP_ID_LEN);

    /* position. */
    status_ptr->position.latitude = encode_latitude(local_status.pos.lat);
    status_ptr->position.longitude = encode_longitude(local_status.pos.lon);
    status_ptr->position.elevation = encode_elevation(local_status.pos.elev);

    /* position accuracy. */
    status_ptr->posaccu.semimajoraxisAccu = encode_semimajor_axis_accuracy(local_status.pos.accu.semi_major_accu);
    status_ptr->posaccu.semiminoraxisAccu = encode_semiminor_axis_accuracy(local_status.pos.accu.semi_minor_accu);
    status_ptr->posaccu.semimajorAxisOrien = encode_semimajor_axis_orientation(local_status.pos.accu.semi_major_orientation);

    /* velocity. */
    status_ptr->velocity = encode_absolute_velocity(local_status.speed);

    /* angle. */
    status_ptr->angle = encode_angle(local_status.dir);


    /* Set data length. */
    txbuf->data_len += FRAME_MSG_HEADER_ST_LEN + MSG_VEHICLE_BASIC_STATUS_ST_LEN;

    return 0;    
    
}


/*****************************************************************************
 @funcname: decode_basic_vehicle_status
 @brief   : apply the v2x basic vehicle info
 @param   :
			p_ehm		- global ehm variable
			pdata		- v2x basic info frame data
			len			- pdata len
 @return  :
			0				- success
			others			- error
*****************************************************************************/
int decode_basic_vehicle_status(uint8_t *pdata, uint16_t len, uint32_t time)
{
	int                                 result = 0;
	vam_stastatus_t                      local = { { 0 }, 0 };
	msg_vehicle_basic_status_st_ptr status_ptr = (msg_vehicle_basic_status_st_ptr)pdata;


    /* Get local valid data. */
    vam_get_local_current_status(&local);

    //待完善，处理node_id关系
    /* node id. */
    memcpy(local.pid, status_ptr->node_id, sizeof(status_ptr->node_id));

    /* position. */
	local.pos.lat =  decode_latitude(status_ptr->position.latitude);
	local.pos.lon =  decode_longitude(status_ptr->position.longitude);
	local.pos.elev = decode_elevation(status_ptr->position.elevation);

    /* position accuracy. */
    local.pos.accu.semi_major_accu = decode_semimajor_axis_accuracy(status_ptr->posaccu.semimajoraxisAccu);
    local.pos.accu.semi_minor_accu = decode_semiminor_axis_accuracy(status_ptr->posaccu.semiminoraxisAccu);
    local.pos.accu.semi_major_orientation = decode_semimajor_axis_orientation(status_ptr->posaccu.semimajorAxisOrien);

    /* velocity. */
    local.speed = decode_absolute_velocity(status_ptr->velocity);

    /* angle. */
    local.dir = decode_angle(status_ptr->angle);

    local.time = time;
    /* Set new local status. */
	result = vam_set_local_status(&local);
	if(result < 0)
	{
		osal_printf("%s: vam_set_local_status err ret = %d. \n",__FUNCTION__, result);
	}
	return result;
}


/*****************************************************************************
 @funcname: decode_full_vehicle_status
 @brief   : apply the v2x basic vehicle info
 @param   :
			p_ehm		- global ehm variable
			pdata		- v2x basic info frame data
			len			- pdata len
 @return  :
			0				- success
			others			- error
*****************************************************************************/
int decode_full_vehicle_status(uint8_t *pdata, uint16_t len, uint32_t time)
{
	int                                 result = 0;
	vam_stastatus_t                      local = { { 0 }, 0 };
	msg_vehicle_full_status_st_ptr  status_ptr = (msg_vehicle_full_status_st_ptr)pdata;


    /* Get local valid data. */
    vam_get_local_current_status(&local);

    /* node id. */
    memcpy(local.pid, status_ptr->node_id, sizeof(status_ptr->node_id));

    /* position. */
	local.pos.lat =  decode_latitude(status_ptr->position.latitude);
	local.pos.lon =  decode_longitude(status_ptr->position.longitude);
	local.pos.elev = decode_elevation(status_ptr->position.elevation);

    /* position accuracy. */
    local.pos.accu.semi_major_accu = decode_semimajor_axis_accuracy(status_ptr->posaccu.semimajoraxisAccu);
    local.pos.accu.semi_minor_accu = decode_semiminor_axis_accuracy(status_ptr->posaccu.semiminoraxisAccu);
    local.pos.accu.semi_major_orientation = decode_semimajor_axis_orientation(status_ptr->posaccu.semimajorAxisOrien);

    /* transmission state. */
    local.tran_state = status_ptr->tran_state;

    /* velocity. */
    local.speed = decode_absolute_velocity(status_ptr->velocity);

    /* angle. */
    local.dir = decode_angle(status_ptr->angle);

    /* steer wheel angle. */
    local.steer_wheel_angle = status_ptr->steerwa;

    /* acceleration set4way. */
    local.acce.lat = decode_acceleration(status_ptr->acce4way.latacce);
    local.acce.lon = decode_acceleration(status_ptr->acce4way.lonacce);
    local.acce.vert = decode_vertical_acceleration(status_ptr->acce4way.veracce);
    local.acce.yaw = decode_yawrate(status_ptr->acce4way.yawrate);

    /* brake system status. */
    local.braksta = status_ptr->braksta;

    /* exterior light. */
    local.exterior_light = status_ptr->exterlight;
    /*recv msg time*/
    local.time = time;
    /* Set new local status. */
	result = vam_set_local_status(&local);
	if(result < 0)
	{
		osal_printf("%s: vam_set_local_status err ret = %d. \n", __FUNCTION__, result);
	}
    
	return result;
}


/* Decode message: vehicle static information. */
int decode_vehicle_static_infor(uint8_t *pdata, uint16_t len)
{
    int                                 result = 0;
	vam_stastatus_t                      local = { { 0 }, 0 };
	msg_vehicle_static_info_st_ptr  status_ptr = (msg_vehicle_static_info_st_ptr)pdata;


    /* Get local valid data. */
    vam_get_local_current_status(&local);

    /* Vehicle type. */
    local.vec_type = status_ptr->vehicle_type;

    /* Vehicle size. */
    local.vehicle_width = decode_vehicle_width(status_ptr->vehicle_size.vehiclewidth);
    local.vehicle_length = decode_vehicle_length(status_ptr->vehicle_size.vehiclelength);

    /* Set new local status. */
	result = vam_set_local_status(&local);
	if(result < 0)
	{
		osal_printf("%s: vam_set_local_status err ret=%d. \n",__FUNCTION__, result);
	}
    
	return result;
}


/* Decode message: local vehicle alert set. */
int decode_local_vehicle_alert_set(uint8_t *pdata, uint16_t len)
{
    int                                 result = 0;
	msg_local_vehicle_alert_st_ptr  status_ptr = (msg_local_vehicle_alert_st_ptr)pdata;


    /* Update vehicle break down alert. */
    if(status_ptr->vecbreakdownalert == VEHICLE_ALERT_ON) 
    {
        vam_active_alert(VAM_ALERT_MASK_VBD);
    }
    else if(status_ptr->vecbreakdownalert == VEHICLE_ALERT_OFF)
    {
        vam_cancel_alert(VAM_ALERT_MASK_VBD);
    }
    else if(status_ptr->vecbreakdownalert == VEHICLE_ALERT_INVALID)
    {
        vam_cancel_alert(VAM_ALERT_MASK_VBD);
    }    

    /* Update vehicle brake hard alert. */
    if(status_ptr->vecbrakehardalert == VEHICLE_ALERT_ON) 
    {
        vam_active_alert(VAM_ALERT_MASK_EBD);
    }
    else if(status_ptr->vecbrakehardalert == VEHICLE_ALERT_OFF)
    {
        vam_cancel_alert(VAM_ALERT_MASK_EBD);
    }
    else if(status_ptr->vecbrakehardalert == VEHICLE_ALERT_INVALID)
    {
        vam_cancel_alert(VAM_ALERT_MASK_EBD);
    }    

	return result;
}


#if 0
/*****************************************************************************
 @funcname: alert_node_report
 @brief   : report alert node information to host 
 @param   : None
 @return  : 
*****************************************************************************/
static int8_t encode_nb_vehicle_alert_OLD(ehm_envar_st * p_ehm)
{

    ehm_txbuf_t * txbuf;
    vsa_envar_t * p_vsa = &p_cms_envar->vsa;
    node_summary_infor_st * p_node_brief;
    ehm_txinfo_t * p_node_info;
    vsa_crd_node_t *p_alert_node;
    uint32_t data_time;
    list_head_t *p_txbuf_waiting_list = &p_ehm->txbuf_waiting_list;

    list_head_t * vsa_crd_list = &p_vsa->crd_list;

    osal_sem_take(p_vsa->sem_alert_list, OSAL_WAITING_FOREVER);

    txbuf = ehm_get_txbuf();
    if (txbuf == NULL) 
    {
        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_INFO, "%s can not get ehm tx buf",__FUNCTION__);
        osal_sem_release(p_vsa->sem_alert_list);
        return -1;
    }

    data_time = osal_get_systemtime();
    p_node_brief = (node_summary_infor_st *)EHM_TXBUF_DATA_PTR(txbuf);
    p_node_info = (ehm_txinfo_t *)EHM_TXBUF_INFO_PTR(txbuf);

    /* fill node information */
    list_for_each_entry(p_alert_node, vsa_crd_node_t, vsa_crd_list,list) {

        /* 节点id */
        memcpy(p_node_brief->node_id,p_alert_node->pid,RCP_TEMP_ID_LEN);

        /*纬度latitudinal,纵向相对距离*/
        p_node_brief->latitudinal_dis = cv_ntohs(p_alert_node->h_offset);  
        
        /*经度longitudinal,横向相对距离*/
        p_node_brief->longitudinal_dis = cv_ntohs(p_alert_node->v_offset);

        /* alert flag set */        
        p_node_brief->alert_flag.alert_word =cv_ntohl(p_alert_node->alert_flag);

        /* 1 */
        p_node_brief ++;

        txbuf->data_len += NODE_SUMMARY_INFOR_ST_LEN;

        p_node_info->f_node_num++;
        if (p_node_info->f_node_num >= EHM_BUF_SUMMARY_NODE_NUM ) {
            /* this fram fill done */
            OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_INFO, "this frame fill done,filled node num:%d",txbuf->info.f_node_num);

            /* fill data information */
            p_node_info->data_type = ALERL_NODE_SUMMARY;
            p_node_info->data_systime =cv_ntohl(data_time);

            /* mount buf to waiting list */
            list_add_tail(&txbuf->list, p_txbuf_waiting_list);

            txbuf = ehm_get_txbuf();
            if (txbuf == NULL) {
                OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_INFO, "re request ehm tx buf failed in %s\n",__FUNCTION__);
                /* jump out for loop */
                break;
            }
        }
    }  


    p_node_info->data_type = ALERL_NODE_SUMMARY;
    p_node_info->data_systime = cv_ntohl(data_time);
    
    /* mount buf to waiting list */
    list_add_tail(&txbuf->list, p_txbuf_waiting_list);

    osal_sem_release(p_vsa->sem_alert_list);

    /* inform tx thread waiting list have data */
    osal_sem_release(p_ehm->sem_ehm_tx);
		
    return 0;
}
#endif


/*****************************************************************************
 @funcname: encode_nb_vehicle_alert
 @brief   : neighbour node vehicle alert message.
 @param   : 
 @param   : 
 @return  :
*****************************************************************************/
static int8_t encode_nb_vehicle_alert(ehm_envar_st * p_ehm, vam_envar_t *p_vam, vsa_envar_t *p_vsa)
{
	ehm_buffer_st * txbuf = NULL;
        
    frame_msg_header_st_ptr          msg_head_ptr = NULL;
    msg_nb_vehicle_alert_st_ptr       nb_node_ptr = NULL;
    nb_node_summary_infor_st_ptr node_summary_ptr = NULL;

    vam_sta_node_t *p_sta = NULL;
    vsa_info_t vsa_position = { { 0 }, 0 };
    uint16_t nodenumber = 0;


    vsa_crd_node_t *p_alert_node = NULL;


    /* Get tx buffer from ehm tx buffer list. */
    txbuf = ehm_get_txbuf(p_ehm);

//    if (txbuf == NULL)
//    {
//        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_INFO, "%s can not get ehm tx buf",__FUNCTION__);
//        return -1;
//    }
    
    /* Initial message header. */
    msg_head_ptr = (frame_msg_header_st_ptr)txbuf->data_ptr;
    msg_head_ptr->mark = 0x0E;
    msg_head_ptr->src = 0;
    msg_head_ptr->reserved1 = 0;
    msg_head_ptr->reserved2 = 0;
    msg_head_ptr->type = MSGTYPE_V2X_APPLY;

    /* Initial message body 1. */
    nb_node_ptr = (msg_nb_vehicle_alert_st_ptr)(txbuf->data_ptr + FRAME_MSG_HEADER_ST_LEN);
    nb_node_ptr->msg_id = MSGID_NBVEHICLE_ALERT;
    nb_node_ptr->system_time = cv_ntohl(osal_get_systemtime());

    /* Set data length. */
    txbuf->data_len += FRAME_MSG_HEADER_ST_LEN + MSG_NB_VEHICLE_ALERT_LEN;

    /* Initial message body 2. */ 
    node_summary_ptr = (nb_node_summary_infor_st_ptr)(txbuf->data_ptr + FRAME_MSG_HEADER_ST_LEN + MSG_NB_VEHICLE_ALERT_LEN);

    osal_sem_take(p_vam->sem_sta, OSAL_WAITING_FOREVER);

    list_for_each_entry(p_sta, vam_sta_node_t, &p_vam->neighbour_list, list)
    {
        /* pid */
        memcpy(node_summary_ptr->node_id, p_sta->s.pid, RCP_TEMP_ID_LEN);

        /* relative position data */
        if (vsa_position_get(p_sta->s.pid, &vsa_position) == 0) 
        {
            if( (vsa_position.vsa_location == SLICE0_000_0T022_5) || (vsa_position.vsa_location == SLICE1_022_5T045_0) 
             || (vsa_position.vsa_location == SLICE2_045_0T067_5) || (vsa_position.vsa_location == SLICE3_067_5T090_0) 
             || (vsa_position.vsa_location == SLICE12_270_0T292_5)|| (vsa_position.vsa_location == SLICE13_292_5T315_0) 
             || (vsa_position.vsa_location == SLICE14_315_0T337_5)|| (vsa_position.vsa_location == SLICE15_337_5T360_0) )
            {
                node_summary_ptr->longitudinal_dis = - vsa_position.v_offset;
            }
            else
            {
                node_summary_ptr->longitudinal_dis = vsa_position.v_offset;
            }

            if( (vsa_position.vsa_location == SLICE0_000_0T022_5) || (vsa_position.vsa_location == SLICE1_022_5T045_0) 
             || (vsa_position.vsa_location == SLICE2_045_0T067_5) || (vsa_position.vsa_location == SLICE3_067_5T090_0) 
             || (vsa_position.vsa_location == SLICE4_090_0T112_5)|| (vsa_position.vsa_location == SLICE5_112_5T135_0) 
             || (vsa_position.vsa_location == SLICE6_135_0T157_5)|| (vsa_position.vsa_location == SLICE7_157_5T180_0) )
            {
                node_summary_ptr->lateral_dis = vsa_position.h_offset;
            }
            else
            {
                node_summary_ptr->lateral_dis = - vsa_position.h_offset;
            }
               
            node_summary_ptr->longitudinal_dis = cv_ntohs(node_summary_ptr->longitudinal_dis);
            node_summary_ptr->lateral_dis = cv_ntohs(node_summary_ptr->lateral_dis);
        }
        else 
        {
            node_summary_ptr->lateral_dis = 0;
            node_summary_ptr->longitudinal_dis = 0;
        }

        /* angle. */
        node_summary_ptr->angle = encode_angle(vsa_position.relative_dir);
        node_summary_ptr->velocity = encode_relative_velocity(vsa_position.relative_speed);
        node_summary_ptr->signalstrength = 0;
        node_summary_ptr->losstolerance = 0;

        /* alert flag. */
        if(list_empty(&p_vsa->crd_list))
        {
            /* Set alert flag to none when crd list empty. */
            node_summary_ptr->alert_flag.alert_word = 0;
        }
        else
        {
            list_for_each_entry(p_alert_node, vsa_crd_node_t, &p_vsa->crd_list, list) 
            {
                if(memcmp(node_summary_ptr->node_id, p_alert_node->pid, RCP_TEMP_ID_LEN) == 0)
                {   
                    /* Set alert flag when find the specific node and cancel the loop.  */
                    node_summary_ptr->alert_flag.alert_word = cv_ntohl(p_alert_node->alert_flag);
                    break;
                }
                else
                {
                    /* Set alert flag to none when no specific node. */
                    node_summary_ptr->alert_flag.alert_word = 0;
                }
            }
        }       
        
        /* Update data length and node number. */
        txbuf->data_len += NB_NODE_SUMMARY_INFOR_ST_LEN;
        
        node_summary_ptr ++;

        /* Stop the loop when no enough room for node infor. */
        if((EHM_BUF_VALID_DATA_LEN / NB_NODE_SUMMARY_INFOR_ST_LEN) <= nodenumber)
        {
            break;
        }
    }  
    
    osal_sem_release(p_vam->sem_sta);

    return 0;    
    
}

/*****************************************************************************
 @funcname: encode_roadsze_alert
 @brief   : report roadsize alert msg
 @param   : p_ehm			- ehm enviroment point
			p_vam			- vam enviroment point
			p_vsa			- vsa enviroment point
 @return  : 0				- success
			<0				- error
*****************************************************************************/
static int8_t encode_roadsze_alert(ehm_envar_st * p_ehm, vam_envar_t *p_vam, vsa_envar_t *p_vsa)
{
	ehm_buffer_st * txbuf = NULL;

	frame_msg_header_st_ptr          msg_head_ptr = NULL;

	/* Get tx buffer from ehm tx buffer list. */
	txbuf = ehm_get_txbuf(p_ehm);

	/* Initial message header. */
	msg_head_ptr = (frame_msg_header_st_ptr)txbuf->data_ptr;
	msg_head_ptr->mark = 0x0E;
	msg_head_ptr->src = 0;
	msg_head_ptr->reserved1 = 0;
	msg_head_ptr->reserved2 = 0;
	msg_head_ptr->type = MSGTYPE_V2X_APPLY;

	return 0;
}


/*****************************************************************************
 @funcname: msg_check_sum
 @brief   : check the chk of receive msg
 @param   :
			buf			- msg data
			len			- frame data len,without uart header ,length ,chk
 @return  :
			0			- success
			others		- error
*****************************************************************************/
static int ehm_msg_check_sum(uint8_t *buf, uint16_t len)
{
	uint16_t chksum = 0x0;
	uint16_t chk = 0x00;

    
	while(len--)
	{
		chksum += *buf;
		buf++;
	}
	chk = *(uint16_t *)buf;
	if(cv_ntohs(chk) == chksum)
		return 0;
	else
		return -1;
}



/**
  * @brief  Parse message header according to data type.
  * @param  See below.
  * @retval 0 access,-1 failure.
  */
void ehm_receive_msg
(
    /* Message receive type. */
    EHM_RECV_TYPE_E recv_type, 

    /* Pointer to receive buffer structure. */
    ehm_buffer_st_ptr buff_ptr
)
{ 
    int result = 0;

    
    /* Receive data from the specific pipeline. */
    switch(recv_type)
    {
        case UART_RECV_TYPE:
        {
            /* Receive data from uart pipeline. */
            result = dstream_device[DSTREAM_USBD].recv(buff_ptr->buffer, sizeof(buff_ptr->buffer));
            if(0 < result)
            {
                buff_ptr->data_ptr = buff_ptr->buffer;
                buff_ptr->data_len = result;
                buff_ptr->time = osal_get_systemtime();
            }
            else
            {
                buff_ptr->data_ptr = buff_ptr->buffer;
                buff_ptr->data_len = 0;
            }
            break;
        }	
        case ETH_RECV_TYPE:
        {
            break;
        }
        default:
        {
            break;
        }

	}
}


/**
  * @brief  Parse message header according to data type.
  * @param  See below.
  * @retval 0 access,-1 failure.
  */
int ehm_parse_msg_header
(
    /* Message receive type. */
    EHM_RECV_TYPE_E recv_type, 

    /* Pointer to receive buffer structure. */
    ehm_buffer_st_ptr buff_ptr
)
{
	uart_msg_header_st_ptr uart_ptr = (uart_msg_header_st_ptr)buff_ptr->data_ptr;
    
    
    switch(recv_type)
    {
        case UART_RECV_TYPE:
        {
            if(UART_MSG_HEADER_ST_LEN <= buff_ptr->data_len)
            {
                /* Update effective data address and length. */
                buff_ptr->data_len -= UART_MSG_HEADER_ST_LEN;
                buff_ptr->data_ptr += UART_MSG_HEADER_ST_LEN;

                /* Detect uart message header magic number. */
                if( (uart_ptr->magic_num1 != MSG_HEADER_MAGIC_NUM1) || (uart_ptr->magic_num2 != MSG_HEADER_MAGIC_NUM2))
                {   
                    osal_printf("%x, %x \n", uart_ptr->magic_num1, uart_ptr->magic_num2);
                    goto ERR_OUT;
                }
                
                /* Detect uart message header length. */
                if(cv_ntohs(uart_ptr->length) <= buff_ptr->data_len)
                {
                    buff_ptr->data_len = cv_ntohs(uart_ptr->length);
                }
                else
                {   
                    osal_printf("%d, %d\n",uart_ptr->length, buff_ptr->data_len);
                    goto ERR_OUT;
                }
            }
            else
            {
                osal_printf("%d \n", buff_ptr->data_len);
                goto ERR_OUT;
            }

            break;
        }	
        case ETH_RECV_TYPE:
        {
            break;
        }
        default:
        {
            goto ERR_OUT;
            break;
        }

	}


    return 0;

ERR_OUT:

    return -1;
}


/**
  * @brief  Parse message type v2x apply according to message id.
  * @param  See below.
  * @retval 0 access,-1 failure.
  */
int ehm_prase_msgtype_v2x_apply
(
    /* Pointer to data address. */
    uint8_t *data_ptr, 

    /* Data length. */
    uint16_t data_len,

    /* recv msg time*/
    uint32_t time
)
{
    int     result = 0;
    uint8_t msg_id = *(uint8_t *)data_ptr;

    
    switch(msg_id)
	{
    	case V2X_BASIC_VEHICLE_MSG: {
            result = decode_basic_vehicle_status(data_ptr, data_len ,time);     break;
        }
    	case V2X_FULL_VEHICLE_MSG:  {
            result = decode_full_vehicle_status(data_ptr, data_len ,time);      break;
        }	
    	case V2X_VEHICLE_STATIC_MSG:  {
            result = decode_vehicle_static_infor(data_ptr, data_len);     break;
        }	
    	case V2X_LC_VEHICLE_ALERT_MSG: {
            result = decode_local_vehicle_alert_set(data_ptr, data_len);  break;
        }
        default:                       {
            result = -1;                                                  break;
        }		
	}
    
	return result;
}


/**
  * @brief  Parse message body according to message type and id.
  * @param  See below.
  * @retval 0 access,-1 failure.
  */
int ehm_parse_msg_body
(
    /* Pointer to receive buffer structure. */
    ehm_buffer_st_ptr buff_ptr
)
{
	int                      result = 0;
	frame_msg_header_st_ptr msg_ptr = (frame_msg_header_st_ptr)buff_ptr->data_ptr;


    /* Detect effective data length. */
    if(buff_ptr->data_len < FRAME_MSG_HEADER_ST_LEN)
    {
        result = -1;
        goto RETURN_MARK;
    }

    /* Update effective data address and length. */
    buff_ptr->data_len -= FRAME_MSG_HEADER_ST_LEN - SIZEOF_MSG_CHK_DOMAIN;
    buff_ptr->data_ptr += FRAME_MSG_HEADER_ST_LEN;

    /* Detect frame header. */
    if((msg_ptr->mark == MSG_HEADER_MARK) && (msg_ptr->src == MSG_SRC_HOST))
    {
		switch(msg_ptr->type)
		{
		    case MSGTYPE_RESERVED:    {  
                result = -1;  break;  
            }
    		case MSGTYPE_DEBUG:       {  
                result = -1;  break;  
            }
    		case MSGTYPE_SYS_MANAGE:  {  
                result = -1;  break;  
            }
    		case MSGTYPE_V2X_APPLY:   {  
                result = ehm_prase_msgtype_v2x_apply(buff_ptr->data_ptr, buff_ptr->data_len, buff_ptr->time);  break;
            }  
    		case MSGTYPE_RAWDATA_DSRC:{  
                result = -1;  break;  
            }
    		case MSGTYPE_RAWDATA_NMEA:{  
                result = -1;  break;  
            }	
    		default:                  {  
              result = -1;  break;  
            }	
		}
    }
    else
    {
        result = -1;
    }

RETURN_MARK:

	return result;
}



/*****************************************************************************
 @funcname: ehm_thread_entry
 @brief   : ehm main thread entry 
 @param   : void *param  
 @return  : 
*****************************************************************************/
void * ehm_main_thread_entry(void *param)
{   

    return NULL;
}


/*****************************************************************************
 @funcname: ehm_package_send
 @brief   : ehm package send  process
 @param   :
			p_ehm		-	p_ehm gloar variable
			tx_info		-	the tx info structure
			pdata		-	buf
			length		-	buf len
 @return  :
			0			- success
			others		- error
*****************************************************************************/
static int ehm_package_send(ehm_envar_st * p_ehm)//, ehm_txinfo_t* tx_info, uint8_t *pdata, uint32_t length)
{
	uint8_t *pdata;
	uint32_t length;
	int result = 0;
    uart_msg_header_st_ptr uart_ptr;

    pdata = p_ehm->buffer_tx.buffer;
    uart_ptr = (uart_msg_header_st_ptr)pdata;

    length = p_ehm->buffer_tx.data_len;
    /* Build uart message header. */
    uart_ptr->magic_num1 = MSG_HEADER_MAGIC_NUM1;
    uart_ptr->magic_num2 = MSG_HEADER_MAGIC_NUM2;
    uart_ptr->length = cv_ntohs(length + SIZEOF_MSG_CHK_DOMAIN);

    //待完善功能-增加CHK,CHK长度数据已经计算
    /*uart send data to periph */
    result = dstream_device[DSTREAM_USBD].send((uint8_t *)uart_ptr, cv_ntohs(uart_ptr->length + UART_MSG_HEADER_ST_LEN));
    if(result < 0)
    {
    	osal_printf("comport_send error ret=%d\n", result);
    }
    //待完善功能-增加网络发送
    /* eth send data to periph*/
    
    return result;
    
}




void ehm_send_msg_group(ehm_envar_st_ptr p_ehm, uint32_t send_info)
{
    int		             ret = 0;

    
    /* neighour node summary information. */
	if(send_info & V2X_NB_NODE_SUMMRAY_INFO)
	{
		ret = encode_nb_node_summary_infor(p_ehm, p_vam_envar, &p_cms_envar->vsa);
		if(ret < 0)
		{
			osal_printf("encode_nb_node_summary_infor error ret=%d. \n", ret);
		}
		ret = ehm_package_send(p_ehm);
		if(ret < 0)
		{
			osal_printf("ehm_package_send error ret=%d. \n", ret);
		}
	}

    /* Neighbour node detail information. */
	if(send_info & V2X_NB_NODE_DETAIL_INFO)
	{
		ret = encode_nb_node_detail_infor(p_ehm, p_vam_envar, &p_cms_envar->vsa);
		if(ret < 0)
		{
			osal_printf("encode_nb_node_summary_infor error ret=%d\n",ret);
		}
		ret = ehm_package_send(p_ehm);
		if(ret < 0)
		{
			osal_printf("ehm_package_send error ret=%d\n",ret);
		}
	}

    /* Local node basic status. */
	if(send_info & V2X_BASIC_VEHICLE_STATUS)
	{
		ret = encode_basic_vehicle_status(p_ehm);
		if(ret < 0)
		{
			osal_printf("encode_nb_node_summary_infor error ret=%d\n",ret);
		}
		ret = ehm_package_send(p_ehm);
		if(ret < 0)
		{
			osal_printf("ehm_package_send error ret=%d\n",ret);
		}
	}

    /* Neighour node alert. */
	if(send_info & V2X_NB_VEHICLE_ALERT)
	{
		ret = encode_nb_vehicle_alert(p_ehm, p_vam_envar, &p_cms_envar->vsa);
		if(ret < 0)
		{
			osal_printf("encode_nb_node_summary_infor error ret=%d\n",ret);
		}
		ret = ehm_package_send(p_ehm);
		if(ret < 0)
		{
			osal_printf("ehm_package_send error ret=%d\n",ret);
		}
	}

    /* Roadside alert. */
	if(send_info & V2X_ROADSIZE_ALERT)
	{
		//待完善,添加路测告警上报功能
		ret = encode_roadsze_alert(p_ehm, p_vam_envar, &p_cms_envar->vsa);
		if(ret < 0)
		{
			osal_printf("encode_nb_node_summary_infor error ret=%d\n",ret);
		}
		ret = ehm_package_send(p_ehm);
		if(ret < 0)
		{
			osal_printf("ehm_package_send error ret=%d\n",ret);
		}
	}
}










/*****************************************************************************
 @funcname: ehm_tx_thread_entry
 @brief   : ehm tx thread process module
 @param   : ehm_envar_st * p_ehm
 @return  : none
*****************************************************************************/
void * ehm_tx_thread_entry(void *arg)
{
    int		             ret = 0;
    ehm_envar_st      *p_ehm = (ehm_envar_st *)arg;
    
    uint8_t buf[EHM_MQ_MSG_SIZE] = { 0 };
    uint32_t 	             len = 0;

    
    /* Print thread trace. */
    OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_TRACE, "Thread %s: ----> \n", __FUNCTION__);

RCV_MSGQ:

    /* Waiting for msg queue forever. */
    ret = osal_queue_recv(p_ehm->queue_tx, buf, &len, OSAL_WAITING_FOREVER);
    if (ret != OSAL_STATUS_SUCCESS)
    {
    	OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_ERROR, "%s: osal_queue_recv error [%d]. \n", __FUNCTION__, ret);
        goto RCV_MSGQ;
    }

    /* Start next receive when msg not for tx thread.  */
    if(((sys_msg_t *)buf)->id != EHM_MSG_VSA_SEND_DATA)
    {
        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_ERROR, "%s: Msg not for this thread. [%d]. \n", __FUNCTION__);
        goto RCV_MSGQ;
    }

    /* Send message group based on information. */
    ehm_send_msg_group(p_ehm, p_ehm->config_ptr->v2x_report_info);

    goto RCV_MSGQ;
    
    return NULL;    
}


/*****************************************************************************
 @funcname: ehm_rx_thread_entry
 @brief   : ehm rx  process thread module
 @param   : void *param
 @return  :	none
*****************************************************************************/
void * ehm_rx_thread_entry(void *param_ptr)
{
    ehm_envar_st_ptr ehm_ptr = (ehm_envar_st_ptr)param_ptr;
    int               result = 0;


START_ROUTINE:
    
    /* Receive message from the specific pipeline. */
    ehm_receive_msg(ehm_ptr->config_ptr->recv_type, &(ehm_ptr->buffer_rx));

    /* Start the next read process when data length error. */
	if(ehm_ptr->buffer_rx.data_len == 0)
	{
	    osal_printf("ehm receive data error. \n");
        goto START_ROUTINE;	  
	}
    
    /* Parse message header according to data type. */
    result = ehm_parse_msg_header(ehm_ptr->config_ptr->recv_type, &(ehm_ptr->buffer_rx));
    if(result != 0)
    {   
        osal_printf("ehm parse msg header error. \n");
        goto START_ROUTINE;
    }

    /* Check message.  */
    result = ehm_msg_check_sum(ehm_ptr->buffer_rx.data_ptr, ehm_ptr->buffer_rx.data_len);
    if(result != 0)
    {   
        osal_printf("ehm msg check sum error. \n");
   //     goto START_ROUTINE;     
    }
    
    /* Parse message body. */
    result = ehm_parse_msg_body(&(ehm_ptr->buffer_rx));
    if(result != 0)
    {
        osal_printf("ehm parse msg body error. \n");
    }

    
    goto START_ROUTINE;

    /* Only for format matching. */
    return NULL;
}

void timer_heartbeat_callback(void *arg)
{
	ehm_envar_st *p_ehm = (ehm_envar_st *)arg;


	if (p_ehm->queue_tx)
	{
		ehm_add_event_queue(p_ehm, EHM_MSG_VSA_SEND_DATA, 0, 0, NULL);
	}
}

/*****************************************************************************
 @funcname: ehm_init
 @brief   : ehm module init 
 @param   : void  
 @return  : 
*****************************************************************************/
void ehm_init(void)
{
//	int i;
    int result = 0;
    ehm_envar_st_ptr p_ehm = &ehm_envar;

    /* Initialize the txbuf queue. */
//    INIT_LIST_HEAD(&p_ehm->txbuf_waiting_list);
//    INIT_LIST_HEAD(&p_ehm->txbuf_free_list);
//    for(i = 0; i < (sizeof(p_ehm->txbuf) / sizeof(p_ehm->txbuf[0])); i++)
//    {
//        list_add_tail(&p_ehm->txbuf[i].list, &p_ehm->txbuf_free_list);
//    }

    /* Open uart module. */
    result = dstream_device[DSTREAM_USBD].open();
    if(result < 0)
    {
    	osal_printf("comport open error\n");
    }
    result = dstream_device[DSTREAM_USBD].config(&p_ehm->config_ptr->comport_config);
	if(result < 0)
	{
		osal_printf("comport config error ret=%d \n", result);
	}

    /* ehm main process event queue. */


    /* ehm module main process. */
    p_ehm->task_main = osal_task_create("t-ehm-main", ehm_main_thread_entry, p_ehm, EHM_MAIN_THREAD_STACK_SIZE, EHM_MAIN_THREAD_PRIORITY);
    osal_assert(p_ehm->task_main !=NULL);


    /* ehm module tx thread related. */
    p_ehm->queue_tx = osal_queue_create("q-ehmtx", EHM_QUEUE_SIZE, EHM_MQ_MSG_SIZE);
    osal_assert(p_ehm->queue_tx != NULL);
    
    p_ehm->task_tx = osal_task_create("task-ehmtx", ehm_tx_thread_entry, p_ehm, EHM_TX_THREAD_STACK_SIZE, EHM_TX_THREAD_PRIORITY);
    osal_assert(p_ehm->task_tx != NULL);

    /* ehm module rx thread related. */
    p_ehm->task_rx = osal_task_create("task-ehmrx", ehm_rx_thread_entry, p_ehm, EHM_RX_THREAD_STACK_SIZE, EHM_RX_THREAD_PRIORITY);
    osal_assert(p_ehm->task_rx != NULL);


    p_ehm->p_timer_heartbeat = osal_timer_create("tm-heartbeat", timer_heartbeat_callback, p_ehm, 100, TIMER_INTERVAL , TIMER_PRIO_NORMAL);
    osal_assert(p_ehm->p_timer_heartbeat != NULL);


    osal_timer_start(p_ehm->p_timer_heartbeat);
}


