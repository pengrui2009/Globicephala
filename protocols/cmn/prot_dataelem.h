/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : prot_dataelem.h
 @brief  : Data Element routine set for J2735/ehmp protocols.
 @author : wangxianwen
 @history:
           2016-09-20    wangxianwen    Created file
           2017-10-12    pengrui        Modify for CSAE
           2017-11-28    wangxianwen    Separate common element for J2735/ehmp.
           ...
******************************************************************************/

#ifndef __PROT_DATAELEM_H__
#define __PROT_DATAELEM_H__

#include "stdint.h"


/*DE_Acceleration Type*/
typedef float                    DE_Acceleration_t;

#define DE_Acceleration_t_len    (sizeof(DE_Acceleration_t))


/*DE_AlertType Type*/
typedef uint16_t                 DE_AlertType_t;

#define DE_AlertType_t_len       (sizeof(DE_AlertType_t))


/* DE_AllowedManeuvers union */
typedef union _DE_AllowedManeuvers_un
{
    uint16_t                                     AllowedManeuvers_word;
    struct{
        uint8_t AllowedManeuvers_maneuverStraightAllowed            :1;
        uint8_t AllowedManeuvers_maneuverLeftAllowed                :1;
        uint8_t AllowedManeuvers_maneuverRightAllowed               :1;
        uint8_t AllowedManeuvers_maneuverUTurnAllowed               :1;
        uint8_t AllowedManeuvers_maneuverLeftTurnOnRedAllowed       :1;
        uint8_t AllowedManeuvers_maneuverRightTurnOnRedAllowed      :1;
        uint8_t AllowedManeuvers_maneuverLaneChangeAllowed          :1;
        uint8_t AllowedManeuvers_maneuverNoStoppingAllowed          :1;
        uint8_t AllowedManeuvers_yieldAllwaysRequired               :1;
        uint8_t AllowedManeuvers_goWithHalt                         :1;
        uint8_t AllowedManeuvers_caution                            :1;
        uint8_t AllowedManeuvers_reserved                           :4;
    }bit;
}DE_AllowedManeuvers_un, *DE_AllowedManeuvers_un_ptr;

#define    DE_AllowedManeuvers_un_len        (sizeof(DE_AllowedManeuvers_un))


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


/* DE_BasicVehicleClass_en. */
typedef enum _DE_BasicVehicleClass_en
{
    unknownVehicleClass                 = 0,
    //Not Equipped, Not known or unavailable
    specialVehicleClass                 = 1,
    // Special use
    //
    // Basic Passenger Motor Vehicle Types
    //
    passenger_Vehicle_TypeUnknown       = 10, // default type
    passenger_Vehicle_TypeOther         = 11,
    // various fuel types are handled in another element
    //
    // Light Trucks, Pickup, Van, Panel
    //
    lightTruck_Vehicle_TypeUnknown      = 20, // default type
    lightTruck_Vehicle_TypeOther        = 21,
    //
    // Trucks, Various axle types, includes HPMS items
    //
    truck_Vehicle_TypeUnknown           = 25, // default type
    truck_Vehicle_TypeOther             = 26,
    truck_axleCnt2                      = 27, // Two axle, six tire single units
    truck_axleCnt3                      = 28, // Three axle, single units
    truck_axleCnt4                      = 29, // Four or more axle, single unit
    truck_axleCnt4Trailer               = 30, // Four or less axle, single trailer
    truck_axleCnt5Trailer               = 31, // Five or less axle, single trailer
    truck_axleCnt6Trailer               = 32, // Six or more axle, single trailer
    truck_axleCnt5MultiTrailer          = 33, // Five or less axle, multi-trailer
    truck_axleCnt6MultiTrailer          = 34, // Six axle, multi-trailer
    truck_axleCnt7MultiTrailer          = 35, // Seven or more axle, multi-trailer
    //
    // Motorcycle Types
    //

    motorcycle_TypeUnknown              = 40, // default type
    motorcycle_TypeOther                = 41,
    motorcycle_Cruiser_Standard         = 42,
    motorcycle_SportUnclad              = 43,
    motorcycle_SportTouring             = 44,
    motorcycle_SuperSport               = 45,
    motorcycle_Touring                  = 46,
    motorcycle_Trike                    = 47,
    motorcycle_wPassengers              = 48, // type not stated
    //
    // Transit Types
    //
    transit_TypeUnknown                 = 50, // default type
    transit_TypeOther                   = 51,
    transit_BRT                         = 52,
    transit_ExpressBus                  = 53,
    transit_LocalBus                    = 54,
    transit_SchoolBus                   = 55,
    transit_FixedGuideway               = 56,
    transit_Paratransit                 = 57,
    transit_Paratransit_Ambulance       = 58,
    //
    // Emergency Vehicle Types
    //
    emergency_TypeUnknown               = 60, // default type
    emergency_TypeOther                 = 61, // includes federal users
    emergency_Fire_Light_Vehicle        = 62,
    emergency_Fire_Heavy_Vehicle        = 63,
    emergency_Fire_Paramedic_Vehicle      = 64,
    emergency_Fire_Ambulance_Vehicle    = 65,
    emergency_Police_Light_Vehicle      = 66,
    emergency_Police_Heavy_Vehicle      = 67,
    emergency_Other_Responder           = 68,
    emergency_Other_Ambulance           = 69,
    //
    // Other DSRC Equipped Travelers
    //
    otherTraveler_TypeUnknown           = 80, // default type
    otherTraveler_TypeOther             = 81,
    otherTraveler_Pedestrian            = 82,
    otherTraveler_Visually_Disabled     = 83,
    otherTraveler_Physically_Disabled   = 84,
    otherTraveler_Bicycle               = 85,
    otherTraveler_Vulnerable_Roadworker = 86,
    //
    // Other DSRC Equipped Device Types
    //
    infrastructure_TypeUnknown          = 90, // default type
    infrastructure_Fixed                = 91,
    infrastructure_Movable              = 92,
    equipped_CargoTrailer               = 93
}DE_BasicVehicleClass_en, DE_BasicVehicleClass_en_ptr;

#define DE_BasicVehicleClass_en_len        sizeof(DE_BasicVehicleClass_en)


/* DE_BrakeAppliedStatus structure. */
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


