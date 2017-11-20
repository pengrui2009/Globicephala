/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : ublox.c
 @brief  : ublox gps  chip drvier
 @author : wanghechao
 @history:
           2017-9-28	  wanghechao	Created file
           ...
******************************************************************************/

#include <memory.h>
#include <stdlib.h>
#include "../cmn/uart.h"
#include "ublox.h"
#include "gps_drv.h"

/*****************************************************
func:ubx_pkt_checknum_calc
desc:calc the checksum of ublox packet
para:buf is data,len is length of data,cknumA is one of the finished checksum,cknumB is the two of the finished checksum
return:NULL
*****************************************************/
void ubx_pkt_checknum_calc(uint8_t *buf, int len, uint8_t *cknumA, uint8_t *cknumB)
{
    uint16_t ckA=0, ckB=0;
    int i=0;
    for(i=0; i<len; i++)
    {
        ckA = ckA + buf[i];
        ckB = ckB + ckA;
    }
    *cknumA = ckA;
    *cknumB = ckB;
}

/*****************************************************
func:ubx_cfg_send
desc:send ubx packet
para:device_num is the fd of uart
return:NULL
*****************************************************/
static void ubx_cfg_send(int device_num,uint8_t buf[],int len)
{
	int ret;
//	int i;
	ubx_pkt_checknum_calc(&buf[2], len-4, &buf[len-2], &buf[len-1]);
	ret = comport_send(device_num, buf, len);//
	if(ret < 0)
	{
		printf("[%s][%d] comport_send error\n", __FUNCTION__, __LINE__);
	}
	#if 0
	if(ret < 0)
	{
		printf("ubx_cfg_send error\n");
	}
	printf("[%s] debug message,buf:\n",__FUNCTION__);
	for(i = 0;i < len;i++)
		printf("[0x%02x]",buf[i]);
	printf("\n");
	#endif
}

/*****************************************************
func:ubx_cfg_recv
desc:receive ubx packet
para:device_num is the fd of uart
return:value < 0 is failed
*****************************************************/
static int ubx_cfg_recv(int device_num,uint8_t buf[],int len)
{
	int ret;
	ret = comport_receive(device_num,buf,len);
	if(ret < 0)
	{
		printf("ubx_cfg_recv error\n");
	}
	return ret;
}

/*****************************************************
func:ubx_cfg_ack_nak
desc:judge the ubx packet function(NAK or ACK),only for debug
para:buf is data,len is length of data
return:value < 0 is failed
*****************************************************/
static void ubx_cfg_ack_nak(uint8_t buf[],int len)
{
	uint8_t cknumA,cknumB;
	if(len < 10)
	{
		printf("[%s] data length error,len=%d\n",__FUNCTION__,len);
		return;
	}
	if((buf[0] == UBX_SYNC_CHAR1)&&(buf[1] == UBX_SYNC_CHAR2))
	{
		/*calc the checksum*/
		ubx_pkt_checknum_calc(&buf[2],len-4,&cknumA,&cknumB);
		if((buf[len-2] == cknumA)&&(buf[len-1] == cknumB))
		{
			if(buf[2] == 0x05)
			{
				switch(buf[3])
				{
					case 0:
						printf("[%s] NAK class_id:%x,message_id:%x\n",__FUNCTION__,buf[6],buf[7]);
						break;
					case 1:
						printf("[%s] ACK class_id:%x,message_id:%x\n",__FUNCTION__,buf[6],buf[7]);
						break;
					default:
						printf("[%s] other message\n",__FUNCTION__);
						break;
				}
			}
		}
		else
		{
			printf("[%s] checksum error\n",__FUNCTION__);
		}
		
	}
	else
	{
		printf("[%s] not ubx cfg data\n",__FUNCTION__);
	}
}

