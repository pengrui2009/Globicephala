/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : drv_rtc.h
 @brief  : rtc drvier
 @author : pengrui
 @history:
           2017-11-30      pengrui        Created file
           
           ...
******************************************************************************/
#ifndef __DRV_RTC_H__
#define __DRV_RTC_H__

#include <stdint.h>

/*
 * The struct used to pass data via the following ioctl. Similar to the
 * struct tm in <time.h>, but it needs to be here so that the kernel
 * source is self contained, allowing cross-compiles, etc. etc.
 */
 
struct rtc_time {
    int tm_sec;         //�룺[0 - 59]
    int tm_min;         //���ӣ�[0 - 59]
    int tm_hour;         //ʱ��[0 - 23]
    int tm_mday;         //�գ�[1 - 31]
    int tm_mon;         //�£��Դ�һ���������·ݣ�[0 - 11]
    int tm_year;         //�꣬�Դ�1900����������:        //8025tΪ��2000����������
    int tm_wday;         //�ܣ��������յ�����[0 - 6]
    int tm_yday;
    int tm_isdst;
};

/*     Rtc_time �ṹ��ʹ��ע�⣺
     Tm_mon Ϊ����1�µ��·ݣ�����tm_mon = 8 ������������9�£�
     tm_yearΪ����1900�������������tm_year = 109����������2009�ꡣ
*/



#define RTC_RD_TIME        _IOR('p', 0x09, struct rtc_time) /* Read RTC time   */
#define RTC_SET_TIME       _IOW('p', 0x0a, struct rtc_time) /* Set RTC time    */

//the structure of year,month,day,hour,minute,second,week
typedef struct {
    uint8_t year;    //year: [0 - 99] since from 2000 year
    uint8_t mon;    //month: [1 - 12]
    uint8_t day;    //day: [1 - 31]
    uint8_t hour;    //hour: [0 - 23]
    uint8_t min;    //minute: [0 - 59]
    uint8_t sec;    //second: [0 - 59]
    uint8_t wday;    //week: [1 - 7]
} st_ymdhmsw_t;

/******************************************************************************
*    ����:    rtc_init
*    ����:    ��ʼ��RTCģ��
*    ����:    ��
*    ����:    0                -    �ɹ�
            -ERR_NODEV        -    �޴��豸
            -ERR_BUSY        -    �豸æ���Ѿ���
            -ERR_SYS        -    ��ʼ����ʧ�ܣ����ڴ治�㣻�Ѿ���ʼ����������û���٣���������ַ��Ч��
*    ˵��:    ��
 ******************************************************************************/
extern int rtc_init (void);


/******************************************************************************
*    ����:    rtc_gettime
*    ����:    ��ȡʵʱʱ��
*    ����:    time            -    ʱ�ӣ����ݴ�����
*    ����:    0                -    �ɹ�
            -ERR_INVAL        -    ��������
            -ERR_NOINIT        -    �豸����δ��ʼ��
            -ERR_SYS        -    ϵͳ����
*    ˵��:    ��
 ******************************************************************************/
extern int rtc_gettime (st_ymdhmsw_t *time);


/******************************************************************************
*    ����:    rtc_settime
*    ����:    ����ʵʱʱ��
*    ����:    time            -    ʱ�ӣ����ݴ��룩
*    ����:    0                -    �ɹ�
            -ERR_INVAL        -    ��������
            -ERR_NOINIT        -    �豸����δ��ʼ��
            -ERR_SYS        -    ϵͳ����
            -ERR_OTHER        -    ���������߳������ܽ����Ĵ���
*    ˵��:    ��
 ******************************************************************************/
extern int rtc_settime (st_ymdhmsw_t *time);



#endif
