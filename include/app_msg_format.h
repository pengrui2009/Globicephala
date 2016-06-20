/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : app_msg_format.h
 @brief  : this file include the application variables and functions prototypes for 
           the message format module.
 @author : wangxianwen
 @history:
           2016-1-21    wangxianwen    Created file
           2016-6-16	pengrui		   add some structure
           ...
******************************************************************************/

/* Define to prevent recursive inclusion. */
#ifndef _APP_MSG_FORMAT_H_
#define _APP_MSG_FORMAT_H_

/*************************************************
 宏定义
*************************************************/
#define __COMPILE_INLINE__
#define __COMPILE_PACK__ __attribute__((packed))

/*
 * CAUTION:
 *    The sentence "declaration __attribute__ ((aligned(x)))" can not be working in this IDE. 
 *    We can use this parameter to make the storage larger then the natural size; but we can 
 *    not make the storage smaller than the natural size.
 * */

/* Save all the compiler settings. */

/* store data to reduce data size and off the optimization. */
#pragma pack(1)
/*************************************************
枚举类型
*************************************************/
/* 消息类型枚举Message type enum*/
typedef enum _msg_type_em
{
    MSGTYPE_DEBUG     		= 0x1,       /* 心跳消息 */
    MSGTYPE_SYS_MANAGE 		= 0x2,       /* 配置查询消息 */
    MSGTYPE_V2X_APPLY  		= 0x3,       /* 事件上报消息 */
    MSGTYPE_RAWDATA_DSRC 	= 0x4,       /* DSRC原始消息 */
    MSGTYPE_RAWDATA_NMEA 	= 0x5        /* NMEA原始消息 */
    
}MSG_TYPE_EM, *MSG_TYPE_EM_PTR;

#define MSG_TYPE_EM_LEN    (sizeof(MSG_TYPE_EM))


/* 车辆消息类型枚举enum*/
typedef enum _msg_subtype_em
{
    MSGSUBTYPE_VEHICLE_ALERT  = 0x1001,   /* 车辆告警消息 */
    MSGSUBTYPE_VEHICLE_STATUS = 0x1002,   /* 车辆状态消息 */
    MSGSUBTYPE_SYSTEM_ERROR   = 0x1003    /* 系统错误消息 */

}MSG_SUBTYPE_EM, *MSG_SUBTYPE_EM_PTR;

#define MSG_SUBTYPE_EM_LEN    (sizeof(MSG_SUBTYPE_EM))


/* 节点信息枚举 enum*/
typedef enum _node_infor_type_em
{
    NODE_INFOR_TYPE_SUMMARY = 0x01,       /* 节点概要信息 */
    NODE_INFOR_TYPE_DETAIL  = 0x02        /* 节点详细信息 */
    
}NODE_INFOR_TYPE_EM, *NODE_INFOR_TYPE_EM_PTR;

#define NODE_INFOR_TYPE_EM_LEN    (sizeof(NODE_INFOR_TYPE_EM))

