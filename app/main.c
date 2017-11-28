#include <math.h>

#include "drv_main.h"
#include "main.h"

#include "cv_msg_wsmp.h"
#include "cv_msg_frame.h"
#include "net.h"

/* Main driver pointer. */
drv_main_st_ptr DrvMainPtr = NULL;
osal_task_t  WnetTask = 0;
osal_task_t  EhmTask = 0;


msg_decode_vehicle_basic_status_st vehicle_basic_status_info;
info_rsu_st rsu_info;



MSG_MessageFrame_st MsgSt = { 0 };


/* Switch two vehicle's position into distance according to earth. Unit: degree to meter. */
float pos_to_distance(double lat1, double lng1, double lat2, double lng2)
{
    float distance = 0;
    double radLat1 = DEGREE_TO_RAD(lat1);
    double radLat2 = DEGREE_TO_RAD(lat2);
    double radLng1 = DEGREE_TO_RAD(lng1);
    double radLng2 = DEGREE_TO_RAD(lng2);

    double a = radLat1 - radLat2;
    double b = radLng1 - radLng2;

    /* Position to distance from google. */
    distance = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1) * cos(radLat2) * pow(sin(b/2),2)));
    distance = distance * EARTH_RADIUS;

    return distance;
}

#if 0
/* Connect two vehicles into line and calculate the line angle between north in clockwise.  
   Caution: the angle based on core vehicle. Unit: degree to degree. */
static float posline_to_angle(double core_lat, double core_lng, double circle_lat, double circle_lng)
{
    float     dis1_2 = 0;
    float     dis2_3 = 0;
    
    float line_angle = 0;;

    
    /* Calculate distance for vehicle 1&2 and 2&3. */
    dis1_2 = pos_to_distance(core_lat, core_lng, circle_lat, circle_lng);
    dis2_3 = pos_to_distance(circle_lat, circle_lng, core_lat, circle_lng);

    /* Get angle in triangle. */
    if(dis1_2 != 0)
    {
        line_angle = RAD_TO_DEGREE(acos(dis2_3 / dis1_2));
    }
    else
    {
        line_angle = 0;
    }

    /* calculate the relative angle against north, clockwise  */
    if(core_lat <= circle_lat)
    {
        if(core_lng <= circle_lng)
        {
            /* North && East. */
        }
        else
        {
            /* North && West. */
            line_angle = 360 - line_angle;
        }
    }
    else
    {
        if(core_lng <= circle_lng)
        {
            /* South && East. */
            line_angle = 180 - line_angle;
        }
        else
        {
            /* South && West. */
            line_angle = 180 + line_angle;
        }
    }

    return line_angle;
}
#endif

/******************************************************************************
*    函数:    recommend_speed_calc
*    功能:    Calculate the suggested speed
*    参数:
            lat1              - Location 1 latitude  Uint: degree
            lng1              - Location 1 longitude Uint: degree
            lat2              - Location 2 latitude  Uint: degree
            lng2              - Location 2 longitude Uint: degree
            time                - The current remaining time  Unit: second
*    返回:
            Recommended speed value  unit: km/h
*    说明:    无
******************************************************************************/

float recommend_speed_calc(double lat1, double lng1, double lat2, double lng2, uint16_t time)
{
    float speed = 0.0;
    float distance = 0.0;

    distance = pos_to_distance(lat1, lng1, lat2, lng2);
    
    if(time == 0)
        return speed;

    speed = (float)abs(distance/time);
    speed = speed * 3.6;

    return speed;
}

/******************************************************************************
*    函数:    direction_from_angle
*    功能:    Determine the direction of the vehicle based on the angle
*    参数:
            speed                -    Current speed
            angle             -    Current angle
            direction            -    Judge the direction
*    返回:
            =0                    -    成功
            -ERR_INVAL            -    接口错误
*    说明:    无
******************************************************************************/
int direction_from_angle(float speed,float angle, direction_vehicle_em_ptr direction)
{
    int result = ERR_OK;

    if(speed < 5.0)
        return -ERR_INVAL;
    
    if (angle <= 10.0){
        *direction = DIRECTION_NORTH;
    }
    else if(angle < 80.0){
        *direction = DIRECTION_NORTH_EAST;
    }
    else if(angle <= 100.0){
        *direction = DIRECTION_EAST;
    }
    else if(angle < 170.0){
        *direction = DIRECTION_SOUTH_EAST;
    }
    else if(angle <= 190.0){
        *direction = DIRECTION_SOUTH;
    }
    else if(angle < 260.0){
        *direction = DIRECTION_SOUTH_WEST;
    }
    else if(angle <= 280.0){
        *direction = DIRECTION_WEST;
    }
    else if(angle < 350.0){
        *direction = DIRECTION_NORTH_WEST;
    }
    else{
        *direction = DIRECTION_NORTH;
    }
    
    return result;
}

