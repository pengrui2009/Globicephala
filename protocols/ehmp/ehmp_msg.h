/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : ehmp_msg.h
 @brief  : ehmp protocol parse api
 @author : pengrui
 @history:
           2017-11-30      wangxianwen    Created file
           ...
******************************************************************************/
#ifndef __EHMP_MSG_H__
#define __EHMP_MSG_H__

#include "stdint.h"
#include "prot_element.h"
#include "ehmp_frame.h"


/* External structure: Structure used by outer users. *********************/

/* Ehmp v2x_apply message msgid. */
typedef enum _ehmp_msgid_en
{
    EHMP_MSGID_RESERVED = 0,             /* Reserved id. */

    /* Message id for v2x application. */
    EHMP_MSGID_V2XAPP_HV_BASIC_INFOR,    /* Basic information for host vehicle. */
    EHMP_MSGID_V2XAPP_HV_FULL_INFOR,     /* Full information for host vehicle. */
    EHMP_MSGID_V2XAPP_HV_STATIC_INFOR,   /* Static information for host vehicle. */
    
    EHMP_MSGID_V2XAPP_HV_ALERT_INFOR,    /* Alert information for host vehicle. */
    EHMP_MSGID_V2XAPP_HV_SYSTEM_TIME,    /* System time for host vehicle. */
    EHMP_MSGID_V2XAPP_HV_TLAMP_SPDGUIDE, /* Traffic lamp and speed guide for host vehicle. */

    EHMP_MSGID_V2XAPP_RS_ALERT_INFOR,    /* Alert information from roadside unit. */

    EHMP_MSGID_V2XAPP_RV_BASIC_INFOR ,   /* Basic information for all remote vehicles. */ 
    EHMP_MSGID_V2XAPP_RV_FULL_INFOR,     /* Full information for all remote vehicles. */
    EHMP_MSGID_V2XAPP_RV_ALERT_INFOR,    /* Alert information for alert remote vehicles. */

    /* Message id for gps application. */
    EHMP_MSGID_GPSAPP_RESERVED,          /* Reserved id. */
    EHMP_MSGID_GPSAPP_RTCMCORRECTION,    /* RTCM correction information. */
    EHMP_MSGID_GPSAPP_NMEA_RAWMSG,       /* NMEA raw message. */

}ehmp_msgid_en, *ehmp_msgid_en_ptr;

#define EHMP_MSGID_EN_LEN    (sizeof(ehmp_msgid_en))


/* Ehmp_HvAlertInfor. */
typedef struct _Ehmp_HvAlertInfor_st
{
    uint8_t                               alert_id; /* Alert id. */
    
    /* Add later. */
    
}Ehmp_HvAlertInfor_st, * Ehmp_HvAlertInfor_st;

#define Ehmp_HvAlertInfor_st_len    (sizeof(Ehmp_HvAlertInfor_st))


/* Ehmp_HvBasicInfor. */
typedef struct _Ehmp_HvBasicInfor_st
{
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    DE_TemporaryID_st                         id; /* Product id. */
    
    df_Position3D_st                         pos; /* position */
    DE_Speed_t                             speed; /* Driving speed. Unit km/h. */
    DE_Heading_t                         heading; /* Driving direction. Unit degree. */
    
}Ehmp_HvBasicInfor_st, * Ehmp_HvBasicInfor_st;

#define Ehmp_HvBasicInfor_st_len    (sizeof(Ehmp_HvBasicInfor_st))


/* Ehmp_HvFullInfor. */
typedef struct _Ehmp_HvFullInfor_st
{
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    DE_TemporaryID_st                         id; /* Product id. */
    
    df_Position3D_st                         pos; /* position */
    DE_TransmissionState_en                trans; /* Transmission state. */
    DE_Speed_t                             speed; /* Driving speed. Unit km/h. */
    DE_Heading_t                         heading; /* Driving direction. Unit degree. */
    DE_SteeringWheelAngle_t       steering_angle; /* Steering wheel angle. */
    DF_AccelerationSet4Way_st    accelerationSet; /* Acceleration set in 4 way. */
    df_BrakeSystemStatus_st         brake_status; /* Brake system status. */
    DE_ExteriorLights_un              ext_lights; /* Exterior lights. */
    
}Ehmp_HvFullInfor_st, * Ehmp_HvFullInfor_st;

