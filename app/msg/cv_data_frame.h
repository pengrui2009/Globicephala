/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_data_frame.h
 @brief  : Data Frame routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/
#ifndef __CV_DATA_FRAME_H__
#define __CV_DATA_FRAME_H__

#include "cv_data_element.h"

#include "DDateTime.h"
#include "FullPositionVector.h"




/* Message optinal status. */
#define MSG_OPTIONAL_YES    0x01
#define MSG_OPTIONAL_NO     0x00



/* Acceleration set 4 way structure. */
typedef struct _DF_AccelerationSet4Way_st
{   
    /* Along the vehicle longitudinal axis. Unit: m/s^2. */
    float  longitudinal;

    /* Along the vehicle lateral axis. Unit: m/s^2. */
    float       lateral;

    /* Alone the vehicle vertical axis. Unit: m/s^2. */
    float      vertical;

    /* Vehicle's yaw rate. Unit: degrees per second. */
    float      yaw_rate;
    
}DF_AccelerationSet4Way_st, * DF_AccelerationSet4Way_st_ptr;

#define DF_AccelerationSet4Way_st_len    (sizeof(DF_AccelerationSet4Way_st))


/* DF_BrakeSystemStatus. */
typedef struct _DF_BrakeSystemStatus_st
{
    DE_BrakeAppliedStatus_st   wheelBrakes;
    DE_TractionControlStatus_en   traction;
    DE_AntiLockBrakeStatus_en          abs;
    DE_StabilityControlStatus_en       scs;
    DE_BrakeBoostApplied_en     brakeBoost;
    DE_AuxiliaryBrakeStatus_en   auxBrakes;

}DF_BrakeSystemStatus_st, * DF_BrakeSystemStatus_st_ptr;

#define DF_BrakeSystemStatus_st_len    (sizeof(DF_BrakeSystemStatus_st))









/* DF_DDateTime optional configuration. */
typedef struct _DF_DDateTime_opt_st
{
    uint8_t DYear   :1;
    uint8_t DMonth  :1;
    uint8_t DDay    :1;
    uint8_t DHour   :1;
    uint8_t DMinute :1;
    uint8_t DSecond :1;
    uint8_t DOffset :1;

}DF_DDateTime_opt_st, *DF_DDateTime_opt_st_ptr;

#define DF_DDateTime_opt_st_len    (sizeof(DF_DDateTime_opt_st))


/* DF_DDateTime. */
typedef struct _DF_DDateTime_st
{
    /* Optional. */
    DF_DDateTime_opt_st opt;

    /* Data. */
    uint16_t           year;
    uint8_t           month;
    uint8_t             day;
    uint8_t            hour;
    uint8_t          minute;
    uint16_t         second;
    uint16_t         offset;

}DF_DDateTime_st, * DF_DDateTime_st_ptr;

#define _DF_DDateTime_st_len    (sizeof(_DF_DDateTime_st))






/* DF_FullPositionVector optional configuration. */
typedef struct _DF_FullPositionVector_opt_st
{
    uint8_t DDateTime              :1;
    uint8_t Elevation              :1;
    uint8_t Heading                :1;
    uint8_t TransmissionAndSpeed   :1;
    uint8_t PositionAccuracy       :1;
    uint8_t TimeConfidence         :1;
    uint8_t PositionConfidenceSet  :1;
    uint8_t SpeedHeadingThrottle   :1;

    DF_DDateTime_opt_st DDataTime_opt;     

}DF_FullPositionVector_opt_st, *DF_FullPositionVector_opt_st_ptr;

#define DF_FullPositionVector_opt_st_len    (sizeof(DF_FullPositionVector_opt_st))




/* DF_PathHistoryPoint optional. */
typedef struct _DF_PathHistoryPoint_opt_st
{
    uint8_t       speed :1;
    uint8_t posAccuracy :1;
    uint8_t     heading :1;

}DF_PathHistoryPoint_opt_st, * DF_PathHistoryPoint_opt_st_ptr;

#define DF_PathHistoryPoint_opt_st_len    (sizeof(DF_PathHistoryPoint_opt_st))


/* Position accuracy structure. */
typedef struct _DF_PositionAccuracy_st
{
    /* Radius of semi-major axis of an ellipsoid. Unit: meter. */
    float        semi_major_accu;

    /* Radius of semi-minor axis of an ellipsoid. Unit: meter. */
    float        semi_minor_accu;

    /* Angle of semi-major axis of an ellipsoid. Unit: degree. */
    float semi_major_orientation;

}DF_PositionAccuracy_st, * DF_PositionAccuracy_st_ptr;

#define DF_PositionAccuracy_st_len    (sizeof(DF_PositionAccuracy_st))


/* DF_PathHistoryPoint. */
typedef struct _DF_PathHistoryPoint_st
{
    /* Optional. */
    DF_PathHistoryPoint_opt_st     opt;

    /* Data. */
    float                    latOffset;
    float                    lonOffset;
    float              elevationOffset;
    float                   timeOffset;

    float                        speed;
    DF_PositionAccuracy_st posAccuracy;
    float                      heading;
    
}DF_PathHistoryPoint_st, * DF_PathHistoryPoint_st_ptr;

