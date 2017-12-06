/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_data_frame.h
 @brief  : Data Frame routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           2017-10-12     pengrui       Modify for CSAE
           2017-11-23    pengrui        Fix the BUG of the check of TimeChangeDetail malloc 
           ...
******************************************************************************/
#ifndef __CV_DATA_FRAME_H__
#define __CV_DATA_FRAME_H__

#include "cv_data_element.h"
#include "AccelerationSet4Way.h"
#include "BrakeSystemStatus.h"
#include "DDateTime.h"
#include "FullPositionVector.h"
#include "PathHistory.h"
#include "PathPointList.h"
#include "PathPrediction.h"
#include "ParticipantData.h"
#include "ParticipantList.h"
#include "VehicleSafetyExtensions.h"
#include "VehicleClassification.h"
#include "VehicleSize.h"
#include "IntersectionStateList.h"
#include "NodeList.h"
#include "RoadPoint.h"

/*Message optinal status. */
#define MSG_OPTIONAL_YES    0x01
#define MSG_OPTIONAL_NO     0x00


/*Acceleration set 4 way structure. */
typedef struct _DF_AccelerationSet4Way_st
{   
    /* Along the vehicle longitudinal axis. Unit: m/s^2. */
    DE_Acceleration_t              longitudinal;

    /* Along the vehicle lateral axis. Unit: m/s^2. */
    DE_Acceleration_t                   lateral;

    /* Alone the vehicle vertical axis. Unit: m/s^2. */
    DE_VerticalAcceleration_t          vertical;
    
    /* Vehicle's yaw rate. Unit: degrees per second. */
    DE_YawRate_t                       yaw_rate;
    
}DF_AccelerationSet4Way_st, * DF_AccelerationSet4Way_st_ptr;

#define DF_AccelerationSet4Way_st_len    (sizeof(DF_AccelerationSet4Way_st))

/*DF_AntennaOffsetSet. */
typedef struct _DF_AntennaOffsetSet_st
{
    /* Antenna offset to vehicle center alone X axis. */
    float antiOffsetX_B12;

    /* Antenna offset to vehicle center alone Y axis. */
    float antiOffsetY_B09;

    /* Antenna offset to vehicle center alone Z axis. */
    float antiOffsetZ_B10;
    
}DF_AntennaOffsetSet_st, * DF_AntennaOffsetSet_st_ptr;

#define DF_AntennaOffsetSet_st_len    (sizeof(DF_AntennaOffsetSet_st))

/* DF_BrakeSystemStatus structure*/
typedef struct _DF_BrakeSystemStatus_opt_st
{
    uint8_t BrakeSystemStatus_opt_brakepadel                :1;
    uint8_t BrakeSystemStatus_opt_wheelBrakes               :1;
    uint8_t    BrakeSystemStatus_opt_traction                  :1;
    uint8_t BrakeSystemStatus_opt_abs                       :1;
    uint8_t BrakeSystemStatus_opt_scs                       :1;
    uint8_t    BrakeSystemStatus_opt_brakeBoost                :1;
    uint8_t    BrakeSystemStatus_opt_auxBrakes                 :1;
    uint8_t    BrakeSystemStatus_opt_reserved                  :1;
}DF_BrakeSystemStatus_opt_st, *DF_BrakeSystemStatus_opt_st_ptr;

/*DF_BrakeSystemStatus. */
typedef struct _DF_BrakeSystemStatus_st
{
    /* Optional */
    DF_BrakeSystemStatus_opt_st        opt;
    
    /* Data */
    DE_BrakePedalStatus_en      brakepedal;
    DE_BrakeAppliedStatus_st   wheelBrakes;
    DE_TractionControlStatus_en   traction;
    DE_AntiLockBrakeStatus_en          abs;
    DE_StabilityControlStatus_en       scs;
    DE_BrakeBoostApplied_en     brakeBoost;
    DE_AuxiliaryBrakeStatus_en   auxBrakes;
}DF_BrakeSystemStatus_st, * DF_BrakeSystemStatus_st_ptr;

#define DF_BrakeSystemStatus_st_len    (sizeof(DF_BrakeSystemStatus_st))

/*DF_ConnectingLane_opt structure */
typedef struct _DF_ConnectingLane_opt_st
{
    uint8_t DF_ConnectingLane_opt_maneuver        :1;
    uint8_t DF_ConnectingLane_opt_reserved        :7;
}DF_ConnectingLane_opt_st, *DF_ConnectingLane_opt_st_ptr;

/*DF_ConnectingLane structure */
typedef struct _DF_ConnectingLane_st
{
    /* optional */
    DF_ConnectingLane_opt_st                     opt;

    /* data */
    DE_LaneID_t                                 lane;
    DE_AllowedManeuvers_un                  maneuver;
    
}DF_ConnectingLane_st, *DF_ConnectingLane_st_ptr;

#define DF_ConnectingLane_st_len        (sizeof(DF_ConnectingLane_st))

/*DF_NodeReferenceID_opt structure */
typedef struct _DF_NodeReferenceID_opt_st
{
    uint8_t        region      :1;
    uint8_t        reserved    :7;
}DF_NodeReferenceID_opt_st, *DF_NodeReferenceID_opt_st_ptr;

/*DF_NodeReferenceID_st */
typedef struct _DF_NodeReferenceID_st
{
    /* optional */
    DF_NodeReferenceID_opt_st         opt;

    /* Data */
    DE_RoadRegulatorID_t           region;
    DE_NodeID_t                        id; /*  The values zero through 255 are allocated for testing purposes */
}DF_NodeReferenceID_st, *DF_NodeReferenceID_st_ptr;

#define DF_NodeReferenceID_st_len        (sizeof(DF_NodeReferenceID_st))

/*DF_Connection_opt structure*/
typedef struct _DE_Connection_opt_st
{
    uint8_t Connection_opt_connectingLane           :1;
    uint8_t Connection_opt_phaseId                  :1;
    uint8_t Connection_opt_reserved                 :6;
}DE_Connection_opt_st, *DE_Connection_opt_st_ptr;

/*DF_Connection structure */
typedef struct _DF_Connection_st
{
    /* optional */
    DE_Connection_opt_st                          opt;

    /* Data */
    DF_NodeReferenceID_st          remoteIntersection;
    DF_ConnectingLane_st               connectingLane;
    DE_PhaseID_t                              phaseid;
}DF_Connection_st, *DF_Connection_st_ptr;

#define DF_Connection_st_len        (sizeof(DF_Connection_st))

/*DF_ConnectsToList structure */
typedef struct _DF_ConnectsToList_st
{
    uint8_t                ConnectionToList_size;
    DF_Connection_st    ConnectionToList_data[8];
}DF_ConnectsToList_st, *DF_ConnectsToList_st_ptr;

#define DF_ConnectsToList_st_len        (sizeof(DF_ConnectsToList_st))

/* DF_DDateTime optional configuration. */
typedef struct _DF_DDateTime_opt_st
{
    uint8_t DDateTime_opt_DYear            :1;
    uint8_t DDateTime_opt_DMonth           :1;
    uint8_t DDateTime_opt_DDay             :1;
    uint8_t DDateTime_opt_DHour            :1;
    uint8_t DDateTime_opt_DMinute          :1;
    uint8_t DDateTime_opt_DSecond          :1;
    uint8_t DDateTime_opt_DOffset          :1;
    uint8_t DDateTime_opt_reserved         :1;
}DF_DDateTime_opt_st, *DF_DDateTime_opt_st_ptr;

/*DF_DDateTime. */
typedef struct _DF_DDateTime_st
{
    /* Optional. */
    DF_DDateTime_opt_st                  opt;

    /* Data. */
    DE_DYear_t                           year;
    DE_DMonth_t                         month;
    DE_DDay_t                            day;
    DE_DHour_t                          hour;
    DE_Dminute_t                      minute;
    DE_DSecond_t                      second;
    DE_DTimeOffset_t                  offset;

}DF_DDateTime_st, * DF_DDateTime_st_ptr;

#define _DF_DDateTime_st_len    (sizeof(_DF_DDateTime_st))

/*DE_TimeChangeDetials_opt structure */
typedef struct _DE_TimeChangeDetails_opt_st
{
    uint8_t        minEndTime        :1;
    uint8_t        maxEndTime        :1;
    uint8_t        TimeConfidence    :1;
    uint8_t        nextStartTime    :1;
    uint8_t        nextDuration    :1;
    uint8_t        reserved        :3;
    
}DF_TimeChangeDetials_opt_st, *DF_TimeChangeDetials_opt_st_ptr;

/*DF_TimeChangeDetials structure */
typedef struct _DF_TimeChangeDetials_st
{
    /* Optional */
    DF_TimeChangeDetials_opt_st                     opt;

    /* Data */
    DE_TimeMark_t                          startTime;
    DE_TimeMark_t                         minEndTime;
    DE_TimeMark_t                         maxEndTime;
    DE_TimeMark_t                      likelyEndTime;
    DE_Confidence_t                   timeConfidence;
    DE_TimeMark_t                      nextStartTime;
    DE_TimeMark_t                       nextDuration;
}DF_TimeChangeDetials_st, *DF_TimeChangeDetials_st_ptr;

