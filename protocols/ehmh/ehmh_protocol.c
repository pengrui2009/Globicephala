/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : ehmh_protocol.c
 @brief  : ehmh protocol parse api
 @author : pengrui
 @history:
           2017-11-23      pengrui    Created file
           2017-11-23      pengrui    waiting for finish rtcm decode
           ...
******************************************************************************/
#include "ehmh_element.h"
#include "ehmh_protocol.h"

/******************************************************************************
*    函数:    encode_nb_node_summary_infor
*    功能:    组邻车节点概要信息
*    参数:
            buffer_ptr                -    缓冲区buffer
*    返回:
            =0                        -    成功
            -ERR_SYS                -    系统错误
            -ERR_INVAL                -    接口错误
            -ERR_BUSY                -    已断开

*    说明:    无
******************************************************************************/
static int8_t encode_nb_node_summary_infor(nb_node_summary_decode_infor_st_ptr info_ptr, uint8_t *data_ptr, uint16_t *len_ptr)
{
    int8_t result = 0;
    nb_node_summary_encode_infor_st_ptr status_ptr = NULL;

    if((NULL == info_ptr) || (NULL == data_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (nb_node_summary_encode_infor_st_ptr)data_ptr;
    memcpy(status_ptr->node_id, info_ptr->node_id, sizeof(info_ptr->node_id));
    // wating for finished
#if 0
    status_ptr->longitudinal_dis = ;
    status_ptr->lateral_dis   = ;
    status_ptr->angle = ;
    status_ptr->velocity = ;
    status_ptr->signalstrength = ;
    status_ptr->alert_flag  = ;
#endif
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    encode_nb_node_detail_infor
*    功能:    组邻车节点详细信息
*    参数:
            buffer_ptr                -    缓冲区buffer
*    返回:
            =0                        -    成功
            -ERR_SYS                -    系统错误
            -ERR_INVAL                -    接口错误
            -ERR_BUSY                -    已断开

*    说明:    无
******************************************************************************/
static int8_t encode_nb_node_detail_infor(nb_node_detail_decode_infor_st_ptr info_ptr, uint8_t *data_ptr, uint16_t *len_ptr)
{
    int result = 0;
    nb_node_detail_encode_infor_st_ptr status_ptr = NULL;
    
    if((NULL == info_ptr) || (NULL == data_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (nb_node_detail_encode_infor_st_ptr)data_ptr;
    
    memcpy(status_ptr->node_id, info_ptr->node_id, sizeof(info_ptr->node_id));
    // wating for finished
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    encode_basic_vehicle_status
*    功能:    组车辆状态状态基本信息
*    参数:
            buffer_ptr                -    缓冲区buffer
*    返回:
            =0                        -    成功
            -ERR_SYS                -    系统错误
            -ERR_INVAL                -    接口错误
            -ERR_BUSY                -    已断开

*    说明:    无
******************************************************************************/
static int8_t encode_basic_vehicle_status(msg_decode_vehicle_basic_status_st_ptr info_ptr, uint8_t *data_ptr, uint16_t *len_ptr)
{
    int result = 0;
    msg_encode_vehicle_basic_status_st_ptr status_ptr = NULL;

    if((NULL == info_ptr) || (NULL == data_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (msg_encode_vehicle_basic_status_st_ptr)data_ptr;
    status_ptr->msg_id = info_ptr->msg_id;

    //waiting for finished
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    encode_nb_vehicle_alert
*    功能:    组车辆告警信息
*    参数:
            buffer_ptr                -    缓冲区buffer
*    返回:
            =0                        -    成功
            -ERR_SYS                -    系统错误
            -ERR_INVAL                -    接口错误
            -ERR_BUSY                -    已断开

*    说明:    无
******************************************************************************/
static int8_t encode_nb_vehicle_alert(msg_decode_nb_vehicle_alert_st_ptr info_ptr, uint8_t *data_ptr, uint16_t *len_ptr)
{
    int result = 0;
    msg_encode_nb_vehicle_alert_st_ptr status_ptr = NULL;
    
    if((NULL == info_ptr) || (NULL == data_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr->msg_id = info_ptr->msg_id;

    //waiting for finished
  
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    encode_nb_node_msg
*    功能:    组邻车节点信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int encode_nb_node_msg(msg_decode_vehicle_nb_status_st_ptr info_ptr, uint8_t *data_ptr, uint16_t *len_ptr)
{
    int result = 0;
    msg_encode_vehicle_nb_status_st_ptr status_ptr = NULL;

    if((NULL == info_ptr) || (NULL == data_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (msg_encode_vehicle_nb_status_st_ptr)data_ptr;
    status_ptr->msg_id         = info_ptr->msg_id;
    status_ptr->systime_ms  = cv_ntohll(info_ptr->systime_ms);
    status_ptr->nodenumber  = info_ptr->nodenumber;
    status_ptr->node_infor_type = info_ptr->node_infor_type;

    data_ptr += MSG_ENCODE_VEHICLE_NB_STATUS_ST_LEN;
    *len_ptr += MSG_ENCODE_VEHICLE_NB_STATUS_ST_LEN;
    if(NODE_INFOR_TYPE_SUMMARY == status_ptr->node_infor_type)
    {
        nb_node_summary_decode_infor_st_ptr summary_info_ptr = NULL;
        result = encode_nb_node_summary_infor(summary_info_ptr, data_ptr, len_ptr);
        //waiting for finished
    }else if(NODE_INFOR_TYPE_DETAIL == status_ptr->node_infor_type){
        nb_node_detail_decode_infor_st_ptr detail_info_ptr = NULL;
        result = encode_nb_node_detail_infor(detail_info_ptr, data_ptr, len_ptr);
        //waiting for finished
    }else{

    }

    *len_ptr += MSG_ENCODE_TRAFFICLAMP_SPEED_GUIDE_ST_LEN;
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    encode_trafficlamp_speed_guide_msg
*    功能:    组交通信号灯速度引导数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int encode_trafficlamp_speed_guide_msg(msg_decode_trafficlamp_speed_guide_st_ptr info_ptr, uint8_t *data_ptr, uint16_t *len_ptr)
{
    int result = 0;
    msg_encode_trafficlamp_speed_guide_st_ptr status_ptr = NULL;

    if((NULL == info_ptr) || (NULL == data_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (msg_encode_trafficlamp_speed_guide_st_ptr)data_ptr;
    status_ptr->msg_id       = info_ptr->msg_id;
    status_ptr->leftlamp     = info_ptr->leftlamp;
    status_ptr->straightlamp = info_ptr->straightlamp;
    status_ptr->rightlamp    = info_ptr->rightlamp;
    status_ptr->timers       = cv_ntohs(info_ptr->timers);
    status_ptr->maxvelocity  = encode_absolute_velocity(info_ptr->maxvelocity);
    status_ptr->minvelocity  = encode_absolute_velocity(info_ptr->minvelocity);

    *len_ptr += MSG_ENCODE_TRAFFICLAMP_SPEED_GUIDE_ST_LEN;
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    decode_basic_vehicle_status
*    功能:    解析本地车辆基本状态信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int decode_basic_vehicle_status(msg_decode_vehicle_basic_status_st_ptr info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int                                 result = 0;
    msg_encode_vehicle_basic_status_st_ptr status_ptr = (msg_encode_vehicle_basic_status_st_ptr)data_ptr;

    if((NULL == info_ptr) || (NULL == data_ptr) || (MSG_DECODE_VEHICLE_BASIC_STATUS_ST_LEN > data_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    /*msg id*/
    info_ptr->msg_id = status_ptr->msg_id;

    /*systime ms*/
    info_ptr->systime_ms = cv_ntohll(status_ptr->systime_ms);

    /* node id. */
    memcpy(info_ptr->node_id, status_ptr->node_id, sizeof(status_ptr->node_id));

    /* position. */
    info_ptr->latitude  = decode_latitude(status_ptr->position.latitude);
    info_ptr->longitude = decode_longitude(status_ptr->position.longitude);
    info_ptr->elevation = decode_elevation(status_ptr->position.elevation);

    /* position accuracy. */
    info_ptr->posaccu.semi_major_accu = decode_semimajor_axis_accuracy(status_ptr->posaccu.semimajoraxisAccu);
    info_ptr->posaccu.semi_minor_accu = decode_semiminor_axis_accuracy(status_ptr->posaccu.semiminoraxisAccu);
    info_ptr->posaccu.semi_major_orientation = decode_semimajor_axis_orientation(cv_ntohs(status_ptr->posaccu.semimajorAxisOrien));

    /* velocity. */
    info_ptr->velocity = decode_absolute_velocity(status_ptr->velocity);

    /* heading. */
    info_ptr->angle = decode_angle(status_ptr->angle);

ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    decode_full_vehicle_status
*    功能:    解析本地车辆完整状态信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int decode_full_vehicle_status(msg_decode_vehicle_full_status_st_ptr info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int                                 result = 0;
    msg_encode_vehicle_full_status_st_ptr  status_ptr = NULL;

    if((NULL == info_ptr) || (NULL == data_ptr) || (MSG_DECODE_VEHICLE_FULL_STATUS_ST_LEN > data_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    status_ptr = (msg_encode_vehicle_full_status_st_ptr)data_ptr;

    /* msg id*/
    info_ptr->msg_id = info_ptr->msg_id;
    
    /* systime ms*/
    info_ptr->systime_ms = cv_ntohll(info_ptr->systime_ms);

    /* node id. */
    memcpy(info_ptr->node_id, status_ptr->node_id, sizeof(status_ptr->node_id));

    /* position. */
    info_ptr->latitude  = decode_latitude(status_ptr->position.latitude);
    info_ptr->longitude = decode_longitude(status_ptr->position.longitude);
    info_ptr->elevation = decode_elevation(status_ptr->position.elevation);

    /* position accuracy. */
    info_ptr->posaccu.semi_major_accu = decode_semimajor_axis_accuracy(status_ptr->posaccu.semimajoraxisAccu);
    info_ptr->posaccu.semi_minor_accu = decode_semiminor_axis_accuracy(status_ptr->posaccu.semiminoraxisAccu);
    info_ptr->posaccu.semi_major_orientation = decode_semimajor_axis_orientation(status_ptr->posaccu.semimajorAxisOrien);

    /* transmission state. */
    info_ptr->tran_state = status_ptr->tran_state;

    /* velocity. */
    info_ptr->velocity = decode_absolute_velocity(status_ptr->velocity);

    /* angle. */
    info_ptr->angle = decode_angle(status_ptr->angle);

    /* steer wheel angle. */
    info_ptr->steerwa = status_ptr->steerwa;

    /* acceleration set4way. */
    info_ptr->acce4way.longitudinal = decode_acceleration(status_ptr->acce4way.lonacce);
    info_ptr->acce4way.lateral = decode_acceleration(status_ptr->acce4way.latacce);
    info_ptr->acce4way.vertical = decode_vertical_acceleration(status_ptr->acce4way.veracce);
    info_ptr->acce4way.yaw_rate = decode_yawrate(status_ptr->acce4way.yawrate);

    /* brake system status. */
    info_ptr->braksta.wheel_brakes.wheel_brake_bit.unavailable = status_ptr->braksta.wheel_brakes.wheel_brake_bit.unavailable;
    info_ptr->braksta.wheel_brakes.wheel_brake_bit.leftfront   = status_ptr->braksta.wheel_brakes.wheel_brake_bit.leftfront;
    info_ptr->braksta.wheel_brakes.wheel_brake_bit.leftrear    = status_ptr->braksta.wheel_brakes.wheel_brake_bit.leftrear;
    info_ptr->braksta.wheel_brakes.wheel_brake_bit.rightfront  = status_ptr->braksta.wheel_brakes.wheel_brake_bit.rightfront;
    info_ptr->braksta.wheel_brakes.wheel_brake_bit.rightrear   = status_ptr->braksta.wheel_brakes.wheel_brake_bit.rightrear;

    info_ptr->braksta.traction = status_ptr->braksta.traction;
    info_ptr->braksta.abs      = status_ptr->braksta.abs;
    info_ptr->braksta.scs      = status_ptr->braksta.scs;
    info_ptr->braksta.brakeboost = status_ptr->braksta.brakeboost;
    info_ptr->braksta.auxbrakes = status_ptr->braksta.auxbrakes;

    /* exterior light. */
    status_ptr->exterlight.exterior_lights_word = cv_ntohs(status_ptr->exterlight.exterior_lights_word);
    
    info_ptr->exterlight.exterior_lights_bit.lowbeamheadlight     = status_ptr->exterlight.exterior_lights_bit.lowbeamheadlight;
    info_ptr->exterlight.exterior_lights_bit.highbeamheadlight    = status_ptr->exterlight.exterior_lights_bit.highbeamheadlight;
    info_ptr->exterlight.exterior_lights_bit.leftturnsignallight  = status_ptr->exterlight.exterior_lights_bit.leftturnsignallight;
    info_ptr->exterlight.exterior_lights_bit.rightturnsignallight = status_ptr->exterlight.exterior_lights_bit.rightturnsignallight;
    info_ptr->exterlight.exterior_lights_bit.hazardsignallight    = status_ptr->exterlight.exterior_lights_bit.hazardsignallight;
    info_ptr->exterlight.exterior_lights_bit.automaticlight       = status_ptr->exterlight.exterior_lights_bit.automaticlight;
    info_ptr->exterlight.exterior_lights_bit.daytimerunninglight  = status_ptr->exterlight.exterior_lights_bit.daytimerunninglight;
    info_ptr->exterlight.exterior_lights_bit.foglighton           = status_ptr->exterlight.exterior_lights_bit.foglighton;
    info_ptr->exterlight.exterior_lights_bit.parkinglight      = status_ptr->exterlight.exterior_lights_bit.parkinglight;

ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    函数:    decode_vehicle_static_infor
*    功能:    解析本地车辆静态信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int decode_vehicle_static_infor(msg_decode_vehicle_static_info_st_ptr info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int                                 result = 0;
    msg_encode_vehicle_static_info_st_ptr  status_ptr = NULL;

    if((NULL == info_ptr) || (NULL == data_ptr) || (MSG_DECODE_VEHICLE_STATIC_INFO_ST_LEN > data_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (msg_encode_vehicle_static_info_st_ptr)data_ptr;

    /*msg id*/
    info_ptr->msg_id = status_ptr->msg_id;

    /*systime ms*/
    info_ptr->systime_ms = cv_ntohll(status_ptr->systime_ms);
    
    /* Vehicle type. */
    info_ptr->vehicle_type = status_ptr->vehicle_type;

    /* Vehicle size. */
    info_ptr->width  = decode_vehicle_width(status_ptr->vehicle_size.vehiclewidth);
    info_ptr->length = decode_vehicle_length(status_ptr->vehicle_size.vehiclelength);

ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    函数:    decode_local_vehicle_alert_set
*    功能:    解析本地车辆告警设置信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int decode_local_vehicle_alert_set(msg_decode_local_vehicle_alert_st_ptr info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int                                 result = 0;
    msg_encode_local_vehicle_alert_st_ptr  status_ptr = NULL;

    if((NULL == info_ptr) || (NULL == data_ptr) || (MSG_DECODE_LOCAL_VEHICLE_ALERT_ST_LEN > data_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (msg_encode_local_vehicle_alert_st_ptr)data_ptr;

    /* msg id */
    info_ptr->msg_id = status_ptr->msg_id;

    /*vehicle alert set*/
    info_ptr->vehicle_alert_set.vehicle_alert_words = cv_ntohl(status_ptr->vehicle_alert_set.vehicle_alert_words);
    
ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    函数:    decode_system_curtime_set
*    功能:    解析本地车辆告警设置信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int decode_system_curtime_set(msg_decode_system_curtime_st_ptr info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int                    result = 0;
    msg_encode_system_curtime_st_ptr status_ptr = NULL;

    if((NULL == info_ptr) || (NULL == data_ptr) || (MSG_ENCODE_SYSTEM_CURTIME_ST_LEN > data_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (msg_encode_system_curtime_st_ptr)data_ptr;

    /*msg id*/
    info_ptr->msg_id = status_ptr->msg_id;
    
    /* system time ms*/
    info_ptr->systime_ms = cv_ntohll(status_ptr->systime_ms);

ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    decode_system_curtime_set
*    功能:    解析本地车辆告警设置信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
int decode_v2x_msg(ehmh_msgtype_en_ptr msgtype_ptr, void *info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int result = 0;
    uint8_t msg_id = 0;
    
    /* Error detection. */
    if((NULL == msgtype_ptr) || (NULL == info_ptr) || (NULL == data_ptr))
    {
        osal_printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    //get v2x child msgtype
    msg_id = *(uint8_t *)data_ptr;
    
    switch(msg_id)
    {
        case EHMH_V2X_BASIC_VEHICLE_STATUS_MSGTYPE:
        {
            *msgtype_ptr = EHMH_V2X_BASIC_VEHICLE_STATUS_MSGTYPE;
            result = decode_basic_vehicle_status(info_ptr, data_ptr, data_len);
            if(result < 0)
            {
                fprintf(stderr, "%s %d decode_basic_vehicle_status error:%d\n", __FUNCTION__, __LINE__, result);
                goto ERR_EXIT;
            }
            break;
        }
        case EHMH_V2X_FULL_VEHICLE_STATUS_MSGTYPE:
        {
            *msgtype_ptr = EHMH_V2X_BASIC_VEHICLE_STATUS_MSGTYPE;
            result = decode_full_vehicle_status(info_ptr, data_ptr, data_len);
            if(result < 0)
            {
                fprintf(stderr, "%s %d decode_full_vehicle_status error:%d\n", __FUNCTION__, __LINE__, result);
                goto ERR_EXIT;
            }
            break;
        }
        case EHMH_V2X_VEHICLE_STATIC_INFO_MSGTYPE:
        {
            *msgtype_ptr = EHMH_V2X_VEHICLE_STATIC_INFO_MSGTYPE;
            result = decode_vehicle_static_infor(info_ptr, data_ptr, data_len);
            if(result < 0)
            {
                fprintf(stderr, "%s %d decode_vehicle_static_infor error:%d\n", __FUNCTION__, __LINE__, result);
                goto ERR_EXIT;
            }
            break;
        }
        case EHMH_V2X_LOCAL_VEHICLE_ALERT_MSGTYPE:
        {
            *msgtype_ptr = EHMH_V2X_LOCAL_VEHICLE_ALERT_MSGTYPE;
            result = decode_local_vehicle_alert_set(info_ptr, data_ptr, data_len);
            if(result < 0)
            {
                fprintf(stderr, "%s %d decode_local_vehicle_alert_set error:%d\n", __FUNCTION__, __LINE__, result);
                goto ERR_EXIT;
            }
            break;
        }
        case EHMH_V2X_SYSTEM_CURTIME_MSGTYPE:
        {
            *msgtype_ptr = EHMH_V2X_SYSTEM_CURTIME_MSGTYPE;
            result = decode_system_curtime_set(info_ptr, data_ptr, data_len);
            if(result < 0)
            {
                fprintf(stderr, "%s %d decode_system_curtime_set error:%d\n", __FUNCTION__, __LINE__, result);
                goto ERR_EXIT;
            }
            break;
        }
        default:
        {
            result = -ERR_INVAL;
            break;
        }
    }
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    decode_dsrc_msg
*    功能:    解析DSRC信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int decode_dsrc_msg(ehmh_msgtype_en_ptr msgtype_ptr, msg_decode_wsmp_payload_info_st_ptr info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int result = 0;
    msg_encode_wsmp_payload_info_st_ptr status_ptr = NULL;
    
    if((NULL == info_ptr) || (NULL == data_ptr) || (MSG_ENCODE_SYSTEM_CURTIME_ST_LEN > data_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (msg_encode_wsmp_payload_info_st_ptr)data_ptr;

    *msgtype_ptr = EHMH_V2X_DSRC_MSGTYPE;
    info_ptr->seqnum = cv_ntohs(status_ptr->seqnum);
    info_ptr->payloadlen = cv_ntohs(status_ptr->payloadlen);
    info_ptr->timems = cv_ntohll(status_ptr->timems);
    memcpy(info_ptr->ueid, status_ptr->ueid, sizeof(status_ptr->ueid));
    info_ptr->txpower = cv_ntohs(status_ptr->txpower);
    info_ptr->gpstime = cv_ntohll(status_ptr->gpstime);
    info_ptr->longtitude = decode_longitude(status_ptr->longtitude);
    info_ptr->latitude = decode_latitude(status_ptr->latitude);
    info_ptr->elevation = decode_elevation(status_ptr->elevation);
    info_ptr->angle = decode_angle(status_ptr->angle);
    info_ptr->velocity = decode_absolute_velocity(status_ptr->velocity);
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    decode_gps_msg
*    功能:    解析GPS信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int decode_nmea_rtcm_info(msg_decode_rtcm_correction_st_ptr info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int result = 0;
    msg_encode_rtcm_correction_st_ptr status_ptr = NULL;
    
    if((NULL == info_ptr) || (NULL == data_ptr) || (MSG_ENCODE_SYSTEM_CURTIME_ST_LEN > data_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (msg_encode_rtcm_correction_st_ptr)data_ptr;

    info_ptr->msg_id = status_ptr->msg_id;
    info_ptr->rtcm_ver = status_ptr->rtcm_ver;

    data_ptr += MSG_ENCODE_SYSTEM_CURTIME_ST_LEN;
    data_len -= MSG_ENCODE_SYSTEM_CURTIME_ST_LEN;

    //wait for finished
    {
        //rtcm_message_st_ptr rtcm_ptr = (rtcm_message_st_ptr)data_ptr;
    }
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    decode_gps_msg
*    功能:    解析GPS信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int decode_nmea_raw_info(msg_nmea_decode_raw_info_st_ptr info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int result = 0;
    msg_encode_nmea_raw_info_st_ptr status_ptr = NULL;
    

    if((NULL == info_ptr) || (NULL == data_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    status_ptr = (msg_encode_nmea_raw_info_st_ptr)data_ptr;

    memcpy(info_ptr, status_ptr, data_len);
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    函数:    decode_gps_msg
*    功能:    解析GPS信息数据
*    参数:
            data_ptr            -    数据指针
            data_len            -    数据长度
            time                -    接收数据帧时间
*    返回:
            =0                    -    成功
            -ERR_SYS            -    系统错误
            -ERR_INVAL            -    接口错误
            -ERR_BUSY            -    已断开

*    说明:    无
******************************************************************************/
static int decode_gps_msg(ehmh_msgtype_en_ptr msgtype_ptr, void *info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int result = 0;
    uint8_t msgtype = 0;

    if((NULL == msgtype_ptr) || (NULL == info_ptr) || (NULL == data_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    msgtype = data_ptr[0];
    
    switch(msgtype)
    {
    case 1:
        *msgtype_ptr = EHMH_V2X_GPS_RTCM_CORRECTION_INFO_MSGTYPE;
        result = decode_nmea_rtcm_info(info_ptr, data_ptr, data_len);
        if(result)
        {
            fprintf(stderr, "decode_nmea_rtcm_info error:%d\n", result);
        }
        break;
    case 2:
        *msgtype_ptr = EHMH_V2X_GPS_NMEA_RAW_INFO_MSGTYPE;
        result = decode_nmea_raw_info(info_ptr, data_ptr, data_len);
        if(result)
        {
            fprintf(stderr, "decode_nmea_raw_info error:%d\n", result);
        }
        break;
    default:
        result = -ERR_INVAL;
        break;
    }

ERR_EXIT:

    return result;
}


/*FUNCTION*****************************************************************
* 
* Returned : ERR_OK when analyse succeed Or negative error code when failure. 
* Comments : Analyse the specific ehm buffer structure.
*
*END*********************************************************************/
int ehmh_decode(ehmh_msgtype_en_ptr msgtype_ptr, void *info_ptr, uint8_t *data_ptr, uint16_t data_len)
{
    int                         ret = 0x00;
    frame_msg_header_st_ptr msg_ptr = NULL;
    
    /* Error detection. */
    if((NULL == msgtype_ptr) || (NULL == info_ptr) || (NULL == data_ptr) || (FRAME_MSG_HEADER_ST_LEN > data_len))
    {
        osal_printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    msg_ptr = (frame_msg_header_st_ptr)data_ptr;

    /* Check message source. */
    if((msg_ptr->mark != MSG_HEADER_MARK) || (msg_ptr->src != MSG_SRC_HOST))
    {
        osal_printf("[%s %d]: Message error. mark = %d, src = %d. \n", __FUNCTION__, __LINE__, msg_ptr->mark, msg_ptr->src);
        ret = ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Update effective data address and length. */
    data_len -= FRAME_MSG_HEADER_ST_LEN;
    data_ptr += FRAME_MSG_HEADER_ST_LEN;

    /* Parse message based on type. */
    switch(msg_ptr->type)
    {
        case MSGTYPE_RESERVED:
        case MSGTYPE_DEBUG:
        case MSGTYPE_SYS_MANAGE:
        {
            ret = -1;                                                                        
            osal_printf("[%s %d]: Not supposed ehm message. msg_type = %d. \n", __FUNCTION__, __LINE__, msg_ptr->type);
            break;
        }
        case MSGTYPE_V2X_APPLY:
        {
            ret = decode_v2x_msg(msgtype_ptr, info_ptr, data_ptr, data_len);
            if(ERR_OK != ret)
            {
                osal_printf("[%s %d]: Parse v2x apply msg error. msg_type = %d. \n", __FUNCTION__, __LINE__, msg_ptr->type);
            }
            break;
        }
        case MSGTYPE_RAWDATA_DSRC:
        {
            ret = decode_dsrc_msg(msgtype_ptr, info_ptr, data_ptr, data_len); 
            if(ERR_OK != ret)
            {
                osal_printf("[%s %d]: Parse v2x apply msg error. msg_type = %d. \n", __FUNCTION__, __LINE__, msg_ptr->type);
            }
            break;
        }
        case MSGTYPE_GPS_APPLY:
        {
            ret = decode_gps_msg(msgtype_ptr, info_ptr, data_ptr, data_len);    
            if(ERR_OK != ret)
            {
                osal_printf("[%s %d]: Parse v2x apply msg error. msg_type = %d. \n", __FUNCTION__, __LINE__, msg_ptr->type);
            }
            break;
        }
        default:
        {
            ret = -1; 
            osal_printf("[%s %d]: Not supposed ehm message. msg_type = %d. \n", __FUNCTION__, __LINE__, msg_ptr->type);
            break;
        }
    }

ERR_EXIT:
    
    return ret;
}

int ehmh_encode(ehmh_msgtype_en msgtype, void *info_ptr, uint8_t *data_ptr, uint16_t *len_ptr)
{
    int result = 0;
    frame_msg_header_st_ptr msg_ptr = NULL;
    
    if((EHMH_V2X_MSGTYPE_END <= msgtype) || (NULL == info_ptr) || (NULL == data_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    msg_ptr = (frame_msg_header_st_ptr)data_ptr;
    msg_ptr->mark = MSG_HEADER_MARK;
    msg_ptr->src  = MSG_SRC_V2X;
    
    data_ptr += FRAME_MSG_HEADER_ST_LEN;
    *len_ptr += FRAME_MSG_HEADER_ST_LEN;
    switch(msgtype)
    {
    case EHMH_V2X_NB_NODE_INFO_MSGTYPE:
        msg_ptr->type = MSGTYPE_V2X_APPLY;
        result = encode_nb_node_msg(info_ptr, data_ptr, len_ptr);
        if(result)
        {
            osal_printf("encode_nb_node_msg error:%d\n", result);
        }
    case EHMH_V2X_BASIC_VEHICLE_STATUS_MSGTYPE:
        msg_ptr->type = MSGTYPE_V2X_APPLY;
        result = encode_basic_vehicle_status(info_ptr, data_ptr, len_ptr);
        if(result)
        {
            osal_printf("encode_basic_vehicle_status error:%d\n", result);
        }

        break;
    case EHMH_V2X_NB_VEHICLE_ALERT_MSGTYPE:
        msg_ptr->type = MSGTYPE_V2X_APPLY;
        result = encode_nb_vehicle_alert(info_ptr, data_ptr, len_ptr);
        if(result)
        {
            osal_printf("encode_nb_vehicle_alert error:%d\n", result);
        }
        break;
    case EHMH_V2X_TRAFFICLAMP_SPEED_GUIDE_MSGTYPE:
        msg_ptr->type = MSGTYPE_V2X_APPLY;
        result = encode_trafficlamp_speed_guide_msg(info_ptr, data_ptr, len_ptr);
        if(result)
        {
            osal_printf("encode_trafficlamp_speed_guide_msg error:%d\n", result);
        }
        break;

    case EHMH_V2X_GPS_RTCM_CORRECTION_INFO_MSGTYPE:
        break;

    default:
        result = -ERR_INVAL;
        break;
    }
ERR_EXIT:
    
    return result;
}


