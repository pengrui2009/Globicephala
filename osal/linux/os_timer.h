/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : os_timer.h
 @brief  : os_timer.h header file
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           ...
******************************************************************************/
#ifndef __OS_TIMER_H__
#define __OS_TIMER_H__

#include <time.h>
#include "error.h"
#include "os_pthread.h"

#ifdef  __cplusplus
extern "C" {
#endif


/* `os_timer_add'flag参数宏定义 */
#define TIMER_ONESHOT       (TIMER_ABSTIME << 1)    /* 一次定时器 */
#define TIMER_INTERVAL      (TIMER_ABSTIME << 2)    /* 周期定时器 */
#define TIMER_STOPPED       (TIMER_ABSTIME << 3)    /* 定时器默认处于停止状态 */

#define TIMER_PRIO_LOW      (TK_PRIO(TK_CLS_LOW,    TK_PRIO_NORMAL))
#define TIMER_PRIO_NORMAL   (TK_PRIO(TK_CLS_NORMAL, TK_PRIO_NORMAL))
#define TIMER_PRIO_HIGH     (TK_PRIO(TK_CLS_HIGH,   TK_PRIO_NORMAL))





/******************************************************************************
*     Function: os_timer_add
*     Descriptions: add the time.
*     Paramters:
            prio               - the priority of the timer
                                 the different priority of the timer was serviced by the different process
                                 the same priority timer was serviced by the same process
                                 the max support timer with different priority is 16
                                 the max support timer is 256
            millisec           - the time of timeout (uint:ms)
            flag               - the flag of the timer
                                 TIMER_ONESHOT     :the timer is once ack timer when we created
                                 TIMER_INTERVAL    :the timer is period ack timer when we created
                                 TIMER_STOPPED     :the timer is stoped when we created
            handler            - the handler function of the timer
            arg                - the arguments of the handler fucntion
            timerid            - the handle id of timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
      1. the timer would not be deleted after it was timeout.
      2. the handler function of the timer can be delete by itself.
      3. when we fail to create the timer, the memory info of the timer will not be modified.
      4. the timer id can modify by os_timer_add or os_timer_del function.
      5. after we use os_timer_del function, we should set the time id zero.
      6. before we create the timer, we had better check the timerid exist or not.
*     Example:
      1. ceate one timer with period 1 second:
        timer_t timerid;
        rc_timer_add(TIMER_PRIO_NORMAL, 1000, 0, handler, NULL, &timerid);
      2. ceate one timer with period 1 second and stop after it was created:
      timer_t timerid;
      rc_timer_add(TIMER_PRIO_NORMAL, 1000, TIMER_STOPPED, handler, NULL, &timerid);
      3. create one timer with period 1 second and ack one time
      timer_t timerid;
      rc_timer_add(TIMER_PRIO_NORMAL, 1000, TIMER_ONESHOT, handler, NULL, &timerid);
      4. create one timer with one shot and stop status
      timer_t timerid;
      rc_timer_add(TIMER_PRIO_NORMAL, 1000, TIMER_ONESHOT | TIMER_STOPPED, handler, NULL, &timerid);
******************************************************************************/
int os_timer_add(int prio, int millisec, int flag, void (*handler)(void *), void * arg, timer_t * timerid);


/******************************************************************************
*     Function: os_timer_del
*     Descriptions: delete the time.
*     Paramters:
            timerid            - the handle of the timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     Before we delete the timer, we must set the timerid 0
******************************************************************************/
int os_timer_del(timer_t timerid);


/******************************************************************************
*     Function: os_timer_start
*     Descriptions: start the time.
*     Paramters:
            timerid            - the handle of the timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     if the timer was started, then it will reset the timer,like os_timer_reset
     if the timer was timeout, then it will call the function and restart the time count.
******************************************************************************/
int os_timer_start(timer_t timerid);


/******************************************************************************
*     Function: os_timer_stop
*     Descriptions: stop the time.
*     Paramters:
            timerid             - the handle of the timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     we can stop the timer many times at the same moment
******************************************************************************/
int os_timer_stop(timer_t timerid);

/******************************************************************************
*     Function: os_timer_reset
*     Descriptions: reset the time.
*     Paramters:
            timerid             - the handle of the timer
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     it has the same effect with the function of os_timer_start
******************************************************************************/
int os_timer_reset(timer_t timerid);


/******************************************************************************
*     Function: os_timer_settime
*     Descriptions: modify the time period of timer.
*     Paramters:
            timerid             - the handle of the timer
            millisec           - the msec time of the time
*     Return:
            = 0                - success
            < 0                - failed
*     Comments: 
     Before we modify the timer, we had better used os_timer_stop stop the timer;
     After we modify the timer, we can take it effect after os_timer_start
     Example: 
      int ret = 0;
      ret |= os_timer_stop(timerid);
      ret |= os_timer_settime(timerid, 2000);
      ret |= os_timer_start(timerid);
******************************************************************************/
int os_timer_settime(timer_t timerid, int millisec);

#ifdef  __cplusplus
}
#endif


#endif
