#include "stdio.h"
#include "stdlib.h"
#include <stdint.h>

#include "ehmh_element.h"
#include "ehmh_protocol.h"
#include "cv_data_element.h"


/* Save all the compiler settings. */
#pragma pack(push)
#pragma pack(1)

/* Get main driver pointer. */
#define GET_DRVMAIN_PTR    (DrvMainPtr)




/* Message buffer size for wnet module. */
#define WNET_MSGBUF_SIZE   (3000)



/* Stack size of all the tasks in system. */
#define DEF_THREAD_STACK_SIZE       (1024*8192)

#define RT_SYS_THREAD_STACK_SIZE    DEF_THREAD_STACK_SIZE
#define RT_GPS_THREAD_STACK_SIZE    DEF_THREAD_STACK_SIZE
#define RT_MEMS_THREAD_STACK_SIZE   DEF_THREAD_STACK_SIZE

#define RT_VSA_THREAD_STACK_SIZE    DEF_THREAD_STACK_SIZE
#define RT_HI_THREAD_STACK_SIZE     DEF_THREAD_STACK_SIZE
#define EHM_MAIN_THREAD_STACK_SIZE  DEF_THREAD_STACK_SIZE
#define EHM_TX_THREAD_STACK_SIZE    DEF_THREAD_STACK_SIZE

#define EHM_RX_THREAD_STACK_SIZE    DEF_THREAD_STACK_SIZE
#define WNET_RX_THREAD_STACK_SIZE   DEF_THREAD_STACK_SIZE


#define EHM_RX_THREAD_PRIORITY        70
#define WNET_RX_THREAD_PRIORITY     80


/*************************************************
  枚举类型
*************************************************/
typedef enum _V2X_MSG_TYPE
{
    V2X_RESERVED = 0,
    V2X_NB_NODE_MSG = 1,
    V2X_BASIC_VEHICLE_MSG = 2,
    V2X_FULL_VEHICLE_MSG = 3,
    V2X_VEHICLE_STATIC_MSG = 4,
    V2X_LC_VEHICLE_ALERT_MSG = 5,
    V2X_NB_VEHICLE_ALERT_MSG = 6,
    V2X_ROADSIZE_ALERT_MSG = 7,
    V2X_SYSTEM_CURTIME_MSG = 8,

    V2X_NB_NODE_SUMMRAY_INFO = 1,
    V2X_NB_NODE_DETAIL_INFO = 2,
    V2X_BASIC_VEHICLE_STATUS = 4,
    V2X_FULL_VEHICLE_STATUS = 8,
    V2X_VEHICLE_STATIC_INFO = 16,
    V2X_LC_VEHICLE_ALERT_SET = 32,
    V2X_NB_VEHICLE_ALERT = 64,
    V2X_ROADSIZE_ALERT = 128,

}V2X_MSG_TYPE_E;

/* Vhost active device enumerate. */
typedef enum _vhost_device_em
{
    VHOST_DEVICE_NET  = 1,
    VHOST_DEVICE_UART = 2
    
}vhost_device_em, *vhost_device_em_ptr;

#define VHOST_DEVICE_EM_LEN    (sizeof(vhost_device_em))

/* Net data infor structure. */
typedef struct _net_infor_st
{   
    uint8_t dummy;
    
}net_infor_st, * net_infor_st_ptr;

#define NET_INFOR_ST_LEN    (sizeof(net_infor_st))

/* Uart data infor structure. */
typedef struct _uart_infor_st
{   
    uint8_t dummy;
    
}uart_infor_st, * uart_infor_st_ptr;

#define UART_INFOR_ST_LEN    (sizeof(uart_infor_st))


/* Vhost data infor structure. */
typedef struct _vhost_infor_st
{   
    /* Active device for data receive and data send. */
    vhost_device_em active_dev;

    /* Device infor union. */
    union _vhost_dev_un
    {
        /* Net device infor. */
        net_infor_st   net;

        /* Uart device infor. */
        uart_infor_st uart;
        
    }vhost_dev_un;  
    
}vhost_infor_st, * vhost_infor_st_ptr;

#define VHOST_INFOR_ST_LEN    (sizeof(vhost_infor_st))

/* Buffer structure for ehm module. */
typedef struct _ehm_buff_st
{
    /* Data information. */
    vhost_infor_st  infor;

    /* Data buffer that store messsage. */
    uint8_t  buffer[4096];

    /* Data length in buffer. */
    uint16_t     data_len;  

}ehm_buff_st, * ehm_buff_st_ptr;

#define EHM_BUFF_ST_LEN    (sizeof(ehm_buff_st))

typedef struct _local_vehicle_gps_info_st
{
    /*3D位置*/
    /* 纬度: unit 0.1 micro degree, (-900000000 - +900000001), 900000001 means invalid. */
    double  latitude;                      /* 纬度 */
    /* 经度: unit 0.1 micro degree, (-1799999999 - 1800000001), 1800000001 means invalid. */
    double longitude;                      /* 经度 */
    /* 海拔: unit 0.1 meter, (-4096 - +61439), -4096 means invalid. */
    float elevation;                      /* 海拔 */
    
    /* 位置精确度 */
    DF_PositionalAccuracy_st    posaccu;
    
    /* 速度: unit 0.02 m/s, (0 - 8191), 8191 means invalid. */
    float              velocity;
    
    /* 行驶方向: unit 0.0125 degree (相对正北顺时针夹角), (0 - 28800), 28800 means invalid. */
    float              angle;
}local_vehicle_gps_info_st,*local_vehicle_gps_info_st_ptr;

#define LOCAL_VEHICLE_GPS_INFO_ST_LEN    (sizeof(local_vehicle_gps_info_st))


#define TRAFFIC_LIGHT_ON            0b01
#define TRAFFIC_LIGHT_OFF           0b00
#define TRAFFIC_LIGHT_BLINK         0b10
#define TRAFFIC_LIGHT_RESERVED      0b11

#define TRAFFIC_INTERSECTION_NUM    4

#define RECOMMAND_SPEED_MAX        (70.0f)   // Uint: Km/h

#define IPADDR_LENGTH  4


/* Earth radius. Unit: m. */
#define EARTH_RADIUS          (6371004.0f)


/* Degree to radian. value = (degree * 3.1415926535898) / 180.0. */
#define DEGREE_TO_RAD(degree) ((degree) / 57.2957795f)

/* Radian to degrr. value = (radian * 180.0) / 3.1415926535898. */
#define RAD_TO_DEGREE(radian) ((radian) * 57.2957795f)


typedef enum _direction_vehicle_em
{
    DIRECTION_NORTH = 0,
    DIRECTION_NORTH_EAST,
    DIRECTION_EAST,
    DIRECTION_SOUTH_EAST,
    DIRECTION_SOUTH,
    DIRECTION_SOUTH_WEST,
    DIRECTION_WEST,
    DIRECTION_NORTH_WEST
}direction_vehicle_em,*direction_vehicle_em_ptr;


typedef struct _msg_intersection_st
{
    uint16_t                                        intersection_id;
    msg_decode_trafficlamp_speed_guide_st_ptr trafficLampSpeedGuide;
}msg_intersection_st,*msg_intersection_st_ptr;

typedef struct _info_rsu_st
{
    uint8_t rsu_id[DE_TemporaryID_BUFSIZE];
    double  latitude; /* Geographic latitude. Unit: degree. */
    double  longitude; /* Geographic longitude. Unit: degree. */
}info_rsu_st,*info_rsu_st_ptr;


/* restore all compiler settings in stacks. */
#pragma pack(pop)