/*****************************************************
func:ubx_cfg_save
desc:save the para of ublox
para:device_num is the fd of uart
return:NULL
*****************************************************/
void ubx_cfg_save(int device_num)
{
	uint8_t cfg_pkt_msg_cfg[] = {0xb5,0x62,0x06,0x09,0x0d,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1d,0xab};//save current configure
	ubx_cfg_send(device_num,cfg_pkt_msg_cfg,sizeof(cfg_pkt_msg_cfg));
}

/*****************************************************
fucn:ubx_cfg_revert_to_default
desc:reset the para of ublox
para:device_num is the fd of uart
return:NULL
*****************************************************/
void ubx_cfg_revert_to_default(int device_num)
{
	uint8_t cfg_pkt_msg_cfg_default[] = {0xB5,0x62,0x06,0x09,0x0D,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x03,0x1B,0x9A}; //revert to default configure
	ubx_cfg_send(device_num,cfg_pkt_msg_cfg_default,sizeof(cfg_pkt_msg_cfg_default));
}

/*****************************************************
func:ubx_cfg_msg_std_nmea
desc:enable or disable nema message
para:device_num is the fd of uart,nmea_id is nema id,enable is 1 or 0
return:NULL
*****************************************************/
static void ubx_cfg_msg_std_nmea(int device_num, ubx_cfg_msg_nmea_id_t nmea_id, uint8_t enable)
{
    int len = 0;
    uint8_t buf[20];
 
    gps_ubx_pkt_hdr_t pkt;
    gps_ubx_cfg_msg_t cfg;
    memset(&cfg, 0x0, sizeof(cfg));
    pkt.syncChar1 = UBX_SYNC_CHAR1;
    pkt.syncChar2 = UBX_SYNC_CHAR2;
    pkt.msgClass = 0x06;
    pkt.msgId = 0x01;
    pkt.length = sizeof(gps_ubx_cfg_msg_t);
    len = sizeof(gps_ubx_pkt_hdr_t);
    memcpy(buf, &pkt, len);

    /* standard NMEA messages */
    cfg.nmeaClass = 0xF0;
    cfg.nmeaid = nmea_id;
    cfg.portOn[1] = enable;

    memcpy(buf+len, &cfg, pkt.length);
    len += pkt.length;

    //ubx_pkt_checknum_calc(buf+2, len-2, &buf[len], &buf[len+1]); 
    len += 2;
	
    ubx_cfg_send(device_num, buf, len);
}
#if 0
static void ubx_cfg_sbas(int device_num,uint8_t enable)
{
   /* set baut rate = 115200 */
    uint8_t cfg_sbas[] = {
        0xB5, 0x62, 0x06, 0x16, 0x08, 0x00,0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x2B, 0xB9
    };
    cfg_sbas[6] = enable;
    //printf("buffer is %d\n",sizeof(cfg_sbas));
   // ubx_pkt_checknum_calc(&cfg_sbas[2],sizeof(cfg_sbas)-4,&cfg_sbas[sizeof(cfg_sbas)-2],&cfg_sbas[sizeof(cfg_sbas)-1]);
    //int i = 0;
    //for(;i < sizeof(cfg_sbas);i++)
        //printf("cfg_sbas[%d] is 0x%X\n",i,cfg_sbas[i]);
    
    ubx_cfg_send(device_num,cfg_sbas,sizeof(cfg_sbas));
}
#endif

