/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_vam_bsm.h
 @brief  : MSG_BasicSafetyMessage routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/

#ifndef __CV_VAM_BSM_H__
#define __CV_VAM_BSM_H__

#include "cv_vam_df.h"


/* Bsm message optional configuration. */
typedef struct _bsm_msg_optional_st
{
    /* Message optional setting. */
    uint8_t msg_partii                      :1;
    uint8_t msg_regional                    :1;


    /* Partii optional setting. */ 
    uint8_t partii_vehicle_safety_ext       :1;
    uint8_t partii_special_vehicle_ext      :1;
    uint8_t partii_supplemental_vehicle_ext :1;

    /* Regional optional setting. */


    /* Vehicle safety extensions optional setting. */
    uint8_t vse_vehicle_event_flags         :1;
    uint8_t vse_path_history                :1;
    uint8_t vse_path_prediction             :1;
    uint8_t vse_exterior_lights             :1;
    
    /* Special vehicle extensions optional setting. */
    uint8_t spve_emergency_details          :1;
    uint8_t spve_event_description          :1;
    uint8_t spve_trailer_data               :1;

    /* Supplemental vehicle extensions optional setting. */
    uint8_t suve_basic_vehicle_class        :1;
    
}bsm_msg_optional_st, *bsm_msg_optional_st_ptr;

#define BSM_MSG_OPTIANAL_ST_LEN    (sizeof(bsm_msg_optional_st))



/* Build bsm message into the specific buffer. */
extern int bsm_build_msg(bsm_msg_optional_st_ptr optional_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *valid_bit_ptr);

/* Parse bsm message from the specific buffer. */
extern int bsm_analyse_msg(vam_envar_t *vam_ptr, uint8_t *buffer_ptr, uint16_t buffer_size);



#endif /* __CV_VAM_BSM_H__ */