#define Ehmp_HvFullInfor_st_len    (sizeof(Ehmp_HvFullInfor_st))


/* Ehmp_HvStaticInfor. */
typedef struct _Ehmp_HvStaticInfor_st
{
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    df_VehicleSize_st                   vec_size; /* Vehicle size. */
    
}Ehmp_HvStaticInfor_st, * Ehmp_HvStaticInfor_st;

#define Ehmp_HvStaticInfor_st_len    (sizeof(Ehmp_HvStaticInfor_st))


/* Ehmp_HvSystemTime. */
typedef struct _Ehmp_HvSystemTime_st
{
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    
}Ehmp_HvSystemTime_st, * Ehmp_HvSystemTime_st;

#define Ehmp_HvSystemTime_st_len    (sizeof(Ehmp_HvSystemTime_st))


/* Ehmp_HvTrafficLampSpeedGuide. */
typedef struct _Ehmp_HvTLampSpdGuide_st
{  
    DE_TrafficLamps_un                  turnLeft; /* Lamp in turn left direction. */
    DE_TrafficLamps_un                  straight; /* Lamp in staight direction. */
    DE_TrafficLamps_un                 turnright; /* Lamp in turn right direction. */

    float                           cntdown_time; /* Countdown time in active direction. unit: s. */                      
    DE_Speed_t                    speedGuide_max; /* Speed guide max. unit: km/h. */
    DE_Speed_t                    speedGuide_min; /* Speed guide min. unit: km/h. */
    
}Ehmp_HvTLampSpdGuide_st, * Ehmp_HvTLampSpdGuide_st;

#define Ehmp_HvTLampSpdGuide_st_len    (sizeof(Ehmp_HvTLampSpdGuide_st))


/* Ehmp_RvAlertInfor. */
typedef struct _Ehmp_RvAlertInfor_st
{
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    uint8_t                              vec_num; /* Vehicle number. */
    
    df_VehicleBasicInfor_st            infor[40]; /* Vehicle basic information. */
    
}Ehmp_RvAlertInfor_st, * Ehmp_RvAlertInfor_st;

#define Ehmp_RvAlertInfor_st_len    (sizeof(Ehmp_RvAlertInfor_st))


/* Ehmp_RvBasicInfor. */
typedef struct _Ehmp_RvBasicInfor_st
{
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    uint8_t                              vec_num; /* Vehicle number. */

    df_VehicleBasicInfor_st            infor[40]; /* Vehicle basic information. */
    
}Ehmp_RvBasicInfor_st, * Ehmp_RvBasicInfor_st;

#define Ehmp_RvBasicInfor_st_len    (sizeof(Ehmp_RvBasicInfor_st))


/* ehmp_RvFullInfor. */
typedef struct _Ehmp_RvFullInfor_st
{
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    uint8_t                              vec_num; /* Vehicle number. */

    df_VehicleFullInfor_st             infor[40]; /* Vehicle full information. */ 
    
}Ehmp_RvFullInfor_st, * Ehmp_RvFullInfor_st;

#define ehmp_RvFullInfor_st_len    (sizeof(ehmp_RvFullInfor_st))


/* Message Frame for all the ehmp mesage. */
typedef struct _EHMP_MessageFrame_st
{
    ehmp_msgid_en              messageid;
    
    union msg_un
    {
        Ehmp_HvAlertInfor_st     HvAlert; /* Host vehicle alert information. */
        Ehmp_HvBasicInfor_st     HvBasic; /* Host vehicle basic information. */
        Ehmp_HvFullInfor_st       HvFull; /* Host vehicle full information. */
        Ehmp_HvStaticInfor_st   HvStatic; /* Host vehicle static information. */

        Ehmp_RvAlertInfor_st     RvAlert; /* Remote vehicles alert information. */
        Ehmp_RvBasicInfor_st     RvBasic; /* Remote vehicles basic information. */
        Ehmp_RvFullInfor_st       RvFull; /* Remote vehicles full information. */ 
        
    }msg;
    
}EHMP_MessageFrame_st, * EHMP_MessageFrame_st_ptr;

