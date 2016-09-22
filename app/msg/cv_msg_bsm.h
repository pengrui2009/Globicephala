/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_msg_bsm.h
 @brief  : MSG_BasicSafetyMessage routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/

#ifndef __CV_MSG_BSM_H__
#define __CV_MSG_BSM_H__

#include "cv_data_frame.h"



/* Bsm message optional configuration. */
typedef struct _bsm_msg_opt_st
{
    uint8_t msg_regional       :1;

    uint8_t reserved           :7;
    
}bsm_msg_opt_st, *bsm_msg_opt_st_ptr;

#define BSM_MSG_OPT_ST_LEN    (sizeof(bsm_msg_opt_st))


/* Bsm message structure. */
typedef struct _bsm_msg_st
{ 
    /* Optional. */
    bsm_msg_opt_st                 opt;

    /* Data. */
    DF_BSMcoreData_st         coreData;
    DF_BSMpartIIExtension_st partIIExt;

    uint8_t region;

} bsm_msg_st, * bsm_msg_st_ptr;

#define BSM_MSG_ST_LEN    (sizeof(bsm_msg_st))




/* Build bsm message into the specific buffer. */
extern int bsm_build_msg(bsm_msg_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *valid_bit_ptr);

/* Parse bsm message from the specific buffer. */
extern int bsm_analyse_msg(bsm_msg_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size);



#endif

