/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_rcp.h
 @brief  : this file include the definitions of  the remote communicate 
           protocol of vehicle
 @author : wangyifeng
 @history:
           2014-6-22    wangyifeng    Created file
           2015-1-20    wanglei       Modified. add dsrc msg
           ...
******************************************************************************/
#ifndef __CV_RCP_H__
#define __CV_RCP_H__

#include "cv_wnet.h"

/*****************************************************************************
 * definition of micro                                                       *
*****************************************************************************/
#define  __COMPILE_INLINE__  inline
/*****************************************************************************
 * definition of structs                                                     *
*****************************************************************************/

/* Part 2 id for message. */
typedef enum _MSG_PART2_ID 
{
    MsgPart2Id_vehicleSafetyExt	        = 0,
	MsgPart2Id_specialVehicleExt	    = 1,
	MsgPart2Id_supplementalVehicleExt	= 2
	
    /* Enumeration is extensible. */
    
} E_MSG_PART2_ID;

/* Data Element: DE_TransmissionState. */
typedef enum _E_TRANSMISSION_STATE 
{
    /* All DER forms are now retired and not to be used. */
    TRANSMISSION_STAT_NEUTRAL	        = 0,
	TRANSMISSION_STAT_PARK	            = 1, 
	TRANSMISSION_STAT_FORWARD_GEARS	    = 2, 
	TRANSMISSION_STAT_REVERSE_GEARS     = 3,
	TRANSMISSION_STAT_RESERVED1	        = 4,
	TRANSMISSION_STAT_RESERVED2	        = 5,
	TRANSMISSION_STAT_RESERVED3         = 6,
	TRANSMISSION_STAT_UNAVAILABLE	    = 7

} E_TRANSMISSION_STATE;


/* Data Element: DE_VehicleType. */
typedef enum _VehicleType 
{
	VehicleType_none	= 0,
	VehicleType_unknown	= 1,
	VehicleType_special	= 2,
	VehicleType_moto	= 3,
	VehicleType_car	    = 4,
	VehicleType_carOther	= 5,
	VehicleType_bus     	= 6,
	VehicleType_axleCnt2	= 7,
	VehicleType_axleCnt3	= 8,
	VehicleType_axleCnt4	= 9,
	VehicleType_axleCnt4Trailer	        = 10,
	VehicleType_axleCnt5Trailer     	= 11,
	VehicleType_axleCnt6Trailer	        = 12,
	VehicleType_axleCnt5MultiTrailer	= 13,
	VehicleType_axleCnt6MultiTrailer	= 14,
	VehicleType_axleCnt7MultiTrailer	= 15
} E_VehicleType;


/* Data Element: DE_ResponseType. */
typedef enum _ResponseType 
{
	ResponseType_notInUseOrNotEquipped	= 0,
	ResponseType_emergency	= 1,
	ResponseType_nonEmergency	= 2,
	ResponseType_pursuit	= 3
} E_ResponseType;


