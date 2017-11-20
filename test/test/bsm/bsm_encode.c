/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : bsm_test.c
 @brief  : test the bsm msg.
 @author : pengrui
 @history:
           2017-09-08    pengrui    Created file
           ...
******************************************************************************/
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "cv_msg_frame.h"

	
MSG_MessageFrame_st msg_bsm_frame = {0};

int main(int argc, char *argv[])
{
	int i = 0;
	int result = 0;
	uint8_t buffer[BSM_MSG_ST_LEN] = {0};
	uint16_t buffer_size = BSM_MSG_ST_LEN;

	msg_bsm_frame.messageid = MSG_MessageFrame_ID_BSM;
	msg_bsm_frame.msg.msg_bsm.msgCnt = 120;
	
	msg_bsm_frame.msg.msg_bsm.opt.msg_BasicSafetyMessage_opt_plateno = MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.opt.msg_BasicSafetyMessage_opt_angle = MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.opt.msg_BasicSafetyMessage_opt_motioncfd = MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.opt.msg_BasicSafetyMessage_opt_safetyExt = MSG_OPTIONAL_YES;

	msg_bsm_frame.msg.msg_bsm.id[0] = 0x11; 
	msg_bsm_frame.msg.msg_bsm.id[1] = 0x22;
	msg_bsm_frame.msg.msg_bsm.id[2] = 0x33;
	msg_bsm_frame.msg.msg_bsm.id[3] = 0x44;

	msg_bsm_frame.msg.msg_bsm.plateno.bufsize = 8;
	msg_bsm_frame.msg.msg_bsm.plateno.buf[0] = 0xAA;
	msg_bsm_frame.msg.msg_bsm.plateno.buf[1] = 0xBB;
	msg_bsm_frame.msg.msg_bsm.plateno.buf[2] = 0xCC;
	msg_bsm_frame.msg.msg_bsm.plateno.buf[3] = 0xDD;
	msg_bsm_frame.msg.msg_bsm.plateno.buf[4] = 0xEE;
	msg_bsm_frame.msg.msg_bsm.plateno.buf[5] = 0xFF;
	msg_bsm_frame.msg.msg_bsm.plateno.buf[6] = 0x99;
	msg_bsm_frame.msg.msg_bsm.plateno.buf[7] = 0x00;

	msg_bsm_frame.msg.msg_bsm.secMark = 5000;

	msg_bsm_frame.msg.msg_bsm.pos.opt.Position3D_opt_elev = MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.pos.latitude = 80.0000000;
	msg_bsm_frame.msg.msg_bsm.pos.longitude = 90.0000000;
	msg_bsm_frame.msg.msg_bsm.pos.elevation = 12.24;

	msg_bsm_frame.msg.msg_bsm.accuracy.opt.PositionConfidenceSet_opt_confidence_elev = MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.accuracy.pos = 3;
	msg_bsm_frame.msg.msg_bsm.accuracy.elevation = 5;

	msg_bsm_frame.msg.msg_bsm.trans = 7;

	msg_bsm_frame.msg.msg_bsm.speed = 10.6;
	
	msg_bsm_frame.msg.msg_bsm.heading = 5.0;	

	msg_bsm_frame.msg.msg_bsm.angle = 30.000;

	msg_bsm_frame.msg.msg_bsm.motioncfd.opt.MotionConfidenceSet_opt_speedcfd = MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.motioncfd.opt.MotionConfidenceSet_opt_headingcfd = MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.motioncfd.opt.MotionConfidenceSet_opt_steercfd = MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.motioncfd.speedcfd = 6;
	msg_bsm_frame.msg.msg_bsm.motioncfd.headingcfd = 7;
	msg_bsm_frame.msg.msg_bsm.motioncfd.steercfd = 3;

	msg_bsm_frame.msg.msg_bsm.acceSet.longitudinal = 10.000;
	msg_bsm_frame.msg.msg_bsm.acceSet.lateral = 9.000;
	msg_bsm_frame.msg.msg_bsm.acceSet.vertical = 8.000;
	msg_bsm_frame.msg.msg_bsm.acceSet.yaw_rate = 7.000;

	msg_bsm_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_brakepadel =MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_wheelBrakes =MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_traction =MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_abs =MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_scs =MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_brakeBoost =MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_auxBrakes =MSG_OPTIONAL_YES;
	
	msg_bsm_frame.msg.msg_bsm.brakes.brakepedal = 1;
	msg_bsm_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_unavailable = 1;
	msg_bsm_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_leftFront = 1;
	msg_bsm_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_leftRear = 1;
	msg_bsm_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_rightFront = 1;
	msg_bsm_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_rightRear = 1;
	msg_bsm_frame.msg.msg_bsm.brakes.traction = 2;
	msg_bsm_frame.msg.msg_bsm.brakes.abs = 1;
	msg_bsm_frame.msg.msg_bsm.brakes.scs = 3;
	msg_bsm_frame.msg.msg_bsm.brakes.brakeBoost = 2;
	msg_bsm_frame.msg.msg_bsm.brakes.auxBrakes = 0;
	
	msg_bsm_frame.msg.msg_bsm.size.opt.vehiclesize_opt_height =MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.size.width = 2.50;
	msg_bsm_frame.msg.msg_bsm.size.length = 5.00;
	msg_bsm_frame.msg.msg_bsm.size.height = 3.00;
	
	msg_bsm_frame.msg.msg_bsm.vehicleclass.classification = 10;
	
	msg_bsm_frame.msg.msg_bsm.safetyExt.opt.VehicleSafetyExternsions_opt_events = MSG_OPTIONAL_YES;
	msg_bsm_frame.msg.msg_bsm.safetyExt.opt.VehicleSafetyExternsions_opt_lights = MSG_OPTIONAL_YES;
	
	
	msg_bsm_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventHazardLights = 1;
	msg_bsm_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventStopLineViolation = 1;
	msg_bsm_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventLightsChanged = 1;
	msg_bsm_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_lowBeamHeadlightsOn = 1;
	msg_bsm_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_highBeamHeadlightsOn = 1;
	msg_bsm_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_parkingLightsOn = 1;
	while(1)
	{
		result = frame_encode_msg(&msg_bsm_frame, buffer,  buffer_size);
		if(result <= 0)
		{
			goto ERR_EXIT;
		}
		printf("encode ok\n");
		sleep(1);
	}
	for(i=0; i<result; i++)
	{
		printf("%02X ", buffer[i]);
	}
	printf("\n");
	
	//frame_decode_msg(MSG_MessageFrame_st_ptr msg_ptr,uint8_t * buffer_ptr,uint16_t buffer_size);
	return result;

	
ERR_EXIT:
	
	return result;
}

