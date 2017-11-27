/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_msg_frame.h
 @brief  : MSG_MessageFrame(MMF) routine set.
 @author : pengrui
 @history:
           2017-09-08    pengrui    Created file
           2017-11-23    pengrui    Add the check of all child msg encode
           ...
******************************************************************************/
#ifndef _CV_MSG_FRAME_H_
#define    _CV_MSG_FRAME_H_

#include "cv_data_element.h"
#include "cv_data_frame.h"
#include "MessageFrame.h"
#include "error.h"


/* ITIS code definition. from J2540-2 doc. */
typedef enum _ITIScode_em
{
    ITIScode_CROSS_WITH_CARE   = 0x1C07,
    ITIScode_CURVE             = 0x1F5A,
    ITIScode_TUNNEL            = 0x2029,
    
    ITIScode_BRIDGE            = 0x2025,
    ITIScode_INTERSECTION      = 0x1F60,
    ITIScode_SIGNAL_LIGHT      = 0x1D07,
    ITIScode_SPEED_RESTRICTION = 0x0A04,
    ITIScode_EMERGENCY_VEC     = 0x0704,
    ITIScode_MAX               = 0x0000,

}ITIScode_em, *ITIScode_em_ptr;

#define ITIScode_em_len    (sizeof(ITIScode_em))


/* MSG_BasicSafetyMessage optional configuration. */
typedef struct _MSG_BasicSafetyMessage_opt_st
{
    uint8_t    msg_BasicSafetyMessage_opt_plateno          :1;
    uint8_t    msg_BasicSafetyMessage_opt_angle            :1;
    uint8_t    msg_BasicSafetyMessage_opt_motioncfd        :1;
    uint8_t    msg_BasicSafetyMessage_opt_safetyExt        :1;
    uint8_t msg_BasicSafetyMessage_opt_reserved         :4;  
    
}MSG_BasicSafetyMessage_opt_st, *MSG_BasicSafetyMessage_opt_st_ptr;

#define BSM_MSG_OPT_ST_LEN    (sizeof(MSG_BasicSafetyMessage_opt_st))


/* MSG_BasicSafetyMessage. */
typedef struct _MSG_BasicSafetyMessage_st
{ 
    /* Optional. */
    MSG_BasicSafetyMessage_opt_st            opt;

    /* Data */
    DE_MsgCount_t                         msgCnt; /* Msg count. */
    DE_TemporaryID_st                         id; /* Product id. */
    DE_PlateNo_st                        plateno;    /* plate no optional */
    DE_DSecond_t                         secMark; /* DSRC second. Unit: millisecond. */
    DF_Position3D_st                         pos;    /* position */
    DF_PositionConfidenceSet_st         accuracy; /* Vehicle position accuracy. */
    DE_TransmissionState_en                trans; /* Transmission status. */
    DE_Speed_t                             speed; /* Driving speed. Unit km/h. */
    DE_Heading_t                         heading; /* Driving direction. Unit degree. */
    DE_SteeringWheelAngle_t                angle;  /* Steering wheel angle. Unit degree. optional*/
    DF_MotionConfidenceSet_st          motioncfd;    /* motion confidenceset optional*/    
    DF_AccelerationSet4Way_st            acceSet;  /* Driving acceleration set. */
    DF_BrakeSystemStatus_st               brakes;  /* Brake system status. */
    DF_VehicleSize_st                       size;  /* Vehicle size. */
     DF_VehicleClassification_st     vehicleclass;/*vehicle class*/
    DF_VehicleSafetyExtensions_st      safetyExt; /* vehicle safety extensions optional */
    
}MSG_BasicSafetyMessage_st, * MSG_BasicSafetyMessage_st_ptr;

#define BSM_MSG_ST_LEN    (sizeof(MSG_BasicSafetyMessage_st))


/* MSG_BasicSafetyMessage optional configuration. */
typedef struct _MSG_MapData_opt_st
{
    uint8_t    msg_MapData_opt_timestamp           :1;
    uint8_t msg_MapData_opt_reserved            :7;
    
}MSG_MapData_opt_st, *MSG_MapData_opt_st_ptr;

#define MSG_MAP_OPT_ST_LEN    (sizeof(MSG_Map_opt_st))


/* MSG_MapData. */
typedef struct _MSG_MapData_st
{ 
    /* Optional. */
    MSG_MapData_opt_st               opt;

    /* Data */
    DE_MsgCount_t                 msgCnt; /* Msg count. */
    DE_MinuteOfTheYear_t       timestamp; /* the value 527040 shall be used for invalid optional */
    DF_NodeList_st                 nodes; /* intersections or road endpoints */
    
}MSG_MapData_st, * MSG_MapData_st_ptr;

#define MAP_MSG_ST_LEN    (sizeof(MSG_MapData_st))