#define EHMP_MESSAGEFRAME_ST_LEN        (sizeof(EHMP_MessageFrame_st))



/* Internal structure: Structure only used by ehmp protocol. *********************/

/* Ehmp message type. */
typedef enum _ehmp_msgtype_en
{
    EHMP_MSGTYPE_RESERVED         = 0,
    EHMP_MSGTYPE_DEBUG            = 1,
    EHMP_MSGTYPE_SYS_MANAGE       = 2,
    EHMP_MSGTYPE_V2X_APP          = 3,
    EHMP_MSGTYPE_DSRC             = 4,
    EHMP_MSGTYPE_GPS_APP          = 5
    
}ehmp_msgtype_en, *ehmp_msgtype_en_ptr;

#define EHMP_MSGTYPE_EN_LEN    (sizeof(ehmp_msgtype_en))


/* Ehmp debug message msgid. */
typedef enum _ehmp_debug_msgid_en
{
    EHMP_DEBUG_MSGID_RESERVE    = 0,
    
}ehmp_debug_msgid_en, *ehmp_debug_msgid_en_ptr;

#define EHMP_DEBUG_MSGID_EN_LEN    (sizeof(ehmp_debug_msgid_en))


/* Ehmp sys_manage message msgid. */
typedef enum _ehmp_sysmanage_msgid_en
{
    EHMP_SYSMANAGE_MSGID_RESERVE    = 0,
    
}ehmp_sysmanage_msgid_en, *ehmp_sysmanage_msgid_en_ptr;

#define EHMP_SYSMANAGE_MSGID_EN_LEN    (sizeof(ehmp_sysmanage_msgid_en))


/* Ehmp v2x_apply message msgid. */
typedef enum _ehmp_v2xapp_msgid_en
{
    EHMP_V2XAPP_RESERVED           = 0,    /* Reserved id. */

    EHMP_V2XAPP_HV_ALERT_INFOR     = 1,  /* Alert information for host vehicle. */
    EHMP_V2XAPP_HV_BASIC_INFOR,            /* Basic information for host vehicle. */
    EHMP_V2XAPP_HV_FULL_INFOR,             /* Full information for host vehicle. */
    EHMP_V2XAPP_HV_STATIC_INFOR,           /* Static information for host vehicle. */
    EHMP_V2XAPP_HV_SYSTEM_TIME,            /* System time for host vehicle. */
    EHMP_V2XAPP_HV_TLAMP_SPDGUIDE,         /* Traffic lamp and speed guide for host vehicle. */

    EHMP_V2XAPP_RS_ALERT_INFOR     = 100,  /* Alert information from roadside unit. */

    EHMP_V2XAPP_RV_BASIC_INFOR     = 180,  /* Basic information for all remote vehicles. */ 
    EHMP_V2XAPP_RV_FULL_INFOR,             /* Full information for all remote vehicles. */
    EHMP_V2XAPP_RV_ALERT_INFOR,            /* Alert information for alert remote vehicles. */

    EHMP_V2XAPP_MAX                = 255   /* Max id. */

}ehmp_v2xapp_msgid_en, *ehmp_v2xapp_msgid_en_ptr;

#define EHMP_V2XAPP_MSGID_EN_LEN    (sizeof(ehmp_v2xapp_msgid_en))


/* Ehmp dsrc message msgid. */
typedef enum _ehmp_dsrc_msgid_en
{
    EHMP_DSRC_MSGID_RESERVE    = 0,
    
}ehmp_dsrc_msgid_en, *ehmp_dsrc_msgid_en_ptr;