/* 变速箱状态enum*/
typedef enum _transmission_st_type_em
{
	TRANSMISSION_TYPE_NEUTRAL,
	TRANSMISSION_TYPE_PARK,
	TRANSMISSION_TYPE_FORWARDGEARS,
	TRANSMISSION_TYPE_REVERSEGEARS,
	TRANSMISSION_TYPE_RESERVED1,
	TRANSMISSION_TYPE_RESERVED2,
	TRANSMISSION_TYPE_RESERVED3,
	TRANSMISSION_TYPE_UNAVAILABLE
}TRANSMISSION_ST_TYPE_EM;
/* 牵引力状态枚举enum*/
typedef enum _traction_control_info_type_em
{
	TRACTION_CONTROL_UNAVAILABLE,
	TRACTION_CONTROL_OFF,
	TRACTION_CONTROL_ON,
	TRACTION_CONTROL_ENGAGED
}TRACTION_CONTROL_INFO_TYPE_EM;
/* ABS防抱死状态枚举enum*/
typedef enum _anti_brake_st_type_em
{
	ANTI_BRAKE_UNAVAILABLE,
	ANTI_RRAKE_OFF,
	ANTI_BRAKE_ON,
	ANTI_BRAKE_ENGAGED
}ANTI_BRAKE_ST_TYPE_EM;
/* SC稳定控制状态枚举 enum*/
typedef enum _stabile_control_st_type_em
{
	STABILE_CONTROL_UNAVAILABLE,
	STABILE_CONTROL_OFF,
	STABILE_CONTROL_ON,
	STABILE_CONTROL_ENGAGED
}STABILE_CONTROL_ST_TYPE_EM;
/* 制动应用状态枚举 enum*/
typedef enum _brake_boost_info_type_em
{
	BRAKE_BOOST_UNAVAILABLE,
	BRAKE_BOOST_OFF,
	BRAKE_BOOST_ON
}BRAKE_BOOST_INFO_TYPE_EM;
/*  辅助制动系统状态枚举 enum*/
typedef enum _aux_brake_st_type_em
{
	/* 不可用*/
	AUX_BRAKE_UNAVAILABLE,
	/*关状态*/
	AUX_BRAKE_OFF,
	/*开状态*/
	AUX_BRAKE_ON,
	/*预留*/
	AUX_BRAKE_RESERVED
}AUX_BRAKE_ST_TYPE_EM;
/* vehicle type enum*/
typedef enum _vehicle_type_em
{
	/* Not Equipped, Not known or unavailable*/
	VEHICLE_TYPE_NONE,
	/* Does not fit any other category*/
	VEHICLE_TYPE_UNKONWN,
	/* Special use*/
	VEHICLE_TYPE_SPECIAL,
	/* Motorcycle*/
	VEHICLE_TYPE_MOTO,
	/* Passenger car*/
	VEHICLE_TYPE_CAR,
	/* Four tire single units*/
	VEHICLE_TYPE_CAR_OTHER,
	/* Buses*/
	VEHICLE_TYPE_BUS,
	/* Two axle, six tire single units*/
	VEHICLE_TYPE_AXLECNT2,
	/* Three axle, single units*/
	VEHICLE_TYPE_AXLECNT3,
	/* Four or more axle, single unit*/
	VEHICLE_TYPE_AXLECNT4,
	/* Four or less axle, single trailer*/
	VEHICLE_TYPE_AXLECNT4_TRAILER,
	/* Five or less axle, single trailer*/
	VEHICLE_TYPE_AXLECNT5_TRAILER,
	/* Six or more axle, single trailer*/
	VEHICLE_TYPE_AXLECNT6_TRAILER,
	/* Five or less axle, multi-trailer*/
	VEHICLE_TYPE_AXLECNT5_MULTITRALIER,
	/* Six axle, multi-trailer*/
	VEHICLE_TYPE_AXLECNT6_MULTITRAILER,
	/* Seven or more axle, multi-trailer*/
	VEHICLE_TYPE_AXLECNT7_MULTITRAILER
}VEHICLE_TYPE_EM;
/*************************************************
  结构体
*************************************************/
/* 3D位置Position 3D structure. */
typedef struct  _position_3d_st
{
	/* 纬度: unit 0.1 micro degree, (-900000000 - +900000001), 900000001 means invalid. */
    int32_t  latitude;                      /* 纬度 */
    /* 经度: unit 0.1 micro degree, (1799999999 - 1800000001), 1800000001 means invalid. */
    int32_t longitude;                      /* 经度 */
    /* 海拔: unit 0.1 meter, (-4096 - +61439), -4096 means invalid. */
    int16_t elevation;                      /* 海拔 */
    
}position_3d_st, *position3d_st_ptr;

#define POSITION3D_ST_LEN    (sizeof(position3d_st))

/* 位置信息准确度 Position accurary structure. */
typedef struct _position_accu_st
{
	/* 长半轴准确度: unit 5 cm, (0 - 255) [254 means >=254], 255 means invalid. */
	uint8_t			semimajoraxisAccu;//长半轴准确度
	/* 短半轴准确度: unit 5 cm, (0 - 255) [254 means >=254], 255 means invalid. */
	uint8_t			semiminoraxisAccu;//短半轴准确度
	/* 长半轴方向角: unit 0.0054932479 degree, (0-65534), 65535 means invalid. */
	uint16_t		semimajorAxisOrien;//长半轴方向
}position_accu_st,*position_accu_st_ptr;