/* MSG_BasicSafetyMessage optional configuration. */
typedef struct _MSG_RoadSideInformation_opt_st
{
    uint8_t    msg_RoadSideInformation_opt_timestamp           :1;
    uint8_t    msg_RoadSideInformation_opt_description         :1;
    uint8_t    msg_RoadSideInformation_opt_priority            :1;
    uint8_t msg_RoadSideInformation_opt_reserved            :5;
    
}MSG_RoadSideInformation_opt_st, *MSG_RoadSideInformation_opt_st_ptr;


/* MSG_BasicSafetyMessage structure. */
typedef struct _MSG_RoadSideInformation_st
{ 
    /* Optional. */
    MSG_RoadSideInformation_opt_st            opt;

    /* Data */
    DE_MsgCount_t                          msgCnt; /* Msg count. */
    DE_MinuteOfTheYear_t                timestamp; /* the value 527040 shall be used for invalid optional */
    DE_TemporaryID_st                          id; /* RSU ID */
    DE_RSIID_t                              rsiId; /* local ID of this rsi information set by RSU */
    DE_AlertType_t                      alertType; /* Text message warning or Traffic sign type according to China GB5768 */
    DE_Description_st                 description; /* Text message if alertType = 0 */
    DE_Priority_st                       priority; /* the urgency of this message, a relative */
    DF_Position3D_st                       refPos; /* Position of traffic alert (traffic sign or incident) */
    DF_PathPointList_st                 alertPath; /* Warning is active if vehicle is within this path */
    DE_Radius_t                       alertRadius;/* The biggest distance away from the alert path */
    
}MSG_RoadSideInformation_st, * MSG_RoadSideInformation_st_ptr;

#define RSI_MSG_ST_LEN    (sizeof(MSG_RoadSideInformation_st))


/* MSG_RoadSideAlert. */
typedef struct _MSG_RoadSideSafetyMessage_st
{ 
    DE_MsgCount_t                           msgCnt; /* Message count. */
    DE_TemporaryID_st                           id; /* RSU id */
    DF_Position3D_st                        refPos;    /* Reference position of this RSM message */
    DF_ParticipantList_st             participants; /* All or part of the participants */
    
} MSG_RoadSideSafetyMessage_st, * MSG_RoadSideSafetyMessage_st_ptr;

#define RSM_MSG_ST_LEN    (sizeof(MSG_RoadSideSafetyMessage_st))


/* MSG_SPAT optional configuration. */
typedef struct _MSG_SPAT_opt_st
{
    uint8_t    timestamp          :1;
    uint8_t    name               :1;
        uint8_t reserved           :6;
    
}MSG_SPAT_opt_st, *MSG_SPAT_opt_st_ptr;

#define MSG_MAP_OPT_ST_LEN    (sizeof(MSG_Map_opt_st))


/* MSG_SPAT. */
typedef struct _MSG_SPAT_st
{ 
    /* Optional. */
    MSG_SPAT_opt_st                          opt;

    /* Data */
    DE_MsgCount_t                         msgCnt; /* Msg count. */
    DE_MinuteOfTheYear_t               timestamp; /* the value 527040 shall be used for invalid optional */
    DE_DescriptiveName_st                   name; /* human readable name for this collection */
    DF_IntersectionStateList_st    intersections; /* sets of SPAT data */
    
}MSG_SPAT_st, * MSG_SPAT_st_ptr;

#define SPAT_MSG_ST_LEN    (sizeof(MSG_SPAT_st))


/* Message frame id. */
typedef enum _MSG_MessageFrame_ID_en
{
    MSG_MessageFrame_ID_MAPDATA = 18,
    MSG_MessageFrame_ID_BSM        = 20,
    MSG_MessageFrame_ID_RSI     = 40,
    MSG_MessageFrame_ID_SPAT    = 49,
    MSG_MessageFrame_ID_RSM     = 50
    
}MSG_MessageFrame_ID_en;


/* Message Frame for all the mesage. */
typedef struct _MSG_MessageFrame_st
{
    MSG_MessageFrame_ID_en          messageid;
    
    union msg_un
    {
        MSG_BasicSafetyMessage_st     msg_bsm;
        MSG_MapData_st            msg_mapdata;   
        MSG_RoadSideInformation_st    msg_rsi;
        MSG_RoadSideSafetyMessage_st  msg_rsm;
        MSG_SPAT_st                  msg_spat;
    }msg;
    
}MSG_MessageFrame_st, * MSG_MessageFrame_st_ptr;

#define MSG_MESSAGEFRAME_ST_LEN        (sizeof(MSG_MessageFrame_st))

/* decode frame msg, if return = 0, then indicate success, else indicate error. */
extern int frame_decode_msg(MSG_MessageFrame_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size);

/* encode frame msg, if return > 0, then indicate that the byte size of the encode buffer, else indicate error. */
extern int frame_encode_msg(MSG_MessageFrame_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size);


#endif /* _CV_MSG_FRAME_H_ */