/* DE_BrakePedalStatus */
typedef enum _DE_BrakePedalStatus_en
{
    brakepedal_status_unavailable      = 0, //Vehicle brake pedal detector is unavailable
    brakepedal_status_off              = 1, //Vehicle's brake padel is not pressed
    brakepedal_status_on               = 2  //Vehicle's brake padel is pressed
    
}DE_BrakePedalStatus_en, *DE_BrakePedalStatus_en_ptr;

#define DE_BrakePedalStatus_en_len         (sizeof(DE_BrakePedalStatus_en))


/*DE_Confidence Type*/
typedef float                           DE_Confidence_t;

#define DE_Confidence_t_len      (sizeof(DE_Confidence_t))


/* DE_Description_st */
typedef struct _DE_Description_st
{
    uint8_t        Description_size;
    uint8_t   Description_data[256];
    
}DE_Description_st, *DE_Description_st_ptr;

#define    DE_Description_st_len        (sizeof(DE_Description_st))


/*DE_DescriptiveName */
typedef struct _DE_DescriptiveName_st
{
    uint8_t     DescriptiveName_size;
    uint8_t DescriptiveName_data[63];
    
}DE_DescriptiveName_st, *DE_DescriptiveName_st_ptr;

#define DE_DescriptiveName_st_len            (sizeof(DE_DescriptiveName_st))


/*DE_DDay Type*/
typedef uint8_t                    DE_DDay_t; /* 0 ~ 31*/

#define DE_DDay_t_len              (sizeof(DE_DDay_t))


/*DE_DHour Type*/
typedef uint8_t                    DE_DHour_t; /* 0 ~ 31, above 24 indicate invalid value*/

#define DE_DHour_t_len             (sizeof(DE_DHour_t))


/*DE_Dminute Type*/
typedef uint8_t                    DE_Dminute_t; /* 0 ~ 59*/

#define DE_Dminute_t_len           (sizeof(DE_Dminute_t))


/*DE_DMonth Type*/
typedef uint8_t                    DE_DMonth_t; /* 0 ~ 12*/

#define DE_DMonth_t_len            (sizeof(DE_DMonth_t))


/*DE_DSecond Type*/
typedef uint16_t                   DE_DSecond_t; /* units of milliseconds 0~59999, >= 60000 is invalid value*/

#define DE_DSecond_t_len           (sizeof(DE_DSecond_t))


/*DE_DTimeOffset Type*/
typedef int16_t                    DE_DTimeOffset_t; /*uints of minutes from  UTC time, -840 ~ 840*/

#define DE_DTimeOffset_t_len       (sizeof(DE_DTimeOffset_t))


/*DE_DYear Type*/
typedef uint16_t                   DE_DYear_t; /* 0 ~ 4095*/

#define DE_DYear_t_len             (sizeof(DE_DYear_t))


/*DE_Elevation Type*/
typedef float                      DE_Elevation_t; 

#define DE_Elevation_t_len         (sizeof(DE_Elevation_t))


/*DE_ElevationConfidence.*/
typedef enum _DE_ElevationConfidence_en
{
    ElevationConf_unavailable    = 0,
    ElevationConf_elev_500_00    = 1,
    ElevationConf_elev_200_00    = 2,
    ElevationConf_elev_100_00    = 3,
    ElevationConf_elev_050_00    = 4,
    ElevationConf_elev_020_00    = 5,
    ElevationConf_elev_010_00    = 6,
    ElevationConf_elev_005_00    = 7,
    ElevationConf_elev_002_00    = 8,
    ElevationConf_elev_001_00    = 9,
    ElevationConf_elev_000_50    = 10,
    ElevationConf_elev_000_20    = 11,
    ElevationConf_elev_000_10    = 12,
    ElevationConf_elev_000_05    = 13,
    ElevationConf_elev_000_02    = 14,
    ElevationConf_elev_000_01    = 15

}DE_ElevationConfidence_en, * DE_ElevationConfidence_en_ptr;

#define DE_ElevationConfidence_en_len    (sizeof(DE_ElevationConfidence_en))


/*DE_Extent. */
typedef enum _DE_Extent_en 
{
    Ext_useInstantlyOnly       = 0,
    Ext_useFor3meters          = 1,
    Ext_useFor10meters         = 2,
    Ext_useFor50meters         = 3,
    Ext_useFor100meters        = 4,
    Ext_useFor500meters        = 5,
    Ext_useFor1000meters       = 6,
    Ext_useFor5000meters       = 7,
    Ext_useFor10000meters      = 8,
    Ext_useFor50000meters      = 9,
    Ext_useFor100000meters     = 10,
    Ext_useFor500000meters     = 11,
    Ext_useFor1000000meters    = 12,
    Ext_useFor5000000meters    = 13,
    Ext_useFor10000000meters   = 14,
    Ext_forever                = 15
    
}DE_Extent_en, * DE_Extent_en_ptr;

#define DE_Extent_en_len    (sizeof(DE_Extent_en))


/* DE_ExteriorLights union. */
typedef union _DE_ExteriorLights_st
{
    uint16_t     ExteriorLights_word;
    
    struct{
        uint16_t ExteriorLights_lowBeamHeadlightsOn     :1;
        uint16_t ExteriorLights_highBeamHeadlightsOn    :1;
        uint16_t ExteriorLights_leftTurnSignalOn        :1;
        uint16_t ExteriorLights_rightTurnSignalOn       :1;
        uint16_t ExteriorLights_hazardSignalOn          :1;
        uint16_t ExteriorLights_automaticLightControlOn :1;
        uint16_t ExteriorLights_daytimeRunningLightsOn  :1;
        uint16_t ExteriorLights_fogLightOn              :1;
    
        uint16_t ExteriorLights_parkingLightsOn         :1;
        uint16_t ExteriorLights_reserved                :7;
    }bit;
    
}DE_ExteriorLights_un, * DE_ExteriorLights_un_ptr;

#define DE_ExteriorLights_un_len    (sizeof(DE_ExteriorLights_un))


/* DE_FurtherInfoID. */
#define DE_FurtherInfoID_BUFSIZE   2

typedef uint8_t                    DE_FurtherInfoID_st[DE_FurtherInfoID_BUFSIZE];
typedef DE_FurtherInfoID_st *      DE_FurtherInfoID_st_ptr;
#define DE_FurtherInfoID_st_len    (sizeof(DE_FurtherInfoID_st))


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

