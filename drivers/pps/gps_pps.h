#ifndef _GPS_PPS_H_
#define _GPS_PPS_H_

#include <stdint.h>
#include <os_core.h>
#include <osal_thread.h>
#include <os_pthread.h>

#define GPS_PPS_DEV        "/dev/ps"   //pps device node
#define GPS_PPS_THREAD_STACK_SIZE    (1024*8192)  //PPS thread stack size  8K
#define GPS_PPS_THREAD_PRIORITY        (TK_PRIO_DEFAULT + 21)  //PPS thread priority

typedef void (*pps_isr_callback_fun)(void);  //PPS callback function type

/*pps driver config type*/
typedef struct _drv_pps_st
{
    int  pps_fd; //pps device file descriptor
    osal_task_t task_pps; //pps deal task
    uint8_t task_pps_valid; /*Whether it is a valid sign,0:invalid,1:valid*/
    pps_isr_callback_fun pps_callback; //pps callback function
    uint8_t pps_start_flag;  // pps start or stop flag
}drv_pps_st;


/*****************************************************
func:drv_pps_init
des:pps init
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_init(void);

/*****************************************************
func:drv_pps_deinit
des:pps deinit
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_deinit(void);

/*****************************************************
func:drv_pps_register_user_isr
des:pps register callback
para: pps_isr_callback (callback function)
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_register_user_isr(pps_isr_callback_fun pps_isr_callback);

/*****************************************************
func:drv_pps_start_time_calibrate
des:pps start calibrate
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_start_time_calibrate(void);

/*****************************************************
func:drv_pps_stop_time_calibrate
des:pps stop calibrate
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_pps_stop_time_calibrate(void);

#endif
