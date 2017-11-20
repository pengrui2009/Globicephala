#ifndef _LED_DRV_H_
#define _LED_DRV_H_

#include <stdint.h>
#include <os_core.h>
#include <osal_mutex.h>
#include <osal_timer.h>


#define LED_NUM        3  /*led numbers*/
#define LED_ON        1  /*1:ON*/
#define LED_OFF        0  /*0:OFF*/


#define LED_BLINK_HZ_1    500   /*LED blink freq: 1Hz*/
#define LED_BLINK_HZ_2  250    /*LED blink freq: 2Hz*/

/*LED ID*/
typedef enum _led_color_em
{
    LED_YELLOW = 0,
    LED_RED    = 1,
    LED_GREEN  = 2
}led_color_em;

/*LED status type*/
typedef enum _led_stat_em
{
    LED_STAT_OFF        = 0,
    LED_STAT_ON         = 1,
    LED_STAT_SLOW_BLINK = 2,
    LED_STAT_FAST_BLINK = 3
}led_stat_em;

/*LED unit type*/
typedef struct _led_array_st
{
    osal_timer_t led_proc_timer; /*led timer*/
    uint8_t led_proc_timer_valid; /*Whether it is a valid sign,0:invalid,1:valid*/
    int led_fd; /*led file descriptor*/
    uint8_t led_param; /*led status:LED_ON or LED_OFF*/
    uint16_t led_period;/*led blink freq:LED_BLINK_HZ_1 or LED_BLINK_HZ_2*/
}led_array_st;

/*led config type*/
typedef struct _drv_led_st
{
    osal_mutex_t led_proc_mutex; /*led operate mutex*/
    uint8_t led_proc_mutex_valid; /*Whether it is a valid sign,0:invalid,1:valid*/
    led_array_st led[LED_NUM]; /*led unit array*/
}drv_led_st,*drv_led_st_ptr;

/*****************************************************
func:drv_led_init
des:led init
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_led_init(void);

/*****************************************************
func:drv_led_deinit
des:led deinit
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_led_deinit(void);

/*****************************************************
func:drv_led_set_stat
des:led set status
para: led_color is id of leds, led_stat is status of leds.
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_led_set_stat(led_color_em led_color, led_stat_em led_stat);


#endif