/*****************************************************
func:ubx_cfg_needed_nmea
desc:set the nema message of needed
para:device_num is the fd of uart
return:NULL
*****************************************************/
static void ubx_cfg_needed_nmea(int device_num)
{
    uint8_t cfg_pkt_msg_rtcm3_2_1077[] = {0xb5,0x62,0x06,0x01,0x08,0x00,0xf5,0x4d,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x66};  /*GPS*/
	uint8_t cfg_pkt_msg_rtcm3_2_1087[] = {0xb5,0x62,0x06,0x01,0x08,0x00,0xf5,0x57,0x00,0x00,0x00,0x00,0x00,0x00,0x5b,0xac};  /*if GLONASS is used*/
    //ubx_cfg_msg_std_nmea(fd, STD_NMEA_ID_GGA, 0);
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GLL, 0);
	UBX_CFG_DELAY;
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GSV, 0);
	UBX_CFG_DELAY;
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_VTG, 0);
	UBX_CFG_DELAY;
	ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GSA, 0);
	UBX_CFG_DELAY;
	ubx_cfg_send(device_num,cfg_pkt_msg_rtcm3_2_1077,sizeof(cfg_pkt_msg_rtcm3_2_1077)); /*disable rtcm 1077*/
	UBX_CFG_DELAY;
	ubx_cfg_send(device_num,cfg_pkt_msg_rtcm3_2_1087,sizeof(cfg_pkt_msg_rtcm3_2_1087)); /*disable rtcm 1087*/
}

/*****************************************************
func:ubx_cfg_recovery
desc:recovery the nema message of needed
para:device_num is the fd of uart
return:NULL
*****************************************************/
void ubx_cfg_recovery(int device_num)
{
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GGA, 1);
	UBX_CFG_DELAY;
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GLL, 1);
	UBX_CFG_DELAY;
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GSV, 1);
	UBX_CFG_DELAY;
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_VTG, 1);
	UBX_CFG_DELAY;
	ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GSA, 1);
}


/*****************************************************
func:ubx_cfg_uart_port
desc:set ublox the baudrate of uart
para:device_num is the fd of uart,baudrate is that need set baudrate
return:NULL
*****************************************************/
void ubx_cfg_uart_port(int device_num,int baudrate)
{
	//int len;
	uint8_t cfg_pkt[] = {
	        0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0,
	        0x08, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x23, 0x00, 0x03, 0x00, 
	        0x00, 0x00, 0x00, 0x00, 0xDC, 0x5E
	    };// 8N1
	//len = sizeof(cfg_pkt);
	switch(baudrate)
	{
		case 115200:
			cfg_pkt[14] = 0x00;
			cfg_pkt[15] = 0xC2;
			cfg_pkt[16] = 0x01;
			cfg_pkt[17] = 0x00;
	   		break;
		case 9600:
			cfg_pkt[14] = 0x80;
			cfg_pkt[15] = 0x25;
			cfg_pkt[16] = 0x00;
			cfg_pkt[17] = 0x00;
			break;
		default:
			cfg_pkt[14] = 0x80;
			cfg_pkt[15] = 0x25;
			cfg_pkt[16] = 0x00;
			cfg_pkt[17] = 0x00;
			break;
	    
	}
	ubx_cfg_send(device_num,cfg_pkt,sizeof(cfg_pkt));  
}


/*****************************************************
func:ubx_cfg_nmea_freq
desc:set message freq of output
para:device_num is the fd of uart,freq is that need set freq
return:NULL
*****************************************************/
void ubx_cfg_nmea_freq(int device_num, uint8_t freq)
{
    uint8_t cfg_pkt[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xF4, 0x01, 
                         0x01, 0x00, 0x01, 0x00, 0x0B, 0x77}; //2Hz

    switch (freq)
    {
        case 1: //1Hz
        {
            cfg_pkt[6] = 0xE8;
            cfg_pkt[7] = 0x03;
            break;
        }
        case 2: //2Hz
        {
            cfg_pkt[6] = 0xF4;
            cfg_pkt[7] = 0x01;
            break;
        }
        case 5:  //5Hz
        {
            cfg_pkt[6] = 0xC8;
            cfg_pkt[7] = 0x00;
            break;
        }
		case 10: //10Hz
		{
			cfg_pkt[6] = 0x64;
            cfg_pkt[7] = 0x00;
            break;
		}
        default: //1Hz
        {
			cfg_pkt[6] = 0xE8;
            cfg_pkt[7] = 0x03;
            break;
        }
    }
    ubx_cfg_send(device_num,cfg_pkt,sizeof(cfg_pkt));  
}