/*DE_Heading type*/
typedef float                   DE_Heading_t;

#define DE_Heading_t_len        sizeof(DE_Heading_t)


/* DE_HeadingConfidence. */
typedef enum _DE_HeadingConfidence_en
{
    HeadingConf_unavailable   = 0,
    HeadingConf_prec10deg     = 1,
    HeadingConf_prec05deg     = 2,
    HeadingConf_prec01deg     = 3,
    HeadingConf_prec0_1deg    = 4,
    HeadingConf_prec0_05deg   = 5,
    HeadingConf_prec0_01deg   = 6,
    HeadingConf_prec0_0125deg = 7
    
}DE_HeadingConfidence_en, * DE_HeadingConfidence_en_ptr;

#define DE_HeadingConfidence_en_len    (sizeof(DE_HeadingConfidence_en))


/* DE_HeadingSlice. */
typedef struct _DE_HeadingSlice_st
{
    uint16_t HeadingSlice_from000_0to022_5degrees    :1;
    uint16_t HeadingSlice_from022_5to045_0degrees    :1;
    uint16_t HeadingSlice_from045_0to067_5degrees    :1;
    uint16_t HeadingSlice_from067_5to090_0degrees    :1;
    uint16_t HeadingSlice_from090_0to112_5degrees    :1;
    uint16_t HeadingSlice_from112_5to135_0degrees    :1;
    uint16_t HeadingSlice_from135_0to157_5degrees    :1;
    uint16_t HeadingSlice_from157_5to180_0degrees    :1;
    uint16_t HeadingSlice_from180_0to202_5degrees    :1;
    uint16_t HeadingSlice_from202_5to225_0degrees    :1;
    uint16_t HeadingSlice_from225_0to247_5degrees    :1;
    uint16_t HeadingSlice_from247_5to270_0degrees    :1;
    uint16_t HeadingSlice_from270_0to292_5degrees    :1;
    uint16_t HeadingSlice_from292_5to315_0degrees    :1;
    uint16_t HeadingSlice_from315_0to337_5degrees    :1;
    uint16_t HeadingSlice_from337_5to360_0degrees    :1;

}DE_HeadingSlice_st, * DE_HeadingSlice_st_ptr;

#define DE_HeadingSlice_st_len    (sizeof(DE_HeadingSlice_st))


/* DE_IntersectionStatusObject union*/
typedef union _DE_IntersectionStatusObject_un
{
    uint16_t       InterStatusObj_word;
    struct{
        uint8_t    InterStatusObj_manualControlIsEnabled             :1;
        uint8_t    InterStatusObj_stopTimeIsActivated                :1;
        uint8_t    InterStatusObj_failureFlash                       :1;
        uint8_t    InterStatusObj_preemptIsActive                    :1;
        uint8_t    InterStatusObj_signalPriorityIsActive             :1;
        uint8_t    InterStatusObj_fixedTimeOperation                 :1;
        uint8_t    InterStatusObj_trafficDependentOperation          :1;
        uint8_t    InterStatusObj_standbyOperation                   :1;
        uint8_t    InterStatusObj_failureMode                        :1;
        uint8_t    InterStatusObj_off                                :1;
        uint8_t    InterStatusObj_recentMAPmessageUpdate             :1;
        uint8_t InterStatusObj_recentChangeInMAPassignedLanesIDsUsed :1;
        uint8_t    InterStatusObj_noValidMAPisAvailableAtThisTime    :1;
        uint8_t    InterStatusObj_noValidSPATisAvailableAtThisTime   :1;
        uint8_t    InterStatusObj_reserved                           :2;
    }bit;
}DE_IntersectionStatusObject_un, *DE_IntersectionStatusObject_un_ptr;

#define DE_IntersectionStatusObject_un_len        (sizeof(DE_IntersectionStatusObject_un))


/* DE_LaneAttributes_Vehicle union */
typedef union _DE_LaneAttributes_Vehicle_un
{
    uint16_t    LaneAttributes_Vehicle_word;
    struct {
        uint8_t LaneAttributes_Vehicle_isVehicleRevocableLane   :1;
        uint8_t LaneAttributes_Vehicle_isVehicleFlyOverLane     :1;
        uint8_t LaneAttributes_Vehicle_hovLaneUseOnly           :1;
        uint8_t LaneAttributes_Vehicle_restrictedToBusUse       :1;
        uint8_t LaneAttributes_Vehicle_restrictedToTaxiUse      :1;
        uint8_t LaneAttributes_Vehicle_restrictedFromPublicUse  :1;
        uint8_t LaneAttributes_Vehicle_hasIRbeaconCoverage      :1;
        uint8_t LaneAttributes_Vehicle_permissionOnRequest      :1;
        uint8_t LaneAttributes_Vehicle_reserved                 :8;
    }bit;

}DE_LaneAttributes_Vehicle_un, * DE_LaneAttributes_Vehicle_un_ptr;

#define DE_LaneAttributes_Vehicle_un_len        (sizeof(DE_LaneAttributes_Vehicle_un))


/* DE_LaneAttributes_Crosswalk union */
typedef union _DE_LaneAttributes_Crosswalk_un
{
    uint16_t    LaneAttributes_Crosswalk_word;
    struct {
        uint8_t LaneAttributes_Crosswalk_crosswalkRevocableLane         :1;
        uint8_t LaneAttributes_Crosswalk_bicyleUseAllowed               :1;
        uint8_t LaneAttributes_Crosswalk_isXwalkFlyOverLane             :1;
        uint8_t LaneAttributes_Crosswalk_fixedCycleTime                 :1;
        uint8_t LaneAttributes_Crosswalk_biDirectionalCycleTimes        :1;
        uint8_t LaneAttributes_Crosswalk_hasPushToWalkButton            :1;
        uint8_t LaneAttributes_Crosswalk_audioSupport                   :1;
        uint8_t LaneAttributes_Crosswalk_rfSignalRequestPresent         :1;
        uint8_t LaneAttributes_Crosswalk_unsignalizedSegmentsPresent    :1;
        uint8_t LaneAttributes_Crosswalk_reserved                       :7;
    }bit;

}DE_LaneAttributes_Crosswalk_un, *DE_LaneAttributes_Crosswalk_un_ptr;

#define     DE_LaneAttributes_Crosswalk_un_len        (sizeof(DE_LaneAttributes_Crosswalk_un))


