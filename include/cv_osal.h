/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_osal.h
 @brief  : This is the OS adapter layer realization.
 @author : wangyf
 @history:
           2014-11-12    wangyf    port from project 'cuckoo'. 
           ...
******************************************************************************/
#ifndef __CV_OSAL_H__
#define __CV_OSAL_H__

#include "osal_cmn.h"
#include "osal_api.h"

/**
 * Basic difinitions
 */
#ifndef NULL
#define NULL (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

typedef uint8_t bool_t;

#ifndef RELEASE
#define OS_ASSERT(EX)                                                         \
if (!(EX))                                                                    \
{                                                                             \
    volatile char dummy = 0;                                                  \
    osal_printf("(%s) assert failed at %s:%d \n", #EX, __FUNCTION__, __LINE__);\
    while (dummy == 0);                                                       \
}
#else
#define OS_ASSERT(EX)
#endif

#define osal_assert(c) OS_ASSERT(c)

/**
 * [Util]
*/
static inline void osal_delay(int usec)
{
    os_delay(usec);
}

static inline uint32_t osal_get_systemtime(void)
{
	int ret;
    struct os_reltime t;
    ret = os_get_reltime(&t);
    if(ret == 0){
        return t.sec*1000 + t.usec/1000;
    }else{
    	return 0;
    }
}
/******************************************************************************
*	函数:	osal_get_systimestamp
*	功能:	将系统时间转换为毫秒数据
*	参数:	无
*	返回:	毫秒数
*	说明:	>0			- 毫秒数
 ******************************************************************************/
static inline uint64_t osal_get_systimestamp(void)
{
	uint64_t val;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	val = tv.tv_sec ;
	return val;
}
/******************************************************************************
*	函数:	osal_get_systime
*	功能:	将系统时间转换为分钟内毫秒数时间数据
*	参数:	无
*	返回:	分钟内毫秒数
*	说明:
*			0 - 60000			- 表示正常数据
*			60000 - 60999		- 表示闰秒
*			61000 - 65534		- 预留数据
*			65535				- 无效数据
 ******************************************************************************/
static inline uint16_t osal_get_systime(void)
{
	uint16_t val;
	struct tm *t;
	struct timeval tv;
	gettimeofday(&tv, NULL);

	t = gmtime(&tv.tv_sec);
	//闰秒
	val = t->tm_sec * 1000 + (tv.tv_usec / 1000);

	return val;
}
/**
 * Notice: The follow IRQ functions are not able to be nested.
 */
//static rt_base_t cpu_sr;
static inline void osal_enter_critical(osal_mutex_t *mutex)
{
    //cpu_sr = rt_hw_interrupt_disable();
    if (mutex != NULL){
        osal_mutex_lock(mutex);
    }
}

static inline void osal_leave_critical(osal_mutex_t *mutex)
{
    //rt_hw_interrupt_enable(cpu_sr);
    if (mutex != NULL){
        osal_mutex_unlock(mutex);
    }
}

static inline void *osal_malloc(uint32_t size)
{
    return os_malloc(size);
}

static inline void osal_free(void *pointer)
{
    os_free(pointer);
}


#endif /* __CV_OSAL_H__ */

