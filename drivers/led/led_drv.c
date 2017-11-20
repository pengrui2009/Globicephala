/*
 *  led_drv.c
 *
 *  Created on: 2017-9-27
 *  Author: wanghechao
 */
#include <stdint.h>
#include <error.h>

#include "led_drv.h"


static drv_led_st_ptr led_proc;  /*led config:global value*/
static char *LED_ARRAY[LED_NUM] = {"/dev/led0", "/dev/led1", "/dev/led2"}; /*device node*/


/*****************************************************
func:led_on_off
des:led on or off
para:led_color is led id,value is LED_ON or LED_OFF
return:ERR_OK is successful, other value is failed
*****************************************************/
static int led_on_off(led_color_em led_color, uint8_t value)
{
    int ret = ERR_OK;
    uint8_t on_off = value;
    if(led_proc == NULL)
    {
        ret = -ERR_NOCFG;
        goto error;
    }
    
    switch(led_color)
    {
        case LED_YELLOW:
            ret = write(led_proc->led[LED_YELLOW].led_fd, &on_off, sizeof(on_off));
            break;
        case LED_RED:
            ret = write(led_proc->led[LED_RED].led_fd, &on_off, sizeof(on_off));
            break;
        case LED_GREEN:
            ret = write(led_proc->led[LED_GREEN].led_fd, &on_off, sizeof(on_off));
            break;
        default:
            ret = -ERR_NODEV;
            goto error;
            break;
    }
error:
    return ret;
}

/*change yellow led status callback*/
static void timer_led_yellow_callback(void *arg)
{
    if(led_proc == NULL)
        return;
    
    if(led_proc->led[LED_YELLOW].led_param == LED_OFF)
    {
        led_on_off(LED_YELLOW, led_proc->led[LED_YELLOW].led_param);
        led_proc->led[LED_YELLOW].led_param = LED_ON;
    }else{
        led_on_off(LED_YELLOW, led_proc->led[LED_YELLOW].led_param);
        led_proc->led[LED_YELLOW].led_param = LED_OFF;
    }
}

/*change red led status callback*/
static void timer_led_red_callback(void *arg)
{
    if(led_proc == NULL)
        return;
    
    if(led_proc->led[LED_RED].led_param == LED_OFF)
    {
        led_on_off(LED_RED, led_proc->led[LED_RED].led_param);
        led_proc->led[LED_RED].led_param = LED_ON;
    }else{
        led_on_off(LED_RED, led_proc->led[LED_RED].led_param);
        led_proc->led[LED_RED].led_param = LED_OFF;
    }
}

/*change green led status callback*/
static void timer_led_green_callback(void *arg)
{
    if(led_proc == NULL)
        return;
    
    if(led_proc->led[LED_GREEN].led_param == LED_OFF)
    {
        led_on_off(LED_GREEN, led_proc->led[LED_GREEN].led_param);
        led_proc->led[LED_GREEN].led_param = LED_ON;
    }else{
        led_on_off(LED_GREEN, led_proc->led[LED_GREEN].led_param);
        led_proc->led[LED_GREEN].led_param = LED_OFF;
    }
}

/*****************************************************
func:drv_led_init
des:led init
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_led_init(void)
{
    int ret = ERR_OK;
    int ret_deinit = ERR_OK;
    int i = 0;

    if(led_proc != NULL) /*led_proc is inited*/
    {
        printf("drv_led_init has been inited\n");
        return ret;
    }
    /*alloc one block of memory for led_proc*/
    led_proc = (drv_led_st_ptr)os_calloc(1, sizeof(drv_led_st));
    if(led_proc == NULL)
    {
        printf("[%s][%d] os_calloc error\n",__FUNCTION__,__LINE__);
        ret = -ERR_NOINIT;
        goto error;
    }

    /*open all of leds and create timers of leds;create mutex*/
    for(i = 0;i < LED_NUM;i++)
    {
        /*open the leds*/
        led_proc->led[i].led_fd = open(LED_ARRAY[i], O_RDWR);
        if(led_proc->led[i].led_fd < 0)
        {
            printf("[%s][%d] open error\n",__FUNCTION__,__LINE__);
            ret = -ERR_NODEV;
            goto error;
        }
        led_proc->led[i].led_param = LED_OFF; /*init the status of led is OFF*/
        led_on_off(i, led_proc->led[i].led_param);
        switch(i)
        {
            case LED_YELLOW:
                /*create led proc timer*/
                 ret = osal_timer_create(&(led_proc->led[LED_YELLOW].led_proc_timer), "LED_YELLOW",timer_led_yellow_callback, NULL, LED_BLINK_HZ_1, TIMER_INTERVAL|TIMER_STOPPED, TIMER_PRIO_NORMAL);
                if(ret < 0)
                {
                    goto error;
                }
                led_proc->led[LED_YELLOW].led_proc_timer_valid = 1;
                led_proc->led[LED_YELLOW].led_period = LED_BLINK_HZ_1;
                break;
            case LED_RED:
                /*create led proc timer*/
                ret = osal_timer_create(&(led_proc->led[LED_RED].led_proc_timer), "LED_RED",timer_led_red_callback, NULL, LED_BLINK_HZ_1, TIMER_INTERVAL|TIMER_STOPPED, TIMER_PRIO_NORMAL);
                if(ret < 0)
                {
                    goto error;
                }
                led_proc->led[LED_RED].led_proc_timer_valid = 1;
                led_proc->led[LED_RED].led_period = LED_BLINK_HZ_1;
                break;
            case LED_GREEN:
                /*create led proc timer*/
                 ret = osal_timer_create(&(led_proc->led[LED_GREEN].led_proc_timer), "LED_GREEN",timer_led_green_callback, NULL, LED_BLINK_HZ_1, TIMER_INTERVAL|TIMER_STOPPED, TIMER_PRIO_NORMAL);
                if(ret < 0)
                {
                    ret = -ERR_NOINIT;
                    goto error;
                }
                led_proc->led[LED_GREEN].led_proc_timer_valid = 1;
                led_proc->led[LED_GREEN].led_period = LED_BLINK_HZ_1;
                break;
            default:
                break;
        }
        
    }
    /*create proc mutex*/
     ret = osal_mutex_create(&(led_proc->led_proc_mutex));
    if(ret < 0)
    {
        printf("[%s][%d] osal_mutex_create error\n",__FUNCTION__,__LINE__);
        goto error;
    }
    led_proc->led_proc_mutex_valid = 1;
    
    return ret;
    
