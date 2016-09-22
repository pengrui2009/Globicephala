/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_data_element.h
 @brief  : Data Element routine set.
 @author : wangxianwen
 @history:
           2016-09-20    wangxianwen    Created file
           ...
******************************************************************************/
#ifndef __CV_DATA_ELEMENT_H__
#define __CV_DATA_ELEMENT_H__

#include "asn_internal.h"



#define STATUS_NO     0x00
#define STATUS_YES    0x01




/* DE_AntiLockBrakeStatus. */
typedef enum _DE_AntiLockBrakeStatus_en
{
    antilock_status_unavailable = 0,
    antilock_status_off         = 1,
    antilock_status_on          = 2,
    antilock_status_engaged     = 3

}DE_AntiLockBrakeStatus_en, * DE_AntiLockBrakeStatus_en_ptr;

#define DE_AntiLockBrakeStatus_en_len    (sizeof(DE_AntiLockBrakeStatus_en))


/* DE_AuxiliaryBrakeStatus. */
typedef enum _DE_AuxiliaryBrakeStatus_en
{
    auxiliary_status_unavailable = 0,
    auxiliary_status_off         = 1,
    auxiliary_status_on          = 2,
    auxiliary_status_reserved    = 3

}DE_AuxiliaryBrakeStatus_en, * DE_AuxiliaryBrakeStatus_en_ptr;

#define DE_AuxiliaryBrakeStatus_en_len    (sizeof(DE_AuxiliaryBrakeStatus_en))


/* DE_BrakeAppliedStatus. */
typedef struct _DE_BrakeAppliedStatus_st
{
    uint8_t brake_status_unavailable :1;
    uint8_t brake_status_leftFront   :1;
    uint8_t brake_status_leftRear    :1;
    uint8_t brake_status_rightFront  :1;
    uint8_t brake_status_rightRear   :1;

    uint8_t reserved                 :3;

}DE_BrakeAppliedStatus_st, * DE_BrakeAppliedStatus_st_ptr;

#define DE_BrakeAppliedStatus_st_len    (sizeof(DE_BrakeAppliedStatus_st))


/* DE_BrakeBoostApplied. */
typedef enum _DE_BrakeBoostApplied_en
{
    brake_boost_unavailable = 0,
    brake_boost_off         = 1,
    brake_boost_on          = 2

}DE_BrakeBoostApplied_en, * DE_BrakeBoostApplied_en_ptr;

#define DE_BrakeBoostApplied_en_len    (sizeof(_DE_BrakeBoostApplied_en))


/* DE_ElevationConfidence.*/
typedef enum _DE_ElevationConfidence_en
{
    ElevationConf_unavailable	= 0,
	ElevationConf_elev_500_00	= 1,
	ElevationConf_elev_200_00	= 2,
	ElevationConf_elev_100_00	= 3,
	ElevationConf_elev_050_00	= 4,
	ElevationConf_elev_020_00	= 5,
	ElevationConf_elev_010_00	= 6,
	ElevationConf_elev_005_00	= 7,
	ElevationConf_elev_002_00	= 8,
	ElevationConf_elev_001_00	= 9,
	ElevationConf_elev_000_50	= 10,
	ElevationConf_elev_000_20	= 11,
	ElevationConf_elev_000_10	= 12,
	ElevationConf_elev_000_05	= 13,
	ElevationConf_elev_000_02	= 14,
	ElevationConf_elev_000_01	= 15

}DE_ElevationConfidence_en, * DE_ElevationConfidence_en_ptr;

#define DE_ElevationConfidence_en_len    (sizeof(DE_ElevationConfidence_en))


/* DE_ExteriorLights. */
typedef struct _DE_ExteriorLights_st
{
    uint16_t lowBeamHeadlightsOn     :1;
    uint16_t highBeamHeadlightsOn    :1;
    uint16_t leftTurnSignalOn        :1;
    uint16_t rightTurnSignalOn       :1;
    uint16_t hazardSignalOn          :1;
    uint16_t automaticLightControlOn :1;
    uint16_t daytimeRunningLightsOn  :1;
    uint16_t fogLightOn              :1;
    
    uint16_t parkingLightsOn         :1;
    uint16_t reserved                :7;

}DE_ExteriorLights_st, * DE_ExteriorLights_st_ptr;

