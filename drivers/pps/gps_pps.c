/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : gps_pps.c
 @brief  : gps pps drvier
 @author : wanghechao
 @history:
           2017-9-29	  wanghechao	Created file
           ...
******************************************************************************/
#include <stdlib.h>
#include <stdint.h>

#include "../../include/error.h"
#include "gps_pps.h"

static drv_pps_st drv_pps_config; /*global pps config value*/

/*pps task deal function*/
static void *pps_thread_entry(void *arg)
{
	int ret = ERR_OK;
	uint8_t buf;
	while(1)
	{
		if((drv_pps_config.pps_fd > 0)&&(drv_pps_config.pps_start_flag != 0))
		{
			/*read pps */
			ret = read(drv_pps_config.pps_fd, (void *)&buf, 1) ;
			if(ret < 0)
			{
				printf("pps_read error\n");
				usleep(100*1000);
				continue;
			}
			/*call callback function*/
			if(drv_pps_config.pps_callback)
				(drv_pps_config.pps_callback)();
		}
		else
			usleep(100*1000);
		
	}
	return (void *)NULL;
}

/*****************************************************
func:drv_pps_register_user_isr
des:pps register callback
para: pps_isr_callback (callback function)
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_register_user_isr(pps_isr_callback_fun pps_isr_callback)
{
	drv_pps_config.pps_callback = pps_isr_callback;
	return ERR_OK;
}

/*****************************************************
func:drv_pps_start_time_calibrate
des:pps start calibrate
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_start_time_calibrate(void)
{
	int ret = ERR_OK;
	drv_pps_config.pps_start_flag = 1; /*1:start calibrate,0:stop calibrate*/
	return ret;
}

/*****************************************************
func:drv_pps_stop_time_calibrate
des:pps stop calibrate
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_stop_time_calibrate(void)
{
	int ret = ERR_OK;
	drv_pps_config.pps_start_flag = 0;/*1:start calibrate,0:stop calibrate*/
	return ret;
}

/*****************************************************
func:drv_pps_init
des:pps init
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_init(void)
{
	int ret = ERR_OK;
	if(drv_pps_config.task_pps_valid && (drv_pps_config.pps_fd > 0))
	{
		printf("drv_pps_init has been inited\n");
		return ret;
	}
	memset(&drv_pps_config, 0,sizeof(drv_pps_config));
	/*open pps device*/
	drv_pps_config.pps_fd = open(GPS_PPS_DEV, O_RDONLY, 0600);
	if(drv_pps_config.pps_fd <= 0)
	{
		ret = -ERR_SYS;
		goto error;
	}
	
	/*create pps deal task*/
	 ret = osal_task_create(&(drv_pps_config.task_pps),"t-pps",pps_thread_entry, NULL, GPS_PPS_THREAD_STACK_SIZE, GPS_PPS_THREAD_PRIORITY);
	if(ret < 0)
	{
		printf("create task_pps falied\n");
		goto error;
	}
	drv_pps_config.task_pps_valid = 1;
	return ret;
error:
	drv_pps_deinit();
	return ret;
}

/*****************************************************
func:drv_pps_deinit
des:pps deinit
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_deinit(void)
{
	int ret = ERR_OK;
	if(drv_pps_config.task_pps_valid)
	{
		/*delete pps task*/
		ret = osal_task_del(drv_pps_config.task_pps);
		if(ret < 0)
			goto error;
		drv_pps_config.task_pps_valid = 0;
	}
	/*close pps device*/
	if(drv_pps_config.pps_fd > 0)
		close(drv_pps_config.pps_fd);

	memset(&drv_pps_config, 0, sizeof(drv_pps_config));
error:
	return ret;
}