error:
    printf("drv_led_init failed\n");
    ret_deinit = drv_led_deinit();
    if(ret_deinit != ERR_OK)
        printf("free resouce failed\n");
    return ret;
}

/*****************************************************
func:drv_led_deinit
des:led deinit
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_led_deinit(void)
{
    int ret = ERR_OK;
    int i;
    if(led_proc != NULL)
    {
        /*close all of led fds,delete all of timers*/
        for(i = 0;i < LED_NUM;i++)
        {
            if(led_proc->led[i].led_proc_timer_valid)
            {
                ret = osal_timer_delete(led_proc->led[i].led_proc_timer);
                if(ret != ERR_OK)
                    goto error;
                
                led_proc->led[i].led_proc_timer_valid = 0;
            }

            led_on_off(i, LED_OFF);
            
            if(led_proc->led[i].led_fd > 0)
            {
                ret = close(led_proc->led[i].led_fd);
                if(ret != ERR_OK)
                    goto error;
                
                led_proc->led[i].led_fd = -1;
            }
        }
        /*delete led proc mutex*/
        if(led_proc->led_proc_mutex_valid)
        {
            ret = osal_mutex_delete(&(led_proc->led_proc_mutex));
            if(ret != ERR_OK)
                goto error;
            
            led_proc->led_proc_mutex_valid = 0;
        }

        os_free(led_proc); /*free memory*/
        led_proc = NULL;
    }
    
    return ret;
error:
    printf("drv_led_deinit failed\n");
    return ret;
}

/*****************************************************
func:drv_led_set_stat
des:led set status
para: led_color is id of leds, led_stat is status of leds.
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_led_set_stat(led_color_em led_color, led_stat_em led_stat)
{    
    int ret = ERR_OK;

    /*lock mutex*/
    if(led_proc != NULL)
    {
        if(led_proc->led_proc_mutex_valid)
        {
            ret = osal_mutex_lock(&(led_proc->led_proc_mutex));
            if(ret < 0)
            {
                printf("[%s][%d] osal_mutex_lock error\n",__FUNCTION__,__LINE__);
                goto error;
            }
        }

        if(led_proc->led[led_color].led_proc_timer_valid);
        {
            /*set led status*/
            switch(led_stat)
            {
                case LED_STAT_OFF:
                    ret = osal_timer_stop(led_proc->led[led_color].led_proc_timer);
                    if(ret < 0)
                        goto error;
                    
                    ret = led_on_off(led_color, LED_OFF);
                    if(ret < 0)
                        goto error;
                    break;
                case LED_STAT_ON:
                    ret = osal_timer_stop(led_proc->led[led_color].led_proc_timer);
                    if(ret < 0)
                        goto error;

                    ret = led_on_off(led_color, LED_ON);
                    if(ret < 0)
                        goto error;
                    break;
                case LED_STAT_SLOW_BLINK:
                    if(led_proc->led[led_color].led_period != LED_BLINK_HZ_1)
                    {
                        led_proc->led[led_color].led_period = LED_BLINK_HZ_1;
                        ret = osal_timer_change(led_proc->led[led_color].led_proc_timer, led_proc->led[led_color].led_period);
                        if(ret < 0)
                            goto error;
                    }
                    
                    ret = osal_timer_start(led_proc->led[led_color].led_proc_timer);
                    if(ret < 0)
                        goto error;
                    
                    break;
                case LED_STAT_FAST_BLINK:
                    if(led_proc->led[led_color].led_period != LED_BLINK_HZ_2)
                    {
                        led_proc->led[led_color].led_period = LED_BLINK_HZ_2;
                        ret = osal_timer_change(led_proc->led[led_color].led_proc_timer, led_proc->led[led_color].led_period);
                        if(ret < 0)
                            goto error;
                    }
                    
                    ret = osal_timer_start(led_proc->led[led_color].led_proc_timer);
                    if(ret < 0)
                        goto error;
                    
                    break;
                default:
                    break;
            }
        }
        
        /*unlock mutex*/
        if(led_proc->led_proc_mutex_valid)
        {
            ret = osal_mutex_unlock(&(led_proc->led_proc_mutex));
            if(ret < 0)
            {
                printf("[%s][%d] osal_mutex_unlock error\n",__FUNCTION__,__LINE__);
                goto error;
            }
        }
    }
    else
        ret = -ERR_NOCFG;

error:
    return ret;
}

