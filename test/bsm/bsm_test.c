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

	msg_bsm_frame.messageid = DSRCmsgIDEnum_basicSafetyMessage;
	msg_bsm_frame.msg.msg_bsm.msgCnt = 120;
	
	result = frame_encode_msg(&msg_bsm_frame, buffer,  buffer_size);
	if(result <= 0)
	{
		goto ERR_EXIT;
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
