/*
 *  drv_barometer.c
 *
 *  Created on: 2017-10-19
 *  Author: wanghechao
 */

#include <stdint.h>
#include <stdio.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>


#include "drv_barometer.h"
#include "bme280.h"
#include "bme280_defs.h"


static barometer_config_t  g_barometer_config = {-1, NULL, 0, 0};


/*****************************************************
func:user_i2c_write
desc:i2c write function
para:dev_id is address of device,reg_addr is address that needed to be write,
    data is the data that needed to be write,len is the length of the data
return:BME280_OK is successful, other value is failed
*****************************************************/
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr,uint8_t *data, uint16_t len)
{

    uint8_t write_buf[64]; //write data buf
    //int nbytes = 0;
    int ret = -1;
    struct i2c_msg msg; //i2c communication
    struct i2c_rdwr_ioctl_data ioctl_data;
    //nbytes = num_bytes + 1;
    if(data == NULL)
        return BME280_E_NULL_PTR;
    
    if(g_barometer_config.fd <= 0)
    {
        return BME280_E_DEV_NOT_FOUND;
    }
    
    write_buf[0] = reg_addr;
    memset(&msg, 0, sizeof(struct i2c_msg));
    memcpy(&write_buf[1],data,len);
    
    msg.addr = dev_id;
    msg.len = len + 1;
    msg.buf = write_buf;
    msg.flags = 0;  //i2c write flag
    ioctl_data.nmsgs = 1;
    ioctl_data.msgs = &msg;
    ret = ioctl(g_barometer_config.fd, I2C_RDWR, &ioctl_data);
    if(ret > 0)
         return BME280_OK;
     else 
     {
         printf("user_i2c_write,return :%d\n", ret);
         return BME280_E_DEV_NOT_FOUND;
     }
}

/*****************************************************
func:user_i2c_read
desc:i2c write function
para:dev_id is address of device,reg_addr is address that needed to be need,
    data is the buf that needed to be read,len is the length of the data that needed to read
return:BME280_OK is successful, other value is failed
*****************************************************/
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr,uint8_t *data, uint16_t len)
{
     struct i2c_msg msgs[2];
     struct i2c_rdwr_ioctl_data ioctl_data;
     int ret = -1;
     //unsigned char data_buf[WRITE_READ_BUF_NUM];
     uint8_t addr_buf[2];
     memset(&msgs, 0, sizeof(msgs));

     if(data == NULL)
         return BME280_E_NULL_PTR;

    if(g_barometer_config.fd <= 0)
    {
        return BME280_E_DEV_NOT_FOUND;
    }
    
     addr_buf[0] = reg_addr;

     msgs[0].addr = dev_id;
     msgs[0].len = 1;
     msgs[0].buf = addr_buf;
     msgs[0].flags = 0;
     msgs[1].addr = dev_id;
     msgs[1].flags |= I2C_M_RD;  //i2c read flag
     msgs[1].len = len;
     msgs[1].buf = data;
     msgs[1].buf[0] = 0;
     ioctl_data.nmsgs = 2;
     ioctl_data.msgs = msgs;
     ret = ioctl(g_barometer_config.fd, I2C_RDWR, &ioctl_data);
     
     if(ret > 0)
         return BME280_OK;
     else 
     {
         printf("user_i2c_read,return :%d\n", ret);
         return BME280_E_DEV_NOT_FOUND;
     }
}

/*barometer delay function*/
void user_delay_ms(uint32_t period)
{
    usleep(period*1000);
}

/*****************************************************
func:atmospheric_to_altitude
desc:translate the atmospheric to altitude
para:atmospheric is the pressure of atmospheric(hPa),seaLevel is the standard pressure of atmospheric(hPa)
return:altitude (unit(m))
*****************************************************/
double atmospheric_to_altitude(double atmospheric, double seaLevel)
{        
    return 44330000 * (1.0 - pow(atmospheric / seaLevel, 1.0F/5255.0F));
}