typedef enum _VehicleGroupAffected {
	VehicleGroupAffected_all_vehicles	= 9217,
	VehicleGroupAffected_bicycles	= 9218,
	VehicleGroupAffected_motorcycles	= 9219,
	VehicleGroupAffected_cars	= 9220,
	VehicleGroupAffected_light_vehicles	= 9221,
	VehicleGroupAffected_cars_and_light_vehicles	= 9222,
	VehicleGroupAffected_cars_with_trailers	= 9223,
	VehicleGroupAffected_cars_with_recreational_trailers	= 9224,
	VehicleGroupAffected_vehicles_with_trailers	= 9225,
	VehicleGroupAffected_heavy_vehicles	= 9226,
	VehicleGroupAffected_trucks	= 9227,
	VehicleGroupAffected_buses	= 9228,
	VehicleGroupAffected_articulated_buses	= 9229,
	VehicleGroupAffected_school_buses	= 9230,
	VehicleGroupAffected_vehicles_with_semi_trailers	= 9231,
	VehicleGroupAffected_vehicles_with_double_trailers	= 9232,
	VehicleGroupAffected_high_profile_vehicles	= 9233,
	VehicleGroupAffected_wide_vehicles	= 9234,
	VehicleGroupAffected_long_vehicles	= 9235,
	VehicleGroupAffected_hazardous_loads	= 9236,
	VehicleGroupAffected_exceptional_loads	= 9237,
	VehicleGroupAffected_abnormal_loads	= 9238,
	VehicleGroupAffected_convoys	= 9239,
	VehicleGroupAffected_maintenance_vehicles	= 9240,
	VehicleGroupAffected_delivery_vehicles	= 9241,
	VehicleGroupAffected_vehicles_with_even_numbered_license_plates	= 9242,
	VehicleGroupAffected_vehicles_with_odd_numbered_license_plates	= 9243,
	VehicleGroupAffected_vehicles_with_parking_permits	= 9244,
	VehicleGroupAffected_vehicles_with_catalytic_converters	= 9245,
	VehicleGroupAffected_vehicles_without_catalytic_converters	= 9246,
	VehicleGroupAffected_gas_powered_vehicles	= 9247,
	VehicleGroupAffected_diesel_powered_vehicles	= 9248,
	VehicleGroupAffected_lPG_vehicles	= 9249,
	VehicleGroupAffected_military_convoys	= 9250,
	VehicleGroupAffected_military_vehicles	= 9251
} E_VehicleGroupAffected;

typedef enum _ResponderGroupAffected {
    /* Default phrase, to be used when one of the below does not fit better */
	ResponderGroupAffected_emergency_vehicle_units	        = 9729,
	ResponderGroupAffected_federal_law_enforcement_units	= 9730,
	ResponderGroupAffected_state_police_units	            = 9731,
	ResponderGroupAffected_county_police_units	            = 9732,
	ResponderGroupAffected_local_police_units	            = 9733,
	ResponderGroupAffected_ambulance_units	                = 9734,
	ResponderGroupAffected_rescue_units	                    = 9735,
	ResponderGroupAffected_fire_units                   	= 9736,
	ResponderGroupAffected_hAZMAT_units	                    = 9737,
	ResponderGroupAffected_light_tow_unit               	= 9738,
	ResponderGroupAffected_heavy_tow_unit               	= 9739,
	ResponderGroupAffected_freeway_service_patrols         	= 9740,
	ResponderGroupAffected_transportation_response_units	= 9741,
	ResponderGroupAffected_private_contractor_response_units	= 9742
} E_ResponderGroupAffected;


/* vehicle event flags. */
typedef enum _VehicleEventFlags 
{
    EventHazardLights               = 0x0001,
    EventStopLineViolation          = 0x0002, /* Intersection Violation */  
    EventABSactivated               = 0x0004,
    EventTractionControlLoss        = 0x0008,
    
    EventStabilityControlactivated  = 0x0010,
    EventHazardousMaterials         = 0x0020,    
    EventReserved1                  = 0x0040,
    EventHardBraking                = 0x0080,
    
    EventLightsChanged              = 0x0100,
    EventWipersChanged              = 0x0200,
    EventFlatTire                   = 0x0400,
    EventDisabledVehicle            = 0x0800,
    
    EventAirBagDeployment           = 0x1000
    
} E_VehicleEventFlags;

typedef enum _TransmissionState {
    TRANS_STATE_neutral   = 0, /* Neutral, speed relative to the vehicle alignment */
    TRANS_STATE_park      = 1, /* Park */
    TRANS_STATE_Forward   = 2, /* Forward gears */
    TRANS_STATE_Reverse   = 3, /* Reverse gears */
    TRANS_STATE_reserved1 = 4, 
    TRANS_STATE_reserved2 = 5, 
    TRANS_STATE_reserved3 = 6, 
    TRANS_STATE_unavailable = 7,         
} E_TransmissionState;

/* Status word for some control module. */
typedef enum _STATUS_WORD 
{
    STATUS_WORD_NOT_EQUIPPED = 0,
    STATUS_WORD_OFF          = 1,
    STATUS_WORD_ON           = 2,
    STATUS_WORD_ENGAGED      = 3
    
} E_STATUS_WORD;


typedef uint16_t heading_slice_t;


#endif /* __CV_RCP_H__ */

