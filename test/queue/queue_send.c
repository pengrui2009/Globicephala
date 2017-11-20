/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : queue_send.c
 @brief  : test the api function of osal queue.
 @author : pengrui
 @history:
           2017-10-24    pengrui    Created file
           ...
******************************************************************************/

#include "osal_queue.h"

#define QUEUE_MAX_MSGS       40
#define QUEUE_MAX_SIZE       2048


int main()
{
    int result = 0;
    osal_queue_t queue;
	
    result = osal_queue_create(&queue, "queue-test", QUEUE_MAX_MSGS, QUEUE_MAX_SIZE);
	if(result)
	{
		fprintf(stderr, "osal_queue_create() error:%d\n", result);
		goto ERR_EXIT;
	}

	while(1)
	{
		char queue_buf[100];
		uint32_t queue_len = 0;
		scanf("%s", queue_buf);
		queue_len = strlen(queue_buf);
		
	    result = osal_queue_send(queue, (void *)&queue_buf, queue_len, 0, 0);
		if(result)
		{
			fprintf(stderr, "osal_queue_send() error:%d\n", result);
			goto ERR_EXIT;
		}
	}

    //printf("queue recv{len:%d buf:%s}\n", queue_len, queue_buf);

	result = osal_queue_delete(queue);
	if(result)
	{
		fprintf(stderr, "osal_queue_delete() error:%d\n", result);
		goto ERR_EXIT;
	}
	
ERR_EXIT:

	return result;
}