#define DE_ExteriorLights_st_len    (sizeof(DE_ExteriorLights_st))



/* DE_GNSSstatus. */
typedef struct _DE_GNSSstatus_st
{
    uint8_t GNSSstat_unavailable              :1;
    uint8_t GNSSstat_isHealthy                :1;
    uint8_t GNSSstat_isMonitored              :1;
    uint8_t GNSSstat_baseStationType          :1;
    uint8_t GNSSstat_aPDOPofUnder5            :1;
    uint8_t GNSSstat_inViewOfUnder5           :1;
    uint8_t GNSSstat_localCorrectionPresend   :1;
    uint8_t GNSSstat_networkCorrectionPresend :1;

}DE_GNSSstatus_st, * DE_GNSSstatus_st_ptr;

#define DE_GNSSstatus_st_len    (sizeof(DE_GNSSstatus_st))




/* DE_HeadingConfidence. */
typedef enum _DE_HeadingConfidence_en
{
    HeadingConf_unavailable   = 0,
	HeadingConf_prec10deg	  = 1,
	HeadingConf_prec05deg     = 2,
	HeadingConf_prec01deg     = 3,
	HeadingConf_prec0_1deg	  = 4,
	HeadingConf_prec0_05deg	  = 5,
	HeadingConf_prec0_01deg	  = 6,
	HeadingConf_prec0_0125deg = 7
	
}DE_HeadingConfidence_en, * DE_HeadingConfidence_en_ptr;

#define DE_HeadingConfidence_en_len    (sizeof(DE_HeadingConfidence_en))


/* DE_PositionConfidence. */
typedef enum _DE_PositionConfidence_en
{
    PositionConf_unavailable = 0,
	PositionConf_a500m	= 1,
	PositionConf_a200m	= 2,
	PositionConf_a100m	= 3,
	PositionConf_a50m	= 4,
	PositionConf_a20m	= 5,
	PositionConf_a10m	= 6,
	PositionConf_a5m	= 7,
	PositionConf_a2m	= 8,
	PositionConf_a1m	= 9,
	PositionConf_a50cm	= 10,
	PositionConf_a20cm	= 11,
	PositionConf_a10cm	= 12,
	PositionConf_a5cm	= 13,
	PositionConf_a2cm	= 14,
	PositionConf_a1cm	= 15
	
}DE_PositionConfidence_en, * DE_PositionConfidence_en_ptr;

#define DE_PositionConfidence_en_len    (sizeof(_DE_PositionConfidence_en))


/* DE_SpeedConfidence. */
typedef enum _DE_SpeedConfidence_en
{
    SpeedConf_unavailable	= 0,
	SpeedConf_prec100ms	    = 1,
	SpeedConf_prec10ms	    = 2,
	SpeedConf_prec5ms	    = 3,
	SpeedConf_prec1ms	    = 4,
	SpeedConf_prec0_1ms	    = 5,
	SpeedConf_prec0_05ms	= 6,
	SpeedConf_prec0_01ms	= 7
    
}DE_SpeedConfidence_en, * DE_SpeedConfidence_en_ptr;

#define DE_SpeedConfidence_en_len    (sizeof(DE_SpeedConfidence_en))





/* DE_StabilityControlStatus. */
typedef enum _DE_StabilityControlStatus_en
{
    stability_status_unavailable = 0,
    stability_status_off         = 1,
    stability_status_on          = 2,
    stability_status_engaged     = 3

}DE_StabilityControlStatus_en, * DE_StabilityControlStatus_en_ptr;

#define DE_StabilityControlStatus_en_len    (sizeof(DE_StabilityControlStatus_en))


/* DE_ThrottleConfidence. */
typedef enum _DE_ThrottleConfidence_en
{
    ThrottleConf_unavailable    = 0,
	ThrottleConf_prec10percent	= 1,
	ThrottleConf_prec1percent   = 2,
	ThrottleConf_prec0_5percent	= 3
    
}DE_ThrottleConfidence_en, * DE_ThrottleConfidence_en_ptr;

#define DE_ThrottleConfidence_en_len    (sizeof(DE_ThrottleConfidence_en))





