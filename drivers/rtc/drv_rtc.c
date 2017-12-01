/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : drv_rtc.c
 @brief  : rtc drvier
 @author : pengrui
 @history:
           2017-11-30      pengrui        Created file
           
           ...
******************************************************************************/

//������ͷ�ļ�
//#include "config.h"

//ģ�����ÿ���
//#if (CFG_RTC_MODULE == 1)

//C��ͷ�ļ�
#include <stdint.h>
#include <stdio.h>            //printf
#include <fcntl.h>            //open
#include <unistd.h>            //read,write
#include <pthread.h>        //pthread�⺯��
#include <sys/ioctl.h>

//�ṩ���û���ͷ�ļ�
#include "drv_rtc.h"
#include "error.h"

//��������ͷ�ļ�

/*************************************************
  �궨��
*************************************************/


/*************************************************
  �ṹ���Ͷ���
*************************************************/


/*************************************************
  ��̬ȫ�ֱ�������
*************************************************/
static int fd;                    //����RTC�����ļ�������
static uint8_t rtc_count = 0;        //ģ��򿪼���
static pthread_mutex_t mutex;    //������

/*************************************************
  API����ʵ��
*************************************************/

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
int rtc_init (void)
{
    int ret = -1;
    
    if(rtc_count == 1){
        ret = -ERR_BUSY;        //�Ѿ���
        goto err;
    }
    fd = open("/dev/rtc", O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        printf("open error!\r\n");
        ret = -ERR_NOFILE;        //û�д�·��
        goto err;
    } 
    rtc_count = 1;
    //��ʼ��������    
    if (pthread_mutex_init(&mutex, NULL)) {
        ret = -ERR_SYS;
        goto err;
    }
    ret = 0;
err:
    return ret;
}

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
int rtc_gettime (st_ymdhmsw_t *time)
{
    int ret = -1;
    struct rtc_time dt;
    
    if(rtc_count == 0)            //�豸û��ʼ��
        return -ERR_NOINIT;
    
    if (!time) {        
        return -ERR_INVAL;
    }
    ret = ioctl(fd, RTC_RD_TIME, &dt);
    if(ret < 0){
        printf("get time error!\r\n");
        return -ERR_SYS;
    }
    //����ʱ��ת��
    time->sec  = dt.tm_sec;
    time->min  = dt.tm_min;
    time->hour = dt.tm_hour;
    time->day = dt.tm_mday;
    time->mon  = dt.tm_mon + 1;
    //DS3231
    time->year = dt.tm_year - 100;        //��ת��
    //8025T
    //time->year = dt.tm_year;
    time->wday = dt.tm_wday + 1;
    ret = 0;
    return ret;
}


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
int rtc_settime (st_ymdhmsw_t *time)
{
    int ret = -1;
    struct rtc_time dt;
    
    if(rtc_count == 0)            //�豸û��ʼ��
        return -ERR_NOINIT;
    
    //�������
    if (!time) {        
        return -ERR_INVAL;
    }
    //������
    if(((time->year%4==0)&&(time->year%100!=0))||(time->year%400==0)){
        if((time->day > 29)&&(time->mon == 2)){
            return -ERR_INVAL;
        }
    }
    else{
        if((time->day > 28)&&(time->mon == 2)){
            return -ERR_INVAL;
        }
    }
        
    //������Ч���ж�
    if ((time->year > 99)||(time->mon < 1)||(time->mon > 12)||(time->day < 1)||(time->day > 31)||(time->hour > 23)||(time->min > 59)||(time->sec > 59)||(time->wday < 1)||(time->wday > 7)) {
        return -ERR_INVAL;
    }

    //��û�����
    if (pthread_mutex_lock (&mutex)) {
        return  -ERR_NOINIT;        
    }
    //����ʱ��ת��
    dt.tm_sec    =    time->sec ;
    dt.tm_min    =    time->min ;
    dt.tm_hour    =    time->hour;
    dt.tm_mday    =    time->day ;
    dt.tm_mon     =    time->mon - 1 ;
    //DS3231
    dt.tm_year     =    time->year + 100;            //��ת��
    //8025T
    //dt.tm_year  =   time->year;
    dt.tm_wday     =    time->wday - 1;
    
    ret = ioctl(fd, RTC_SET_TIME, &dt);
    if(ret < 0){
        printf("set time error!\r\n");
        ret = ERR_SYS;
        goto err;
    }
    
    ret = 0;
err:    //����
    if (pthread_mutex_unlock (&mutex)) {
        ret = -ERR_OTHER;        
    }
    return ret;
}

#if 0
/******************************************************************************
*    ����:    rtc_getstat
*    ����:    ��ѯʵʱʱ�ӵĹ���״̬���Ƿ�ͣ��
*    ����:    stat            -    ״̬�����ݴ�����,0-������1-ͣ���
*    ����:    0                -    �ɹ�
            -ERR_NOINIT        -    �豸����δ��ʼ��
            -ERR_SYS        -    ϵͳ����
*    ˵��:    ��
 ******************************************************************************/
int rtc_getstat (uint8_t *stat)
{
    int ret = -1;

    if(rtc_count == 0)            //�豸û��ʼ��
        return -ERR_NOINIT;

    ret = ioctl(fd, RTC_GET_STAT, stat);
    if(ret < 0){
        return -ERR_SYS;

    }
    ret = 0;
    return ret;
}

/******************************************************************************
*    ����:    rtc_set1hz
*    ����:    �����Ƿ����1Hz����
*    ����:    active            -    0-ֹͣ��1-����
*    ����:    0                -    �ɹ�
            -ERR_NOINIT        -    �豸����δ��ʼ��
            -ERR_SYS        -    ϵͳ����
*    ˵��:    ��
 ******************************************************************************/
int rtc_set1hz (uint8_t active)
{
    int ret = -1;

    if(rtc_count == 0)            //�豸û��ʼ��
        return -ERR_NOINIT;

    ret = ioctl(fd, RTC_SET_1HZ, active);
    if(ret < 0){
        return -ERR_SYS;
    }

    ret = 0;
    return ret;
}
#endif
/******************************************************************************
*    ����:    rtc_close
*    ����:    �ر�RTCģ��
*    ����:    ��
*    ����:    0                -    �ɹ�
            -ERR_SYS        -    ϵͳ����
            -ERR_NOINIT        -    ģ��δ��ʼ��
            -ERR_OTHER        -    ��ǰ�̲߳�ӵ�л�����������δ��ʼ��
*    ˵��:    ��
 ******************************************************************************/
int rtc_close (void)
{
    int ret = -1;
    
    if(rtc_count == 0)            //�豸û��ʼ��
        return -ERR_NOINIT;
    
    ret = close(fd);
    if(ret < 0)
        return -ERR_SYS;
    rtc_count = 0;
    
    //���ٻ�����    
    if (pthread_mutex_destroy(&mutex)) {
        ret = -ERR_OTHER;
    }    
    ret = 0;
    return ret;
}

//#endif

