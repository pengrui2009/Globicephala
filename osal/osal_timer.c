/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : os_timer.c
 @brief  : timer 
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           2017-10-24   pengrui    Add the comments
           ...
******************************************************************************/

#include "osal_timer.h"

/******************************************************************************
*    Function: osal_timer_create
*    Descriptions: create the timer.
*    Paramters:
            timefd_ptr                       - the handle of the timer(Returned)
            name                             - the name of the timer
            handler                          - the handler function of the timer
            arg                              - the arguments of the handler function
            millisec                         - the time of the timer(uint msec)
            flag                             - the attribute of the timer {TIMER_ONESHOT, TIMER_INTERVAL, TIMER_STOPPED}
            prio     						 - the PRIO of the timer
*    Return:
            = 0                              - success
            < 0                              - failed
*    Comments: 
******************************************************************************/
int osal_timer_create(osal_timer_t *timer_ptr, const char* name, void (*handler)(void *), void * arg, int millisec, int flag, int prio)
{
    int ret = 0;

    ret = os_timer_add(prio, millisec, flag, handler, arg, timer_ptr);
   
    return ret;
}

/******************************************************************************
*    Function: osal_timer_delete
*    Descriptions: delete the timer.
*    Paramters:
            timerfd                         - the handler of the timer
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments: 
     
******************************************************************************/
int osal_timer_delete(osal_timer_t timerfd)
{
    int ret = 0;

    ret = os_timer_del(timerfd);

    return ret; 
}

/******************************************************************************
*    Function: osal_timer_start
*    Descriptions: start the timer.
*    Paramters:
            timerfd                         - the handler of the timer
*    Return:
            = 0                             - success
            < 0                             - failed
*    Comments: 
******************************************************************************/
int osal_timer_start(osal_timer_t timerfd)
{
    int ret = 0;

    ret = os_timer_start(timerfd);
	
    return ret; 
}

/******************************************************************************
*    Function: osal_timer_stop
*    Descriptions: stop the timer.
*    Paramters:
            timerfd                          - the handler of the timer
*    Return:
             = 0                             - success
             < 0                             - failed
*    Comments: 
******************************************************************************/
int osal_timer_stop(osal_timer_t timerfd)
{
    int ret = 0;

    ret = os_timer_stop(timerfd);
	
    return ret; 
}
    
/******************************************************************************
*    Function: osal_timer_reset
*    Descriptions:reset the timer.
*    Paramters:
            timer_ptr                        - the handler of the timer
*    Return:
			= 0 						     - success
			< 0 							 - failed
*    Comments: 
******************************************************************************/
int osal_timer_reset(osal_timer_t timerfd)
{
    int ret;
   
    ret = os_timer_start(timerfd);
	
    return ret; 
}

/******************************************************************************
*    Function: osal_timer_change
*    Descriptions: change the time period of timer.
*    Paramters:
            timerfd                          - the handler of the timer
            millisec                         - the period of the timer(uint msec)
*    Return:
			= 0 						     - success
			< 0 							 - failed
*    Comments: 
******************************************************************************/
osal_status_t osal_timer_change(osal_timer_t timerfd, int millisec)
{
    int ret = 0;
    
    ret |= os_timer_stop(timerfd);
    ret |= os_timer_settime(timerfd, millisec);
    ret |= os_timer_start(timerfd);
    
    return ret; 
}