/* DE_TimeConfidence. */
typedef enum _DE_TimeConfidence_en
{
	TimeConf_unavailable	= 0,
	TimeConf_time_100_000	= 1,
	TimeConf_time_050_000	= 2,
	TimeConf_time_020_000	= 3,
	TimeConf_time_010_000	= 4,
	TimeConf_time_002_000	= 5,
	TimeConf_time_001_000	= 6,
	TimeConf_time_000_500	= 7,
	TimeConf_time_000_200	= 8,
	TimeConf_time_000_100	= 9,
	TimeConf_time_000_050	= 10,
	TimeConf_time_000_020	= 11,
	TimeConf_time_000_010	= 12,
	TimeConf_time_000_005	= 13,
	TimeConf_time_000_002	= 14,
	TimeConf_time_000_001	= 15,
	TimeConf_time_000_000_5	= 16,
	TimeConf_time_000_000_2	= 17,
	TimeConf_time_000_000_1	= 18,
	TimeConf_time_000_000_05	= 19,
	TimeConf_time_000_000_02	= 20,
	TimeConf_time_000_000_01	= 21,
	TimeConf_time_000_000_005	= 22,
	TimeConf_time_000_000_002	= 23,
	TimeConf_time_000_000_001	= 24,
	TimeConf_time_000_000_000_5	= 25,
	TimeConf_time_000_000_000_2	= 26,
	TimeConf_time_000_000_000_1	= 27,
	TimeConf_time_000_000_000_05	= 28,
	TimeConf_time_000_000_000_02	= 29,
	TimeConf_time_000_000_000_01	= 30,
	TimeConf_time_000_000_000_005	= 31,
	TimeConf_time_000_000_000_002	= 32,
	TimeConf_time_000_000_000_001	= 33,
	TimeConf_time_000_000_000_000_5	= 34,
	TimeConf_time_000_000_000_000_2	= 35,
	TimeConf_time_000_000_000_000_1	= 36,
	TimeConf_time_000_000_000_000_05	= 37,
	TimeConf_time_000_000_000_000_02	= 38,
	TimeConf_time_000_000_000_000_01	= 39

}DE_TimeConfidence_en, * DE_TimeConfidence_en_ptr;

#define DE_TimeConfidence_en_len    (sizeof(DE_TimeConfidence_en))


/* DE_TractionControlStatus. */
typedef enum _DE_TractionControlStatus_en
{
    traction_status_unavailable = 0,
    traction_status_off         = 1,
    traction_status_on          = 2,
    traction_status_engaged     = 3

}DE_TractionControlStatus_en, DE_TractionControlStatus_en_ptr;

#define DE_TractionControlStatus_en_len    (sizeof(DE_TractionControlStatus_en))


/* DE_TransmissionState. */
typedef enum _DE_TransmissionState_en
{
    trans_state_neutral      = 0,
    trans_state_park         = 1, 
    trans_state_forwardGears = 2,
    trans_state_reverseGears = 3,
    trans_state_reserved1    = 4,
    trans_state_reserved2    = 5,
    trans_state_reserved3    = 6,
    trans_state_unavailable  = 7
    
}DE_TransmissionState_en, * DE_TransmissionState_en_ptr;

#define DE_TransmissionState_en_len    (sizeof(DE_TransmissionState_en))


/* DE_VehicleEventFlags. */
typedef struct _DE_VehicleEventFlags_st
{
    uint16_t eventHazardLights              :1;
    uint16_t eventStopLineViolation         :1;
    uint16_t eventABSactivated              :1;
    uint16_t eventTractionControlLoss       :1;
    uint16_t eventStabilityControlactivated :1;
    uint16_t eventHazardousMaterials        :1;
    uint16_t eventReserved1                 :1;
    uint16_t eventHardBraking               :1;
    uint16_t eventLightsChanged             :1;
    uint16_t eventWipersChanged             :1;
    uint16_t eventFlatTire                  :1;
    uint16_t eventDisabledVehicle           :1;
    uint16_t eventAirBagDeployment          :1;

    uint16_t reserved                       :3;
    
}DE_VehicleEventFlags_st, * DE_VehicleEventFlags_st_ptr;

#define DE_VehicleEventFlags_st_len    (sizeof(DE_VehicleEventFlags_st))















#endif /* __CV_DATA_ELEMENT_H__ */