#define DF_PathHistoryPoint_st_len    (sizeof(DF_PathHistoryPoint_st))


/* DF_PathPrediction. */
typedef struct _DF_PathPrediction_st
{
    float radiusOfCurve;  /* Unit: m. */
    float    confidence;  /* Unit: percent. */

}DF_PathPrediction_st, * DF_PathPrediction_st_ptr;

#define DF_PathPrediction_st_len    (sizeof(DF_PathPrediction_st))












/* DF_PositionConfidenceSet. */
typedef struct _DF_PositionConfidenceSet_st
{
    DE_PositionConfidence_en        pos;
    DE_ElevationConfidence_en elevation;

}DF_PositionConfidenceSet_st, * DF_PositionConfidenceSet_st_ptr;

#define DF_PositionConfidenceSet_st_len    (sizeof(DF_PositionConfidenceSet_st))


/* DF_SpecialVehicleExtensions optional. */
typedef struct _DF_SpecialVehicleExtensions_opt_st
{
    uint8_t vehicleAlerts  :1;
    uint8_t description    :1;
    uint8_t trailers       :1;
    
    uint8_t reserved       :5;
    
}DF_SpecialVehicleExtensions_opt_st, * DF_SpecialVehicleExtensions_opt_st_ptr;

#define DF_SpecialVehicleExtensions_opt_st_len    (sizeof(DF_SpecialVehicleExtensions_opt_st))


/* DF_SpecialVehicleExtensions. */
typedef struct _DF_SpecialVehicleExtensions_st
{
    /* Optional. */
    DF_SpecialVehicleExtensions_opt_st opt;
    
    /* Data. */
    
    
}DF_SpecialVehicleExtensions_st, * DF_SpecialVehicleExtensions_st_ptr;

#define DF_SpecialVehicleExtensions_st_len    (sizeof(DF_SpecialVehicleExtensions_st))


/* DF_SpeedandHeadingandThrottleConfidence. */
typedef struct _DF_SpeedHeadingThrottleConf_st
{
    DE_HeadingConfidence_en   heading;
    DE_SpeedConfidence_en       speed;
    DE_ThrottleConfidence_en throttle;

}DF_SpeedHeadingThrottleConf_st, * DF_SpeedHeadingThrottleConf_st_ptr;

#define DF_SpeedHeadingThrottleConf_st_len    (sizeof(DF_SpeedHeadingThrottleConf_st))


/* DF_SupplementalVehicleExtensions optional. */
typedef struct _DF_SupplementalVehicleExtensions_opt_st
{
    uint8_t vehicleAlerts  :1;
    uint8_t description    :1;
    uint8_t trailers       :1;
    
    uint8_t reserved       :5;
    
}DF_SupplementalVehicleExtensions_opt_st, * DF_SupplementalVehicleExtensions_opt_st_ptr;

#define DF_SupplementalVehicleExtensions_opt_st_len    (sizeof(DF_SupplementalVehicleExtensions_opt_st))


/* DF_SupplementalVehicleExtensions. */
typedef struct _DF_SupplementalVehicleExtensions_st
{
    /* Optional. */
    DF_SupplementalVehicleExtensions_opt_st opt;
    
    /* Data. */
    
    
}DF_SupplementalVehicleExtensions_st, * DF_SupplementalVehicleExtensions_st_ptr;

#define DF_SupplementalVehicleExtensions_st_len    (sizeof(DF_SupplementalVehicleExtensions_st))


/* DF_TransmissionAndSpeed. */
typedef struct _DF_TransmissionAndSpeed_st
{
    DE_TransmissionState_en transmission;
    float                          speed;  /* Driving speed. Unit km/h. */
    
}DF_TransmissionAndSpeed_st, * DF_TransmissionAndSpeed_st_ptr;

#define DF_TransmissionAndSpeed_st_len    (sizeof(DF_TransmissionAndSpeed_st))






/* Vehicle size structure. */
typedef struct _DF_VehicleSize_st
{
    /* Vehicle size unit: m. */
    float   width;
    float  length;

}DF_VehicleSize_st, * DF_VehicleSize_st_ptr;

#define DF_VehicleSize_st_len    (sizeof(DF_VehicleSize_st))


/* DF_FullPositionVector. */
typedef struct _DF_FullPositionVector_st
{
    /* Optional. */
    DF_FullPositionVector_opt_st   opt;

    /* Data. */
    DF_DDateTime_st            utcTime;

    double                   longitude;  /* Geographic longitude. Unit: degree. */
    double                    latitude;  /* Geographic latitude. Unit: degree. */  
    float                    elevation;  /* Geographic position above or below the reference ellipsoid. Unit: meter. */
    float                      heading;  /* Driving direction. Unit degree. */
    DF_TransmissionAndSpeed_st   speed;
    
    DF_PositionAccuracy_st posAccuracy;
    
    DE_TimeConfidence_en            timeConf;
    DF_PositionConfidenceSet_st      posConf;
    DF_SpeedHeadingThrottleConf_st speedConf;

}DF_FullPositionVector_st, * DF_FullPositionVector_st_ptr;

