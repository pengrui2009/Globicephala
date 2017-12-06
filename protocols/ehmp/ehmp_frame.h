/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : ehmp_frame.h
 @brief  : ehmp protocol data frame parse api
 @author : pengrui
 @history:
           2017-11-23      wangxianwen    Created file
           ...
******************************************************************************/
#ifndef __EHMP_FRAME_H__
#define __EHMP_FRAME_H__

#include "prot_element.h"




#if 0




/* 本地车辆告警标识*/
typedef union __vehicle_alert_st{
    /* 本地车辆告警标识字*/
    uint32_t  vehicle_alert_words;
    /* 本地车辆告警标识位*/
    struct _vehicle_alert_bit{
    #ifndef ENDIAN_LITTLE

        uint32_t reserved             :28;

        /* 紧急刹车告警:00b(Invalid),01b(Off),10b(On),11b(Reserved)*/
        uint32_t vecbrakehardalert    :2;

        /* 车辆故障告警：00b(Invalid),01b(Off),10b(On),11b(Reserved)*/
        uint32_t    vecbreakdownalert :2;

      #else

        /* 车辆故障告警：00b(Invalid),01b(Off),10b(On),11b(Reserved)*/
        uint32_t    vecbreakdownalert :2;

        /* 紧急刹车告警:00b(Invalid),01b(Off),10b(On),11b(Reserved)*/
        uint32_t vecbrakehardalert    :2;

        uint32_t reserved             :28;

      #endif
    }vehicle_alert_bit;

}vehicle_alert_st;


#endif

/* df_AccelerationSet4Way. */
typedef struct _df_AccelerationSet4Way_st
{   
    /* Along the vehicle longitudinal axis. Unit: m/s^2. */
    DE_Acceleration_t              longitudinal;

    /* Along the vehicle lateral axis. Unit: m/s^2. */
    DE_Acceleration_t                   lateral;

    /* Alone the vehicle vertical axis. Unit: m/s^2. */
    DE_VerticalAcceleration_t          vertical;
    
    /* Vehicle's yaw rate. Unit: degrees per second. */
    DE_YawRate_t                       yaw_rate;
    
}df_AccelerationSet4Way_st, * df_AccelerationSet4Way_st_ptr;

#define df_AccelerationSet4Way_st_len    (sizeof(df_AccelerationSet4Way_st))


/*df_BrakeSystemStatus. */
typedef struct _df_BrakeSystemStatus_st
{
    DE_BrakePedalStatus_en      brakepedal;
    DE_BrakeAppliedStatus_st   wheelBrakes;
    DE_TractionControlStatus_en   traction;
    DE_AntiLockBrakeStatus_en          abs;
    DE_StabilityControlStatus_en       scs;
    DE_BrakeBoostApplied_en     brakeBoost;
    DE_AuxiliaryBrakeStatus_en   auxBrakes;
    
}df_BrakeSystemStatus_st, * df_BrakeSystemStatus_st_ptr;

#define df_BrakeSystemStatus_st_len    (sizeof(df_BrakeSystemStatus_st))


/* df_Position3D. */
typedef struct _df_Position3D_st
{    
    DE_Latitude_t        latitude; /* Geographic latitude. Unit: degree. */
    DE_Longitude_t      longitude; /* Geographic longitude. Unit: degree. */
    DE_Elevation_t      elevation; /* Geographic position above or below the reference ellipsoid. Unit: meter. */

}df_Position3D_st, *df_Position3D_st_ptr;


/* df_VehicleSize. */
typedef struct _df_VehicleSize_st
{
    /* Vehicle size unit: m. */
    DE_VehicleWidth_t            width;
    DE_VehicleLength_t          length;
    DE_VehicleHeight_t          height;
    
}df_VehicleSize_st, * df_VehicleSize_st_ptr;

#define df_VehicleSize_st_len    (sizeof(df_VehicleSize_st))


/* df_VehicleBasicInfor. */
typedef struct _df_VehicleBasicInfor_st
{   
    DE_TemporaryID_st                         id; /* Product id. */

    DE_LongitudinalDis_t                  lonDis; /* Distance in longitudinal. */
    DE_LatitudinalDis_t                   latDis; /* Distance in latitudinal. */
    DE_Heading_t                         heading; /* Driving direction based on the specific heading. Unit degree. */
    DE_Speed_t                             speed; /* Driving speed. Unit km/h. */

    DE_AlertFlag_un                        alert; /* Alart flag in current vehicle. */
    
}df_VehicleBasicInfor_st, * df_VehicleBasicInfor_st_ptr;

#define df_VehicleBasicInfor_st_len    (sizeof(df_VehicleBasicInfor_st))