#define DF_TimeChangeDetials_st_len        (sizeof(DF_TimeChangeDetials_st))

/*DF_PhaseState_opt structure*/
typedef struct _DF_PhaseState_opt_st
{
    uint8_t    PhaseState_opt_timing        :1;
    uint8_t    PhaseState_opt_reserved        :7;
}DF_PhaseState_opt_st, *DF_PhaseState_opt_st_ptr;

/*DF_PhaseState structure */
typedef struct _DF_PhaseState_st
{
    /* Optional */
    DF_PhaseState_opt_st                opt;
    
    /* Data */
    DE_LightState_en                  light;
    DF_TimeChangeDetials_st          timing;
}DF_PhaseState_st, *DF_PhaseState_st_ptr;

#define DF_PhaseState_st_len        (sizeof(DF_PhaseState_st))

/*DF_PhaseStateList */
typedef struct _DF_PhaseStateList_st
{
    uint8_t                pointNum;
    DF_PhaseState_st      array[16];
    
}DF_PhaseStateList_st, *DF_PhaseStateList_st_ptr;

#define DF_PhaseStateList_st_len            (sizeof(DF_PhaseStateList_st))

/*DF_Phase structure */
typedef struct _DF_Phase_st
{
    uint8_t                    id;
    DF_PhaseStateList_st    phaseStates;
}DF_Phase_st, *DF_Phase_st_ptr;

#define DF_Phase_st_len            (sizeof(DF_Phase_st))

/* DF_PhaseList structure */
typedef struct _DF_PhaseList_st
{
    uint8_t         pointNum;
    DF_Phase_st    array[16];
    
}DF_PhaseList_st, *DF_PhaseList_st_ptr;

#define DF_PhastList_st_len        (sizeof(DF_PhastList_st))

/*DF_IntersectionState_opt structure */
typedef struct _DF_IntersectionState_opt_st
{
    uint8_t moy              :1;
    uint8_t    timeStamp        :1;
    uint8_t    reserved         :6;
    
}DF_IntersectionState_opt_st, *DF_IntersectionState_opt_st_ptr;

/*DF_IntersectionState */
typedef struct _DF_IntersectionState_st
{
    /* Optional */
    DF_IntersectionState_opt_st                         opt;
    
    /* Data */
    DF_NodeReferenceID_st                 intersectionId;
    DE_IntersectionStatusObject_un                  status;
    DE_MinuteOfTheYear_t                             moy;
    DE_DSecond_t                               timeStamp;
    DF_PhaseList_st                                  phases;
}DF_IntersectionState_st, *DF_IntersectionState_st_ptr;

#define DF_IntersectionState_st_len        (sizeof(DF_IntersectionState_st))

/*DF_IntersectionStateList */
typedef struct _DF_IntersectionStateList_st
{
    uint8_t                       pointNum;
    DF_IntersectionState_st      array[32];
    
}DF_IntersectionStateList_st, *DF_IntersectionStateList_st_ptr;

#define DF_IntersectionStateList_st_len            (sizeof(DF_IntersectionStateList_st))

/*DF_LaneTypeAttributes*/
typedef struct _DF_LaneTypeAttributes_st
{
    LaneTypeAttributes_PR                          present;
    union {
    DE_LaneAttributes_Vehicle_un                  vehicle;
    DE_LaneAttributes_Crosswalk_un              crosswalk;
    DE_LaneAttributes_Bike_un                    bikeLane;
    DE_LaneAttributes_Sidewalk_un                sidewalk;
    DE_LaneAttributes_Barrier_un                   median;
    DE_LaneAttributes_Striping_un                striping;
    DE_LaneAttributes_TrackedVehicle_un    trackedvehicle;    
    DE_LaneAttributes_Parking_un                  parking;        
    }choice;
}DF_LaneTypeAttributes_st, * DF_LaneTypeAttributes_st_ptr;

#define DF_LaneTypeAttributes_st_len        (sizeof(DF_LaneTypeAttributes_st))

/*DF_LaneAttributes_opt structure */
typedef struct _DF_LaneAttributes_opt_st
{
    uint8_t    LaneAttributes_opt_sharewith       :1;
    uint8_t    LaneAttributes_opt_reserved        :7;
}DF_LaneAttributes_opt_st, * DF_LaneAttributes_opt_st_ptr;

/*DF_LaneAttributes structure */
typedef struct _DF_LaneAttributes_st
{
    /* optional */
    DF_LaneAttributes_opt_st              opt;
    
    /* Data */
    DE_LaneSharing_un               shareWith;
    DF_LaneTypeAttributes_st         laneType;

}DF_LaneAttributes_st, *DF_LaneAttributes_st_ptr;

#define DF_LaneAttributes_st_len        (sizeof(DF_LaneAttributes_st))

/*DF_RegulatorySpeedLimit structure*/
typedef struct _DF_RegulatorySpeedLimit_st
{
    DE_SpeedLimitType_en        type;
    DE_Speed_t                 speed;
}DF_RegulatorySpeedLimit_st, *DF_RegulatorySpeedLimit_st_ptr;

#define DF_RegulatorySpeedLimit_st_len        (sizeof(DF_RegulatorySpeedLimit_st))


/*DF_SpeedLimitList structure */
typedef struct _DF_SpeedLimitList_st
{
    uint8_t                        SpeedLimitList_size;
    DF_RegulatorySpeedLimit_st   SpeedLimitList_data[9];
}DF_SpeedLimitList_st, * DF_SpeedLimitList_st_ptr;

#define DF_SpeedLimitList_st_len        (sizeof(DF_SpeedLimitList_st))

/*DF_Position_LL_24B structure */
typedef struct _DF_Position_LL_24B_st
{
    DE_OffsetLL_B12_t     lon; /* OffsetLL-B12 A range of +- 0.0002047 degrees */
    DE_OffsetLL_B12_t     lat; /* OffsetLL-B12 A range of +- 0.0002047 degrees */
}DF_Position_LL_24B_st, *DF_Position_LL_24B_st_ptr;

#define DF_Position_LL_24B_st_len        (sizeof(DF_Position_LL_24B_st))

/*DF_Position_LL_28B structure */
typedef struct _DF_Position_LL_28B_st
{
    DE_OffsetLL_B14_t     lon; /* OffsetLL-B14 A range of +- 0.0008191 degrees */
    DE_OffsetLL_B14_t     lat; /* OffsetLL-B14 A range of +- 0.0008191 degrees */
}DF_Position_LL_28B_st, *DF_Position_LL_28B_st_ptr;    

#define DF_Position_LL_28B_st_len    (sizeof(DF_Position_LL_28B_st))

/*DF_Position_LL_32B */
typedef struct _DF_Position_LL_32B_st
{
    DE_OffsetLL_B16_t       lon; /* OffsetLL-B16 A range of +- 0.0032767 degrees */
    DE_OffsetLL_B16_t      lat; /* OffsetLL-B16 A range of +- 0.0032767 degrees */
}DF_Position_LL_32B_st, *DF_Position_LL_32B_st_ptr;

#define    DF_Position_LL_32B_st_len    (sizeof(DF_Position_LL_32B_st))

/*DF_Position_LL_36B */
typedef struct _DF_Position_LL_36B_st
{
    DE_OffsetLL_B18_t      lon; /* OffsetLL-B18 A range of +- 0.0131071 degrees */
    DE_OffsetLL_B18_t      lat; /* OffsetLL-B18 A range of +- 0.0131071 degrees */
}DF_Position_LL_36B_st, *DF_Position_LL_36B_st_ptr;

#define DF_Position_LL_36_st_len    (sizeof(DF_Position_LL_36_st))

/*DF_Position_LL_44B */    
typedef struct _DF_Position_LL_44B_st
{
    DE_OffsetLL_B22_t      lon; /* OffsetLL-B22 A range of +- 0.2097151 degrees */
    DE_OffsetLL_B22_t      lat; /* OffsetLL-B22 A range of +- 0.2097151 degrees */
}DF_Position_LL_44B_st, *DF_Position_LL_44B_st_ptr;

#define DF_Position_LL_44B_st_len    (sizeof(DF_Position_LL_44B_st))
    
/*DF_Position-LL-48B    */
typedef struct _DF_Position_LL_48B_st
{
    DE_OffsetLL_B24_t      lon; /* OffsetLL-B24 A range of +- 0.8388607 degrees */
    DE_OffsetLL_B24_t      lat; /* OffsetLL-B24 A range of +- 0.8388607 degrees */
}DF_Position_LL_48B_st, *DF_Position_LL_48B_st_ptr;

#define DF_Position_LL_48B_st_len (sizeof(DF_Position_LL_48B_st))

/*DF_Position-LLmD-64b */
typedef struct _DF_Position_LLmD_64b_st
{
    DE_Longitude_t         lon; /* Longitude */
    DE_Latitude_t          lat; /* Latitude */
}DF_Position_LLmD_64b_st, *DF_Position_LLmD_64b_st_ptr;        

