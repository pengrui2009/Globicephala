#ifndef _UBLOX_H_
#define _UBLOX_H_

#include <unistd.h>
#include <stdint.h>

#include "gps_drv.h"


#define UBX_CFG_DELAY	usleep(100000)  /*ubx cfg command wait time*/
#define UBX_SYNC_CHAR1  0xB5 /*ubx sync character 1*/
#define UBX_SYNC_CHAR2  0x62 /*ubx sync character 2*/

/*nmea message type id*/
typedef enum ubx_cfg_msg_nmea_id
{
    STD_NMEA_ID_GGA = 0x00,
    STD_NMEA_ID_GLL,
    STD_NMEA_ID_GSA,
    STD_NMEA_ID_GSV,
    STD_NMEA_ID_RMC,
    STD_NMEA_ID_VTG,
    
    STD_NMEA_ID_GRS,
    STD_NMEA_ID_GST,
    STD_NMEA_ID_ZDA,
    STD_NMEA_ID_GBS,
    STD_NMEA_ID_DTM,
    
    STD_NMEA_ID_END,
                 
}ubx_cfg_msg_nmea_id_t;

/*ublox gps type of combination*/
typedef enum ubx_cfg_gnss_type
{
    UBX_CFG_GNSS_GPS_ONLY = 0x00, /*gps only*/
    UBX_CFG_GNSS_GPS_GLONASS, /*gps and glonass*/
    UBX_CFG_GNSS_GPS_BEIDOU,  /*gps and beidou*/ 
    UBX_CFG_GNSS_END, /*end symbol*/
}ubx_cfg_gnss_type_t;

/*ublox gps reset mode*/
typedef enum ubx_cfg_reset_mode
{
	UBX_CFG_RESET_HOTSTART = 0x00,
	UBX_CFG_RESET_WARMSTART,
	UBX_CFG_RESET_COLDSTART,
	UBX_CFG_RESET_END
}ubx_cfg_reset_mode_t;

/*ublox ubx header of packet*/
typedef struct _ubx_pkt_hdr
{
    uint8_t syncChar1;
    uint8_t syncChar2;
    uint8_t msgClass;
    uint8_t msgId;
    uint16_t length;
}gps_ubx_pkt_hdr_t;

/*ublox config nmea type */
typedef struct _ubx_cfg_msg
{
    uint8_t nmeaClass;
    uint8_t nmeaid;
    uint8_t portOn[6]; //send to port: i2c, uart1, uart2, usb, spi, other. 
}gps_ubx_cfg_msg_t;

/*****************************************************
func:ubx_cfg_uart_port
desc:set ublox the baudrate of uart
para:device_num is the fd of uart,baudrate is that need set baudrate
return:NULL
*****************************************************/
void ubx_cfg_uart_port(int device_num,int baudrate);

/*****************************************************
func:ubx_nav_timegps_off
desc:close ublox NAV_TIMEGPS message out
para:gps_config is the config of gps
return:NULL
*****************************************************/
void ubx_nav_timegps_off(gps_config_t *gps_config);

/*****************************************************
func:ubx_pkt_checknum_calc
desc:calc the checksum of ublox packet
para:buf is data,len is length of data,cknumA is one of the finished checksum,cknumB is the two of the finished checksum
return:NULL
*****************************************************/
void ubx_pkt_checknum_calc(uint8_t *buf, int len, uint8_t *cknumA, uint8_t *cknumB);

/*****************************************************
func:gps_chip_config
desc:configure gps
para:gps_config is the config of gps
return:NULL
*****************************************************/
void gps_chip_config(gps_config_t *gps_config);


#endif