/* df_VehicleFullInfor. */
typedef struct _df_VehicleFullInfor_st
{   
    DE_TemporaryID_st                         id; /* Product id. */
    
    df_Position3D_st                         pos; /* position */
    DE_TransmissionState_en                trans; /* Transmission state. */
    DE_Speed_t                             speed; /* Driving speed. Unit km/h. */
    DE_Heading_t                         heading; /* Driving direction. Unit degree. */
    DE_SteeringWheelAngle_t       steering_angle; /* Steering wheel angle. */
    df_AccelerationSet4Way_st    accelerationSet; /* Acceleration set in 4 way. */
    df_BrakeSystemStatus_st         brake_status; /* Brake system status. */
    DE_ExteriorLights_un              ext_lights; /* Exterior lights. */
    
    DE_AlertFlag_un                        alert; /* Alart flag in current vehicle. */
    
}df_VehicleFullInfor_st, * df_VehicleFullInfor_st_ptr;

#define df_VehicleFullInfor_st_len    (sizeof(df_VehicleFullInfor_st))


/* Save all the compiler settings. */
#pragma pack(push)
#pragma pack(1)


/*Acceleration set 4 way structure. */
typedef struct _AccelerationSet4Way_st
{   
    /* Along the vehicle longitudinal axis. unit 0.01 m/s^2,(-2000 - +2000),2001 means invalid. */
    int16_t     longitudinal;

    /* Along the vehicle lateral axis. unit 0.01 m/s^2,(-2000 - +2000),2001 means invalid. */
    int16_t          lateral;

    /* Alone the vehicle vertical axis. unit 0.02G(0.1962 m/s^2),(-127 - +127),-127 means invalid. */
    int8_t          vertical;
    
    /* Vehicle's yaw rate. unit 0.01 degree/s,(-32767 - +32767). */
    int16_t         yaw_rate;
    
}AccelerationSet4Way_st, * AccelerationSet4Way_st_ptr;

#define AccelerationSet4Way_st_len    (sizeof(AccelerationSet4Way_st))


/*BrakeSystemStatus. */
typedef struct _BrakeSystemStatus_st
{
    uint8_t                   brakepedal;
    DE_BrakeAppliedStatus_st wheelBrakes;
    uint8_t                     traction;
    uint8_t                          abs;
    uint8_t                          scs;
    uint8_t                   brakeBoost;
    uint8_t                    auxBrakes;
    
}BrakeSystemStatus_st, * BrakeSystemStatus_st_ptr;

#define BrakeSystemStatus_st_len    (sizeof(BrakeSystemStatus_st))


/* Position3D. */
typedef struct _Position3D_st
{    
    int32_t       latitude; /* unit 0.1 micro degree, (-900000000 - +900000001), 900000001 means invalid. */
    int32_t      longitude; /* unit 0.1 micro degree, (-1799999999 - 1800000001), 1800000001 means invalid. */
    int32_t      elevation; /* unit 0.1 meter, (-4096 - +61439), -4096 means invalid. */

}Position3D_st, *Position3D_st_ptr;

#define Position3D_st_len    (sizeof(Position3D_st))


/* VehicleSize. */
typedef struct _VehicleSize_st
{
    uint16_t          width;  /* Unit 1cm. */
    uint16_t         length;  /* Unit 1cm. */
    uint8_t          height;  /* Unit 5cm. */
    
}VehicleSize_st, * VehicleSize_st_ptr;

#define VehicleSize_st_len    (sizeof(VehicleSize_st))


/* df_VehicleBasicInfor. */
typedef struct _VehicleBasicInfor_st
{   
    DE_TemporaryID_st                         id; /* Product id. */

    uint16_t                              lonDis; /* Distance in longitudinal. */
    uint16_t                              latDis; /* Distance in latitudinal. */
    uint16_t                             heading; /* Driving direction based on the specific heading. Unit degree. */
    uint16_t                               speed; /* Driving speed. Unit km/h. */

    DE_AlertFlag_un                        alert; /* Alart flag in current vehicle. */
    
}VehicleBasicInfor_st, * VehicleBasicInfor_st_ptr;

#define VehicleBasicInfor_st_len    (sizeof(VehicleBasicInfor_st))


/* VehicleFullInfor. */
typedef struct _VehicleFullInfor_st
{   
    DE_TemporaryID_st                         id; /* Product id. */
    
    Position3D_st                            pos; /* position */
    uint8_t                                trans; /* Transmission state. */
    uint16_t                               speed; /* Driving speed. Unit km/h. */
    uint16_t                             heading; /* Driving direction. Unit degree. */
    uint8_t                       steering_angle; /* Steering wheel angle. */
    AccelerationSet4Way_st    accelerationSet; /* Acceleration set in 4 way. */
    BrakeSystemStatus_st         brake_status; /* Brake system status. */
    DE_ExteriorLights_un              ext_lights; /* Exterior lights. */
    
    DE_AlertFlag_un                        alert; /* Alart flag in current vehicle. */
    
}VehicleFullInfor_st, * VehicleFullInfor_st_ptr;

#define VehicleFullInfor_st_len    (sizeof(VehicleFullInfor_st))


/* restore all compiler settings in stacks. */
#pragma pack(pop)


#endif

