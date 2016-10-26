/******************************************************************************
	��Ŀ����	��E600G�����ն���Ŀ
	�ļ�		��uart.c
	����		��
	�汾		��0.1
	����		��pengrui
	��������	��2016-6-13
******************************************************************************/

//C��ͷ�ļ�
#include <stdio.h>						//printf
#include <fcntl.h>						//open
#include <unistd.h>						//read,write
#include <termios.h>					//�������ü��궨��
#include <string.h>						//bzero,memcyp
#include <sys/ioctl.h>					//ioctl

//�ṩ���û���ͷ�ļ�
#include "uart.h"
#include "cv_ehm.h"
#include "cv_drv_uart.h"
#include "error.h"
/*************************************************
  �ṹ���Ͷ���
*************************************************/
typedef struct{
	int fd;
	uint8_t opend;				//�򿪼���
} tbox_uart_cfg_t;
/*************************************************
  ��̬ȫ�ֱ�������
*************************************************/
static tbox_uart_cfg_t tbox_uart_cfg;
/*************************************************
  API����ʵ��
*************************************************/
/*************************************************
  �����ײ����õ��ĺ���
*************************************************/
/******************************************************************************
*	����:	drv_uart_open
*	����:	�򿪴����豸
*	����:
			��
*	����:	0				-	�ɹ�
			-ERR_SYS		-	����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
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
*	����:	drv_uart_cfg
*	����:	���ô����豸����
*	����:
			cfg				-	����������Ϣ
*	����:	0				-	�ɹ�
			-ERR_SYS		-	����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
 ******************************************************************************/
int drv_uart_config (comport_config_t *cfg)
{
	int ret;

	if(tbox_uart_cfg.opend <= 0)
	{
		ret = -ERR_NOINIT;		//û�г�ʼ��
		goto err;
	}

	if(tbox_uart_cfg.fd <= 0)
	{
		ret = -ERR_NOINIT;		//û�г�ʼ��
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
*	����:	drv_uart_send
*	����:	���ڷ�������
*	����:
			buf				-	����ָ��
			len				-	���ݳ���
*	����:
			>0				-	�ɹ����͵��ֽ���
			-ERR_SYS		-	����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
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
		ret = -ERR_NOINIT;		//û�г�ʼ��
		goto err;
	}

	if(len <= 0)
	{
		ret = -ERR_INVAL;		//û�г�ʼ��
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
*	����:	drv_uart_recv
*	����:	��ȡ�����豸����
*	����:
			buf				-	����ָ��
			len 			-	���ݳ���
*	����:
			> 0				-	�ɹ����յ��Ĵ��������ֽ���
			-ERR_SYS		-	����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
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
		ret = -ERR_NOINIT;		//û�г�ʼ��
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
*	����:	drv_uart_close
*	����:	�رմ����豸
*	����:
			��
*	����:
			0				-	�ɹ�
			-ERR_SYS		-	����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
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
