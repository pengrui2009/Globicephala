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

/*****************************************************************************
 * declaration of variables and functions                                    *
*****************************************************************************/

/* need add to global param  later*/
ehm_config_t ehm_config = { ALL_NODE, DETAIL_INFO };

ehm_envar_t   ehm_envar = { &ehm_config, 0 };





/* Encode acceleration data from float to int16 format. */
int16_t encode_acceleration(float acceleration)
{
    int16_t result = 0;


    /* unit 0.01m/s^2. */
    result = (int16_t)(acceleration * (float)100);
    if(result != 2001)
    {
        result = (2000 < result)? 2000 : result;
        result = (result < -2000)? -2000 : result;
    }
    result = cv_ntohs(result);
    
	return result;
}

/* Decode acceleration data from int16 to float format. */
float decode_acceleration(int16_t acceleration)
{
    float result = 0;


    /* unit 0.01m/s^2. */
    acceleration = cv_ntohs(acceleration);
    result = (float)((float)acceleration / (float)100);
    if(result != 20.01)
    {
        result = (20.00 < result)? 20.00 : result;
        result = (result < -20.00)? -20.00 : result;
    }

	return result;
}

/* Encode angle data from float to uint16 format. */
uint16_t encode_angle(float angle)
{
    uint16_t result = 0;

    
	result = ((uint16_t)(angle / 0.0125));
    if(28800 <= result)
    {
        result = 28800;
    }
    result = cv_ntohs(result);
    
    return result;
}

/* Decode angle data from uint16 to float format. */
float decode_angle(uint16_t angle)
{
    float result = 0;


    angle = cv_ntohs(angle);
    if(28800 <= angle)
    {
        angle = 28800;
    }
    result = ((float)((float)angle * 0.0125));  
    
	return result; 
}

/* Encode elevation data from float to int32 format. */
int32_t encode_elevation(float elevation)
{
    int32_t result = 0;


    /* unit 10 cm. */
    result = (int32_t)(elevation * (float)10);
    if(result != -4096)
    {
        result = (61439 < result)? 61439 : result;
        result = (result < -4095)? -4095 : result;
    }
    result = cv_ntohl(result);
    
	return result;
}

/* Decode elevation data from int32 to float format. */
float decode_elevation(int32_t elevation)
{
    float result = 0;


    /* unit 10 cm. */
    elevation = cv_ntohl(elevation);
    result = (float)((float)elevation / (float)10);
    if(result != -409.6)
    {
        result = (6143.9 < result)? 6143.9 : result;
        result = (result < -409.5)? -409.5 : result;
    }

	return result;
}

/* Encode latitude data from float to int32 format. */
int32_t encode_latitude(float latitude)
{
    int32_t result = 0;


    /* unit 0.1 micro degree. */
    result = (int32_t)(latitude * (float)10000000);
    if(result != 900000001)
    {
        result = (900000000 < result)? 900000000 : result;
        result = (result < -900000000)? -900000000 : result;
    }
    result = cv_ntohl(result);
    
	return result;
}

/* Decode latitude data from int32 to float format. */
float decode_latitude(int32_t latitude)
{
    float result = 0;


    /* unit 0.1 micro degree. */
    latitude = cv_ntohl(latitude);
    result = (float)((float)latitude / (float)10000000);
    if(result != 90.0000001)
    {
        result = (90.0 < result)? 90.0 : result;
        result = (result < -90.0)? -90.0 : result;
    }

	return result;
}

/* Encode longitude data from float to int32 format. */
int32_t encode_longitude(float longitude)
{
    int32_t result = 0;


    /* unit 0.1 micro degree. */
    result = (int32_t)(longitude * (float)10000000);
    if(result != 1800000001)
    {
        result = (1800000000 < result)? 1800000000 : result;
        result = (result < -1799999999)? -1799999999 : result;
    }
    result = cv_ntohl(result);
    
	return result;
}

/* Decode longitude data from int32 to float format. */
float decode_longitude(int32_t longitude)
{
    float result = 0;


    /* unit 0.1 micro degree. */
    longitude = cv_ntohl(longitude);
    result = (float)((float)longitude / (float)10000000);
    if(result != 180.0000001)
    {
        result = (180.0000000 < result)? 180.0000000 : result;
        result = (result < -179.9999999)? -179.9999999 : result;
    }

	return result;
}

/* Encode semi-major axis accuracy data from float to uint8 format. */
uint8_t encode_semimajor_axis_accuracy(float accuracy)
{
    uint8_t result = 0;

    
	result = ((uint8_t)(accuracy / 0.05));
    return result;
}

/* Decode semi-major axis accuracy data from uint8 to float format. */
float decode_semimajor_axis_accuracy(uint8_t accuracy)
{
    float result = 0;


    result = ((float)((float)accuracy * 0.05));  
	return result; 
}

/* Encode semi-major axis orientation data from float to uint8 format. */
uint16_t encode_semimajor_axis_orientation(float orientation)
{
    uint16_t result = 0;

    
	result = ((uint16_t)(orientation / 0.0054932479));
    result = cv_ntohs(result);
    
    return result;
}

