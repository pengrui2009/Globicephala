/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_msg_rsa.h
 @brief  : MSG_RoadSideAlert(RSA) routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/
#ifndef __CV_MSG_RSA_H__
#define __CV_MSG_RSA_H__

#include "cv_data_frame.h"
#include "cv_vam.h"



typedef uint16_t itis_codes_t;







/* Rsa message optional configuration. */
typedef struct _rsa_msg_opt_st
{
    /* Message optional setting. */
    uint8_t msg_timestamp                   :1;
    uint8_t msg_description                 :1;
    uint8_t msg_priority                    :1;
    uint8_t msg_heading                     :1;
    uint8_t msg_extent                      :1;
    uint8_t msg_position                    :1;
    uint8_t msg_further_infoid              :1;
    uint8_t msg_regional                    :1;

    /* Description optional setting. */
    uint8_t description_ele1                :1;
    uint8_t description_ele2                :1;
    uint8_t description_ele3                :1;
    uint8_t description_ele4                :1;
    uint8_t description_ele5                :1;
    uint8_t description_ele6                :1;
    uint8_t description_ele7                :1;
    uint8_t description_ele8                :1;
    
    /* Position optional setting. */ 
    DF_FullPositionVector_opt_st  position_opt;


    /* Regional optional setting. */
    
}rsa_msg_opt_st, *rsa_msg_opt_st_ptr;

#define RSA_MSG_OPT_ST_LEN    (sizeof(rsa_msg_opt_st))




/* Build rsa message into the specific buffer. */
extern int rsa_build_msg(rsa_msg_opt_st_ptr opt_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *valid_bit_ptr);

/* Parse rsa message from the specific buffer. */
extern int rsa_analyse_msg(vam_rsa_evt_info_t *evt_ptr, uint8_t *buffer_ptr, uint16_t buffer_size);


#endif