/*****************************************************
func:ubx_nav_timegps_off
desc:close ublox NAV_TIMEGPS message out
para:gps_config is the config of gps
return:NULL
*****************************************************/
void ubx_nav_timegps_off(gps_config_t *gps_config)
{
	if(gps_config->mode == GPS_MODE_RSU)  /*RSU mode*/
	{
		//uint8_t cfg_pkt_msg_nav_timegps[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x20,0x00,0x01,0x00,0x00,0x00,0x00,0x31,0x90};
		uint8_t cfg_pkt_msg_nav_timegps[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x8B};
		ubx_cfg_send(gps_config->port,cfg_pkt_msg_nav_timegps,sizeof(cfg_pkt_msg_nav_timegps));
		UBX_CFG_DELAY;
	}
	else  /*normal mode*/
	{
		uint8_t cfg_pkt_msg_prt[] = {0xb5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xd0,0x08,0x00,0x00,0x00,0xc2,0x01,0x00,0x23,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0xdb,0x58};/*uart1 115200 ubx-nmea-rtcm3 in,nmea out*/
		ubx_cfg_send(gps_config->port,cfg_pkt_msg_prt,sizeof(cfg_pkt_msg_prt));
		UBX_CFG_DELAY;
	}
}

/*****************************************************
func:ubx_cfg_rover_mode
desc:set ublox to rover mode
para:device_num is the fd of uart
return:NULL
*****************************************************/
void ubx_cfg_rover_mode(uint16_t device_num)
{
	//uint8_t cfg_pkt_msg_prt[] = {0xb5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xd0,0x08,0x00,0x00,0x80,0x25,0x00,0x00,0x23,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0xbe,0x95};/*uart1 9600 ubx-nmea-rtcm3 in,nmea out*/
	//uint8_t cfg_pkt_msg_prt[] = {0xb5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xd0,0x08,0x00,0x00,0x00,0xc2,0x01,0x00,0x23,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0xdb,0x58};/*uart1 115200 ubx-nmea-rtcm3 in,nmea out*/
	uint8_t cfg_pkt_msg_prt[] = {0xb5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xd0,0x08,0x00,0x00,0x00,0xc2,0x01,0x00,0x23,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0xdb,0x58};/*uart1 115200 ubx-nmea-rtcm3 in,ubx-nmea out*/
	//uint8_t cfg_pkt_msg_prt[] = {0xb5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xd0,0x08,0x00,0x00,0x80,0x25,0x00,0x00,0x23,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0xbe,0x95};/*uart1 9600 ubx-nmea-rtcm3 in,ubx-nmea out*/
	uint8_t cfg_pkt_nmea[] = {0xb5,0x62,0x06,0x17,0x14,0x00,0x00,0x41,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x75,0x57};/*NMEA 4.1*/
	//uint8_t cfg_pkt_nav_mode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x04,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x0A,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87,0x3C};/*dynamic model:automotive*/
	uint8_t cfg_pkt_nav_mode[] = {0xB5,0x62,0x06,0x24,0x24,0x00,0xFF,0xFF,0x00,0x03,0x00,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x0A,0x00,0xFA,0x00,0xFA,0x00,0x64,0x00,0x5E,0x01,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x83,0xB4};/*dynamic model:portable*/
	//uint8_t cfg_pkt_msg_relposned[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x3C,0x00,0x01,0x00,0x00,0x00,0x00,0x4D,0x54}; /*enable output NAV_RELPOSNED*/
	//uint8_t cfg_pkt_msg_rxm_rtcm[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0x02,0x32,0x00,0x01,0x00,0x00,0x00,0x00,0x44,0x16};  /*enable output RXM_RTCM*/
	uint8_t cfg_pkt_msg_nav_timegps[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x20,0x00,0x01,0x00,0x00,0x00,0x00,0x31,0x90};
	
	ubx_cfg_send(device_num,cfg_pkt_nmea,sizeof(cfg_pkt_nmea));
	UBX_CFG_DELAY;
	ubx_cfg_send(device_num,cfg_pkt_nav_mode,sizeof(cfg_pkt_nav_mode));
	UBX_CFG_DELAY;

	ubx_cfg_send(device_num,cfg_pkt_msg_nav_timegps,sizeof(cfg_pkt_msg_nav_timegps));
	UBX_CFG_DELAY;
	//ubx_cfg_send(device_num,cfg_pkt_msg_relposned,sizeof(cfg_pkt_msg_relposned));
	//UBX_CFG_DELAY;
	//ubx_cfg_send(device_num,cfg_pkt_msg_rxm_rtcm,sizeof(cfg_pkt_msg_rxm_rtcm));
	//UBX_CFG_DELAY;
	ubx_cfg_send(device_num,cfg_pkt_msg_prt,sizeof(cfg_pkt_msg_prt));
	UBX_CFG_DELAY;
}

