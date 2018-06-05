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


/* Rsa message optional configuration. */
typedef struct _rsa_msg_opt_st
{
    uint8_t timeStamp               :1;
    uint8_t description             :1;
    uint8_t priority                :1;
    uint8_t heading                 :1;
    uint8_t extent                  :1;
    uint8_t position                :1;
    uint8_t furtherInforID          :1;
    uint8_t regional                :1;
    
}rsa_msg_opt_st, *rsa_msg_opt_st_ptr;

#define RSA_MSG_OPT_ST_LEN    (sizeof(rsa_msg_opt_st))


/* Rsa message structure. */
typedef struct _rsa_msg_st
{ 
    /* Optional. */
    rsa_msg_opt_st                opt;

    /* Data. */
    uint8_t                    msgCnt;
    uint32_t                timeStamp;
    uint16_t                typeEvent;

    uint8_t            decription_num;
    uint16_t       description_ele[8];

    uint8_t                  priority;
    DE_HeadingSlice_st        heading;
    DE_Extent_en               extent;
    DF_FullPositionVector_st position;
    uint8_t         furtherInforID[2];

    uint8_t               regional[4];

} rsa_msg_st, * rsa_msg_st_ptr;

#define RSA_MSG_ST_LEN    (sizeof(rsa_msg_st))



/* Build rsa message into the specific buffer. */
extern int rsa_build_msg(rsa_msg_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *valid_bit_ptr);

/* Parse rsa message from the specific buffer. */
extern int rsa_analyse_msg(rsa_msg_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size);


#endif

