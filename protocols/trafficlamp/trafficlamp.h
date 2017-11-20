/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : trafficlamp.h
 @brief  : traffic lamp protocols api
 @author : wanghechao
 @history:
           2017-11-2      pengrui    Created file
           2017-11-10     pengrui    add the api function of both road and traffic side
           ...
******************************************************************************/
#ifndef __TRAFFICLAMP_H__
#define __TRAFFICLAMP_H__
#ifdef __cplusplus
extern "C" {
#endif

#define RUN_OK                  0x00
#define RUN_ERR                 0x01

#define FRAME_HEADER            0xEE
#define FRAME_ENCODE            0xAA
#define FRAME_DECODE            0xBB
#define FRAME_ERROR             0xBE

#define MAX_TRAFFIC_MSG_SIZE    256

typedef enum
{
    POWEROFF_MODE_OFF,
    POWEROFF_MODE_BLINK
}poweroff_mode_en;

typedef enum
{
    POWERON_MODE_SINGLE_DIR_PASS,
    POWERON_MODE_OPPOSITE_DIR_PASS
}poweron_mode_en;

/*the cmd type*/
typedef enum
{
    GET_TIME_CMD = 0xC2,
    GET_POWER_TIME_CMD,
    GET_POWER_MODE_CMD,
    GET_BLINK_TIME_CMD,
    GET_WORK_TIME_CMD,
    GET_CONTROL_STATUS_CMD,
    SET_TIME_CMD = 0xD2,
    SET_POWER_TIME_CMD,
    SET_POWER_MODE_CMD,
    SET_BLINK_TIME_CMD,
    SET_WORK_TIME_CMD,
    SET_DELAY_WORK_TIME_CMD,
    SET_EW_PASS_CMD,
    SET_SN_PASS_CMD,
    SET_YELLOW_BLINK_CMD,
    SET_NORMAL_MODE_CMD
}cmd_type_en;

/*the current time of device info structure*/
typedef struct _time_info_st
{
    uint8_t hour;
    uint8_t minute;
}time_info_st;

#define TIME_INFO_ST_LEN        sizeof(time_info_st)

/*the power on and off time of device info structure*/
typedef struct _power_time_info_st
{
    uint8_t poweron_hour;
    uint8_t poweron_minute;
    uint8_t poweroff_hour;
    uint8_t poweroff_minute;
}power_time_info_st;

#define POWER_TIME_INFO_ST_LEN        sizeof(power_time_info_st)

/*the info of device about poweroff and run mode structure*/
typedef struct _power_mode_info_st
{
    //0:close  1:yellow lamp link
    uint8_t poweroff_mode:1;
    //0:signal direction run(E.S.W.N) 1:double direction run(EW.SN)
    uint8_t poweron_mode :1;
}power_mode_info_st;

#define POWER_MODE_INFO_ST_LEN    sizeof(power_mode_info_st)

/*the blink time of all lamps (RED,BLUE,YELLOW)*/
typedef struct _blink_time_info_st
{
    uint8_t red;
    uint8_t yellow;
    uint8_t green;
}blink_time_info_st;

#define BLINK_TIME_INFO_ST_LEN    sizeof(blink_time_info_st)

/*the time of blue and yellow lamp work*/
typedef struct _work_time_info_st
{
    uint8_t east_green;
    uint8_t east_yellow;
    uint8_t south_green;
    uint8_t south_yellow;
    uint8_t west_green;
    uint8_t west_yellow;
    uint8_t north_green;
    uint8_t north_yellow;
}work_time_info_st;

#define WORK_TIME_INFO_ST_LEN        sizeof(work_time_info_st)

/*the  status of device and lamp structure*/
typedef struct _control_status_info_st
{
    //0:pause  1:run
    uint8_t device_mode;
    //0x80 red lamp 0x40 yellow lamp 0x20 green lamp 0x00 all lamps
    uint8_t left_time;
    uint8_t east_lamp;
    uint8_t south_lamp;
    uint8_t west_lamp;
    uint8_t north_lamp;
}control_status_info_st;

#define CONTROL_STATUS_INFO_LEN        sizeof(control_status_info_st)

typedef struct _traffic_decode_info_st
{
    cmd_type_en                     cmd_type;
    union {
        time_info_st                currenttime_info;
        power_time_info_st          powertime_info;
        power_mode_info_st          mode_info;
        blink_time_info_st          blinktime_info;
        work_time_info_st           worktime_info;
        control_status_info_st      controlstatus_info;
        uint8_t                     delay_time_info;
    }info;
}traffic_decode_info_st;

typedef struct _traffic_encode_info_st
{
    cmd_type_en                     cmd_type;
    union{
        time_info_st                currenttime_info;
        power_time_info_st          powertime_info;
        power_mode_info_st          mode_info;
        blink_time_info_st          blinktime_info;
        work_time_info_st           worktime_info;
        control_status_info_st      controlstatus_info;
        //the status of cmd run
        uint8_t                     status;
    }info;
}traffic_encode_info_st;


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
int rsuside_encode(traffic_decode_info_st *traffic_encode_info_ptr, uint8_t *buffer_ptr, uint16_t *buffer_len);

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
int rsuside_decode(traffic_encode_info_st *traffic_decode_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len);

/******************************************************************************
*    Function: traffic_encode
*    Descriptions: traffic side encode the info
*    Paramters:
            traffic_encode_info_ptr          - the encode info
            buffer_ptr                       - the encode data pointer
            len_ptr                          - the length of encode data
*    Return:
            = 0                              - success
            < 0                              - failed
*    Comments:
******************************************************************************/
int traffic_encode(traffic_encode_info_st *traffic_encode_info_ptr, uint8_t *buffer_ptr, uint16_t *buffer_len);

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
int traffic_decode(traffic_decode_info_st *traffic_decode_info_ptr, uint8_t *buffer_ptr, uint16_t buffer_len);

#ifdef __cplusplus
}
#endif

#endif