/* DE_LaneAttributes_Bike union */
typedef union _DE_LaneAttributes_Bike_un
{
    uint16_t       LaneAttributes_Bike_word;
    struct {
        uint8_t    LaneAttributes_Bike_bikeRevocableLane            :1;
        uint8_t    LaneAttributes_Bike_pedestrianUseAllowed         :1;
        uint8_t    LaneAttributes_Bike_isBikeFlyOverLane            :1;
        uint8_t    LaneAttributes_Bike_fixedCycleTime               :1;
        uint8_t    LaneAttributes_Bike_biDirectionalCycleTimes      :1;
        uint8_t    LaneAttributes_Bike_isolatedByBarrier            :1;
        uint8_t    LaneAttributes_Bike_unsignalizedSegmentsPresent  :1;
        uint8_t    LaneAttributes_Bike_reserved1                    :1;
        uint8_t    LaneAttributes_Bike_reserved2                    :8;
    }bit;
}DE_LaneAttributes_Bike_un, *DE_LaneAttributes_Bike_un_ptr;

#define     DE_LaneAttributes_Bike_un_len        (sizeof(DE_LaneAttributes_Bike_un))


/* DE_LaneAttributes_Sidewalk structure */
typedef union _DE_LaneAttributes_Sidewalk_un
{
    uint16_t    LaneAttributes_Sidewalk_word;
    struct {
        uint8_t LaneAttributes_Sidewalk_sidewalk_RevocableLane      :1;
        uint8_t LaneAttributes_Sidewalk_bicyleUseAllowed            :1;
        uint8_t LaneAttributes_Sidewalk_isSidewalkFlyOverLane       :1;
        uint8_t LaneAttributes_Sidewalk_walkBikes                   :1;
        uint8_t LaneAttributes_Sidewalk_reserved1                   :4;
        uint8_t LaneAttributes_Sidewalk_reserved2                   :8;
    }bit;

}DE_LaneAttributes_Sidewalk_un, *DE_LaneAttributes_Sidewalk_un_ptr;

#define     DE_LaneAttributes_Sidewalk_un_len        (sizeof(DE_LaneAttributes_Sidewalk_un))


/* DE_LaneAttributes_Barrier structure */
typedef union _DE_LaneAttributes_Barrier_un
{
    uint16_t    LaneAttributes_Barrier_word;
    struct {
        uint8_t LaneAttributes_Barrier_median_RevocableLane         :1;
        uint8_t LaneAttributes_Barrier_median                       :1;
        uint8_t LaneAttributes_Barrier_whiteLineHashing             :1;
        uint8_t LaneAttributes_Barrier_stripedLines                 :1;
        uint8_t LaneAttributes_Barrier_doubleStripedLines           :1;
        uint8_t LaneAttributes_Barrier_trafficCones                 :1;
        uint8_t LaneAttributes_Barrier_constructionBarrier          :1;
        uint8_t LaneAttributes_Barrier_trafficChannels              :1;
        uint8_t LaneAttributes_Barrier_lowCurbs                     :1;
        uint8_t LaneAttributes_Barrier_highCurbs                    :1;
        uint8_t LaneAttributes_Barrier_reserved                     :6;
    }bit;

}DE_LaneAttributes_Barrier_un, *DE_LaneAttributes_Barrier_un_ptr;

#define     DE_LaneAttributes_Barrier_un_len        (sizeof(DE_LaneAttributes_Barrier_un))


/* DE_LaneAttributes_Striping structure */
typedef union _DE_LaneAttributes_Striping_un
{
    uint16_t    LaneAttributes_Striping_word;
    struct {
        uint8_t LaneAttributes_Striping_stripeToConnectingLanesRevocableLane    :1;
        uint8_t LaneAttributes_Striping_stripeDrawOnLeft                        :1;
        uint8_t LaneAttributes_Striping_stripeDrawOnRight                       :1;
        uint8_t LaneAttributes_Striping_stripeToConnectingLanesLeft             :1;
        uint8_t LaneAttributes_Striping_stripeToConnectingLanesRight            :1;
        uint8_t LaneAttributes_Striping_stripeToConnectingLanesAhead            :1;
        uint8_t LaneAttributes_Striping_reserved1                               :2;
        uint8_t LaneAttributes_Striping_reserved2                               :8;
    }bit;

}DE_LaneAttributes_Striping_un, *DE_LaneAttributes_Striping_un_ptr;

#define     DE_LaneAttributes_Striping_un_len        (sizeof(DE_LaneAttributes_Striping_un))


/* DE_LaneAttributes_TrackedVehicle structure */
typedef union _DE_LaneAttributes_TrackedVehicle_un
{
    uint16_t    LaneAttributes_TrackedVehicle_word;
    struct {
        uint8_t LaneAttributes_TrackedVehicle_spec_RevocableLane                :1;
        uint8_t LaneAttributes_TrackedVehicle_spec_commuterRailRoadTrack        :1;
        uint8_t LaneAttributes_TrackedVehicle_spec_lightRailRoadTrack           :1;
        uint8_t LaneAttributes_TrackedVehicle_spec_heavyRailRoadTrack           :1;
        uint8_t LaneAttributes_TrackedVehicle_spec_otherRailType                :1;
        uint8_t LaneAttributes_TrackedVehicle_reserved1                         :3;
        uint8_t LaneAttributes_TrackedVehicle_reserved2                         :8;
    }bit;


}DE_LaneAttributes_TrackedVehicle_un, *DE_LaneAttributes_TrackedVehicle_un_ptr;

#define     DE_LaneAttributes_TrackedVehicle_un_len        (sizeof(DE_LaneAttributes_TrackedVehicle_un))


/* DE_LaneAttributes_Parking  structure */
typedef union _DE_LaneAttributes_Parking_un
{
    uint16_t    LaneAttributes_Parking_word;
    struct {
        uint8_t LaneAttributes_Parking_parkingRevocableLane                  :1;
        uint8_t LaneAttributes_Parking_parallelParkingInUse                  :1;
        uint8_t LaneAttributes_Parking_headInParkingInUse                    :1;
        uint8_t LaneAttributes_Parking_doNotParkZone                         :1;
        uint8_t LaneAttributes_Parking_parkingForBusUse                      :1;
        uint8_t LaneAttributes_Parking_parkingForTaxiUse                     :1;
        uint8_t LaneAttributes_Parking_noPublicParkingUse                    :1;
        uint8_t LaneAttributes_Parking_reserved1                             :1;
        uint8_t LaneAttributes_Parking_reserved2                             :8;
    }bit;
}DE_LaneAttributes_Parking_un, *DE_LaneAttributes_Parking_un_ptr;

