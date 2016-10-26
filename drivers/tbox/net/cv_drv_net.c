/******************************************************************************
	项目名称	：E600G车载终端项目
	文件		：cv_drv_net.c
	描述		：主要实现通过网口方式与TBOX通信功能
	版本		：0.1
	作者		：pengrui
	创建日期	：2016-10-25
******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "net.h"
#include "cv_osal.h"
#include "cv_wnet.h"
#include "cv_vam.h"
#include "cv_osal_dbg.h"
#include "cv_drv_net.h"
#include "error.h"
/*************************************************
  宏定义
*************************************************/
#define 	MAX_BUF_LEN			2048
#define 	SIMU_PORT			9000		//仿真测试接口

/*************************************************
  结构体
*************************************************/
typedef struct{
	int opend;
	int sockfd;
	uint16_t port;
	uint8_t ipaddr[IPADDR_LENGTH];
}tbox_net_cfg_t;

/*************************************************
  变量
*************************************************/
static tbox_net_cfg_t tbox_net_cfg;
/*************************************************
 功能函数
*************************************************/

/******************************************************************************
*	函数:	drv_net_init
*	功能:	初始化V2V与TBOX网络通信接口
*	参数:
			dev				- 网络接口名称
			ipaddr			- 本地IP地址
			port			- 本地端口号
*	返回:
			=0				-	成功
			-ERR_SYS		-	系统错误
			-ERR_INVAL		-	接口错误
			-ERR_BUSY		-	已断开

*	说明:	无
******************************************************************************/
int drv_net_init(char *ifname, uint16_t port)
{
	int ret;
	uint16_t timeout;

	if(tbox_net_cfg.opend > 0)
	{
		ret = -ERR_BUSY;
		goto err;
	}

	ret = net_ip_get(ifname , tbox_net_cfg.ipaddr);
	if(ret < 0)
	{
		goto err;
	}

	tbox_net_cfg.port = port;

	timeout =0xFFFF;//永久阻塞
	ret = net_udp_init(port, tbox_net_cfg.ipaddr, timeout);
	if(ret < 0)
	{
		osal_printf("%s net_udp_init error ret=%d\n", __FUNCTION__, ret);
		goto err;
	}else{
		tbox_net_cfg.sockfd = ret;
	}

	tbox_net_cfg.opend = 1;

	ret = 0;
err:
	return ret;
}
/******************************************************************************
*	函数:	drv_net_send
*	功能:	V2V网络接口发送数据
*	参数:
			pdata 			-	数据指针
			len				-	数据长度
*	返回:
			=0				-	成功
			-ERR_SYS		-	系统错误
			-ERR_INVAL		-	接口错误
			-ERR_BUSY		-	已断开

*	说明:	无
******************************************************************************/
int drv_net_send(uint8_t *pdata, uint16_t len)
{
    int ret;

    uint8_t send_ipaddr[IPADDR_LENGTH];
    if(tbox_net_cfg.opend <= 0)
	{
		ret = -ERR_NOINIT;
		goto err;
	}

	if(tbox_net_cfg.sockfd < 0)
	{
		ret = -ERR_NOINIT;
		goto err;
	}

	memcpy(send_ipaddr, tbox_net_cfg.ipaddr, IPADDR_LENGTH);
	send_ipaddr[IPADDR_LENGTH - 1] = 0xFF;

	ret =  net_udp_send(tbox_net_cfg.sockfd, pdata, len, send_ipaddr, tbox_net_cfg.port);
	if(ret < 0)
	{
		goto err;
	}

    ret = 0;
err:
	return ret;
}
/******************************************************************************
*	函数:	drv_net_recv
*	功能:	V2V网络接口接收数据
*	参数:
			pdata 			-	接收到数据指针(返回)
			len				-	接收到数据长度(返回)
*	返回:
			= 0				-	成功
			= 1				-  	接收到自己发送的广播数据
			-ERR_SYS		-	系统错误
			-ERR_INVAL		-	接口错误
			-ERR_BUSY		-	已断开

*	说明:	无
******************************************************************************/
int drv_net_recv(uint8_t *pdata, uint16_t *len)
{
	int ret;
	uint16_t recv_port;
	uint8_t recv_ipaddr[IPADDR_LENGTH];

	if(tbox_net_cfg.opend <= 0)
	{
		ret = -ERR_NOINIT;
		goto err;
	}

	if(tbox_net_cfg.sockfd < 0)
	{
		ret = -ERR_NOINIT;
		goto err;
	}

	ret = net_udp_recv(tbox_net_cfg.sockfd, pdata, MAX_BUF_LEN, len, NET_BLOCK, recv_ipaddr, &recv_port);
	if(ret < 0)
	{
		goto err;
	}

	if(memcmp(tbox_net_cfg.ipaddr, recv_ipaddr, IPADDR_LENGTH) != 0 )
	{
		ret = 0x0;
	}else{
		ret = 0x1;
	}
err:
	return ret;
}
/******************************************************************************
*	函数:	drv_net_close
*	功能:	关闭V2V网络接口
*	参数:
			无
*	返回:
			无
*	说明:
			无
******************************************************************************/
void drv_net_close(void)
{
	close(tbox_net_cfg.sockfd);
	tbox_net_cfg.opend = 0;
}
