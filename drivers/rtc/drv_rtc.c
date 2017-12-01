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

//库配置头文件
//#include "config.h"

//模块启用开关
//#if (CFG_RTC_MODULE == 1)

//C库头文件
#include <stdint.h>
#include <stdio.h>            //printf
#include <fcntl.h>            //open
#include <unistd.h>            //read,write
#include <pthread.h>        //pthread库函数
#include <sys/ioctl.h>

//提供给用户的头文件
#include "drv_rtc.h"
#include "error.h"

//驱动调用头文件

/*************************************************
  宏定义
*************************************************/


/*************************************************
  结构类型定义
*************************************************/


/*************************************************
  静态全局变量定义
*************************************************/
static int fd;                    //保存RTC驱动文件描述符
static uint8_t rtc_count = 0;        //模快打开计数
static pthread_mutex_t mutex;    //互斥锁

/*************************************************
  API函数实现
*************************************************/

/******************************************************************************
*    函数:    rtc_init
*    功能:    初始化RTC模块
*    参数:    无
*    返回:    0                -    成功
            -ERR_NODEV        -    无此设备
            -ERR_BUSY        -    设备忙，已经打开
            -ERR_SYS        -    初始化锁失败，如内存不足；已经初始化化，但还没销毁；互斥锁地址无效等
*    说明:    无
 ******************************************************************************/
int rtc_init (void)
{
    int ret = -1;
    
    if(rtc_count == 1){
        ret = -ERR_BUSY;        //已经打开
        goto err;
    }
    fd = open("/dev/rtc", O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        printf("open error!\r\n");
        ret = -ERR_NOFILE;        //没有此路径
        goto err;
    } 
    rtc_count = 1;
    //初始化互斥锁    
    if (pthread_mutex_init(&mutex, NULL)) {
        ret = -ERR_SYS;
        goto err;
    }
    ret = 0;
err:
    return ret;
}

/******************************************************************************
*    函数:    rtc_gettime
*    功能:    获取实时时钟
*    参数:    time            -    时钟（数据传出）
*    返回:    0                -    成功
            -ERR_INVAL        -    参数错误
            -ERR_NOINIT        -    设备或锁未初始化
            -ERR_SYS        -    系统错误
*    说明:    无
 ******************************************************************************/
int rtc_gettime (st_ymdhmsw_t *time)
{
    int ret = -1;
    struct rtc_time dt;
    
    if(rtc_count == 0)            //设备没初始化
        return -ERR_NOINIT;
    
    if (!time) {        
        return -ERR_INVAL;
    }
    ret = ioctl(fd, RTC_RD_TIME, &dt);
    if(ret < 0){
        printf("get time error!\r\n");
        return -ERR_SYS;
    }
    //两种时间转换
    time->sec  = dt.tm_sec;
    time->min  = dt.tm_min;
    time->hour = dt.tm_hour;
    time->day = dt.tm_mday;
    time->mon  = dt.tm_mon + 1;
    //DS3231
    time->year = dt.tm_year - 100;        //年转换
    //8025T
    //time->year = dt.tm_year;
    time->wday = dt.tm_wday + 1;
    ret = 0;
    return ret;
}


/******************************************************************************
*    函数:    rtc_settime
*    功能:    设置实时时钟
*    参数:    time            -    时钟（数据传入）
*    返回:    0                -    成功
            -ERR_INVAL        -    参数错误
            -ERR_NOINIT        -    设备或锁未初始化
            -ERR_SYS        -    系统错误
            -ERR_OTHER        -    其他关于线程锁不能解锁的错误
*    说明:    无
 ******************************************************************************/
int rtc_settime (st_ymdhmsw_t *time)
{
    int ret = -1;
    struct rtc_time dt;
    
    if(rtc_count == 0)            //设备没初始化
        return -ERR_NOINIT;
    
    //参数检查
    if (!time) {        
        return -ERR_INVAL;
    }
    //闰年检查
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
        
    //参数有效性判断
    if ((time->year > 99)||(time->mon < 1)||(time->mon > 12)||(time->day < 1)||(time->day > 31)||(time->hour > 23)||(time->min > 59)||(time->sec > 59)||(time->wday < 1)||(time->wday > 7)) {
        return -ERR_INVAL;
    }

    //获得互斥锁
    if (pthread_mutex_lock (&mutex)) {
        return  -ERR_NOINIT;        
    }
    //两种时间转换
    dt.tm_sec    =    time->sec ;
    dt.tm_min    =    time->min ;
    dt.tm_hour    =    time->hour;
    dt.tm_mday    =    time->day ;
    dt.tm_mon     =    time->mon - 1 ;
    //DS3231
    dt.tm_year     =    time->year + 100;            //年转换
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
err:    //解锁
    if (pthread_mutex_unlock (&mutex)) {
        ret = -ERR_OTHER;        
    }
    return ret;
}

#if 0
/******************************************************************************
*    函数:    rtc_getstat
*    功能:    查询实时时钟的工作状态，是否停振
*    参数:    stat            -    状态（数据传出）,0-正常，1-停振过
*    返回:    0                -    成功
            -ERR_NOINIT        -    设备或锁未初始化
            -ERR_SYS        -    系统错误
*    说明:    无
 ******************************************************************************/
int rtc_getstat (uint8_t *stat)
{
    int ret = -1;

    if(rtc_count == 0)            //设备没初始化
        return -ERR_NOINIT;

    ret = ioctl(fd, RTC_GET_STAT, stat);
    if(ret < 0){
        return -ERR_SYS;

    }
    ret = 0;
    return ret;
}

/******************************************************************************
*    函数:    rtc_set1hz
*    功能:    设置是否输出1Hz方波
*    参数:    active            -    0-停止，1-启动
*    返回:    0                -    成功
            -ERR_NOINIT        -    设备或锁未初始化
            -ERR_SYS        -    系统错误
*    说明:    无
 ******************************************************************************/
int rtc_set1hz (uint8_t active)
{
    int ret = -1;

    if(rtc_count == 0)            //设备没初始化
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
*    函数:    rtc_close
*    功能:    关闭RTC模块
*    参数:    无
*    返回:    0                -    成功
            -ERR_SYS        -    系统错误
            -ERR_NOINIT        -    模块未初始化
            -ERR_OTHER        -    当前线程不拥有互斥锁，或锁未初始化
*    说明:    无
 ******************************************************************************/
int rtc_close (void)
{
    int ret = -1;
    
    if(rtc_count == 0)            //设备没初始化
        return -ERR_NOINIT;
    
    ret = close(fd);
    if(ret < 0)
        return -ERR_SYS;
    rtc_count = 0;
    
    //销毁互斥锁    
    if (pthread_mutex_destroy(&mutex)) {
        ret = -ERR_OTHER;
    }    
    ret = 0;
    return ret;
}

//#endif

