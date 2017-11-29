/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_data_element.c
 @brief  : Data Element routine set.
 @author : wangxianwen
 @history:
           2016-09-20    wangxianwen    Created file
           2017-10-12     pengrui        Modify for CSAE
           ...
******************************************************************************/

#include "cv_data_element.h"
#include "prot_dataelem.h"
#include "error.h"


/* Free routine for DE_AllowedManeuvers. */
int DE_AllowedManeuvers_free(AllowedManeuvers_t *maneuvers_ptr)
{
    int result = 0;
    
    if(maneuvers_ptr != NULL)
    {
        if(maneuvers_ptr->buf != NULL)
        {
            FREEMEM(maneuvers_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(maneuvers_ptr, 0, sizeof(*maneuvers_ptr));
        
    }

   return result;
}


/* Allocate routine for DE_AllowedManeuvers. */
int DE_AllowedManeuvers_allocate(AllowedManeuvers_t *maneuvers_ptr, DE_AllowedManeuvers_un_ptr Maneuvers_ptr)
{
    int result = 0;

    uint16_t    event_mask = 0;


    /* Error detect. */
    if((maneuvers_ptr == NULL) || (Maneuvers_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(maneuvers_ptr, 0, sizeof(*maneuvers_ptr));

    /* Allocate bit data zone. */
    maneuvers_ptr->size = 2;
    maneuvers_ptr->bits_unused = 4;
    if((maneuvers_ptr->buf = CALLOC(1, maneuvers_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Maneuvers_ptr->bit.AllowedManeuvers_maneuverStraightAllowed       << (15 - AllowedManeuvers_maneuverStraightAllowed))
               | (Maneuvers_ptr->bit.AllowedManeuvers_maneuverLeftAllowed           << (15 - AllowedManeuvers_maneuverLeftAllowed))
               | (Maneuvers_ptr->bit.AllowedManeuvers_maneuverRightAllowed          << (15 - AllowedManeuvers_maneuverRightAllowed))
               | (Maneuvers_ptr->bit.AllowedManeuvers_maneuverUTurnAllowed          << (15 - AllowedManeuvers_maneuverUTurnAllowed))
               | (Maneuvers_ptr->bit.AllowedManeuvers_maneuverLeftTurnOnRedAllowed  << (15 - AllowedManeuvers_maneuverLeftTurnOnRedAllowed))
               | (Maneuvers_ptr->bit.AllowedManeuvers_maneuverRightTurnOnRedAllowed << (15 - AllowedManeuvers_maneuverRightTurnOnRedAllowed))
               | (Maneuvers_ptr->bit.AllowedManeuvers_maneuverLaneChangeAllowed     << (15 - AllowedManeuvers_maneuverLaneChangeAllowed))
               | (Maneuvers_ptr->bit.AllowedManeuvers_maneuverNoStoppingAllowed     << (15 - AllowedManeuvers_maneuverNoStoppingAllowed))
               | (Maneuvers_ptr->bit.AllowedManeuvers_yieldAllwaysRequired          << (15 - AllowedManeuvers_yieldAllwaysRequired))
               | (Maneuvers_ptr->bit.AllowedManeuvers_goWithHalt                    << (15 - AllowedManeuvers_goWithHalt))
               | (Maneuvers_ptr->bit.AllowedManeuvers_caution                       << (15 - AllowedManeuvers_caution));
        
    maneuvers_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    maneuvers_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_AllowedManeuvers_free(maneuvers_ptr);
    return result;
}


/* Parse routine for DE_AllowedManeuvers. */
int DE_AllowedManeuvers_parse(AllowedManeuvers_t *maneuvers_ptr, DE_AllowedManeuvers_un_ptr Maneuvers_ptr)
{
    int result = 0;
    
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((maneuvers_ptr == NULL) || (Maneuvers_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Maneuvers_ptr, 0, sizeof(*Maneuvers_ptr));

    /* Parse the buffer. */
    if(maneuvers_ptr->buf == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    event_mask = maneuvers_ptr->buf[0];
    event_mask = (event_mask << 8) | maneuvers_ptr->buf[1]; 

    /* Decode event data to alert mask. */
    Maneuvers_ptr->bit.AllowedManeuvers_maneuverStraightAllowed       = (event_mask >> (15 - AllowedManeuvers_maneuverStraightAllowed)) & 0x01;        
    Maneuvers_ptr->bit.AllowedManeuvers_maneuverLeftAllowed           = (event_mask >> (15 - AllowedManeuvers_maneuverLeftAllowed)) & 0x01;       
    Maneuvers_ptr->bit.AllowedManeuvers_maneuverRightAllowed          = (event_mask >> (15 - AllowedManeuvers_maneuverRightAllowed)) & 0x01;        
    Maneuvers_ptr->bit.AllowedManeuvers_maneuverUTurnAllowed          = (event_mask >> (15 - AllowedManeuvers_maneuverUTurnAllowed)) & 0x01;        
    Maneuvers_ptr->bit.AllowedManeuvers_maneuverLeftTurnOnRedAllowed  = (event_mask >> (15 - AllowedManeuvers_maneuverLeftTurnOnRedAllowed)) & 0x01;        
    Maneuvers_ptr->bit.AllowedManeuvers_maneuverRightTurnOnRedAllowed = (event_mask >> (15 - AllowedManeuvers_maneuverRightTurnOnRedAllowed)) & 0x01;        
    Maneuvers_ptr->bit.AllowedManeuvers_maneuverLaneChangeAllowed     = (event_mask >> (15 - AllowedManeuvers_maneuverLaneChangeAllowed)) & 0x01;         
    Maneuvers_ptr->bit.AllowedManeuvers_maneuverNoStoppingAllowed     = (event_mask >> (15 - AllowedManeuvers_maneuverNoStoppingAllowed)) & 0x01;    
    Maneuvers_ptr->bit.AllowedManeuvers_yieldAllwaysRequired          = (event_mask >> (15 - AllowedManeuvers_yieldAllwaysRequired)) & 0x01;  
    Maneuvers_ptr->bit.AllowedManeuvers_goWithHalt                    = (event_mask >> (15 - AllowedManeuvers_goWithHalt)) & 0x01; 
    Maneuvers_ptr->bit.AllowedManeuvers_caution                       = (event_mask >> (15 - AllowedManeuvers_caution)) & 0x01; 

    return result;

ERR_EXIT:
    
    return result;
}


/* Free routine for DE_BrakeAppliedStatus. */
int DE_BrakeAppliedStatus_free(BrakeAppliedStatus_t *wheelBrakes_ptr)
{
    int result = 0;

    if(wheelBrakes_ptr != NULL)
    {
        if(wheelBrakes_ptr->buf != NULL)
        {
            FREEMEM(wheelBrakes_ptr->buf);
        }

        memset(wheelBrakes_ptr, 0x00, sizeof(*wheelBrakes_ptr));
    }

    return result;
}


/* Allocate routine for DE_BrakeAppliedStatus. */
int DE_BrakeAppliedStatus_allocate(BrakeAppliedStatus_t *wheelBrakes_ptr, DE_BrakeAppliedStatus_st_ptr WheelBrakes_ptr)
{
    int result = 0;

    if((wheelBrakes_ptr == NULL) || (WheelBrakes_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* reset all the zone */
    memset(wheelBrakes_ptr, 0x00, sizeof(*wheelBrakes_ptr));

    wheelBrakes_ptr->size = 1;
    wheelBrakes_ptr->bits_unused = 3;
    wheelBrakes_ptr->buf = CALLOC(1, wheelBrakes_ptr->size);
    
    if(wheelBrakes_ptr->buf == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    *(wheelBrakes_ptr->buf) = ((WheelBrakes_ptr->brake_status_unavailable << (7 - BrakeAppliedStatus_unavailable)) | \
                              (WheelBrakes_ptr->brake_status_leftFront << (7 - BrakeAppliedStatus_leftFront)) | \
                              (WheelBrakes_ptr->brake_status_leftRear << (7 - BrakeAppliedStatus_leftRear)) | \
                              (WheelBrakes_ptr->brake_status_rightFront << (7 - BrakeAppliedStatus_rightFront)) | \
                              (WheelBrakes_ptr->brake_status_rightRear<< (7 - BrakeAppliedStatus_rightRear)));
    
    return result;
    
ERR_EXIT:

    DE_BrakeAppliedStatus_free(wheelBrakes_ptr);

    return result;
}


/* parse routine for DE_BrakeAppliedStatus. */
int DE_BrakeAppliedStatus_parse(BrakeAppliedStatus_t *wheelBrakes_ptr, DE_BrakeAppliedStatus_st_ptr WheelBrakes_ptr)
{
    uint8_t mask = 0x00;
    int result = 0;
    
    if((wheelBrakes_ptr == NULL) || (WheelBrakes_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(WheelBrakes_ptr, 0, sizeof(*WheelBrakes_ptr));

    /* Parse the buffer. */
    if(wheelBrakes_ptr->buf == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    mask = wheelBrakes_ptr->buf[0];
    WheelBrakes_ptr->brake_status_unavailable = !!(mask & 0x80 );
    WheelBrakes_ptr->brake_status_leftFront = !!(mask & 0x40);
    WheelBrakes_ptr->brake_status_leftRear = !!(mask & 0x20);
    WheelBrakes_ptr->brake_status_rightFront = !!(mask & 0x10);
    WheelBrakes_ptr->brake_status_rightRear = !!(mask & 0x08);

ERR_EXIT:
    
    return result;
}


/* free routine of DE_DescriptiveName */
int DE_DescriptiveName_free(DescriptiveName_t *name_ptr)
{
    int    result = 0;
    
    
    if(name_ptr != NULL)
    {
        if(name_ptr->buf != NULL)
        {
            FREEMEM(name_ptr->buf);
        }
        
        /* Must clear all the zone and avoid repeat free operation*/
        memset(name_ptr, 0x00, sizeof(*name_ptr));    
        
        result = ERR_OK;
    }
    else
    {
        result = -ERR_INVAL;
    }

    return result;
}


/* allocate routine for DE_DescriptiveName */
int DE_DescriptiveName_allocate(DescriptiveName_t *name_ptr, DE_DescriptiveName_st_ptr Name_ptr)
{
    int    result = 0;

    
    /* Erroe detection. */
    if((name_ptr == NULL) || (Name_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /*Reset all the zone. */
    memset(name_ptr, 0x00, sizeof(*name_ptr));

    /* Set descriptive name. */
    name_ptr->size = Name_ptr->DescriptiveName_size;
    if((name_ptr->buf = CALLOC(1, name_ptr->size)) != NULL)
    {
        memcpy(name_ptr->buf, Name_ptr->DescriptiveName_data, Name_ptr->DescriptiveName_size);
    }
    else
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    return result;
    
ERR_EXIT:
    
    DE_DescriptiveName_free(name_ptr);
    return result;
}


/* parse routine of DE_DescriptiveName */
int DE_DescriptiveName_parse(DescriptiveName_t *name_ptr, DE_DescriptiveName_st_ptr Name_ptr)
{
    int result = 0;


    /* Error detectin. */
    if((name_ptr == NULL) || (Name_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Name_ptr, 0, sizeof(*Name_ptr));

    /* Copy id data. */
    Name_ptr->DescriptiveName_size = name_ptr->size;
    memcpy(Name_ptr->DescriptiveName_data, name_ptr->buf, name_ptr->size);

ERR_EXIT:

    return result;
}


/* Free routine for DE_ExteriorLights. */
int DE_ExteriorLights_free(ExteriorLights_t *lights_ptr)
{
    int result = 0;
    
    if(lights_ptr != NULL)
    {
        if(lights_ptr->buf != NULL)
        {
            FREEMEM(lights_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(lights_ptr, 0, sizeof(*lights_ptr));
        
    }

   return result;
}


/* Allocate routine for DE_ExteriorLights. */
int DE_ExteriorLights_allocate(ExteriorLights_t *lights_ptr, DE_ExteriorLights_un_ptr Lights_ptr)
{
    int result = 0;

    uint16_t    event_mask = 0;


    /* Error detect. */
    if((lights_ptr == NULL) || (Lights_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(lights_ptr, 0, sizeof(*lights_ptr));

    /* Allocate bit data zone. */
    lights_ptr->size = 2;
    lights_ptr->bits_unused = 7;
    if((lights_ptr->buf = CALLOC(1, lights_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Lights_ptr->bit.ExteriorLights_lowBeamHeadlightsOn     << (15 - ExteriorLights_lowBeamHeadlightsOn))
               | (Lights_ptr->bit.ExteriorLights_highBeamHeadlightsOn    << (15 - ExteriorLights_highBeamHeadlightsOn))
               | (Lights_ptr->bit.ExteriorLights_leftTurnSignalOn        << (15 - ExteriorLights_leftTurnSignalOn))
               | (Lights_ptr->bit.ExteriorLights_rightTurnSignalOn       << (15 - ExteriorLights_rightTurnSignalOn))
               | (Lights_ptr->bit.ExteriorLights_hazardSignalOn          << (15 - ExteriorLights_hazardSignalOn))
               | (Lights_ptr->bit.ExteriorLights_automaticLightControlOn << (15 - ExteriorLights_automaticLightControlOn))
               | (Lights_ptr->bit.ExteriorLights_daytimeRunningLightsOn  << (15 - ExteriorLights_daytimeRunningLightsOn))
               | (Lights_ptr->bit.ExteriorLights_fogLightOn              << (15 - ExteriorLights_fogLightOn))
               | (Lights_ptr->bit.ExteriorLights_parkingLightsOn         << (15 - ExteriorLights_parkingLightsOn));
        
    lights_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    lights_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_ExteriorLights_free(lights_ptr);
    return result;
}


/* Parse routine for DE_ExteriorLights. */
int DE_ExteriorLights_parse(ExteriorLights_t *lights_ptr, DE_ExteriorLights_un_ptr Lights_ptr)
{
    int result = 0;
    
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((lights_ptr == NULL) || (Lights_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Lights_ptr, 0, sizeof(*Lights_ptr));

    /* Parse the buffer. */
    if(lights_ptr->buf == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    event_mask = lights_ptr->buf[0];
    event_mask = (event_mask << 8) | lights_ptr->buf[1]; 

    /* Decode event data to alert mask. */
    Lights_ptr->bit.ExteriorLights_lowBeamHeadlightsOn     = (event_mask >> (15 - ExteriorLights_lowBeamHeadlightsOn)) & 0x01;        
    Lights_ptr->bit.ExteriorLights_highBeamHeadlightsOn    = (event_mask >> (15 - ExteriorLights_highBeamHeadlightsOn)) & 0x01;       
    Lights_ptr->bit.ExteriorLights_leftTurnSignalOn        = (event_mask >> (15 - ExteriorLights_leftTurnSignalOn)) & 0x01;        
    Lights_ptr->bit.ExteriorLights_rightTurnSignalOn       = (event_mask >> (15 - ExteriorLights_rightTurnSignalOn)) & 0x01;        
    Lights_ptr->bit.ExteriorLights_hazardSignalOn          = (event_mask >> (15 - ExteriorLights_hazardSignalOn)) & 0x01;        
    Lights_ptr->bit.ExteriorLights_automaticLightControlOn = (event_mask >> (15 - ExteriorLights_automaticLightControlOn)) & 0x01;        
    Lights_ptr->bit.ExteriorLights_daytimeRunningLightsOn  = (event_mask >> (15 - ExteriorLights_daytimeRunningLightsOn)) & 0x01;         
    Lights_ptr->bit.ExteriorLights_fogLightOn              = (event_mask >> (15 - ExteriorLights_fogLightOn)) & 0x01;
    
    Lights_ptr->bit.ExteriorLights_parkingLightsOn         = (event_mask >> (15 - ExteriorLights_parkingLightsOn)) & 0x01;         

    return result;

ERR_EXIT:
    
    return result;
}

#if 0
/* Free routine for DE_FurtherInfoID. */
int DE_FurtherInfoID_free(FurtherInfoID_t *id_ptr)
{
    if(id_ptr != NULL)
    {
        if(id_ptr->buf != NULL)
        {
            FREEMEM(id_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(id_ptr, 0, sizeof(*id_ptr));

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DE_FurtherInfoID. */
int DE_FurtherInfoID_allocate(FurtherInfoID_t *id_ptr, DE_FurtherInfoID_st_ptr ID_ptr)
{
    int        result  =0;
    /* Error detect. */
    if((id_ptr == NULL) || (ID_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(id_ptr, 0, sizeof(*id_ptr));

    /* TemporaryID. */
    id_ptr->size = DE_FurtherInfoID_BUFSIZE;
    if((id_ptr->buf = CALLOC(1, id_ptr->size)) != NULL)
    {
        memcpy(id_ptr->buf, &((*ID_ptr)[0]), id_ptr->size);                  
    }
    else
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    return result;

ERR_EXIT:

    DE_FurtherInfoID_free(id_ptr);
    return result;
}


/* Parse routine for DE_FurtherInfoID. */
int DE_FurtherInfoID_parse(FurtherInfoID_t *id_ptr, DE_FurtherInfoID_st_ptr ID_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((id_ptr == NULL) || (ID_ptr == NULL))
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(&(*ID_ptr[0]), 0, DE_FurtherInfoID_st_len);

    /* Copy id data. */
    memcpy(&(*ID_ptr[0]), id_ptr->buf, DE_FurtherInfoID_st_len);

ERR_EXIT:
    
    return result;
}
#endif

/* Free routine for DE_GNSSstatus. */
int DE_GNSSstatus_free(GNSSstatus_t *status_ptr)
{
    int result = 0;

    
    if(status_ptr != NULL)
    {
        if(status_ptr->buf != NULL)
        {
            FREEMEM(status_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(status_ptr, 0, sizeof(*status_ptr));

    }

    return result;
}


/* Allocate routine for DE_GNSSstatus. */
int DE_GNSSstatus_allocate(GNSSstatus_t *status_ptr, DE_GNSSstatus_st_ptr Status_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((status_ptr == NULL) || (Status_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(status_ptr, 0, sizeof(*status_ptr));

    /* Set the buffer. */
    status_ptr->size = 1;
    status_ptr->bits_unused = 0;
    if((status_ptr->buf = CALLOC(1, status_ptr->size)) != NULL)
    {
        * status_ptr->buf = (Status_ptr->GNSSstat_unavailable << (7 - GNSSstatus_unavailable))
                          | (Status_ptr->GNSSstat_isHealthy << (7 - GNSSstatus_isHealthy))
                          | (Status_ptr->GNSSstat_isMonitored << (7 - GNSSstatus_isMonitored))
                          | (Status_ptr->GNSSstat_baseStationType << (7 - GNSSstatus_baseStationType))
                          | (Status_ptr->GNSSstat_aPDOPofUnder5 << (7 - GNSSstatus_aPDOPofUnder5))
                          | (Status_ptr->GNSSstat_inViewOfUnder5 << (7 - GNSSstatus_inViewOfUnder5))
                          | (Status_ptr->GNSSstat_localCorrectionPresend << (7 - GNSSstatus_localCorrectionsPresent))
                          | (Status_ptr->GNSSstat_networkCorrectionPresend << (7 - GNSSstatus_networkCorrectionsPresent));            
    }
    else
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    return result;

ERR_EXIT:

    DE_GNSSstatus_free(status_ptr);
    return result;
}


/* Parse routine for DE_GNSSstatus. */
int DE_GNSSstatus_parse(GNSSstatus_t *status_ptr, DE_GNSSstatus_st_ptr Status_ptr)
{
    int        result  = 0;
    /* Error detect. */
    if((status_ptr == NULL) || (Status_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Status_ptr, 0, sizeof(*Status_ptr));

    /* Parse the buffer. */
    Status_ptr->GNSSstat_unavailable = ((* status_ptr->buf) >> (7 - GNSSstatus_unavailable)) & 0x01;
    Status_ptr->GNSSstat_isHealthy = ((* status_ptr->buf) >> (7 - GNSSstatus_isHealthy)) & 0x01;
    Status_ptr->GNSSstat_isMonitored = ((* status_ptr->buf) >> (7 - GNSSstatus_isMonitored)) & 0x01;
    Status_ptr->GNSSstat_baseStationType = ((* status_ptr->buf) >> (7 - GNSSstatus_baseStationType)) & 0x01;
    Status_ptr->GNSSstat_aPDOPofUnder5 = ((* status_ptr->buf) >> (7 - GNSSstatus_aPDOPofUnder5)) & 0x01;
    Status_ptr->GNSSstat_inViewOfUnder5 = ((* status_ptr->buf) >> (7 - GNSSstatus_inViewOfUnder5)) & 0x01;
    Status_ptr->GNSSstat_localCorrectionPresend = ((* status_ptr->buf) >> (7 - GNSSstatus_localCorrectionsPresent)) & 0x01;
    Status_ptr->GNSSstat_networkCorrectionPresend = ((* status_ptr->buf) >> (7 - GNSSstatus_networkCorrectionsPresent)) & 0x01;
    
    return result;

ERR_EXIT:
    
    return result;
}


/* Free routine for DE_IntersectionStatusObject. */
int DE_IntersectionStatusObject_free(IntersectionStatusObject_t *status_ptr)
{
    int result = 0;


    if(status_ptr != NULL)
    {
        if(status_ptr->buf != NULL)
        {
            FREEMEM(status_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(status_ptr, 0, sizeof(*status_ptr));  
    }

    return result;
}


/* Allocate routine for DE_IntersectionStatusObject. */
int DE_IntersectionStatusObject_allocate(IntersectionStatusObject_t *status_ptr, DE_IntersectionStatusObject_un_ptr Status_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((status_ptr == NULL) || (Status_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(status_ptr, 0, sizeof(*status_ptr));

    /* Allocate bit data zone. */
    status_ptr->size = 2;
    status_ptr->bits_unused = 0;
    if((status_ptr->buf = CALLOC(1, status_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Status_ptr->bit.InterStatusObj_manualControlIsEnabled         << (15 - IntersectionStatusObject_manualControlIsEnabled))
               | (Status_ptr->bit.InterStatusObj_stopTimeIsActivated            << (15 - IntersectionStatusObject_stopTimeIsActivated))
               | (Status_ptr->bit.InterStatusObj_failureFlash                   << (15 - IntersectionStatusObject_failureFlash))
               | (Status_ptr->bit.InterStatusObj_preemptIsActive                << (15 - IntersectionStatusObject_preemptIsActive))
               | (Status_ptr->bit.InterStatusObj_signalPriorityIsActive         << (15 - IntersectionStatusObject_signalPriorityIsActive))
               | (Status_ptr->bit.InterStatusObj_fixedTimeOperation             << (15 - IntersectionStatusObject_fixedTimeOperation))
               | (Status_ptr->bit.InterStatusObj_trafficDependentOperation      << (15 - IntersectionStatusObject_trafficDependentOperation))
               | (Status_ptr->bit.InterStatusObj_standbyOperation               << (15 - IntersectionStatusObject_standbyOperation))
               | (Status_ptr->bit.InterStatusObj_failureMode                    << (15 - IntersectionStatusObject_failureMode))
               | (Status_ptr->bit.InterStatusObj_off                            << (15 - IntersectionStatusObject_off))
               | (Status_ptr->bit.InterStatusObj_recentMAPmessageUpdate                  << (15 - IntersectionStatusObject_recentMAPmessageUpdate))
               | (Status_ptr->bit.InterStatusObj_recentChangeInMAPassignedLanesIDsUsed   << (15 - IntersectionStatusObject_recentChangeInMAPassignedLanesIDsUsed))
               | (Status_ptr->bit.InterStatusObj_noValidMAPisAvailableAtThisTime         << (15 - IntersectionStatusObject_noValidMAPisAvailableAtThisTime))
               | (Status_ptr->bit.InterStatusObj_noValidSPATisAvailableAtThisTime        << (15 - IntersectionStatusObject_noValidSPATisAvailableAtThisTime));
        
    status_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    status_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_IntersectionStatusObject_free(status_ptr);
    return result;
}


/* Parse routine for DE_IntersectionStatusObject. */
int DE_IntersectionStatusObject_parse(IntersectionStatusObject_t *status_ptr, DE_IntersectionStatusObject_un_ptr Status_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((status_ptr == NULL) || (Status_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Status_ptr, 0, sizeof(*Status_ptr));

    /* Parse the buffer. */
    if(status_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = status_ptr->buf[0];
    event_mask = (event_mask << 8) | status_ptr->buf[1]; 

    /* Decode event data to alert mask. */
    Status_ptr->bit.InterStatusObj_manualControlIsEnabled           = (event_mask >> (15 - IntersectionStatusObject_manualControlIsEnabled)) & 0x01;        
    Status_ptr->bit.InterStatusObj_stopTimeIsActivated              = (event_mask >> (15 - IntersectionStatusObject_stopTimeIsActivated)) & 0x01;       
    Status_ptr->bit.InterStatusObj_failureFlash                     = (event_mask >> (15 - IntersectionStatusObject_failureFlash)) & 0x01;        
    Status_ptr->bit.InterStatusObj_preemptIsActive                  = (event_mask >> (15 - IntersectionStatusObject_preemptIsActive)) & 0x01;       
    Status_ptr->bit.InterStatusObj_signalPriorityIsActive           = (event_mask >> (15 - IntersectionStatusObject_signalPriorityIsActive)) & 0x01;        
  
    Status_ptr->bit.InterStatusObj_fixedTimeOperation               = (event_mask >> (15 - IntersectionStatusObject_fixedTimeOperation)) & 0x01;        
    Status_ptr->bit.InterStatusObj_trafficDependentOperation        = (event_mask >> (15 - IntersectionStatusObject_trafficDependentOperation)) & 0x01;        
    Status_ptr->bit.InterStatusObj_standbyOperation                 = (event_mask >> (15 - IntersectionStatusObject_standbyOperation)) & 0x01;        
    Status_ptr->bit.InterStatusObj_failureMode                      = (event_mask >> (15 - IntersectionStatusObject_failureMode)) & 0x01;        
    Status_ptr->bit.InterStatusObj_off                              = (event_mask >> (15 - IntersectionStatusObject_off)) & 0x01;        

    Status_ptr->bit.InterStatusObj_recentMAPmessageUpdate                  = (event_mask >> (15 - IntersectionStatusObject_recentMAPmessageUpdate)) & 0x01;    
    Status_ptr->bit.InterStatusObj_recentChangeInMAPassignedLanesIDsUsed   = (event_mask >> (15 - IntersectionStatusObject_recentChangeInMAPassignedLanesIDsUsed)) & 0x01;    
    Status_ptr->bit.InterStatusObj_noValidMAPisAvailableAtThisTime         = (event_mask >> (15 - IntersectionStatusObject_noValidMAPisAvailableAtThisTime)) & 0x01; 
    Status_ptr->bit.InterStatusObj_noValidSPATisAvailableAtThisTime        = (event_mask >> (15 - IntersectionStatusObject_noValidSPATisAvailableAtThisTime)) & 0x01; 

ERR_EXIT:
    
    return result;
}

/* Free routine for DE_LaneAttributes_Vehicle. */
int DE_LaneAttributes_Vehicle_free(LaneAttributes_Vehicle_t *vehicle_ptr)
{
    int result = 0;

    if(vehicle_ptr != NULL)
    {
        if(vehicle_ptr->buf != NULL)
        {
            FREEMEM(vehicle_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(vehicle_ptr, 0, sizeof(*vehicle_ptr));  
    }

    return result;
}


/* Allocate routine for DE_LaneAttributes_Vehicle. */
int DE_LaneAttributes_Vehicle_allocate(LaneAttributes_Vehicle_t *vehicle_ptr, DE_LaneAttributes_Vehicle_un_ptr Vehicle_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((vehicle_ptr == NULL) || (Vehicle_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(vehicle_ptr, 0, sizeof(*vehicle_ptr));

    /* Allocate bit data zone. */
    vehicle_ptr->size = 2;
    vehicle_ptr->bits_unused = 8;
    if((vehicle_ptr->buf = CALLOC(1, vehicle_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Vehicle_ptr->bit.LaneAttributes_Vehicle_isVehicleRevocableLane   << (15 - LaneAttributes_Vehicle_isVehicleRevocableLane))
               | (Vehicle_ptr->bit.LaneAttributes_Vehicle_isVehicleFlyOverLane     << (15 - LaneAttributes_Vehicle_isVehicleFlyOverLane))
               | (Vehicle_ptr->bit.LaneAttributes_Vehicle_hovLaneUseOnly           << (15 - LaneAttributes_Vehicle_hovLaneUseOnly))
               | (Vehicle_ptr->bit.LaneAttributes_Vehicle_restrictedToBusUse       << (15 - LaneAttributes_Vehicle_restrictedToBusUse))
               | (Vehicle_ptr->bit.LaneAttributes_Vehicle_restrictedToTaxiUse      << (15 - LaneAttributes_Vehicle_restrictedToTaxiUse))
               | (Vehicle_ptr->bit.LaneAttributes_Vehicle_restrictedFromPublicUse  << (15 - LaneAttributes_Vehicle_restrictedFromPublicUse))
               | (Vehicle_ptr->bit.LaneAttributes_Vehicle_hasIRbeaconCoverage      << (15 - LaneAttributes_Vehicle_hasIRbeaconCoverage))
               | (Vehicle_ptr->bit.LaneAttributes_Vehicle_permissionOnRequest      << (15 - LaneAttributes_Vehicle_permissionOnRequest));
               
    vehicle_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    vehicle_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_LaneAttributes_Vehicle_free(vehicle_ptr);
    return result;
}


/* Parse routine for DE_LaneAttributes_Vehicle. */
int DE_LaneAttributes_Vehicle_parse(LaneAttributes_Vehicle_t *vehicle_ptr, DE_LaneAttributes_Vehicle_un_ptr Vehicle_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((vehicle_ptr == NULL) || (Vehicle_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Vehicle_ptr, 0, sizeof(*Vehicle_ptr));

    /* Parse the buffer. */
    if(vehicle_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = vehicle_ptr->buf[0];
    event_mask = (event_mask << 8) | vehicle_ptr->buf[1]; 

    /* Decode event data to alert mask. */
    Vehicle_ptr->bit.LaneAttributes_Vehicle_isVehicleRevocableLane     = (event_mask >> (15 - LaneAttributes_Vehicle_isVehicleRevocableLane)) & 0x01;        
    Vehicle_ptr->bit.LaneAttributes_Vehicle_isVehicleFlyOverLane       = (event_mask >> (15 - LaneAttributes_Vehicle_isVehicleFlyOverLane)) & 0x01;       
    Vehicle_ptr->bit.LaneAttributes_Vehicle_hovLaneUseOnly             = (event_mask >> (15 - LaneAttributes_Vehicle_hovLaneUseOnly)) & 0x01;        
    Vehicle_ptr->bit.LaneAttributes_Vehicle_restrictedToBusUse         = (event_mask >> (15 - LaneAttributes_Vehicle_restrictedToBusUse)) & 0x01;       
    Vehicle_ptr->bit.LaneAttributes_Vehicle_restrictedToTaxiUse        = (event_mask >> (15 - LaneAttributes_Vehicle_restrictedToTaxiUse)) & 0x01;         
    Vehicle_ptr->bit.LaneAttributes_Vehicle_restrictedFromPublicUse    = (event_mask >> (15 - LaneAttributes_Vehicle_restrictedFromPublicUse)) & 0x01;        
    Vehicle_ptr->bit.LaneAttributes_Vehicle_hasIRbeaconCoverage        = (event_mask >> (15 - LaneAttributes_Vehicle_hasIRbeaconCoverage)) & 0x01;        
    Vehicle_ptr->bit.LaneAttributes_Vehicle_permissionOnRequest        = (event_mask >> (15 - LaneAttributes_Vehicle_permissionOnRequest)) & 0x01;        
    

ERR_EXIT:
    
    return result;
}

/* Free routine for DE_LaneAttributes_Crosswalk. */
int DE_LaneAttributes_Crosswalk_free(LaneAttributes_Crosswalk_t *crosswalk_ptr)
{
    int result = 0;

    if(crosswalk_ptr != NULL)
    {
        if(crosswalk_ptr->buf != NULL)
        {
            FREEMEM(crosswalk_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(crosswalk_ptr, 0, sizeof(*crosswalk_ptr));  
    }

    return result;
}


/* Allocate routine for DE_LaneAttributes_Crosswalk. */
int DE_LaneAttributes_Crosswalk_allocate(LaneAttributes_Crosswalk_t *crosswalk_ptr, DE_LaneAttributes_Crosswalk_un_ptr Crosswalk_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((crosswalk_ptr == NULL) || (Crosswalk_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(crosswalk_ptr, 0, sizeof(*crosswalk_ptr));

    /* Allocate bit data zone. */
    crosswalk_ptr->size = 2;
    crosswalk_ptr->bits_unused = 7;
    if((crosswalk_ptr->buf = CALLOC(1, crosswalk_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Crosswalk_ptr->bit.LaneAttributes_Crosswalk_crosswalkRevocableLane      << (15 - LaneAttributes_Crosswalk_crosswalkRevocableLane))
               | (Crosswalk_ptr->bit.LaneAttributes_Crosswalk_bicyleUseAllowed            << (15 - LaneAttributes_Crosswalk_bicyleUseAllowed))
               | (Crosswalk_ptr->bit.LaneAttributes_Crosswalk_isXwalkFlyOverLane          << (15 - LaneAttributes_Crosswalk_isXwalkFlyOverLane))
               | (Crosswalk_ptr->bit.LaneAttributes_Crosswalk_fixedCycleTime              << (15 - LaneAttributes_Crosswalk_fixedCycleTime))
               | (Crosswalk_ptr->bit.LaneAttributes_Crosswalk_biDirectionalCycleTimes     << (15 - LaneAttributes_Crosswalk_biDirectionalCycleTimes))
               | (Crosswalk_ptr->bit.LaneAttributes_Crosswalk_hasPushToWalkButton         << (15 - LaneAttributes_Crosswalk_hasPushToWalkButton))
               | (Crosswalk_ptr->bit.LaneAttributes_Crosswalk_audioSupport                << (15 - LaneAttributes_Crosswalk_audioSupport))
               | (Crosswalk_ptr->bit.LaneAttributes_Crosswalk_rfSignalRequestPresent      << (15 - LaneAttributes_Crosswalk_rfSignalRequestPresent))
               | (Crosswalk_ptr->bit.LaneAttributes_Crosswalk_unsignalizedSegmentsPresent << (15 - LaneAttributes_Crosswalk_unsignalizedSegmentsPresent));
               
    crosswalk_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    crosswalk_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_LaneAttributes_Crosswalk_free(crosswalk_ptr);
    return result;
}


/* Parse routine for DE_LaneAttributes_Crosswalk. */
int DE_LaneAttributes_Crosswalk_parse(LaneAttributes_Crosswalk_t *crosswalk_ptr, DE_LaneAttributes_Crosswalk_un_ptr Crosswalk_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((crosswalk_ptr == NULL) || (Crosswalk_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Crosswalk_ptr, 0, sizeof(*Crosswalk_ptr));

    /* Parse the buffer. */
    if(crosswalk_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = crosswalk_ptr->buf[0];
    event_mask = (event_mask << 8) | crosswalk_ptr->buf[1]; 

    /* Decode event data to alert mask. */
    Crosswalk_ptr->bit.LaneAttributes_Crosswalk_crosswalkRevocableLane        = (event_mask >> (15 - LaneAttributes_Crosswalk_crosswalkRevocableLane)) & 0x01;        
    Crosswalk_ptr->bit.LaneAttributes_Crosswalk_bicyleUseAllowed              = (event_mask >> (15 - LaneAttributes_Crosswalk_bicyleUseAllowed)) & 0x01;       
    Crosswalk_ptr->bit.LaneAttributes_Crosswalk_isXwalkFlyOverLane            = (event_mask >> (15 - LaneAttributes_Crosswalk_isXwalkFlyOverLane)) & 0x01;        
    Crosswalk_ptr->bit.LaneAttributes_Crosswalk_fixedCycleTime                = (event_mask >> (15 - LaneAttributes_Crosswalk_fixedCycleTime)) & 0x01;       
    Crosswalk_ptr->bit.LaneAttributes_Crosswalk_biDirectionalCycleTimes       = (event_mask >> (15 - LaneAttributes_Crosswalk_biDirectionalCycleTimes)) & 0x01;         
    Crosswalk_ptr->bit.LaneAttributes_Crosswalk_hasPushToWalkButton           = (event_mask >> (15 - LaneAttributes_Crosswalk_hasPushToWalkButton)) & 0x01;        
    Crosswalk_ptr->bit.LaneAttributes_Crosswalk_audioSupport                  = (event_mask >> (15 - LaneAttributes_Crosswalk_audioSupport)) & 0x01;        
    Crosswalk_ptr->bit.LaneAttributes_Crosswalk_rfSignalRequestPresent        = (event_mask >> (15 - LaneAttributes_Crosswalk_rfSignalRequestPresent)) & 0x01;   
    Crosswalk_ptr->bit.LaneAttributes_Crosswalk_unsignalizedSegmentsPresent   = (event_mask >> (15 - LaneAttributes_Crosswalk_unsignalizedSegmentsPresent)) & 0x01;        
    

ERR_EXIT:
    
    return result;
}

/* Free routine for DE_LaneAttributes_Bike. */
int DE_LaneAttributes_Bike_free(LaneAttributes_Bike_t *bike_ptr)
{
    int result = 0;

    if(bike_ptr != NULL)
    {
        if(bike_ptr->buf != NULL)
        {
            FREEMEM(bike_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(bike_ptr, 0, sizeof(*bike_ptr));  
    }

    return result;
}


/* Allocate routine for DE_LaneAttributes_Bike. */
int DE_LaneAttributes_Bike_allocate(LaneAttributes_Bike_t *bike_ptr, DE_LaneAttributes_Bike_un_ptr Bike_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((bike_ptr == NULL) || (Bike_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(bike_ptr, 0, sizeof(*bike_ptr));

    /* Allocate bit data zone. */
    bike_ptr->size = 2;
    bike_ptr->bits_unused = 9;
    if((bike_ptr->buf = CALLOC(1, bike_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Bike_ptr->bit.LaneAttributes_Bike_bikeRevocableLane           << (15 - LaneAttributes_Bike_bikeRevocableLane))
               | (Bike_ptr->bit.LaneAttributes_Bike_pedestrianUseAllowed        << (15 - LaneAttributes_Bike_pedestrianUseAllowed))
               | (Bike_ptr->bit.LaneAttributes_Bike_isBikeFlyOverLane           << (15 - LaneAttributes_Bike_isBikeFlyOverLane))
               | (Bike_ptr->bit.LaneAttributes_Bike_fixedCycleTime              << (15 - LaneAttributes_Bike_fixedCycleTime))
               | (Bike_ptr->bit.LaneAttributes_Bike_biDirectionalCycleTimes     << (15 - LaneAttributes_Bike_biDirectionalCycleTimes))
               | (Bike_ptr->bit.LaneAttributes_Bike_isolatedByBarrier           << (15 - LaneAttributes_Bike_isolatedByBarrier))
               | (Bike_ptr->bit.LaneAttributes_Bike_unsignalizedSegmentsPresent << (15 - LaneAttributes_Bike_unsignalizedSegmentsPresent));
               
    bike_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    bike_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_LaneAttributes_Bike_free(bike_ptr);
    return result;
}


/* Parse routine for DE_LaneAttributes_Bike. */
int DE_LaneAttributes_Bike_parse(LaneAttributes_Bike_t *bike_ptr, DE_LaneAttributes_Bike_un_ptr Bike_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((bike_ptr == NULL) || (Bike_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Bike_ptr, 0, sizeof(*Bike_ptr));

    /* Parse the buffer. */
    if(bike_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = bike_ptr->buf[0];
    event_mask = (event_mask << 8) | bike_ptr->buf[1]; 

    /* Decode event data to alert mask. */
    Bike_ptr->bit.LaneAttributes_Bike_bikeRevocableLane           = (event_mask >> (15 - LaneAttributes_Bike_bikeRevocableLane)) & 0x01;        
    Bike_ptr->bit.LaneAttributes_Bike_pedestrianUseAllowed        = (event_mask >> (15 - LaneAttributes_Bike_pedestrianUseAllowed)) & 0x01;       
    Bike_ptr->bit.LaneAttributes_Bike_isBikeFlyOverLane           = (event_mask >> (15 - LaneAttributes_Bike_isBikeFlyOverLane)) & 0x01;        
    Bike_ptr->bit.LaneAttributes_Bike_fixedCycleTime              = (event_mask >> (15 - LaneAttributes_Bike_fixedCycleTime)) & 0x01;       
    Bike_ptr->bit.LaneAttributes_Bike_biDirectionalCycleTimes     = (event_mask >> (15 - LaneAttributes_Bike_biDirectionalCycleTimes)) & 0x01;         
    Bike_ptr->bit.LaneAttributes_Bike_isolatedByBarrier           = (event_mask >> (15 - LaneAttributes_Bike_isolatedByBarrier)) & 0x01;        
    Bike_ptr->bit.LaneAttributes_Bike_unsignalizedSegmentsPresent = (event_mask >> (15 - LaneAttributes_Bike_unsignalizedSegmentsPresent)) & 0x01;
    

ERR_EXIT:
    
    return result;
}

/* Free routine for DE_LaneAttributes_Sidewalk. */
int DE_LaneAttributes_Sidewalk_free(LaneAttributes_Sidewalk_t *sidewalk_ptr)
{
    int result = 0;

    if(sidewalk_ptr != NULL)
    {
        if(sidewalk_ptr->buf != NULL)
        {
            FREEMEM(sidewalk_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(sidewalk_ptr, 0, sizeof(*sidewalk_ptr));  
    }

    return result;
}

/* Allocate routine for DE_LaneAttributes_Sidewalk. */
int DE_LaneAttributes_Sidewalk_allocate(LaneAttributes_Sidewalk_t *sidewalk_ptr, DE_LaneAttributes_Sidewalk_un_ptr Sidewalk_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((sidewalk_ptr == NULL) || (Sidewalk_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(sidewalk_ptr, 0, sizeof(*sidewalk_ptr));

    /* Allocate bit data zone. */
    sidewalk_ptr->size = 2;
    sidewalk_ptr->bits_unused = 12;
    if((sidewalk_ptr->buf = CALLOC(1, sidewalk_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Sidewalk_ptr->bit.LaneAttributes_Sidewalk_sidewalk_RevocableLane  << (15 - LaneAttributes_Sidewalk_sidewalk_RevocableLane))
               | (Sidewalk_ptr->bit.LaneAttributes_Sidewalk_bicyleUseAllowed        << (15 - LaneAttributes_Sidewalk_bicyleUseAllowed))
               | (Sidewalk_ptr->bit.LaneAttributes_Sidewalk_isSidewalkFlyOverLane   << (15 - LaneAttributes_Sidewalk_isSidewalkFlyOverLane))
               | (Sidewalk_ptr->bit.LaneAttributes_Sidewalk_walkBikes               << (15 - LaneAttributes_Sidewalk_walkBikes));
               
    sidewalk_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    sidewalk_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_LaneAttributes_Sidewalk_free(sidewalk_ptr);
    return result;
}


/* Parse routine for DE_LaneAttributes_Sidewalk. */
int DE_LaneAttributes_Sidewalk_parse(LaneAttributes_Sidewalk_t *sidewalk_ptr, DE_LaneAttributes_Sidewalk_un_ptr Sidewalk_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((sidewalk_ptr == NULL) || (Sidewalk_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Sidewalk_ptr, 0, sizeof(*Sidewalk_ptr));

    /* Parse the buffer. */
    if(sidewalk_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = sidewalk_ptr->buf[0];
    event_mask = (event_mask << 8) | sidewalk_ptr->buf[1]; 

    /* Decode event data to mask. */
    Sidewalk_ptr->bit.LaneAttributes_Sidewalk_sidewalk_RevocableLane = (event_mask >> (15 - LaneAttributes_Sidewalk_sidewalk_RevocableLane)) & 0x01;        
    Sidewalk_ptr->bit.LaneAttributes_Sidewalk_bicyleUseAllowed       = (event_mask >> (15 - LaneAttributes_Sidewalk_bicyleUseAllowed)) & 0x01;       
    Sidewalk_ptr->bit.LaneAttributes_Sidewalk_isSidewalkFlyOverLane  = (event_mask >> (15 - LaneAttributes_Sidewalk_isSidewalkFlyOverLane)) & 0x01;        
    Sidewalk_ptr->bit.LaneAttributes_Sidewalk_walkBikes              = (event_mask >> (15 - LaneAttributes_Sidewalk_walkBikes)) & 0x01;
    

ERR_EXIT:
    
    return result;
}

/* Free routine for DE_LaneAttributes_Barrier. */
int DE_LaneAttributes_Barrier_free(LaneAttributes_Barrier_t *barrier_ptr)
{
    int result = 0;

    if(barrier_ptr != NULL)
    {
        if(barrier_ptr->buf != NULL)
        {
            FREEMEM(barrier_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(barrier_ptr, 0, sizeof(*barrier_ptr));  
    }

    return result;
}

/* Allocate routine for DE_LaneAttributes_Barrier. */
int DE_LaneAttributes_Barrier_allocate(LaneAttributes_Barrier_t *barrier_ptr, DE_LaneAttributes_Barrier_un_ptr Barrier_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((barrier_ptr == NULL) || (Barrier_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(barrier_ptr, 0, sizeof(*barrier_ptr));

    /* Allocate bit data zone. */
    barrier_ptr->size = 2;
    barrier_ptr->bits_unused = 6;
    if((barrier_ptr->buf = CALLOC(1, barrier_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Barrier_ptr->bit.LaneAttributes_Barrier_median_RevocableLane  << (15 - LaneAttributes_Barrier_median_RevocableLane))
               | (Barrier_ptr->bit.LaneAttributes_Barrier_median                << (15 - LaneAttributes_Barrier_median))
               | (Barrier_ptr->bit.LaneAttributes_Barrier_whiteLineHashing      << (15 - LaneAttributes_Barrier_whiteLineHashing))
               | (Barrier_ptr->bit.LaneAttributes_Barrier_stripedLines          << (15 - LaneAttributes_Barrier_stripedLines))
               | (Barrier_ptr->bit.LaneAttributes_Barrier_doubleStripedLines    << (15 - LaneAttributes_Barrier_doubleStripedLines))
               | (Barrier_ptr->bit.LaneAttributes_Barrier_trafficCones          << (15 - LaneAttributes_Barrier_trafficCones))
               | (Barrier_ptr->bit.LaneAttributes_Barrier_constructionBarrier   << (15 - LaneAttributes_Barrier_constructionBarrier))
               | (Barrier_ptr->bit.LaneAttributes_Barrier_trafficChannels       << (15 - LaneAttributes_Barrier_trafficChannels))
               | (Barrier_ptr->bit.LaneAttributes_Barrier_lowCurbs              << (15 - LaneAttributes_Barrier_lowCurbs))
               | (Barrier_ptr->bit.LaneAttributes_Barrier_highCurbs             << (15 - LaneAttributes_Barrier_highCurbs));
               
    barrier_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    barrier_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_LaneAttributes_Barrier_free(barrier_ptr);
    return result;
}


/* Parse routine for DE_LaneAttributes_Barrier. */
int DE_LaneAttributes_Barrier_parse(LaneAttributes_Barrier_t *barrier_ptr, DE_LaneAttributes_Barrier_un_ptr Barrier_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((barrier_ptr == NULL) || (Barrier_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Barrier_ptr, 0, sizeof(*Barrier_ptr));

    /* Parse the buffer. */
    if(barrier_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = barrier_ptr->buf[0];
    event_mask = (event_mask << 8) | barrier_ptr->buf[1]; 

    /* Decode event data to mask. */
    Barrier_ptr->bit.LaneAttributes_Barrier_median_RevocableLane = (event_mask >> (15 - LaneAttributes_Barrier_median_RevocableLane)) & 0x01;        
    Barrier_ptr->bit.LaneAttributes_Barrier_median               = (event_mask >> (15 - LaneAttributes_Barrier_median)) & 0x01;       
    Barrier_ptr->bit.LaneAttributes_Barrier_whiteLineHashing     = (event_mask >> (15 - LaneAttributes_Barrier_whiteLineHashing)) & 0x01;        
    Barrier_ptr->bit.LaneAttributes_Barrier_stripedLines         = (event_mask >> (15 - LaneAttributes_Barrier_stripedLines)) & 0x01;
    Barrier_ptr->bit.LaneAttributes_Barrier_doubleStripedLines   = (event_mask >> (15 - LaneAttributes_Barrier_doubleStripedLines)) & 0x01;        
    Barrier_ptr->bit.LaneAttributes_Barrier_trafficCones         = (event_mask >> (15 - LaneAttributes_Barrier_trafficCones)) & 0x01;       
    Barrier_ptr->bit.LaneAttributes_Barrier_constructionBarrier  = (event_mask >> (15 - LaneAttributes_Barrier_constructionBarrier)) & 0x01;        
    Barrier_ptr->bit.LaneAttributes_Barrier_trafficChannels      = (event_mask >> (15 - LaneAttributes_Barrier_trafficChannels)) & 0x01;
    Barrier_ptr->bit.LaneAttributes_Barrier_lowCurbs             = (event_mask >> (15 - LaneAttributes_Barrier_lowCurbs)) & 0x01;        
    Barrier_ptr->bit.LaneAttributes_Barrier_highCurbs            = (event_mask >> (15 - LaneAttributes_Barrier_highCurbs)) & 0x01;
    

ERR_EXIT:
    
    return result;
}

/* Free routine for DE_LaneAttributes_Striping. */
int DE_LaneAttributes_Striping_free(LaneAttributes_Striping_t *striping_ptr)
{
    int result = 0;

    if(striping_ptr != NULL)
    {
        if(striping_ptr->buf != NULL)
        {
            FREEMEM(striping_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(striping_ptr, 0, sizeof(*striping_ptr));  
    }

    return result;
}

/* Allocate routine for DE_LaneAttributes_Striping. */
int DE_LaneAttributes_Striping_allocate(LaneAttributes_Striping_t *striping_ptr, DE_LaneAttributes_Striping_un_ptr Striping_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((striping_ptr == NULL) || (Striping_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(striping_ptr, 0, sizeof(*striping_ptr));

    /* Allocate bit data zone. */
    striping_ptr->size = 2;
    striping_ptr->bits_unused = 10;
    if((striping_ptr->buf = CALLOC(1, striping_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Striping_ptr->bit.LaneAttributes_Striping_stripeToConnectingLanesRevocableLane << (15 - LaneAttributes_Striping_stripeToConnectingLanesRevocableLane))
               | (Striping_ptr->bit.LaneAttributes_Striping_stripeDrawOnLeft                     << (15 - LaneAttributes_Striping_stripeDrawOnLeft))
               | (Striping_ptr->bit.LaneAttributes_Striping_stripeDrawOnRight                    << (15 - LaneAttributes_Striping_stripeDrawOnRight))
               | (Striping_ptr->bit.LaneAttributes_Striping_stripeToConnectingLanesLeft          << (15 - LaneAttributes_Striping_stripeToConnectingLanesLeft))
               | (Striping_ptr->bit.LaneAttributes_Striping_stripeToConnectingLanesRight         << (15 - LaneAttributes_Striping_stripeToConnectingLanesRight))
               | (Striping_ptr->bit.LaneAttributes_Striping_stripeToConnectingLanesAhead         << (15 - LaneAttributes_Striping_stripeToConnectingLanesAhead));
               
    striping_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    striping_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_LaneAttributes_Striping_free(striping_ptr);
    return result;
}


/* Parse routine for DE_LaneAttributes_Striping. */
int DE_LaneAttributes_Striping_parse(LaneAttributes_Striping_t *striping_ptr, DE_LaneAttributes_Striping_un_ptr Striping_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((striping_ptr == NULL) || (Striping_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Striping_ptr, 0, sizeof(*Striping_ptr));

    /* Parse the buffer. */
    if(striping_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = striping_ptr->buf[0];
    event_mask = (event_mask << 8) | striping_ptr->buf[1]; 

    /* Decode event data to mask. */
    Striping_ptr->bit.LaneAttributes_Striping_stripeToConnectingLanesRevocableLane = (event_mask >> (15 - LaneAttributes_Striping_stripeToConnectingLanesRevocableLane)) & 0x01;        
    Striping_ptr->bit.LaneAttributes_Striping_stripeDrawOnLeft                     = (event_mask >> (15 - LaneAttributes_Striping_stripeDrawOnLeft)) & 0x01;       
    Striping_ptr->bit.LaneAttributes_Striping_stripeDrawOnRight                    = (event_mask >> (15 - LaneAttributes_Striping_stripeDrawOnRight)) & 0x01;        
    Striping_ptr->bit.LaneAttributes_Striping_stripeToConnectingLanesLeft          = (event_mask >> (15 - LaneAttributes_Striping_stripeToConnectingLanesLeft)) & 0x01;
    Striping_ptr->bit.LaneAttributes_Striping_stripeToConnectingLanesRight         = (event_mask >> (15 - LaneAttributes_Striping_stripeToConnectingLanesRight)) & 0x01;        
    Striping_ptr->bit.LaneAttributes_Striping_stripeToConnectingLanesAhead         = (event_mask >> (15 - LaneAttributes_Striping_stripeToConnectingLanesAhead)) & 0x01;

ERR_EXIT:
    
    return result;
}

/* Free routine for DE_LaneAttributes_TrackedVehicle. */
int DE_LaneAttributes_TrackedVehicle_free(LaneAttributes_TrackedVehicle_t *trackedVehicle_ptr)
{
    int result = 0;

    if(trackedVehicle_ptr != NULL)
    {
        if(trackedVehicle_ptr->buf != NULL)
        {
            FREEMEM(trackedVehicle_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(trackedVehicle_ptr, 0, sizeof(*trackedVehicle_ptr));  
    }

    return result;
}

/* Allocate routine for DE_LaneAttributes_TrackedVehicle. */
int DE_LaneAttributes_TrackedVehicle_allocate(LaneAttributes_TrackedVehicle_t *trackedVehicle_ptr, DE_LaneAttributes_TrackedVehicle_un_ptr TrackedVehicle_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((trackedVehicle_ptr == NULL) || (TrackedVehicle_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(trackedVehicle_ptr, 0, sizeof(*trackedVehicle_ptr));

    /* Allocate bit data zone. */
    trackedVehicle_ptr->size = 2;
    trackedVehicle_ptr->bits_unused = 11;
    if((trackedVehicle_ptr->buf = CALLOC(1, trackedVehicle_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_RevocableLane         << (15 - LaneAttributes_TrackedVehicle_spec_RevocableLane))
               | (TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_commuterRailRoadTrack << (15 - LaneAttributes_TrackedVehicle_spec_commuterRailRoadTrack))
               | (TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_lightRailRoadTrack    << (15 - LaneAttributes_TrackedVehicle_spec_lightRailRoadTrack))
               | (TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_heavyRailRoadTrack    << (15 - LaneAttributes_TrackedVehicle_spec_heavyRailRoadTrack))
               | (TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_otherRailType         << (15 - LaneAttributes_TrackedVehicle_spec_otherRailType));
               
    trackedVehicle_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    trackedVehicle_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_LaneAttributes_TrackedVehicle_free(trackedVehicle_ptr);
    return result;
}


/* Parse routine for DE_LaneAttributes_TrackedVehicle. */
int DE_LaneAttributes_TrackedVehicle_parse(LaneAttributes_TrackedVehicle_t *trackedVehicle_ptr, DE_LaneAttributes_TrackedVehicle_un_ptr TrackedVehicle_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((trackedVehicle_ptr == NULL) || (TrackedVehicle_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(TrackedVehicle_ptr, 0, sizeof(*TrackedVehicle_ptr));

    /* Parse the buffer. */
    if(trackedVehicle_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = trackedVehicle_ptr->buf[0];
    event_mask = (event_mask << 8) | trackedVehicle_ptr->buf[1]; 

    /* Decode event data to mask. */
    TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_RevocableLane         = (event_mask >> (15 - LaneAttributes_TrackedVehicle_spec_RevocableLane)) & 0x01;        
    TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_commuterRailRoadTrack = (event_mask >> (15 - LaneAttributes_TrackedVehicle_spec_commuterRailRoadTrack)) & 0x01;       
    TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_lightRailRoadTrack    = (event_mask >> (15 - LaneAttributes_TrackedVehicle_spec_lightRailRoadTrack)) & 0x01;        
    TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_heavyRailRoadTrack    = (event_mask >> (15 - LaneAttributes_TrackedVehicle_spec_heavyRailRoadTrack)) & 0x01;
    TrackedVehicle_ptr->bit.LaneAttributes_TrackedVehicle_spec_otherRailType         = (event_mask >> (15 - LaneAttributes_TrackedVehicle_spec_otherRailType)) & 0x01;        
    
ERR_EXIT:
    
    return result;
}

/* Free routine for DE_LaneAttributes_Parking. */
int DE_LaneAttributes_Parking_free(LaneAttributes_Parking_t *parking_ptr)
{
    int result = 0;

    if(parking_ptr != NULL)
    {
        if(parking_ptr->buf != NULL)
        {
            FREEMEM(parking_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(parking_ptr, 0, sizeof(*parking_ptr));  
    }

    return result;
}

/* Allocate routine for DE_LaneAttributes_Parking. */
int DE_LaneAttributes_Parking_allocate(LaneAttributes_Parking_t *parking_ptr, DE_LaneAttributes_Parking_un_ptr Parking_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((parking_ptr == NULL) || (Parking_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(parking_ptr, 0, sizeof(*parking_ptr));

    /* Allocate bit data zone. */
    parking_ptr->size = 2;
    parking_ptr->bits_unused = 9;
    if((parking_ptr->buf = CALLOC(1, parking_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Parking_ptr->bit.LaneAttributes_Parking_parkingRevocableLane << (15 - LaneAttributes_Parking_parkingRevocableLane))
               | (Parking_ptr->bit.LaneAttributes_Parking_parallelParkingInUse << (15 - LaneAttributes_Parking_parallelParkingInUse))
               | (Parking_ptr->bit.LaneAttributes_Parking_headInParkingInUse   << (15 - LaneAttributes_Parking_headInParkingInUse))
               | (Parking_ptr->bit.LaneAttributes_Parking_doNotParkZone        << (15 - LaneAttributes_Parking_doNotParkZone))
               | (Parking_ptr->bit.LaneAttributes_Parking_parkingForBusUse     << (15 - LaneAttributes_Parking_parkingForBusUse))
               | (Parking_ptr->bit.LaneAttributes_Parking_parkingForTaxiUse    << (15 - LaneAttributes_Parking_parkingForTaxiUse))
               | (Parking_ptr->bit.LaneAttributes_Parking_noPublicParkingUse   << (15 - LaneAttributes_Parking_noPublicParkingUse));
               
    parking_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    parking_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_LaneAttributes_Parking_free(parking_ptr);
    return result;
}


/* Parse routine for DE_LaneAttributes_Parking. */
int DE_LaneAttributes_Parking_parse(LaneAttributes_Parking_t *parking_ptr, DE_LaneAttributes_Parking_un_ptr Parking_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((parking_ptr == NULL) || (Parking_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Parking_ptr, 0, sizeof(*Parking_ptr));

    /* Parse the buffer. */
    if(parking_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = parking_ptr->buf[0];
    event_mask = (event_mask << 8) | parking_ptr->buf[1]; 

    /* Decode event data to mask. */
    Parking_ptr->bit.LaneAttributes_Parking_parkingRevocableLane = (event_mask >> (15 - LaneAttributes_Parking_parkingRevocableLane)) & 0x01;        
    Parking_ptr->bit.LaneAttributes_Parking_parallelParkingInUse = (event_mask >> (15 - LaneAttributes_Parking_parallelParkingInUse)) & 0x01;       
    Parking_ptr->bit.LaneAttributes_Parking_headInParkingInUse   = (event_mask >> (15 - LaneAttributes_Parking_headInParkingInUse)) & 0x01;        
    Parking_ptr->bit.LaneAttributes_Parking_doNotParkZone        = (event_mask >> (15 - LaneAttributes_Parking_doNotParkZone)) & 0x01;
    Parking_ptr->bit.LaneAttributes_Parking_parkingForBusUse     = (event_mask >> (15 - LaneAttributes_Parking_parkingForBusUse)) & 0x01;  
    Parking_ptr->bit.LaneAttributes_Parking_parkingForTaxiUse    = (event_mask >> (15 - LaneAttributes_Parking_parkingForTaxiUse)) & 0x01;
    Parking_ptr->bit.LaneAttributes_Parking_noPublicParkingUse   = (event_mask >> (15 - LaneAttributes_Parking_noPublicParkingUse)) & 0x01;        
    
ERR_EXIT:
    
    return result;
}

/* Free routine for DE_LaneSharing. */
int DE_LaneSharing_free(LaneSharing_t *sharing_ptr)
{
    int result = 0;

    if(sharing_ptr != NULL)
    {
        if(sharing_ptr->buf != NULL)
        {
            FREEMEM(sharing_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(sharing_ptr, 0, sizeof(*sharing_ptr));  
    }

    return result;
}


/* Allocate routine for DE_LaneSharing. */
int DE_LaneSharing_allocate(LaneSharing_t *sharing_ptr, DE_LaneSharing_un_ptr Sharing_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((sharing_ptr == NULL) || (Sharing_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(sharing_ptr, 0, sizeof(*sharing_ptr));

    /* Allocate bit data zone. */
    sharing_ptr->size = 2;
    sharing_ptr->bits_unused = 6;
    if((sharing_ptr->buf = CALLOC(1, sharing_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Sharing_ptr->bit.LaneSharing_overlappingLaneDescriptionProvided << (15 - LaneSharing_overlappingLaneDescriptionProvided))
               | (Sharing_ptr->bit.LaneSharing_multipleLanesTreatedAsOneLane      << (15 - LaneSharing_multipleLanesTreatedAsOneLane))
               | (Sharing_ptr->bit.LaneSharing_otherNonMotorizedTrafficTypes      << (15 - LaneSharing_otherNonMotorizedTrafficTypes))
               | (Sharing_ptr->bit.LaneSharing_individualMotorizedVehicleTraffic  << (15 - LaneSharing_individualMotorizedVehicleTraffic))
               | (Sharing_ptr->bit.LaneSharing_busVehicleTraffic                  << (15 - LaneSharing_busVehicleTraffic))
               | (Sharing_ptr->bit.LaneSharing_taxiVehicleTraffic                 << (15 - LaneSharing_taxiVehicleTraffic))
               | (Sharing_ptr->bit.LaneSharing_pedestriansTraffic                 << (15 - LaneSharing_pedestriansTraffic))
               | (Sharing_ptr->bit.LaneSharing_cyclistVehicleTraffic              << (15 - LaneSharing_cyclistVehicleTraffic))
               | (Sharing_ptr->bit.LaneSharing_trackedVehicleTraffic              << (15 - LaneSharing_trackedVehicleTraffic))
               | (Sharing_ptr->bit.LaneSharing_pedestrianTraffic                  << (15 - LaneSharing_pedestrianTraffic));
               
    sharing_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    sharing_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_LaneSharing_free(sharing_ptr);
    return result;
}


/* Parse routine for DE_LaneSharing. */
int DE_LaneSharing_parse(LaneSharing_t *sharing_ptr, DE_LaneSharing_un_ptr Sharing_ptr)
{
    int             result = 0;
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((sharing_ptr == NULL) || (Sharing_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Sharing_ptr, 0, sizeof(*Sharing_ptr));

    /* Parse the buffer. */
    if(sharing_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = sharing_ptr->buf[0];
    event_mask = (event_mask << 8) | sharing_ptr->buf[1]; 

    /* Decode event data to alert mask. */
    Sharing_ptr->bit.LaneSharing_overlappingLaneDescriptionProvided = (event_mask >> (15 - LaneSharing_overlappingLaneDescriptionProvided)) & 0x01;        
    Sharing_ptr->bit.LaneSharing_multipleLanesTreatedAsOneLane      = (event_mask >> (15 - LaneSharing_multipleLanesTreatedAsOneLane)) & 0x01;       
    Sharing_ptr->bit.LaneSharing_otherNonMotorizedTrafficTypes      = (event_mask >> (15 - LaneSharing_otherNonMotorizedTrafficTypes)) & 0x01;        
    Sharing_ptr->bit.LaneSharing_individualMotorizedVehicleTraffic  = (event_mask >> (15 - LaneSharing_individualMotorizedVehicleTraffic)) & 0x01;       
    Sharing_ptr->bit.LaneSharing_busVehicleTraffic                  = (event_mask >> (15 - LaneSharing_busVehicleTraffic)) & 0x01;         
    Sharing_ptr->bit.LaneSharing_taxiVehicleTraffic                 = (event_mask >> (15 - LaneSharing_taxiVehicleTraffic)) & 0x01;        
    Sharing_ptr->bit.LaneSharing_pedestriansTraffic                 = (event_mask >> (15 - LaneSharing_pedestriansTraffic)) & 0x01;        
    Sharing_ptr->bit.LaneSharing_cyclistVehicleTraffic              = (event_mask >> (15 - LaneSharing_cyclistVehicleTraffic)) & 0x01;   
    Sharing_ptr->bit.LaneSharing_trackedVehicleTraffic              = (event_mask >> (15 - LaneSharing_trackedVehicleTraffic)) & 0x01;        
    Sharing_ptr->bit.LaneSharing_pedestrianTraffic                  = (event_mask >> (15 - LaneSharing_pedestrianTraffic)) & 0x01;

ERR_EXIT:
    
    return result;
}



#if 0
/* Free routine for DE_HeadingSlice. */
int DE_HeadingSlice_free(HeadingSlice_t *heading_ptr)
{
    if(heading_ptr != NULL)
    {
        if(heading_ptr->buf != NULL)
        {
            FREEMEM(heading_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(heading_ptr, 0, sizeof(*heading_ptr));

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DE_HeadingSlice. */
int DE_HeadingSlice_allocate(HeadingSlice_t *heading_ptr, DE_HeadingSlice_st_ptr Heading_ptr)
{
    int        result  = 0;
    /* Error detect. */
    if((heading_ptr == NULL) || (Heading_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(heading_ptr, 0, sizeof(*heading_ptr));

    /* Set the buffer. */
    heading_ptr->size = 2;
    heading_ptr->bits_unused = 0;
    if((heading_ptr->buf = CALLOC(1, heading_ptr->size)) != NULL)
    {
        heading_ptr->buf[0] = (Heading_ptr->HeadingSlice_from000_0to022_5degrees << (7 - HeadingSlice_from000_0to022_5degrees))
                            | (Heading_ptr->HeadingSlice_from022_5to045_0degrees << (7 - HeadingSlice_from022_5to045_0degrees))
                            | (Heading_ptr->HeadingSlice_from045_0to067_5degrees << (7 - HeadingSlice_from045_0to067_5degrees))
                            | (Heading_ptr->HeadingSlice_from067_5to090_0degrees << (7 - HeadingSlice_from067_5to090_0degrees))
                            | (Heading_ptr->HeadingSlice_from090_0to112_5degrees << (7 - HeadingSlice_from090_0to112_5degrees))
                            | (Heading_ptr->HeadingSlice_from112_5to135_0degrees << (7 - HeadingSlice_from112_5to135_0degrees))
                            | (Heading_ptr->HeadingSlice_from135_0to157_5degrees << (7 - HeadingSlice_from135_0to157_5degrees))
                            | (Heading_ptr->HeadingSlice_from157_5to180_0degrees << (7 - HeadingSlice_from157_5to180_0degrees));   

        heading_ptr->buf[1] = (Heading_ptr->HeadingSlice_from180_0to202_5degrees << (7+8 - HeadingSlice_from180_0to202_5degrees))
                            | (Heading_ptr->HeadingSlice_from202_5to225_0degrees << (7+8 - HeadingSlice_from202_5to225_0degrees))
                            | (Heading_ptr->HeadingSlice_from225_0to247_5degrees << (7+8 - HeadingSlice_from225_0to247_5degrees))
                            | (Heading_ptr->HeadingSlice_from247_5to270_0degrees << (7+8 - HeadingSlice_from247_5to270_0degrees))
                            | (Heading_ptr->HeadingSlice_from270_0to292_5degrees << (7+8 - HeadingSlice_from270_0to292_5degrees))
                            | (Heading_ptr->HeadingSlice_from292_5to315_0degrees << (7+8 - HeadingSlice_from292_5to315_0degrees))
                            | (Heading_ptr->HeadingSlice_from315_0to337_5degrees << (7+8 - HeadingSlice_from315_0to337_5degrees))
                            | (Heading_ptr->HeadingSlice_from337_5to360_0degrees << (7+8 - HeadingSlice_from337_5to360_0degrees));    
    }
    else
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    return result;

ERR_EXIT:

    DE_GNSSstatus_free(heading_ptr);
    return result;
}


/* Parse routine for DE_HeadingSlice. */
int DE_HeadingSlice_parse(HeadingSlice_t *heading_ptr, DE_HeadingSlice_st_ptr Heading_ptr)
{
    int result = 0;
    /* Error detect. */
    if((heading_ptr == NULL) || (Heading_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Heading_ptr, 0, sizeof(*Heading_ptr));

    /* Parse the buffer. */
    Heading_ptr->HeadingSlice_from000_0to022_5degrees = ((heading_ptr->buf[0]) >> (7 - HeadingSlice_from000_0to022_5degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from022_5to045_0degrees = ((heading_ptr->buf[0]) >> (7 - HeadingSlice_from022_5to045_0degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from045_0to067_5degrees = ((heading_ptr->buf[0]) >> (7 - HeadingSlice_from045_0to067_5degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from067_5to090_0degrees = ((heading_ptr->buf[0]) >> (7 - HeadingSlice_from067_5to090_0degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from090_0to112_5degrees = ((heading_ptr->buf[0]) >> (7 - HeadingSlice_from090_0to112_5degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from112_5to135_0degrees = ((heading_ptr->buf[0]) >> (7 - HeadingSlice_from112_5to135_0degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from135_0to157_5degrees = ((heading_ptr->buf[0]) >> (7 - HeadingSlice_from135_0to157_5degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from157_5to180_0degrees = ((heading_ptr->buf[0]) >> (7 - HeadingSlice_from157_5to180_0degrees)) & 0x01;

    Heading_ptr->HeadingSlice_from180_0to202_5degrees = ((heading_ptr->buf[1]) >> (7+8 - HeadingSlice_from180_0to202_5degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from202_5to225_0degrees = ((heading_ptr->buf[1]) >> (7+8 - HeadingSlice_from202_5to225_0degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from225_0to247_5degrees = ((heading_ptr->buf[1]) >> (7+8 - HeadingSlice_from225_0to247_5degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from247_5to270_0degrees = ((heading_ptr->buf[1]) >> (7+8 - HeadingSlice_from247_5to270_0degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from270_0to292_5degrees = ((heading_ptr->buf[1]) >> (7+8 - HeadingSlice_from270_0to292_5degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from292_5to315_0degrees = ((heading_ptr->buf[1]) >> (7+8 - HeadingSlice_from292_5to315_0degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from315_0to337_5degrees = ((heading_ptr->buf[1]) >> (7+8 - HeadingSlice_from315_0to337_5degrees)) & 0x01;
    Heading_ptr->HeadingSlice_from337_5to360_0degrees = ((heading_ptr->buf[1]) >> (7+8 - HeadingSlice_from337_5to360_0degrees)) & 0x01;
 
    result =  0;

ERR_EXIT:
    
    return result;
}
#endif


/* free routine of DE_plateNo */
int DE_PlateNo_free(OCTET_STRING_t *plateNo_ptr)
{
    int result = 0;
    
    if(plateNo_ptr != NULL)
    {
        if(plateNo_ptr->buf != NULL)
        {
            FREEMEM(plateNo_ptr->buf);
        }
        /* Must clear all the zone and avoid repeat free operation*/
        memset(plateNo_ptr, 0x00, sizeof(*plateNo_ptr));
        
    }else{
    
        result = -ERR_NOMEM;
    }

    return result;
}


/* allocate routine for DE_plateNo */
int DE_PlateNo_allocate(OCTET_STRING_t *plateNo_ptr, DE_PlateNo_st_ptr PlateNo_ptr)
{
    int result = 0;

    
    if((plateNo_ptr == NULL) || (PlateNo_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /*Reset all the zone. */
    memset(plateNo_ptr, 0x00, sizeof(*plateNo_ptr));
    
    plateNo_ptr->size = PlateNo_ptr->bufsize;
    plateNo_ptr->buf = CALLOC(1, plateNo_ptr->size);
    
    if(plateNo_ptr->buf == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }else{
        memcpy(plateNo_ptr->buf, PlateNo_ptr->buf, plateNo_ptr->size);
    }
    
    return result;
    
ERR_EXIT:
    
    DE_PlateNo_free(plateNo_ptr);
    return result;
}

/* parse routine of DE_plateNo */
int DE_PlateNo_parse(OCTET_STRING_t *plateNo_ptr, DE_PlateNo_st_ptr PlateNo_ptr)
{
    int result = 0;

    if((plateNo_ptr == NULL) || (PlateNo_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(PlateNo_ptr, 0, DE_PlateNo_st_len);

    /* Copy id data. */
    PlateNo_ptr->bufsize = plateNo_ptr->size;
    memcpy(PlateNo_ptr->buf, plateNo_ptr->buf, PlateNo_ptr->bufsize);

    return result;

ERR_EXIT:

    return result;
}


/* Free routine for DE_Priority. */
int DE_Priority_free(Priority_t *pri_ptr)
{
    if(pri_ptr != NULL)
    {
        if(pri_ptr->buf != NULL)
        {
            FREEMEM(pri_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(pri_ptr, 0, sizeof(*pri_ptr));

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DE_Priority. */
int DE_Priority_allocate(Priority_t *pri_ptr, DE_Priority_st_ptr Pri_ptr)
{
    int        result = 0;


    /* Error detect. */
    if((pri_ptr == NULL) || (Pri_ptr == NULL))
    {
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(pri_ptr, 0, sizeof(*pri_ptr));

    /* TemporaryID. */
    pri_ptr->size = DE_Priority_BUFSIZE;
    if((pri_ptr->buf = CALLOC(1, pri_ptr->size)) != NULL)
    {
        memcpy(pri_ptr->buf, &((*Pri_ptr)[0]), pri_ptr->size);                  
    }
    else
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    return result;

ERR_EXIT:

    DE_Priority_free(pri_ptr);
    return result;
}


/* Parse routine for DE_Priority. */
int DE_Priority_parse(Priority_t *pri_ptr, DE_Priority_st_ptr Pri_ptr)
{
    int     result = 0;
    /* Error detect. */
    if((pri_ptr == NULL) || (Pri_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(&(*Pri_ptr[0]), 0, DE_Priority_st_len);

    /* Copy id data. */
    memcpy(&(*Pri_ptr[0]), pri_ptr->buf, DE_Priority_st_len);

    return result;

ERR_EXIT:
    
    return result;
}

#if 0
/* Free routine for DE_RTCMmessage. */
int DE_RTCMmessage_free(RTCMmessage_t *msg_ptr)
{
    if(msg_ptr != NULL)
    {
        if(msg_ptr->buf != NULL)
        {
            FREEMEM(msg_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(msg_ptr, 0, sizeof(*msg_ptr));

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Free extention routine for DE_RTCMmessage. Diff with DE_RTCMmessage_free() and this will free the point itself. */
int DE_RTCMmessage_freeExt(RTCMmessage_t *msg_ptr)
{
    if(msg_ptr != NULL)
    {
        /* Free the message content and message itself. */
        DE_RTCMmessage_free(msg_ptr);
        FREEMEM(msg_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DE_RTCMmessage. */
int DE_RTCMmessage_allocate(RTCMmessage_t *msg_ptr, DE_RTCMmessage_st_ptr Msg_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((msg_ptr == NULL) || (Msg_ptr == NULL) 
    || (Msg_ptr->length < DE_RTCMmessage_BUFSIZE_MIN) || (DE_RTCMmessage_BUFSIZE_MAX < Msg_ptr->length))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(msg_ptr, 0, sizeof(*msg_ptr));

    /* TemporaryID. */
    msg_ptr->size = Msg_ptr->length;
    if((msg_ptr->buf = CALLOC(1, msg_ptr->size)) != NULL)
    {
        memcpy(msg_ptr->buf, Msg_ptr->data, msg_ptr->size);                  
    }
    else
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    return result;

ERR_EXIT:

    DE_TemporaryID_free(msg_ptr);
    return result;
}


/* Parse routine for DE_RTCMmessage. */
int DE_RTCMmessage_parse(RTCMmessage_t *msg_ptr, DE_RTCMmessage_st_ptr Msg_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((msg_ptr == NULL) || (Msg_ptr == NULL) 
    || (msg_ptr->size < DE_RTCMmessage_BUFSIZE_MIN) || (DE_RTCMmessage_BUFSIZE_MAX < msg_ptr->size))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Msg_ptr, 0, sizeof(*Msg_ptr));

    /* Copy id data and return actual data length. */
    memcpy(Msg_ptr->data, msg_ptr->buf, msg_ptr->size);
    Msg_ptr->length = msg_ptr->size;

    return result;

ERR_EXIT:
    
    return result;
}
#endif

/* Free routine for DE_TemporaryID. */
int DE_TemporaryID_free(OCTET_STRING_t *id_ptr)
{
    int result = 0;

    if(id_ptr != NULL)
    {
        if(id_ptr->buf != NULL)
        {
            FREEMEM(id_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(id_ptr, 0, sizeof(*id_ptr));
    }

    return result;
}


/* Allocate routine for DE_TemporaryID. */
int DE_TemporaryID_allocate(OCTET_STRING_t *id_ptr, DE_TemporaryID_st_ptr ID_ptr)
{
    int        result = 0;

    
    /* Error detect. */
    if((id_ptr == NULL) || (ID_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(id_ptr, 0, sizeof(*id_ptr));

    /* TemporaryID. */
    id_ptr->size = DE_TemporaryID_BUFSIZE;
    if((id_ptr->buf = CALLOC(1, id_ptr->size)) != NULL)
    {
        memcpy(id_ptr->buf, &((*ID_ptr)[0]), id_ptr->size);                  
    }
    else
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    return result;

ERR_EXIT:

    DE_TemporaryID_free(id_ptr);
    return result;
}


/* Parse routine for DE_TemporaryID. */
int DE_TemporaryID_parse(OCTET_STRING_t *id_ptr, DE_TemporaryID_st_ptr ID_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((id_ptr == NULL) || (ID_ptr == NULL))
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(&(*ID_ptr[0]), 0, DE_TemporaryID_st_len);

    /* Copy id data. */
    memcpy(&(*ID_ptr[0]), id_ptr->buf, DE_TemporaryID_st_len);

    return result;

ERR_EXIT:
    
    return result;
}


/* Free routine for DE_VehicleEventFlags. */
int DE_VehicleEventFlags_free(VehicleEventFlags_t *event_ptr)
{
    int result = 0;

    if(event_ptr != NULL)
    {
        if(event_ptr->buf != NULL)
        {
            FREEMEM(event_ptr->buf);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(event_ptr, 0, sizeof(*event_ptr));
        
    }

    return result;
    
}


/* Allocate routine for DE_VehicleEventFlags. */
int DE_VehicleEventFlags_allocate(VehicleEventFlags_t *event_ptr, DE_VehicleEventFlags_un_ptr Event_ptr)
{
    int result = 0;

    uint16_t    event_mask = 0;


    /* Error detect. */
    if((event_ptr == NULL) || (Event_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(event_ptr, 0, sizeof(*event_ptr));

    /* Allocate bit data zone. */
    event_ptr->size = 2;
    event_ptr->bits_unused = 3;
    if((event_ptr->buf = CALLOC(1, event_ptr->size)) == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Encode event data. */
    event_mask = (Event_ptr->bit.VehicleEventFlags_eventHazardLights              << (15 - VehicleEventFlags_eventHazardLights))
               | (Event_ptr->bit.VehicleEventFlags_eventStopLineViolation         << (15 - VehicleEventFlags_eventStopLineViolation))
               | (Event_ptr->bit.VehicleEventFlags_eventABSactivated              << (15 - VehicleEventFlags_eventABSactivated))
               | (Event_ptr->bit.VehicleEventFlags_eventTractionControlLoss       << (15 - VehicleEventFlags_eventTractionControlLoss))
               | (Event_ptr->bit.VehicleEventFlags_eventStabilityControlactivated << (15 - VehicleEventFlags_eventStabilityControlactivated))
               | (Event_ptr->bit.VehicleEventFlags_eventHazardousMaterials        << (15 - VehicleEventFlags_eventHazardousMaterials))
               | (Event_ptr->bit.VehicleEventFlags_eventReserved1                 << (15 - VehicleEventFlags_eventReserved1))
               | (Event_ptr->bit.VehicleEventFlags_eventHardBraking               << (15 - VehicleEventFlags_eventHardBraking))
               | (Event_ptr->bit.VehicleEventFlags_eventLightsChanged             << (15 - VehicleEventFlags_eventLightsChanged))
               | (Event_ptr->bit.VehicleEventFlags_eventWipersChanged             << (15 - VehicleEventFlags_eventWipersChanged))
               | (Event_ptr->bit.VehicleEventFlags_eventFlatTire                  << (15 - VehicleEventFlags_eventFlatTire))
               | (Event_ptr->bit.VehicleEventFlags_eventDisabledVehicle           << (15 - VehicleEventFlags_eventDisabledVehicle))
               | (Event_ptr->bit.VehicleEventFlags_eventAirBagDeployment          << (15 - VehicleEventFlags_eventAirBagDeployment));
        
    event_ptr->buf[0] = (event_mask >> 8) & 0x00FF;
    event_ptr->buf[1] = event_mask & 0x00FF;

    return result;

ERR_EXIT:

    DE_VehicleEventFlags_free(event_ptr);
    return result;
}


/* Parse routine for DE_VehicleEventFlags. */
int DE_VehicleEventFlags_parse(VehicleEventFlags_t *event_ptr, DE_VehicleEventFlags_un_ptr Event_ptr)
{
    int result = 0;
    
    uint16_t    event_mask = 0;


    /* Error detect. */
    if((event_ptr == NULL) || (Event_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Event_ptr, 0, sizeof(*Event_ptr));

    /* Parse the buffer. */
    if(event_ptr->buf == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    event_mask = event_ptr->buf[0];
    event_mask = (event_mask << 8) | event_ptr->buf[1]; 

    /* Decode event data to alert mask. */
    Event_ptr->bit.VehicleEventFlags_eventHazardLights              = (event_mask >> (15 - VehicleEventFlags_eventHazardLights)) & 0x01;        
    Event_ptr->bit.VehicleEventFlags_eventStopLineViolation         = (event_mask >> (15 - VehicleEventFlags_eventStopLineViolation)) & 0x01;       
    Event_ptr->bit.VehicleEventFlags_eventABSactivated              = (event_mask >> (15 - VehicleEventFlags_eventABSactivated)) & 0x01;        
    Event_ptr->bit.VehicleEventFlags_eventTractionControlLoss       = (event_mask >> (15 - VehicleEventFlags_eventTractionControlLoss)) & 0x01;       
    Event_ptr->bit.VehicleEventFlags_eventStabilityControlactivated = (event_mask >> (15 - VehicleEventFlags_eventStabilityControlactivated)) & 0x01;        
  
    Event_ptr->bit.VehicleEventFlags_eventHazardousMaterials        = (event_mask >> (15 - VehicleEventFlags_eventHazardousMaterials)) & 0x01;        
    Event_ptr->bit.VehicleEventFlags_eventReserved1                 = (event_mask >> (15 - VehicleEventFlags_eventReserved1)) & 0x01;        
    Event_ptr->bit.VehicleEventFlags_eventHardBraking               = (event_mask >> (15 - VehicleEventFlags_eventHardBraking)) & 0x01;        
    Event_ptr->bit.VehicleEventFlags_eventLightsChanged             = (event_mask >> (15 - VehicleEventFlags_eventLightsChanged)) & 0x01;        
    Event_ptr->bit.VehicleEventFlags_eventWipersChanged             = (event_mask >> (15 - VehicleEventFlags_eventWipersChanged)) & 0x01;        

    Event_ptr->bit.VehicleEventFlags_eventFlatTire                  = (event_mask >> (15 - VehicleEventFlags_eventFlatTire)) & 0x01;    
    Event_ptr->bit.VehicleEventFlags_eventDisabledVehicle           = (event_mask >> (15 - VehicleEventFlags_eventDisabledVehicle)) & 0x01;    
    Event_ptr->bit.VehicleEventFlags_eventAirBagDeployment          = (event_mask >> (15 - VehicleEventFlags_eventAirBagDeployment)) & 0x01; 

    return result;

ERR_EXIT:
    
    return result;
}