#define     DE_LaneAttributes_Parking_un_len        (sizeof(DE_LaneAttributes_Parking_un))

/*DE_LaneID Type*/
typedef uint8_t                       DE_LaneID_t;

#define DE_LaneID_t_len               (sizeof(DE_LaneID_t))


/* DE_LaneSharing structure */
typedef union _DE_LaneSharing_un
{
    uint16_t       LaneSharing_word;
    struct {
        uint8_t    LaneSharing_overlappingLaneDescriptionProvided        :1;
        uint8_t    LaneSharing_multipleLanesTreatedAsOneLane             :1;
        uint8_t    LaneSharing_otherNonMotorizedTrafficTypes             :1;
        uint8_t    LaneSharing_individualMotorizedVehicleTraffic         :1;
        uint8_t    LaneSharing_busVehicleTraffic                         :1;
        uint8_t    LaneSharing_taxiVehicleTraffic                        :1;
        uint8_t    LaneSharing_pedestriansTraffic                        :1;
        uint8_t    LaneSharing_cyclistVehicleTraffic                     :1;
        uint8_t    LaneSharing_trackedVehicleTraffic                     :1;
        uint8_t    LaneSharing_pedestrianTraffic                         :1;
        uint8_t    LaneSharing_reserved                                  :6;
    }bit;
}DE_LaneSharing_un, * DE_LaneSharing_un_ptr;

#define DE_LaneSharing_un_len        (sizeof(DE_LaneSharing_un))


/*DE_LaneWidth type*/
typedef float                   DE_LaneWidth_t;

#define DE_LaneWidth_t_len      (sizeof(DE_LaneWidth_t))


/*DE_Latitude type*/
typedef double                  DE_Latitude_t; /* LSB = 1/10 micro degree Providing a range of plus-minus 90 degree */

#define DE_Latitude_t_len       (sizeof(DE_Latitude_t))

/*DE_Longitude Type*/
typedef double                  DE_Longitude_t; /*  LSB = 1/10 micro degree Providing a range of plus-minus 180 degrees */

#define DE_Longitude_t_len      (sizeof(DE_Longitude_t))


/* DE_LightState enum */
typedef enum _DE_LightState_en
{
    DE_LightState_unavailable                   = 0,
    DE_LightState_dark                          = 1,
    DE_LightState_stop_Then_Proceed             = 2,
    DE_LightState_stop_And_Remain               = 3,
    DE_LightState_pre_Movement                  = 4,
    DE_LightState_permissive_Movement_Allowed   = 5,
    DE_LightState_protected_Movement_Allowed    = 6,
    DE_LightState_intersection_clearance        = 7,
    DE_LightState_caution_Conflicting_Traffic   = 8
    
}DE_LightState_en, *DE_LightState_en_ptr;

#define DE_LightState_en_len        (sizeof(DE_LightState_en))


/*DE_MinuteOfTheYear Type*/
typedef uint32_t                   DE_MinuteOfTheYear_t;

#define DE_MinuteOfTheYear_t_len   (sizeof(DE_MinuteOfTheYear_t))


/*DE_MsgCount type */
typedef uint8_t                    DE_MsgCount_t; /* start from 0 to 127,when =127, then next is 0 again*/

#define DE_MsgCount_t_len          (sizeof(DE_MsgCount_t))


/*DE_NodeID type*/
typedef uint16_t                   DE_NodeID_t;

#define DE_NodeID_t_len            (sizeof(DE_NodeID_t))


/*DE_OffsetLL_B12 Type*/
typedef double                     DE_OffsetLL_B12_t;

#define DE_OffsetLL_B12_t_len      (sizeof(DE_OffsetLL_B12_t))


/*DE_OffsetLL_B14 Type*/
typedef double                     DE_OffsetLL_B14_t;

#define DE_OffsetLL_B14_t_len      (sizeof(DE_OffsetLL_B14_t))


/*DE_OffsetLL_B16 Type */
typedef double                     DE_OffsetLL_B16_t;

#define DE_OffsetLL_B16_t_len      (sizeof(DE_OffsetLL_B16_t))


/*DE_OffsetLL_B18 Type*/
typedef double                     DE_OffsetLL_B18_t;

#define DE_OffsetLL_B18_t_len      (sizeof(DE_OffsetLL_B18_t))


/*DE_OffsetLL_B22 Type*/
typedef double                     DE_OffsetLL_B22_t;

#define DE_OffsetLL_B22_t_len      (sizeof(DE_OffsetLL_B22_t))


/*DE_OffsetLL_B24 Type*/
typedef double                     DE_OffsetLL_B24_t;

#define DE_OffsetLL_B24_t_len      (sizeof(DE_OffsetLL_B24_t))


/*DE_ParticipantType. */
typedef enum _DE_ParticipantType_en
{
    DE_ParticipantType_unknown      = 0,
    DE_ParticipantType_motor        = 1, 
    DE_ParticipantType_non_motor    = 2, 
    DE_ParticipantType_pedestrian   = 3,
    DE_ParticipantType_rsu          = 4  
    
}DE_ParticipantType_en, DE_ParticipantType_en_ptr;

#define DE_ParticipantType_en_len    sizeof(DE_ParticipantType_en);


/*DE_PhaseID type*/
typedef uint8_t                    DE_PhaseID_t;

#define DE_PhaseID_t_len           (sizeof(DE_PhaseID_t))


/*DE_PositionConfidence. */
typedef enum _DE_PositionConfidence_en
{
    PositionConf_unavailable = 0,
    PositionConf_a500m       = 1,
    PositionConf_a200m       = 2,
    PositionConf_a100m       = 3,
    PositionConf_a50m        = 4,
    PositionConf_a20m        = 5,
    PositionConf_a10m        = 6,
    PositionConf_a5m         = 7,
    PositionConf_a2m         = 8,
    PositionConf_a1m         = 9,
    PositionConf_a50cm       = 10,
    PositionConf_a20cm       = 11,
    PositionConf_a10cm       = 12,
    PositionConf_a5cm        = 13,
    PositionConf_a2cm        = 14,
    PositionConf_a1cm        = 15
    
}DE_PositionConfidence_en, * DE_PositionConfidence_en_ptr;

