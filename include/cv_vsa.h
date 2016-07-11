/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_vsa.h
 @brief  : this file include the definition of vsa module
 @author : wangyifeng
 @history:
           2014-6-20    wangyifeng    Created file
           ...
******************************************************************************/
#ifndef __CV_VSA_H__
#define __CV_VSA_H__

/*****************************************************************************
 * definition of micro                                                       *
*****************************************************************************/

#define VSA_TIMER_PERIOD         SECOND_TO_TICK(1)
#define VSA_EBD_SEND_PERIOD      SECOND_TO_TICK(5)
#define VSA_POS_PERIOD           MS_TO_TICK(100)
#define DIRECTION_DIVIDE         22.5f

#define PI 3.1415926f

#define VSA_MSG_PROC    (VSA_MSG_BASE+1)
#define CCW_DEBOUNCE     5











/*down to top*/
#define  HIGHWAY_MODE   0x0F9F
#define  MOUNTAIN_MODE   0x0FBF
#define  CITY_MODE       0x0FD7
#define  CUSTOM_MODE     0x019F



enum VSA_APP_ID{
    VSA_ID_NONE = 0,
    VSA_ID_CRD,
    VSA_ID_CRD_REAR,
    VSA_ID_OPS,
    VSA_ID_SIDE,
    VSA_ID_VBD,    
    VSA_ID_EBD,
    VSA_ID_VOT,
    VSM_ID_END
};


/* target classification locations */

enum VSA_TARGET_LOCATION{
    POSITION_ERROR = 0,
    AHEAD_LEFT,        
    AHEAD,
    AHEAD_RIGHT,   
    RIGHT,
    BEHIND_RIGHT,
    BEHIND,
    BEHIND_LEFT,   
    LEFT
};

typedef struct _adpcm{

    uint32_t addr;
    uint32_t size;
    uint8_t  channel;
    uint8_t  cmd;

}adpcm_t;

/* Vehicle heading slice definition. */
typedef enum _vehicle_heading_slice
{
    SLICEx_NO_HEADING = 0,
    SLICEx_ALL_HEADING,

    SLICE0_000_0T022_5,  /* 22.5 degree starting from North and moving Eastward. */
    SLICE1_022_5T045_0,
    SLICE2_045_0T067_5,
    SLICE3_067_5T090_0,

    SLICE4_090_0T112_5,
    SLICE5_112_5T135_0,
    SLICE6_135_0T157_5,
    SLICE7_157_5T180_0,

    SLICE8_180_0T202_5,
    SLICE9_202_5T225_0,
    SLICE10_225_0T247_5,
    SLICE11_247_5T270_0,

    SLICE12_270_0T292_5,
    SLICE13_292_5T315_0,
    SLICE14_315_0T337_5,
    SLICE15_337_5T360_0

}vehicle_heading_slice;


#define DEGREE_PER_HEADING_SLICE    ((float)22.5)









typedef struct _vsa_info
{
    uint8_t pid[RCP_TEMP_ID_LEN];
    
    uint32_t vsa_location;

    float local_speed;

    float remote_speed;

    float relative_speed;
    float relative_dir;
    
    uint32_t v_offset;
    uint32_t h_offset;

    int32_t linear_distance;
    uint32_t  safe_distance;

    float dir;

    int8_t flag_dir;

    uint8_t jump_count;

    uint8_t vsa_id;

    uint8_t period;

}vsa_info_t;



typedef struct _vsa_position_node{

    list_head_t list;

    vsa_info_t vsa_position;
}vsa_position_node_t;


/*****************************************************************************
 * definition of struct                                                      *
*****************************************************************************/

typedef struct _vsa_config{
    /*
        General
    */
    
    uint8_t danger_detect_speed_threshold;  /* unit: km/h */
    uint16_t lane_dis;  /* unit:m, min accuracy :1m*/
    /*
        Close Range Danger function:
    */
    uint8_t crd_saftyfactor;  /* 1~10 */
    uint8_t crd_oppsite_speed;/*<=255:30km/h*/
    uint8_t crd_oppsite_rear_speed;/*<=255:30km/h*/
    uint8_t crd_rear_distance;/*<=255:10m*/
    /*
        Emergency Braking Danger function:
    */
    uint8_t ebd_mode;  /* 0 - disable, 1 - enable */
    uint8_t ebd_acceleration_threshold; /* unit:m/s2 */
    uint8_t ebd_alert_hold_time;  /* unit:s */
	
}vsa_config_t;



/* Vsa environment structure. */
typedef struct _vsa_envar_t
{
    uint32_t vsa_mode;
    
    /* working_param */
    vsa_config_t working_param;

    uint32_t gps_status;

    /* Vsa functionality switch definition. */
    uint32_t alert_mask;


    uint32_t alert_pend;

    /* add by wangliang */
    uint32_t alert_num;

    vam_stastatus_t remote;

    adpcm_t adpcm_data;

    /*List head*/
    list_head_t crd_list;
    list_head_t position_list;

    vsa_position_node_t position_node[VAM_NEIGHBOUR_MAXNUM];


    /* os related */
    osal_task_t  *task_vsa_l;
    osal_task_t  *task_vsa_r;

    osal_sem_t * sem_vsa_proc;
    osal_sem_t * sem_alert_list;
    
    osal_queue_t *queue_vsa;

    osal_queue_t *queue_voc;

    osal_timer_t *timer_ebd_send;

    osal_timer_t *timer_position_prepro;
                 
}vsa_envar_t, * vsa_envar_t_ptr;

#define VSA_ENVAR_T_LEN    (sizeof(vsa_envar_t))




typedef struct _vsa_crd_node{
    /* !!!DON'T modify it!!! */
    list_head_t list;

    uint8_t pid[RCP_TEMP_ID_LEN];  //temporary ID

    /*add by wangliang alarm node brief infor*/
    int16_t v_offset;

    int16_t h_offset;

    uint32_t alert_flag;
    /*add by wangliang*/
    
    /* ccw_id = 1(VSA_ID_CRD) is cfcw,ccw_id = 2(VSA_ID_CRD_REAR) is crcw.*/
    uint8_t ccw_id;

	uint8_t ccw_cnt;

    /* private */
    uint16_t life;
    

}vsa_crd_node_t;



typedef int (*vsa_app_handler)(vsa_envar_t *p_vsa, void *p_msg);


/*****************************************************************************
 * declaration of global variables and functions                             *
*****************************************************************************/



extern int8_t vsa_position_get(uint8_t *pid,vsa_info_t *p_vsa_position);






void vsa_start(void);

osal_status_t vsa_add_event_queue(vsa_envar_t *p_vsa, 
                             uint16_t msg_id, 
                             uint16_t msg_len, 
                             uint32_t msg_argc,
                             void    *msg_argv);

uint32_t vsa_get_alarm(uint32_t vsa_id);
#endif /* __CV_VSA_H__ */

