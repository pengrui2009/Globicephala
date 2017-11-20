#ifndef _GPS_DRV_H_
#define _GPS_DRV_H_

#include <stdint.h>
#include <os_pthread.h>
#include <osal_queue.h>
#include <os_core.h>

#define GPS_MODE_NORMAL		0 /*gps normal mode */
#define GPS_MODE_RSU		1 /*gps rsu mode */

#define GPS_BUFF_SIZE 		1024  /*gps recv buff size*/
#define GPS_PIPE 			30  /*gps buffer pipe num*/

#define GPS_DEV				"/dev/ttymxc2"  /*gps device node*/
#define GPS_THREAD_STACK_SIZE	(1024*8192) /*gps thread stack size,8K*/
#define GPS_THREAD_PRIORITY		(TK_PRIO_DEFAULT + 10) /*gps thread priority*/
#define GPS_UBX_THREAD_PRIORITY		(TK_PRIO_DEFAULT) /*gps ubx thread priority*/

#define GPS_UBX_QUEUE_NUM 	100  /*gps ubx data queue num*/
#define GPS_UBX_QUEUE_MSG_SIZE 	512 /*gps ubx data queue size*/

/*gps recv buff type*/
typedef struct {
	uint8_t Flag;
	uint8_t Buf[GPS_BUFF_SIZE];
	uint32_t Len;
} t_buff;

/*gps recv pipe type*/
typedef struct {
	uint8_t Pipe;
	t_buff PpBuf[GPS_PIPE];
}t_gps_buff;

typedef void (*gps_callback_fun)(uint8_t *, uint32_t); /*gps callback type,uint8_t * is data buf,uint32_t is the length of data buf*/

/*gps config type*/
typedef struct _gps_config
{
	uint8_t mode;//gps mode
	int     port;//no need init,no need set
	uint8_t gps_nmea_freq; // Hz, (1,2,5,10)
	uint32_t base_min_observation_time;//the min observation time of base mode
	float base_required_position_accu;//the required position accu of base mode
	gps_callback_fun gps_deal_callback;//gps data deal callback
	gps_callback_fun gps_rtcm_callback;//gps rtcm data deal callback
	gps_callback_fun gps_ublox_leapsecond_callback;//ublox leapsecond flag callback
} gps_config_t;

/*gps deal handle and flag*/
typedef struct _gps_deal_handle
{
	osal_queue_t gps_ubx_queue;  /*the queue of ubx data*/
	osal_task_t task_gps; /*gps receive data task*/
	osal_task_t task_ubx; /*deal ubx data task*/
	uint8_t gps_ubx_queue_valid; /*Whether it is a valid sign,0:invalid,1:valid*/
	uint8_t task_gps_valid; /*Whether it is a valid sign,0:invalid,1:valid*/
	uint8_t task_ubx_valid; /*Whether it is a valid sign,0:invalid,1:valid*/
}gps_deal_handle_t;

/*****************************************************
func:drv_gps_init
desc:init gps device
para:gps_config is the config of gps
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_gps_init(gps_config_t gps_config);

/*****************************************************
func:drv_gps_deinit
desc:deinit gps device
para:NULL
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_gps_deinit(void);

#endif