/*****************************************************
func:base_mode_test
desc:set ublox to the test mode of base
para:device_num is the fd of uart
return:NULL
*****************************************************/
void base_mode_test(uint16_t device_num)
{
	
	uint8_t cfg_pkt_msg_nav_svin[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x3B,0x00,0x01,0x00,0x00,0x00,0x00,0x4C,0x4D}; /*enable output NAV_SVIN*/
	
	#if 0
	ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GLL, 0);
	UBX_CFG_DELAY;
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GSV, 0);
	UBX_CFG_DELAY;
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_VTG, 0);
	UBX_CFG_DELAY;
	ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GST, 0);
	UBX_CFG_DELAY;
	ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_RMC, 1);
	UBX_CFG_DELAY;
    ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GGA, 1);
	UBX_CFG_DELAY;
	ubx_cfg_msg_std_nmea(device_num, STD_NMEA_ID_GSA, 0);
	//UBX_CFG_DELAY;
	#endif
	ubx_cfg_send(device_num,cfg_pkt_msg_nav_svin,sizeof(cfg_pkt_msg_nav_svin));
	UBX_CFG_DELAY;
	
	//ubx_cfg_nmea_freq(device_num,5);
	//UBX_CFG_DELAY;
}

/*****************************************************
func:ubx_cfg_base_mode
desc:set ublox to base mode
para:device_num is the fd of uart,min_observation_time is the min observation time,required_position_accu is the required position accu,gnss_type is gps combine type
return:NULL
*****************************************************/
void ubx_cfg_base_mode(uint16_t device_num, uint32_t min_observation_time, float required_position_accu, ubx_cfg_gnss_type_t gnss_type)
{
	uint32_t required_position_accu_uint = 0;
	//uint8_t cfg_pkt_msg_prt[] = {0xb5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xd0,0x08,0x00,0x00,0x80,0x25,0x00,0x00,0x01,0x00,0x23,0x00,0x00,0x00,0x00,0x00,0xbc,0x45};/*uart1 9600 ubx in,ubx+nmea+rtcm out*/
	uint8_t cfg_pkt_msg_prt[] = {0xb5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xd0,0x08,0x00,0x00,0x00,0xc2,0x01,0x00,0x01,0x00,0x23,0x00,0x00,0x00,0x00,0x00,0xda,0x0e};/*uart1 115200 ubx in,ubx+nmea+rtcm out*/
	uint8_t cfg_pkt_msg_rtcm3_2_1005[] = {0xb5,0x62,0x06,0x01,0x08,0x00,0xf5,0x05,0x00,0x01,0x00,0x00,0x00,0x00,0x0a,0x73};  /*Station coordinates*/
	uint8_t cfg_pkt_msg_rtcm3_2_1077[] = {0xb5,0x62,0x06,0x01,0x08,0x00,0xf5,0x4d,0x00,0x01,0x00,0x00,0x00,0x00,0x52,0x6b};  /*GPS*/
	uint8_t cfg_pkt_msg_rtcm3_2_1087[] = {0xb5,0x62,0x06,0x01,0x08,0x00,0xf5,0x57,0x00,0x01,0x00,0x00,0x00,0x00,0x5c,0xb1};  /*if GLONASS is used*/
	uint8_t cfg_pkt_msg_rtcm3_2_1127[] = {0xb5,0x62,0x06,0x01,0x08,0x00,0xf5,0x7f,0x00,0x01,0x00,0x00,0x00,0x00,0x84,0xc9};  /*if BeiDou is used*/
	uint8_t cfg_pkt_msg_tmode3[] = {0xb5,0x62,0x06,0x71,0x28,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2c,0x01,0x00,0x00,0x30,0x75,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x72,0xcf};/*observation:300s,required positon accuracy:3m*/
	//uint8_t cfg_pkt_msg_tmode3_test[] = {0xb5,0x62,0x06,0x71,0x28,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2c,0x01,0x00,0x00,0x90,0xd0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x56};/*observation:300s,required positon accuracy:25m*/
	//uint8_t cfg_pkt_msg_prt[] = {0xb5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xd0,0x08,0x00,0x00,0x80,0x25,0x00,0x00,0x01,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0xb9,0x33};/*uart1 9600 ubx in,rtcm out*/
	uint8_t cfg_pkt_msg_nav_timegps[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x20,0x00,0x01,0x00,0x00,0x00,0x00,0x31,0x90};
	
	//cfg_pkt_msg_rtcm3_2_1005[9] = 1;
	ubx_cfg_send(device_num,cfg_pkt_msg_rtcm3_2_1005,sizeof(cfg_pkt_msg_rtcm3_2_1005));
	UBX_CFG_DELAY;

	//cfg_pkt_msg_rtcm3_2_1077[9] = 1;
	ubx_cfg_send(device_num,cfg_pkt_msg_rtcm3_2_1077,sizeof(cfg_pkt_msg_rtcm3_2_1077));
	UBX_CFG_DELAY;

	ubx_cfg_send(device_num,cfg_pkt_msg_nav_timegps,sizeof(cfg_pkt_msg_nav_timegps));
	UBX_CFG_DELAY;

	/*gps combine mode*/
	switch(gnss_type)
	{
		case UBX_CFG_GNSS_GPS_ONLY:
			break;
		case UBX_CFG_GNSS_GPS_GLONASS:
			/*组合方式有:GPS+GLONASS*/
			//cfg_pkt_msg_rtcm3_2_1087[9] = 1;
			ubx_cfg_send(device_num,cfg_pkt_msg_rtcm3_2_1087,sizeof(cfg_pkt_msg_rtcm3_2_1087));
			break;
		case UBX_CFG_GNSS_GPS_BEIDOU:
			/*GPS+BeiDou*/
			//cfg_pkt_msg_rtcm3_2_1127[9] = 1;
			ubx_cfg_send(device_num,cfg_pkt_msg_rtcm3_2_1127,sizeof(cfg_pkt_msg_rtcm3_2_1127));
			break;
		default:
			return;
			break; 
	}
	UBX_CFG_DELAY;
	
	cfg_pkt_msg_tmode3[30] = (uint8_t)(min_observation_time & 0xff);
	cfg_pkt_msg_tmode3[31] = (uint8_t)((min_observation_time>>8)&0xff);
	cfg_pkt_msg_tmode3[32] = (uint8_t)((min_observation_time>>16)&0xff);
	cfg_pkt_msg_tmode3[33] = (uint8_t)((min_observation_time>>24)&0xff);
	
	if(required_position_accu >= 429.496)
	{
		cfg_pkt_msg_tmode3[34] = 0x30;
		cfg_pkt_msg_tmode3[35] = 0x89;
		cfg_pkt_msg_tmode3[36] = 0x41;
		cfg_pkt_msg_tmode3[37] = 0x00;
	}
	else
	{
		required_position_accu_uint = (uint32_t)(required_position_accu*10000);
		cfg_pkt_msg_tmode3[34] = (uint8_t)(required_position_accu_uint & 0xff); 
		cfg_pkt_msg_tmode3[35] = (uint8_t)((required_position_accu_uint>>8)&0xff);
		cfg_pkt_msg_tmode3[36] = (uint8_t)((required_position_accu_uint>>16)&0xff);
		cfg_pkt_msg_tmode3[37] = (uint8_t)((required_position_accu_uint>>24)&0xff);
	}
	ubx_cfg_send(device_num,cfg_pkt_msg_tmode3,sizeof(cfg_pkt_msg_tmode3));
	UBX_CFG_DELAY;

	//ubx_cfg_send(device_num,cfg_pkt_msg_prt,sizeof(cfg_pkt_msg_prt));
	//base_mode_test(device_num);
	//UBX_CFG_DELAY;
	ubx_cfg_send(device_num,cfg_pkt_msg_prt,sizeof(cfg_pkt_msg_prt));
	UBX_CFG_DELAY;
}

