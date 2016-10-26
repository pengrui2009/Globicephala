/******************************************************************************
	项目名称	：E600G车载终端项目
	文件		：uart.c
	描述		：
	版本		：0.1
	作者		：pengrui
	创建日期	：2016-6-13
******************************************************************************/

//C库头文件
#include <stdio.h>						//printf
#include <fcntl.h>						//open
#include <unistd.h>						//read,write
#include <termios.h>					//串口设置及宏定义
#include <string.h>						//bzero,memcyp
#include <sys/ioctl.h>					//ioctl

//提供给用户的头文件
#include "uart.h"
#include "cv_ehm.h"
#include "cv_drv_uart.h"
#include "error.h"
/*************************************************
  结构类型定义
*************************************************/
typedef struct{
	int fd;
	uint8_t opend;				//打开计数
} tbox_uart_cfg_t;
/*************************************************
  静态全局变量定义
*************************************************/
static tbox_uart_cfg_t tbox_uart_cfg;
/*************************************************
  API函数实现
*************************************************/
/*************************************************
  操作底层所用到的函数
*************************************************/
/******************************************************************************
*	函数:	drv_uart_open
*	功能:	打开串口设备
*	参数:
			无
*	返回:	0				-	成功
			-ERR_SYS		-	错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
int drv_uart_open(void)
{
	int ret;

	if(tbox_uart_cfg.opend > 0)
	{
		ret = -ERR_BUSY;
		goto err;
	}
	ret = comport_init("/dev/ttyATH0");
	if(ret < 0)
	{
		goto err;
	}

	tbox_uart_cfg.fd = ret;
	tbox_uart_cfg.opend = 1;

	ret = 0;
err:
	return ret;
}
/******************************************************************************
*	函数:	drv_uart_cfg
*	功能:	配置串口设备参数
*	参数:
			cfg				-	串口配置信息
*	返回:	0				-	成功
			-ERR_SYS		-	错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
int drv_uart_config (comport_config_t *cfg)
{
	int ret;

	if(tbox_uart_cfg.opend <= 0)
	{
		ret = -ERR_NOINIT;		//没有初始化
		goto err;
	}

	if(tbox_uart_cfg.fd <= 0)
	{
		ret = -ERR_NOINIT;		//没有初始化
		goto err;
	}

	ret = comport_setconfig(tbox_uart_cfg.fd, cfg);
	if(ret < 0){
		goto err;
	}
	ret = 0;
err:
	return ret;
}
/******************************************************************************
*	函数:	drv_uart_send
*	功能:	串口发送数据
*	参数:
			buf				-	数据指针
			len				-	数据长度
*	返回:
			>0				-	成功发送的字节数
			-ERR_SYS		-	错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
int drv_uart_send (uint8_t *buf, uint16_t len)
{
	int ret = -1;

	if(tbox_uart_cfg.opend < 0 )
	{
		ret = -ERR_NOINIT;
		goto err;
	}
	if(tbox_uart_cfg.fd <= 0)
	{
		ret = -ERR_NOINIT;		//没有初始化
		goto err;
	}

	if(len <= 0)
	{
		ret = -ERR_INVAL;		//没有初始化
		goto err;
	}

	ret = write(tbox_uart_cfg.fd, buf, len);
	if(ret < 0)
	{
		ret = -ERR_SYS;
		goto err;
	}

err:
	return ret;
}
/******************************************************************************
*	函数:	drv_uart_recv
*	功能:	读取串口设备数据
*	参数:
			buf				-	数据指针
			len 			-	数据长度
*	返回:
			> 0				-	成功接收到的串口数据字节数
			-ERR_SYS		-	错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
int drv_uart_recv (uint8_t *buf, uint16_t len)
{
	int ret = -1;

	if(tbox_uart_cfg.opend < 0 )
	{
		ret = -ERR_NOINIT;
		goto err;
	}

	if(tbox_uart_cfg.fd <= 0)
	{
		ret = -ERR_NOINIT;		//没有初始化
		goto err;
	}

	ret = read(tbox_uart_cfg.fd, buf, len);
	if(ret == 0){
		ret = -ERR_TIMEOUT;
		goto err;
	}
	if(ret < 0){
		ret = -ERR_SYS;
		goto err;
	}

err:
	return ret;
}
/******************************************************************************
*	函数:	drv_uart_close
*	功能:	关闭串口设备
*	参数:
			无
*	返回:
			0				-	成功
			-ERR_SYS		-	错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
int drv_uart_close (void)
{
	int ret;

	ret = close(tbox_uart_cfg.fd);
	if(ret < 0){
		goto error;
	}
	tbox_uart_cfg.opend = 0;
	ret = 0;

error:
	return ret;
}