/* Decode semi-major axis arientation data from uint8 to float format. */
float decode_semimajor_axis_orientation(uint16_t orientation)
{
    float result = 0;


    orientation = cv_ntohs(orientation);
    result = ((float)((float)orientation * 0.0054932479));  
	return result; 
}

/* Encode semi-minor axis accuracy data from float to uint8 format. */
uint8_t encode_semiminor_axis_accuracy(float accuracy)
{
    uint8_t result = 0;

    
	result = ((uint8_t)(accuracy / 0.05));
    return result;
}

/* Decode semi-minor axis accuracy data from uint8 to float format. */
float decode_semiminor_axis_accuracy(uint8_t accuracy)
{
    float result = 0;


    result = ((float)((float)accuracy * 0.05));  
	return result; 
}

/* Encode semi-minor axis accuracy data from float to uint8 format. */
int8_t encode_steer_wheel_angle(float angle)
{
	return ((int8_t)(angle / 1.5));
}

/* Decode semi-minor axis accuracy data from float to uint8 format. */
float decode_steer_wheel_angle(int8_t angle)
{
	return ((float)((float)angle * 1.5));
}

/* Encode width data from float to uint16 format. */
uint16_t encode_vehicle_width(float width)
{
    uint16_t result = 0;

    
	result = (uint16_t)(width * 100);
    result = cv_ntohs(result);

    return result;
}

/* Decode width data from uint16 to float format. */
float decode_vehicle_width(uint16_t width)
{   
    float result = 0;


    width = cv_ntohs(width);
    result = (float)width / (float)100;
    
	return result;
}

/* Encode length data from float to uint16 format. */
uint16_t encode_vehicle_length(float length)
{
    uint16_t result = 0;

    
	result = (uint16_t)(length * 100);
    result = cv_ntohs(result);

    return result;
}

/* Decode length data from uint16 to float format. */
float decode_vehicle_length(uint16_t length)
{   
    float result = 0;


    length = cv_ntohs(length);
    result = (float)length / (float)100;
    
	return result;
}









/* Encode velocity data from float to int16 format. */
int16_t encode_velocity(float velocity)
{
    int16_t result = 0;


    result = (int16_t)(velocity / 0.02);

    result = (8190 < result)? 8190 : result;
    result = (result < -8190)? -8190 : result;
    result = cv_ntohs(result);
    
    return result;
}

/* Decode velocity data from int16 to float. */
float decode_velocity(int16_t velocity)
{
    float result = 0;

    velocity = cv_ntohs(velocity);
    result = (float)velocity * 0.02;
    return result;
}

/* Encode vertical acceleration data from float to int8 format. */
int8_t encode_vertical_acceleration(float acceleration)
{
    int8_t result = 0;


    /* Correct the acceleration data. */
    if((127 * 0.1962) < acceleration)
    {
        acceleration = 127 * 0.1962;
    }
    if(acceleration < -(126 * 0.1962))
    {
        acceleration = -(126 * 0.1962);
    }
    
    result = (int8_t)(acceleration / 0.1962);

    return result;
}

/* Decode vertical acceleration data from int8 to float format. */
float decode_vertical_acceleration(int8_t acceleration)
{
    float result = 0;

    result = (float)((float)acceleration * 0.1962);
    return result;
}

/* Encode yawrate data from float to int16 format. */
int16_t encode_yawrate(float yawrate)
{
    int16_t result = 0;


    /* Unit 0.01 degree/s. */
    result = (int16_t)(yawrate / 0.01);
    result = cv_ntohs(result);
    
    return result;    
}

/* Decode yawrate data from int16 to float format. */
float decode_yawrate(int16_t yawrate)
{
    float result = 0;

    /* Unit 0.01 degree/s. */
    yawrate = cv_ntohs(yawrate);
    result = (float)((float)yawrate * 0.01);
    return result;
}














/*****************************************************************************
 @funcname: encode_vehicle_alert_flag
 @brief   : tran the local vehicle alert flag to extrenal vehicle alert flag
 @param   : warning_id		-	the local vehicle alert flag
 @return  :	extrenal vehicle alert flag
*****************************************************************************/
__COMPILE_INLINE__ uint32_t encode_vehicle_alert_flag(uint16_t warning_id)
{
    alert_flag_st alert_flag;

    alert_flag.alert_word = 0;

    if (warning_id & VAM_ALERT_MASK_VBD) {

        alert_flag.alert_bit.vec_breakdown = 1;
    }

    if (warning_id & VAM_ALERT_MASK_EBD) {

        alert_flag.alert_bit.vec_brake_hard = 1;
    }

    return alert_flag.alert_word;
}
/*****************************************************************************
 @funcname: decode_vehicle_alert_flag
 @brief   : tran extrenal vehicle alert flag to the local vehicle alert flag
 @param   : warning_id		- the extrenal vehicle alert flag
 @return  : the local vehicle alert flag
*****************************************************************************/
__COMPILE_INLINE__ uint16_t decode_vehicle_alert_flag(uint32_t x)
{
    uint16_t r = 0;
    x = cv_ntohl(x);
    if (x & EventHazardLights) {
        r |= VAM_ALERT_MASK_VBD;
    }

    if (x & EventHardBraking){
        r |= VAM_ALERT_MASK_EBD;
    }

    if (x & EventDisabledVehicle){
        r |= VAM_ALERT_MASK_VOT;
    }
    return r;
}




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
int ehm_add_event_queue(ehm_envar_t *p_ehm, 
                             uint16_t msg_id, 
                             uint16_t msg_len, 
                             uint32_t msg_argc,
                             void    *msg_argv)