#define POSITION_ACCURARY_LEN	sizeof(ehm_position_accu)

/* 加速度accleration 4 way structure*/
typedef struct _accleration_set4way_st
{
	/* 纵向加速度(x): unit 0.01 m/s^2, (-2000 - +2000) [-2000 means <=-2000 +2000 means >=2000], 2001 means invalid. */
	int16_t			lonacce;//x加速度
	/* 横向加速度(y): unit 0.01 m/s^2, (-2000 - +2001) [-2000 means <=-2000 +2000 means >=2000], 2001 means invalid. */
	int16_t			latacce;//y加速度
	/* 垂直加速度(z): unit 0.02G(0.1962 m/s^2), (-127 - +127) [+127 means >= 2.54G -126 means <= -2.52G ], -127 means invalid. */
	int8_t			veracce;//垂直加速度
	/* 偏航率: unit 0.01 degree/s, (-32767 - +32767). */
	int16_t			yawrate;//偏航率
}accleration_set4way_st,*accleration_set4way_st_ptr;

#define ACCLERATION_SET4WAY_LEN	(sizeof(accleration_set4way_st))
/* 车辆轮子制动状态brake applied status structure*/
typedef union _wheel_brake_st
{
	uint8_t wheel_brake_word;

	struct _wheel_brake_bit{
		#ifndef __LITTLE_ENDIAN
		/*预留*/
			uint8_t	reserved:4;
		/*右后轮 	0:disactive		1:active ,如果车辆后轮只有一个轮子,则该位:0*/
			uint8_t rightrear:1;
		/*右前轮		0:active		1:active ,如果车辆后轮只有一个轮子,则该位:0*/
			uint8_t rightfront:1;
		/*左后轮		0:active		1:active ,如果车辆后轮只有一个轮子,则左侧轮子代表有效*/
			uint8_t leftrear:1;
		/*左前轮		0:active		1:active ,如果车辆前轮只有一个轮子,则左侧轮子代表有效*/
			uint8_t leftfront:1;
		#else
		/*左前轮		0:active		1:active ,如果车辆前轮只有一个轮子,则左侧轮子代表有效*/
			uint8_t leftfront:1;
		/*左后轮		0:active		1:active ,如果车辆后轮只有一个轮子,则左侧轮子代表有效*/
			uint8_t leftrear:1;
		/*右前轮		0:active		1:active ,如果车辆后轮只有一个轮子,则该位:0*/
			uint8_t rightfront:1;
		/*右后轮 	0:disactive		1:active ,如果车辆后轮只有一个轮子,则该位:0*/
			uint8_t rightrear:1;
			uint8_t	reserved:4;
		#endif
	}__COMPILE_PACK__ wheel_brake_bit;
}wheel_brake_st;

/* 制动系统状态 brake system status structure*/
typedef struct _brake_system_status_st
{
	/*制动应用状态 */
	wheel_brake_st		braas;//制动应用状态
	/*牵引力控制状态  0:无效 	1:开 	 2:关	3：使用中*/
	uint8_t		tracs;//牵引力控制状态
	/*ABS防抱死状态   0:无效 	1:开 	 2:关	3：使用中*/
	uint8_t		antbs;//ABS防抱死状态
	/*SC稳定控制状态   0:无效 	1:开 	 2:关	3：使用中*/
	uint8_t		stacs;//SC稳定控制状态
	/*制动增压应用	0:无效 	1:开 	 2:关	*/
	uint8_t		braba;//制动增压应用
	/*辅助制动状态   0:无效 	1:开 	 2:关	3：预留*/
	uint8_t		auxbs;//辅助制动状态
}brake_system_status_st,*brake_system_status_st_ptr;