#define DE_PositionConfidence_en_len    (sizeof(_DE_PositionConfidence_en))


/*DE_PositionOffsetLL enum */
typedef enum _DE_PositionOffsetLL_en
{
    PositionOffsetLL_NOTHING = 0,    /* No components present */
    PositionOffsetLL_position_LL1,
    PositionOffsetLL_position_LL2,
    PositionOffsetLL_position_LL3,
    PositionOffsetLL_position_LL4,
    PositionOffsetLL_position_LL5,
    PositionOffsetLL_position_LL6,
    PositionOffsetLL_position_LatLon
    
}DE_PositionOffsetLL_en, *PositionOffsetLL_en_ptr;

#define PositionOffsetLL_en_len  (sizeof(PositionOffsetLL_en))


/*DE_Priority. */
#define DE_Priority_BUFSIZE   1

typedef uint8_t               DE_Priority_st[DE_Priority_BUFSIZE];
typedef DE_Priority_st *      DE_Priority_st_ptr;
#define DE_Priority_st_len    (sizeof(DE_Priority_st))


/*DE_PTCID type*/
typedef uint16_t                     DE_PTCID_t;

#define DE_PTCID_t_len               (sizeof(DE_PTCID_t))


/*DE_RadiusOfCurvature Type*/
typedef float                        DE_RadiusOfCurvature_t;

#define DE_RadiusOfCurvature_t_len   (sizeof(DE_RadiusOfCurvature_t))


/*DE_RoadRegulatorID type*/
typedef uint16_t                     DE_RoadRegulatorID_t;

#define DE_RoadRegulatorID_t_len     (sizeof(DE_RoadRegulatorID_t))

/*DE_RDIID Type*/
typedef uint8_t                      DE_RSIID_t;

#define DE_RSIID_t_len               (sizeof(DE_RSIID_t))


/*DE_RTCM_Revision. */
typedef enum _DE_RTCM_Revision_en
{
    RTCMRevision_unknown    = 0,
    RTCMRevision_rtcmRev2   = 1,  /* Std 10402.x et al. */
    RTCMRevision_rtcmRev3   = 2,  /* Std 10403.x et al. */
    RTCMRevision_reserved   = 3
    
}DE_RTCM_Revision_en, * DE_RTCM_Revision_en_ptr;

#define DE_RTCM_Revision_en_len    (sizeof(DE_RTCM_Revision_en))


/*DF_RTCMmessage */
#define DE_RTCMmessage_BUFSIZE_MIN    1
#define DE_RTCMmessage_BUFSIZE_MAX    1023

typedef struct DE_RTCMmessage_st
{
    uint16_t                          length;
    uint8_t data[DE_RTCMmessage_BUFSIZE_MAX];

}DE_RTCMmessage_st, * DE_RTCMmessage_st_ptr;

#define DE_RTCMmessage_st_len    (sizeof(DE_RTCMmessage_st))


/*DE_SourceType */
typedef enum _DE_SourceType_en
{
    sourceType_unknown            = 0,
    sourceType_dsrc               = 1, 
    sourceType_video              = 2, 
    sourceType_radar              = 3,
    sourceType_loop               = 4
    
}DE_SourceType_en, *DE_SourceType_en_ptr;

#define DE_SourceType_en_len sizeof(DE_SourceType_en);


/*DE_Speed Type*/
typedef float DE_Speed_t, * DE_Speed_t_ptr;


/*DE_SpeedConfidence. */
typedef enum _DE_SpeedConfidence_en
{
    SpeedConf_unavailable    = 0,
    SpeedConf_prec100ms      = 1,
    SpeedConf_prec10ms       = 2,
    SpeedConf_prec5ms        = 3,
    SpeedConf_prec1ms        = 4,
    SpeedConf_prec0_1ms      = 5,
    SpeedConf_prec0_05ms     = 6,
    SpeedConf_prec0_01ms     = 7
    
}DE_SpeedConfidence_en, * DE_SpeedConfidence_en_ptr;

#define DE_SpeedConfidence_en_len    (sizeof(DE_SpeedConfidence_en))


/*DE_StabilityControlStatus. */
typedef enum _DE_StabilityControlStatus_en
{
    stability_status_unavailable = 0,
    stability_status_off         = 1,
    stability_status_on          = 2,
    stability_status_engaged     = 3

}DE_StabilityControlStatus_en, * DE_StabilityControlStatus_en_ptr;

#define DE_StabilityControlStatus_en_len    (sizeof(DE_StabilityControlStatus_en))


/*DE_SteeringWheelAngle Type*/
typedef float                            DE_SteeringWheelAngle_t;

#define DE_SteeringWheelAngle_t_len      (sizeof(DE_SteeringWheelAngle_t))


/*DE_StabilityControlStatus. */
typedef enum _DE_SteeringWheelAngleConfidence_en
{
    steerwheel_angle_confidence_unavailable = 0,
    steerwheel_angle_confidence_prec2deg    = 1,
    steerwheel_angle_confidence_prec1deg    = 2,
    steerwheel_angle_confidence_prec0_02deg = 3

}DE_SteeringWheelAngleConfidence_en, * DE_SteeringWheelAngleConfidence_en_ptr;

#define DE_DE_SteeringWheelAngleConfidence_en_len    (sizeof(DE_SteeringWheelAngleConfidence_en))


/*DE_TemporaryID. */
#define DE_TemporaryID_BUFSIZE    8

typedef uint8_t                   DE_TemporaryID_st[DE_TemporaryID_BUFSIZE];
typedef DE_TemporaryID_st *       DE_TemporaryID_st_ptr;
#define DE_TemporaryID_st_len     (sizeof(DE_TemporaryID_st))


/*DE_PlateNo. */
#define DE_PlateNo_MIN_BUFSIZE        4
#define DE_PlateNo_MAX_BUFSIZE        16

typedef struct _DE_PlateNo_st
{
    uint8_t        bufsize;
    uint8_t     buf[DE_PlateNo_MAX_BUFSIZE];
}DE_PlateNo_st, *DE_PlateNo_st_ptr;

#define DE_PlateNo_st_len    (sizeof(DE_PlateNo_st))


/*DE_Radius Type. Unit: m. */
typedef float                  DE_Radius_t;

