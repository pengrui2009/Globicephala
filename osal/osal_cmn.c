/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_cmn.c
 @brief  : common osal api
 @author : wanglei
 @history:
           2015-8-11    wanglei    Created file
           ...
******************************************************************************/

#include "osal_cmn.h"

uint64_t osal_current_time(void)
{
    uint64_t time = 0;

    time = os_tick_get();
    return time;
}

void osal_udelay(int32_t microseconds)
{
    os_delay(microseconds);
    return;
}

void osal_mdelay(int32_t milliseconds)
{
    os_delay(milliseconds*1000);
}

void osal_sleep(int32_t milliseconds)
{
    os_sleep(milliseconds);
    return;
}
/******************************************************************************
*	����:	cv_ntohs
*	����:	����2���ֽ�֮��������ֽ���
*	����:	s16					- 	����
*	����:	>0					-	�ɹ�
*	˵��:	��
 ******************************************************************************/
inline uint16_t cv_ntohs(uint16_t s16)
{
	uint16_t ret = 0;
#ifdef __LITTLE_ENDIAN
	uint8_t *s, *d;
#endif
	#ifndef __LITTLE_ENDIAN
	ret = s16;
	#else
	s = (uint8_t *)(&s16);
	d = (uint8_t *)(&ret) + 1;
	*d-- = *s++;
	*d-- = *s++;
	#endif
	return ret;
}
/******************************************************************************
*	����:	cv_ntohl
*	����:	����4���ֽ�֮��������ֽ���
*	����:	l32					- 	����
*	����:	>0					-	�ɹ�
*	˵��:	��
 ******************************************************************************/
inline uint32_t cv_ntohl(uint32_t l32)
{
	uint32_t ret = 0;
#ifdef __LITTLE_ENDIAN
	uint8_t *s, *d;
#endif
	//#ifdef BIG_ENDIAN
	#ifndef __LITTLE_ENDIAN
	ret = l32;
	#else
 	s = (uint8_t *)(&l32);
 	d = (uint8_t *)(&ret) + 3;

 	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	#endif



	return ret;
}
/******************************************************************************
*	����:	cv_ntohll
*	����:	����8���ֽ�֮��������ֽ���
*	����:	l64					- 	����
*	����:	>0					-	�ɹ�
*	˵��:	��
 ******************************************************************************/
inline uint64_t cv_ntohll(uint64_t l64)
{
	uint64_t ret = 0;
#ifdef __LITTLE_ENDIAN
	uint8_t *s, *d;
#endif
	//#ifdef BIG_ENDIAN
	#ifndef __LITTLE_ENDIAN
	ret = l64;
	#else
 	s = (uint8_t *)(&l64);
 	d = (uint8_t *)(&ret) + 7;

 	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	#endif



	return ret;
}
/******************************************************************************
*	����:	cv_ntohf
*	����:	����4���ֽ�֮��������ֽ���
*	����:	f32					- 	����
*	����:	>0					-	�ɹ�
*	˵��:	��
 ******************************************************************************/
inline float cv_ntohf(float f32)
{
	float ret;
#ifdef __LITTLE_ENDIAN
	uint8_t *s, *d;
#endif
    #ifndef __LITTLE_ENDIAN
	ret = f32;
	#else
	s = (uint8_t *)(&f32);
	d = (uint8_t *)(&ret) + 3;


	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	*d-- = *s++;
	#endif
	return ret;
}
/*lint -restore*/