{
    int err = OSAL_STATUS_NOMEM;
    sys_msg_t *p_msg;
    uint32_t len = sizeof(sys_msg_t);

    p_msg = osal_malloc(sizeof(sys_msg_t));
    p_msg->id = msg_id;
    p_msg->len = msg_len;
    p_msg->argc = msg_argc;
    p_msg->argv = msg_argv;
    err = osal_queue_send(p_ehm->queue_ehm_main, p_msg, len, 0, OSAL_NO_WAIT);

    if (err != OSAL_STATUS_SUCCESS) 
    {
        OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_WARN, "%s: failed=[%d], msg=%04x\n", __FUNCTION__, err, msg_id);
        osal_free(p_msg);  
    }

    return err;
}
/*****************************************************************************
 @funcname: inform_ehm_caculate_done
 @brief   :
 @param   : none
 @return  :
			0		- success
			others 	- error
*****************************************************************************/
int inform_ehm_caculate_done(void)
{
    ehm_envar_t *p_ehm = &ehm_envar;

    if (p_ehm->queue_ehm_main) 
    {
        ehm_add_event_queue(p_ehm, EHM_MSG_VSA_ANALY_DONE, 0, 0, NULL);
    }
    return 0;
}


/* Message group.----------------------------------------------------------- */


/*****************************************************************************
 @funcname: nb_node_summary_infor
 @brief   : neighbour node summary infor message.
 @param   : 
 @param   : 
 @return  :
*****************************************************************************/
static int8_t encode_nb_node_summary_infor(ehm_envar_t * p_ehm, vam_envar_t *p_vam, vsa_envar_t *p_vsa)
{
    ehm_txbuf_t * txbuf;
        
    frame_msg_header_st_ptr          msg_head_ptr = NULL;
    msg_vehicle_nb_status_st_ptr      nb_node_ptr = NULL;
    nb_node_summary_infor_st_ptr node_summary_ptr = NULL;

    vam_sta_node_t *p_sta = NULL;
    vsa_info_t vsa_position = { { 0 }, 0 };

    vsa_crd_node_t *p_alert_node = NULL;


    /* Get tx buffer from ehm tx buffer list. */
    txbuf = ehm_get_txbuf();
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
        node_summary_ptr->velocity = encode_velocity(vsa_position.relative_speed);
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
        nb_node_ptr->nodenumber ++;
        
        node_summary_ptr ++;

        /* Stop the loop when no enough room for node infor. */
        if((EHM_BUF_VALID_DATA_LEN / NB_NODE_SUMMARY_INFOR_ST_LEN) <= nb_node_ptr->nodenumber)
        {
            break;
        }
    }  
    
    osal_sem_release(p_vam->sem_sta);

    
    /* mount buf to waiting list and inform tx thread waiting list have data. */
    list_add_tail(&txbuf->list, &p_ehm->txbuf_waiting_list);
    osal_sem_release(p_ehm->sem_ehm_tx);

    return 0;    
    
}


/*****************************************************************************
 @funcname: nb_node_detail_infor
 @brief   : neighbour node detail infor message.
 @param   : 
 @param   : 
 @return  :
*****************************************************************************/
static int8_t encode_nb_node_detail_infor(ehm_envar_t * p_ehm, vam_envar_t *p_vam, vsa_envar_t *p_vsa)
{
    ehm_txbuf_t * txbuf;
        
    frame_msg_header_st_ptr          msg_head_ptr = NULL;
    msg_vehicle_nb_status_st_ptr      nb_node_ptr = NULL;
    nb_node_detail_infor_st_ptr   node_detail_ptr = NULL;

    vam_sta_node_t *p_sta = NULL;


    /* Get tx buffer from ehm tx buffer list. */
    txbuf = ehm_get_txbuf();
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
        node_detail_ptr->posaccu.semimajoraxisAccu = encode_semimajor_axis_accuracy(0);
        node_detail_ptr->posaccu.semiminoraxisAccu = encode_semiminor_axis_accuracy(0);
        node_detail_ptr->posaccu.semimajorAxisOrien = encode_semimajor_axis_orientation(0);

        /* velocity. */
        node_detail_ptr->velocity = encode_velocity(p_sta->s.speed);

        /* angle. */
        node_detail_ptr->angle = encode_angle(p_sta->s.dir);

        /* steer wheel angle. */
        node_detail_ptr->steer_wheel_angle = encode_steer_wheel_angle(0);

        /* acceleration set 4 way. */  
        node_detail_ptr->acc.lonacce = encode_acceleration(0);
        node_detail_ptr->acc.latacce = encode_acceleration(0);
        node_detail_ptr->acc.veracce = encode_vertical_acceleration(0);
        node_detail_ptr->acc.yawrate = encode_yawrate(0);

        /* brake. */
        node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.reserved = 0;
        node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.rightrear = 0;
        node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.rightfront = 0;
        node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.leftrear = 0;
        node_detail_ptr->brake.wheel_brakes.wheel_brake_bit.leftfront = 0;
        
        node_detail_ptr->brake.traction = 0;
        node_detail_ptr->brake.abs = 0;
        node_detail_ptr->brake.scs = 0;
        node_detail_ptr->brake.brakeboost = 0;
        node_detail_ptr->brake.auxbrakes = 0;

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

    
    /* mount buf to waiting list and inform tx thread waiting list have data. */
    list_add_tail(&txbuf->list, &p_ehm->txbuf_waiting_list);
    osal_sem_release(p_ehm->sem_ehm_tx);

    return 0;    
    
}