#define    DF_Position_LLmD_64b_st_len        (sizeof(DF_Position_LLmD_64b_st))


/*DF_PositionOffsetLL structure */
typedef struct _DF_PositionOffsetLL_st
{
    DE_PositionOffsetLL_en             choiceid;
    union posoffset_un{
        DF_Position_LL_24B_st      position_LL1;
        DF_Position_LL_28B_st      position_LL2;
        DF_Position_LL_32B_st      position_LL3;
        DF_Position_LL_36B_st      position_LL4;
        DF_Position_LL_44B_st      position_LL5;
        DF_Position_LL_48B_st      position_LL6;
        DF_Position_LLmD_64b_st position_LatLon;
    }choice;
}DF_PositionOffsetLL_st, *DF_PositionOffsetLL_st_ptr;

#define DF_PositionOffsetLL_st_len        (sizeof(DF_PositionOffsetLL_st))

/*DF_VerticalOffset structure */
typedef struct _DF_VerticalOffset_st
{
    DE_VerticalOffset_en          choiceid;
    union vertoffset_un{
        /* All below in steps of 10cm above or below the reference ellipsoid */
        DE_VertOffset_B07_t         offset1; /* with a range of +- 6.3 meters vertical */
        DE_VertOffset_B08_t         offset2; /* with a range of +- 12.7 meters vertical */
        DE_VertOffset_B09_t         offset3; /* with a range of +- 25.5 meters vertical */
        DE_VertOffset_B10_t         offset4; /* with a range of +- 51.1 meters vertical */
        DE_VertOffset_B11_t         offset5; /* with a range of +- 102.3 meters vertical */
        DE_VertOffset_B12_t            offset6; /* with a range of +- 204.7 meters vertical */
        DE_Elevation_t            elevation; /* with a range of -409.5 to + 6143.9 meters */
    }choice;
}DF_VerticalOffset_st, *DF_VerticalOffset_st_ptr;

#define DF_VerticalOffset_st_len        (sizeof(DF_VerticalOffset_st))

/*DF_PositionOffsetLLV_opt structure*/
typedef struct _DF_PositionOffsetLLV_opt_st
{
    uint8_t PositionOffsetLLV_opt_VerticalOffset     :1;
    uint8_t PositionOffsetLLV_opt_reserved           :7;
}DF_PositionOffsetLLV_opt_st, *DF_PositionOffsetLLV_opt_st_ptr;

/*DF_PositionOffsetLLV structure*/
typedef struct _DF_PositionOffsetLLV_st
{
    /* Optional */
    DF_PositionOffsetLLV_opt_st              opt;

    /* Data */
    DF_PositionOffsetLL_st              offsetLL; /* offset in lon/lat */
    DF_VerticalOffset_st                 offsetV; /* offset in elevation OPTIONAL */
}DF_PositionOffsetLLV_st, *DF_PositionOffsetLLV_st_ptr;

#define DF_PositionOffsetLLV_st_len        (sizeof(DF_PositionOffsetLLV_st))

/*DF_RoadPoint structure */
typedef struct _DF_RoadPoint_st
{
    DF_PositionOffsetLLV_st                         posOffset;
}DF_RoadPoint_st, *DF_RoadPoint_st_ptr;

#define DF_RoadPoint_st_len        (sizeof(DF_RoadPoint_st))

/*DF_PointList structure */
typedef struct _DF_PointList_st
{
    uint8_t                     PointList_size;
    DF_RoadPoint_st         PointList_data[31];    
}DF_PointList_st, *DF_PointList_st_ptr;

#define DF_PointList_st_len        (sizeof(DF_PointList_st))

/*DF_Lane_opt structure*/
typedef struct _DF_Lane_opt_st
{
    uint8_t Lane_opt_lanAttributes          :1;
    uint8_t Lane_opt_maneuvers              :1;
    uint8_t Lane_opt_connectsTo             :1;
    uint8_t Lane_opt_speedLimits            :1;
    uint8_t Lane_opt_points                 :1;
    uint8_t Lane_opt_reserved               :3;
}DF_Lane_opt_st, * DF_Lane_opt_st_ptr;

/*DF_Lane structure */
typedef struct _DF_Lane_st
{
    /* optional */
    DF_Lane_opt_st                    opt;

    /* data */
    DE_LaneID_t                    laneID;
    DF_LaneAttributes_st   laneAttributes;
    DE_AllowedManeuvers_un      maneuvers;
    DF_ConnectsToList_st       connectsTo;
    DF_SpeedLimitList_st      speedLimits;
    DF_PointList_st                points;
}DF_Lane_st, * DF_Lane_st_ptr;

#define DF_Lane_st_len        (sizeof(DF_Lane_st))

/*DF_LaneList structure*/
typedef struct _DF_LaneList_st
{
    uint8_t          LaneList_size;
    DF_Lane_st   laneList_data[32];
}DF_LaneList_st, * DF_LaneList_st_ptr;

#define DF_LaneList_st_len        (sizeof(DF_LaneList_st))

/*DF_Movement_opt structure */
typedef struct _DF_Movement_opt_st
{
    uint8_t Movement_opt_phaseid           :1;
    uint8_t Movement_opt_reserved          :7;
}DF_Movement_opt_st, * DF_Movement_opt_st_ptr;

/*DF_Movement structure */
typedef struct _DF_Movement_st
{
    /* optional */
    DF_Movement_opt_st                         opt;

    /* Data */
    DF_NodeReferenceID_st       remoteIntersection;
    DE_PhaseID_t                           phaseid;
}DF_Movement_st, *DF_Movement_st_ptr;

/*DF_MovementList structure */
typedef struct _DF_MovementList_st
{
    uint8_t                MovementList_size;
    DF_Movement_st     MovementList_data[32];
}DF_MovementList_st, * DF_MovementList_st_ptr;

#define    DF_MovementList_st_len        (sizeof(DF_MovementList_st))

/*DF_Link_opt_st */
typedef struct _DF_Link_opt_st
{
    uint8_t     Link_opt_name               :1;
    uint8_t     Link_opt_speedLimits        :1;
    uint8_t     Link_opt_points             :1;
    uint8_t     Link_opt_movements          :1;
    uint8_t     Link_opt_reserved           :4;
}DF_Link_opt_st, *DF_Link_opt_st_ptr;

/*DF_Link structure */
typedef struct _DF_Link_st
{
    /* Optional */
    DF_Link_opt_st                      opt;

    /* Data */
    DE_DescriptiveName_st              name;
    DF_NodeReferenceID_st     upstreamNodeId;
    DF_SpeedLimitList_st        speedLimits;
    DE_LaneWidth_t                laneWidth;
    DF_PointList_st                  points;
    DF_MovementList_st            movements;
    DF_LaneList_st                    lanes;
}DF_Link_st, *DF_Link_st_ptr;

#define DF_Link_st_len        (sizeof(DF_Link_st))

/*DF_LinkList structure */
typedef struct _DF_LinkList_st
{
    uint8_t         LinkList_size;
    DF_Link_st  LinkList_data[32];
}DF_LinkList_st, *DF_LinkList_st_ptr;

#define    DF_LinkList_st_len        (sizeof(DF_LinkList_st))


/*DE_MotionConfidenceSet_opt structure */
typedef struct _DF_MotionConfidenceSet_opt_st
{
    uint8_t    MotionConfidenceSet_opt_speedcfd                        :1;
    uint8_t    MotionConfidenceSet_opt_headingcfd                      :1;
    uint8_t    MotionConfidenceSet_opt_steercfd                        :1;
    uint8_t    MotionConfidenceSet_opt_reserved                        :5;
}DF_MotionConfidenceSet_opt_st, *DF_MotionConfidenceSet_opt_st_ptr;

#define DF_MotionConfidenceSet_opt_st_len    sizeof(DF_MotionConfidenceSet_opt_st)

/*DF_MotionConfidenceSet strucuture */
typedef struct _DF_MotionConfidenceSet_st
{
    /* Optional */
    DF_MotionConfidenceSet_opt_st             opt;
    
    /* Data */
    DE_SpeedConfidence_en                speedcfd;
    DE_HeadingConfidence_en            headingcfd;
    DE_SteeringWheelAngleConfidence_en   steercfd;

}DF_MotionConfidenceSet_st, *DF_MotionConfidenceSet_st_ptr;

/*DF_Position3D optional structure. */
typedef struct _DF_Position3D_opt_st
{
    uint8_t Position3D_opt_elev                 :1;            
    uint8_t Position3D_optreserved              :7;
    
}DF_Position3D_opt_st, *DF_Position3D_opt_st_ptr;

