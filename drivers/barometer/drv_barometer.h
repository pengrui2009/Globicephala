#ifndef _DRV_BAROMETER_H_
#define _DRV_BAROMETER_H_

#include <osal_mutex.h>
#include <osal_thread.h>
#include <error.h>

#define BAROMETER_DEV_BUS	"dev/i2c-1"

#define BAROMETER_THREAD_STACK_SIZE	(1024*8192) /*gps thread stack size,8K*/
#define BAROMETER_THREAD_PRIORITY		(TK_PRIO_DEFAULT) /*gps ubx thread priority*/


/*the data of collected */
typedef struct _barometer_data_t
{
	/* Compensated pressure */
	double pressure;
	/* Compensated temperature */
	double temperature;
	/* Compensated humidity */
	double humidity; 
}barometer_data_t,*barometer_data_t_ptr;

typedef void (*barometer_callback_fun)(barometer_data_t_ptr, uint32_t); /*barometer callback type,barometer_data_t_ptr is data buf,uint32_t is the length of data buf*/

/*barometer config type*/
typedef struct _barometer_config_t
{
	int fd; //barometer device descstor
	barometer_callback_fun barometer_callback; //barometer callback function
	osal_task_t task_barometer; //the task of barometer is that collect data
	uint8_t task_barometer_valid;//the flag of barometer, 0:invalid 1:valid
}barometer_config_t;

/*****************************************************
func:drv_barometer_init
desc:init barometer device
para:callback_fun is callback function
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_barometer_init(barometer_callback_fun barometer_callback);

/*****************************************************
func:drv_barometer_deinit
desc:deinit barometer device
para:NULL
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_barometer_deinit(void);

/*****************************************************
func:atmospheric_to_altitude
desc:translate the atmospheric to altitude
para:atmospheric is the pressure of atmospheric(hPa),seaLevel is the standard pressure of atmospheric(hPa)
return:altitude (unit(m))
*****************************************************/
double atmospheric_to_altitude(double atmospheric, double seaLevel);

/*****************************************************
func:atmospheric_to_altitude
desc:translate the atmospheric to altitude
para:atmospheric is the pressure of atmospheric(hPa),altitude is the height above sealevel(m)
return:seaLevel(seaLevel is the standard pressure of atmospheric(hPa))
*****************************************************/
double altitude_to_sealevel(double atmospheric,double altitude);



#endif