#define EHMP_DSRC_MSGID_EN_LEN    (sizeof(ehmp_dsrc_msgid_en))


/* Ehmp gps_apply message msgid. */
typedef enum _ehmp_gpsapp_msgid_en
{
    EHMP_GPSAPP_RESERVED           = 0,    /* Reserved id. */
    EHMP_GPSAPP_RTCMCORRECTION     = 1,    /* RTCM correction information. */
    EHMP_GPSAPP_NMEA_RAWMSG        = 2,    /* NMEA raw message. */

    EHMP_GPSAPP_MAX                = 255   /* Max id. */

}ehmp_gpsapp_msgid_en, *ehmp_gpsapp_msgid_en_ptr;

#define EHMP_GPSAPPLY_MSGID_EN_LEN    (sizeof(ehmp_gpsapp_msgid_en))


/* Save all the compiler settings. */
#pragma pack(push)
#pragma pack(1)

/* Ehmp message head structure. */
typedef struct _ehmp_msghead_st
{
#if(ENDIAN_STAT_LITT)

    uint8_t    reserved1:2;  /* Reserved. */
    uint8_t    src      :2;  /* Message source ID. */
    uint8_t    mark     :4;  /* Message mark. 0x0E. */
    
    uint8_t    type     :4;  /* Message type. */
    uint8_t    reserved2:4;  /* Reserved. */
    
#else

    uint8_t    mark     :4;  /* Message mark. 0x0E. */
    uint8_t    src      :2;  /* Message source ID. */
    uint8_t    reserved1:2;  /* Reserved. */
    
    uint8_t    reserved2:4;  /* Reserved. */
    uint8_t    type     :4;  /* Message type. */
    
#endif
}ehmp_msghead_st, *ehmp_msghead_st_ptr;

#define EHMP_MSGHEAD_ST_LEN    (sizeof(ehmp_msghead_st))

/* Message mark. */
#define EHMP_MSGHEAD_MARK     0x0E


/* ehmp_HvAlertInfor. */
typedef struct _ehmp_HvAlertInfor_st
{
    ehmp_msghead_st                     msg_head; /* Message head. */
    uint8_t                               msg_id; /* Message id. */
    
    /* Add later. */
    
}ehmp_HvAlertInfor_st, * ehmp_HvAlertInfor_st;

#define ehmp_HvAlertInfor_st_len    (sizeof(ehmp_HvAlertInfor_st))


/* ehmp_HvBasicInfor. */
typedef struct _ehmp_HvBasicInfor_st
{
    ehmp_msghead_st                     msg_head; /* Message head. */
    uint8_t                               msg_id; /* Message id. */

    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    DE_TemporaryID_st                         id; /* Product id. */
    
    Position3D_st                            pos; /* position */
    uint16_t                               speed; /* Driving speed. */
    uint16_t                             heading; /* Driving direction. */
    
}ehmp_HvBasicInfor_st, * ehmp_HvBasicInfor_st;

#define ehmp_HvBasicInfor_st_len    (sizeof(ehmp_HvBasicInfor_st))


/* ehmp_HvFullInfor. */
typedef struct _ehmp_HvFullInfor_st
{
    ehmp_msghead_st                     msg_head; /* Message head. */
    uint8_t                               msg_id; /* Message id. */
    
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    DE_TemporaryID_st                         id; /* Product id. */
    
    Position3D_st                            pos; /* position */
    uint8_t                                trans; /* Transmission state. */
    uint16_t                               speed; /* Driving speed. */
    uint16_t                             heading; /* Driving direction. */
    uint8_t                       steering_angle; /* Steering wheel angle. */
    AccelerationSet4Way_st       accelerationSet; /* Acceleration set in 4 way. */
    BrakeSystemStatus_st            brake_status; /* Brake system status. */
    DE_ExteriorLights_un              ext_lights; /* Exterior lights. */
    
}ehmp_HvFullInfor_st, * ehmp_HvFullInfor_st_ptr;

