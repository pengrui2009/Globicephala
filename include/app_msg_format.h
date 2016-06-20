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
 �궨��
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
ö������
*************************************************/
/* ��Ϣ����ö��Message type enum*/
typedef enum _msg_type_em
{
    MSGTYPE_DEBUG     		= 0x1,       /* ������Ϣ */
    MSGTYPE_SYS_MANAGE 		= 0x2,       /* ���ò�ѯ��Ϣ */
    MSGTYPE_V2X_APPLY  		= 0x3,       /* �¼��ϱ���Ϣ */
    MSGTYPE_RAWDATA_DSRC 	= 0x4,       /* DSRCԭʼ��Ϣ */
    MSGTYPE_RAWDATA_NMEA 	= 0x5        /* NMEAԭʼ��Ϣ */
    
}MSG_TYPE_EM, *MSG_TYPE_EM_PTR;

#define MSG_TYPE_EM_LEN    (sizeof(MSG_TYPE_EM))


/* ������Ϣ����ö��enum*/
typedef enum _msg_subtype_em
{
    MSGSUBTYPE_VEHICLE_ALERT  = 0x1001,   /* �����澯��Ϣ */
    MSGSUBTYPE_VEHICLE_STATUS = 0x1002,   /* ����״̬��Ϣ */
    MSGSUBTYPE_SYSTEM_ERROR   = 0x1003    /* ϵͳ������Ϣ */

}MSG_SUBTYPE_EM, *MSG_SUBTYPE_EM_PTR;

#define MSG_SUBTYPE_EM_LEN    (sizeof(MSG_SUBTYPE_EM))


/* �ڵ���Ϣö�� enum*/
typedef enum _node_infor_type_em
{
    NODE_INFOR_TYPE_SUMMARY = 0x01,       /* �ڵ��Ҫ��Ϣ */
    NODE_INFOR_TYPE_DETAIL  = 0x02        /* �ڵ���ϸ��Ϣ */
    
}NODE_INFOR_TYPE_EM, *NODE_INFOR_TYPE_EM_PTR;

#define NODE_INFOR_TYPE_EM_LEN    (sizeof(NODE_INFOR_TYPE_EM))

