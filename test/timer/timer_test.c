/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : timer_test.c
 @brief  : test the timer api
 @author : pengrui
 @history:
           2017-10-24    pengrui    Created file
           ...
******************************************************************************/
#include "osal_timer.h"

#define TIEMR_TEST_PRIO 	TIMER_PRIO_HIGH

void timer_handler(void *arg)
{
	printf("%s %d\n", __FUNCTION__, __LINE__);
}

int main()
{
	int ret = 0;
	osal_timer_t timer_test;

	ret = osal_timer_create(&timer_test, "timer-test", timer_handler, NULL, 1000, TIMER_INTERVAL, TIEMR_TEST_PRIO);
	if(ret)
	{
		fprintf(stderr, "osal_timer_create() error:%d \n", ret);
		goto ERR_EXIT;
	}

	ret = osal_timer_start(timer_test);
	if(ret)
	{
		fprintf(stderr, "osal_timer_start() :%d\n", ret);
		//goto ERR_EXIT;
	}

	while(1)
	{
		sleep(10);
		osal_timer_stop(timer_test);	
	}

ERR_EXIT:

	return ret;
}