#define BRAKE_SYSTEM_STATUS_LEN	(sizeof(brake_system_status_st))
/* 外部灯状态external lights structure*/
typedef union _exterior_lights_un
{
	uint16_t	exterior_ligths;
	struct _exterior_lights_st
	{
		/* reserved*/
		uint16_t		reserved:7;
		/* 停车指示灯开启*/
		uint16_t		parkinglight:1;
		/* 雾灯开启*/
		uint16_t		fogligthon:1;
		/* 日间行车灯开启*/
		uint16_t		daytimerunninglight:1;
		/* 自动亮度调节开启*/
		uint16_t		automaticlight:1;
		/* 危险指示灯开启*/
		uint16_t		hazardsignallight:1;
		/* 右转信号灯开启*/
		uint16_t		rightturnsignallight:1;
		/* 左转信号灯开启*/
		uint16_t		leftturnsignallight:1;
		/* 远光灯开启*/
		uint16_t		highbeamheadlight:1;
		/* 近光灯开启*/
		uint16_t		lowbeamheadlight:1;
	}exterior_lights_st;
}exterior_lights_un;

/* 车辆大小结构体 vehicle size structure*/
typedef struct _vehicle_size_info_st
{
	/* 车辆宽度: unit 1 cm, (0 - 1023). */
	uint16_t		vehiclewidth;//车辆宽度
	/* 车辆长度: unit 1 cm, (0 - 4095). */
	uint16_t		vehiclelength;//车辆长度
}vehicle_size_info_st,*vehicle_size_info_st_ptr;

#define VEHICLE_SIZE_LEN	sizeof(vehicle_size_info);
/* 告警标识 Alert flag structure */
typedef union _alert_flag_st
{
    /* Alert word for whole group. */
    uint32_t alert_word;                  /* 告警位组合 */
    
    /* Alert bits. */
    struct _alert_bit
    {
	#ifndef __LITTLE_ENDIAN
    	uint32_t reserved          :25;   /* 保留位 */
		uint32_t rsd_tunnel        :1;    /* 隧道告警 */
		uint32_t vec_danger_goods  :1;    /* 危险货物运输车告警 */
		uint32_t vec_ambulance     :1;    /* 救护车告警 */
		uint32_t vec_brake_hard    :1;    /* 紧急刹车告警 */
		uint32_t vec_breakdown     :1;    /* 车辆故障告警 */
		uint32_t vec_neardis_rear  :1;    /* 后车近距离告警 */
		uint32_t vec_neardis_front :1;    /* 前车近距离告警 */
	#else
    	uint32_t vec_neardis_front :1;    /* 前车近距离告警 */
		uint32_t vec_neardis_rear  :1;    /* 后车近距离告警 */
		uint32_t vec_breakdown     :1;    /* 车辆故障告警 */
		uint32_t vec_brake_hard    :1;    /* 紧急刹车告警 */
		uint32_t vec_ambulance     :1;    /* 救护车告警 */
		uint32_t vec_danger_goods  :1;    /* 危险货物运输车告警 */
		uint32_t rsd_tunnel        :1;    /* 隧道告警 */
		uint32_t reserved          :25;   /* 保留位 */
	#endif
    }__COMPILE_PACK__ alert_bit;
    
}alert_flag_st, *alert_flag_st_ptr;

#define ALERT_FLAG_ST_LEN    (sizeof(alert_flag_st))

/* Alert bit status. */
#define ALERT_FLAG_BIT_YES         0x01
#define ALERT_FLAG_BIT_NO          0x00

/* 节点概要信息Node summary information structure. */
typedef struct _node_summary_info_st{
	/* 节点ID:(0,0,0,0)-invalid id */
	uint8_t       node_id[4];
	/* 纵向距离(以本车位置为基准): unit 0.01 m, (-32768 - +32767). */
	int16_t longitudinal_dis;
	/* 横向距离(以本车位置为基准): unit 0.01 m, (-32768 - +32767). */
	int16_t  latitudinal_dis;
	/* 告警标记 */
	alert_flag_st alert_flag;
}node_summary_info_st,*node_summary_info_st_ptr;