/* DF_Position3D */
typedef struct _DF_Position3D_st
{
    /* Optional */
    DF_Position3D_opt_st                 opt; /* optional configure */
    
    /* Data */
    DE_Latitude_t                   latitude; /* Geographic latitude. Unit: degree. */
    DE_Longitude_t                 longitude; /* Geographic longitude. Unit: degree. */
    DE_Elevation_t                 elevation; /* Geographic position above or below the reference ellipsoid. Unit: meter. */
}DF_Position3D_st, *DF_Position3D_st_ptr;

/*DF_Node_opt_st */
typedef struct _DF_Node_opt_st 
{
    uint8_t    Node_opt_name            :1;
    uint8_t    Node_opt_inLinks         :1;
    uint8_t    Node_opt_reserved        :6;
    
}DF_Node_opt_st, *DF_Node_opt_st_ptr;

/*DF_Node structure */
typedef struct _DF_Node_st
{
    /*Optional*/
    DF_Node_opt_st                opt;

    /*Data*/
    DE_DescriptiveName_st        name;    
    DF_NodeReferenceID_st          id;
    DF_Position3D_st           refPos;
    DF_LinkList_st            inLinks;
}DF_Node_st, *DF_Node_st_ptr;

#define DF_Node_st_len        (sizeof(DF_Node_st))

/*DF_NodeList */
typedef struct _DF_NodeList_st
{
    uint8_t         NodeList_size;
    DF_Node_st  NodeList_data[16];
}DF_NodeList_st, *DF_NodeList_st_ptr;

#define DF_NodeList_st_len        (sizeof(DF_NodeList_st))


/*DF_VehicleSize_opt tructure */
typedef struct _DF_VehicleSize_opt_st
{
    uint8_t    vehiclesize_opt_height                      :1;
    uint8_t    vehiclesize_opt_reserverd                   :7;
    
}DF_VehicleSize_opt_st, *DF_VehicleSize_opt_st_ptr;

#define    DE_VehicleSize_opt_st_len        (sizeof(DE_VehicleSize_opt_st))

/* Vehicle size structure. */
typedef struct _DF_VehicleSize_st
{
    /* Optional */
    DF_VehicleSize_opt_st          opt;
    
    /* Data */
    /* Vehicle size unit: m. */
    DE_VehicleWidth_t            width;
    DE_VehicleLength_t          length;
    DE_VehicleHeight_t          height;
    
}DF_VehicleSize_st, * DF_VehicleSize_st_ptr;

#define DF_VehicleSize_st_len    (sizeof(DF_VehicleSize_st))


/* DF_VehicleSafetyExternsions optional. */
typedef struct _DF_VehicleClassification_st
{
    DE_BasicVehicleClass_en            classification;
}DF_VehicleClassification_st, *DF_VehicleClassification_st_ptr;

#define DF_VehicleClassification_st_len    (sizeof(DF_VehicleClassification_st));

/*DF_PositionConfidenceSet optional configuration. */
typedef struct _DF_MSG_PositionConfidenceSet_opt_st
{
    uint8_t    PositionConfidenceSet_opt_confidence_elev           :1;
    uint8_t PositionConfidenceSet_opt_reserved                  :7;
}DF_PositionConfidenceSet_opt_st, *DF_MSG_PositionConfidenceSet_opt_st_ptr;

/* DF_PositionConfidenceSet. */
typedef struct _DF_PositionConfidenceSet_st
{
    /* Optional */
    DF_PositionConfidenceSet_opt_st       opt; /* optional configure */
    /* Data */
    
    DE_PositionConfidence_en              pos; /* position confidence */
    DE_ElevationConfidence_en       elevation; /* elevation confidence */

}DF_PositionConfidenceSet_st, * DF_PositionConfidenceSet_st_ptr;

#define DF_PositionConfidenceSet_st_len    (sizeof(DF_PositionConfidenceSet_st))


/*DF_ParticipantData_opt structure */
typedef struct _DF_ParticipantData_opt_st
{
    uint8_t    id                  :1;
    uint8_t    plateNo             :1;
    uint8_t    transmission        :1;
    uint8_t    angle               :1;
    uint8_t    motioncfd           :1;
    uint8_t    accelset            :1;
    uint8_t    vehicleclass        :1;
    uint8_t    reserved            :1;
    
}DF_ParticipantData_opt_st, *DF_ParticipantData_opt_st_ptr;


/*DF_ParticipantData structure */
typedef struct _DF_ParticipantData_st
{
    /* Optional */
    DF_ParticipantData_opt_st                  opt;
    
    /* Data */
    DE_ParticipantType_en                  ptcType;
    DE_PTCID_t                               ptcId;  /* temporary ID set by RSU. 0 is RSU itself  1..65535 represent participants detected by RSU*/
    DE_SourceType_en                    sourceType;
    DE_TemporaryID_st                           id;
    DE_PlateNo_st                           plateNo;
    DE_DSecond_t                           secMark;
    DF_PositionOffsetLLV_st                    pos;
    DF_PositionConfidenceSet_st           accuracy;
    DE_TransmissionState_en           transmission;
    DE_Speed_t                               speed;
    DE_Heading_t                           heading;
    DE_SteeringWheelAngle_t                  angle;
    DF_MotionConfidenceSet_st            motioncfd;
    DF_AccelerationSet4Way_st             accelset;
    DF_VehicleSize_st                         size;
    DF_VehicleClassification_st       vehicleclass;
    
}DF_ParticipantData_st, *DF_ParticipantData_st_ptr;

#define DF_ParticipantData_st_len        (sizeof(DF_ParticipantData_st))


/*DF_ParticipantList structure */
typedef struct _DF_ParticipantList_st
{
    uint8_t                   pointNum;
    DF_ParticipantData_st    array[16];
    
}DF_ParticipantList_st, *DF_ParticipantList_st_ptr;

#define DF_ParticipantList_st_len            (sizeof(DF_ParticipantList_st))


/*DF_PathHistoryPoint optional. */
typedef struct _DF_PathHistoryPoint_opt_st
{
    uint8_t PathHistoryPoint_opt_speed       :1;
    uint8_t PathHistoryPoint_opt_posAccuracy :1;
    uint8_t PathHistoryPoint_opt_heading     :1;
    uint8_t    PathHistoryPoint_opt_reserved    :5;
    
}DF_PathHistoryPoint_opt_st, * DF_PathHistoryPoint_opt_st_ptr;


/*DF_PathHistoryPoint. */
typedef struct _DF_PathHistoryPoint_st
{
    /* Optional. */
    DF_PathHistoryPoint_opt_st          opt;
    
    DF_PositionOffsetLLV_st       llvOffset;
    DE_TimeOffset_t              timeoffset;
    DE_Speed_t                        speed; /* Speed over the reported period */
    DF_PositionConfidenceSet_st posaccuracy;
    DE_Heading_t                    heading;
}DF_PathHistoryPoint_st, * DF_PathHistoryPoint_st_ptr;

#define DF_PathHistoryPoint_st_len    (sizeof(DF_PathHistoryPoint_st))


/*DF_PathHistoryPointList. */
typedef struct DF_PathHistoryPointList_st
{
    uint8_t                         pointNum;
    DF_PathHistoryPoint_st         array[23];
}DF_PathHistoryPointList_st, * DF_PathHistoryPointList_st_ptr;

#define DF_PathHistoryPointList_st_len    (sizeof(DF_PathHistoryPointList_st))

/*DF_PathPrediction. */
typedef struct _DF_PathPrediction_st
{
    DE_RadiusOfCurvature_t           radiusOfCurve; /* Unit: m. */
    DE_Confidence_t                     confidence; /* Unit: percent. */
}DF_PathPrediction_st, * DF_PathPrediction_st_ptr;

#define DF_PathPrediction_st_len    (sizeof(DF_PathPrediction_st))


#define DF_PathPointList_pointNum_MIN    2
#define DF_PathPointList_pointNum_MAX    32

/*DF_PathPointList structure */
typedef struct _DF_PathPointList_st
{
    uint8_t                        pointNum;
    DF_PositionOffsetLLV_st    array[DF_PathPointList_pointNum_MAX];
    
}DF_PathPointList_st, *DF_PathPointList_st_ptr;

#define DF_PathPointList_st_len        (sizeof(DF_PathPointList_st))

/*DF_Position accuracy structure. */
typedef struct _DF_PositionAccuracy_st
{
    /* Radius of semi-major axis of an ellipsoid. Unit: meter. */
    float        semi_major_accu;

    /* Radius of semi-minor axis of an ellipsoid. Unit: meter. */
    float        semi_minor_accu;

    /* Angle of semi-major axis of an ellipsoid. Unit: degree. */
    float semi_major_orientation;

}DF_PositionalAccuracy_st, * DF_PositionalAccuracy_st_ptr;

#define DF_PositionAccuracy_st_len    (sizeof(DF_PositionalAccuracy_st))

/*DF_RTCMheader. */
typedef struct _DF_RTCMheader_st
{
    DE_GNSSstatus_st          status;
    DF_AntennaOffsetSet_st offsetSet;

}DF_RTCMheader_st, * DF_RTCMheader_st_ptr;

#define DF_RTCMheader_st_len    (sizeof(DF_RTCMheader_st))