#define DF_FullPositionVector_st_len    (sizeof(DF_FullPositionVector_st))


/* DF_PathHistory optional. */
typedef struct _DF_PathHistory_opt_st
{
    uint8_t initialPosition :1;
    uint8_t currGNSSstatus  :1;

    uint8_t reserved        :6;
    
}DF_PathHistory_opt_st, * DF_PathHistory_opt_st_ptr;

#define DF_PathHistory_opt_st_len    (sizeof(DF_PathHistory_opt_st))


/* DF_PathHistory. */
typedef struct _DF_PathHistory_st
{
    /* Optional. */
    DF_PathHistoryPoint_opt_st           opt;

    /* Data. */
    DF_FullPositionVector_st initialPosition;
    DE_GNSSstatus_st          currGNSSstatus;

    uint8_t                         pointNum;
    DF_PathHistoryPoint_st     crumbData[23];
    
}DF_PathHistory_st, * DF_PathHistory_st_ptr;

#define DF_PathHistory_st_len    (sizeof(DF_PathHistory_st))


/* DF_VehicleSafetyExternsions optional. */
typedef struct _DF_VehicleSafetyExternsions_opt_st
{
    uint8_t events         :1;
    uint8_t pathHistory    :1;
    uint8_t pathPrediction :1;
    uint8_t lights         :1;

    uint8_t reserved       :4;

}DF_VehicleSafetyExternsions_opt_st, * DF_VehicleSafetyExternsions_opt_st_ptr;

#define DF_VehicleSafetyExternsions_opt_st_len    (sizeof(DF_VehicleSafetyExternsions_opt_st))


/* DF_VehicleSafetyExtensions. */
typedef struct _DF_VehicleSafetyExtensions_st
{
    /* Optinal. */
    DF_VehicleSafetyExternsions_opt_st opt;

    /* Data. */
    DE_VehicleEventFlags_st         events;
    DF_PathHistory_st          pathHistory;
    DF_PathPrediction_st    pathPrediction;
    DE_ExteriorLights_st            lights;
    
}DF_VehicleSafetyExtensions_st, * DF_VehicleSafetyExtensions_st_ptr;

#define DF_VehicleSafetyExtensions_st_len    (sizeof(DF_VehicleSafetyExtensions_st))


/* DF_BSMcoreData. */
typedef struct _DF_BSMcoreData_st
{
    uint8_t                    msgCnt;
    uint8_t                     id[4];  /* Product id. */
    uint16_t                  secMark;  /* DSRC second. */

    double                   latitude;  /* Geographic latitude. Unit: degree. */
    double                  longitude;  /* Geographic longitude. Unit: degree. */
    float                   elevation;  /* Geographic position above or below the reference ellipsoid. Unit: meter. */
   
    DF_PositionAccuracy_st   accuracy;  /* Vehicle position accuracy. */

    DE_TransmissionState_en     trans;  /* Transmission status. */
    float                       speed;  /* Driving speed. Unit km/h. */
    float                     heading;  /* Driving direction. Unit degree. */
    float	                    angle;  /* Steering wheel angle. Unit degree. */
    DF_AccelerationSet4Way_st acceSet;  /* Driving acceleration set. */
    DF_BrakeSystemStatus_st    brakes;  /* Brake system status. */
    DF_VehicleSize_st            size;  /* Vehicle size. */
    
}DF_BSMcoreData_st, * DF_BSMcoreData_st_ptr;

#define DF_BSMcoreData_st_len    (sizeof(DF_BSMcoreData_st))


/* DF_BSMpartIIExtension optional. */
typedef struct _DF_BSMpartIIExtension_opt_st
{
    uint8_t vecSafetyExt       :1;
    uint8_t specialVecExt      :1;
    uint8_t supplementalVecExt :1;

    uint8_t reserved           :5;
    
}DF_BSMpartIIExtension_opt_st, * DF_BSMpartIIExtension_opt_st_ptr;

#define DF_BSMpartIIExtension_opt_st_len    (sizeof(DF_BSMpartIIExtension_opt_st))


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

#define DF_BSMpartIIExtension_st_len    (sizeof(DF_BSMpartIIExtension_st))





/* Allocate routine for DF_DDateTime. */
extern int DF_DDateTime_allocate(DDateTime_t **time_ptr_ptr, DF_DDateTime_st_ptr DDate_ptr);

/* Free routine for DF_DDateTime. */
extern int DF_DDateTime_free(DDateTime_t *time_ptr);

/* Allocate routine for DF_FullPositionVector. */
extern int DF_FullPositionVector_allocate(FullPositionVector_t **pos_ptr_ptr, DF_FullPositionVector_st_ptr fullPos_ptr);

/* Free routine for FullPositionVector. */
extern int DF_FullPositionVector_free(FullPositionVector_t *pos_ptr);



#endif /* __CV_VAM_DF_H__ */

