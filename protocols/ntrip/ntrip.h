#ifndef _NTRIP_H_
#define _NTRIP_H_

#include <sys/socket.h>
#include <stdint.h>

#include <os_pthread.h>
#include <osal_queue.h>
#include <os_core.h>

#define NTRIP_SERVER	"rtk.ntrip.qxwz.com" /*ntrip server URL*/
#define NTRIP_PORT	"8002" /*The port number*/

#define NTRIP_USER	"carsmart001" /*username*/
#define NTRIP_PASSWD	"ce3a7e1" /*password*/

#define closesocket(sock)       close(sock) /*Close the socket*/

/* The string, which is send as agent in HTTP request */
#define AGENTSTRING "NTRIP NtripClientPOSIX"

#define MAXDATASIZE 1000 /* max number of bytes we can get at once */
#define GGA_BUF_SIZE	200 /*gga buffer size*/

#define NTRIP_THREAD_STACK_SIZE	(1024*8192) /*ntrip thread stack size,8K*/
#define NTRIP_THREAD_PRIORITY		(TK_PRIO_DEFAULT) /*ntrip thread priority*/

/*Network request mode*/
enum MODE { HTTP = 1, RTSP = 2, NTRIP1 = 3, AUTO = 4, UDP = 5, END };

/*NTRIP request parameters*/
typedef struct _ntrip_args
{
  const char *server; /*ntrip server URL*/
  const char *port;  /*The port number*/
  const char *user; /*username*/
  const char *proxyhost;  /*Proxy host*/
  const char *proxyport; /*Proxy host port*/
  const char *password;  /*password*/
  const char *nmea; /*nmea frame data(gga)*/
  const char *data; /*rtcm data version*/
  int         bitrate; /*Bit rate*/
  int         mode; /*Network request mode*/

  int         udpport; /*udp port*/
  int         initudp; /*Initialized udp*/
}ntrip_args_t;

typedef void (*ntrip_callback_fun)(uint8_t *, uint32_t); /*ntrip callback type,uint8_t * is data buf,uint32_t is the length of this buffer*/

/*ntrip config type*/
typedef struct _ntrip_config_t
{
	uint8_t mode;//gps mode
	int gps_port;//gps fd
	ntrip_callback_fun ntrip_rsu_callback;//Rtcm data callback function in RSU mode
}ntrip_config_t;

/*ntrip control type*/
typedef struct _ntrip_contrl_t
{
	uint8_t ntrip_start_flag;  /*NTRIP service start flag*/
	uint8_t gga_valid_flag;  /*gga is a valid sign*/
	uint8_t task_ntrip_valid;  /*NTRIP task is a valid sign*/
	osal_task_t task_ntrip; /*NTRIP handles data tasks*/
}ntrip_contrl_t;

/*****************************************************
func:ntrip_init
desc:init ntrip services
para:ntrip_config is the config of ntrip.
	When mode is RSU, you need to initialize the ntrip_rsu_callback parameter;When mode is normal, you need to initialize the gps_port parameter
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_init(ntrip_config_t ntrip_config);

/*****************************************************
func:ntrip_deinit
desc:deinit ntrip services
para:void
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_deinit();

/*****************************************************
func:ntrip_gga_update
desc:Update gga data
para:buf is the data buffer,len is the length of this buffer 
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_gga_update(uint8_t *buf, uint32_t len);

/*****************************************************
func:ntrip_start
desc:Start the NTRIP service
para:void
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_start();

/*****************************************************
func:ntrip_stop
desc:stop the NTRIP service
para:void
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_stop();

#endif