/*DF_RTCMmessageList. */
typedef struct DF_RTCMmessageList_st
{
    uint8_t         messageNum;
    DE_RTCMmessage_st array[5];

}DF_RTCMmessageList_st, * DF_RTCMmessageList_st_ptr;

#define DF_RTCMmessageList_st_len    (sizeof(DF_RTCMmessageList_st))

/*DF_SpecialVehicleExtensions optional. */
typedef struct _DF_SpecialVehicleExtensions_opt_st
{
    uint8_t vehicleAlerts  :1;
    uint8_t description    :1;
    uint8_t trailers       :1;
    
    uint8_t reserved       :5;
    
}DF_SpecialVehicleExtensions_opt_st, * DF_SpecialVehicleExtensions_opt_st_ptr;

/*DF_SpecialVehicleExtensions. */
typedef struct _DF_SpecialVehicleExtensions_st
{
    /* Optional. */
    DF_SpecialVehicleExtensions_opt_st opt;
    
    /* Data. */
    
    
}DF_SpecialVehicleExtensions_st, * DF_SpecialVehicleExtensions_st_ptr;

#define DF_SpecialVehicleExtensions_st_len    (sizeof(DF_SpecialVehicleExtensions_st))

/*DF_SpeedandHeadingandThrottleConfidence. */
typedef struct _DF_SpeedHeadingThrottleConf_st
{
    DE_HeadingConfidence_en   heading;
    DE_SpeedConfidence_en       speed;
    DE_ThrottleConfidence_en throttle;

}DF_SpeedHeadingThrottleConf_st, * DF_SpeedHeadingThrottleConf_st_ptr;

#define DF_SpeedHeadingThrottleConf_st_len    (sizeof(DF_SpeedHeadingThrottleConf_st))

/*DF_SupplementalVehicleExtensions optional. */
typedef struct _DF_SupplementalVehicleExtensions_opt_st
{
    uint8_t vehicleAlerts  :1;
    uint8_t description    :1;
    uint8_t trailers       :1;
    uint8_t reserved       :5;
    
}DF_SupplementalVehicleExtensions_opt_st, * DF_SupplementalVehicleExtensions_opt_st_ptr;

/*DF_SupplementalVehicleExtensions. */
typedef struct _DF_SupplementalVehicleExtensions_st
{
    /* Optional. */
    DF_SupplementalVehicleExtensions_opt_st opt;
    
    /* Data. */
    
    
}DF_SupplementalVehicleExtensions_st, * DF_SupplementalVehicleExtensions_st_ptr;

#define DF_SupplementalVehicleExtensions_st_len    (sizeof(DF_SupplementalVehicleExtensions_st))

/*DF_TransmissionAndSpeed. */
typedef struct _DF_TransmissionAndSpeed_st
{
    DE_TransmissionState_en transmission;
    float                          speed;  /* Driving speed. Unit km/h. */
}DF_TransmissionAndSpeed_st, * DF_TransmissionAndSpeed_st_ptr;

#define DF_TransmissionAndSpeed_st_len    (sizeof(DF_TransmissionAndSpeed_st))

/*DF_FullPositionVector optional configuration. */
typedef struct _DF_FullPositionVector_opt_st
{
    uint8_t FullPositionVector_opt_DDateTime              :1;
    uint8_t FullPositionVector_opt_Heading                :1;
    uint8_t FullPositionVector_opt_Transmission           :1;    
    uint8_t    FullPositionVector_opt_Speed                  :1;
    uint8_t FullPositionVector_opt_posAccuracy            :1;
    uint8_t FullPositionVector_opt_timeConfidence         :1;
    uint8_t FullPositionVector_opt_posConfidence          :1;
    uint8_t FullPositionVector_opt_motioncfd              :1;
}DF_FullPositionVector_opt_st, *DF_FullPositionVector_opt_st_ptr;

/*DF_FullPositionVector. */
typedef struct _DF_FullPositionVector_st
{
    /* Optional. */
    DF_FullPositionVector_opt_st          opt;

    /* Data. */
    DF_DDateTime_st                   utcTime;
    DF_Position3D_st                      pos;
    DE_Heading_t                      heading; /* Driving direction. Unit degree. */
    DE_TransmissionState_en      transmission;
    DE_Speed_t                          speed;
    DF_PositionConfidenceSet_st   posAccuracy;
    DE_TimeConfidence_en             timeConf;
    DF_PositionConfidenceSet_st posConfidence;
    DF_MotionConfidenceSet_st       motioncfd;
}DF_FullPositionVector_st, * DF_FullPositionVector_st_ptr;

#define DF_FullPositionVector_st_len    (sizeof(DF_FullPositionVector_st))


/*DF_PathHistory optional. */
typedef struct _DF_PathHistory_opt_st
{
    uint8_t PathHistory_opt_initialPosition       :1;
    uint8_t PathHistory_opt_currGNSSstatus        :1;
    uint8_t PathHistory_opt_reserved              :6;
    
}DF_PathHistory_opt_st, * DF_PathHistory_opt_st_ptr;

#define DE_PathHistory_opt_st_len    (sizeof(DE_PathHistory_opt_st))


/*DF_PathHistory. */
typedef struct _DF_PathHistory_st
{
    /* Optional. */
    DF_PathHistory_opt_st                         opt;

    /* Data. */
    DF_FullPositionVector_st          initialPosition;
    DE_GNSSstatus_st                   currGNSSstatus;
    DF_PathHistoryPointList_st              crumbData;
    
}DF_PathHistory_st, * DF_PathHistory_st_ptr;

#define DF_PathHistory_st_len    (sizeof(DF_PathHistory_st))

/*DF_VehicleSafetyExternsions optional structure. */
typedef struct _DF_VehicleSafetyExternsions_opt_st
{
    uint8_t VehicleSafetyExternsions_opt_events                          :1;
    uint8_t VehicleSafetyExternsions_opt_pathHistory                     :1;
    uint8_t VehicleSafetyExternsions_opt_pathPrediction                  :1;
    uint8_t VehicleSafetyExternsions_opt_lights                          :1;
    uint8_t VehicleSafetyExternsions_opt_reserved                        :4;
}DF_VehicleSafetyExternsions_opt_st, * DF_VehicleSafetyExternsions_opt_st_ptr;

/*DF_VehicleSafetyExtensions. */
typedef struct _DF_VehicleSafetyExtensions_st
{
    /* Optinal. */
    DF_VehicleSafetyExternsions_opt_st      opt;

    /* Data. */
    DE_VehicleEventFlags_un              events;
    DF_PathHistory_st               pathHistory;
    DF_PathPrediction_st         pathPrediction;
    DE_ExteriorLights_un                 lights;
    
}DF_VehicleSafetyExtensions_st, * DF_VehicleSafetyExtensions_st_ptr;

#define DF_VehicleSafetyExtensions_st_len    (sizeof(DF_VehicleSafetyExtensions_st))

/*DF_BSMcoreData structure*/
typedef struct _DF_BSMcoreData_st
{


}DF_BSMcoreData_st, *DF_BSMcoreData_st_ptr;

#define DF_BSMcoreData_st_len               (sizeof(DF_BSMcoreData_st))

/*DF_BSMpartIIExtension optional. */
typedef struct _DF_BSMpartIIExtension_opt_st
{
    uint8_t vecSafetyExt       :1;
    uint8_t specialVecExt      :1;
    uint8_t supplementalVecExt :1;

    uint8_t reserved           :5;
    
}DF_BSMpartIIExtension_opt_st, * DF_BSMpartIIExtension_opt_st_ptr;

/* DF_BSMpartIIExtension. */
typedef struct _DF_BSMpartIIExtension_st
{
    /* Optional. */
    DF_BSMpartIIExtension_opt_st                       opt;
    
    /* Data. */
    DF_VehicleSafetyExtensions_st             vecSafetyExt;
    DF_SpecialVehicleExtensions_st           specialVecExt;
    DF_SupplementalVehicleExtensions_st supplementalVecExt;
    
}DF_BSMpartIIExtension_st, * DF_BSMpartIIExtension_st_ptr;

#define DF_BSMpartIIExtension_st_len (sizeof(DF_BSMpartIIExtension_st))

/* . */
typedef struct _DF_BSMRegionalExtension_st
{
    
    
}DF_BSMRegionalExtension_st, * DF_BSMRegionalExtension_st_ptr;

#if 0
/* Free routine for DF_AntennaOffsetSet. */
extern int DF_AntennaOffsetSet_free(AntennaOffsetSet_t *offset_ptr);

/* Allocate routine for DF_AntennaOffsetSet. */
extern int DF_AntennaOffsetSet_allocate(AntennaOffsetSet_t *offset_ptr, DF_AntennaOffsetSet_st_ptr Offset_ptr);

/* Parse routine for DF_AntennaOffsetSet. */
extern int DF_AntennaOffsetSet_parse(AntennaOffsetSet_t *offset_ptr, DF_AntennaOffsetSet_st_ptr Offset_ptr);

