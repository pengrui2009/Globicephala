/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_data_element.h
 @brief  : Data Element routine set.
 @author : wangxianwen
 @history:
           2016-09-20    wangxianwen    Created file
           2017-10-12     pengrui        Modify for CSAE
           ...
******************************************************************************/
#ifndef __CV_DATA_ELEMENT_H__
#define __CV_DATA_ELEMENT_H__

#include "asn_internal.h"
#include "BrakeAppliedStatus.h"
#include "ExteriorLights.h"
#include "GNSSstatus.h"
#include "Priority.h"
#include "VehicleEventFlags.h"
#include "DescriptiveName.h"
#include "IntersectionStatusObject.h"
#include "AllowedManeuvers.h"
#include "LaneAttributes-Vehicle.h"
#include "LaneAttributes-Crosswalk.h"
#include "LaneAttributes-Bike.h"
#include "LaneAttributes-Sidewalk.h"
#include "LaneAttributes-Barrier.h"
#include "LaneAttributes-Striping.h"
#include "LaneAttributes-TrackedVehicle.h"
#include "LaneAttributes-Parking.h"
#include "LaneSharing.h"

#include "prot_element.h"
#include "error.h"


/* Free routine for DE_AllowedManeuvers. */
extern int DE_AllowedManeuvers_free(AllowedManeuvers_t *maneuvers_ptr);

/* Allocate routine for DE_AllowedManeuvers. */
extern int DE_AllowedManeuvers_allocate(AllowedManeuvers_t *maneuvers_ptr, DE_AllowedManeuvers_un_ptr Maneuvers_ptr);

/* Parse routine for DE_AllowedManeuvers. */
extern int DE_AllowedManeuvers_parse(AllowedManeuvers_t *maneuvers_ptr, DE_AllowedManeuvers_un_ptr Maneuvers_ptr);

/* Free routine for DE_BrakeAppliedStatus. */
extern int DE_BrakeAppliedStatus_free(BrakeAppliedStatus_t *wheelBrakes_ptr);

/* Allocate routine for DE_BrakeAppliedStatus. */
extern int DE_BrakeAppliedStatus_allocate(BrakeAppliedStatus_t *wheelBrakes_ptr, DE_BrakeAppliedStatus_st_ptr WheelBrakes_ptr);

/* parse routine for DE_BrakeAppliedStatus. */
extern int DE_BrakeAppliedStatus_parse(BrakeAppliedStatus_t *wheelBrakes_ptr, DE_BrakeAppliedStatus_st_ptr WheelBrakes_ptr);

/* free routine of DE_DescriptiveName */
extern int DE_DescriptiveName_free(DescriptiveName_t *name_ptr);

/* allocate routine for DE_DescriptiveName */
extern int DE_DescriptiveName_allocate(DescriptiveName_t *name_ptr, DE_DescriptiveName_st_ptr Name_ptr);

/* parse routine of DE_DescriptiveName */
extern int DE_DescriptiveName_parse(DescriptiveName_t *name_ptr, DE_DescriptiveName_st_ptr Name_ptr);

/* Free routine for DE_ExteriorLights. */
extern int DE_ExteriorLights_free(ExteriorLights_t *lights_ptr);

/* Allocate routine for DE_ExteriorLights. */
extern int DE_ExteriorLights_allocate(ExteriorLights_t *lights_ptr, DE_ExteriorLights_un_ptr Lights_ptr);

/* Parse routine for DE_ExteriorLights. */
extern int DE_ExteriorLights_parse(ExteriorLights_t *lights_ptr, DE_ExteriorLights_un_ptr Lights_ptr);

/* Free routine for DE_GNSSstatus. */
extern int DE_GNSSstatus_free(GNSSstatus_t *status_ptr);

/* Allocate routine for DE_GNSSstatus. */
extern int DE_GNSSstatus_allocate(GNSSstatus_t *status_ptr, DE_GNSSstatus_st_ptr Status_ptr);

/* Parse routine for DE_GNSSstatus. */
extern int DE_GNSSstatus_parse(GNSSstatus_t *status_ptr, DE_GNSSstatus_st_ptr Status_ptr);

/* Free routine for DE_IntersectionStatusObject. */
extern int DE_IntersectionStatusObject_free(IntersectionStatusObject_t *status_ptr);

/* Allocate routine for DE_IntersectionStatusObject. */
extern int DE_IntersectionStatusObject_allocate(IntersectionStatusObject_t *status_ptr, DE_IntersectionStatusObject_un_ptr Status_ptr);

/* Parse routine for DE_IntersectionStatusObject. */
extern int DE_IntersectionStatusObject_parse(IntersectionStatusObject_t *status_ptr, DE_IntersectionStatusObject_un_ptr Status_ptr);

