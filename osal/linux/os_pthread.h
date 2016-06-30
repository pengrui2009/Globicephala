/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : os_pthread.h
 @brief  : os_pthread.h header file
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           ...
******************************************************************************/


#ifndef __OS_PTHREAD_H__
#define __OS_PTHREAD_H__

#include <pthread.h>
#include <stdint.h>
#ifdef  __cplusplus
extern "C" {
#endif

#define TK_CLS_LOW              (10)
#define TK_CLS_BELOW_NORMAL     (20)
#define TK_CLS_NORMAL           (30)
#define TK_CLS_ABOVE_NORMAL     (40)
#define TK_CLS_HIGH             (48)

#define TK_PRIO_LOW             (-2)
#define TK_PRIO_NORMAL          (0)
#define TK_PRIO_HIGH            (2)

#define TK_PRIO(cls, prio)      ((cls) + (prio))
#define TK_PRIO_DEFAULT         (TK_PRIO(TK_CLS_NORMAL, TK_PRIO_NORMAL))

#define TK_NAME_PREFIX          "tk_"

#define THREAD_MODE_NORMAL 			0			//普通线程
#define THREAD_MODE_REALTIME		1			//实时线程
/*************************************************
  API
*************************************************/
/******************************************************************************
*	函数:	thread_create
*	功能:	建立线程
*	参数:	id				-	线程号
 			function		-	线程函数
 			arg				-	线程函数参数
 			mode			-	线程模式（普通和实时）
 			prio			-	优先级（1-99）
*	返回:	0				-	成功
 			-ERR_NOINIT		-	模块未初始化
 			-ERR_INVAL		-	参数不正确
 			-ERR_BUSY		-	线程已存在
 			-ERR_NOMEM		-	内存不足
*	说明:	只有实时模式支持优先级，99为最高优先级。普通模式优先级固定为0。
 ******************************************************************************/
int thread_create(uint8_t id, void * (*function)(void *), void *arg, uint8_t mode, uint8_t prio);

/******************************************************************************
*	函数:	thread_cancel
*	功能:	终止线程
*	参数:	id				-	线程号
*	返回:	0				-	成功
 			-ERR_NODEV		-	无此线程
 			-ERR_INVAL		-	参数错误
*	说明:
 ******************************************************************************/
int thread_cancel(pthread_t tid);

/******************************************************************************
*	函数:	thread_setpriority
*	功能:	设置优先级
*	参数:	id				-	线程号
 			prio			-	优先级
*	返回:	0				-	成功
 			-ERR_NODEV		-	无此线程
 			-ERR_INVAL		-	参数错误
*	说明:	优先级为1-99，99为最高优先级
 ******************************************************************************/
int thread_setpriority(pthread_t tid, uint8_t prio);

/******************************************************************************
*	函数:	thread_create_base
*	功能:	建立线程
*	参数:	tid				-	系统线程号
 			function		-	线程函数
 			arg				-	线程函数参数
 			mode			-	线程模式（普通和实时）
 			prio			-	优先级（1-99）
*	返回:	0				-	成功
 			-ERR_INVAL		-	参数不正确
 			-ERR_NOMEM		-	内存不足
*	说明:	只有实时模式支持优先级，99为最高优先级。普通模式优先级固定为0。
 ******************************************************************************/
int thread_create_base(pthread_t *tid, void *(*function)(void *), void *arg, uint8_t mode, uint8_t prio);

/******************************************************************************
*	函数:	thread_setpriority_base
*	功能:	设置线程优先级
*	参数:	tid				-	系统线程号
 			prio			-	优先级
*	返回:	0				-	成功
 			-ERR_INVAL		-	参数错误
*	说明:	优先级为1-99，99为最高优先级。
 ******************************************************************************/
int thread_setpriority_base(pthread_t tid, uint8_t prio);

/******************************************************************************
*	函数:	thread_gettid_base
*	功能:	得到当前系统线程号
*	参数:	void
*	返回:	*				-	系统线程号
*	说明:
 ******************************************************************************/
int thread_gettid_base(void);
extern void os_blockallsigs(void);
extern void os_unblocksig(int sig);
extern void os_procselfexepath(char * path, size_t len);
extern void os_procselfexename(char * name, size_t len);
extern void os_setprocparam(const char * name);
extern int os_pthread_create(pthread_t * thread, const char * name, int prio, 
                    int stack_size, void *(*start_routine)(void *), void * arg);
extern int os_pthread_create_joinable(pthread_t * thread, const char * name, int prio, 
                    int stack_size, void * (*start_routine)(void *), void * arg);

#ifdef  __cplusplus
}
#endif


#endif
