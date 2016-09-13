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
#include "error.h"
//CRC16 table
static unsigned char auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

static char auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

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
*	函数:	cv_ntohs
*	功能:	调整2个字节之间的网络字节序
*	参数:	s16					- 	数据
*	返回:	>0					-	成功
*	说明:	无
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
*	函数:	cv_ntohl
*	功能:	调整4个字节之间的网络字节序
*	参数:	l32					- 	数据
*	返回:	>0					-	成功
*	说明:	无
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
*	函数:	cv_ntohll
*	功能:	调整8个字节之间的网络字节序
*	参数:	l64					- 	数据
*	返回:	>0					-	成功
*	说明:	无
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
*	函数:	cv_ntohf
*	功能:	调整4个字节之间的网络字节序
*	参数:	f32					- 	数据
*	返回:	>0					-	成功
*	说明:	无
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

/******************************************************************************
*  函数:	CRC16
*  功能:	计算数据帧的CRC16校验和
*  参数:	pdata           -   数据帧
		DataLen         -   数据长度
*  返回:	num             -   帧校验和
*  说明:	CRC计算公式:x^16 + x^15 + x^2 + x^0
 ******************************************************************************/
inline uint16_t CRC16(uint8_t *pdata , uint32_t DataLen)
{
	uint8_t uchCRCHi =0xff; 	/*CRC高字节初始化*/
	uint8_t uchCRCLo =0xff; 	/*CRC低字节初始化*/
	uint8_t uIndex ;	/*CRC循环索引*/
	while (DataLen--)	/*传输消息缓冲区*/
	{
		uIndex = uchCRCHi ^ *pdata++;	/*计算CRC*/
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}
/******************************************************************************
*	函数:	net_mac_get
*	功能:	获取mac
*	参数:	mac				-	mac地址
*	返回:	0				-	成功
			-ERR_SYS		-	系统错误
*	说明:	获取mask地址
******************************************************************************/
int net_mac_get(char *ifname,uint8_t *mac)
{
	int fd;
	int ret = 0;
	struct ifreq ifr;
	//char *interface = "eth0";

	memset(&ifr, 0, sizeof(ifr));
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (fd == -ERR_SYS) {
		return fd;
	}
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);        //配置接口属性
	if (mac) {
		if (ioctl(fd, SIOCGIFHWADDR, &ifr) != 0) {
			close(fd);
			return -ERR_SYS;
		}
		memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
	}
	close(fd);
	return ret;
}