/* Free routine for DF_BSMcoreData. */
extern int DF_BSMcoreData_free(BSMcoreData_t *coredata_ptr);

/* Allocate routine for DF_BSMcoreData. */
extern int DF_BSMcoreData_allocate(BSMcoreData_t *coredata_ptr, DF_BSMcoreData_st_ptr coreData_ptr);

/* Parse routine for DF_BSMcoreData. */
extern int DF_BSMcoreData_parse(BSMcoreData_t *coredata_ptr, DF_BSMcoreData_st_ptr coreData_ptr);
#endif

/* Free routine for DF_AccelerationSet4Way. */
extern int DF_AccelerationSet4Way_free(AccelerationSet4Way_t *accset_ptr);

/* Allocate routine for DF_AccelerationSet4Way. */
extern int DF_AccelerationSet4Way_allocate(AccelerationSet4Way_t *accset_ptr, DF_AccelerationSet4Way_st_ptr Accset_ptr);

/* Parse routine for DF_AccelerationSet4Way. */
extern int DF_AccelerationSet4Way_parse(AccelerationSet4Way_t *accset_ptr, DF_AccelerationSet4Way_st_ptr Accset_ptr);

/* Free routine for DF_BrakeSystemStatus. */
extern int DF_BrakeSystemStatus_free(BrakeSystemStatus_t *brakes_ptr);

/* Allocate routine for DF_BrakeSystemStatus. */
extern int DF_BrakeSystemStatus_allocate(BrakeSystemStatus_t *brakes_ptr, DF_BrakeSystemStatus_st_ptr Brakes_ptr);

/* parse routine for DF_BrakeSystemStatus. */
extern int DF_BrakeSystemStatus_parse(BrakeSystemStatus_t *brakes_ptr, DF_BrakeSystemStatus_st_ptr Brakes_ptr);

/* Free routine for DF_DDateTime. */
extern int DF_DDateTime_free(DDateTime_t *time_ptr);

/* Allocate routine for DF_DDateTime. */
extern int DF_DDateTime_allocate(DDateTime_t *time_ptr, DF_DDateTime_st_ptr DDate_ptr);

/* Parse routine for DF_DDateTime. */
extern int DF_DDateTime_parse(DDateTime_t *time_ptr, DF_DDateTime_st_ptr DDate_ptr);

/* Free routine for FullPositionVector. */
extern int DF_FullPositionVector_free(FullPositionVector_t *pos_ptr);

/* Allocate routine for DF_FullPositionVector. */
extern int DF_FullPositionVector_allocate(FullPositionVector_t *pos_ptr, DF_FullPositionVector_st_ptr fullPos_ptr);

/* Parse routine for DF_FullPositionVector. */
extern int DF_FullPositionVector_parse(FullPositionVector_t *pos_ptr, DF_FullPositionVector_st_ptr fullPos_ptr);

/* Free routine for DF_IntersectionStateList. */
extern int DF_IntersectionStateList_free(IntersectionStateList_t *list_ptr);

/* Allocate routine for DF_IntersectionStateList. */
extern int DF_IntersectionStateList_allocate(IntersectionStateList_t *list_ptr, DF_IntersectionStateList_st_ptr List_ptr);

/* Parse routine for DF_IntersectionStateList. */
extern int DF_IntersectionStateList_parse(IntersectionStateList_t *list_ptr, DF_IntersectionStateList_st_ptr List_ptr);

/* Free routine for DF_IntersectionState. */
extern int DF_IntersectionState_free(IntersectionState_t *iss_ptr);

/* Free extention routine for DF_IntersectionState. Diff with DF_IntersectionState_free() and this will free the point itself. */
extern int DF_IntersectionState_freeExt(IntersectionState_t *iss_ptr);

/* Allocate routine for DF_IntersectionState. */
extern int DF_IntersectionState_allocate(IntersectionState_t *iss_ptr, DF_IntersectionState_st_ptr Iss_ptr);

/* Parse routine for DF_IntersectionState. */
extern int DF_IntersectionState_parse(IntersectionState_t *iss_ptr, DF_IntersectionState_st_ptr Iss_ptr);

/* Free routine for DF_MotionConfidenceSet. */
extern int DF_MotionConfidenceSet_free(struct MotionConfidenceSet *motion_ptr);

/* Allocate routine for DF_MotionConfidenceSet.*/
extern int DF_MotionConfidenceSet_Allocate(struct MotionConfidenceSet *motion_ptr, DF_MotionConfidenceSet_st_ptr Motion_ptr);

/* Parse routine for DF_MotionConfidenceSet. */
extern int DF_MotionConfidenceSet_parse(struct MotionConfidenceSet *motion_ptr, DF_MotionConfidenceSet_st_ptr Motion_ptr);

/* Free routine for DF_MovementList. */
extern int DF_MovementList_free(MovementList_t *list_ptr);

/* Allocate routine for DF_MovementList. */
extern int DF_MovementList_allocate(MovementList_t *list_ptr, DF_MovementList_st_ptr List_ptr);

/* Parse routine for DF_MovementList. */
extern int DF_MovementList_parse(MovementList_t *list_ptr, DF_MovementList_st_ptr List_ptr);

/* Free routine for DF_NodeReferenceID. */
extern int DF_NodeReferenceID_free(NodeReferenceID_t *id_ptr);

/* Allocate routine for DF_NodeReferenceID.*/
extern int DF_NodeReferenceID_allocate(NodeReferenceID_t *id_ptr, DF_NodeReferenceID_st_ptr ID_ptr);

/* Parse routine for DF_NodeReferenceID. */
extern int DF_NodeReferenceID_parse(NodeReferenceID_t *id_ptr, DF_NodeReferenceID_st_ptr ID_ptr);

/* Free routine for DF_NodeList. */
extern int DF_NodeList_free(NodeList_t *list_ptr);

/* Allocate routine for DF_NodeList. */
extern int DF_NodeList_allocate(NodeList_t *list_ptr, DF_NodeList_st_ptr List_ptr);

/* Parse routine for DF_NodeList. */
extern int DF_NodeList_parse(NodeList_t *list_ptr, DF_NodeList_st_ptr List_ptr);

/* Free routine for DF_ParticipantData. */
extern int DF_ParticipantData_free(ParticipantData_t *ptc_ptr);

/* Free extention routine for DF_ParticipantData. Diff with DF_ParticipantData_free() and this will free the point itself. */
extern int DF_ParticipantData_freeExt(ParticipantData_t *ptc_ptr);

/* Allocate routine for DF_ParticipantData. */
extern int DF_ParticipantData_allocate(ParticipantData_t *ptc_ptr, DF_ParticipantData_st_ptr Ptc_ptr);

/* Parse routine for DF_ParticipantData. */
extern int DF_ParticipantData_parse(ParticipantData_t *ptc_ptr, DF_ParticipantData_st_ptr Ptc_ptr);

/* Free routine for DF_ParticipantList. */
extern int DF_ParticipantList_free(ParticipantList_t *list_ptr);

/* Allocate routine for DF_ParticipantList. */
extern int DF_ParticipantList_allocate(ParticipantList_t *list_ptr, DF_ParticipantList_st_ptr List_ptr);

/* Parse routine for DF_ParticipantList. */
extern int DF_ParticipantList_parse(ParticipantList_t *list_ptr, DF_ParticipantList_st_ptr List_ptr);

/* Free routine for DF_PathHistory. */
extern int DF_PathHistory_free(PathHistory_t *path_ptr);

/* Allocate routine for DF_PathHistory. */
extern int DF_PathHistory_allocate(PathHistory_t *path_ptr, DF_PathHistory_st_ptr Path_ptr);

/* Parse bsm partii element vehicleSafetyExt. */
extern int DF_PathHistory_parse(PathHistory_t *path_ptr, DF_PathHistory_st_ptr Path_ptr);

/* Free routine for DF_PathHistoryPointList. */
extern int DF_PathHistoryPointList_free(PathHistoryPointList_t *list_ptr);

/* Allocate routine for DF_PathHistoryPointList. */
extern int DF_PathHistoryPointList_allocate(PathHistoryPointList_t *list_ptr, DF_PathHistoryPointList_st_ptr List_ptr);

/* Parse routine for DF_PathHistoryPointList. */
extern int DF_PathHistoryPointList_parse(PathHistoryPointList_t *list_ptr, DF_PathHistoryPointList_st_ptr List_ptr);

/* Free routine for DF_PathHistoryPoint. */
extern int DF_PathHistoryPoint_free(PathHistoryPoint_t *point_ptr);

/* Free extention routine for DF_PathHistoryPoint. diff with DF_PathHistoryPoint_free() and this will free the point itself. */
extern int DF_PathHistoryPoint_freeExt(PathHistoryPoint_t *point_ptr);

/* Allocate routine for DF_PathHistoryPoint. */
extern int DF_PathHistoryPoint_allocate(PathHistoryPoint_t *point_ptr, DF_PathHistoryPoint_st_ptr Point_ptr);