/*****************************************************
func:ubx_cfg_gnss
desc:set ublox gps combine mode
para:device_num is the fd of uart,gnss_type is gps combine type
return:NULL
*****************************************************/
void ubx_cfg_gnss(int device_num,ubx_cfg_gnss_type_t gnss_type)
{
	int ret;
	uint8_t recv_buf[20];
	uint8_t cfg_pkt_gps[] = {0xB5,0x62,0x06,0x3E,0x24,0x00,0x00,0x00,0x1E,0x04,0x00,0x08,0x10,0x00,0x01,0x00,0x01,0x01,0x03,0x08,0x10,0x00,0x00,0x00,0x01,0x01,0x05,0x00,0x03,0x00,0x00,0x00,0x01,0x01,0x06,0x08,0x0E,0x00,0x00,0x00,0x01,0x01,0xEA,0x30};
	uint8_t cfg_pkt_gps_glonass[] = {0xB5,0x62,0x06,0x3E,0x24,0x00,0x00,0x00,0x1E,0x04,0x00,0x08,0x10,0x00,0x01,0x00,0x01,0x01,0x03,0x08,0x10,0x00,0x00,0x00,0x01,0x01,0x05,0x00,0x03,0x00,0x00,0x00,0x01,0x01,0x06,0x08,0x0E,0x00,0x01,0x00,0x01,0x01,0xEB,0x34};
	uint8_t cfg_pkt_gps_beidou[] = {0xB5,0x62,0x06,0x3E,0x24,0x00,0x00,0x00,0x1E,0x04,0x00,0x08,0x10,0x00,0x01,0x00,0x01,0x01,0x03,0x08,0x10,0x00,0x01,0x00,0x01,0x01,0x05,0x00,0x03,0x00,0x00,0x00,0x01,0x01,0x06,0x08,0x0E,0x00,0x00,0x00,0x01,0x01,0xEB,0x44};
	if(gnss_type >= UBX_CFG_GNSS_END)
	{
		printf("%s set gnss_type error\n",__FUNCTION__);
		return;
	}
	switch(gnss_type)
	{
		case UBX_CFG_GNSS_GPS_ONLY:
			ubx_cfg_send(device_num,cfg_pkt_gps,sizeof(cfg_pkt_gps));
			break;
		case UBX_CFG_GNSS_GPS_GLONASS:
			ubx_cfg_send(device_num,cfg_pkt_gps_glonass,sizeof(cfg_pkt_gps_glonass));
			break;
		case UBX_CFG_GNSS_GPS_BEIDOU:
			ubx_cfg_send(device_num,cfg_pkt_gps_beidou,sizeof(cfg_pkt_gps_beidou));
			break;
		default:
			return;
			break;
	}
	UBX_CFG_DELAY;
	ret = ubx_cfg_recv(device_num,recv_buf,sizeof(recv_buf));
	if(ret >0 )
	{
		ubx_cfg_ack_nak(recv_buf,ret);
	}
}