#define DE_Radius_t_len        (sizeof(DE_Radius_t))


/*DE_SpeedLimitType enum */
typedef enum _DE_SpeedLimitType_en
{
    DE_SpeedLimitType_en_unkonwn                                      = 0,
    DE_SpeedLimitType_en_maxSpeedInSchoolZone                         = 1,
    DE_SpeedLimitType_en_maxSpeedInSchoolZoneWhenChildrenArePresent   = 2,
    DE_SpeedLimitType_en_maxSpeedInConstructionZone                   = 3,
    DE_SpeedLimitType_en_vehicleMinSpeed                              = 4,
    DE_SpeedLimitType_en_vehicleMaxSpeed                              = 5,
    DE_SpeedLimitType_en_vehicleNightMaxSpeed                         = 6,
    DE_SpeedLimitType_en_trunkMaxSpeed                                = 7,
    DE_SpeedLimitType_en_trunkMinSpeed                                = 8,
    DE_SpeedLimitType_en_trunknightMaxSpeed                           = 9,
    DE_SpeedLimitType_en_vehiclesWithTrailersMinSpeed                 = 10,
    DE_SpeedLimitType_en_vehiclesWithTrailersMaxSpeed                 = 11,
    DE_SpeedLimitType_en_vehiclesWithTrailersNightMaxSpeed            = 12
    
}DE_SpeedLimitType_en, * DE_SpeedLimitType_en_ptr;

#define DE_SpeedLimitType_en_len    (sizeof(DE_SpeedLimitType_en))


/*DE_ThrottleConfidence. */
typedef enum _DE_ThrottleConfidence_en
{
    ThrottleConf_unavailable      = 0,
    ThrottleConf_prec10percent    = 1,
    ThrottleConf_prec1percent     = 2,
    ThrottleConf_prec0_5percent   = 3
    
}DE_ThrottleConfidence_en, * DE_ThrottleConfidence_en_ptr;

#define DE_ThrottleConfidence_en_len    (sizeof(DE_ThrottleConfidence_en))


/*DE_TimeConfidence. */
typedef enum _DE_TimeConfidence_en
{
    TimeConf_unavailable    = 0,
    TimeConf_time_100_000    = 1,
    TimeConf_time_050_000    = 2,
    TimeConf_time_020_000    = 3,
    TimeConf_time_010_000    = 4,
    TimeConf_time_002_000    = 5,
    TimeConf_time_001_000    = 6,
    TimeConf_time_000_500    = 7,
    TimeConf_time_000_200    = 8,
    TimeConf_time_000_100    = 9,
    TimeConf_time_000_050    = 10,
    TimeConf_time_000_020    = 11,
    TimeConf_time_000_010    = 12,
    TimeConf_time_000_005    = 13,
    TimeConf_time_000_002    = 14,
    TimeConf_time_000_001    = 15,
    TimeConf_time_000_000_5    = 16,
    TimeConf_time_000_000_2    = 17,
    TimeConf_time_000_000_1    = 18,
    TimeConf_time_000_000_05    = 19,
    TimeConf_time_000_000_02    = 20,
    TimeConf_time_000_000_01    = 21,
    TimeConf_time_000_000_005    = 22,
    TimeConf_time_000_000_002    = 23,
    TimeConf_time_000_000_001    = 24,
    TimeConf_time_000_000_000_5    = 25,
    TimeConf_time_000_000_000_2    = 26,
    TimeConf_time_000_000_000_1    = 27,
    TimeConf_time_000_000_000_05    = 28,
    TimeConf_time_000_000_000_02    = 29,
    TimeConf_time_000_000_000_01    = 30,
    TimeConf_time_000_000_000_005    = 31,
    TimeConf_time_000_000_000_002    = 32,
    TimeConf_time_000_000_000_001    = 33,
    TimeConf_time_000_000_000_000_5    = 34,
    TimeConf_time_000_000_000_000_2    = 35,
    TimeConf_time_000_000_000_000_1    = 36,
    TimeConf_time_000_000_000_000_05    = 37,
    TimeConf_time_000_000_000_000_02    = 38,
    TimeConf_time_000_000_000_000_01    = 39

}DE_TimeConfidence_en, * DE_TimeConfidence_en_ptr;

#define DE_TimeConfidence_en_len    (sizeof(DE_TimeConfidence_en))


/*DE_TimeOffset. Unit: second. */
typedef float DE_TimeOffset_t, * DE_TimeOffset_t_ptr;


/*DE_TractionControlStatus. */
typedef enum _DE_TractionControlStatus_en
{
    traction_status_unavailable = 0,
    traction_status_off         = 1,
    traction_status_on          = 2,
    traction_status_engaged     = 3

}DE_TractionControlStatus_en, DE_TractionControlStatus_en_ptr;

#define DE_TractionControlStatus_en_len    (sizeof(DE_TractionControlStatus_en))


/*DE_TimeMark Type*/
typedef float                     DE_TimeMark_t;

#define DE_TimeMark_t_len         (sizeof(DE_TimeMark_t))


/*DE_TransmissionState. */
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


/*DE_VehicleEventID_en. */
typedef enum _DE_VehicleEventID_en
{
    EVTID_HazardLights = 0,
    EVTID_StopLineViolation,
    EVTID_ABSactivated,
    EVTID_TractionControlLoss,
    EVTID_StabilityControlactivated,
    EVTID_HazardousMaterials,
    EVTID_Reserved1,
    EVTID_HardBraking,
    EVTID_LightsChanged,
    EVTID_WipersChanged,
    EVTID_FlatTire,
    EVTID_DisabledVehicle,
    EVTID_AirBagDeployment
    
}DE_VehicleEventID_en, * DE_VehicleEventID_en_ptr;

#define DE_VehicleEventID_en_len    (sizeof(DE_VehicleEventID_en))


