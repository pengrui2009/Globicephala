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
    int tm_sec;         //秒：[0 - 59]
    int tm_min;         //分钟：[0 - 59]
    int tm_hour;         //时：[0 - 23]
    int tm_mday;         //日：[1 - 31]
    int tm_mon;         //月，自从一月以来的月份：[0 - 11]
    int tm_year;         //年，自从1900年以来的年:        //8025t为自2000年以来的年
    int tm_wday;         //周：距离周日的天数[0 - 6]
    int tm_yday;
    int tm_isdst;
};

/*     Rtc_time 结构体使用注意：
     Tm_mon 为距离1月的月份，比如tm_mon = 8 代表着现在是9月；
     tm_year为距离1900年的年数，比如tm_year = 109代表现在是2009年。
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
*    函数:    rtc_init
*    功能:    初始化RTC模块
*    参数:    无
*    返回:    0                -    成功
            -ERR_NODEV        -    无此设备
            -ERR_BUSY        -    设备忙，已经打开
            -ERR_SYS        -    初始化锁失败，如内存不足；已经初始化化，但还没销毁；互斥锁地址无效等
*    说明:    无
 ******************************************************************************/
extern int rtc_init (void);


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
extern int rtc_gettime (st_ymdhmsw_t *time);


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
extern int rtc_settime (st_ymdhmsw_t *time);



#endif
