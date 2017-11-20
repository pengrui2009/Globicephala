/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : trafficlamp.c
 @brief  : the protocol of control roadlamp
 @author : wanghechao
 @history:
           2017-11-2      pengrui    Created file
           2017-11-10     pengrui    support both side of road and traffic control encode and decode
           ...
******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "trafficlamp.h"
#include "error.h"

/******************************************************************************
*    Function: bcd_to_bin
*    Descriptions: bcd value to  decimal value
*    Paramters:
            bcd                            - the value of bcd
*    Return:
            > 0                            - decimal value
*    Comments: 
******************************************************************************/
static uint8_t bcd_to_bin(uint8_t bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0xf);
}

/******************************************************************************
*    Function: bin_to_bcd
*    Descriptions: decimal value to bcd value
*    Paramters:
            decimal                        - the value of decimal
*    Return:
            > 0                            - bcd value
*    Comments:
******************************************************************************/
static uint8_t bin_to_bcd(uint8_t decimal)
{
    uint8_t value = (uint8_t)((decimal / 10) << 4) + (decimal % 10);

    return value;
}

/******************************************************************************
*    Function: protocol_checksum
*    Descriptions: calculate the checksum of frame
*    Paramters:
            buffer_ptr                    - the data pointer
            buffer_len                    - the length of data
*    Return:
            > 0                           - checksum value
*    Comments: 
******************************************************************************/
static uint8_t protocol_checksum(uint8_t *buffer_ptr, uint16_t buffer_len)
{
   uint16_t i;
   uint8_t csnum = 0;

   //
   for (i=0; i<buffer_len; i++)
   {
       csnum += buffer_ptr[i];
   }
   
   return csnum;
}