/* ������״̬enum*/
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
/* ǣ����״̬ö��enum*/
typedef enum _traction_control_info_type_em
{
	TRACTION_CONTROL_UNAVAILABLE,
	TRACTION_CONTROL_OFF,
	TRACTION_CONTROL_ON,
	TRACTION_CONTROL_ENGAGED
}TRACTION_CONTROL_INFO_TYPE_EM;
/* ABS������״̬ö��enum*/
typedef enum _anti_brake_st_type_em
{
	ANTI_BRAKE_UNAVAILABLE,
	ANTI_RRAKE_OFF,
	ANTI_BRAKE_ON,
	ANTI_BRAKE_ENGAGED
}ANTI_BRAKE_ST_TYPE_EM;
/* SC�ȶ�����״̬ö�� enum*/
typedef enum _stabile_control_st_type_em
{
	STABILE_CONTROL_UNAVAILABLE,
	STABILE_CONTROL_OFF,
	STABILE_CONTROL_ON,
	STABILE_CONTROL_ENGAGED
}STABILE_CONTROL_ST_TYPE_EM;
/* �ƶ�Ӧ��״̬ö�� enum*/
typedef enum _brake_boost_info_type_em
{
	BRAKE_BOOST_UNAVAILABLE,
	BRAKE_BOOST_OFF,
	BRAKE_BOOST_ON
}BRAKE_BOOST_INFO_TYPE_EM;
/*  �����ƶ�ϵͳ״̬ö�� enum*/
typedef enum _aux_brake_st_type_em
{
	/* ������*/
	AUX_BRAKE_UNAVAILABLE,
	/*��״̬*/
	AUX_BRAKE_OFF,
	/*��״̬*/
	AUX_BRAKE_ON,
	/*Ԥ��*/
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
  �ṹ��
*************************************************/
/* 3Dλ��Position 3D structure. */
typedef struct  _position_3d_st
{
	/* γ��: unit 0.1 micro degree, (-900000000 - +900000001), 900000001 means invalid. */
    int32_t  latitude;                      /* γ�� */
    /* ����: unit 0.1 micro degree, (1799999999 - 1800000001), 1800000001 means invalid. */
    int32_t longitude;                      /* ���� */
    /* ����: unit 0.1 meter, (-4096 - +61439), -4096 means invalid. */
    int16_t elevation;                      /* ���� */
    
}position_3d_st, *position3d_st_ptr;

#define POSITION3D_ST_LEN    (sizeof(position3d_st))

/* λ����Ϣ׼ȷ�� Position accurary structure. */
typedef struct _position_accu_st
{
	/* ������׼ȷ��: unit 5 cm, (0 - 255) [254 means >=254], 255 means invalid. */
	uint8_t			semimajoraxisAccu;//������׼ȷ��
	/* �̰���׼ȷ��: unit 5 cm, (0 - 255) [254 means >=254], 255 means invalid. */
	uint8_t			semiminoraxisAccu;//�̰���׼ȷ��
	/* �����᷽���: unit 0.0054932479 degree, (0-65534), 65535 means invalid. */
	uint16_t		semimajorAxisOrien;//�����᷽��
}position_accu_st,*position_accu_st_ptr;

#define POSITION_ACCURARY_LEN	sizeof(ehm_position_accu)

/* ���ٶ�accleration 4 way structure*/
typedef struct _accleration_set4way_st
{
	/* ������ٶ�(x): unit 0.01 m/s^2, (-2000 - +2000) [-2000 means <=-2000 +2000 means >=2000], 2001 means invalid. */
	int16_t			lonacce;//x���ٶ�
	/* ������ٶ�(y): unit 0.01 m/s^2, (-2000 - +2001) [-2000 means <=-2000 +2000 means >=2000], 2001 means invalid. */
	int16_t			latacce;//y���ٶ�
	/* ��ֱ���ٶ�(z): unit 0.02G(0.1962 m/s^2), (-127 - +127) [+127 means >= 2.54G -126 means <= -2.52G ], -127 means invalid. */
	int8_t			veracce;//��ֱ���ٶ�
	/* ƫ����: unit 0.01 degree/s, (-32767 - +32767). */
	int16_t			yawrate;//ƫ����
}accleration_set4way_st,*accleration_set4way_st_ptr;

#define ACCLERATION_SET4WAY_LEN	(sizeof(accleration_set4way_st))
/* ���������ƶ�״̬brake applied status structure*/
typedef union _wheel_brake_st
{
	uint8_t wheel_brake_word;

	struct _wheel_brake_bit{
		#ifndef __LITTLE_ENDIAN
		/*Ԥ��*/
			uint8_t	reserved:4;
		/*�Һ��� 	0:disactive		1:active ,�����������ֻ��һ������,���λ:0*/
			uint8_t rightrear:1;
		/*��ǰ��		0:active		1:active ,�����������ֻ��һ������,���λ:0*/
			uint8_t rightfront:1;
		/*�����		0:active		1:active ,�����������ֻ��һ������,��������Ӵ�����Ч*/
			uint8_t leftrear:1;
		/*��ǰ��		0:active		1:active ,�������ǰ��ֻ��һ������,��������Ӵ�����Ч*/
			uint8_t leftfront:1;
		#else
		/*��ǰ��		0:active		1:active ,�������ǰ��ֻ��һ������,��������Ӵ�����Ч*/
			uint8_t leftfront:1;
		/*�����		0:active		1:active ,�����������ֻ��һ������,��������Ӵ�����Ч*/
			uint8_t leftrear:1;
		/*��ǰ��		0:active		1:active ,�����������ֻ��һ������,���λ:0*/
			uint8_t rightfront:1;
		/*�Һ��� 	0:disactive		1:active ,�����������ֻ��һ������,���λ:0*/
			uint8_t rightrear:1;
			uint8_t	reserved:4;
		#endif
	}__COMPILE_PACK__ wheel_brake_bit;
}wheel_brake_st;

/* �ƶ�ϵͳ״̬ brake system status structure*/
typedef struct _brake_system_status_st
{
	/*�ƶ�Ӧ��״̬ */
	wheel_brake_st		braas;//�ƶ�Ӧ��״̬
	/*ǣ��������״̬  0:��Ч 	1:�� 	 2:��	3��ʹ����*/
	uint8_t		tracs;//ǣ��������״̬
	/*ABS������״̬   0:��Ч 	1:�� 	 2:��	3��ʹ����*/
	uint8_t		antbs;//ABS������״̬
	/*SC�ȶ�����״̬   0:��Ч 	1:�� 	 2:��	3��ʹ����*/
	uint8_t		stacs;//SC�ȶ�����״̬
	/*�ƶ���ѹӦ��	0:��Ч 	1:�� 	 2:��	*/
	uint8_t		braba;//�ƶ���ѹӦ��
	/*�����ƶ�״̬   0:��Ч 	1:�� 	 2:��	3��Ԥ��*/
	uint8_t		auxbs;//�����ƶ�״̬
}brake_system_status_st,*brake_system_status_st_ptr;

#define BRAKE_SYSTEM_STATUS_LEN	(sizeof(brake_system_status_st))
/* �ⲿ��״̬external lights structure*/
typedef union _exterior_lights_un
{
	uint16_t	exterior_ligths;
	struct _exterior_lights_st
	{
		/* reserved*/
		uint16_t		reserved:7;
		/* ͣ��ָʾ�ƿ���*/
		uint16_t		parkinglight:1;
		/* ��ƿ���*/
		uint16_t		fogligthon:1;
		/* �ռ��г��ƿ���*/
		uint16_t		daytimerunninglight:1;
		/* �Զ����ȵ��ڿ���*/
		uint16_t		automaticlight:1;
		/* Σ��ָʾ�ƿ���*/
		uint16_t		hazardsignallight:1;
		/* ��ת�źŵƿ���*/
		uint16_t		rightturnsignallight:1;
		/* ��ת�źŵƿ���*/
		uint16_t		leftturnsignallight:1;
		/* Զ��ƿ���*/
		uint16_t		highbeamheadlight:1;
		/* ����ƿ���*/
		uint16_t		lowbeamheadlight:1;
	}exterior_lights_st;
}exterior_lights_un;

/* ������С�ṹ�� vehicle size structure*/
typedef struct _vehicle_size_info_st
{
	/* �������: unit 1 cm, (0 - 1023). */
	uint16_t		vehiclewidth;//�������
	/* ��������: unit 1 cm, (0 - 4095). */
	uint16_t		vehiclelength;//��������
}vehicle_size_info_st,*vehicle_size_info_st_ptr;

#define VEHICLE_SIZE_LEN	sizeof(vehicle_size_info);
/* �澯��ʶ Alert flag structure */
typedef union _alert_flag_st
{
    /* Alert word for whole group. */
    uint32_t alert_word;                  /* �澯λ��� */
    
    /* Alert bits. */
    struct _alert_bit
    {
	#ifndef __LITTLE_ENDIAN
    	uint32_t reserved          :25;   /* ����λ */
		uint32_t rsd_tunnel        :1;    /* ����澯 */
		uint32_t vec_danger_goods  :1;    /* Σ�ջ������䳵�澯 */
		uint32_t vec_ambulance     :1;    /* �Ȼ����澯 */
		uint32_t vec_brake_hard    :1;    /* ����ɲ���澯 */
		uint32_t vec_breakdown     :1;    /* �������ϸ澯 */
		uint32_t vec_neardis_rear  :1;    /* �󳵽�����澯 */
		uint32_t vec_neardis_front :1;    /* ǰ��������澯 */
	#else
    	uint32_t vec_neardis_front :1;    /* ǰ��������澯 */
		uint32_t vec_neardis_rear  :1;    /* �󳵽�����澯 */
		uint32_t vec_breakdown     :1;    /* �������ϸ澯 */
		uint32_t vec_brake_hard    :1;    /* ����ɲ���澯 */
		uint32_t vec_ambulance     :1;    /* �Ȼ����澯 */
		uint32_t vec_danger_goods  :1;    /* Σ�ջ������䳵�澯 */
		uint32_t rsd_tunnel        :1;    /* ����澯 */
		uint32_t reserved          :25;   /* ����λ */
	#endif
    }__COMPILE_PACK__ alert_bit;
    
}alert_flag_st, *alert_flag_st_ptr;

#define ALERT_FLAG_ST_LEN    (sizeof(alert_flag_st))

/* Alert bit status. */
#define ALERT_FLAG_BIT_YES         0x01
#define ALERT_FLAG_BIT_NO          0x00

/* �ڵ��Ҫ��ϢNode summary information structure. */
typedef struct _node_summary_info_st{
	/* �ڵ�ID:(0,0,0,0)-invalid id */
	uint8_t       node_id[4];
	/* �������(�Ա���λ��Ϊ��׼): unit 0.01 m, (-32768 - +32767). */
	int16_t longitudinal_dis;
	/* �������(�Ա���λ��Ϊ��׼): unit 0.01 m, (-32768 - +32767). */
	int16_t  latitudinal_dis;
	/* �澯��� */
	alert_flag_st alert_flag;
}node_summary_info_st,*node_summary_info_st_ptr;

/* �ڵ���ϸ��ϢNode detailed information structure. */
typedef struct  _node_detail_infor_st
{
	/* �ڵ�ID*/
    uint8_t       node_id[4];
    /* �г�����(�������˳ʱ��н�): unit 0.0125 degree, 	(0 - +359.9875), 360 means invalid. */
    float          direction;
    /* �ٶ�:km/h. */
    float              speed;
    /* 3Dλ�� */
    position_3d_st      pos3d;
    /* �������: unit 0.01 m, (-32768 - +32767). */
    int16_t  longitudinal_dis;
    /* �������: unit 0.01 m, (-32768 - +32767). */
    int16_t  latitudinal_dis;
    /*�澯��� */
    alert_flag_st alert_flag;    
}node_detail_infor_st, *node_detail_infor_st_ptr;

#define NODE_DETAIL_INFOR_ST_LEN    (sizeof(node_detail_infor_st))

/* �ڽڵ��Ҫ��Ϣneigbhour node summary information structure. */
typedef struct  _nb_node_summary_infor_st
{
	/* �ڵ�ID:(0,0,0,0)-invalid id */
    uint8_t       node_id[4];
    /* �������(�Ա���λ��Ϊ��׼): unit 0.01 m, (-32768 - +32767). */
    int16_t longitudinal_dis;
    /* �������(�Ա���λ��Ϊ��׼): unit 0.01 m, (-32768 - +32767). */
    int16_t  latitudinal_dis;
    /* �����ʻ����(�Ա�����ʻ����Ϊ��׼)��uint 0.0125 degree (0 - +28799) +28800 means invalid*/
    uint16_t	angle;
    /* ��Գ���(�Ա�������Ϊ��׼)��uint 0.02m/s (0 - +8190) 8191 means invalid*/
    uint16_t	velocity;
    /* �ź�ǿ��(�ھ�-����ͨ��):uint 1dBm (-127 - +126) [-127 means <=-127 +126 means >=+126] +127 means unavailable value*/
    int8_t		signalstrength;
    /* ������(�ھ�-����ͨ��):	uint 1%	[0 means 0% 100 means 100%] 101 means invalid */
    int8_t		losstolerance;
    
}nb_node_summary_infor_st, *nb_node_summary_infor_st_ptr;
#define NB_NODE_SUMMARY_INFOR_ST_LEN	sizeof(nb_node_summary_infor_st)
/*�ڽڵ���ϸ��Ϣ*/
typedef struct _nb_node_detial_infor_st
{
	/* �ڵ�ID:(0,0,0,0)-invalid id */
	uint8_t       		node_id[4];
	/* 3Dλ�� */
	position_3d_st     	pos3d;
	/* λ�þ�ȷ��*/
	position_accu_st 	posaccu;
	/* ������״̬: 0������״̬ 	1��פ��״̬	2:ǰ��״̬	3:����״̬	4:����	5:����	6:����	7:unavailable */
	uint8_t		 			tran_state;//������״̬
	/* ����(Դ��GPSģ����߳�������)��uint 0.02m/s (0 - +8190) 8191 means invalid*/
	uint16_t	velocity;
	/* ������ʻ����(��������Ϊ��׼)��uint 0.0125 degree (0 - +28799) +28800 means invalid*/
	uint16_t	angle;
	/* ������ת��:uint 1.5 degrees (-126 - +126) [-126 means <= -189 deg +126 means >=+189 deg] +127 means unavaliable*/
	int8_t 		steer_wheel_angle;
	/* ���ٶ�*/
	accleration_set4way_st acc;
	/* �ƶ�ϵͳ״̬(���ǰ��Ϊ����,����ǰ��Ϊ�ο�;��� ����Ϊ����,�������Ϊ�ο�)*/
	brake_system_status_st brake;
	/* �ⲿ�ƹ�״̬*/
	exterior_lights_un	exterlight;
}nb_node_detail_infor_st,*nb_node_detial_infor_st_ptr;

#define NB_NODE_DETAIL_INFOR_ST_LEN    (sizeof(nb_node_detail_infor_st))
/* Message header structure ---------------------------------------------*/
/* ����֡ͷ��Ϣ�ṹ�� uart Message header structure */
typedef  struct  _uart_msg_header_st
{
	/* ����֡�ױ�ʶ��:	0x55. */
	uint8_t magic_num1;

	/* ����֡�ױ�ʶ��:	0xAA. */
	uint8_t magic_num2;

	/*֡����: DATA + CHK */
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
/*֡��Ϣͷ��Ϣ  frame msg header structure*/
typedef  struct  _frame_msg_header_st
{
#ifndef __LITTLE_ENDIAN
	/*��Ϣ��ʶ��:	0xE*/
	uint8_t	mark:4;//B1110[0xE]
	/*��ϢԴ��	0:v2x	1:host*/
	uint8_t	src:1;//0: from v2x 1: from host
	/*Ԥ���ֶΣ�	0*/
	uint8_t	reserved1:3;//B000[0x0]
	/*Ԥ���ֶΣ�	0*/
	uint8_t	reserved2:4;//B000[0x0]
	/*��Ϣ����:	0:Ԥ�� 1:������Ϣ 2:ϵͳ������Ϣ 3:v2xӦ����Ϣ 4:dsrc��Ϣ 5:gps��Ϣ 6:δ����*/
	uint8_t	type:4;//0:reserved 1:debug 2:sys manager 3:v2x apply 4:dsrc 5:gps 6:undefine
#else
	/*Ԥ���ֶΣ�	0*/
	uint8_t	reserved1:3;//B000[0x0]
	/*��ϢԴ��	0:v2x	1:host*/
	uint8_t	src:1;//0: from v2x 1: from host
	/*��Ϣ��ʶ��:	0xE*/
	uint8_t	mark:4;//:B1110[0xE]
	/*��Ϣ����:	0:Ԥ�� 1:������Ϣ 2:ϵͳ������Ϣ 3:v2xӦ����Ϣ 4:dsrc��Ϣ 5:gps��Ϣ 6:δ����*/
	uint8_t	type:4;//0:reserved 1:debug 2:sys manager 3:v2x apply 4:dsrc 5:gps
	/*Ԥ���ֶΣ�	0*/
	uint8_t	reserved2:4;//B000[0x0]
#endif
}frame_msg_header_st, *frame_msg_header_st_ptr;

#define MSG_HEADER_MARK						0xE
#define MSG_SRC_V2X							0x0
#define MSG_SRC_HOST						0x1
#define FRAME_MSG_HEADER_ST_LEN              		(sizeof(frame_msg_header_st))

/* Message Sets ----------------------------------------------------------*/

/* �ھӽڵ���Ϣ  msg neighbour node info structure */
typedef struct  _msg_vehicle_nb_status_st
{
    /* Message system time. */
    uint32_t            system_time;
    /*Node Number*/
    uint8_t				nodenumber;
    /* Set the neigbour's node information type. */
    uint8_t         	node_infor_type;
    /* �ڳ���Ҫ��Ϣ/�ڳ�������Ϣ N * (Node summary/detail information structure). (0 <= N) */
    

}msg_vehicle_nb_status_st, *msg_vehicle_nb_status_st_ptr;

#define MSG_VEHICLE_NB_STATUS_ST_LEN    (sizeof(msg_vehicle_nb_status_st))

/* ��������״̬basic Vehicle status structure */
typedef struct _msg_basic_status_st
{
	/* �ڵ�ID:(0,0,0,0)-invalid id */
	uint8_t       			node_id[4];
	/*3Dλ��*/
	position_3d_st 			position;//3Dλ��
	/* λ�þ�ȷ�� */
	position_accu_st		posaccu;//λ�þ�ȷ��
	/* �ٶ�: unit 0.02 m/s, (0 - 8191), 8191 means invalid. */
	uint16_t				velocity;//�ٶ�
	/* ��ʻ����: unit 0.0125 degree (�������˳ʱ��н�), (0 - 28800), 28800 means invalid. */
	uint16_t				angle;//��ʻ����
}msg_vehicle_basic_status_st,*msg_basic_status_st_ptr;

#define MSG_VEHICLE_BASIC_STATUS_ST_LEN		(sizeof(msg_vehicle_basic_status_st))

/* ��������״̬ full Vehicle status structure */
typedef struct _msg_full_status_st
{
	/* �ڵ�ID:(0,0,0,0)-invalid id */
	uint8_t       			node_id[4];
	/* 3Dλ��  */
	position_3d_st 			position;//3Dλ��
	/* λ�þ�ȷ�� */
	position_accu_st		posaccu;//λ�þ�ȷ��
	/* ������״̬: 0������״̬ 	1��פ��״̬	2:ǰ��״̬	3:����״̬	4:����	5:����	6:����	7:unavailable */
	uint8_t		 			tran_state;//������״̬
	/* �ٶ�: unit 0.02 m/s, (0 - 8191), 8191 means invalid. */
	uint16_t				velocity;//�ٶ�
	/* ��ʻ����: unit 0.0125 degree(�������˳ʱ��н�), 	(0 - 28800), 28800 means invalid. */
	uint16_t				angle;//��ʻ����
	/* ������ת��: unit 1.5 degree, (-126 - +127), 127 means invalid. */
	int8_t					steerwa;//������ת��
	/* 4·���ٶȼ�   */
	accleration_set4way_st	acce4way;//4·���ٶȼ�
	/* �ƶ�ϵͳ״̬  */
	brake_system_status_st	braksta;//�ƶ�ϵͳ״̬
	/* �ⲿ�ƹ�״̬*/
	exterior_lights_un		exterlight;
}msg_vehicle_full_status_st,msg_full_status_st_ptr;

#define	MSG_VEHICLE_FULL_STATUS_ST_LEN	(sizeof(msg_vehicle_full_status_st))

/* ������̬��Ϣ Vehicle static info structure */
typedef struct _vehicle_static_info_st
{
	/* ��������*/
	VEHICLE_TYPE_EM			vehicletype;
	/* �����ߴ�*/
	vehicle_size_info_st 	vehicle_size;
}msg_vehicle_static_info_st,vehicle_static_info_st_ptr;

#define MSG_VEHICLE_STATIC_INFO_ST_LEN	sizeof(msg_vehicle_static_info_st)
/* ���س����澯���� local Vehicle alert set info structure */
typedef struct _msg_local_vehicle_alert_st
{
	/* �������ϸ澯��00b(Invalid),01b(Off),10b(On),11b(Reserved)*/
	uint16_t	vecbrakedownalert;
	/* ����ɲ���澯:00b(Invalid),01b(Off),10b(On),11b(Reserved)*/
	uint16_t vecbrakehardalert;
}msg_local_vehicle_alert_st,*msg_local_vehicle_alert_st_ptr;

#define MSG_LOCAL_VEHICLE_ALERT_ST_LEN	sizeof(msg_local_vehicle_alert_st)
/* �ڳ�Σ�ո澯 neighbour vehicle alert info structure*/
typedef struct _msg_nb_vehicle_alert_info_st
{
	/* Message system time. */
	uint32_t            system_time;
	/*�ڽڵ�澯��Ϣ*/
}msg_nb_vehicle_alert_info_st,*msg_nb_vehicle_alert_info_st_ptr;

#define	MSG_NB_VEHICLE_ALERT_INFO_LEN	sizeof(msg_nb_vehicle_alert_info_st)

/* ·����ʾ�澯 roadsize alert info structure*/
typedef struct _msg_roadsize_alert_info_st
{
	uint8_t reserved;
}msg_roadsize_alert_info_st,*msg_roadsize_alert_info_st_ptr;

#define MSG_ROADSIZE_ALERT_INFO_ST_LEN	sizeof(msg_roadsize_alert_info_st)

#endif /* _APP_MSG_FORMAT_H_ */