#define ehmp_HvFullInfor_st_len    (sizeof(ehmp_HvFullInfor_st))


/* ehmp_HvStaticInfor. */
typedef struct _ehmp_HvStaticInfor_st
{
    ehmp_msghead_st                     msg_head; /* Message head. */
    uint8_t                               msg_id; /* Message id. */
    
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    VehicleSize_st                      vec_size; /* Vehicle size. */
    
}ehmp_HvStaticInfor_st, * ehmp_HvStaticInfor_st;

#define ehmp_HvStaticInfor_st_len    (sizeof(ehmp_HvStaticInfor_st))


/* ehmp_HvSystemTime. */
typedef struct _ehmp_HvSystemTime_st
{
    ehmp_msghead_st                     msg_head; /* Message head. */
    uint8_t                               msg_id; /* Message id. */

    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    
}ehmp_HvSystemTime_st, * ehmp_HvSystemTime_st;

#define ehmp_HvSystemTime_st_len    (sizeof(ehmp_HvSystemTime_st))


/* ehmp_HvTrafficLampSpeedGuide. */
typedef struct _ehmp_HvTLampSpdGuide_st
{
    ehmp_msghead_st                     msg_head; /* Message head. */
    uint8_t                               msg_id; /* Message id. */
    
    DE_TrafficLamps_un                  turnLeft; /* Lamp in turn left direction. */
    DE_TrafficLamps_un                  straight; /* Lamp in staight direction. */
    DE_TrafficLamps_un                 turnright; /* Lamp in turn right direction. */

    uint16_t                        cntdown_time; /* Countdown time in active direction. unit: 0.1s. */                      
    uint16_t                      speedGuide_max; /* Speed guide max. */
    uint16_t                      speedGuide_min; /* Speed guide min. */
    
}ehmp_HvTLampSpdGuide_st, * ehmp_HvTLampSpdGuide_st;

#define ehmp_HvTLampSpdGuide_st_len    (sizeof(ehmp_HvTLampSpdGuide_st))


/* ehmp_RvAlertInfor. */
typedef struct _ehmp_RvAlertInfor_st
{
    ehmp_msghead_st                     msg_head; /* Message head. */
    uint8_t                               msg_id; /* Message id. */
    
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    uint8_t                              vec_num; /* Vehicle number. */
    
    VehicleBasicInfor_st_ptr           infor_ptr; /* Vehicle basic information. */ 
    
}ehmp_RvAlertInfor_st, * ehmp_RvAlertInfor_st;

#define ehmp_RvAlertInfor_st_len    (sizeof(ehmp_RvAlertInfor_st))


/* ehmp_RvBasicInfor. */
typedef struct _ehmp_RvBasicInfor_st
{
    ehmp_msghead_st                     msg_head; /* Message head. */
    uint8_t                               msg_id; /* Message id. */
    
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    uint8_t                              vec_num; /* Vehicle number. */

    VehicleBasicInfor_st_ptr           infor_ptr; /* Vehicle basic information. */
    
}ehmp_RvBasicInfor_st, * ehmp_RvBasicInfor_st;

#define ehmp_RvBasicInfor_st_len    (sizeof(ehmp_RvBasicInfor_st))


/* ehmp_RvFullInfor. */
typedef struct _ehmp_RvFullInfor_st
{
    ehmp_msghead_st                     msg_head; /* Message head. */
    uint8_t                               msg_id; /* Message id. */
    
    DE_SystemTimeMS_t                     timeMs; /* System time in ms. */
    uint8_t                              vec_num; /* Vehicle number. */

    VehicleFullInfor_st_ptr            infor_ptr; /* Vehicle full information. */ 
    
}ehmp_RvFullInfor_st, * ehmp_RvFullInfor_st;

#define ehmp_RvFullInfor_st_len    (sizeof(ehmp_RvFullInfor_st))


/* restore all compiler settings in stacks. */
#pragma pack(pop)

#endif