/* 节点详细信息Node detailed information structure. */
typedef struct  _node_detail_infor_st
{
	/* 节点ID*/
    uint8_t       node_id[4];
    /* 行车方向(相对正北顺时针夹角): unit 0.0125 degree, 	(0 - +359.9875), 360 means invalid. */
    float          direction;
    /* 速度:km/h. */
    float              speed;
    /* 3D位置 */
    position_3d_st      pos3d;
    /* 纵向距离: unit 0.01 m, (-32768 - +32767). */
    int16_t  longitudinal_dis;
    /* 横向距离: unit 0.01 m, (-32768 - +32767). */
    int16_t  latitudinal_dis;
    /*告警标记 */
    alert_flag_st alert_flag;    
}node_detail_infor_st, *node_detail_infor_st_ptr;

#define NODE_DETAIL_INFOR_ST_LEN    (sizeof(node_detail_infor_st))

/* 邻节点概要信息neigbhour node summary information structure. */
typedef struct  _nb_node_summary_infor_st
{
	/* 节点ID:(0,0,0,0)-invalid id */
    uint8_t       node_id[4];
    /* 纵向距离(以本车位置为基准): unit 0.01 m, (-32768 - +32767). */
    int16_t longitudinal_dis;
    /* 横向距离(以本车位置为基准): unit 0.01 m, (-32768 - +32767). */
    int16_t  latitudinal_dis;
    /* 相对行驶方向(以本车行驶方向为基准)：uint 0.0125 degree (0 - +28799) +28800 means invalid*/
    uint16_t	angle;
    /* 相对车速(以本车车速为基准)：uint 0.02m/s (0 - +8190) 8191 means invalid*/
    uint16_t	velocity;
    /* 信号强度(邻居-本车通信):uint 1dBm (-127 - +126) [-127 means <=-127 +126 means >=+126] +127 means unavailable value*/
    int8_t		signalstrength;
    /* 丢包率(邻居-本车通信):	uint 1%	[0 means 0% 100 means 100%] 101 means invalid */
    int8_t		losstolerance;
    
}nb_node_summary_infor_st, *nb_node_summary_infor_st_ptr;
#define NB_NODE_SUMMARY_INFOR_ST_LEN	sizeof(nb_node_summary_infor_st)
/*邻节点详细信息*/
typedef struct _nb_node_detial_infor_st
{
	/* 节点ID:(0,0,0,0)-invalid id */
	uint8_t       		node_id[4];
	/* 3D位置 */
	position_3d_st     	pos3d;
	/* 位置精确度*/
	position_accu_st 	posaccu;
	/* 变速箱状态: 0：悬空状态 	1：驻车状态	2:前进状态	3:后退状态	4:保留	5:保留	6:保留	7:unavailable */
	uint8_t		 			tran_state;//变速箱状态
	/* 车速(源自GPS模块或者车辆总线)：uint 0.02m/s (0 - +8190) 8191 means invalid*/
	uint16_t	velocity;
	/* 绝对行驶方向(以正北方为基准)：uint 0.0125 degree (0 - +28799) +28800 means invalid*/
	uint16_t	angle;
	/* 方向盘转角:uint 1.5 degrees (-126 - +126) [-126 means <= -189 deg +126 means >=+189 deg] +127 means unavaliable*/
	int8_t 		steer_wheel_angle;
	/* 加速度*/
	accleration_set4way_st acc;
	/* 制动系统状态(如果前轮为单轮,以左前轮为参考;如果 后轮为单轮,以左后轮为参考)*/
	brake_system_status_st brake;
	/* 外部灯光状态*/
	exterior_lights_un	exterlight;
}nb_node_detail_infor_st,*nb_node_detial_infor_st_ptr;

#define NB_NODE_DETAIL_INFOR_ST_LEN    (sizeof(nb_node_detail_infor_st))
/* Message header structure ---------------------------------------------*/
/* 串口帧头信息结构体 uart Message header structure */
typedef  struct  _uart_msg_header_st
{
	/* 串口帧首标识符:	0x55. */
	uint8_t magic_num1;

	/* 串口帧首标识符:	0xAA. */
	uint8_t magic_num2;

	/*帧长度: DATA + CHK */
	uint16_t    length;
}uart_msg_header_st,uart_msg_header_st_ptr;