int phase_parse(DF_PhaseList_st_ptr phaseList_data, msg_decode_trafficlamp_speed_guide_st_ptr trafficLampSpeedGuide_ptr)
{
    int result = ERR_OK;
    int i = 0, j = 0;
    uint8_t light_flag = 0; //1:light valid,0:light invalid

    if((phaseList_data == NULL) || (trafficLampSpeedGuide_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
        
    for(i = 0; i < phaseList_data->pointNum; i++)
    {
        os_printf("phase id[%d] = %d\n", i, phaseList_data->array[i].id);
        //memset(trafficLampSpeedGuide_ptr, 0, (phaseList_data->pointNum)*sizeof(msg_decode_trafficlamp_speed_guide_st));
        for(j = 0; j < phaseList_data->array[i].phaseStates.pointNum; j++)
        {
            switch(phaseList_data->array[i].phaseStates.array[j].light)
            {
                case DE_LightState_unavailable:
                case DE_LightState_dark:
                    break;
                case DE_LightState_pre_Movement:
                case DE_LightState_stop_Then_Proceed:
                    if(j == 0)
                        trafficLampSpeedGuide_ptr[i].straightlamp.RedLightStat = TRAFFIC_LIGHT_BLINK;
                    else if(j == 1)
                        trafficLampSpeedGuide_ptr[i].leftlamp.RedLightStat = TRAFFIC_LIGHT_BLINK;
                    else if(j == 2)
                        trafficLampSpeedGuide_ptr[i].rightlamp.RedLightStat = TRAFFIC_LIGHT_BLINK;
                    
                    light_flag = 1;
                    break;
                case DE_LightState_stop_And_Remain:
                    if(j == 0)
                        trafficLampSpeedGuide_ptr[i].straightlamp.RedLightStat = TRAFFIC_LIGHT_ON;
                    else if(j == 1)
                        trafficLampSpeedGuide_ptr[i].leftlamp.RedLightStat = TRAFFIC_LIGHT_ON;
                    else if(j == 2)
                        trafficLampSpeedGuide_ptr[i].rightlamp.RedLightStat = TRAFFIC_LIGHT_ON;
                    
                    light_flag = 1;
                    break;
                case DE_LightState_permissive_Movement_Allowed:
                case DE_LightState_protected_Movement_Allowed:
                    if(j == 0)
                        trafficLampSpeedGuide_ptr[i].straightlamp.GreenLightStat = TRAFFIC_LIGHT_ON;
                    else if(j == 1)
                        trafficLampSpeedGuide_ptr[i].leftlamp.GreenLightStat = TRAFFIC_LIGHT_ON;
                    else if(j == 2)
                        trafficLampSpeedGuide_ptr[i].rightlamp.GreenLightStat = TRAFFIC_LIGHT_ON;

                    light_flag = 1;
                    break;
                case DE_LightState_intersection_clearance:
                    if(j == 0)
                        trafficLampSpeedGuide_ptr[i].straightlamp.YellowLightStat = TRAFFIC_LIGHT_ON;
                    else if(j == 1)
                        trafficLampSpeedGuide_ptr[i].leftlamp.YellowLightStat = TRAFFIC_LIGHT_ON;
                    else if(j == 2)
                        trafficLampSpeedGuide_ptr[i].rightlamp.YellowLightStat = TRAFFIC_LIGHT_ON;

                    light_flag = 1;
                    break;
                case DE_LightState_caution_Conflicting_Traffic:
                    if(j == 0)
                        trafficLampSpeedGuide_ptr[i].straightlamp.YellowLightStat = TRAFFIC_LIGHT_BLINK;
                    else if(j == 1)
                        trafficLampSpeedGuide_ptr[i].leftlamp.YellowLightStat = TRAFFIC_LIGHT_BLINK;
                    else if(j == 2)
                        trafficLampSpeedGuide_ptr[i].rightlamp.YellowLightStat = TRAFFIC_LIGHT_BLINK;

                    light_flag = 1;
                    break;
                default:
                    break;
            }
            if((phaseList_data->array[i].phaseStates.array[j].opt.PhaseState_opt_timing == MSG_OPTIONAL_YES)&&(light_flag))
            {
                trafficLampSpeedGuide_ptr[i].timers = phaseList_data->array[i].phaseStates.array[j].timing.likelyEndTime;
            }
            
        }
    }
        
ERR_EXIT:

    return result;    
}

/*FUNCTION*****************************************************************
* 
* Returned : ERR_OK said success, other values indicate failure.  
* Comments : Process the received message and send it to host.
*
*END*********************************************************************/
int spat_message_deal(MSG_SPAT_st_ptr msg_ptr)
{
    int result = ERR_OK;
    int i = 0;
    drv_main_st_ptr  drv_ptr = GET_DRVMAIN_PTR;
    direction_vehicle_em vehicle_direction;
    msg_intersection_st_ptr intersection_ptr = NULL;
    msg_decode_trafficlamp_speed_guide_st trafficLampSpeedGuide;
    uint8_t send_buf[512];
    uint16_t send_len = 0;
    float speed = 0.0;
    
    if(msg_ptr == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    os_printf("spat msgcnt:%d\n", msg_ptr->msgCnt);
    
    if((intersection_ptr = os_calloc(1, ((sizeof(msg_intersection_st))*(msg_ptr->intersections.pointNum)))) == NULL)
    {
        osal_printf("[%s %d]: Allocate message buffer for msg_intersection error. \n", __FUNCTION__, __LINE__);
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    for(i = 0; i < msg_ptr->intersections.pointNum; i++)
    {
        intersection_ptr[i].intersection_id = msg_ptr->intersections.array[i].intersectionId.id;
        if((intersection_ptr[i].trafficLampSpeedGuide = os_calloc(1, (sizeof(msg_decode_trafficlamp_speed_guide_st)*(msg_ptr->intersections.array[i].phases.pointNum)))) == NULL)
        {
            osal_printf("[%s %d]: Allocate message buffer for trafficLampSpeedGuide error. \n", __FUNCTION__, __LINE__);
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        result = phase_parse(&(msg_ptr->intersections.array[i].phases), intersection_ptr[i].trafficLampSpeedGuide);
        if(result)
            goto ERR_EXIT;
        
    }

    //result = direction_from_angle(vehicle_basic_status_info.velocity, vehicle_basic_status_info.angle, &vehicle_direction);
    vehicle_direction = DIRECTION_EAST;
    //if(result == ERR_OK)
    {
        switch(vehicle_direction)
        {
            case DIRECTION_NORTH:
            case DIRECTION_SOUTH:
                memcpy(&trafficLampSpeedGuide, &(intersection_ptr[0].trafficLampSpeedGuide[1]), sizeof(trafficLampSpeedGuide));
                break;
            case DIRECTION_EAST:
            case DIRECTION_WEST:
                memcpy(&trafficLampSpeedGuide, &(intersection_ptr[0].trafficLampSpeedGuide[0]), sizeof(trafficLampSpeedGuide));
                break;
            default:
                break;
        }
    }

    if(trafficLampSpeedGuide.straightlamp.RedLightStat != 0)
    {
        trafficLampSpeedGuide.maxvelocity = 0.0;
        trafficLampSpeedGuide.minvelocity = 0.0;
    }
    else if(trafficLampSpeedGuide.straightlamp.GreenLightStat || trafficLampSpeedGuide.straightlamp.YellowLightStat)
    {
        speed = recommend_speed_calc(rsu_info.latitude, rsu_info.longitude, vehicle_basic_status_info.latitude, vehicle_basic_status_info.longitude, trafficLampSpeedGuide.timers);
        if((speed == 0) || (speed > RECOMMAND_SPEED_MAX))
        {
            trafficLampSpeedGuide.maxvelocity = 0.0;
            trafficLampSpeedGuide.minvelocity = 0.0;
        }
        else
        {
            trafficLampSpeedGuide.maxvelocity = RECOMMAND_SPEED_MAX;
            trafficLampSpeedGuide.minvelocity = speed;
        }
            
    }

    for(i = 0; i < msg_ptr->intersections.array[0].phases.pointNum; i++)
    {
        osal_printf("intersection_ptr[0].trafficLampSpeedGuide[%d],left=0x%x,straight=0x%x,right=0x%x,timer=%d\n",
                    i,
                    intersection_ptr[0].trafficLampSpeedGuide[i].leftlamp.YellowLightStat,intersection_ptr[0].trafficLampSpeedGuide[i].leftlamp.GreenLightStat,intersection_ptr[0].trafficLampSpeedGuide[i].leftlamp.RedLightStat,
                    intersection_ptr[0].trafficLampSpeedGuide[i].straightlamp.YellowLightStat,intersection_ptr[0].trafficLampSpeedGuide[i].straightlamp.GreenLightStat,intersection_ptr[0].trafficLampSpeedGuide[i].straightlamp.RedLightStat,
                    intersection_ptr[0].trafficLampSpeedGuide[i].rightlamp.YellowLightStat,intersection_ptr[0].trafficLampSpeedGuide[i].rightlamp.GreenLightStat,intersection_ptr[0].trafficLampSpeedGuide[i].rightlamp.RedLightStat,
                    intersection_ptr[0].trafficLampSpeedGuide[i].timers);
    }
    trafficLampSpeedGuide.msg_id = EHMH_V2X_TRAFFICLAMP_SPEED_GUIDE_MSGTYPE;
    result = ehmh_encode(EHMH_V2X_TRAFFICLAMP_SPEED_GUIDE_MSGTYPE, &trafficLampSpeedGuide, send_buf, &send_len);
    if(result)
    {
        osal_printf("ehmh_encode failed,ret=%d\n",result);
        goto ERR_EXIT;
    }

    result = net_send(drv_ptr->ehost_fd, send_buf, send_len);
    if(result < 0)
    {
        osal_printf("net_send failed,ret=%d\n",result);
        goto ERR_EXIT;
    }
    
ERR_EXIT:
    for(i = 0; i < msg_ptr->intersections.pointNum; i++)
    {
        if(intersection_ptr[i].trafficLampSpeedGuide != NULL)
            os_free(intersection_ptr[i].trafficLampSpeedGuide);
    }
    
    if(intersection_ptr != NULL)
        os_free(intersection_ptr);
    
    return result;
}


/*FUNCTION*****************************************************************
* 
* Returned : ERR_OK said success, other values indicate failure.  
* Comments : Process the received message and send it to host.
*
*END*********************************************************************/
int rsm_message_deal(MSG_RoadSideSafetyMessage_st_ptr msg_ptr)
{
    int result = ERR_OK;
    
    if(msg_ptr == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    os_printf("rsm msgcnt:%d\n", msg_ptr->msgCnt);

    memset(&(rsu_info.rsu_id), 0, sizeof(rsu_info.rsu_id));
    memcpy(&(rsu_info.rsu_id), msg_ptr->id, sizeof(rsu_info.rsu_id));

    rsu_info.latitude = msg_ptr->refPos.latitude;
    rsu_info.longitude = msg_ptr->refPos.longitude;

    osal_printf("rsu_info:[latitude=%f,longitude=%f]\n", rsu_info.latitude, rsu_info.longitude);

ERR_EXIT:    
    return result;
}


/*FUNCTION*****************************************************************
* 
* Returned : ERR_OK said success, other values indicate failure.  
* Comments : Process the received message and send it to host.
*
*END*********************************************************************/
int net_message_deal(MSG_MessageFrame_st_ptr msg_ptr)
{
    int result = ERR_OK;
    
    if(msg_ptr == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    switch(msg_ptr->messageid)
    {
        case MSG_MessageFrame_ID_BSM:
        {
            
            break;
        }
        case MSG_MessageFrame_ID_MAPDATA:
        {
            
            break;
        }
        case MSG_MessageFrame_ID_RSM:
        {
            rsm_message_deal(&(msg_ptr->msg.msg_rsm));
            break;
        }
        case MSG_MessageFrame_ID_SPAT:
        {
            spat_message_deal(&(msg_ptr->msg.msg_spat));
            //ehm_send(MSGTYPE_V2X_APPLY, (uint8_t *)&trafficLampSpeedGuide, sizeof(trafficLampSpeedGuide));
            break;
        }
        case MSG_MessageFrame_ID_RSI:
        {
            
        }
        default:
        {
            result = -ERR_INVAL;
            break;
        }
    }

    return result;
ERR_EXIT:
    return result;
}


/*FUNCTION*****************************************************************
* 
* Returned : No return normally.  
* Comments : Wnet receive data main handle thread entry.
*
*END*********************************************************************/
void * wnet_rx_thread_entry(void *param_ptr)
{
    int               result = ERR_OK;
    drv_main_st_ptr  drv_ptr = GET_DRVMAIN_PTR;

    /* Buffer address and message point. */
    uint8_t            * buffer_ptr = NULL;
    MSG_MessageFrame_st_ptr msg_ptr = NULL;
    
    /* Ltev message information and data length. */
    ltev_infor_st ltev_infor;
    uint16_t    ltev_datalen = 0;
    
    /* Wsmp message information and data length. */
    wsmp_infor_st wsmp_infor;
    uint16_t    wsmp_datalen = 0;

    
    /* Allocate message buffer for wnet module. */
    if((buffer_ptr = os_calloc(1, WNET_MSGBUF_SIZE)) == NULL)
    {
        osal_printf("[%s %d]: Allocate message buffer for wnet module error. \n", __FUNCTION__, __LINE__);

        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Allocate message for wnet module. */
    if((msg_ptr = os_calloc(1, sizeof(*msg_ptr))) == NULL)
    {
        osal_printf("[%s %d]: Allocate ads message for wnet module error. \n", __FUNCTION__, __LINE__);

        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
RX_LOOP:

    /* Get messgae from ltev device. */
    result = drv_ltev_receive(drv_ptr->ltev_fd, &ltev_infor, buffer_ptr, WNET_MSGBUF_SIZE); 
    if(result < ERR_OK)
    {
        osal_printf("[%s %d]: Receive data from ltev device error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto RX_LOOP;
    }
    else
    {
        ltev_datalen = result;
    }

    /* Analyse wsmp header and get data. */
    wsmp_datalen = ltev_datalen;
    if((result = wsmp_analyse_msg(&wsmp_infor, buffer_ptr, &wsmp_datalen)) != ERR_OK)
    {
        osal_printf("[%s %d]: Analyse wsmp message header error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto RX_LOOP;
    }

    /* Parse wsmp message based on type. */
    switch(wsmp_infor.dsmp_aid)
    {
        case WAVE_AID_vehicle_safety:
        {
            switch(wsmp_infor.dsmp_element_id)
            {
                case WAVE_ELEMENT_ID_WSM:
                {
                    /* Decode ADS frame into common message structure. */
                    if((result = frame_decode_msg(msg_ptr, buffer_ptr + (ltev_datalen - wsmp_datalen), wsmp_datalen)) != ERR_OK)
                    {
                        osal_printf("[%s %d]: Parse ADS message error. ret = %d. \n", __FUNCTION__, __LINE__, result);
                        goto RX_LOOP;
                    }

                    /* Add message structure into list. 这里暂时先用一个变量代替，后期会换成链表操作。 */
                    memcpy(&MsgSt, msg_ptr, sizeof(MsgSt));
                    net_message_deal(&MsgSt);
                    break;
                }
                default:
                {
                    osal_printf("[%s %d]: Received not supposed WSMP message. dsmp_element_id = %d. \n", __FUNCTION__, __LINE__, wsmp_infor.dsmp_element_id);
                    break;
                }
            }
            break;
        }
        case WAVE_AID_private:
        {
            osal_printf("[%s %d]: Received private WSMP message. dsmp_aid = %d. \n", __FUNCTION__, __LINE__, wsmp_infor.dsmp_aid);
            break;
        }
        default:
        {
            osal_printf("[%s %d]: Received not supposed WSMP message. dsmp_aid = %d. \n", __FUNCTION__, __LINE__, wsmp_infor.dsmp_aid);
            break;
        }
    }

    goto RX_LOOP;

ERR_EXIT:

    /* Free buffer and message memory. */
    if(buffer_ptr != NULL)
    {
        os_free(buffer_ptr);
    }
    if(msg_ptr != NULL)
    {
        os_free(msg_ptr);
    }

    return (void*)result;
}


/*FUNCTION*****************************************************************
* 
* Returned : No return normally.  
* Comments : Wnet receive data main handle thread entry.
*
*END*********************************************************************/
void * ehm_rx_thread_entry(void *param_ptr)
{
    int               result = ERR_OK;
    drv_main_st_ptr  drv_ptr = GET_DRVMAIN_PTR;

    ehm_buff_st_ptr ehmbuf_ptr = NULL;
    uint16_t vhost_datalen = 0;
    ehmh_msgtype_en sub_msgtype;
    uint8_t decode_buf[1024];

     /* Allocate message buffer for wnet module. */
    if((ehmbuf_ptr = os_calloc(1, EHM_BUFF_ST_LEN)) == NULL)
    {
        osal_printf("[%s %d]: Allocate message buffer for wnet module error. \n", __FUNCTION__, __LINE__);

        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    ehmbuf_ptr->data_len = sizeof(ehmbuf_ptr->buffer);
    ehmbuf_ptr->infor.active_dev = VHOST_DEVICE_NET;
RX_LOOP:
    result = net_receive(drv_ptr->ehost_fd, ehmbuf_ptr->buffer, ehmbuf_ptr->data_len);
    if(result < ERR_OK)
    {
        osal_printf("[%s %d]: Receive data from ehm net device error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto RX_LOOP;
    }
    else
    {
        vhost_datalen = result;
    }

    memset(&decode_buf, 0, sizeof(decode_buf));
    /* Analyse ehm message buffer. */
    if((result = ehmh_decode(&sub_msgtype, decode_buf, ehmbuf_ptr->buffer, vhost_datalen)) != ERR_OK)
    {
        osal_printf("[%s %d]: Analyse ehm message buffer error. ret = %d. \n", __FUNCTION__, __LINE__, result);
    }

    if(sub_msgtype == EHMH_V2X_BASIC_VEHICLE_STATUS_MSGTYPE)
    {
        memcpy(&vehicle_basic_status_info, (msg_decode_vehicle_basic_status_st_ptr)&decode_buf, sizeof(msg_decode_vehicle_basic_status_st));
        osal_printf("vehicle_basic_status_info:[angle=%f][velocity=%f][latitude=%f][longitude=%f]\n",vehicle_basic_status_info.angle, vehicle_basic_status_info.velocity, vehicle_basic_status_info.latitude, vehicle_basic_status_info.longitude);
    }
    
    goto RX_LOOP;

ERR_EXIT:

    /* Free buffer and message memory. */
    if(ehmbuf_ptr != NULL)
    {
        os_free(ehmbuf_ptr);
    }

    return NULL;
}


/* Main routine for the project. */
int main(int argc, char *argv[])
{
    int result = ERR_OK;

    //osal_printf("----1-----\n");
    /* Init the driver groups. */
    if((result = drv_main_init(&DrvMainPtr)) != ERR_OK)
    {
        osal_printf("[%s %d]: Init driver main error. \n", __FUNCTION__, __LINE__);
        goto ERR_EXIT;
    }

    //osal_printf("----2-----\n");
    /* Create wnet message receive task. */
    result = osal_task_create(&WnetTask, "WnetRx", wnet_rx_thread_entry, NULL, WNET_RX_THREAD_STACK_SIZE, WNET_RX_THREAD_PRIORITY);
    if(result != ERR_OK)
    {
        osal_printf("[%s %d]:create WnetRx failed. \n", __FUNCTION__, __LINE__);
        goto ERR_EXIT;
    }
    //osal_printf("----3-----\n");
    /* Create ehm message receive task. */
     result = osal_task_create(&EhmTask, "EhmRx", ehm_rx_thread_entry, NULL, EHM_RX_THREAD_STACK_SIZE, EHM_RX_THREAD_PRIORITY);
    if(result != ERR_OK)
    {
        osal_printf("[%s %d]:create EhmRx failed. \n", __FUNCTION__, __LINE__);
        goto ERR_EXIT;
    }

    while (1)
    {
        osal_sleep(10000);
    }

ERR_EXIT:

    return result;
}