/* Parse routine for DF_PathHistoryPoint. */
extern int DF_PathHistoryPoint_parse(PathHistoryPoint_t *point_ptr, DF_PathHistoryPoint_st_ptr Point_ptr);

/* Free routine for DF_PathPointList. */
extern int DF_PathPointList_free(PathPointList_t *list_ptr);

/* Allocate routine for DF_PathPointList. */
extern int DF_PathPointList_allocate(PathPointList_t *list_ptr, DF_PathPointList_st_ptr List_ptr);

/* Parse routine for DF_PathPointList. */
extern int DF_PathPointList_parse(PathPointList_t *list_ptr, DF_PathPointList_st_ptr List_ptr);

/* Free routine for DF_PathPrediction. */
extern int DF_PathPrediction_free(PathPrediction_t *path_ptr);

/* Allocate routine for DF_PathPrediction. */
extern int DF_PathPrediction_allocate(PathPrediction_t *path_ptr, DF_PathPrediction_st_ptr Path_ptr);

/* Parse routine for DF_PathPrediction. */
extern int DF_PathPrediction_parse(PathPrediction_t *path_ptr, DF_PathPrediction_st_ptr Path_ptr);

/* Free routine for DF_PhaseList. */
extern int DF_PhaseList_free(PhaseList_t *list_ptr);

/* Allocate routine for DF_PhaseList. */
extern int DF_PhaseList_allocate(PhaseList_t *list_ptr, DF_PhaseList_st_ptr List_ptr);

/* Parse routine for DF_PhaseList. */
extern int DF_PhaseList_parse(PhaseList_t *list_ptr, DF_PhaseList_st_ptr List_ptr);

/* Free routine for DF_PhaseState. */
extern int DF_PhaseState_free(PhaseState_t *ps_ptr);

/* Free extention routine for DF_PhaseState. Diff with DF_PhaseState_free() and this will free the point itself. */
extern int DF_PhaseState_freeExt(PhaseState_t *ps_ptr);

/* Allocate routine for DF_PhaseState.*/
extern int DF_PhaseState_allocate(PhaseState_t *ps_ptr, DF_PhaseState_st_ptr Ps_ptr);

/* Parse routine for DF_PhaseState. */
extern int DF_PhaseState_parse(PhaseState_t *ps_ptr, DF_PhaseState_st_ptr Ps_ptr);

/* Free routine for DF_PhaseStateList. */
extern int DF_PhaseStateList_free(PhaseStateList_t *list_ptr);

/* Allocate routine for DF_PhaseStateList. */
extern int DF_PhaseStateList_allocate(PhaseStateList_t *list_ptr, DF_PhaseStateList_st_ptr List_ptr);

/* Parse routine for DF_PhaseStateList. */
extern int DF_PhaseStateList_parse(PhaseStateList_t *list_ptr, DF_PhaseStateList_st_ptr List_ptr);

/* Free routine for DF_Phase. */
extern int DF_Phase_free(Phase_t *phase_ptr);

/* Free extention routine for DF_Phase. Diff with DF_Phase_free() and this will free the point itself. */
extern int DF_Phase_freeExt(Phase_t *phase_ptr);

/* Allocate routine for DF_Phase. */
extern int DF_Phase_allocate(Phase_t *phase_ptr, DF_Phase_st_ptr Phase_ptr);

/* Parse routine for DF_Phase. */
extern int DF_Phase_parse(Phase_t *phase_ptr, DF_Phase_st_ptr Phase_ptr);

/* allocate routine for DF_Position_LL_24B. */
extern int DF_Position_LL_24B_allocate(Position_LL_24B_t *pos_ll_24b_ptr, DF_Position_LL_24B_st_ptr Pos_LL_24B_ptr);

/* parse routine for DF_Position_LL_24B. */
extern int DF_Position_LL_24B_parse(Position_LL_24B_t *pos_ll_24b_ptr, DF_Position_LL_24B_st_ptr Pos_LL_24B_ptr);

/* allocate routine for DF_Position_LL_28B. */
extern int DF_Position_LL_28B_allocate(Position_LL_28B_t *pos_ll_28b_ptr, DF_Position_LL_28B_st_ptr Pos_LL_28B_ptr);

/* parse routine for DF_Position_LL_28B. */
extern int DF_Position_LL_28B_parse(Position_LL_28B_t *pos_ll_28b_ptr, DF_Position_LL_28B_st_ptr Pos_LL_28B_ptr);

/* allocate routine for DF_Position_LL_32B. */
extern int DF_Position_LL_32B_allocate(Position_LL_32B_t *pos_ll_32b_ptr, DF_Position_LL_32B_st_ptr Pos_LL_32B_ptr);

/* parse routine for DF_Position_LL_32B. */
extern int DF_Position_LL_32B_parse(Position_LL_32B_t *pos_ll_32b_ptr, DF_Position_LL_32B_st_ptr Pos_LL_32B_ptr);

/* allocate routine for DF_Position_LL_36B. */
extern int DF_Position_LL_36B_allocate(Position_LL_36B_t *pos_ll_36b_ptr, DF_Position_LL_36B_st_ptr Pos_LL_36B_ptr);

/* parse routine for DF_Position_LL_36B. */
extern int DF_Position_LL_36B_parse(Position_LL_36B_t *pos_ll_36b_ptr, DF_Position_LL_36B_st_ptr Pos_LL_36B_ptr);

/* allocate routine for DF_Position_LL_44B. */
extern int DF_Position_LL_44B_allocate(Position_LL_44B_t *pos_ll_44b_ptr, DF_Position_LL_44B_st_ptr Pos_LL_44B_ptr);

/* parse routine for DF_Position_LL_44B. */
extern int DF_Position_LL_44B_parse(Position_LL_44B_t *pos_ll_44b_ptr, DF_Position_LL_44B_st_ptr Pos_LL_44B_ptr);

/* allocate routine for DF_Position_LL_48B. */
extern int DF_Position_LL_48B_allocate(Position_LL_48B_t *pos_ll_48b_ptr, DF_Position_LL_48B_st_ptr Pos_LL_48B_ptr);

/* parse routine for DF_Position_LL_48B. */
extern int DF_Position_LL_48B_parse(Position_LL_48B_t *pos_ll_48b_ptr, DF_Position_LL_48B_st_ptr Pos_LL_48B_ptr);

/* allocate routine for DF_Position_LLmD_64B. */
extern int DF_Position_LLmD_64B_allocate(Position_LLmD_64b_t *pos_ll_64b_ptr, DF_Position_LLmD_64b_st_ptr Pos_LL_64B_ptr);

/* parse routine for DF_Position_LL_64B. */
extern int DF_Position_LL_64B_parse(Position_LLmD_64b_t *pos_ll_64b_ptr, DF_Position_LLmD_64b_st_ptr Pos_LL_64B_ptr);

#if 0
/* Free routine for DF_PositionalAccuracy. */
extern int DF_PositionalAccuracy_free(PositionalAccuracy_t *pos_ptr);

/* Allocate routine for DF_PositionalAccuracy. */
extern int DF_PositionalAccuracy_allocate(PositionalAccuracy_t *pos_ptr, DF_PositionalAccuracy_st_ptr Pos_ptr);

/* Parse routine for DF_PositionalAccuracy. */
extern int DF_PositionalAccuracy_parse(PositionalAccuracy_t *pos_ptr, DF_PositionalAccuracy_st_ptr Pos_ptr);
#endif
/* Free routine for DF_Position3D. */
extern int DF_Position3D_free(Position3D_t *pos_ptr);

/* Allocate routine for DF_Position3D. */
extern int DF_Position3D_allocate(Position3D_t *pos_ptr, DF_Position3D_st_ptr Pos_ptr);

/* Parse routine for DF_Position3D. */
extern int DF_Position3D_parse(Position3D_t *pos_ptr, DF_Position3D_st_ptr Pos_ptr);

/* Free routine for DF_PositionConfidenceSet. */
extern int DF_PositionConfidenceSet_free(PositionConfidenceSet_t *pos_ptr);

/* Allocate routine for DF_PositionConfidenceSet. */
extern int DF_PositionConfidenceSet_allocate(PositionConfidenceSet_t *pos_ptr, DF_PositionConfidenceSet_st_ptr Pos_ptr);

/* Parse routine for DF_PositionConfidenceSet. */
extern int DF_PositionConfidenceSet_parse(PositionConfidenceSet_t *pos_ptr, DF_PositionConfidenceSet_st_ptr Pos_ptr);

/* Free extention routine for DF_PositionOffsetLLV. Diff with DF_PositionOffsetLLV_free() and this will free the point itself. */
extern int DF_PositionOffsetLLV_freeExt(PositionOffsetLLV_t *posoffsetllv_ptr);

/* Free routine for DF_PositionOffsetLLV*/
extern int DF_PositionOffsetLLV_free(PositionOffsetLLV_t *posoffsetllv_ptr);

/* Allocate routine for DF_PositionOffsetLLV. */
extern int DF_PositionOffsetLLV_allocate(PositionOffsetLLV_t *posoffsetllv_ptr, DF_PositionOffsetLLV_st_ptr PosOffsetLLV_ptr);