/*****************************************************
func:atmospheric_to_altitude
desc:translate the atmospheric to altitude
para:atmospheric is the pressure of atmospheric(hPa),altitude is the height above sealevel(m)
return:seaLevel(seaLevel is the standard pressure of atmospheric(hPa))
*****************************************************/
double altitude_to_sealevel(double atmospheric,double altitude)
{
    return atmospheric / pow((1 - altitude / 44330000), 5255);
}


/*****************************************************
func:gps_thread_entry
desc:gps read data thread
para:parameter is the fd of gps uart
return:NULL
*****************************************************/
void *barometer_thread_entry(void *parameter)
{
    struct bme280_dev *dev = (struct bme280_dev *)parameter;

    int8_t rslt;
    uint8_t settings_sel;
    struct bme280_data comp_data;
    barometer_data_t barometer_data;
    uint32_t len = 0;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;
    dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
    
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    if(rslt != BME280_OK)
    {
        printf("bme280_set_sensor_settings failed,errcode=%d\n", rslt);
    }
    
    rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);
    if(rslt != BME280_OK)
    {
        printf("bme280_set_sensor_mode failed,errcode=%d\n", rslt);
    }
    
    while(1) /*recv gps data*/
    {
        dev->delay_ms(70);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        if(rslt == BME280_OK)
        {
            barometer_data.humidity = comp_data.humidity;
            barometer_data.pressure = comp_data.pressure;
            barometer_data.temperature = comp_data.temperature;
            len = 1;
            if(g_barometer_config.barometer_callback != NULL)
                (g_barometer_config.barometer_callback)(&barometer_data, len);
        }
        else
        {
            printf("bme280_get_sensor_data failed,errcode=%d\n", rslt);
        }
    }
    return NULL;
}


/*****************************************************
func:drv_barometer_init
desc:init barometer device
para:callback_fun is callback function
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_barometer_init(barometer_callback_fun barometer_callback)
{
    int ret = ERR_OK;
    struct bme280_dev dev;
    
    if(barometer_callback == NULL)
    {
        return -ERR_INVAL;
    }
    
    if(g_barometer_config.fd > 0)
    {
        printf("drv_barometer_init already init\n");
        return ERR_OK;
    }

    g_barometer_config.barometer_callback = barometer_callback; //callback function

    /*open the i2c bus*/
    g_barometer_config.fd = open(BAROMETER_DEV_BUS,O_RDWR);
    if(g_barometer_config.fd <= 0)
    {
        printf("Failed to open the bus.\n");
        ret = -(g_barometer_config.fd);
        goto error;
    }

    /*init bme280 dev*/
    dev.dev_id = BME280_I2C_ADDR_PRIM;
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_ms = user_delay_ms;

    ret = bme280_init(&dev);
    if(ret != BME280_OK)
    {
        printf("bme280_init failed,errcode=%d\n",ret);
        ret = -ERR_SYS;
        goto error;
    }

    /*create deal gps barometer data thread*/
    ret = osal_task_create(&(g_barometer_config.task_barometer), "tk_barometer", barometer_thread_entry, &dev,BAROMETER_THREAD_STACK_SIZE, BAROMETER_THREAD_PRIORITY);
    if(ret < 0)
    {
        printf("[%s][%d] osal_task_create error\n",__FUNCTION__,__LINE__);
        goto error;
    }
    g_barometer_config.task_barometer_valid = 1;
    
    return ret;
error:
    drv_barometer_deinit();
    return ret;
}

/*****************************************************
func:drv_barometer_deinit
desc:deinit barometer device
para:NULL
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_barometer_deinit(void)
{
    int ret = ERR_OK;
    if(g_barometer_config.barometer_callback != NULL)
    {
        g_barometer_config.barometer_callback = NULL;
    }

    if(g_barometer_config.task_barometer_valid)
    {
        ret = osal_task_del(g_barometer_config.task_barometer);
        if(ret < 0)
            goto error;

        g_barometer_config.task_barometer_valid = 0;
    }

    if(g_barometer_config.fd > 0)
    {
        close(g_barometer_config.fd);
    }

    memset(&g_barometer_config, 0, sizeof(g_barometer_config));
error:
    return ret;
}




