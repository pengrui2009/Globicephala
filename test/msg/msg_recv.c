/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : msg_recv.c
 @brief  : test the api function of osal msg.
 @author : pengrui
 @history:
           2017-10-19    pengrui    Created file
           ...
******************************************************************************/
#include "osal_msg.h"

#define MSG_KEY_NAME    "."
#define MSG_KEY_ID      'A'

int main()
{
	int result = 0;
	osal_msg_t msgfd = 0;

	result = osal_msg_open(&msgfd, MSG_KEY_NAME, MSG_KEY_ID);
	if(result)
	{
		fprintf(stderr, "osal_msg_open() error.\n");
		goto ERR_EXIT;
	}

	while(1)
	{
		char msgbuf[50] = { 0 };
		uint32_t msglen = 50;
		long msgtype = 0;
		result = osal_msg_recv(msgfd, &msgtype, msgbuf, &msglen, OSAL_WAITING_FOREVER);
		if(result)
		{
			fprintf(stderr, "osal_msg_recv() error\n");
		}

		printf("Recv msg{type:%ld len:%d buf:%s}\n", msgtype, msglen, msgbuf);
	}

    result = osal_msg_close(msgfd);
	if(result)
	{
		fprintf(stderr, "osal_msg_close() error.\n");
		goto ERR_EXIT;
	}
ERR_EXIT:

	return 0;
}