/* Free routine for DE_LaneAttributes_Vehicle. */
extern int DE_LaneAttributes_Vehicle_free(LaneAttributes_Vehicle_t *vehicle_ptr);

/* Allocate routine for DE_LaneAttributes_Vehicle. */
extern int DE_LaneAttributes_Vehicle_allocate(LaneAttributes_Vehicle_t *vehicle_ptr, DE_LaneAttributes_Vehicle_un_ptr Vehicle_ptr);

/* Parse routine for DE_LaneAttributes_Vehicle. */
extern int DE_LaneAttributes_Vehicle_parse(LaneAttributes_Vehicle_t *vehicle_ptr, DE_LaneAttributes_Vehicle_un_ptr Vehicle_ptr);

/* Free routine for DE_LaneAttributes_Crosswalk. */
extern int DE_LaneAttributes_Crosswalk_free(LaneAttributes_Crosswalk_t *crosswalk_ptr);

/* Allocate routine for DE_LaneAttributes_Crosswalk. */
extern int DE_LaneAttributes_Crosswalk_allocate(LaneAttributes_Crosswalk_t *crosswalk_ptr, DE_LaneAttributes_Crosswalk_un_ptr Crosswalk_ptr);

/* Parse routine for DE_LaneAttributes_Crosswalk. */
extern int DE_LaneAttributes_Crosswalk_parse(LaneAttributes_Crosswalk_t *crosswalk_ptr, DE_LaneAttributes_Crosswalk_un_ptr Crosswalk_ptr);

/* Free routine for DE_LaneAttributes_Bike. */
extern int DE_LaneAttributes_Bike_free(LaneAttributes_Bike_t *bike_ptr);

/* Allocate routine for DE_LaneAttributes_Bike. */
extern int DE_LaneAttributes_Bike_allocate(LaneAttributes_Bike_t *bike_ptr, DE_LaneAttributes_Bike_un_ptr Bike_ptr);

/* Parse routine for DE_LaneAttributes_Bike. */
extern int DE_LaneAttributes_Bike_parse(LaneAttributes_Bike_t *bike_ptr, DE_LaneAttributes_Bike_un_ptr Bike_ptr);

/* Free routine for DE_LaneAttributes_Sidewalk. */
extern int DE_LaneAttributes_Sidewalk_free(LaneAttributes_Sidewalk_t *sidewalk_ptr);

/* Allocate routine for DE_LaneAttributes_Sidewalk. */
extern int DE_LaneAttributes_Sidewalk_allocate(LaneAttributes_Sidewalk_t *sidewalk_ptr, DE_LaneAttributes_Sidewalk_un_ptr Sidewalk_ptr);

/* Parse routine for DE_LaneAttributes_Sidewalk. */
extern int DE_LaneAttributes_Sidewalk_parse(LaneAttributes_Sidewalk_t *sidewalk_ptr, DE_LaneAttributes_Sidewalk_un_ptr Sidewalk_ptr);

/* Free routine for DE_LaneAttributes_Barrier. */
extern int DE_LaneAttributes_Barrier_free(LaneAttributes_Barrier_t *barrier_ptr);

/* Allocate routine for DE_LaneAttributes_Barrier. */
extern int DE_LaneAttributes_Barrier_allocate(LaneAttributes_Barrier_t *barrier_ptr, DE_LaneAttributes_Barrier_un_ptr Barrier_ptr);

/* Parse routine for DE_LaneAttributes_Barrier. */
extern int DE_LaneAttributes_Barrier_parse(LaneAttributes_Barrier_t *barrier_ptr, DE_LaneAttributes_Barrier_un_ptr Barrier_ptr);

/* Free routine for DE_LaneAttributes_Striping. */
extern int DE_LaneAttributes_Striping_free(LaneAttributes_Striping_t *striping_ptr);

/* Allocate routine for DE_LaneAttributes_Striping. */
extern int DE_LaneAttributes_Striping_allocate(LaneAttributes_Striping_t *striping_ptr, DE_LaneAttributes_Striping_un_ptr Striping_ptr);

/* Parse routine for DE_LaneAttributes_Striping. */
extern int DE_LaneAttributes_Striping_parse(LaneAttributes_Striping_t *striping_ptr, DE_LaneAttributes_Striping_un_ptr Striping_ptr);

/* Free routine for DE_LaneAttributes_TrackedVehicle. */
extern int DE_LaneAttributes_TrackedVehicle_free(LaneAttributes_TrackedVehicle_t *trackedVehicle_ptr);

/* Allocate routine for DE_LaneAttributes_TrackedVehicle. */
extern int DE_LaneAttributes_TrackedVehicle_allocate(LaneAttributes_TrackedVehicle_t *trackedVehicle_ptr, DE_LaneAttributes_TrackedVehicle_un_ptr TrackedVehicle_ptr);

