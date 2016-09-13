/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_vam_df.c
 @brief  : Data Frame routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/

#include "cv_vam_df.h"
#include "J2735.h"


/* Allocate routine for DF_DDateTime. */
int DF_DDateTime_allocate(DDateTime_t **time_ptr_ptr, DF_DDateTime_opt_st_ptr opt_ptr)
{
    DDateTime_t *time_ptr = NULL;
    

    /* Allocate room for structure itself. */
    time_ptr = calloc(1, sizeof(*time_ptr));
    if(time_ptr == NULL)
    {
        goto ERR_EXIT;
    }

    /* DYear. */
    if(opt_ptr->DYear == MSG_OPTIONAL_YES)
    {
        time_ptr->year = calloc(1, sizeof(*time_ptr->year));
        if(time_ptr->year != NULL)
        {
            * time_ptr->year = 2016;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->year = NULL;
    }

    /* DMonth. */
    if(opt_ptr->DMonth == MSG_OPTIONAL_YES)
    {
        time_ptr->month = calloc(1, sizeof(*time_ptr->month));
        if(time_ptr->month != NULL)
        {
            * time_ptr->month = 9;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->month = NULL;
    }
    
    /* DDay. */
    if(opt_ptr->DDay == MSG_OPTIONAL_YES)
    {
        time_ptr->day = calloc(1, sizeof(*time_ptr->day));
        if(time_ptr->day != NULL)
        {
            * time_ptr->day = 21;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->day = NULL;
    }

    /* DHour. */
    if(opt_ptr->DHour == MSG_OPTIONAL_YES)
    {
        time_ptr->hour = calloc(1, sizeof(*time_ptr->hour));
        if(time_ptr->hour != NULL)
        {
            * time_ptr->hour = 8;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->hour = NULL;
    }
    
    /* DMinute. */
    if(opt_ptr->DMinute == MSG_OPTIONAL_YES)
    {
        time_ptr->minute = calloc(1, sizeof(*time_ptr->minute));
        if(time_ptr->minute != NULL)
        {
            * time_ptr->minute = 30;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->minute = NULL;
    }

    /* DSecond. */
    if(opt_ptr->DSecond == MSG_OPTIONAL_YES)
    {
        time_ptr->second = calloc(1, sizeof(*time_ptr->second));
        if(time_ptr->second != NULL)
        {
            * time_ptr->second = 47;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->second = NULL;
    }

    /* DOffset. */
    if(opt_ptr->DOffset == MSG_OPTIONAL_YES)
    {
        time_ptr->offset = calloc(1, sizeof(*time_ptr->offset));
        if(time_ptr->offset != NULL)
        {
            * time_ptr->offset = 30;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->offset = NULL;
    }

    *time_ptr_ptr = time_ptr;
    return 0;


ERR_EXIT:

    DF_DDateTime_free(time_ptr);
    *time_ptr_ptr = NULL;
    
    return -1;
}


/* Free routine for DF_DDateTime. */
int DF_DDateTime_free(DDateTime_t *time_ptr)
{
    if(time_ptr != NULL)
    {
        if(time_ptr->year != NULL)
        {
            free(time_ptr->year);
        }

        if(time_ptr->month != NULL)
        {
            free(time_ptr->month);
        }
        
        if(time_ptr->day != NULL)
        {
            free(time_ptr->day);
        }
        
        if(time_ptr->hour != NULL)
        {
            free(time_ptr->hour);
        }
        
        if(time_ptr->minute != NULL)
        {
            free(time_ptr->minute);
        }
        
        if(time_ptr->second != NULL)
        {
            free(time_ptr->second);
        }
        
        if(time_ptr->offset != NULL)
        {
            free(time_ptr->offset);
        }

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_FullPositionVector. */
int DF_FullPositionVector_allocate(FullPositionVector_t **pos_ptr_ptr, DF_FullPositionVector_opt_st_ptr opt_ptr, vam_stastatus_t *vamstatus_ptr)
{
    FullPositionVector_t  *pos_ptr = NULL;


    /* Allocate room for structure itself. */
    pos_ptr = calloc(1, sizeof(*pos_ptr));
    if(pos_ptr == NULL)
    {
        goto ERR_EXIT;
    }

    /* DDateTime. */
    if(opt_ptr->DDateTime == MSG_OPTIONAL_YES)
    {
        if(DF_DDateTime_allocate(&(pos_ptr->utcTime), &(opt_ptr->DDataTime_opt)) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->utcTime = NULL;
    }

    /* Longitude. */
    pos_ptr->Long = encode_longitude(vamstatus_ptr->pos.longitude);

    /* Latitude. */
    pos_ptr->lat = encode_latitude(vamstatus_ptr->pos.latitude);

    /* Elevation. */
    if(opt_ptr->Elevation == MSG_OPTIONAL_YES)
    {
        pos_ptr->elevation = calloc(1, sizeof(*pos_ptr->elevation));
        if(pos_ptr->elevation != NULL)
        {
            * pos_ptr->elevation = encode_elevation(vamstatus_ptr->pos.elevation);
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->elevation = NULL;
    }

    /* Heading. */
    if(opt_ptr->Heading == MSG_OPTIONAL_YES)
    {
        pos_ptr->heading = calloc(1, sizeof(*pos_ptr->heading));
        if(pos_ptr->heading != NULL)
        {
            * pos_ptr->heading = encode_angle(vamstatus_ptr->dir);;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->heading = NULL;
    }

    /* TransmissionAndSpeed. */
    if(opt_ptr->TransmissionAndSpeed == MSG_OPTIONAL_YES)
    {
        pos_ptr->speed = calloc(1, sizeof(*pos_ptr->speed));
        if(pos_ptr->speed != NULL)
        {
            pos_ptr->speed->transmisson = vamstatus_ptr->transmission_state;;
            pos_ptr->speed->speed = encode_absolute_velocity(vamstatus_ptr->speed);
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->speed = NULL;
    }
    
    /* PositionAccuracy. */
    if(opt_ptr->PositionAccuracy == MSG_OPTIONAL_YES)
    {
        pos_ptr->posAccuracy = calloc(1, sizeof(*pos_ptr->posAccuracy));
        if(pos_ptr->posAccuracy != NULL)
        {
            pos_ptr->posAccuracy->semiMajor = encode_semimajor_axis_accuracy(vamstatus_ptr->pos_accuracy.semi_major_accu);
            pos_ptr->posAccuracy->semiMinor = encode_semiminor_axis_accuracy(vamstatus_ptr->pos_accuracy.semi_minor_accu);
            pos_ptr->posAccuracy->orientation = encode_semimajor_axis_orientation(vamstatus_ptr->pos_accuracy.semi_major_orientation);
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->posAccuracy = NULL;
    }

    /* TimeConfidence. */
    if(opt_ptr->TimeConfidence == MSG_OPTIONAL_YES)
    {
        pos_ptr->timeConfidence = calloc(1, sizeof(*pos_ptr->timeConfidence));
        if(pos_ptr->timeConfidence != NULL)
        {
            *pos_ptr->timeConfidence = TimeConfidence_time_050_000;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->timeConfidence = NULL;
    }

    /* PositionConfidenceSet. */
    if(opt_ptr->PositionConfidenceSet == MSG_OPTIONAL_YES)
    {
        pos_ptr->posConfidence = calloc(1, sizeof(*pos_ptr->posConfidence));
        if(pos_ptr->posConfidence != NULL)
        {
            pos_ptr->posConfidence->pos = PositionConfidence_a10m;
            pos_ptr->posConfidence->elevation = ElevationConfidence_elev_000_05;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->posConfidence = NULL;
    }

    /* SpeedandHeadingandThrottleConfidence. */
    if(opt_ptr->SpeedHeadingThrottle == MSG_OPTIONAL_YES)
    {
        pos_ptr->speedConfidence = calloc(1, sizeof(*pos_ptr->posConfidence));
        if(pos_ptr->speedConfidence != NULL)
        {
            pos_ptr->speedConfidence->heading = HeadingConfidence_prec05deg;
            pos_ptr->speedConfidence->speed = SpeedConfidence_prec10ms;
            pos_ptr->speedConfidence->throttle = ThrottleConfidence_prec1percent;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->speedConfidence = NULL;
    }

    *pos_ptr_ptr = pos_ptr;
    
    return 0;
   
ERR_EXIT:
    
    DF_FullPositionVector_free(pos_ptr);
    *pos_ptr_ptr = NULL;
    
    return -1;
}


/* Free routine for FullPositionVector. */
int DF_FullPositionVector_free(FullPositionVector_t *pos_ptr)
{
    if(pos_ptr != NULL)
    {
        if(pos_ptr->utcTime != NULL)
        {
            DF_DDateTime_free(pos_ptr->utcTime);
        }
        if(pos_ptr->elevation != NULL)
        {
            free(pos_ptr->elevation);
        }
        if(pos_ptr->heading != NULL)
        {
            free(pos_ptr->heading);
        }
        if(pos_ptr->speed != NULL)
        {
            free(pos_ptr->speed);
        }
        if(pos_ptr->posAccuracy != NULL)
        {
            free(pos_ptr->posAccuracy);
        }
        if(pos_ptr->timeConfidence != NULL)
        {
            free(pos_ptr->timeConfidence);
        }
        if(pos_ptr->posConfidence != NULL)
        {
            free(pos_ptr->posConfidence);
        }
        if(pos_ptr->speedConfidence != NULL)
        {
            free(pos_ptr->speedConfidence);
        }

        free(pos_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}

