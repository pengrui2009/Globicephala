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
  �궨��
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
  �ṹ���Ͷ���
*************************************************/
typedef struct
{
	uint8_t		used;
	char		*ifname;//wifi�豸����
	uint8_t		channel;			 //wifiͨ��
	uint16_t	txpower;			 //wifi���书��
    uint8_t 	txrate;				//������
    uint8_t 	mode;  				/* 0:normal, 1:qc test */
    uint8_t 	macaddr[MACADDR_LENGTH];

} wireless_config_st;

/*************************************************
  API
*************************************************/
/******************************************************************************
*	����:	wireless_config
*	����:	��������ģ��
*	����:
*			cfg						-	����ģ�����ò���
*	����:
			0						-	�ɹ�
 			-ERR_SYS				-	ϵͳ����
			-ERR_BUSY				- 	ģ���ѳ�ʼ��
*	˵��:	�˺�����framework_init���á�
 ******************************************************************************/
int wireless_config(wireless_config_st *cfg);
/******************************************************************************
*	����:	wireless_init
*	����:	����ģ���ʼ��
*	����:
*			cfg						- 	����ģ�����ò���
*	����:
			> 0						-	�ɹ�, ����socket�׽���
 			-ERR_SYS				-	ϵͳ����
			-ERR_BUSY				- 	ģ���ѳ�ʼ��
*	˵��:	�˺�����framework_init���á�
 ******************************************************************************/
int wireless_init(wireless_config_st *cfg);
/******************************************************************************
*	����:	wireless_send
*	����:	����ģ�鷢������
*	����:
*			buf						-	���ݻ�����
*			len						-	���ݳ���
*	����:
*			0						-	�ɹ�
* 			-ERR_SYS				-	ϵͳ����
*			-ERR_BUSY				- 	ģ���ѳ�ʼ��
*	˵��:
 ******************************************************************************/
int wireless_send(uint8_t *buf, int len);
/******************************************************************************
*	����:	wireless_recv
*	����:	����ģ���������
*	����:
*			buf						-	���ݻ�����
*			max_len					-	����������󳤶�
*			len						-	���ݳ���
*	����:
*			0						-	�ɹ�
* 			-ERR_SYS				-	ϵͳ����
*			-ERR_BUSY				- 	ģ���ѳ�ʼ��
*	˵��:
 ******************************************************************************/
int wireless_recv(uint8_t *buf, uint16_t max_len, uint16_t *len);
/******************************************************************************
*	����:	wireless_close
*	����:	�ر���������ģ��
*	����:
*			��
*	����:
*			0						-	�ɹ�
* 			-ERR_NOINIT				-	ģ��δ��ʼ��
*	˵��:	��
 ******************************************************************************/
int wireless_close(void);
#endif /* WIRELESS_H_ */