/*DE_VehicleEventFlags union. */
typedef union _DE_VehicleEventFlags_un
{
    uint16_t     VehicleEventFlags_word;

    struct{
        uint16_t VehicleEventFlags_eventHazardLights              :1;
        uint16_t VehicleEventFlags_eventStopLineViolation         :1;
        uint16_t VehicleEventFlags_eventABSactivated              :1;
        uint16_t VehicleEventFlags_eventTractionControlLoss       :1;
        uint16_t VehicleEventFlags_eventStabilityControlactivated :1;
        uint16_t VehicleEventFlags_eventHazardousMaterials        :1;
        uint16_t VehicleEventFlags_eventReserved1                 :1;
        uint16_t VehicleEventFlags_eventHardBraking               :1;
        uint16_t VehicleEventFlags_eventLightsChanged             :1;
        uint16_t VehicleEventFlags_eventWipersChanged             :1;
        uint16_t VehicleEventFlags_eventFlatTire                  :1;
        uint16_t VehicleEventFlags_eventDisabledVehicle           :1;
        uint16_t VehicleEventFlags_eventAirBagDeployment          :1;
        uint16_t VehicleEventFlags_reserved                       :3;
    }bit;
    
}DE_VehicleEventFlags_un, * DE_VehicleEventFlags_un_ptr;

#define DE_VehicleEventFlags_un_len    (sizeof(DE_VehicleEventFlags_un))


/*DE_VehicleHeight Type*/
typedef float                     DE_VehicleHeight_t; /* 0~6.35 meters*/

#define DE_VehicleHeight_t_len    (sizeof(DE_VehicleHeight_t))


/*DE_VehicleLength Type*/
typedef float                     DE_VehicleLength_t; /* 1 to 40 meters*/

#define DE_VehicleLength_t_len    (sizeof(DE_VehicleLength_t))


/*DE_VehicleWidth Type*/
typedef float                     DE_VehicleWidth_t; /* 1 to 10 meters*/

#define DE_VehicleWidth_t_len     (sizeof(DE_VehicleWidth_t))


/*DE_VerticalAcceleration Type*/
typedef float                            DE_VerticalAcceleration_t;


#define DE_VerticalAcceleration_t_len    (sizeof(DE_VerticalAcceleration_t))

/*DE_VertOffset_B07 Type*/
typedef float                            DE_VertOffset_B07_t;

#define DE_VertOffset_B07_t_len          (sizeof(DE_VertOffset_B07_t))


/*DE_VertOffset_B08 Type*/
typedef float                            DE_VertOffset_B08_t;

#define DE_VertOffset_B08_t_len          (sizeof(DE_VertOffset_B08_t))


/*DE_VertOffset_B09 Type*/
typedef float                            DE_VertOffset_B09_t;

#define DE_VertOffset_B09_t_len          (sizeof(DE_VertOffset_B09_t))


/*DE_VertOffset_B10 Type*/
typedef float                            DE_VertOffset_B10_t;

#define DE_VertOffset_B10_t_len          (sizeof(DE_VertOffset_B10_t))


/*DE_VertOffset_B11 Type*/
typedef float                            DE_VertOffset_B11_t;

#define DE_VertOffset_B11_t_len          (sizeof(DE_VertOffset_B11_t))


/*DE_VertOffset_B12 Type*/
typedef float                            DE_VertOffset_B12_t;

#define DE_VertOffset_B12_t_len          (sizeof(DE_VertOffset_B12_t))


/*DE_VerticalOffset enum */
typedef enum _DE_VerticalOffset_en
{
    VerticalOffset_NOTHING  = 0,    /* No components present */
    VerticalOffset_offset1,
    VerticalOffset_offset2,
    VerticalOffset_offset3,
    VerticalOffset_offset4,
    VerticalOffset_offset5,
    VerticalOffset_offset6,
    VerticalOffset_elevation
    
}DE_VerticalOffset_en, *DE_VerticalOffset__ptr;


/*DE_YawRate Type*/
typedef float                   DE_YawRate_t;
#define DE_YawRate_t_len        (sizeof(DE_YawRate_t))


/*Basic type definition. */
typedef double innertyp_t, *innertyp_t_ptr;
typedef long   outertyp_t, *outertyp_t_ptr;


/* Basic type transform structure. */
typedef struct _bsctyp_transf_st
{
    /* value max min and error for inner data. */
    innertyp_t     value_max;
    innertyp_t     value_min;
    innertyp_t     value_err;

    /* Factor for inner value to outer value. */
    innertyp_t encode_factor; 

    /* Factor for outer value to inner value. */
    innertyp_t decode_factor;

}bsctyp_transf_st, * bsctyp_trans_st_ptr;

#define BSCTYP_TRANSF_ST_LEN    (sizeof(bsctyp_transf_st))


/*Basic type definition. */
typedef enum _bsctyp_def_en
{
    DE_Acceleration = 0,
        
    DE_CoarseHeading,
    DE_Confidence,
    
    DE_DDay,
    DE_DHour,
    DE_DMinute,
    DE_DMonth,
    DE_DOffset,
    DE_DSecond,
    DE_DYear,
    
    DE_Elevation,

    DE_LaneID,
    DE_LaneWidth,
    DE_Latitude,
    DE_Longitude,

    DE_Heading,
    
    DE_MinuteOfTheYear,
    DE_MsgCount,

    DE_Offset_B09,
    DE_Offset_B10,
    DE_Offset_B12,
    DE_OffsetLL_B12,
    DE_OffsetLL_B14,
    DE_OffsetLL_B16,
    DE_OffsetLL_B18,
    DE_OffsetLL_B22,
    DE_OffsetLL_B24,
    
    DE_PhaseID,
 
    DE_RadiusOfCurvature,
    DE_Radius,
    DE_RoadRegulatorID,
    
    DE_SemiMajorAxisAccuracy,
    DE_SemiMajorAxisOrientation,
    DE_SemiMinorAxisAccuracy,
    DE_Speed,
    DE_SteeringWheelAngle,
    
    DE_TimeMark,
    DE_TimeOffset,

    DE_VehicleHeight,
    DE_VehicleLength,
    DE_VehicleWidth,
    DE_Velocity,
    DE_VerticalAcceleration,
    DE_VertOffset_B07,
    DE_VertOffset_B08,
    DE_VertOffset_B09,
    DE_VertOffset_B10,
    DE_VertOffset_B11,
    DE_VertOffset_B12,
    
    
    DE_YawRate,
    
}bsctyp_def_en, * bsctyp_def_en_ptr;


/* Encode basic type data and return the outer type data. */
extern outertyp_t bsctyp_encode(bsctyp_def_en type, innertyp_t inner_data);

/* Decode basic type data and return the inner type data. */
extern innertyp_t bsctyp_decode(bsctyp_def_en type, outertyp_t outer_data);


#endif