/* Parse routine for DE_LaneAttributes_TrackedVehicle. */
extern int DE_LaneAttributes_TrackedVehicle_parse(LaneAttributes_TrackedVehicle_t *trackedVehicle_ptr, DE_LaneAttributes_TrackedVehicle_un_ptr TrackedVehicle_ptr);

/* Free routine for DE_LaneAttributes_Parking. */
extern int DE_LaneAttributes_Parking_free(LaneAttributes_Parking_t *parking_ptr);

/* Allocate routine for DE_LaneAttributes_Parking. */
extern int DE_LaneAttributes_Parking_allocate(LaneAttributes_Parking_t *parking_ptr, DE_LaneAttributes_Parking_un_ptr Parking_ptr);

/* Parse routine for DE_LaneAttributes_Parking. */
extern int DE_LaneAttributes_Parking_parse(LaneAttributes_Parking_t *parking_ptr, DE_LaneAttributes_Parking_un_ptr Parking_ptr);

/* Free routine for DE_LaneSharing. */
extern int DE_LaneSharing_free(LaneSharing_t *sharing_ptr);

/* Allocate routine for DE_LaneSharing. */
extern int DE_LaneSharing_allocate(LaneSharing_t *sharing_ptr, DE_LaneSharing_un_ptr Sharing_ptr);

/* Parse routine for DE_LaneSharing. */
extern int DE_LaneSharing_parse(LaneSharing_t *sharing_ptr, DE_LaneSharing_un_ptr Sharing_ptr);

#if 0
/* Free routine for DE_HeadingSlice. */
extern int DE_HeadingSlice_free(HeadingSlice_t *heading_ptr);

/* Allocate routine for DE_HeadingSlice. */
extern int DE_HeadingSlice_allocate(HeadingSlice_t *heading_ptr, DE_HeadingSlice_st_ptr Heading_ptr);

/* Parse routine for DE_HeadingSlice. */
extern int DE_HeadingSlice_parse(HeadingSlice_t *heading_ptr, DE_HeadingSlice_st_ptr Heading_ptr);
#endif


/* free routine of DE_plateNo */
extern int DE_PlateNo_free(OCTET_STRING_t *plateNo_ptr);

/* allocate routine for DE_plateNo */
extern int DE_PlateNo_allocate(OCTET_STRING_t *plateNo_ptr, DE_PlateNo_st_ptr PlateNo_ptr);

/* parse routine of DE_plateNo */
extern int DE_PlateNo_parse(OCTET_STRING_t *plateNo_ptr, DE_PlateNo_st_ptr PlateNo_ptr);

/* Free routine for DE_Priority. */
extern int DE_Priority_free(Priority_t *pri_ptr);

/* Allocate routine for DE_Priority. */
extern int DE_Priority_allocate(Priority_t *pri_ptr, DE_Priority_st_ptr Pri_ptr);

/* Parse routine for DE_Priority. */
extern int DE_Priority_parse(Priority_t *pri_ptr, DE_Priority_st_ptr Pri_ptr);

#if 0
/* Free routine for DE_RTCMmessage. */
extern int DE_RTCMmessage_free(RTCMmessage_t *msg_ptr);

/* Free extention routine for DE_RTCMmessage. Diff with DE_RTCMmessage_free() and this will free the point itself. */
extern int DE_RTCMmessage_freeExt(RTCMmessage_t *msg_ptr);

/* Allocate routine for DE_RTCMmessage. */
extern int DE_RTCMmessage_allocate(RTCMmessage_t *msg_ptr, DE_RTCMmessage_st_ptr Msg_ptr);

/* Parse routine for DE_RTCMmessage. */
extern int DE_RTCMmessage_parse(RTCMmessage_t *msg_ptr, DE_RTCMmessage_st_ptr Msg_ptr);
#endif
/* Free routine for DE_TemporaryID. */
extern int DE_TemporaryID_free(OCTET_STRING_t *id_ptr);

/* Allocate routine for DE_TemporaryID. */
extern int DE_TemporaryID_allocate(OCTET_STRING_t *id_ptr, DE_TemporaryID_st_ptr ID_ptr);

/* Parse routine for DE_TemporaryID. */
extern int DE_TemporaryID_parse(OCTET_STRING_t *id_ptr, DE_TemporaryID_st_ptr ID_ptr);

/* Free routine for DE_VehicleEventFlags. */
extern int DE_VehicleEventFlags_free(VehicleEventFlags_t *event_ptr);

/* Allocate routine for DE_VehicleEventFlags. */
extern int DE_VehicleEventFlags_allocate(VehicleEventFlags_t *event_ptr, DE_VehicleEventFlags_un_ptr Event_ptr);

/* Parse routine for DE_VehicleEventFlags. */
extern int DE_VehicleEventFlags_parse(VehicleEventFlags_t *event_ptr, DE_VehicleEventFlags_un_ptr Event_ptr);


#endif