/* Magic number for wnet main header. */
#define MSG_HEADER_MAGIC_NUM1          0x55
#define MSG_HEADER_MAGIC_NUM2          0xAA
#define	MSG_CHK_LEN					   0x2
#define UART_MSG_HEADER_ST_LEN         (sizeof(uart_msg_header_st))

/* Domain size in wnet header structrue. */
#define SIZEOF_MSG_HEADER_MAGIC_NUM1   (sizeof(((msg_header_st_ptr)0)->magic_num1))
#define SIZEOF_MSG_HEADER_MAGIC_NUM2   (sizeof(((msg_header_st_ptr)0)->magic_num2))
#define SIZEOF_MSG_HEADER_LENGTH       (sizeof(((msg_header_st_ptr)0)->length))

/* Domain offset in msg header structure. */
#define OFFSET_MSG_HEADER_MAGIC_NUM1   ((uint32_t)&(((msg_header_st_ptr)0)->magic_num1))
#define OFFSET_MSG_HEADER_MAGIC_NUM2   ((uint32_t)&(((msg_header_st_ptr)0)->magic_num2))
#define OFFSET_MSG_HEADER_LENGTH       ((uint32_t)&(((msg_header_st_ptr)0)->length))

/* Data Frames ------------------------------------------------------*/
/*帧消息头信息  frame msg header structure*/
typedef  struct  _frame_msg_header_st
{
#ifndef __LITTLE_ENDIAN
	/*消息标识符:	0xE*/
	uint8_t	mark:4;//B1110[0xE]
	/*消息源：	0:v2x	1:host*/
	uint8_t	src:1;//0: from v2x 1: from host
	/*预留字段：	0*/
	uint8_t	reserved1:3;//B000[0x0]
	/*预留字段：	0*/
	uint8_t	reserved2:4;//B000[0x0]
	/*消息类型:	0:预留 1:调试消息 2:系统管理消息 3:v2x应用消息 4:dsrc消息 5:gps消息 6:未定义*/
	uint8_t	type:4;//0:reserved 1:debug 2:sys manager 3:v2x apply 4:dsrc 5:gps 6:undefine
#else
	/*预留字段：	0*/
	uint8_t	reserved1:3;//B000[0x0]
	/*消息源：	0:v2x	1:host*/
	uint8_t	src:1;//0: from v2x 1: from host
	/*消息标识符:	0xE*/
	uint8_t	mark:4;//:B1110[0xE]
	/*消息类型:	0:预留 1:调试消息 2:系统管理消息 3:v2x应用消息 4:dsrc消息 5:gps消息 6:未定义*/
	uint8_t	type:4;//0:reserved 1:debug 2:sys manager 3:v2x apply 4:dsrc 5:gps
	/*预留字段：	0*/
	uint8_t	reserved2:4;//B000[0x0]
#endif
}frame_msg_header_st, *frame_msg_header_st_ptr;

#define MSG_HEADER_MARK						0xE
#define MSG_SRC_V2X							0x0
#define MSG_SRC_HOST						0x1
#define FRAME_MSG_HEADER_ST_LEN              		(sizeof(frame_msg_header_st))

/* Message Sets ----------------------------------------------------------*/

/* 邻居节点信息  msg neighbour node info structure */
typedef struct  _msg_vehicle_nb_status_st
{
    /* Message system time. */
    uint32_t            system_time;
    /*Node Number*/
    uint8_t				nodenumber;
    /* Set the neigbour's node information type. */
    uint8_t         	node_infor_type;
    /* 邻车概要信息/邻车完整信息 N * (Node summary/detail information structure). (0 <= N) */
    

}msg_vehicle_nb_status_st, *msg_vehicle_nb_status_st_ptr;

#define MSG_VEHICLE_NB_STATUS_ST_LEN    (sizeof(msg_vehicle_nb_status_st))