/******************************************************************************
*    Function: checksum_valid
*    Descriptions: valid the checksum value
*    Paramters:
            checksum1                    - the checksum value
            checksum2                    - the checksum value
*    Return:
            = 1                          - checksum valid
            = 0                          - checksum invalid
*    Comments:
******************************************************************************/
static int checksum_valid(uint8_t checksum1, uint8_t checksum2)
{
    return (checksum1 == checksum2) ? 1 : 0;
}
/******************************************************************************
*    Function: rsuside_encode_currenttime
*    Descriptions: Set the time of the signal device
*    Paramters:
            info_ptr            - the pointer of the time info
            buffer_ptr            - the data point (Return)
            len_ptr             - the length of data(Return)
*    Return:
            = 0                 - success
            < 0                 - failed
*    Comments: 
******************************************************************************/
static int rsuside_encode_currenttime(time_info_st *info_ptr, uint8_t *buffer_ptr, uint16_t *len_ptr)
{
    int result = 0;
    
    if((NULL == info_ptr) || (NULL == buffer_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_ENCODE;
    buffer_ptr[2] = SET_TIME_CMD;
    buffer_ptr[3] = bin_to_bcd(info_ptr->hour);
    buffer_ptr[4] = bin_to_bcd(info_ptr->minute);
    buffer_ptr[5] = protocol_checksum(buffer_ptr, 5);
    
    *len_ptr = 6;

ERR_EXIT:

    return result;

}

/******************************************************************************
*    Function: rsuside_encode_power_time
*    Descriptions: set the  time of signal device which power on and off
*    Paramters:
            info_ptr            - the pointer of the power time info
            buffer_ptr          - the data point (Return)
            len_ptr             - the length of data(Return)
*    Return:
            = 0                 - success
            < 0                 - failed
*    Comments: 
******************************************************************************/
static int rsuside_encode_power_time(power_time_info_st *info_ptr, uint8_t *buffer_ptr, uint16_t *len_ptr)
{
    int result = 0;
        
    if((NULL == info_ptr) || (NULL == buffer_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_ENCODE;
    buffer_ptr[2] = SET_POWER_TIME_CMD;
    buffer_ptr[3] = bin_to_bcd(info_ptr->poweron_hour);
    buffer_ptr[4] = bin_to_bcd(info_ptr->poweron_minute);
    buffer_ptr[5] = bin_to_bcd(info_ptr->poweroff_hour);
    buffer_ptr[6] = bin_to_bcd(info_ptr->poweroff_minute);
    buffer_ptr[7] = protocol_checksum(buffer_ptr, 7);
    *len_ptr = 8;

ERR_EXIT:

    return result;

}

/******************************************************************************
*    Function: rsuside_encode_mode
*    Descriptions: set the  mode of signal device which power off and run
*    Paramters:
            info_ptr               - the pointer of the power mode info
            buffer_ptr             - the data point (Return)
            len_ptr                - the length of data(Return)
*    Return:
            = 0                    -    success
            < 0                    -    failed
*    Comments: 
******************************************************************************/
static int rsuside_encode_mode(power_mode_info_st *info_ptr, uint8_t *buffer_ptr, uint16_t *len_ptr)
{
    int result = 0;
        
    if((NULL == info_ptr) || (NULL == buffer_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_ENCODE;
    buffer_ptr[2] = SET_POWER_MODE_CMD;
    buffer_ptr[3] = info_ptr->poweroff_mode;
    buffer_ptr[4] = info_ptr->poweron_mode;
    buffer_ptr[5] = protocol_checksum(buffer_ptr, 5);
    *len_ptr = 6;

ERR_EXIT:

    return result;

}

/******************************************************************************
*    Function: rsuside_encode_blink_time
*    Descriptions: set the  blink time of lamp (red, blue , yellow)
*    Paramters:
            info_ptr                - the pointer of the blink info
            buffer_ptr              - the data point (Return)
            len_ptr                 - the length of data(Return)
*    Return:
            = 0                     -    success
            < 0                     -    failed
*    Comments: 
******************************************************************************/
static int rsuside_encode_blink_time(blink_time_info_st *info_ptr, uint8_t *buffer_ptr, uint16_t *len_ptr)
{
    int result = 0;
        
    if((NULL == info_ptr) || (NULL == buffer_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_ENCODE;
    buffer_ptr[2] = SET_BLINK_TIME_CMD;
    buffer_ptr[3] = info_ptr->red;
    buffer_ptr[4] = info_ptr->yellow;
    buffer_ptr[5] = info_ptr->green;
    buffer_ptr[6] = protocol_checksum(buffer_ptr, 6);
    
    *len_ptr = 7;

ERR_EXIT:

    return result;

}

/******************************************************************************
*    Function: rsuside_encode_work_time
*    Descriptions: set the  run time of blue and yellow lamp from all direction
*    Paramters:
            info_ptr                - the pointer of the work time info
            buffer_ptr              - the data point (Return)
            len_ptr                 - the length of data(Return)
*    Return:
            = 0                     -    success
            < 0                     -    failed
*    Comments: 
******************************************************************************/
static int rsuside_encode_work_time(work_time_info_st *info_ptr, uint8_t *buffer_ptr, uint16_t *len_ptr)
{
    int result = 0;
        
    if((NULL == info_ptr) || (NULL == buffer_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_ENCODE;
    buffer_ptr[2] = SET_WORK_TIME_CMD;
    buffer_ptr[3] = info_ptr->east_green;
    buffer_ptr[4] = info_ptr->east_yellow;
    buffer_ptr[5] = info_ptr->south_green;
    buffer_ptr[6] = info_ptr->south_yellow;
    buffer_ptr[7] = info_ptr->west_green;
    buffer_ptr[8] = info_ptr->west_yellow;
    buffer_ptr[9] = info_ptr->north_green;
    buffer_ptr[10] = info_ptr->north_yellow;
    buffer_ptr[11] = protocol_checksum(buffer_ptr, 11);

    *len_ptr = 12;

ERR_EXIT:

    return result;

}

/******************************************************************************
*    Function: rsuside_encode_delay_work_time
*    Descriptions: delay the lamp time
*    Paramters:
            delay_time              - the time of the lamp delay 
            buffer_ptr              - the data point (Return)
            len_ptr                 - the length of data(Return)
*    Return:
            = 0                     -    success
            < 0                     -    failed
*    Comments: 
******************************************************************************/
static int rsuside_encode_delay_work_time(uint8_t *delay_time, uint8_t *buffer_ptr, uint16_t *len_ptr)
{
    int result = 0;
        
    if((NULL == buffer_ptr) || (NULL == len_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_ENCODE;
    buffer_ptr[2] = SET_DELAY_WORK_TIME_CMD;
    buffer_ptr[3] = *delay_time;
    buffer_ptr[4] = protocol_checksum(buffer_ptr, 4);
        
    *len_ptr = 5;

ERR_EXIT:

    return result;
}


/******************************************************************************
*    Function: traffic_encode_current_time
*    Descriptions: traffic side encode the info of current time
*    Paramters:
            time_info                      - the time info point
            buffer_ptr                     - the encode data pointer
            len_ptr                        - the length of encode data
*    Return:
            = 0                            - success
            < 0                            - failed
*    Comments:
******************************************************************************/
static int traffic_encode_current_time(time_info_st time_info, uint8_t *buffer_ptr, uint16_t *buffer_len)
{
    int result = 0;
    if((NULL == buffer_ptr) || (NULL == buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_DECODE;
    buffer_ptr[2] = GET_TIME_CMD;
    buffer_ptr[3] = bin_to_bcd(time_info.hour);
    buffer_ptr[4] = bin_to_bcd(time_info.minute);
    buffer_ptr[5] = protocol_checksum(buffer_ptr, 5);

    *buffer_len = 6;
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: traffic_encode_power_time
*    Descriptions: traffic side encode the info of power time
*    Paramters:
            powertime_info                  - the time info power on and off
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int traffic_encode_power_time(power_time_info_st powertime_info, uint8_t *buffer_ptr, uint16_t *buffer_len)
{
    int result = 0;
    if((NULL == buffer_ptr) || (NULL == buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_DECODE;
    buffer_ptr[2] = GET_POWER_TIME_CMD;
    buffer_ptr[3] = bin_to_bcd(powertime_info.poweron_hour);
    buffer_ptr[4] = bin_to_bcd(powertime_info.poweron_minute);
    buffer_ptr[5] = bin_to_bcd(powertime_info.poweroff_hour);
    buffer_ptr[6] = bin_to_bcd(powertime_info.poweroff_minute);
    buffer_ptr[7] = protocol_checksum(buffer_ptr, 7);

    *buffer_len = 8;
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: traffic_encode_mode
*    Descriptions: traffic side encode the info of mode
*    Paramters:
            mode_info                       - the info of the mode
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int traffic_encode_mode(power_mode_info_st mode_info, uint8_t *buffer_ptr, uint16_t *buffer_len)
{
    int result = 0;
    if((NULL == buffer_ptr) || (NULL == buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_DECODE;
    buffer_ptr[2] = GET_POWER_MODE_CMD;
    buffer_ptr[3] = mode_info.poweroff_mode;
    buffer_ptr[4] = mode_info.poweron_mode;
    buffer_ptr[5] = protocol_checksum(buffer_ptr, 5);

    *buffer_len = 6;
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: traffic_encode_blinktime_info
*    Descriptions: traffic side encode the info of blink time info
*    Paramters:
            blink_info                      - the info of the blink time
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int traffic_encode_blinktime_info(blink_time_info_st blinktime_info, uint8_t *buffer_ptr, uint16_t *buffer_len)
{
    int result = 0;
    if((NULL == buffer_ptr) || (NULL == buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_DECODE;
    buffer_ptr[2] = GET_BLINK_TIME_CMD;
    buffer_ptr[3] = blinktime_info.red;
    buffer_ptr[4] = blinktime_info.yellow;
    buffer_ptr[5] = blinktime_info.green;
    buffer_ptr[6] = protocol_checksum(buffer_ptr, 6);

    *buffer_len = 7;
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: traffic_encode_worktime_info
*    Descriptions: traffic side encode the info of work time
*    Paramters:
            mode_info                       - the info of the work time
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int traffic_encode_worktime_info(work_time_info_st worktime_info, uint8_t *buffer_ptr, uint16_t *buffer_len)
{
    int result = 0;
    if((NULL == buffer_ptr) || (NULL == buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_DECODE;
    buffer_ptr[2] = GET_WORK_TIME_CMD;
    buffer_ptr[3] = worktime_info.east_green;
    buffer_ptr[4] = worktime_info.east_yellow;
    buffer_ptr[5] = worktime_info.south_green;
    buffer_ptr[6] = worktime_info.south_yellow;
    buffer_ptr[7] = worktime_info.west_green;
    buffer_ptr[8] = worktime_info.west_yellow;
    buffer_ptr[9] = worktime_info.north_green;
    buffer_ptr[10] = worktime_info.north_yellow;
    buffer_ptr[11] = protocol_checksum(buffer_ptr, 11);

    *buffer_len = 12;
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: control_status_info_st
*    Descriptions: traffic side encode the info of control status info
*    Paramters:
            controlstatus_info              - the info of the control status info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int traffic_encode_controlstatus_info(control_status_info_st controlstatus_info, uint8_t *buffer_ptr, uint16_t *buffer_len)
{
    int result = 0;
    if((NULL == buffer_ptr) || (NULL == buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    buffer_ptr[0] = FRAME_HEADER;
    buffer_ptr[1] = FRAME_DECODE;
    buffer_ptr[2] = GET_CONTROL_STATUS_CMD;
    buffer_ptr[3] = controlstatus_info.device_mode;
    buffer_ptr[4] = controlstatus_info.left_time;
    buffer_ptr[5] = controlstatus_info.east_lamp;
    buffer_ptr[6] = controlstatus_info.south_lamp;
    buffer_ptr[7] = controlstatus_info.west_lamp;
    buffer_ptr[8] = controlstatus_info.north_lamp;
    buffer_ptr[9] = protocol_checksum(buffer_ptr, 9);

    *buffer_len = 10;
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: traffic_encode_replay_info
*    Descriptions: traffic side encode the info of replay
*    Paramters:
            cmd_type                        - the info of the cmd type
            status                          - the cmd run status
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int traffic_encode_replay_info(cmd_type_en cmd_type, uint8_t status, uint8_t *buffer_ptr, uint16_t *buffer_len)
{
    int result = 0;
    if((NULL == buffer_ptr) || (NULL == buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    buffer_ptr[0] = FRAME_HEADER;
    if(status)
    {
        // OK
        buffer_ptr[1] = FRAME_DECODE;
    }else{
        buffer_ptr[1] = FRAME_ERROR;
    }
    buffer_ptr[2] = cmd_type;
    buffer_ptr[3] = protocol_checksum(buffer_ptr, 3);

    *buffer_len = 4;
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: decode_current_time
*    Descriptions: traffic side decode the info of current time
*    Paramters:
            time_info_ptr                   - the point of the time info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int decode_current_time(time_info_st *time_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len)
{
    int result = 0;
    if((NULL == time_info_ptr) || (NULL == buffer_ptr) || (TIME_INFO_ST_LEN > buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    time_info_ptr->hour = bcd_to_bin(buffer_ptr[0]);
    time_info_ptr->minute = bcd_to_bin(buffer_ptr[1]);

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: decode_power_time
*    Descriptions: traffic side decode the info of power time
*    Paramters:
            time_info_ptr                   - the point of the time info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int decode_power_time(power_time_info_st *powertime_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len)
{
    int result = 0;
    if((NULL == powertime_info_ptr) || (NULL == buffer_ptr) || (POWER_TIME_INFO_ST_LEN > buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    powertime_info_ptr->poweron_hour   = bcd_to_bin(buffer_ptr[0]);
    powertime_info_ptr->poweron_minute = bcd_to_bin(buffer_ptr[1]);
    powertime_info_ptr->poweroff_hour   = bcd_to_bin(buffer_ptr[2]);
    powertime_info_ptr->poweroff_minute = bcd_to_bin(buffer_ptr[3]);

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: decode_power_mode
*    Descriptions: traffic side decode the info of power mode
*    Paramters:
            powermode_info_ptr              - the point of the power mode info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int decode_power_mode(power_mode_info_st *powermode_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len)
{
    int result = 0;
    if((NULL == powermode_info_ptr) || (NULL == buffer_ptr) || (POWER_MODE_INFO_ST_LEN > buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    powermode_info_ptr->poweroff_mode = buffer_ptr[0];
    powermode_info_ptr->poweron_mode  = buffer_ptr[1];

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: decode_blinkinfo
*    Descriptions: traffic side decode the info of blink info
*    Paramters:
            blink_info_ptr                  - the point of the blink info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int decode_blinkinfo(blink_time_info_st *blink_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len)
{
    int result = 0;
    if((NULL == blink_info_ptr) || (NULL == buffer_ptr) || (BLINK_TIME_INFO_ST_LEN > buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    blink_info_ptr->red    = buffer_ptr[0];
    blink_info_ptr->yellow = buffer_ptr[1];
    blink_info_ptr->green  = buffer_ptr[2];

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: decode_worktime
*    Descriptions: traffic side decode the info of traffic lights work time
*    Paramters:
            worktime_info_ptr               - the point of the work time info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int decode_worktime(work_time_info_st *worktime_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len)
{
    int result = 0;
    if((NULL == worktime_info_ptr) || (NULL == buffer_ptr) || (WORK_TIME_INFO_ST_LEN > buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    worktime_info_ptr->east_green   = buffer_ptr[0];
    worktime_info_ptr->east_yellow  = buffer_ptr[1];
    worktime_info_ptr->south_green  = buffer_ptr[2];
    worktime_info_ptr->south_yellow = buffer_ptr[3];
    worktime_info_ptr->west_green   = buffer_ptr[4];
    worktime_info_ptr->west_yellow  = buffer_ptr[5];
    worktime_info_ptr->north_green  = buffer_ptr[6];
    worktime_info_ptr->north_yellow = buffer_ptr[7];

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: decode_delaytime
*    Descriptions: traffic side decode the info of traffic lights delay time
*    Paramters:
            delay_time_ptr                  - the point of the delay time info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int decode_delaytime(uint8_t *delay_time_ptr, uint8_t *buffer_ptr, uint16_t buffer_len)
{
    int result = 0;
    if((NULL == delay_time_ptr) || (NULL == buffer_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    *delay_time_ptr   = buffer_ptr[0];

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: decode_delaytime
*    Descriptions: traffic side decode the info of traffic lights delay time
*    Paramters:
            delay_time_ptr                  - the point of the delay time info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
static int decode_control_status(control_status_info_st *control_status_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len)
{
    int result = 0;
    if((NULL == control_status_info_ptr) || (NULL == buffer_ptr))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    if(CONTROL_STATUS_INFO_LEN > buffer_len)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    control_status_info_ptr->device_mode = buffer_ptr[0];
    control_status_info_ptr->left_time   = buffer_ptr[1];
    control_status_info_ptr->east_lamp   = buffer_ptr[2];
    control_status_info_ptr->south_lamp  = buffer_ptr[3];
    control_status_info_ptr->west_lamp   = buffer_ptr[4];
    control_status_info_ptr->north_lamp  = buffer_ptr[5];

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: rsuside_encode
*    Descriptions: rsu side encode the info
*    Paramters:
            traffic_encode_info_ptr         - the encode info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
int rsuside_encode(traffic_decode_info_st *traffic_encode_info_ptr, uint8_t *buffer_ptr, uint16_t *buffer_len)
{
    int result = 0;
    cmd_type_en cmd_type = traffic_encode_info_ptr->cmd_type;

    if((NULL == traffic_encode_info_ptr) || (NULL == buffer_ptr) || (NULL == buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    switch(cmd_type)
    {
    case GET_TIME_CMD:
    case GET_POWER_TIME_CMD:
    case GET_POWER_MODE_CMD:
    case GET_BLINK_TIME_CMD:
    case GET_WORK_TIME_CMD:
    case GET_CONTROL_STATUS_CMD:
    case SET_EW_PASS_CMD:
    case SET_SN_PASS_CMD:
    case SET_YELLOW_BLINK_CMD:
    case SET_NORMAL_MODE_CMD:
        buffer_ptr[0] = FRAME_HEADER;
        buffer_ptr[1] = FRAME_ENCODE;
        buffer_ptr[2] = cmd_type;
        *buffer_len = 3;
        break;
    case SET_TIME_CMD:
        result = rsuside_encode_currenttime(&traffic_encode_info_ptr->info.currenttime_info, buffer_ptr, buffer_len);
        break;
    case SET_POWER_TIME_CMD:
        result = rsuside_encode_power_time(&traffic_encode_info_ptr->info.powertime_info, buffer_ptr, buffer_len);
        break;
    case SET_POWER_MODE_CMD:
        result = rsuside_encode_mode(&traffic_encode_info_ptr->info.mode_info, buffer_ptr, buffer_len);
        break;
    case SET_BLINK_TIME_CMD:
        result = rsuside_encode_blink_time(&traffic_encode_info_ptr->info.blinktime_info, buffer_ptr, buffer_len);
        break;
    case SET_WORK_TIME_CMD:
        result = rsuside_encode_work_time(&traffic_encode_info_ptr->info.worktime_info, buffer_ptr, buffer_len);
        break;
    case SET_DELAY_WORK_TIME_CMD:
        result = rsuside_encode_delay_work_time(&traffic_encode_info_ptr->info.delay_time_info, buffer_ptr, buffer_len);
        break;
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
*    Function: rsuside_decode
*    Descriptions: rsu side decode the info
*    Paramters:
            decode_info_st                  - the decode info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            = 1                             - cmd run error
            < 0                             - failed
*    Comments:
******************************************************************************/
int rsuside_decode(traffic_encode_info_st *traffic_decode_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len)
{
    int result = 0;
    uint8_t checksum = 0;

    if((NULL == traffic_decode_info_ptr) || (NULL == buffer_ptr) || (3 > buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    //cal checksum
    checksum = protocol_checksum(buffer_ptr, (buffer_len - 1));
    if(checksum != buffer_ptr[buffer_len-1])
    {
        result = -ERR_OTHER;
        goto ERR_EXIT;
    }

    if((FRAME_HEADER == buffer_ptr[0]) || (FRAME_DECODE == buffer_ptr[1]))
    {
        cmd_type_en  cmd_type = 0;

        cmd_type = *(buffer_ptr + 2);

        buffer_ptr += 3;
        buffer_len -= 3;

        switch(cmd_type)
        {
        case GET_TIME_CMD:
            result = decode_current_time(&traffic_decode_info_ptr->info.currenttime_info, buffer_ptr, buffer_len);
            break;
        case GET_POWER_TIME_CMD:
            result = decode_power_time(&traffic_decode_info_ptr->info.powertime_info, buffer_ptr, buffer_len);
            break;
        case GET_POWER_MODE_CMD:
            result = decode_power_mode(&traffic_decode_info_ptr->info.mode_info, buffer_ptr, buffer_len);
            break;
        case GET_BLINK_TIME_CMD:
            result = decode_blinkinfo(&traffic_decode_info_ptr->info.blinktime_info, buffer_ptr, buffer_len);
            break;
        case GET_WORK_TIME_CMD:
            result = decode_worktime(&traffic_decode_info_ptr->info.worktime_info, buffer_ptr, buffer_len);
            break;
        case GET_CONTROL_STATUS_CMD:
            result = decode_control_status(&traffic_decode_info_ptr->info.controlstatus_info, buffer_ptr, buffer_len);
            break;
        case SET_TIME_CMD:
        case SET_POWER_TIME_CMD:
        case SET_POWER_MODE_CMD:
        case SET_BLINK_TIME_CMD:
        case SET_WORK_TIME_CMD:
        case SET_DELAY_WORK_TIME_CMD:
        case SET_EW_PASS_CMD:
        case SET_SN_PASS_CMD:
        case SET_YELLOW_BLINK_CMD:
        case SET_NORMAL_MODE_CMD:
            traffic_decode_info_ptr->cmd_type = cmd_type;
            traffic_decode_info_ptr->info.status = RUN_OK;
            break;
        default:
            {
                result = -ERR_OTHER;
                break;
            }
        }
    }else if((FRAME_HEADER == buffer_ptr[0]) || (FRAME_ERROR == buffer_ptr[1])){
        traffic_decode_info_ptr->cmd_type = buffer_ptr[2];
        traffic_decode_info_ptr->info.status = RUN_ERR;
    }else{
        result = -ERR_INVAL;
    }

ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: traffic_encode
*    Descriptions: traffic side encode the info
*    Paramters:
            traffic_encode_info_ptr         - the encode info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments:
******************************************************************************/
int traffic_encode(traffic_encode_info_st *traffic_encode_info_ptr, uint8_t *buffer_ptr, uint16_t *buffer_len)
{
    int result = 0;
    cmd_type_en cmd_type = traffic_encode_info_ptr->cmd_type;

    if((NULL == traffic_encode_info_ptr) || (NULL == buffer_ptr) || (NULL == buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    switch(cmd_type)
    {
    case GET_TIME_CMD:
        result = traffic_encode_current_time(traffic_encode_info_ptr->info.currenttime_info, buffer_ptr, buffer_len);
        break;
    case GET_POWER_TIME_CMD:
        result = traffic_encode_power_time(traffic_encode_info_ptr->info.powertime_info, buffer_ptr, buffer_len);
        break;
    case GET_POWER_MODE_CMD:
        result = traffic_encode_mode(traffic_encode_info_ptr->info.mode_info, buffer_ptr, buffer_len);
        break;
    case GET_BLINK_TIME_CMD:
        result = traffic_encode_blinktime_info(traffic_encode_info_ptr->info.blinktime_info, buffer_ptr, buffer_len);
        break;
    case GET_WORK_TIME_CMD:
        result = traffic_encode_worktime_info(traffic_encode_info_ptr->info.worktime_info, buffer_ptr, buffer_len);
        break;
    case GET_CONTROL_STATUS_CMD:
        result = traffic_encode_controlstatus_info(traffic_encode_info_ptr->info.controlstatus_info, buffer_ptr, buffer_len);
        break;
    case SET_TIME_CMD:
    case SET_POWER_TIME_CMD:
    case SET_POWER_MODE_CMD:
    case SET_BLINK_TIME_CMD:
    case SET_WORK_TIME_CMD:
    case SET_DELAY_WORK_TIME_CMD:
    case SET_EW_PASS_CMD:
    case SET_SN_PASS_CMD:
    case SET_YELLOW_BLINK_CMD:
    case SET_NORMAL_MODE_CMD:
        result = traffic_encode_replay_info(cmd_type, traffic_encode_info_ptr->info.status, buffer_ptr, buffer_len);
        break;
    default:
        {
            result = -ERR_OTHER;
            break;
        }
    }
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: traffic_decode
*    Descriptions: traffic side decode the info
*    Paramters:
            decode_info_st                  - the decode info
            buffer_ptr                      - the encode data pointer
            len_ptr                         - the length of encode data
*    Return:
            = 0                             - success
            = 1                             - cmd run error
            < 0                             - failed
*    Comments:
******************************************************************************/
int traffic_decode(traffic_decode_info_st *traffic_decode_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len)
{
    int result = 0;

    if((NULL == traffic_decode_info_ptr) || (NULL == buffer_ptr) || (3 > buffer_len))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    if((FRAME_HEADER == buffer_ptr[0]) || (FRAME_ENCODE == buffer_ptr[1]))
    {
        cmd_type_en  cmd_type = 0;

        cmd_type = *(buffer_ptr + 2);

        buffer_ptr += 3;
        buffer_len -= 3;

        switch(cmd_type)
        {
        case GET_TIME_CMD:
        case GET_POWER_TIME_CMD:
        case GET_POWER_MODE_CMD:
        case GET_BLINK_TIME_CMD:
        case GET_WORK_TIME_CMD:
        case GET_CONTROL_STATUS_CMD:
        case SET_EW_PASS_CMD:
        case SET_SN_PASS_CMD:
        case SET_YELLOW_BLINK_CMD:
        case SET_NORMAL_MODE_CMD:
            traffic_decode_info_ptr->cmd_type = cmd_type;
            break;
        case SET_TIME_CMD:
        {
            if(checksum_valid(protocol_checksum((buffer_ptr - 3), (buffer_len + 2)), buffer_ptr[buffer_len - 1]))
            {
                traffic_decode_info_ptr->cmd_type = SET_TIME_CMD;
                result = decode_current_time(&traffic_decode_info_ptr->info.currenttime_info, buffer_ptr, buffer_len);
            }
            break;
        }
        case SET_POWER_TIME_CMD:
        {
            if(checksum_valid(protocol_checksum((buffer_ptr -3), (buffer_len + 2)), buffer_ptr[buffer_len - 1]))
            {
                traffic_decode_info_ptr->cmd_type = SET_POWER_TIME_CMD;
                result = decode_power_time(&traffic_decode_info_ptr->info.powertime_info, buffer_ptr, buffer_len);
            }
            break;
        }
        case SET_POWER_MODE_CMD:
        {
            if(checksum_valid(protocol_checksum((buffer_ptr-3), (buffer_len+2)), buffer_ptr[buffer_len - 1]))
            {
                traffic_decode_info_ptr->cmd_type = SET_POWER_MODE_CMD;
                result = decode_power_mode(&traffic_decode_info_ptr->info.mode_info, buffer_ptr, buffer_len);
            }
            break;
        }
        case SET_BLINK_TIME_CMD:
        {
            if(checksum_valid(protocol_checksum((buffer_ptr-3), (buffer_len+2)), buffer_ptr[buffer_len - 1]))
            {
                traffic_decode_info_ptr->cmd_type = SET_BLINK_TIME_CMD;
                result = decode_blinkinfo(&traffic_decode_info_ptr->info.blinktime_info, buffer_ptr, buffer_len);
            }
            break;
        }
        case SET_WORK_TIME_CMD:
        {
            if(checksum_valid(protocol_checksum((buffer_ptr-3), (buffer_len+2)), buffer_ptr[buffer_len - 1]))
            {
                traffic_decode_info_ptr->cmd_type = SET_WORK_TIME_CMD;
                result = decode_worktime(&traffic_decode_info_ptr->info.worktime_info, buffer_ptr, buffer_len);
            }
            break;
        }
        case SET_DELAY_WORK_TIME_CMD:
        {
            if(checksum_valid(protocol_checksum((buffer_ptr-3), (buffer_len+3)), buffer_ptr[buffer_len - 1]))
            {
                traffic_decode_info_ptr->cmd_type = SET_DELAY_WORK_TIME_CMD;
                result = decode_delaytime(&traffic_decode_info_ptr->info.delay_time_info, buffer_ptr, buffer_len);
            }
            break;
        }
        default:
        {
            result = -ERR_OTHER;
            break;
        }
        }
    }

ERR_EXIT:

    return result;
}
