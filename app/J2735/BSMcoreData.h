/*
 * Generated by asn1c-0.9.27 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "J2735_WXW.ASN"
 */

#ifndef	_BSMcoreData_H_
#define	_BSMcoreData_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MsgCount.h"
#include "TemporaryID.h"
#include "DSecond.h"
#include "Latitude.h"
#include "Longitude.h"
#include "Elevation.h"
#include "PositionalAccuracy.h"
#include "TransmissionState.h"
#include "Speed.h"
#include "Heading.h"
#include "SteeringWheelAngle.h"
#include "AccelerationSet4Way.h"
#include "BrakeSystemStatus.h"
#include "VehicleSize.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BSMcoreData */
typedef struct BSMcoreData {
	MsgCount_t	 msgCnt;
	TemporaryID_t	 id;
	DSecond_t	 secMark;
	Latitude_t	 lat;
	Longitude_t	 Long;
	Elevation_t	 elev;
	PositionalAccuracy_t	 accuracy;
	TransmissionState_t	 transmission;
	Speed_t	 speed;
	Heading_t	 heading;
	SteeringWheelAngle_t	 angle;
	AccelerationSet4Way_t	 accelSet;
	BrakeSystemStatus_t	 brakes;
	VehicleSize_t	 size;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BSMcoreData_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BSMcoreData;

#ifdef __cplusplus
}
#endif

#endif	/* _BSMcoreData_H_ */
#include <asn_internal.h>
