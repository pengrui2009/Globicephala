/******************************************************************************
	��Ŀ����	��E600G�����ն���Ŀ
	�ļ�		��cv_drv_net.c
	����		����Ҫʵ��ͨ�����ڷ�ʽ��TBOXͨ�Ź���
	�汾		��0.1
	����		��pengrui
	��������	��2016-10-25
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
  �궨��
*************************************************/
#define 	MAX_BUF_LEN			2048
#define 	SIMU_PORT			9000		//������Խӿ�

/*************************************************
  �ṹ��
*************************************************/
typedef struct{
	int opend;
	int sockfd;
	uint16_t port;
	uint8_t ipaddr[IPADDR_LENGTH];
}tbox_net_cfg_t;

/*************************************************
  ����
*************************************************/
static tbox_net_cfg_t tbox_net_cfg;
/*************************************************
 ���ܺ���
*************************************************/

/******************************************************************************
*	����:	drv_net_init
*	����:	��ʼ��V2V��TBOX����ͨ�Žӿ�
*	����:
			dev				- ����ӿ�����
			ipaddr			- ����IP��ַ
			port			- ���ض˿ں�
*	����:
			=0				-	�ɹ�
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
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

	timeout =0xFFFF;//��������
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
*	����:	drv_net_send
*	����:	V2V����ӿڷ�������
*	����:
			pdata 			-	����ָ��
			len				-	���ݳ���
*	����:
			=0				-	�ɹ�
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
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
*	����:	drv_net_recv
*	����:	V2V����ӿڽ�������
*	����:
			pdata 			-	���յ�����ָ��(����)
			len				-	���յ����ݳ���(����)
*	����:
			= 0				-	�ɹ�
			= 1				-  	���յ��Լ����͵Ĺ㲥����
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
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
*	����:	drv_net_close
*	����:	�ر�V2V����ӿ�
*	����:
			��
*	����:
			��
*	˵��:
			��
******************************************************************************/
void drv_net_close(void)
{
	close(tbox_net_cfg.sockfd);
	tbox_net_cfg.opend = 0;
}