/*****************************************************
func:ubx_cfg_reset
desc:set ublox the type of reset
para:device_num is the fd of uart,reset_mode is reset mode
return:NULL
*****************************************************/
void ubx_cfg_reset(int device_num,ubx_cfg_reset_mode_t reset_mode)
{
	uint8_t cfg_pkt_reset_hotstart[] = {0xB5,0x62,0x06,0x04,0x04,0x00,0x00,0x00,0x02,0x00,0x10,0x68};
	uint8_t cfg_pkt_reset_warmstart[] = {0xB5,0x62,0x06,0x04,0x04,0x00,0x01,0x00,0x02,0x00,0x11,0x6C};
	uint8_t cfg_pkt_reset_coldstart[] = {0xB5,0x62,0x06,0x04,0x04,0x00,0xFF,0xFF,0x02,0x00,0x0E,0x61};

	if(reset_mode >= UBX_CFG_RESET_END)
	{
		printf("%s set reset_mode error\n",__FUNCTION__);
	}
	switch(reset_mode)
	{
		case UBX_CFG_RESET_HOTSTART:
			ubx_cfg_send(device_num,cfg_pkt_reset_hotstart,sizeof(cfg_pkt_reset_hotstart));
			break;
		case UBX_CFG_RESET_WARMSTART:
			ubx_cfg_send(device_num,cfg_pkt_reset_warmstart,sizeof(cfg_pkt_reset_warmstart));
			break;
		case UBX_CFG_RESET_COLDSTART:
			ubx_cfg_send(device_num,cfg_pkt_reset_coldstart,sizeof(cfg_pkt_reset_coldstart));
			break;
		default:
			break;
	}
}