/* Parse routine for DF_PositionOffsetLLV. */
extern int DF_PositionOffsetLLV_parse(PositionOffsetLLV_t *posoffsetllv_ptr, DF_PositionOffsetLLV_st_ptr PosOffsetLLV_ptr);

/* Free routine for DF_PointList. */
extern int DF_PointList_free(PointList_t *list_ptr);

/* Allocate routine for DF_PointList. */
extern int DF_PointList_allocate(PointList_t *list_ptr, DF_PointList_st_ptr List_ptr);

/* Parse routine for DF_PointList. */
extern int DF_PointList_parse(PointList_t *list_ptr, DF_PointList_st_ptr List_ptr);

/* Free routine for DF_RoadPoint*/
extern int DF_RoadPoint_free(RoadPoint_t *roadpoint_ptr);

/* Free extention routine for DF_RoadPoint. Diff with DF_RoadPoint_free() and this will free the point itself. */
extern int DF_RoadPoint_freeExt(RoadPoint_t *roadpoint_ptr);

/* Allocate routine for DF_RoadPoint. */
extern int DF_RoadPoint_allocate(RoadPoint_t *roadpoint_ptr, DF_RoadPoint_st_ptr Roadpoint_ptr);

/* Parse routine for DF_RoadPoint. */
extern int DF_RoadPoint_parse(RoadPoint_t *roadpoint_ptr, DF_RoadPoint_st_ptr Roadpoint_ptr);

/* Free routine for DF_SpeedLimitList. */
extern int DF_SpeedLimitList_free(SpeedLimitList_t *list_ptr);

/* Allocate routine for DF_SpeedLimitList. */
extern int DF_SpeedLimitList_allocate(SpeedLimitList_t *list_ptr, DF_SpeedLimitList_st_ptr List_ptr);

/* Parse routine for DF_SpeedLimitList. */
extern int DF_SpeedLimitList_parse(SpeedLimitList_t *list_ptr, DF_SpeedLimitList_st_ptr List_ptr);

#if 0
/* Free routine for DF_RTCMheader. */
extern int DF_RTCMheader_free(RTCMheader_t *header_ptr);

/* Allocate routine for DF_RTCMheader. */
extern int DF_RTCMheader_allocate(RTCMheader_t *header_ptr, DF_RTCMheader_st_ptr Header_ptr);

/* Parse routine for DF_RTCMheader. */
extern int DF_RTCMheader_parse(RTCMheader_t *header_ptr, DF_RTCMheader_st_ptr Header_ptr);

/* Free routine for DF_RTCMmessageList. */
extern int DF_RTCMmessageList_free(RTCMmessageList_t *list_ptr);

/* Allocate routine for DF_RTCMmessageList. */
extern int DF_RTCMmessageList_allocate(RTCMmessageList_t *list_ptr, DF_RTCMmessageList_st_ptr List_ptr);

/* Parse routine for DF_RTCMmessageList. */
extern int DF_RTCMmessageList_parse(RTCMmessageList_t *list_ptr, DF_RTCMmessageList_st_ptr List_ptr);

/* Free routine for DF_SpecialVehicleExtensions. */
extern int DF_SpecialVehicleExtensions_free(SpecialVehicleExtensions_t * vecext_ptr);

/* Allocate routine for DF_SpecialVehicleExtensions. */
extern int DF_SpecialVehicleExtensions_allocate(SpecialVehicleExtensions_t * vecext_ptr, DF_SpecialVehicleExtensions_st_ptr vecExt_ptr);

/* Parse routine for DF_SpecialVehicleExtensions. */
extern int DF_SpecialVehicleExtensions_parse(SpecialVehicleExtensions_t * vecext_ptr, DF_SpecialVehicleExtensions_st_ptr vecExt_ptr);


/* Free routine for DF_SpeedHeadingThrottleConfidence. */
extern int DF_SpeedHeadingThrottleConfidence_free(SpeedandHeadingandThrottleConfidence_t *conf_ptr);

/* Allocate routine for DF_SpeedHeadingThrottleConfidence. */
extern int DF_SpeedHeadingThrottleConfidence_allocate(SpeedandHeadingandThrottleConfidence_t *conf_ptr, DF_SpeedHeadingThrottleConf_st_ptr Conf_ptr);

/* Parse routine for DF_SpeedHeadingThrottleConfidence. */
extern int DF_SpeedHeadingThrottleConfidence_parse(SpeedandHeadingandThrottleConfidence_t *conf_ptr, DF_SpeedHeadingThrottleConf_st_ptr Conf_ptr);

/* Free routine for DF_SupplementalVehicleExtensions. */
extern int DF_SupplementalVehicleExtensions_free(VehicleDummyStruct_t * vecext_ptr);

/* allocate routine for DF_SupplementalVehicleExtensions. */
extern int DF_SupplementalVehicleExtensions_allocate(VehicleDummyStruct_t * vecext_ptr, DF_SupplementalVehicleExtensions_st_ptr vecExt_ptr);

/* Parse routine for DF_SupplementalVehicleExtensions. */
extern int DF_SupplementalVehicleExtensions_parse(VehicleDummyStruct_t * vecext_ptr, DF_SupplementalVehicleExtensions_st_ptr vecExt_ptr);

/* Free routine for DF_TransmissionAndSpeed. */
extern int DF_TransmissionAndSpeed_free(TransmissionAndSpeed_t *trans_ptr);

/* Allocate routine for DF_TransmissionAndSpeed. */
extern int DF_TransmissionAndSpeed_allocate(TransmissionAndSpeed_t *trans_ptr, DF_TransmissionAndSpeed_st_ptr Trans_ptr);

/* Parse routine for DF_TransmissionAndSpeed. */
extern int DF_TransmissionAndSpeed_parse(TransmissionAndSpeed_t *trans_ptr, DF_TransmissionAndSpeed_st_ptr Trans_ptr);
#endif

/* Free routine for DF_TimeChangeDetails. */
extern int DF_TimeChangeDetails_free(TimeChangeDetails_t *tcd_ptr);

/* Allocate routine for DF_TimeChangeDetails.*/
extern int DF_TimeChangeDetails_allocate(TimeChangeDetails_t *tcd_ptr, DF_TimeChangeDetials_st_ptr Tcd_ptr);

/* Parse routine for DF_TimeChangeDetails. */
extern int DF_TimeChangeDetails_parse(TimeChangeDetails_t *tcd_ptr, DF_TimeChangeDetials_st_ptr Tcd_ptr);

/* Free routine for DF_VehicleClassification. */
extern int DF_VehicleClassification_free(VehicleClassification_t* vehicleclass_ptr);

/* Allocate routine for DF_VehicleClassification. */
extern int DF_VehicleClassification_allocate(VehicleClassification_t* vehicleclass_ptr , DF_VehicleClassification_st_ptr VehicleClass_ptr);

/* Parse routine for DF_VehicleClassification. */
extern int DF_VehicleClassification_parse(VehicleClassification_t* vehicleclass_ptr , DF_VehicleClassification_st_ptr VehicleClass_ptr);

/* Free routine for DF_VehicleSafetyExtensions. */
extern int DF_VehicleSafetyExtensions_free(VehicleSafetyExtensions_t * vecsafety_ptr);

/* Allocate routine for DF_VehicleSafetyExtensions. */
extern int DF_VehicleSafetyExtensions_allocate(VehicleSafetyExtensions_t * vecsafety_ptr, DF_VehicleSafetyExtensions_st_ptr vecSafety_ptr);

/* Parse routine for DF_VehicleSafetyExtensions. */
extern int DF_VehicleSafetyExtensions_parse(VehicleSafetyExtensions_t * vecsafety_ptr, DF_VehicleSafetyExtensions_st_ptr vecSafety_ptr);

/* Free routine for DF_VehicleSize. */
extern int DF_VehicleSize_free(VehicleSize_t* vehiclesize_ptr);

/* Allocate routine for DF_VehicleSize. */
extern int DF_VehicleSize_allocate(VehicleSize_t* vehiclesize_ptr , DF_VehicleSize_st_ptr VehicleSize_ptr);

/* Parse routine for DF_VehicleSize. */
extern int DF_VehicleSize_parse(VehicleSize_t* vehiclesize_ptr , DF_VehicleSize_st_ptr VehicleSize_ptr);

/* Free routine for DF_VerticalOffset. */
extern int DF_VerticalOffset_free(VerticalOffset_t *vecoffset_ptr);

/* Allocate routine for DF_VerticalOffset. */
extern int DF_VerticalOffset_allocate(VerticalOffset_t *vecoffset_ptr, DF_VerticalOffset_st_ptr VecOffset_ptr);

/* Parse routine for DF_VerticalOffset. */
extern int DF_VerticalOffset_parse(VerticalOffset_t *vecoffset_ptr, DF_VerticalOffset_st_ptr VecOffset_ptr);


#endif /* __CV_VAM_DF_H__ */

