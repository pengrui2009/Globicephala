/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : wireless.h
 @brief  : This file provider the function of the driver of wireless.
 @author : pengrui
 @history:
           2016-08-30    pengrui      Created file
           ...
******************************************************************************/


#ifndef WIRELESS_H_
#define WIRELESS_H_
#include <stdio.h>
#include <stdint.h>
/*************************************************
  宏定义
*************************************************/

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#ifndef ETH_HLEN
#define ETH_HLEN 14
#endif

#ifndef ETH_P_ALL
#define ETH_P_ALL 0x0003
#endif

#define MACADDR_LENGTH 6
/*************************************************
  结构类型定义
*************************************************/
typedef struct
{
	uint8_t		used;
	char		*ifname;//wifi设备名称
	uint8_t		channel;			 //wifi通道
	uint16_t	txpower;			 //wifi发射功率
    uint8_t 	txrate;				//发送率
    uint8_t 	mode;  				/* 0:normal, 1:qc test */
    uint8_t 	macaddr[MACADDR_LENGTH];

} wireless_config_st;

/*************************************************
  API
*************************************************/
/******************************************************************************
*	函数:	wireless_config
*	功能:	配置无线模块
*	参数:
*			cfg						-	无线模块配置参数
*	返回:
			0						-	成功
 			-ERR_SYS				-	系统错误
			-ERR_BUSY				- 	模块已初始化
*	说明:	此函数由framework_init调用。
 ******************************************************************************/
int wireless_config(wireless_config_st *cfg);
/******************************************************************************
*	函数:	wireless_init
*	功能:	无线模块初始化
*	参数:
*			cfg						- 	无线模块配置参数
*	返回:
			> 0						-	成功, 返回socket套接字
 			-ERR_SYS				-	系统错误
			-ERR_BUSY				- 	模块已初始化
*	说明:	此函数由framework_init调用。
 ******************************************************************************/
int wireless_init(wireless_config_st *cfg);
/******************************************************************************
*	函数:	wireless_send
*	功能:	无线模块发送数据
*	参数:
*			buf						-	数据缓冲区
*			len						-	数据长度
*	返回:
*			0						-	成功
* 			-ERR_SYS				-	系统错误
*			-ERR_BUSY				- 	模块已初始化
*	说明:
 ******************************************************************************/
int wireless_send(uint8_t *buf, int len);
/******************************************************************************
*	函数:	wireless_recv
*	功能:	无线模块接收数据
*	参数:
*			buf						-	数据缓冲区
*			max_len					-	接收数据最大长度
*			len						-	数据长度
*	返回:
*			0						-	成功
* 			-ERR_SYS				-	系统错误
*			-ERR_BUSY				- 	模块已初始化
*	说明:
 ******************************************************************************/
int wireless_recv(uint8_t *buf, uint16_t max_len, uint16_t *len);
/******************************************************************************
*	函数:	wireless_close
*	功能:	关闭无线网卡模块
*	参数:
*			无
*	返回:
*			0						-	成功
* 			-ERR_NOINIT				-	模块未初始化
*	说明:	无
 ******************************************************************************/
int wireless_close(void);
#endif /* WIRELESS_H_ */