/*****************************************************************************
 @funcname: encode_basic_vehicle_status
 @brief   : basic vehicle status message.
 @param   : 
 @param   : 
 @return  :
*****************************************************************************/
static int8_t encode_basic_vehicle_status(ehm_envar_t * p_ehm)
{
    ehm_txbuf_t                   * txbuf = NULL;
        
    frame_msg_header_st_ptr       msg_head_ptr = NULL;
    msg_vehicle_basic_status_st_ptr status_ptr = NULL;
    vam_stastatus_t               local_status = { { 0 }, 0 };
    

    /* Get tx buffer from ehm tx buffer list. */
    txbuf = ehm_get_txbuf();
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
    status_ptr->posaccu.semimajoraxisAccu = encode_semimajor_axis_accuracy(0);
    status_ptr->posaccu.semiminoraxisAccu = encode_semiminor_axis_accuracy(0);
    status_ptr->posaccu.semimajorAxisOrien = encode_semimajor_axis_orientation(0);

    /* velocity. */
    status_ptr->velocity = encode_velocity(local_status.speed);

    /* angle. */
    status_ptr->angle = encode_angle(local_status.dir);


    /* Set data length. */
    txbuf->data_len += FRAME_MSG_HEADER_ST_LEN + MSG_VEHICLE_BASIC_STATUS_ST_LEN;

    /* mount buf to waiting list and inform tx thread waiting list have data. */
    list_add_tail(&txbuf->list, &p_ehm->txbuf_waiting_list);
    osal_sem_release(p_ehm->sem_ehm_tx);

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
int decode_basic_vehicle_status(uint8_t *pdata, uint16_t len)
{
	int ret;
	vam_stastatus_t                      local = { { 0 }, 0 };
	msg_vehicle_basic_status_st_ptr status_ptr = (msg_vehicle_basic_status_st_ptr)pdata;


    /* Get local valid data. */
    vam_get_local_current_status(&local);

    /* node id. */
    memcpy(local.pid, status_ptr->node_id, RCP_TEMP_ID_LEN);

    /* position. */
	local.pos.lat =  decode_latitude(status_ptr->position.latitude);
	local.pos.lon =  decode_longitude(status_ptr->position.longitude);
	local.pos.elev = decode_elevation(status_ptr->position.elevation);

    /* position accuracy. */
    local.pos.accu.semi_major_accu = decode_semimajor_axis_accuracy(status_ptr->posaccu.semimajoraxisAccu);
    local.pos.accu.semi_minor_accu = decode_semiminor_axis_accuracy(status_ptr->posaccu.semiminoraxisAccu);
    local.pos.accu.semi_major_orientation = decode_semimajor_axis_orientation(status_ptr->posaccu.semimajorAxisOrien);

    /* velocity. */
    local.speed = decode_velocity(status_ptr->velocity);

    /* angle. */
    local.dir = decode_angle(status_ptr->angle);

    /* Set new local status. */
	ret = vam_set_local_status(&local);
	if(ret < 0)
	{
		osal_printf("%s:vam_set_local_status err ret=%d\n",__FUNCTION__,ret);
		goto err;
	}
    
	ret = 0;
    
err:
	return ret;
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
int decode_full_vehicle_status(uint8_t *pdata, uint16_t len)
{
	int ret;
	vam_stastatus_t                      local = { { 0 }, 0 };
	msg_vehicle_full_status_st_ptr  status_ptr = (msg_vehicle_full_status_st_ptr)pdata;


    /* Get local valid data. */
    vam_get_local_current_status(&local);

    /* node id. */
    memcpy(local.pid, status_ptr->node_id, RCP_TEMP_ID_LEN);

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
    local.speed = decode_velocity(status_ptr->velocity);

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

    /* Set new local status. */
	ret = vam_set_local_status(&local);
	if(ret < 0)
	{
		osal_printf("%s:vam_set_local_status err ret=%d\n",__FUNCTION__,ret);
		goto err;
	}
    
	ret = 0;
    
err:
	return ret;
}


/* Decode message: vehicle static information. */
int decode_vehicle_static_infor(uint8_t *pdata, uint16_t len)
{
    int ret;
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
	ret = vam_set_local_status(&local);
	if(ret < 0)
	{
		osal_printf("%s:vam_set_local_status err ret=%d\n",__FUNCTION__,ret);
		goto err;
	}
    
	ret = 0;
    
err:
	return ret;
}

#if 0
/* Decode message: local vehicle alert set. */
int decode_local_vehicle_alert_set(uint8_t *pdata, uint16_t len)
{
    int ret;
	vam_stastatus_t                      local = { { 0 }, 0 };
	msg_local_vehicle_alert_st_ptr  status_ptr = (msg_local_vehicle_alert_st_ptr)pdata;


    /* Get local valid data. */
    vam_get_local_current_status(&local);


    /*- Set local alert. -----------------------------*/

    
    /* Set new local status. */
	ret = vam_set_local_status(&local);
	if(ret < 0)
	{
		osal_printf("%s:vam_set_local_status err ret=%d\n",__FUNCTION__,ret);
		goto err;
	}
    
	ret = 0;
    
err:
	return ret;
}
#endif

#if 0
/*****************************************************************************
 @funcname: alert_node_report
 @brief   : report alert node information to host 
 @param   : None
 @return  : 
*****************************************************************************/
static int8_t encode_nb_vehicle_alert_OLD(ehm_envar_t * p_ehm)
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
static int8_t encode_nb_vehicle_alert(ehm_envar_t * p_ehm, vam_envar_t *p_vam, vsa_envar_t *p_vsa)
{
    ehm_txbuf_t * txbuf = NULL;
        
    frame_msg_header_st_ptr          msg_head_ptr = NULL;
    msg_nb_vehicle_alert_st_ptr       nb_node_ptr = NULL;
    nb_node_summary_infor_st_ptr node_summary_ptr = NULL;

    vam_sta_node_t *p_sta = NULL;
    vsa_info_t vsa_position = { { 0 }, 0 };
    uint16_t nodenumber = 0;


    vsa_crd_node_t *p_alert_node = NULL;


    /* Get tx buffer from ehm tx buffer list. */
    txbuf = ehm_get_txbuf();
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
        node_summary_ptr->velocity = encode_velocity(vsa_position.relative_speed);
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

    
    /* mount buf to waiting list and inform tx thread waiting list have data. */
    list_add_tail(&txbuf->list, &p_ehm->txbuf_waiting_list);
    osal_sem_release(p_ehm->sem_ehm_tx);

    return 0;    
    
}




/*****************************************************************************
 @funcname: ehm_rx_handle_complete
 @brief   : release the rxbuf
 @param   : ehm_envar_t * p_ehm
 @return  : void
*****************************************************************************/
void ehm_rx_handle_complete(ehm_envar_t * p_ehm)
{
	ehm_rxbuf_t *rxbuf = NULL;
	list_head_t *p_rxbuf_waiting_list = &p_ehm->rxbuf_waiting_list;
    

	if (!list_empty(p_rxbuf_waiting_list))
	{
		rxbuf = list_first_entry(p_rxbuf_waiting_list, ehm_rxbuf_t, list);
		list_move_tail(&rxbuf->list, &p_ehm->rxbuf_free_list);
	}
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
static int msg_check_sum(uint8_t *buf,uint16_t len)
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
/*****************************************************************************
 @funcname: ehm_parse_sys_manage
 @brief   : parse sys manage msg
 @param   :
			p_ehm			-	global ehm variable
			pdata			-	frame data
			len				-	frame data len
 @return  :
			0				- success
			others			- error
*****************************************************************************/
int ehm_parse_sys_manage(ehm_envar_t * p_ehm, uint8_t *pdata, uint16_t len)
{
	return 0;
}



/*****************************************************************************
 @funcname: ehm_v2x_static_vehicle_notice
 @brief   : apply v2x static vehicle info
 @param   :
			p_ehm		- global ehm variable
			rxinfo		- rxinfo structure
			pdata		- v2x static vehicle info frame data
			len			- pdata len
 @return  :
			0				- success
			others			- error
*****************************************************************************/
void ehm_v2x_static_vehicle_notice(ehm_envar_t * p_ehm, ehm_rxinfo_t *rxinfo,uint8_t *pdata, uint16_t len)
{

}
/*****************************************************************************
 @funcname: ehm_v2x_alert_vehicle_notice
 @brief   : apply v2x alert vehicle info
 @param   : ehm_envar_t * p_ehm
 @param   :
			p_ehm		- global ehm variable
			rxinfo		- rxinfo structure
			pdata		- v2x alert vehicle info frame data
			len			- pdata len
 @return  :
			0			- success
			others		- error
*****************************************************************************/
void ehm_v2x_alert_vehicle_notice(ehm_envar_t * p_ehm, ehm_rxinfo_t *rxinfo,uint8_t *pdata, uint16_t len)
{

}
/*****************************************************************************
 @funcname: ehm_prase_v2x
 @brief   : ehm module main handle process
 @param   : ehm_envar_t * p_ehm
 @param   : sys_msg_t *p_msg
 @return  :
*****************************************************************************/
int ehm_prase_v2x(ehm_envar_t * p_ehm, ehm_rxinfo_t *rxinfo, uint8_t *pdata, uint16_t len)
{
	uint8_t v2x_id;
	v2x_id = *pdata;
	switch(v2x_id)
	{
	case V2X_BASIC_VEHICLE_STATUS:
		//ehm_v2x_basic_vehicle_notice(p_ehm, rxinfo, pdata, len);
		break;
	case V2X_FULL_VEHICLE_STATUS:
		//ehm_v2x_full_vehicle_notice(p_ehm, rxinfo, pdata, len);
		break;
	case V2X_VEHICLE_STATIC_INFO:
		//ehm_v2x_static_vehicle_notice(p_ehm, rxinfo,pdata, len);
		break;
	case V2X_LC_VEHICLE_ALERT_SET:
		//ehm_v2x_alert_vehicle_notice(p_ehm, rxinfo, pdata, len);
		break;
	}
	return 0;
}
/*****************************************************************************
 @funcname: ehm_parse_frame
 @brief   : ehm parse the  data frame
 @param   :
			p_ehm		- global ehm variable
			rxinfo		- rxinfo structure
			pdata		- frame info data
			len			- pdata len
 @return  :
			0			- success
			others		- error
*****************************************************************************/
int ehm_parse_frame(ehm_envar_t * p_ehm, ehm_rxinfo_t *rxinfo, uint8_t *pdata,uint16_t len)
{
	int ret = 0;
	frame_msg_header_st_ptr msg_header = (frame_msg_header_st_ptr)pdata;


	len += FRAME_MSG_HEADER_ST_LEN;
	pdata += FRAME_MSG_HEADER_ST_LEN;
	if((msg_header->mark == MSG_HEADER_MARK) && (msg_header->src == MSG_SRC_HOST))
	{
		switch(msg_header->type)
		{
		case MSGTYPE_DEBUG:
			break;
		case MSGTYPE_SYS_MANAGE:
			break;
		case MSGTYPE_V2X_APPLY:
			ret = ehm_prase_v2x(p_ehm, rxinfo, pdata, len);
			break;
//		case MSGTYPE_RAWDATA_DSRC:
//			break;
//		case MSGTYPE_RAWDATA_NMEA:
//			break;
		default:
			break;
		}
	}else{
		ret = -1;
		goto err;
	}
err:
	return ret;
}
/*****************************************************************************
 @funcname: ehm_parse_msg
 @brief   : ehm parse msg info
 @param   :
			p_ehm		- global ehm variable
			rxinfo		- rxinfo structure
			pdata		- msg data
			len			- pdata len
 @return  :
			0			- success
			others		- error
*****************************************************************************/
void ehm_parse_msg(ehm_envar_t * p_ehm, ehm_rxinfo_t *rxinfo,uint8_t *pdata,uint32_t msg_len)
{
	int ret;
	uint16_t len;

	uart_msg_header_st *uart_msg_header;
	switch(rxinfo->recv_type)
	{
	case UART_RECV_TYPE:
		if(msg_len >= UART_MSG_HEADER_ST_LEN)
		{
			uart_msg_header = (uart_msg_header_st *)pdata;

			if((uart_msg_header->magic_num1 == MSG_HEADER_MAGIC_NUM1) && (uart_msg_header->magic_num2 == MSG_HEADER_MAGIC_NUM2))
			{
				len = cv_ntohs(uart_msg_header->length);
				pdata += UART_MSG_HEADER_ST_LEN;
				if(msg_len >= (len + UART_MSG_HEADER_ST_LEN))
				{
					len = len - MSG_CHK_LEN;
					if(msg_check_sum(pdata, len) == 0)
					{
						ret = ehm_parse_frame(p_ehm, rxinfo, pdata, len);
						if(ret < 0)
						{
							goto err;
						}
					}
				}
			}
		}
		break;
	case ETH_RECV_TYPE:
		break;
	}

err:
	ehm_rx_handle_complete(p_ehm);
}
/*****************************************************************************
 @funcname: ehm_rx_handle
 @brief   : get rxbuf from waiting list and handle rx msg info
 @param   : ehm_envar_t * p_ehm
 @return  : void
*****************************************************************************/
void ehm_rx_handle(ehm_envar_t * p_ehm)
{
	list_head_t * p_rxbuf_waiting_list;
	ehm_rxbuf_t * rxbuf;
	int recv_next = 0;
	p_rxbuf_waiting_list = (list_head_t *)&p_ehm->rxbuf_waiting_list;

	if (!list_empty(p_rxbuf_waiting_list))
	{
		rxbuf = list_first_entry(p_rxbuf_waiting_list, ehm_rxbuf_t, list);
		recv_next = 1;
	}


	if (recv_next)
	{
		/* data packed according to data type */
		ehm_parse_msg(p_ehm, &rxbuf->info, rxbuf->data_ptr, rxbuf->data_len);
	}

}
/*****************************************************************************
 @funcname: ehm_main_proc
 @brief   : ehm module main handle process
 @param   : ehm_envar_t * p_ehm  
 @param   : sys_msg_t *p_msg     
 @return  : void
*****************************************************************************/
void ehm_main_proc(ehm_envar_t * p_ehm, sys_msg_t *p_msg)
{
    ehm_config_t *p_work_param = p_ehm->working_param;
    
    switch (p_msg->id) 
    {
        case EHM_MSG_VSA_ANALY_DONE: 
        {
            if ((p_work_param->report_node_type == ALERT_NODE)) 
            {
                //alert_node_report(p_ehm);  
            }
            
            else if (p_work_param->report_node_type == ALL_NODE) 
            {
                if (p_work_param->node_info_type ==SUMMARY_INFO) 
                {
                    
                }

                if (p_work_param->node_info_type ==DETAIL_INFO) 
                {
                    //detail_node_report(p_ehm);
                }
            }
            
            break;  
        }
        
        case EHM_MSG_VSA_ANALY_DATA:
        {
            ehm_rx_handle(p_ehm);
        	break;
        }
        	
        default: 
        {

        }
        break;
    }
}

/*****************************************************************************
 @funcname: ehm_thread_entry
 @brief   : ehm main thread entry 
 @param   : void *param  
 @return  : 
*****************************************************************************/
void * ehm_main_thread_entry(void *param)
{
    ehm_envar_t *p_ehm = param;
    osal_status_t err;
    sys_msg_t *p_msg;
    uint32_t len;
    
    while (1) 
    {
        err = osal_queue_recv(p_ehm->queue_ehm_main, &p_msg, &len, OSAL_WAITING_FOREVER);
        if (err == OSAL_STATUS_SUCCESS) 
        {
            /* ehm man process handle */
            ehm_main_proc(p_ehm,p_msg);
            osal_free(p_msg);
        }
        else 
        {
            OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_ERROR, "%s: osal_queue_recv error [%d]\n", __FUNCTION__, err);            
        }
    }

    return NULL;
}


/*****************************************************************************
 @funcname: ehm_tx_complete
 @brief   : Put the TXBUF in waiting list back to free list only.
 @param   : ehm_envar_t * p_ehm
 @return  : none
*****************************************************************************/
void ehm_tx_complete(ehm_envar_t *p_ehm)
{
    ehm_txbuf_t *txbuf = NULL;

    
    if (!list_empty(&(p_ehm->txbuf_waiting_list))) 
    {
        txbuf = list_first_entry(&(p_ehm->txbuf_waiting_list), ehm_txbuf_t, list);
        list_move_tail(&txbuf->list, &p_ehm->txbuf_free_list);
    }		
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
static int ehm_package_send(ehm_envar_t * p_ehm, ehm_txinfo_t* tx_info, uint8_t *pdata, uint32_t length)
{
    uart_msg_header_st_ptr uart_ptr = (uart_msg_header_st_ptr)(pdata - UART_MSG_HEADER_ST_LEN);
    int result = 0;


    /* Build uart message header. */
    uart_ptr->magic_num1 = MSG_HEADER_MAGIC_NUM1;
    uart_ptr->magic_num2 = MSG_HEADER_MAGIC_NUM2;
    uart_ptr->length = cv_ntohs(length + SIZEOF_MSG_CHK_DOMAIN);

    /* send data to periph */  
  //  result = comport_send(uart_ptr, cv_ntohs(uart_ptr->length) + UART_MSG_HEADER_ST_LEN);
    if(result < 0)
    {
    	osal_printf("comport_send error ret=%d\n", result);
    }
    
    return result;
    
}


/* the reentrant should be considered */
//void ehm_rx_handle(ehm_envar_t * p_ehm)
//{
//	ehm_rxbuf_t * rxbuf;
//	list_head_t * p_rxbuf_waiting_list = &p_ehm->rxbuf_waiting_list;
//	rxbuf = ehm_get_rxbuf();
//
//}


/*****************************************************************************
 @funcname: ehm_tx_thread_entry
 @brief   : ehm tx thread process module
 @param   : ehm_envar_t * p_ehm
 @return  : none
*****************************************************************************/
void * ehm_tx_thread_entry(ehm_envar_t *p_ehm)
{
    int                 err = 0;
    ehm_txbuf_t * txbuf_ptr = NULL;


    /* Print thread trace. */
    OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_TRACE, "Thread %s: ---->\n", __FUNCTION__);

    while(1)
    {



        encode_nb_node_summary_infor(p_ehm, p_vam_envar, &p_cms_envar->vsa);
        encode_nb_node_detail_infor(p_ehm, p_vam_envar, &p_cms_envar->vsa);
        encode_basic_vehicle_status(p_ehm);
        encode_nb_vehicle_alert(p_ehm, p_vam_envar, &p_cms_envar->vsa);

    
        err = osal_sem_take(p_ehm->sem_ehm_tx, OSAL_WAITING_FOREVER);
        if (err == OSAL_STATUS_SUCCESS)
        {
            if (!list_empty(&(p_ehm->txbuf_waiting_list))) 
            {
                txbuf_ptr = list_first_entry(&(p_ehm->txbuf_waiting_list), ehm_txbuf_t, list);
            }
            else
            {
                txbuf_ptr = NULL;
            }

            /* Send tx_buffer data when data valid. */
            if (txbuf_ptr != NULL) 
            {
                /* data packed according to data type */
                err = ehm_package_send(p_ehm, &txbuf_ptr->info, txbuf_ptr->data_ptr, txbuf_ptr->data_len);

                /* Move the first element in waiting list to free list. */
                if(0 <= err)
                {
                    ehm_tx_complete(p_ehm);
                }               
            }
        }
        else
        {
            OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_ERROR, "Thread %s: Failed to take semaphore(%d)\n", __FUNCTION__, err);
        }
    }
    
    return NULL;    
}


/*****************************************************************************
 @funcname: ehm_rx_thread_entry
 @brief   : ehm rx  process thread module
 @param   : void *param
 @return  :	none
*****************************************************************************/
void * ehm_rx_thread_entry(ehm_envar_t *p_ehm)
{
    int ret = 0;
    ehm_rxbuf_t * rxbuf;
    ehm_rxinfo_t *p_node_info;
    list_head_t *p_rxbuf_waiting_list = &p_ehm->rxbuf_waiting_list;
    uint32_t data_time;


    /* Print thread trace. */
    OSAL_MODULE_DBGPRT(MODULE_NAME, OSAL_DEBUG_TRACE, "Thread %s: ---->\n", __FUNCTION__);

    while(1)
    {
    	rxbuf = ehm_get_rxbuf();
    	p_node_info = (ehm_rxinfo_t *)(&(rxbuf->info));;

        
    	//ret = comport_recv(rxbuf->data_ptr, EHM_RX_LEN);
    	if(ret > 0)
    	{
    		data_time = osal_get_systemtime();
    		p_node_info->data_systime = data_time;
    		rxbuf->data_len = ret;
            
    		list_add_tail(&rxbuf->list, p_rxbuf_waiting_list);
            
    		if (p_ehm->queue_ehm_main) 
            {
    			ehm_add_event_queue(p_ehm, EHM_MSG_VSA_ANALY_DATA, 0, 0, NULL);
    		}
    	}
    }

    return NULL;
}



/*****************************************************************************
 @funcname: ehm_init
 @brief   : ehm module init 
 @param   : void  
 @return  : 
*****************************************************************************/
void ehm_init(ehm_envar_t *p_ehm)
{
	int ret = 0;
    uint8_t i = 0;


    /* Open uart module. */
   // ret = comport_init();
    if(ret < 0)
    {
    	osal_printf("comport_init error\n");
    }

    
    /* Initialize the txbuf queue. */
    INIT_LIST_HEAD(&p_ehm->txbuf_waiting_list);
    INIT_LIST_HEAD(&p_ehm->txbuf_free_list);
    for(i = 0; i < (sizeof(p_ehm->txbuf) / sizeof(p_ehm->txbuf[0])); i++)
    {
        list_add_tail(&p_ehm->txbuf[i].list, &p_ehm->txbuf_free_list);
    }

    /* Initialize the rxbuf queue. */
    INIT_LIST_HEAD(&p_ehm->rxbuf_waiting_list);
    INIT_LIST_HEAD(&p_ehm->rxbuf_free_list);
    for(i = 0; i < (sizeof(p_ehm->rxbuf) / sizeof(p_ehm->rxbuf[0])); i++)
    {
        list_add_tail(&p_ehm->rxbuf[i].list, &p_ehm->rxbuf_free_list);
    }

    
    /* ehm main process event queue. */
    p_ehm->queue_ehm_main = osal_queue_create("q-ehm", EHM_QUEUE_SIZE, EHM_MQ_MSG_SIZE);
    osal_assert(p_ehm->queue_ehm_main != NULL);

    /* ehm module main process. */
    p_ehm->task_ehm_main = osal_task_create("t-ehm-main", ehm_main_thread_entry, p_ehm, EHM_MAIN_THREAD_STACK_SIZE, EHM_MAIN_THREAD_PRIORITY);
    osal_assert(p_ehm->task_ehm_main !=NULL);


    /* ehm module tx thread related. */
    p_ehm->sem_ehm_tx = osal_sem_create("sem-ehmtx", 0);
    osal_assert(p_ehm->sem_ehm_tx != NULL);

    p_ehm->task_ehm_tx = osal_task_create("t-ehm-tx", (void *(*)(void *))ehm_tx_thread_entry,p_ehm, EHM_TX_THREAD_STACK_SIZE, EHM_TX_THREAD_PRIORITY);
    osal_assert(p_ehm->task_ehm_tx != NULL);

    /* ehm module rx thread related. */
    p_ehm->sem_ehm_rx = osal_sem_create("sem-ehmrx", 0);
    osal_assert(p_ehm->sem_ehm_rx != NULL);

    p_ehm->task_ehm_rx = osal_task_create("t-ehm-tx", (void *(*)(void *))ehm_rx_thread_entry,p_ehm, EHM_RX_THREAD_STACK_SIZE, EHM_RX_THREAD_PRIORITY);
    osal_assert(p_ehm->task_ehm_tx != NULL);

}