/* 基本车辆状态basic Vehicle status structure */
typedef struct _msg_basic_status_st
{
	/* 节点ID:(0,0,0,0)-invalid id */
	uint8_t       			node_id[4];
	/*3D位置*/
	position_3d_st 			position;//3D位置
	/* 位置精确度 */
	position_accu_st		posaccu;//位置精确度
	/* 速度: unit 0.02 m/s, (0 - 8191), 8191 means invalid. */
	uint16_t				velocity;//速度
	/* 行驶方向: unit 0.0125 degree (相对正北顺时针夹角), (0 - 28800), 28800 means invalid. */
	uint16_t				angle;//行驶方向
}msg_vehicle_basic_status_st,*msg_basic_status_st_ptr;

#define MSG_VEHICLE_BASIC_STATUS_ST_LEN		(sizeof(msg_vehicle_basic_status_st))

/* 完整车辆状态 full Vehicle status structure */
typedef struct _msg_full_status_st
{
	/* 节点ID:(0,0,0,0)-invalid id */
	uint8_t       			node_id[4];
	/* 3D位置  */
	position_3d_st 			position;//3D位置
	/* 位置精确度 */
	position_accu_st		posaccu;//位置精确度
	/* 变速箱状态: 0：悬空状态 	1：驻车状态	2:前进状态	3:后退状态	4:保留	5:保留	6:保留	7:unavailable */
	uint8_t		 			tran_state;//变速箱状态
	/* 速度: unit 0.02 m/s, (0 - 8191), 8191 means invalid. */
	uint16_t				velocity;//速度
	/* 行驶方向: unit 0.0125 degree(相对正北顺时针夹角), 	(0 - 28800), 28800 means invalid. */
	uint16_t				angle;//行驶方向
	/* 方向盘转角: unit 1.5 degree, (-126 - +127), 127 means invalid. */
	int8_t					steerwa;//方向盘转角
	/* 4路加速度集   */
	accleration_set4way_st	acce4way;//4路加速度集
	/* 制动系统状态  */
	brake_system_status_st	braksta;//制动系统状态
	/* 外部灯光状态*/
	exterior_lights_un		exterlight;
}msg_vehicle_full_status_st,msg_full_status_st_ptr;

#define	MSG_VEHICLE_FULL_STATUS_ST_LEN	(sizeof(msg_vehicle_full_status_st))

/* 车辆静态信息 Vehicle static info structure */
typedef struct _vehicle_static_info_st
{
	/* 车辆类型*/
	VEHICLE_TYPE_EM			vehicletype;
	/* 车辆尺寸*/
	vehicle_size_info_st 	vehicle_size;
}msg_vehicle_static_info_st,vehicle_static_info_st_ptr;

#define MSG_VEHICLE_STATIC_INFO_ST_LEN	sizeof(msg_vehicle_static_info_st)
/* 本地车辆告警设置 local Vehicle alert set info structure */
typedef struct _msg_local_vehicle_alert_st
{
	/* 车辆故障告警：00b(Invalid),01b(Off),10b(On),11b(Reserved)*/
	uint16_t	vecbrakedownalert;
	/* 紧急刹车告警:00b(Invalid),01b(Off),10b(On),11b(Reserved)*/
	uint16_t vecbrakehardalert;
}msg_local_vehicle_alert_st,*msg_local_vehicle_alert_st_ptr;

#define MSG_LOCAL_VEHICLE_ALERT_ST_LEN	sizeof(msg_local_vehicle_alert_st)
/* 邻车危险告警 neighbour vehicle alert info structure*/
typedef struct _msg_nb_vehicle_alert_info_st
{
	/* Message system time. */
	uint32_t            system_time;
	/*邻节点告警信息*/
}msg_nb_vehicle_alert_info_st,*msg_nb_vehicle_alert_info_st_ptr;

#define	MSG_NB_VEHICLE_ALERT_INFO_LEN	sizeof(msg_nb_vehicle_alert_info_st)

/* 路测提示告警 roadsize alert info structure*/
typedef struct _msg_roadsize_alert_info_st
{
	uint8_t reserved;
}msg_roadsize_alert_info_st,*msg_roadsize_alert_info_st_ptr;

#define MSG_ROADSIZE_ALERT_INFO_ST_LEN	sizeof(msg_roadsize_alert_info_st)

#endif /* _APP_MSG_FORMAT_H_ */
