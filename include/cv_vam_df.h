/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_vam_df.h
 @brief  : Data Frame routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/
#ifndef __CV_VAM_DF_H__
#define __CV_VAM_DF_H__

#include "DDateTime.h"
#include "FullPositionVector.h"
#include "cv_vam.h"


/* DF_DDateTime optional configuration. */
typedef struct _DF_DDateTime_opt_st
{
    uint8_t DYear   :1;
    uint8_t DMonth  :1;
    uint8_t DDay    :1;
    uint8_t DHour   :1;
    uint8_t DMinute :1;
    uint8_t DSecond :1;
    uint8_t DOffset :1;

}DF_DDateTime_opt_st, *DF_DDateTime_opt_st_ptr;

#define DF_DDateTime_opt_st_len    (sizeof(DF_DDateTime_opt_st))


/* DF_FullPositionVector optional configuration. */
typedef struct _DF_FullPositionVector_opt_st
{
    uint8_t DDateTime              :1;
    uint8_t Elevation              :1;
    uint8_t Heading                :1;
    uint8_t TransmissionAndSpeed   :1;
    uint8_t PositionAccuracy       :1;
    uint8_t TimeConfidence         :1;
    uint8_t PositionConfidenceSet  :1;
    uint8_t SpeedHeadingThrottle   :1;

    DF_DDateTime_opt_st DDataTime_opt;     

}DF_FullPositionVector_opt_st, *DF_FullPositionVector_opt_st_ptr;

#define DF_FullPositionVector_opt_st_len    (sizeof(DF_FullPositionVector_opt_st))


/* Message optinal status. */
#define MSG_OPTIONAL_YES    0x01
#define MSG_OPTIONAL_NO     0x00



/* Allocate routine for DF_DDateTime. */
extern int DF_DDateTime_allocate(DDateTime_t **time_ptr_ptr, DF_DDateTime_opt_st_ptr opt_ptr);

/* Free routine for DF_DDateTime. */
extern int DF_DDateTime_free(DDateTime_t *time_ptr);

/* Allocate routine for DF_FullPositionVector. */
extern int DF_FullPositionVector_allocate(FullPositionVector_t **pos_ptr_ptr, DF_FullPositionVector_opt_st_ptr opt_ptr, vam_stastatus_t *vamstatus_ptr);

/* Free routine for FullPositionVector. */
extern int DF_FullPositionVector_free(FullPositionVector_t *pos_ptr);



#endif /* __CV_VAM_DF_H__ */