/*****************************************************
func:gps_chip_config
desc:configure gps
para:gps_config is the config of gps
return:NULL
*****************************************************/
void gps_chip_config(gps_config_t *gps_config)
{
    /* get gps nmea. config needed nmea */
    ubx_cfg_needed_nmea(gps_config->port);
	if(gps_config->mode == GPS_MODE_RSU) /*rsu mode,ublox base mode*/
	{
		ubx_cfg_nmea_freq(gps_config->port,1); /*rsu nmea message default freq 1Hz*/
		ubx_cfg_base_mode(gps_config->port, gps_config->base_min_observation_time, gps_config->base_required_position_accu, UBX_CFG_GNSS_GPS_GLONASS);
		printf("%s:BASE-min_observation_time:[%d(s)],required_position_accu:[%f(m)]\n",__FUNCTION__,gps_config->base_min_observation_time,gps_config->base_required_position_accu);
		
	}
	else /*normal mode,ublox rover mode*/
	{
		if((gps_config->gps_nmea_freq != 1)&&(gps_config->gps_nmea_freq != 2)&&(gps_config->gps_nmea_freq != 5)&&(gps_config->gps_nmea_freq != 10))
		{
			ubx_cfg_nmea_freq(gps_config->port,5); /*rsu nmea message default freq 5Hz*/
		}
		else
		{
			ubx_cfg_nmea_freq(gps_config->port, gps_config->gps_nmea_freq);
		}
		ubx_cfg_rover_mode(gps_config->port);
	}
	
    usleep(10*1000);	
    //ubx_cfg_sbas(fd,0);
    //osal_sleep(10);	
#if 0
    /* conifg ublox gps rate 5Hz */
    ubx_cfg_nmea_freq(fd, freq);
    osal_sleep(10);
    /* config gps baud to 115200 */
    ubx_cfg_uart_port(fd);
#endif
}

