/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_data_frame.c
 @brief  : Data Frame routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/

#include "cv_data_frame.h"
#include "cv_data_element.h"


#if 0
/* Free routine for DF_AntennaOffsetSet. */
int DF_AntennaOffsetSet_free(AntennaOffsetSet_t *offset_ptr)
{
    if(offset_ptr != NULL)
    {
        /* Must clear all the zone and avoid repeat free operation. */
        memset(offset_ptr, 0, sizeof(*offset_ptr));
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_AntennaOffsetSet. */
int DF_AntennaOffsetSet_allocate(AntennaOffsetSet_t *offset_ptr, DF_AntennaOffsetSet_st_ptr Offset_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((offset_ptr == NULL) || (Offset_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(offset_ptr, 0, sizeof(*offset_ptr));

    /* Encode offset group. */
    offset_ptr->antOffsetX = bsctyp_encode(DE_Offset_B12, Offset_ptr->antiOffsetX_B12);
    offset_ptr->antOffsetX = bsctyp_encode(DE_Offset_B09, Offset_ptr->antiOffsetY_B09);
    offset_ptr->antOffsetX = bsctyp_encode(DE_Offset_B10, Offset_ptr->antiOffsetZ_B10);
    
    return result;

ERR_EXIT:

    DF_AntennaOffsetSet_free(offset_ptr);
    return result;
}


/* Parse routine for DF_AntennaOffsetSet. */
int DF_AntennaOffsetSet_parse(AntennaOffsetSet_t *offset_ptr, DF_AntennaOffsetSet_st_ptr Offset_ptr)
{  
    int        result = 0;
    /* Error detect. */
    if((offset_ptr == NULL) || (Offset_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Offset_ptr, 0, sizeof(*Offset_ptr));

    /* Decode offset group. */
    Offset_ptr->antiOffsetX_B12 = bsctyp_decode(DE_Offset_B12, offset_ptr->antOffsetX);
    Offset_ptr->antiOffsetY_B09 = bsctyp_decode(DE_Offset_B09, offset_ptr->antOffsetX);
    Offset_ptr->antiOffsetZ_B10 = bsctyp_decode(DE_Offset_B10, offset_ptr->antOffsetX);
  
    return result;

ERR_EXIT:

    return result;
}
#endif


/* Free routine for DF_AccelerationSet4Way. */
int DF_AccelerationSet4Way_free(AccelerationSet4Way_t *accset_ptr)
{
    if(accset_ptr != NULL)
    {
        /* Must clear all the zone and avoid repeat free operation. */
        memset(accset_ptr, 0, sizeof(*accset_ptr));
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_AccelerationSet4Way. */
int DF_AccelerationSet4Way_allocate(AccelerationSet4Way_t *accset_ptr, DF_AccelerationSet4Way_st_ptr Accset_ptr)
{
    int result = 0;

    if((accset_ptr == NULL) || (Accset_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(accset_ptr, 0x00, sizeof(*accset_ptr));

    accset_ptr->Long   = bsctyp_encode(DE_Acceleration, Accset_ptr->longitudinal);
    accset_ptr->lat    = bsctyp_encode(DE_Acceleration, Accset_ptr->lateral);
    accset_ptr->vert   = bsctyp_encode(DE_VerticalAcceleration, Accset_ptr->vertical);
    accset_ptr->yaw    = bsctyp_encode(DE_YawRate, Accset_ptr->yaw_rate);

    return result;
    
ERR_EXIT:
    
    DF_AccelerationSet4Way_free(accset_ptr);
    return result;
}

/* Parse routine for DF_AccelerationSet4Way. */
int DF_AccelerationSet4Way_parse(AccelerationSet4Way_t *accset_ptr, DF_AccelerationSet4Way_st_ptr Accset_ptr)
{
    int result = 0;

    if((accset_ptr == NULL) || (Accset_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Accset_ptr, 0x00, sizeof(*Accset_ptr));
    
    Accset_ptr->longitudinal = bsctyp_decode(DE_Acceleration, accset_ptr->Long);
    Accset_ptr->lateral      = bsctyp_decode(DE_Acceleration, accset_ptr->lat);
    Accset_ptr->vertical     = bsctyp_decode(DE_VerticalAcceleration, accset_ptr->vert);
    Accset_ptr->yaw_rate     = bsctyp_decode(DE_YawRate, accset_ptr->yaw);
    
    return result;
    
ERR_EXIT:

    return result;

}


/* Free routine for DF_BrakeSystemStatus. */
int DF_BrakeSystemStatus_free(BrakeSystemStatus_t *brakes_ptr)
{
    int result = 0;

    if(brakes_ptr != NULL)
    {
        if(brakes_ptr->brakePadel != NULL)
        {
            FREEMEM(brakes_ptr->brakePadel);
        }

        if(brakes_ptr->wheelBrakes != NULL)
        {
            DE_BrakeAppliedStatus_free(brakes_ptr->wheelBrakes);
            FREEMEM(brakes_ptr->wheelBrakes);
        }

        if(brakes_ptr->traction != NULL)
        {
            FREEMEM(brakes_ptr->traction);
        }

        if(brakes_ptr->abs != NULL)
        {
            FREEMEM(brakes_ptr->abs);
        }

        if(brakes_ptr->scs != NULL)
        {
            FREEMEM(brakes_ptr->scs);
        }

        if(brakes_ptr->brakeBoost!= NULL)
        {
            FREEMEM(brakes_ptr->brakeBoost);
        }

        if(brakes_ptr->auxBrakes!= NULL)
        {
            FREEMEM(brakes_ptr->auxBrakes);
        }

        memset(brakes_ptr, 0x00, sizeof(*brakes_ptr));
    }

    return result;
}

/* Allocate routine for DF_BrakeSystemStatus. */
int DF_BrakeSystemStatus_allocate(BrakeSystemStatus_t *brakes_ptr, DF_BrakeSystemStatus_st_ptr Brakes_ptr)
{
    int result = 0;
    
    if((brakes_ptr == NULL)||(Brakes_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(brakes_ptr, 0x00, sizeof(*brakes_ptr));

    if(Brakes_ptr->opt.BrakeSystemStatus_opt_brakepadel == MSG_OPTIONAL_YES)
    {
        brakes_ptr->brakePadel = CALLOC(1, sizeof(*brakes_ptr->brakePadel));
        if(brakes_ptr->brakePadel == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *brakes_ptr->brakePadel = Brakes_ptr->brakepedal;
    }else{
        brakes_ptr->brakePadel = NULL;
    }

    if(Brakes_ptr->opt.BrakeSystemStatus_opt_wheelBrakes == MSG_OPTIONAL_YES)
    {
        brakes_ptr->wheelBrakes = CALLOC(1, sizeof(*brakes_ptr->wheelBrakes));
        if(brakes_ptr->wheelBrakes == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        if((result = DE_BrakeAppliedStatus_allocate(brakes_ptr->wheelBrakes, &Brakes_ptr->wheelBrakes)) != 0)
        {
            goto ERR_EXIT;
        }
    }else{
        brakes_ptr->wheelBrakes = NULL;
    }

    if(Brakes_ptr->opt.BrakeSystemStatus_opt_traction == MSG_OPTIONAL_YES)
    {
        brakes_ptr->traction = CALLOC(1, sizeof(*brakes_ptr->traction));
        if(brakes_ptr->traction == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *brakes_ptr->traction = Brakes_ptr->traction;
    }else{
        brakes_ptr->traction = NULL;
    }

    if(Brakes_ptr->opt.BrakeSystemStatus_opt_abs == MSG_OPTIONAL_YES)
    {
        brakes_ptr->abs = CALLOC(1, sizeof(*brakes_ptr->abs));
        if(brakes_ptr->abs == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *brakes_ptr->abs = Brakes_ptr->abs;
    }else{
        brakes_ptr->abs = NULL;
    }

    if(Brakes_ptr->opt.BrakeSystemStatus_opt_scs == MSG_OPTIONAL_YES)
    {
        brakes_ptr->scs = CALLOC(1, sizeof(*brakes_ptr->scs));
        if(brakes_ptr->scs == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *brakes_ptr->scs = Brakes_ptr->scs;
    }else{
        brakes_ptr->scs = NULL;
    }

    if(Brakes_ptr->opt.BrakeSystemStatus_opt_brakeBoost == MSG_OPTIONAL_YES)
    {
        brakes_ptr->brakeBoost= CALLOC(1, sizeof(*brakes_ptr->brakeBoost));
        if(brakes_ptr->brakeBoost == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *brakes_ptr->brakeBoost = Brakes_ptr->brakeBoost;
    }else{
        brakes_ptr->brakeBoost = NULL;
    }

    if(Brakes_ptr->opt.BrakeSystemStatus_opt_auxBrakes == MSG_OPTIONAL_YES)
    {
        brakes_ptr->auxBrakes = CALLOC(1, sizeof(*brakes_ptr->auxBrakes));
        if(brakes_ptr->auxBrakes == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *brakes_ptr->auxBrakes = Brakes_ptr->auxBrakes;
    }else{
        brakes_ptr->auxBrakes = NULL;
    }

    return result;
ERR_EXIT:

    DF_BrakeSystemStatus_free(brakes_ptr);

    return result;
}


/* parse routine for DF_BrakeSystemStatus. */
int DF_BrakeSystemStatus_parse(BrakeSystemStatus_t *brakes_ptr, DF_BrakeSystemStatus_st_ptr Brakes_ptr)
{
    int result = 0;

    if((brakes_ptr == NULL)||(Brakes_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone */
    memset(Brakes_ptr, 0x00, sizeof(*Brakes_ptr));

    if(brakes_ptr->brakePadel != NULL)
    {
        Brakes_ptr->opt.BrakeSystemStatus_opt_brakepadel = MSG_OPTIONAL_YES;
        Brakes_ptr->brakepedal = *brakes_ptr->brakePadel;
    }

    if(brakes_ptr->wheelBrakes != NULL)
    {
        Brakes_ptr->opt.BrakeSystemStatus_opt_wheelBrakes = MSG_OPTIONAL_YES;
        if((result = DE_BrakeAppliedStatus_parse(brakes_ptr->wheelBrakes, &Brakes_ptr->wheelBrakes)) != 0)
        {
            goto ERR_EXIT;
        }
    }

    if(brakes_ptr->traction!= NULL)
    {
        Brakes_ptr->opt.BrakeSystemStatus_opt_traction = MSG_OPTIONAL_YES;
        Brakes_ptr->traction = *brakes_ptr->traction;
    }

    if(brakes_ptr->abs != NULL)
    {
        Brakes_ptr->opt.BrakeSystemStatus_opt_abs = MSG_OPTIONAL_YES;
        Brakes_ptr->abs = *brakes_ptr->abs;
    }

    if(brakes_ptr->scs != NULL)
    {
        Brakes_ptr->opt.BrakeSystemStatus_opt_scs = MSG_OPTIONAL_YES;
        Brakes_ptr->scs = *brakes_ptr->scs;
    }

    if(brakes_ptr->brakeBoost != NULL)
    {
        Brakes_ptr->opt.BrakeSystemStatus_opt_brakeBoost = MSG_OPTIONAL_YES;
        Brakes_ptr->brakeBoost = *brakes_ptr->brakeBoost;
    }

    if(brakes_ptr->auxBrakes!= NULL)
    {
        Brakes_ptr->opt.BrakeSystemStatus_opt_auxBrakes = MSG_OPTIONAL_YES;
        Brakes_ptr->auxBrakes = *brakes_ptr->auxBrakes;
    }

ERR_EXIT:
    
    return result;

}

/* Free routine for DF_ConnectingLane. */
int DF_ConnectingLane_free(ConnectingLane_t *connlane_ptr)
{
    int result = 0;
    if(connlane_ptr != NULL)
    {
        if(connlane_ptr->maneuver != NULL)
        {
            result = DE_AllowedManeuvers_free(connlane_ptr->maneuver);
            if(result)
                return result;
            
            FREEMEM(connlane_ptr->maneuver);
        }
            
        /* Must clear all the zone and avoid repeat free operation. */
        memset(connlane_ptr, 0, sizeof(*connlane_ptr));
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_ConnectingLane. */
int DF_ConnectingLane_allocate(ConnectingLane_t *connlane_ptr, DF_ConnectingLane_st_ptr Connlane_ptr)
{
    int result = 0;

    if((connlane_ptr == NULL) || (Connlane_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(connlane_ptr, 0x00, sizeof(*connlane_ptr));

    connlane_ptr->lane = bsctyp_encode(DE_LaneID, Connlane_ptr->lane);

    
    if(Connlane_ptr->opt.DF_ConnectingLane_opt_maneuver == MSG_OPTIONAL_YES)
    {
        if((connlane_ptr->maneuver = CALLOC(1, sizeof(*(connlane_ptr->maneuver)))) != NULL)
        {
            result = DE_AllowedManeuvers_allocate(connlane_ptr->maneuver, &(Connlane_ptr->maneuver));
            if(result)
                goto ERR_EXIT;
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        connlane_ptr->maneuver = NULL;
    }

    return result;
    
ERR_EXIT:
    
    DF_ConnectingLane_free(connlane_ptr);
    return result;
}

/* Parse routine for DF_ConnectingLane. */
int DF_ConnectingLane_parse(ConnectingLane_t *connlane_ptr, DF_ConnectingLane_st_ptr Connlane_ptr)
{
    int result = 0;

    if((connlane_ptr == NULL) || (Connlane_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Connlane_ptr, 0x00, sizeof(*Connlane_ptr));
    
    Connlane_ptr->lane = bsctyp_decode(DE_LaneID, connlane_ptr->lane);
    
    if(connlane_ptr->maneuver != NULL)
    {
        result = DE_AllowedManeuvers_parse(connlane_ptr->maneuver, &(Connlane_ptr->maneuver));
        if(result)
            goto ERR_EXIT;
        
        Connlane_ptr->opt.DF_ConnectingLane_opt_maneuver = MSG_OPTIONAL_YES;
    }
    else
        Connlane_ptr->opt.DF_ConnectingLane_opt_maneuver = MSG_OPTIONAL_NO;
    
    return result;
    
ERR_EXIT:

    return result;

}

/* Free routine for DF_Connection. */
int DF_Connection_free(Connection_t *conn_ptr)
{
    int result = 0;
    if(conn_ptr != NULL)
    {
        if(conn_ptr->connectingLane != NULL)
        {
            result = DF_ConnectingLane_free(conn_ptr->connectingLane);
            if(result)
                return result;
            
            FREEMEM(conn_ptr->connectingLane);
        }

        if(conn_ptr->phaseId != NULL)
        {
            FREEMEM(conn_ptr->phaseId);
        }
        
        /* Must clear all the zone and avoid repeat free operation. */
        memset(conn_ptr, 0, sizeof(*conn_ptr));
        return 0;
    }
    else
    {
        return -1;
    }
}

/* Free extention routine for DF_Connection. Diff with DF_Connection_free() and this will free the point itself. */
int DF_Connection_freeExt(Connection_t *conn_ptr)
{
    int result = 0;
    if(conn_ptr != NULL)
    {
        /* Free Connection point content and point itself. */
        result = DF_Connection_free(conn_ptr);
        if(result)
            return result;
        
        FREEMEM(conn_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_Connection. */
int DF_Connection_allocate(Connection_t *conn_ptr, DF_Connection_st_ptr Conn_ptr)
{
    int result = 0;

    if((conn_ptr == NULL) || (Conn_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(conn_ptr, 0x00, sizeof(*conn_ptr));

    result = DF_NodeReferenceID_allocate(&(conn_ptr->remoteIntersection),&(Conn_ptr->remoteIntersection));
    if(result)
        goto ERR_EXIT;

    if(Conn_ptr->opt.Connection_opt_connectingLane == MSG_OPTIONAL_YES)
    {
        if((conn_ptr->connectingLane = CALLOC(1, sizeof(*(conn_ptr->connectingLane)))) != NULL)
        {
            result = DF_ConnectingLane_allocate(conn_ptr->connectingLane, &(Conn_ptr->connectingLane));
            if(result)
                goto ERR_EXIT;
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        conn_ptr->connectingLane = NULL;
    }
    
    if(Conn_ptr->opt.Connection_opt_phaseId == MSG_OPTIONAL_YES)
    {
        if((conn_ptr->phaseId = CALLOC(1, sizeof(*(conn_ptr->phaseId)))) != NULL)
        {
            *(conn_ptr->phaseId) = bsctyp_encode(DE_PhaseID, Conn_ptr->phaseid);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        conn_ptr->connectingLane = NULL;
    }

    return result;
    
ERR_EXIT:
    
    DF_Connection_free(conn_ptr);
    return result;
}

/* Parse routine for DF_Connection. */
int DF_Connection_parse(Connection_t *conn_ptr, DF_Connection_st_ptr Conn_ptr)
{
    int result = 0;

    if((conn_ptr == NULL) || (Conn_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Conn_ptr, 0x00, sizeof(*Conn_ptr));

    result = DF_NodeReferenceID_parse(&(conn_ptr->remoteIntersection),&(Conn_ptr->remoteIntersection));
    if(result)
        goto ERR_EXIT;

    if(conn_ptr->connectingLane != NULL)
    {
        result = DF_ConnectingLane_parse(conn_ptr->connectingLane, &(Conn_ptr->connectingLane));
        if(result)
            goto ERR_EXIT;
        
        Conn_ptr->opt.Connection_opt_connectingLane = MSG_OPTIONAL_YES;
    }
    else
        Conn_ptr->opt.Connection_opt_connectingLane = MSG_OPTIONAL_NO;
    
    if(conn_ptr->phaseId != NULL)
    {
        Conn_ptr->phaseid = bsctyp_decode(DE_PhaseID, *(conn_ptr->phaseId));
        Conn_ptr->opt.Connection_opt_phaseId = MSG_OPTIONAL_YES;
    }
    else
        Conn_ptr->opt.Connection_opt_phaseId = MSG_OPTIONAL_NO;
    
    return result;
    
ERR_EXIT:

    return result;

}

/* Free routine for DF_ConnectsToList. */
int DF_ConnectsToList_free(ConnectsToList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}

/* Allocate routine for DF_ConnectsToList. */
int DF_ConnectsToList_allocate(ConnectsToList_t *list_ptr, DF_ConnectsToList_st_ptr List_ptr)
{
    int                  result = 0;
    uint8_t         point_index = 0;
    Connection_t     * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* PhaseList. */
    list_ptr->list.free = (void (*)(Connection_t *))DF_Connection_freeExt;
    
    for(point_index = 0; point_index < List_ptr->ConnectionToList_size; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if(DF_Connection_allocate(elem_ptr, &(List_ptr->ConnectionToList_data[point_index])) == 0)
            {
                /* Add point element to sequence. */
                if(asn_sequence_add(&(list_ptr->list), elem_ptr) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_Connection_free(elem_ptr);
                    FREEMEM(elem_ptr);

                    result = -ERR_SYS;
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);

                result = -ERR_SYS;
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_ConnectsToList_free(list_ptr);
    return result;
}


/* Parse routine for DF_ConnectsToList. */
int DF_ConnectsToList_parse(ConnectsToList_t *list_ptr, DF_ConnectsToList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* PhaseList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        DF_Connection_parse(list_ptr->list.array[point_index], &(List_ptr->ConnectionToList_data[point_index]));
    }
    List_ptr->ConnectionToList_size = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}




/* Free routine for DF_DDateTime. */
int DF_DDateTime_free(DDateTime_t *time_ptr)
{
    int        result = 0;
    if(time_ptr != NULL)
    {
        if(time_ptr->year != NULL)
        {
            FREEMEM(time_ptr->year);
        }

        if(time_ptr->month != NULL)
        {
            FREEMEM(time_ptr->month);
        }
        
        if(time_ptr->day != NULL)
        {
            FREEMEM(time_ptr->day);
        }
        
        if(time_ptr->hour != NULL)
        {
            FREEMEM(time_ptr->hour);
        }
        
        if(time_ptr->minute != NULL)
        {
            FREEMEM(time_ptr->minute);
        }
        
        if(time_ptr->second != NULL)
        {
            FREEMEM(time_ptr->second);
        }
        
        if(time_ptr->offset != NULL)
        {
            FREEMEM(time_ptr->offset);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(time_ptr, 0, sizeof(*time_ptr));
        
    }
    else
    {
        result = -ERR_INVAL;
    }

    return result;
}


/* Allocate routine for DF_DDateTime. */
int DF_DDateTime_allocate(DDateTime_t *time_ptr, DF_DDateTime_st_ptr DDate_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((time_ptr == NULL) || (DDate_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(time_ptr, 0, sizeof(*time_ptr));
    
    /* DYear. */
    if(DDate_ptr->opt.DDateTime_opt_DYear == MSG_OPTIONAL_YES)
    {
        if((time_ptr->year = CALLOC(1, sizeof(*time_ptr->year))) != NULL)
        {
            * time_ptr->year = bsctyp_encode(DE_DYear, DDate_ptr->year);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->year = NULL;
    }

    /* DMonth. */
    if(DDate_ptr->opt.DDateTime_opt_DMonth == MSG_OPTIONAL_YES)
    {
        if((time_ptr->month = CALLOC(1, sizeof(*time_ptr->month))) != NULL)
        {
            * time_ptr->month = bsctyp_encode(DE_DMonth, DDate_ptr->month);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->month = NULL;
    }
    
    /* DDay. */
    if(DDate_ptr->opt.DDateTime_opt_DDay == MSG_OPTIONAL_YES)
    {
        if((time_ptr->day = CALLOC(1, sizeof(*time_ptr->day))) != NULL)
        {
            * time_ptr->day = bsctyp_encode(DE_DDay, DDate_ptr->day);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->day = NULL;
    }

    /* DHour. */
    if(DDate_ptr->opt.DDateTime_opt_DHour == MSG_OPTIONAL_YES)
    {
        if((time_ptr->hour = CALLOC(1, sizeof(*time_ptr->hour))) != NULL)
        {
            * time_ptr->hour = bsctyp_encode(DE_DHour, DDate_ptr->hour);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->hour = NULL;
    }
    
    /* DMinute. */
    if(DDate_ptr->opt.DDateTime_opt_DMinute == MSG_OPTIONAL_YES)
    {
        if((time_ptr->minute = CALLOC(1, sizeof(*time_ptr->minute))) != NULL)
        {
            * time_ptr->minute = bsctyp_encode(DE_DMinute, DDate_ptr->minute);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->minute = NULL;
    }

    /* DSecond. */
    if(DDate_ptr->opt.DDateTime_opt_DSecond == MSG_OPTIONAL_YES)
    {
        if((time_ptr->second = CALLOC(1, sizeof(*time_ptr->second))) != NULL)
        {
            * time_ptr->second = bsctyp_encode(DE_DSecond, DDate_ptr->second);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->second = NULL;
    }

    /* DOffset. */
    if(DDate_ptr->opt.DDateTime_opt_DOffset == MSG_OPTIONAL_YES)
    {
        if((time_ptr->offset = CALLOC(1, sizeof(*time_ptr->offset))) != NULL)
        {
            * time_ptr->offset = bsctyp_encode(DE_DOffset, DDate_ptr->offset);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        time_ptr->offset = NULL;
    }
    
    return result;

ERR_EXIT:

    DF_DDateTime_free(time_ptr);
    return result;
}


/* Parse routine for DF_DDateTime. */
int DF_DDateTime_parse(DDateTime_t *time_ptr, DF_DDateTime_st_ptr DDate_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((time_ptr == NULL) || (DDate_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(DDate_ptr, 0, sizeof(*DDate_ptr));
    
    /* DYear. */
    if(time_ptr->year != NULL)
    {
        DDate_ptr->opt.DDateTime_opt_DYear = MSG_OPTIONAL_YES;
        DDate_ptr->year = bsctyp_decode(DE_DYear, *time_ptr->year);
    }
    
    /* DMonth. */
    if(time_ptr->month != NULL)
    {
        DDate_ptr->opt.DDateTime_opt_DMonth = MSG_OPTIONAL_YES;
        DDate_ptr->month = bsctyp_decode(DE_DMonth, *time_ptr->month);
    }
    
    /* DDay. */
    if(time_ptr->day != NULL)
    {
        DDate_ptr->opt.DDateTime_opt_DDay = MSG_OPTIONAL_YES;
        DDate_ptr->day = bsctyp_decode(DE_DDay, *time_ptr->day);
    }
    
    /* DHour. */
    if(time_ptr->hour != NULL)
    {
        DDate_ptr->opt.DDateTime_opt_DHour = MSG_OPTIONAL_YES;
        DDate_ptr->hour = bsctyp_decode(DE_DHour, *time_ptr->hour);
    }
    
    /* DMinute. */
    if(time_ptr->minute != NULL)
    {
        DDate_ptr->opt.DDateTime_opt_DMinute = MSG_OPTIONAL_YES;
        DDate_ptr->minute = bsctyp_decode(DE_DMinute, *time_ptr->minute);
    }

    /* DSecond. */
    if(time_ptr->second != NULL)
    {
        DDate_ptr->opt.DDateTime_opt_DSecond = MSG_OPTIONAL_YES;
        DDate_ptr->second = bsctyp_decode(DE_DSecond, *time_ptr->second);
    }
    
    /* DOffset. */
    if(time_ptr->offset != NULL)
    {
        DDate_ptr->opt.DDateTime_opt_DOffset = MSG_OPTIONAL_YES;
        DDate_ptr->offset = bsctyp_decode(DE_DOffset, *time_ptr->offset);
    }
    
    return result;

ERR_EXIT:

    return result;
}


/* Free routine for FullPositionVector. */
int DF_FullPositionVector_free(FullPositionVector_t *pos_ptr)
{
    int result = 0;

    if(pos_ptr != NULL)
    {
        if(pos_ptr->utcTime != NULL)
        {
            DF_DDateTime_free(pos_ptr->utcTime);
            FREEMEM(pos_ptr->utcTime);
        }

        DF_Position3D_free(&pos_ptr->pos);
        
        if(pos_ptr->heading != NULL)
        {
            FREEMEM(pos_ptr->heading);
        }
        
        if(pos_ptr->transmission != NULL)
        {
            FREEMEM(pos_ptr->transmission);
        }
        
        if(pos_ptr->speed != NULL)
        {
            FREEMEM(pos_ptr->speed);
        }
        
        if(pos_ptr->posAccuracy != NULL)
        {
            DF_PositionConfidenceSet_free(pos_ptr->posAccuracy);
            FREEMEM(pos_ptr->posAccuracy);
        }
        
        if(pos_ptr->timeConfidence != NULL)
        {
            FREEMEM(pos_ptr->timeConfidence);
        }
        
        if(pos_ptr->posConfidence != NULL)
        {   
            DF_PositionConfidenceSet_free(pos_ptr->posConfidence);
            FREEMEM(pos_ptr->posConfidence);
        }
        
        if(pos_ptr->motionCfd != NULL)
        {
            DF_MotionConfidenceSet_free(pos_ptr->motionCfd);
            FREEMEM(pos_ptr->motionCfd);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(pos_ptr, 0, sizeof(*pos_ptr));
        

    }

    return result;
}


/* Allocate routine for DF_FullPositionVector. */
int DF_FullPositionVector_allocate(FullPositionVector_t *pos_ptr, DF_FullPositionVector_st_ptr fullPos_ptr)
{
    int        result = 0;
    
    /* Error detect. */
    if((pos_ptr == NULL) || (fullPos_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ptr, 0, sizeof(*pos_ptr));
    
    /* DDateTime. */
    if(fullPos_ptr->opt.FullPositionVector_opt_DDateTime == MSG_OPTIONAL_YES)
    {
        if((pos_ptr->utcTime = CALLOC(1, sizeof(*pos_ptr->utcTime))) != NULL)
        {
            if((result = DF_DDateTime_allocate(pos_ptr->utcTime, &(fullPos_ptr->utcTime))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }    
    }
    else
    {
        pos_ptr->utcTime = NULL;
    }

    /* Positon 3D. */
    if((result = DF_Position3D_allocate(&pos_ptr->pos, &fullPos_ptr->pos)) != 0)
    {
        goto ERR_EXIT;
    }

    /* Heading. */
    if(fullPos_ptr->opt.FullPositionVector_opt_Heading == MSG_OPTIONAL_YES)
    {
        if((pos_ptr->heading = CALLOC(1, sizeof(*pos_ptr->heading))) != NULL)
        {
            * pos_ptr->heading = bsctyp_encode(DE_Heading, fullPos_ptr->heading);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->heading = NULL;
    }

    /* Transmission. */
    if(fullPos_ptr->opt.FullPositionVector_opt_Transmission == MSG_OPTIONAL_YES)
    {
        if((pos_ptr->transmission = CALLOC(1, sizeof(*pos_ptr->transmission))) != NULL)
        {
            *pos_ptr->transmission = fullPos_ptr->transmission;
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->transmission= NULL;
    }

    /* speed. */
    if(fullPos_ptr->opt.FullPositionVector_opt_Speed == MSG_OPTIONAL_YES)
    {
        if((pos_ptr->speed = CALLOC(1, sizeof(*pos_ptr->speed))) != NULL)
        {
            *pos_ptr->speed = bsctyp_encode(DE_Speed, fullPos_ptr->speed);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->speed = NULL;
    }
    
    /* PositionalAccuracy. */
    if(fullPos_ptr->opt.FullPositionVector_opt_posAccuracy == MSG_OPTIONAL_YES)
    {
        if((pos_ptr->posAccuracy = CALLOC(1, sizeof(*pos_ptr->posAccuracy))) != NULL)
        {
            if((result = DF_PositionConfidenceSet_allocate(pos_ptr->posAccuracy, &(fullPos_ptr->posAccuracy))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->posAccuracy = NULL;
    }

    /* TimeConfidence. */
    if(fullPos_ptr->opt.FullPositionVector_opt_timeConfidence == MSG_OPTIONAL_YES)
    {
        if((pos_ptr->timeConfidence = CALLOC(1, sizeof(*pos_ptr->timeConfidence))) != NULL)
        {
            * pos_ptr->timeConfidence = fullPos_ptr->timeConf;
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->timeConfidence = NULL;
    }

    /* PositionConfidenceSet. */
    if(fullPos_ptr->opt.FullPositionVector_opt_posConfidence == MSG_OPTIONAL_YES)
    {
        if((pos_ptr->posConfidence = CALLOC(1, sizeof(*pos_ptr->posConfidence))) != NULL)
        {
            if((result = DF_PositionConfidenceSet_allocate(pos_ptr->posConfidence, &(fullPos_ptr->posConfidence))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->posConfidence = NULL;
    }

    /* MotionConfidenceSet. */
    if(fullPos_ptr->opt.FullPositionVector_opt_motioncfd == MSG_OPTIONAL_YES)
    {
        if((pos_ptr->motionCfd = CALLOC(1, sizeof(*pos_ptr->motionCfd))) != NULL)
        {
            if((result = DF_MotionConfidenceSet_Allocate(pos_ptr->motionCfd , &(fullPos_ptr->motioncfd))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        pos_ptr->motionCfd = NULL;
    }
    
       return result;
ERR_EXIT:
    
    DF_FullPositionVector_free(pos_ptr);
    return result;
}


/* Parse routine for DF_FullPositionVector. */
int DF_FullPositionVector_parse(FullPositionVector_t *pos_ptr, DF_FullPositionVector_st_ptr fullPos_ptr)
{
    int     result = 0;

    
    /* Error detect. */
    if((pos_ptr == NULL) || (fullPos_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(fullPos_ptr, 0, sizeof(*fullPos_ptr));
    
    /* DDateTime. */
    if(pos_ptr->utcTime != NULL)
    {
        fullPos_ptr->opt.FullPositionVector_opt_DDateTime = MSG_OPTIONAL_YES;
        if((result = DF_DDateTime_parse(pos_ptr->utcTime, &(fullPos_ptr->utcTime))) != 0)
        {
            goto ERR_EXIT;
        }   
    }

    /*Position 3D. */
    if((result = DF_Position3D_parse(&pos_ptr->pos, &fullPos_ptr->pos)) != 0)
    {
        goto ERR_EXIT;
    }

    /* Heading. */
    if(pos_ptr->heading != NULL)
    {
        fullPos_ptr->opt.FullPositionVector_opt_Heading = MSG_OPTIONAL_YES;
        fullPos_ptr->heading = bsctyp_decode(DE_Heading, * pos_ptr->heading);
    }

    /* Transmission*/
    if(pos_ptr->transmission != NULL)
    {
        fullPos_ptr->opt.FullPositionVector_opt_Transmission = MSG_OPTIONAL_YES;
        fullPos_ptr->transmission = *pos_ptr->transmission;
    }

    /* Speed. */
    if(pos_ptr->speed != NULL)
    {
        fullPos_ptr->opt.FullPositionVector_opt_Speed = MSG_OPTIONAL_YES;
        fullPos_ptr->speed = bsctyp_decode(DE_Speed, *pos_ptr->speed);
    }
   
    
    /* PositionalAccuracy. */
    if(pos_ptr->posAccuracy != NULL)
    {
        fullPos_ptr->opt.FullPositionVector_opt_posAccuracy = MSG_OPTIONAL_YES;
        if((result = DF_PositionConfidenceSet_parse(pos_ptr->posAccuracy, &(fullPos_ptr->posAccuracy))) != 0)
        {
            goto ERR_EXIT;
        }
    }

    /* TimeConfidence. */
    if(pos_ptr->timeConfidence != NULL)
    {
        fullPos_ptr->opt.FullPositionVector_opt_timeConfidence = MSG_OPTIONAL_YES;
        fullPos_ptr->timeConf = * pos_ptr->timeConfidence;
    }


    /* PositionConfidenceSet. */
    if(pos_ptr->posConfidence != NULL)
    {
        fullPos_ptr->opt.FullPositionVector_opt_posConfidence = MSG_OPTIONAL_YES;
        if((result = DF_PositionConfidenceSet_parse(pos_ptr->posConfidence, &(fullPos_ptr->posConfidence))) != 0)
        {
            goto ERR_EXIT;
        }
    }


    /* SpeedandHeadingandThrottleConfidence. */
    if(pos_ptr->motionCfd != NULL)
    {
        fullPos_ptr->opt.FullPositionVector_opt_motioncfd = MSG_OPTIONAL_YES;
        if((result = DF_MotionConfidenceSet_parse(pos_ptr->motionCfd, &(fullPos_ptr->motioncfd))) != 0)
        {
            goto ERR_EXIT;
        }
    }
     
    result = 0;
   
ERR_EXIT:
    
    return result;
}


/* Free routine for DF_IntersectionStateList. */
int DF_IntersectionStateList_free(IntersectionStateList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_IntersectionStateList. */
int DF_IntersectionStateList_allocate(IntersectionStateList_t *list_ptr, DF_IntersectionStateList_st_ptr List_ptr)
{
    int                         result = 0;
    uint8_t             point_index = 0;
    IntersectionState_t  * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* IntersectionStateList. */
    list_ptr->list.free = (void (*)(IntersectionState_t *))DF_IntersectionState_freeExt;
    
    for(point_index = 0; point_index < List_ptr->pointNum; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if(DF_IntersectionState_allocate(elem_ptr, &(List_ptr->array[point_index])) == 0)
            {
                /* Add point element to sequence. */
                if(asn_sequence_add(&(list_ptr->list), elem_ptr) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_IntersectionState_free(elem_ptr);
                    FREEMEM(elem_ptr);

                    result = -ERR_SYS;
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);

                result = -ERR_SYS;
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_IntersectionStateList_free(list_ptr);
    return result;
}


/* Parse routine for DF_IntersectionStateList. */
int DF_IntersectionStateList_parse(IntersectionStateList_t *list_ptr, DF_IntersectionStateList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* IntersectionStateList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_IntersectionState_parse(list_ptr->list.array[point_index], &(List_ptr->array[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->pointNum = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


/* Free routine for DF_IntersectionState. */
int DF_IntersectionState_free(IntersectionState_t *iss_ptr)
{
    int    result = 0;

    
    if(iss_ptr != NULL)
    {
        /* NodeReferenceID. */
        if((result = DF_NodeReferenceID_free(&(iss_ptr->intersectionId))) != 0)
        {
            goto ERR_EXIT;
        }

        /* IntersectionStatusObject. */
        if((result = DE_IntersectionStatusObject_free(&(iss_ptr->status))) != 0)
        {
            goto ERR_EXIT;
        }
        
        /* Minute of year. */
        if(iss_ptr->moy != NULL)
        {
            FREEMEM(iss_ptr->moy);
        }

        /* DSecond. */
        if(iss_ptr->timeStamp != NULL)
        {
            FREEMEM(iss_ptr->timeStamp);
        }

        if((result = DF_PhaseList_free(&(iss_ptr->phases))) != 0)
        {
            goto ERR_EXIT;
        }
            
        /* Must clear all the zone and avoid repeat free operation. */
        memset(iss_ptr, 0, sizeof(*iss_ptr));
    }
    else
    {
        result = -ERR_INVAL;
    }

ERR_EXIT:

    return result;
}


/* Free extention routine for DF_IntersectionState. Diff with DF_IntersectionState_free() and this will free the point itself. */
int DF_IntersectionState_freeExt(IntersectionState_t *iss_ptr)
{
    if(iss_ptr != NULL)
    {
        /* Free IntersectionState point content and point itself. */
        DF_IntersectionState_free(iss_ptr);
        FREEMEM(iss_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_IntersectionState. */
int DF_IntersectionState_allocate(IntersectionState_t *iss_ptr, DF_IntersectionState_st_ptr Iss_ptr)
{
    int    result = 0;


    /* Error detect. */
    if((iss_ptr == NULL) || (Iss_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(iss_ptr, 0, sizeof(*iss_ptr));

    /* NodeReferenceID. */
    if((result = DF_NodeReferenceID_allocate(&(iss_ptr->intersectionId), &(Iss_ptr->intersectionId))) != 0)
    {
        goto ERR_EXIT;
    }

    /* IntersectionStatusObject. */
    if((result = DE_IntersectionStatusObject_allocate(&(iss_ptr->status), &(Iss_ptr->status))) != 0)
    {
        goto ERR_EXIT;
    }

    /* MinuteOfTheYear. */
    if(Iss_ptr->opt.moy == MSG_OPTIONAL_YES)
    {
        if((iss_ptr->moy = CALLOC(1, sizeof(*iss_ptr->moy))) != NULL)
        {
            *(iss_ptr->moy) = bsctyp_encode(DE_MinuteOfTheYear, Iss_ptr->moy);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        iss_ptr->moy = NULL;
    }

    /* Dsecond. */
    if(Iss_ptr->opt.timeStamp == MSG_OPTIONAL_YES)
    {
        if((iss_ptr->timeStamp = CALLOC(1, sizeof(*(iss_ptr->timeStamp)))) != NULL)
        {
            *(iss_ptr->timeStamp) = bsctyp_encode(DE_DSecond, Iss_ptr->timeStamp);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        iss_ptr->timeStamp = NULL;
    }

    /* PhaseList. */
    if((result = DF_PhaseList_allocate(&(iss_ptr->phases), &(Iss_ptr->phases))) != 0)
    {
        goto ERR_EXIT;
    }
        
    return result;

ERR_EXIT:

    DF_IntersectionState_free(iss_ptr);
    return result;
}


/* Parse routine for DF_IntersectionState. */
int DF_IntersectionState_parse(IntersectionState_t *iss_ptr, DF_IntersectionState_st_ptr Iss_ptr)
{
    int    result = 0;


    /* Error detect. */
    if((iss_ptr == NULL) || (Iss_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Iss_ptr, 0, sizeof(*Iss_ptr));

    /* NodeReferenceID. */
    if((result = DF_NodeReferenceID_parse(&(iss_ptr->intersectionId), &(Iss_ptr->intersectionId))) != 0)
    {
        goto ERR_EXIT;
    }

    /* IntersectionStatusObject. */
    if((result = DE_IntersectionStatusObject_parse(&(iss_ptr->status), &(Iss_ptr->status))) != 0)
    {
        goto ERR_EXIT;
    }

    /* Minute of year. */
    if(iss_ptr->moy != NULL)
    {
        Iss_ptr->moy = bsctyp_decode(DE_MinuteOfTheYear, *(iss_ptr->moy));
        Iss_ptr->opt.moy = MSG_OPTIONAL_YES;
    }

    /* DSecond. */
    if(iss_ptr->timeStamp != NULL)
    {
        Iss_ptr->timeStamp = bsctyp_decode(DE_DSecond, *(iss_ptr->timeStamp));
        Iss_ptr->opt.timeStamp = MSG_OPTIONAL_YES;
    }

    /* PhaseList. */
    if((result = DF_PhaseList_parse(&(iss_ptr->phases), &(Iss_ptr->phases))) != 0)
    {
        goto ERR_EXIT;
    }

ERR_EXIT:
    
    return result;
}

/* Free routine for DF_LaneTypeAttributes. */
int DF_LaneTypeAttributes_free(LaneTypeAttributes_t *attr_ptr)
{
    int        result = 0;
    if(attr_ptr != NULL)
    {
        switch(attr_ptr->present)
        {
            case LaneTypeAttributes_PR_NOTHING:
                break;
            case LaneTypeAttributes_PR_vehicle:
                result = DE_LaneAttributes_Vehicle_free(&(attr_ptr->choice.vehicle));
                break;
            case LaneTypeAttributes_PR_crosswalk:
                result = DE_LaneAttributes_Crosswalk_free(&(attr_ptr->choice.crosswalk));
                break;
            case LaneTypeAttributes_PR_bikeLane:
                result = DE_LaneAttributes_Bike_free(&(attr_ptr->choice.bikeLane));
                break;
            case LaneTypeAttributes_PR_sidewalk:
                result = DE_LaneAttributes_Sidewalk_free(&(attr_ptr->choice.sidewalk));
                break;
            case LaneTypeAttributes_PR_median:
                result = DE_LaneAttributes_Barrier_free(&(attr_ptr->choice.median));
                break;
            case LaneTypeAttributes_PR_striping:
                result = DE_LaneAttributes_Striping_free(&(attr_ptr->choice.striping));
                break;
            case LaneTypeAttributes_PR_trackedVehicle:
                result = DE_LaneAttributes_TrackedVehicle_free(&(attr_ptr->choice.trackedVehicle));
                break;
            case LaneTypeAttributes_PR_parking:
                result = DE_LaneAttributes_Parking_free(&(attr_ptr->choice.parking));
                break;
            default:
                break;
        }

        if(result)
            goto ERR_EXIT;
        
        /* Must clear all the zone and avoid repeat free operation. */
        memset(attr_ptr, 0, sizeof(*attr_ptr));
        
    }
    else
    {
        result = -ERR_INVAL;
    }
    
ERR_EXIT:
    return result;
}


/* Allocate routine for DF_LaneTypeAttributes. */
int DF_LaneTypeAttributes_allocate(LaneTypeAttributes_t *attr_ptr, DF_LaneTypeAttributes_st_ptr Attr_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((attr_ptr == NULL) || (Attr_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(attr_ptr, 0, sizeof(*attr_ptr));
    
    switch(Attr_ptr->present)
    {
        case LaneTypeAttributes_PR_NOTHING:
            break;
        case LaneTypeAttributes_PR_vehicle:
            result = DE_LaneAttributes_Vehicle_allocate(&(attr_ptr->choice.vehicle), &(Attr_ptr->choice.vehicle));
            break;
        case LaneTypeAttributes_PR_crosswalk:
            result = DE_LaneAttributes_Crosswalk_allocate(&(attr_ptr->choice.crosswalk), &(Attr_ptr->choice.crosswalk));
            break;
        case LaneTypeAttributes_PR_bikeLane:
            result = DE_LaneAttributes_Bike_allocate(&(attr_ptr->choice.bikeLane), &(Attr_ptr->choice.bikeLane));
            break;
        case LaneTypeAttributes_PR_sidewalk:
            result = DE_LaneAttributes_Sidewalk_allocate(&(attr_ptr->choice.sidewalk), &(Attr_ptr->choice.sidewalk));
            break;
        case LaneTypeAttributes_PR_median:
            result = DE_LaneAttributes_Barrier_allocate(&(attr_ptr->choice.median), &(Attr_ptr->choice.median));
            break;
        case LaneTypeAttributes_PR_striping:
            result = DE_LaneAttributes_Striping_allocate(&(attr_ptr->choice.striping), &(Attr_ptr->choice.striping));
            break;
        case LaneTypeAttributes_PR_trackedVehicle:
            result = DE_LaneAttributes_TrackedVehicle_allocate(&(attr_ptr->choice.trackedVehicle), &(Attr_ptr->choice.trackedvehicle));
            break;
        case LaneTypeAttributes_PR_parking:
            result = DE_LaneAttributes_Parking_allocate(&(attr_ptr->choice.parking), &(Attr_ptr->choice.parking));
            break;
        default:
            break;
    }

    if(result)
        goto ERR_EXIT;
    
    return result;

ERR_EXIT:

    DF_LaneTypeAttributes_free(attr_ptr);
    return result;
}


/* Parse routine for DF_LaneTypeAttributes. */
int DF_LaneTypeAttributes_parse(LaneTypeAttributes_t *attr_ptr, DF_LaneTypeAttributes_st_ptr Attr_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((attr_ptr == NULL) || (Attr_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(Attr_ptr, 0, sizeof(*Attr_ptr));
    
    switch(Attr_ptr->present)
    {
        case LaneTypeAttributes_PR_NOTHING:
            break;
        case LaneTypeAttributes_PR_vehicle:
            result = DE_LaneAttributes_Vehicle_parse(&(attr_ptr->choice.vehicle), &(Attr_ptr->choice.vehicle));
            break;
        case LaneTypeAttributes_PR_crosswalk:
            result = DE_LaneAttributes_Crosswalk_parse(&(attr_ptr->choice.crosswalk), &(Attr_ptr->choice.crosswalk));
            break;
        case LaneTypeAttributes_PR_bikeLane:
            result = DE_LaneAttributes_Bike_parse(&(attr_ptr->choice.bikeLane), &(Attr_ptr->choice.bikeLane));
            break;
        case LaneTypeAttributes_PR_sidewalk:
            result = DE_LaneAttributes_Sidewalk_parse(&(attr_ptr->choice.sidewalk), &(Attr_ptr->choice.sidewalk));
            break;
        case LaneTypeAttributes_PR_median:
            result = DE_LaneAttributes_Barrier_parse(&(attr_ptr->choice.median), &(Attr_ptr->choice.median));
            break;
        case LaneTypeAttributes_PR_striping:
            result = DE_LaneAttributes_Striping_parse(&(attr_ptr->choice.striping), &(Attr_ptr->choice.striping));
            break;
        case LaneTypeAttributes_PR_trackedVehicle:
            result = DE_LaneAttributes_TrackedVehicle_parse(&(attr_ptr->choice.trackedVehicle), &(Attr_ptr->choice.trackedvehicle));
            break;
        case LaneTypeAttributes_PR_parking:
            result = DE_LaneAttributes_Parking_parse(&(attr_ptr->choice.parking), &(Attr_ptr->choice.parking));
            break;
        default:
            break;
    }

    if(result)
        goto ERR_EXIT;
    
    return result;

ERR_EXIT:

    return result;
}

/* Free routine for DF_LaneAttributes. */
int DF_LaneAttributes_free(LaneAttributes_t *attr_ptr)
{
    int result = 0;
    
    if(attr_ptr != NULL)
    {
        if(attr_ptr->shareWith != NULL)
        {
            FREEMEM(attr_ptr->shareWith);
        }

        result = DF_LaneTypeAttributes_free(&(attr_ptr->laneType));
        if(result)
            return result;

        memset(attr_ptr, 0x00, sizeof(*attr_ptr));
    }

    return result;
}


/* Allocate routine for DF_LaneAttributes.*/
int DF_LaneAttributes_Allocate(LaneAttributes_t *attr_ptr, DF_LaneAttributes_st_ptr Attr_ptr)
{
    int result = 0;

    if((attr_ptr == NULL) || (Attr_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(attr_ptr, 0x00, sizeof(*attr_ptr));
        
    if(Attr_ptr->opt.LaneAttributes_opt_sharewith == MSG_OPTIONAL_YES)
    {
        attr_ptr->shareWith = CALLOC(1, sizeof(*(attr_ptr->shareWith)));
        if(attr_ptr->shareWith == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        result = DE_LaneSharing_allocate(attr_ptr->shareWith, &(Attr_ptr->shareWith));
        if(result)
            goto ERR_EXIT;
    }else{
        attr_ptr->shareWith = NULL;
    }

    result = DF_LaneTypeAttributes_allocate(&(attr_ptr->laneType), &(Attr_ptr->laneType));
    if(result)
        goto ERR_EXIT;

    return result;
ERR_EXIT:
    
    DF_LaneAttributes_free(attr_ptr);
    
    return result;
}


/* Parse routine for DF_LaneAttributes. */
int DF_LaneAttributes_parse(LaneAttributes_t *attr_ptr, DF_LaneAttributes_st_ptr Attr_ptr)
{
    int result = 0;
    
    if((attr_ptr == NULL) || (Attr_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* clear  Attr_ptr memory */
    memset(Attr_ptr, 0x00, sizeof(*Attr_ptr));
    
    if(attr_ptr->shareWith != NULL)
    {
        result = DE_LaneSharing_parse(attr_ptr->shareWith, &(Attr_ptr->shareWith));
        if(result)
            goto ERR_EXIT;
        
        Attr_ptr->opt.LaneAttributes_opt_sharewith = MSG_OPTIONAL_YES;
    }
    else
        Attr_ptr->opt.LaneAttributes_opt_sharewith = MSG_OPTIONAL_NO;

    result = DF_LaneTypeAttributes_parse(&(attr_ptr->laneType), &(Attr_ptr->laneType));
    if(result)
            goto ERR_EXIT;
    
ERR_EXIT:

    return result;
}

/* Free routine for DF_Lane. */
int DF_Lane_free(Lane_t *lane_ptr)
{
    int result = 0;
    
    if(lane_ptr != NULL)
    {
        if(lane_ptr->laneAttributes != NULL)
        {
            result = DF_LaneAttributes_free(lane_ptr->laneAttributes);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(lane_ptr->laneAttributes);
        }

        if(lane_ptr->maneuvers != NULL)
        {
            result = DE_AllowedManeuvers_free(lane_ptr->maneuvers);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(lane_ptr->maneuvers);
        }

        if(lane_ptr->connectsTo != NULL)
        {
            result = DF_ConnectsToList_free(lane_ptr->connectsTo);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(lane_ptr->connectsTo);
        }

        if(lane_ptr->speedLimits != NULL)
        {
            result = DF_SpeedLimitList_free(lane_ptr->speedLimits);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(lane_ptr->speedLimits);
        }

        if(lane_ptr->points != NULL)
        {
            result = DF_PointList_free(lane_ptr->points);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(lane_ptr->points);
        }
        
        memset(lane_ptr, 0x00, sizeof(*lane_ptr));
    }

ERR_EXIT:
    return result;
}

/* Free extention routine for DF_Lane. Diff with DF_Lane_free() and this will free the point itself. */
int DF_Lane_freeExt(Lane_t *lane_ptr)
{
    if(lane_ptr != NULL)
    {
        /* Free movement point content and point itself. */
        DF_Lane_free(lane_ptr);
        FREEMEM(lane_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_Lane.*/
int DF_Lane_Allocate(Lane_t *lane_ptr, DF_Lane_st_ptr Lane_ptr)
{
    int result = 0;

    if((lane_ptr == NULL) || (Lane_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(lane_ptr, 0x00, sizeof(*lane_ptr));

    lane_ptr->laneID = bsctyp_encode(DE_LaneID, Lane_ptr->laneID);
        
    if(Lane_ptr->opt.Lane_opt_lanAttributes == MSG_OPTIONAL_YES)
    {
        lane_ptr->laneAttributes = CALLOC(1, sizeof(*(lane_ptr->laneAttributes)));
        if(lane_ptr->laneAttributes == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        result = DF_LaneAttributes_Allocate(lane_ptr->laneAttributes, &(Lane_ptr->laneAttributes));
        if(result)
            goto ERR_EXIT;
    }else{
        lane_ptr->laneAttributes = NULL;
    }

    if(Lane_ptr->opt.Lane_opt_maneuvers == MSG_OPTIONAL_YES)
    {
        lane_ptr->maneuvers = CALLOC(1, sizeof(*(lane_ptr->maneuvers)));
        if(lane_ptr->maneuvers == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        result = DE_AllowedManeuvers_allocate(lane_ptr->maneuvers, &(Lane_ptr->maneuvers));
        if(result)
            goto ERR_EXIT;
    }else{
        lane_ptr->maneuvers = NULL;
    }

    if(Lane_ptr->opt.Lane_opt_connectsTo == MSG_OPTIONAL_YES)
    {
        lane_ptr->connectsTo = CALLOC(1, sizeof(*(lane_ptr->connectsTo)));
        if(lane_ptr->connectsTo == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        result = DF_ConnectsToList_allocate(lane_ptr->connectsTo, &(Lane_ptr->connectsTo));
        if(result)
            goto ERR_EXIT;
    }else{
        lane_ptr->connectsTo = NULL;
    }

    if(Lane_ptr->opt.Lane_opt_speedLimits == MSG_OPTIONAL_YES)
    {
        lane_ptr->speedLimits = CALLOC(1, sizeof(*(lane_ptr->speedLimits)));
        if(lane_ptr->speedLimits == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        result = DF_SpeedLimitList_allocate(lane_ptr->speedLimits, &(Lane_ptr->speedLimits));
        if(result)
            goto ERR_EXIT;
    }else{
        lane_ptr->speedLimits = NULL;
    }

    if(Lane_ptr->opt.Lane_opt_points == MSG_OPTIONAL_YES)
    {
        lane_ptr->points = CALLOC(1, sizeof(*(lane_ptr->points)));
        if(lane_ptr->points == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        result = DF_PointList_allocate(lane_ptr->points, &(Lane_ptr->points));
        if(result)
            goto ERR_EXIT;
    }else{
        lane_ptr->points = NULL;
    }

    return result;
ERR_EXIT:
    
    DF_Lane_free(lane_ptr);
    
    return result;
}


/* Parse routine for DF_Lane. */
int DF_Lane_parse(Lane_t *lane_ptr, DF_Lane_st_ptr Lane_ptr)
{
    int result = 0;
    
    if((lane_ptr == NULL) || (Lane_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* clear  Attr_ptr memory */
    memset(Lane_ptr, 0x00, sizeof(*Lane_ptr));

    Lane_ptr->laneID = bsctyp_decode(DE_LaneID,lane_ptr->laneID);
    
    if(lane_ptr->laneAttributes != NULL)
    {
        result = DF_LaneAttributes_parse(lane_ptr->laneAttributes, &(Lane_ptr->laneAttributes));
        if(result)
            goto ERR_EXIT;
        
        Lane_ptr->opt.Lane_opt_lanAttributes = MSG_OPTIONAL_YES;
    }
    else
        Lane_ptr->opt.Lane_opt_lanAttributes = MSG_OPTIONAL_NO;

    if(lane_ptr->maneuvers != NULL)
    {
        result = DE_AllowedManeuvers_parse(lane_ptr->maneuvers, &(Lane_ptr->maneuvers));
        if(result)
            goto ERR_EXIT;
        
        Lane_ptr->opt.Lane_opt_maneuvers = MSG_OPTIONAL_YES;
    }
    else
        Lane_ptr->opt.Lane_opt_maneuvers = MSG_OPTIONAL_NO;

    if(lane_ptr->connectsTo != NULL)
    {
        result = DF_ConnectsToList_parse(lane_ptr->connectsTo, &(Lane_ptr->connectsTo));
        if(result)
            goto ERR_EXIT;
        
        Lane_ptr->opt.Lane_opt_connectsTo = MSG_OPTIONAL_YES;
    }
    else
        Lane_ptr->opt.Lane_opt_connectsTo = MSG_OPTIONAL_NO;

    if(lane_ptr->speedLimits != NULL)
    {
        result = DF_SpeedLimitList_parse(lane_ptr->speedLimits, &(Lane_ptr->speedLimits));
        if(result)
            goto ERR_EXIT;
        
        Lane_ptr->opt.Lane_opt_speedLimits = MSG_OPTIONAL_YES;
    }
    else
        Lane_ptr->opt.Lane_opt_speedLimits = MSG_OPTIONAL_NO;

    if(lane_ptr->points != NULL)
    {
        result = DF_PointList_parse(lane_ptr->points, &(Lane_ptr->points));
        if(result)
            goto ERR_EXIT;
        
        Lane_ptr->opt.Lane_opt_points = MSG_OPTIONAL_YES;
    }
    else
        Lane_ptr->opt.Lane_opt_points = MSG_OPTIONAL_NO;
    
ERR_EXIT:

    return result;
}

/* Free routine for DF_LaneList. */
int DF_LaneList_free(LaneList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_LaneList. */
int DF_LaneList_allocate(LaneList_t *list_ptr, DF_LaneList_st_ptr List_ptr)
{
    int           result = 0;
    uint8_t  point_index = 0;
    Lane_t    * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* ParticipantList. */
    list_ptr->list.free = (void (*)(Lane_t *))DF_Lane_freeExt;
    
    for(point_index = 0; point_index < List_ptr->LaneList_size; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if((result = DF_Lane_Allocate(elem_ptr, &(List_ptr->laneList_data[point_index]))) == 0)
            {
                /* Add point element to sequence. */
                if((result = asn_sequence_add(&(list_ptr->list), elem_ptr)) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_Lane_free(elem_ptr);
                    FREEMEM(elem_ptr);
 
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);
                
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_LaneList_free(list_ptr);
    return result;
}


/* Parse routine for DF_LaneList. */
int DF_LaneList_parse(LaneList_t *list_ptr, DF_LaneList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* ParticipantList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_Lane_parse(list_ptr->list.array[point_index], &(List_ptr->laneList_data[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->LaneList_size = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}

/* Free routine for DF_Link. */
int DF_Link_free(Link_t *link_ptr)
{
    int result = 0;

    if(link_ptr != NULL)
    {
        if(link_ptr->name != NULL)
        {
            result = DE_DescriptiveName_free(link_ptr->name);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(link_ptr->name);
        }

        result = DF_NodeReferenceID_free(&(link_ptr->upstreamNodeId));
        if(result)
            goto ERR_EXIT;
        
        if(link_ptr->speedLimits != NULL)
        {
            result = DF_SpeedLimitList_free(link_ptr->speedLimits);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(link_ptr->speedLimits);
        }

        if(link_ptr->points != NULL)
        {
            result = DF_PointList_free(link_ptr->points);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(link_ptr->points);
        }

        if(link_ptr->movements != NULL)
        {
            result = DF_MovementList_free(link_ptr->movements);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(link_ptr->movements);
        }

        result = DF_LaneList_free(&(link_ptr->lanes));
        if(result)
            goto ERR_EXIT;

        memset(link_ptr, 0x00, sizeof(*link_ptr));
    }
    else
    {
        result = -ERR_INVAL;
    }
ERR_EXIT:
    return result;
}

/* Free extention routine for DF_Link. Diff with DF_Link_free() and this will free the point itself. */
int DF_Link_freeExt(Link_t *link_ptr)
{
    if(link_ptr != NULL)
    {
        /* Free movement point content and point itself. */
        DF_Link_free(link_ptr);
        FREEMEM(link_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_Link.*/
int DF_Link_allocate(Link_t *link_ptr, DF_Link_st_ptr Link_ptr)
{
    int result = 0;


    /* Error detection. */
    if((link_ptr == NULL) || (Link_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(link_ptr, 0x00, sizeof(*link_ptr));

    /*name*/
    if(Link_ptr->opt.Link_opt_name == MSG_OPTIONAL_YES)
    {
        if((link_ptr->name = CALLOC(1, sizeof(*(link_ptr->name)))) != NULL)
        {
             result = DE_DescriptiveName_allocate(link_ptr->name, &(Link_ptr->name));
             if(result)
                 goto ERR_EXIT;
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        link_ptr->name = NULL;
    }
    
    /*upstreamNodeId*/
    result = DF_NodeReferenceID_allocate(&(link_ptr->upstreamNodeId), &(Link_ptr->upstreamNodeId));
    if(result)
        goto ERR_EXIT;

    /*speedLimits*/
    if(Link_ptr->opt.Link_opt_speedLimits == MSG_OPTIONAL_YES)
    {
        if((link_ptr->speedLimits = CALLOC(1, sizeof(*(link_ptr->speedLimits)))) != NULL)
        {
             result = DF_SpeedLimitList_allocate(link_ptr->speedLimits, &(Link_ptr->speedLimits));
             if(result)
                 goto ERR_EXIT;
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        link_ptr->speedLimits = NULL;
    }
    
    /*laneWidth*/
    link_ptr->laneWidth = bsctyp_encode(DE_LaneWidth, Link_ptr->laneWidth);

    /*points*/
    if(Link_ptr->opt.Link_opt_points == MSG_OPTIONAL_YES)
    {
        if((link_ptr->points = CALLOC(1, sizeof(*(link_ptr->points)))) != NULL)
        {
             result = DF_PointList_allocate(link_ptr->points, &(Link_ptr->points));
             if(result)
                 goto ERR_EXIT;
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        link_ptr->points = NULL;
    }

    /*movements*/
    if(Link_ptr->opt.Link_opt_movements == MSG_OPTIONAL_YES)
    {
        if((link_ptr->movements = CALLOC(1, sizeof(*(link_ptr->movements)))) != NULL)
        {
             result = DF_MovementList_allocate(link_ptr->movements, &(Link_ptr->movements));
             if(result)
                 goto ERR_EXIT;
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        link_ptr->movements = NULL;
    }

    /*lanes*/
    result = DF_LaneList_allocate(&(link_ptr->lanes), &(Link_ptr->lanes));
    if(result)
        goto ERR_EXIT;
    
    return result;
    
ERR_EXIT:
    
    DF_Link_free(link_ptr);
    return result;
}


/* Parse routine for DF_Link. */
int DF_Link_parse(Link_t *link_ptr, DF_Link_st_ptr Link_ptr)
{
    int result = 0;
   
    /* Error detection. */
    if((link_ptr == NULL) || (Link_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* clear  Movement_ptr memory */
    memset(Link_ptr, 0x00, sizeof(*Link_ptr));

    /* name. */
    if(link_ptr->name != NULL)
    {
        result = DE_DescriptiveName_parse(link_ptr->name, &(Link_ptr->name));
        if(result)
            goto ERR_EXIT;
        
        Link_ptr->opt.Link_opt_name = MSG_OPTIONAL_YES;
    }
    else
    {
        Link_ptr->opt.Link_opt_name = MSG_OPTIONAL_NO;
    }

    /*upstreamNodeId*/
    result = DF_NodeReferenceID_parse(&(link_ptr->upstreamNodeId), &(Link_ptr->upstreamNodeId));
    if(result)
        goto ERR_EXIT;

    /*speedLimits. */
    if(link_ptr->speedLimits != NULL)
    {
        result = DF_SpeedLimitList_parse(link_ptr->speedLimits, &(Link_ptr->speedLimits));
        if(result)
            goto ERR_EXIT;
        
        Link_ptr->opt.Link_opt_speedLimits = MSG_OPTIONAL_YES;
    }
    else
    {
        Link_ptr->opt.Link_opt_speedLimits = MSG_OPTIONAL_NO;
    }
    
    /*laneWidth*/
    Link_ptr->laneWidth = bsctyp_decode(DE_LaneWidth, link_ptr->laneWidth);

    /*points. */
    if(link_ptr->points != NULL)
    {
        result = DF_PointList_parse(link_ptr->points, &(Link_ptr->points));
        if(result)
            goto ERR_EXIT;
        
        Link_ptr->opt.Link_opt_points = MSG_OPTIONAL_YES;
    }
    else
    {
        Link_ptr->opt.Link_opt_points = MSG_OPTIONAL_NO;
    }

    /*movements. */
    if(link_ptr->movements != NULL)
    {
        result = DF_MovementList_parse(link_ptr->movements, &(Link_ptr->movements));
        if(result)
            goto ERR_EXIT;
        
        Link_ptr->opt.Link_opt_movements = MSG_OPTIONAL_YES;
    }
    else
    {
        Link_ptr->opt.Link_opt_movements = MSG_OPTIONAL_NO;
    }

    /*lanes*/
    result = DF_LaneList_parse(&(link_ptr->lanes), &(Link_ptr->lanes));
    if(result)
        goto ERR_EXIT;
    
ERR_EXIT:

    return result;
}

/* Free routine for DF_LinkList. */
int DF_LinkList_free(LinkList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_LinkList. */
int DF_LinkList_allocate(LinkList_t *list_ptr, DF_LinkList_st_ptr List_ptr)
{
    int           result = 0;
    uint8_t  point_index = 0;
    Link_t    * elem_ptr = NULL;

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* ParticipantList. */
    list_ptr->list.free = (void (*)(Link_t *))DF_Link_freeExt;
    
    for(point_index = 0; point_index < List_ptr->LinkList_size; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if((result = DF_Link_allocate(elem_ptr, &(List_ptr->LinkList_data[point_index]))) == 0)
            {
                /* Add point element to sequence. */
                if((result = asn_sequence_add(&(list_ptr->list), elem_ptr)) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_Link_free(elem_ptr);
                    FREEMEM(elem_ptr);
 
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);
                
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_LinkList_free(list_ptr);
    return result;
}


/* Parse routine for DF_LinkList. */
int DF_LinkList_parse(LinkList_t *list_ptr, DF_LinkList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* ParticipantList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_Link_parse(list_ptr->list.array[point_index], &(List_ptr->LinkList_data[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->LinkList_size = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


/* Free routine for DF_MotionConfidenceSet. */
int DF_MotionConfidenceSet_free(MotionConfidenceSet_t *motion_ptr)
{
    int result = 0;
    
    if(motion_ptr != NULL)
    {
        if(motion_ptr->speedCfd != NULL)
        {
            FREEMEM(motion_ptr->speedCfd);
        }

        if(motion_ptr->headingCfd != NULL)
        {
            FREEMEM(motion_ptr->headingCfd);
        }

        if(motion_ptr->steerCfd != NULL)
        {
            FREEMEM(motion_ptr->steerCfd);
        }

        memset(motion_ptr, 0x00, sizeof(*motion_ptr));
    }

    return result;
}


/* Allocate routine for DF_MotionConfidenceSet.*/
int DF_MotionConfidenceSet_Allocate(MotionConfidenceSet_t *motion_ptr, DF_MotionConfidenceSet_st_ptr Motion_ptr)
{
    int result = 0;

    if((motion_ptr == NULL) || (Motion_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(motion_ptr, 0x00, sizeof(*motion_ptr));
        
    if(Motion_ptr->opt.MotionConfidenceSet_opt_speedcfd == MSG_OPTIONAL_YES)
    {
        motion_ptr->speedCfd = CALLOC(1, sizeof(*motion_ptr->speedCfd));
        if(motion_ptr->speedCfd == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *motion_ptr->speedCfd = Motion_ptr->speedcfd;
    }else{
        motion_ptr->speedCfd = NULL;
    }

    if(Motion_ptr->opt.MotionConfidenceSet_opt_headingcfd == MSG_OPTIONAL_YES)
    {
        motion_ptr->headingCfd = CALLOC(1, sizeof(*motion_ptr->headingCfd));
        if(motion_ptr->headingCfd == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *motion_ptr->headingCfd = Motion_ptr->headingcfd;
    }else{
        motion_ptr->headingCfd = NULL;
    }

    if(Motion_ptr->opt.MotionConfidenceSet_opt_steercfd == MSG_OPTIONAL_YES)
    {
        motion_ptr->steerCfd = CALLOC(1, sizeof(*motion_ptr->steerCfd));
        if(motion_ptr->steerCfd == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *motion_ptr->steerCfd = Motion_ptr->steercfd;
    }else{
        motion_ptr->steerCfd = NULL;
    }

    return result;
ERR_EXIT:
    
    DF_MotionConfidenceSet_free(motion_ptr);
    
    return result;
}


/* Parse routine for DF_MotionConfidenceSet. */
int DF_MotionConfidenceSet_parse(MotionConfidenceSet_t *motion_ptr, DF_MotionConfidenceSet_st_ptr Motion_ptr)
{
    int result = 0;
    
    if((motion_ptr == NULL) || (Motion_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* clear  Motion_ptr memory */
    memset(Motion_ptr, 0x00, sizeof(DF_MotionConfidenceSet_st));
    
    if(motion_ptr->speedCfd != NULL)
    {
        Motion_ptr->opt.MotionConfidenceSet_opt_speedcfd = MSG_OPTIONAL_YES;
        Motion_ptr->speedcfd = *motion_ptr->speedCfd;
    }

    if(motion_ptr->headingCfd != NULL)
    {
        Motion_ptr->opt.MotionConfidenceSet_opt_headingcfd = MSG_OPTIONAL_YES;
        Motion_ptr->headingcfd = *motion_ptr->headingCfd;
    }

    if(motion_ptr->steerCfd != NULL)
    {
        Motion_ptr->opt.MotionConfidenceSet_opt_steercfd = MSG_OPTIONAL_YES;
        Motion_ptr->steercfd = *motion_ptr->steerCfd;
    }
    
ERR_EXIT:

    return result;
}

/* Free routine for DF_Movement. */
int DF_Movement_free(Movement_t *movement_ptr)
{
    int result = 0;

    if(movement_ptr != NULL)
    {
        result = DF_NodeReferenceID_free(&(movement_ptr->remoteIntersection));
        if(result)
            return result;
        
        if(movement_ptr->phaseId != NULL)
        {
            FREEMEM(movement_ptr->phaseId);
        }

        memset(movement_ptr, 0x00, sizeof(*movement_ptr));
    }
    else
    {
        result = -ERR_INVAL;
    }

    return result;
}

/* Free extention routine for DF_Movement. Diff with DF_Movement_free() and this will free the point itself. */
int DF_Movement_freeExt(Movement_t *movement_ptr)
{
    if(movement_ptr != NULL)
    {
        /* Free movement point content and point itself. */
        DF_Movement_free(movement_ptr);
        FREEMEM(movement_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_Movement.*/
int DF_Movement_allocate(Movement_t *movement_ptr, DF_Movement_st_ptr Movement_ptr)
{
    int result = 0;


    /* Error detection. */
    if((movement_ptr == NULL) || (Movement_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(movement_ptr, 0x00, sizeof(*movement_ptr));

    /* remoteIntersection */
    result = DF_NodeReferenceID_allocate(&(movement_ptr->remoteIntersection),&(Movement_ptr->remoteIntersection));
    if(result)
        goto ERR_EXIT;

    /* phaseid. */
    if(Movement_ptr->opt.Movement_opt_phaseid == MSG_OPTIONAL_YES)
    {
        if((movement_ptr->phaseId = CALLOC(1, sizeof(*(movement_ptr->phaseId)))) != NULL)
        {
             *(movement_ptr->phaseId) = bsctyp_encode(DE_PhaseID, Movement_ptr->phaseid);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        movement_ptr->phaseId = NULL;
    }

    return result;
    
ERR_EXIT:
    
    DF_Movement_free(movement_ptr);
    return result;
}


/* Parse routine for DF_Movement. */
int DF_Movement_parse(Movement_t *movement_ptr, DF_Movement_st_ptr Movement_ptr)
{
    int result = 0;
   
    /* Error detection. */
    if((movement_ptr == NULL) || (Movement_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* clear  Movement_ptr memory */
    memset(Movement_ptr, 0x00, sizeof(*Movement_ptr));

    /*remoteIntersection*/
    result = DF_NodeReferenceID_parse(&(movement_ptr->remoteIntersection), &(Movement_ptr->remoteIntersection));
    if(result)
        goto ERR_EXIT;

    /* phaseId. */
    if(movement_ptr->phaseId != NULL)
    {
        Movement_ptr->phaseid = bsctyp_decode(DE_PhaseID, *(movement_ptr->phaseId));
        Movement_ptr->opt.Movement_opt_phaseid = MSG_OPTIONAL_YES;
    }
    else
    {
        Movement_ptr->opt.Movement_opt_phaseid = MSG_OPTIONAL_NO;
    }
    
ERR_EXIT:

    return result;
}

/* Free routine for DF_MovementList. */
int DF_MovementList_free(MovementList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_MovementList. */
int DF_MovementList_allocate(MovementList_t *list_ptr, DF_MovementList_st_ptr List_ptr)
{
    int                 result = 0;
    uint8_t     point_index = 0;
    Movement_t   * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* IntersectionStateList. */
    list_ptr->list.free = (void (*)(Movement_t *))DF_Movement_freeExt;
    
    for(point_index = 0; point_index < List_ptr->MovementList_size; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if(DF_Movement_allocate(elem_ptr, &(List_ptr->MovementList_data[point_index])) == 0)
            {
                /* Add point element to sequence. */
                if(asn_sequence_add(&(list_ptr->list), elem_ptr) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_Movement_free(elem_ptr);
                    FREEMEM(elem_ptr);

                    result = -ERR_SYS;
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);

                result = -ERR_SYS;
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_MovementList_free(list_ptr);
    return result;
}


/* Parse routine for DF_MovementList. */
int DF_MovementList_parse(MovementList_t *list_ptr, DF_MovementList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* MovementList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_Movement_parse(list_ptr->list.array[point_index], &(List_ptr->MovementList_data[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->MovementList_size = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


/* Free routine for DF_NodeReferenceID. */
int DF_NodeReferenceID_free(NodeReferenceID_t *id_ptr)
{
    int result = 0;

    
    if(id_ptr != NULL)
    {
        if(id_ptr->region != NULL)
        {
            FREEMEM(id_ptr->region);
        }

        memset(id_ptr, 0x00, sizeof(*id_ptr));
    }
    else
    {
        result = -ERR_INVAL;
    }

    return result;
}


/* Allocate routine for DF_NodeReferenceID.*/
int DF_NodeReferenceID_allocate(NodeReferenceID_t *id_ptr, DF_NodeReferenceID_st_ptr ID_ptr)
{
    int result = 0;


    /* Error detection. */
    if((id_ptr == NULL) || (ID_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(id_ptr, 0x00, sizeof(*id_ptr));

    /* Road regulator id. */
    if(ID_ptr->opt.region == MSG_OPTIONAL_YES)
    {
        if((id_ptr->region = CALLOC(1, sizeof(*id_ptr->region))) != NULL)
        {
             *(id_ptr->region) = bsctyp_encode(DE_RoadRegulatorID, ID_ptr->region);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        id_ptr->region = NULL;
    }

    /* Node id. */
    id_ptr->id = ID_ptr->id;

    return result;
    
ERR_EXIT:
    
    DF_NodeReferenceID_free(id_ptr);
    return result;
}


/* Parse routine for DF_NodeReferenceID. */
int DF_NodeReferenceID_parse(NodeReferenceID_t *id_ptr, DF_NodeReferenceID_st_ptr ID_ptr)
{
    int result = 0;

    
    /* Error detection. */
    if((id_ptr == NULL) || (ID_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* clear  Motion_ptr memory */
    memset(ID_ptr, 0x00, sizeof(*ID_ptr));

    /* Road regulator id. */
    if(id_ptr->region != NULL)
    {
        ID_ptr->region = bsctyp_decode(DE_RoadRegulatorID, *(id_ptr->region));
        ID_ptr->opt.region = MSG_OPTIONAL_YES;
    }
    else
    {
        ID_ptr->opt.region = MSG_OPTIONAL_NO;
    }

    /* Node id. */
    ID_ptr->id = id_ptr->id;
    
ERR_EXIT:

    return result;
}

/* Free routine for DF_Node. */
int DF_Node_free(Node_t *node_ptr)
{
    int result = 0;

    if(node_ptr != NULL)
    {
        /* name. */
        if(node_ptr->name != NULL)
        {
            result = DE_DescriptiveName_free(node_ptr->name);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(node_ptr->name);
        }

        /*id*/
        result = DF_NodeReferenceID_free(&(node_ptr->id));
        if(result)
            goto ERR_EXIT;

        /*refPos*/
        result = DF_Position3D_free(&(node_ptr->refPos));
        if(result)
            goto ERR_EXIT;

        /* inLinks. */
        if(node_ptr->inLinks != NULL)
        {
            result = DF_LinkList_free(node_ptr->inLinks);
            if(result)
                goto ERR_EXIT;
            
            FREEMEM(node_ptr->inLinks);
        }
        
        memset(node_ptr, 0x00, sizeof(*node_ptr));
        result = ERR_OK;
    }
    else
    {
        result = -ERR_INVAL;
    }
ERR_EXIT:
    return result;
}


/* Free extention routine for DF_Node. Diff with DF_Node_free() and this will free the point itself. */
int DF_Node_freeExt(Node_t *node_ptr)
{
    if(node_ptr != NULL)
    {
        /* Free the node point content and point itself. */
        DF_Node_free(node_ptr);
        FREEMEM(node_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}

/* Allocate routine for DF_Node.*/
int DF_Node_allocate(Node_t *node_ptr, DF_Node_st_ptr Node_ptr)
{
    int result = 0;


    /* Error detection. */
    if((node_ptr == NULL) || (Node_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(node_ptr, 0x00, sizeof(*node_ptr));

    /* name. */
    if(Node_ptr->opt.Node_opt_name == MSG_OPTIONAL_YES)
    {
        if((node_ptr->name = CALLOC(1, sizeof(*(node_ptr->name)))) != NULL)
        {
            if((result = DE_DescriptiveName_allocate(node_ptr->name, &(Node_ptr->name))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        node_ptr->name = NULL;
    }

    /*id*/
    result = DF_NodeReferenceID_allocate(&(node_ptr->id), &(Node_ptr->id));
    if(result)
        goto ERR_EXIT;

    /*refPos*/
    result = DF_Position3D_allocate(&(node_ptr->refPos), &(Node_ptr->refPos));
    if(result)
        goto ERR_EXIT;

    /* inLinks. */
    if(Node_ptr->opt.Node_opt_inLinks == MSG_OPTIONAL_YES)
    {
        if((node_ptr->inLinks = CALLOC(1, sizeof(*(node_ptr->inLinks)))) != NULL)
        {
            if((result = DF_LinkList_allocate(node_ptr->inLinks, &(Node_ptr->inLinks))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        node_ptr->inLinks = NULL;
    }
    
    return result;
    
ERR_EXIT:
    
    DF_Node_free(node_ptr);
    return result;
}


/* Parse routine for DF_Node. */
int DF_Node_parse(Node_t *node_ptr, DF_Node_st_ptr Node_ptr)
{
    int result = 0;


    /* Error detection. */
    if((node_ptr == NULL) || (Node_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* clear Node_ptr memory */
    memset(Node_ptr, 0x00, sizeof(*Node_ptr));

    /*name. */
    if(node_ptr->name != NULL)
    {
        if((result = DE_DescriptiveName_parse(node_ptr->name, &(Node_ptr->name))) != 0)
        {
            goto ERR_EXIT;
        }
        Node_ptr->opt.Node_opt_name = MSG_OPTIONAL_YES;
    }
    else
        Node_ptr->opt.Node_opt_name = MSG_OPTIONAL_NO;

    /*id*/
    result = DF_NodeReferenceID_parse(&(node_ptr->id), &(Node_ptr->id));
    if(result)
        goto ERR_EXIT;

    /*refPos*/
    result = DF_Position3D_parse(&(node_ptr->refPos), &(Node_ptr->refPos));
    if(result)
        goto ERR_EXIT; 

    /*inLinks. */
    if(node_ptr->inLinks != NULL)
    {
        if((result = DF_LinkList_parse(node_ptr->inLinks, &(Node_ptr->inLinks))) != 0)
        {
            goto ERR_EXIT;
        }
        Node_ptr->opt.Node_opt_inLinks = MSG_OPTIONAL_YES;
    }
    else
        Node_ptr->opt.Node_opt_inLinks = MSG_OPTIONAL_NO;

ERR_EXIT:

    return result;
}

/* Free routine for DF_NodeList. */
int DF_NodeList_free(NodeList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}

/* Allocate routine for DF_NodeList. */
int DF_NodeList_allocate(NodeList_t *list_ptr, DF_NodeList_st_ptr List_ptr)
{
    int             result = 0;
    uint8_t point_index = 0;
    Node_t   * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* IntersectionStateList. */
    list_ptr->list.free = (void (*)(Node_t *))DF_Node_freeExt;
    
    for(point_index = 0; point_index < List_ptr->NodeList_size; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if(DF_Node_allocate(elem_ptr, &(List_ptr->NodeList_data[point_index])) == 0)
            {
                /* Add point element to sequence. */
                if(asn_sequence_add(&(list_ptr->list), elem_ptr) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_Node_free(elem_ptr);
                    FREEMEM(elem_ptr);

                    result = -ERR_SYS;
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);

                result = -ERR_SYS;
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_NodeList_free(list_ptr);
    return result;
}


/* Parse routine for DF_NodeList. */
int DF_NodeList_parse(NodeList_t *list_ptr, DF_NodeList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* IntersectionStateList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_Node_parse(list_ptr->list.array[point_index], &(List_ptr->NodeList_data[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->NodeList_size = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


/* Free routine for DF_ParticipantData. */
int DF_ParticipantData_free(ParticipantData_t *ptc_ptr)
{
    if(ptc_ptr != NULL)
    {
        /* Id. */
        if(ptc_ptr->id != NULL)
        {
            DE_TemporaryID_free(ptc_ptr->id);
            FREEMEM(ptc_ptr->id);
        }

        /* PlateNo. */
        if(ptc_ptr->plateNo != NULL)
        {
            DE_PlateNo_free(ptc_ptr->plateNo);
            FREEMEM(ptc_ptr->plateNo);
        }

        /* TransmissionState. */
        if(ptc_ptr->transmission != NULL)
        {
            FREEMEM(ptc_ptr->transmission);
        }

        /* SteeringWheelAngle. */
        if(ptc_ptr->angle != NULL)
        {
            FREEMEM(ptc_ptr->angle);
        }

        /* MotionConfidenceSet. */
        if(ptc_ptr->motionCfd != NULL)
        {
            DF_MotionConfidenceSet_free(ptc_ptr->motionCfd);
            FREEMEM(ptc_ptr->motionCfd);
        }

        /* AccelerationSet4Way. */
        if(ptc_ptr->accelSet != NULL)
        {
            DF_AccelerationSet4Way_free(ptc_ptr->accelSet);
            FREEMEM(ptc_ptr->accelSet);
        }

        /* VehicleClassification. */
        if(ptc_ptr->vehicleClass != NULL)
        {
            DF_VehicleClassification_free(ptc_ptr->vehicleClass);
            FREEMEM(ptc_ptr->vehicleClass);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(ptc_ptr, 0, sizeof(*ptc_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Free extention routine for DF_ParticipantData. Diff with DF_ParticipantData_free() and this will free the point itself. */
int DF_ParticipantData_freeExt(ParticipantData_t *ptc_ptr)
{
    if(ptc_ptr != NULL)
    {
        /* Free the path history point content and point itself. */
        DF_ParticipantData_free(ptc_ptr);
        FREEMEM(ptc_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_ParticipantData. */
int DF_ParticipantData_allocate(ParticipantData_t *ptc_ptr, DF_ParticipantData_st_ptr Ptc_ptr)
{
    int result = 0;


    /* Error detect. */
    if((ptc_ptr == NULL) || (Ptc_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(ptc_ptr, 0, sizeof(*ptc_ptr));

    /* ParticipantType. */
    ptc_ptr->ptcType = Ptc_ptr->ptcType;

    /* ptcId. */
    ptc_ptr->ptcId = Ptc_ptr->ptcId;

    /* Source type. */
    ptc_ptr->source = Ptc_ptr->sourceType;

    /* id. */
    if(Ptc_ptr->opt.id == MSG_OPTIONAL_YES)
    {
        if((ptc_ptr->id = CALLOC(1, sizeof(*(ptc_ptr->id)))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        if((result = DE_TemporaryID_allocate(ptc_ptr->id, &(Ptc_ptr->id))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        ptc_ptr->id = NULL;
    }
    
    /* plateNo */
    if(Ptc_ptr->opt.plateNo == MSG_OPTIONAL_YES)
    {
        if((ptc_ptr->plateNo = CALLOC(1, sizeof(*(ptc_ptr->plateNo)))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        if((result = DE_PlateNo_allocate(ptc_ptr->plateNo, &(Ptc_ptr->plateNo))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        ptc_ptr->plateNo = NULL;
    }

    /* Dsecond. */
    ptc_ptr->secMark = bsctyp_encode(DE_DSecond, Ptc_ptr->secMark);

    /* PositionOffset LLV*/
    if((result = DF_PositionOffsetLLV_allocate(&ptc_ptr->pos, &Ptc_ptr->pos)) != 0)
    {
        goto ERR_EXIT;
    }

    /* PositionConfidenceSet. */
    if((result = DF_PositionConfidenceSet_allocate(&(ptc_ptr->accuracy), &(Ptc_ptr->accuracy))) != 0)
    {
        goto ERR_EXIT;
    }

      /* TransmissionState. */
    if(Ptc_ptr->opt.transmission == MSG_OPTIONAL_YES)
    {
        if((ptc_ptr->transmission = CALLOC(1, sizeof(*(ptc_ptr->transmission)))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        *(ptc_ptr->transmission) = Ptc_ptr->transmission;
    }
    else
    {
        ptc_ptr->transmission = NULL;
    }
  
    /* Speed. */
    ptc_ptr->speed = bsctyp_encode(DE_Speed, Ptc_ptr->speed);

    /* Heading. */
    ptc_ptr->heading = bsctyp_encode(DE_Heading, Ptc_ptr->heading);

      /* SteeringWheelAngle. */
    if(Ptc_ptr->opt.angle == MSG_OPTIONAL_YES)
    {
        if((ptc_ptr->angle = CALLOC(1, sizeof(*(ptc_ptr->angle)))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        *(ptc_ptr->angle) = bsctyp_encode(DE_SteeringWheelAngle, Ptc_ptr->angle);
    }
    else
    {
        ptc_ptr->angle = NULL;
    }
  
      /* MotionConfidenceSet. */
    if(Ptc_ptr->opt.motioncfd == MSG_OPTIONAL_YES)
    {
        if((ptc_ptr->motionCfd = CALLOC(1, sizeof(*(ptc_ptr->motionCfd)))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        if((result = DF_MotionConfidenceSet_Allocate(ptc_ptr->motionCfd, &(Ptc_ptr->motioncfd))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        ptc_ptr->motionCfd = NULL;
    }
  
      /* AccelerationSet4Way. */
    if(Ptc_ptr->opt.accelset == MSG_OPTIONAL_YES)
    {
        if((ptc_ptr->accelSet = CALLOC(1, sizeof(*(ptc_ptr->accelSet)))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        if((result = DF_AccelerationSet4Way_allocate(ptc_ptr->accelSet, &(Ptc_ptr->accelset))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        ptc_ptr->accelSet = NULL;
    }

    /* VehicleSize. */
    if((result = DF_VehicleSize_allocate(&(ptc_ptr->size), &(Ptc_ptr->size))) != 0)
    {
        goto ERR_EXIT;
    }

    /* VehicleClassification. */
    if(Ptc_ptr->opt.vehicleclass == MSG_OPTIONAL_YES)
    {
        if((ptc_ptr->vehicleClass = CALLOC(1, sizeof(*ptc_ptr->vehicleClass))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        if((result = DF_VehicleClassification_allocate(ptc_ptr->vehicleClass, &(Ptc_ptr->vehicleclass))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        ptc_ptr->vehicleClass = NULL;
    }

    return result;

ERR_EXIT:

    DF_ParticipantData_free(ptc_ptr);
    return result;
}


/* Parse routine for DF_ParticipantData. */
int DF_ParticipantData_parse(ParticipantData_t *ptc_ptr, DF_ParticipantData_st_ptr Ptc_ptr)
{
    int result = 0;


    /* Error detect. */
    if((ptc_ptr == NULL) || (Ptc_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Ptc_ptr, 0, sizeof(*Ptc_ptr));

    /* ParticipantType. */
    Ptc_ptr->ptcType = ptc_ptr->ptcType;

    /* ptcId. */
    Ptc_ptr->ptcId = ptc_ptr->ptcId;

    /* Source type. */
    Ptc_ptr->sourceType = ptc_ptr->source;

    /* id. */
    if(ptc_ptr->id != NULL)
    {
        Ptc_ptr->opt.id = MSG_OPTIONAL_YES;
        if((result = DE_TemporaryID_parse(ptc_ptr->id, &(Ptc_ptr->id))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        Ptc_ptr->opt.id = MSG_OPTIONAL_NO;
    }

    /* plateNo */
    if(ptc_ptr->plateNo != NULL)
    {
        Ptc_ptr->opt.plateNo = MSG_OPTIONAL_YES;
        if((result = DE_PlateNo_parse(ptc_ptr->plateNo, &(Ptc_ptr->plateNo))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        Ptc_ptr->opt.plateNo = MSG_OPTIONAL_NO;
    }

    /* Dsecond. */
    Ptc_ptr->secMark = bsctyp_decode(DE_DSecond, ptc_ptr->secMark);

    /* PositionOffset LLV*/
    if((result = DF_PositionOffsetLLV_parse(&ptc_ptr->pos, &Ptc_ptr->pos)) != 0)
    {
        goto ERR_EXIT;
    }

    /* PositionConfidenceSet. */
    if((result = DF_PositionConfidenceSet_parse(&(ptc_ptr->accuracy), &(Ptc_ptr->accuracy))) != 0)
    {
        goto ERR_EXIT;
    }

      /* TransmissionState. */
    if(ptc_ptr->transmission != NULL)
    {
        Ptc_ptr->opt.transmission = MSG_OPTIONAL_YES;
        Ptc_ptr->transmission = *(ptc_ptr->transmission);
    }
    else
    {
        Ptc_ptr->opt.transmission = MSG_OPTIONAL_NO;
    }

    /* Speed. */
    Ptc_ptr->speed = bsctyp_decode(DE_Speed, ptc_ptr->speed);

    /* Heading. */
    Ptc_ptr->heading = bsctyp_decode(DE_Heading, ptc_ptr->heading);

      /* SteeringWheelAngle. */
    if(ptc_ptr->angle != NULL)
    {
        Ptc_ptr->opt.angle = MSG_OPTIONAL_YES;
        Ptc_ptr->angle = bsctyp_decode(DE_SteeringWheelAngle, *(ptc_ptr->angle));
    }
    else
    {
        Ptc_ptr->opt.angle = MSG_OPTIONAL_NO;
    }

      /* MotionConfidenceSet. */
    if(ptc_ptr->motionCfd != NULL)
    {
        Ptc_ptr->opt.motioncfd = MSG_OPTIONAL_YES;
        if((result = DF_MotionConfidenceSet_parse(ptc_ptr->motionCfd, &(Ptc_ptr->motioncfd))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        Ptc_ptr->opt.motioncfd = MSG_OPTIONAL_NO;
    }
  
      /* AccelerationSet4Way. */
    if(ptc_ptr->accelSet != NULL)
    {
        Ptc_ptr->opt.accelset = MSG_OPTIONAL_YES;
        if((result = DF_AccelerationSet4Way_parse(ptc_ptr->accelSet, &(Ptc_ptr->accelset))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        Ptc_ptr->opt.accelset = MSG_OPTIONAL_NO;
    }

    /* VehicleSize. */
    if((result = DF_VehicleSize_parse(&(ptc_ptr->size), &(Ptc_ptr->size))) != 0)
    {
        goto ERR_EXIT;
    }

    /* VehicleClassification. */
    if(ptc_ptr->vehicleClass != NULL)
    {
        Ptc_ptr->opt.vehicleclass = MSG_OPTIONAL_YES;
        if((result = DF_VehicleClassification_parse(ptc_ptr->vehicleClass, &(Ptc_ptr->vehicleclass))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        Ptc_ptr->opt.vehicleclass = MSG_OPTIONAL_NO;
    }
    
    return result;

ERR_EXIT:
 
    return result;
}


/* Free routine for DF_ParticipantList. */
int DF_ParticipantList_free(ParticipantList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_ParticipantList. */
int DF_ParticipantList_allocate(ParticipantList_t *list_ptr, DF_ParticipantList_st_ptr List_ptr)
{
    int                     result = 0;
    uint8_t            point_index = 0;
    ParticipantData_t   * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* ParticipantList. */
    list_ptr->list.free = (void (*)(ParticipantData_t *))DF_ParticipantData_freeExt;
    
    for(point_index = 0; point_index < List_ptr->pointNum; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if((result = DF_ParticipantData_allocate(elem_ptr, &(List_ptr->array[point_index]))) == 0)
            {
                /* Add point element to sequence. */
                if((result = asn_sequence_add(&(list_ptr->list), elem_ptr)) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_ParticipantData_free(elem_ptr);
                    FREEMEM(elem_ptr);
 
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);
                
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_ParticipantList_free(list_ptr);
    return result;
}


/* Parse routine for DF_ParticipantList. */
int DF_ParticipantList_parse(ParticipantList_t *list_ptr, DF_ParticipantList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* ParticipantList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_ParticipantData_parse(list_ptr->list.array[point_index], &(List_ptr->array[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->pointNum = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


/* Free routine for DF_PathHistory. */
int DF_PathHistory_free(PathHistory_t *path_ptr)
{
    int result = 0;

    if(path_ptr != NULL)
    {
        if(path_ptr->initialPosition != NULL)
        {
            DF_FullPositionVector_free(path_ptr->initialPosition);
            FREEMEM(path_ptr->initialPosition);
        }
        
        if(path_ptr->currGNSSstatus != NULL)
        {
            DE_GNSSstatus_free(path_ptr->currGNSSstatus);
            FREEMEM(path_ptr->currGNSSstatus);
        }
        
        DF_PathHistoryPointList_free(&(path_ptr->crumbData));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(path_ptr, 0, sizeof(*path_ptr));
        
    }

      return result;
}


/* Allocate routine for DF_PathHistory. */
int DF_PathHistory_allocate(PathHistory_t *path_ptr, DF_PathHistory_st_ptr Path_ptr)
{
    int result = 0;

    
    /* Error detect. */
    if((path_ptr == NULL) || (Path_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(path_ptr, 0, sizeof(*path_ptr));

    /* FullPositionVector. */
    if(Path_ptr->opt.PathHistory_opt_initialPosition == MSG_OPTIONAL_YES)
    {
        if((path_ptr->initialPosition = CALLOC(1, sizeof(*path_ptr->initialPosition))) != NULL)
        {
            if((result = DF_FullPositionVector_allocate(path_ptr->initialPosition, &(Path_ptr->initialPosition))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        path_ptr->initialPosition = NULL;
    }

    /* GNSSstatus. */
    if(Path_ptr->opt.PathHistory_opt_currGNSSstatus == MSG_OPTIONAL_YES)
    {
        if((path_ptr->currGNSSstatus = CALLOC(1, sizeof(*path_ptr->currGNSSstatus))) != NULL)
        {
            if((result = DE_GNSSstatus_allocate(path_ptr->currGNSSstatus, &(Path_ptr->currGNSSstatus))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        path_ptr->currGNSSstatus = NULL;
    }

    /* PathHistoryPointList. */
    if((result = DF_PathHistoryPointList_allocate(&(path_ptr->crumbData), &(Path_ptr->crumbData))) != 0)
    {
        goto ERR_EXIT;
    }
    
    return result;

ERR_EXIT:

    DF_PathHistory_free(path_ptr);
    return result;
}


/* Parse routine for DF_PathHistory. */
int DF_PathHistory_parse(PathHistory_t *path_ptr, DF_PathHistory_st_ptr Path_ptr)
{  
    int result = 0;

    
    /* Error detect. */
    if((path_ptr == NULL) || (Path_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Path_ptr, 0, sizeof(*Path_ptr));

    /* FullPositionVector. */
    if(path_ptr->initialPosition != NULL)
    {
        Path_ptr->opt.PathHistory_opt_initialPosition = MSG_OPTIONAL_YES;
        if((result = DF_FullPositionVector_parse(path_ptr->initialPosition, &(Path_ptr->initialPosition))) != 0)
        {
            goto ERR_EXIT;
        }
    }

    /* GNSSstatus. */
    if(path_ptr->currGNSSstatus != NULL)
    {
        Path_ptr->opt.PathHistory_opt_currGNSSstatus = MSG_OPTIONAL_YES;
        if((result = DE_GNSSstatus_parse(path_ptr->currGNSSstatus, &(Path_ptr->currGNSSstatus))) != 0)
        {
            goto ERR_EXIT;
        }
    }

    /* PathHistoryPointList. */
    if((result = DF_PathHistoryPointList_parse(&(path_ptr->crumbData), &(Path_ptr->crumbData))) != 0)
    {
        goto ERR_EXIT;
    }
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


/* Free routine for DF_PathHistoryPointList. */
int DF_PathHistoryPointList_free(PathHistoryPointList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_PathHistoryPointList. */
int DF_PathHistoryPointList_allocate(PathHistoryPointList_t *list_ptr, DF_PathHistoryPointList_st_ptr List_ptr)
{
    int    result = 0;
    uint8_t            point_index = 0;
    PathHistoryPoint_t  * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* PathHistoryPointList. */
    list_ptr->list.free = (void (*)(PathHistoryPoint_t *))DF_PathHistoryPoint_freeExt;
    
    for(point_index = 0; point_index < List_ptr->pointNum; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if((result = DF_PathHistoryPoint_allocate(elem_ptr, &(List_ptr->array[point_index]))) == 0)
            {
                /* Add point element to sequence. */
                if((result = asn_sequence_add(&(list_ptr->list), elem_ptr)) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_PathHistoryPoint_free(elem_ptr);
                    FREEMEM(elem_ptr);
                    
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);
                
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_PathHistoryPointList_free(list_ptr);
    return result;
}


/* Parse routine for DF_PathHistoryPointList. */
int DF_PathHistoryPointList_parse(PathHistoryPointList_t *list_ptr, DF_PathHistoryPointList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* PathHistoryPointList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_PathHistoryPoint_parse(list_ptr->list.array[point_index], &(List_ptr->array[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->pointNum = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


/* Free routine for DF_PathHistoryPoint. */
int DF_PathHistoryPoint_free(PathHistoryPoint_t *point_ptr)
{
    if(point_ptr != NULL)
    {
        DF_PositionOffsetLLV_free(&point_ptr->llvOffset);
        
        if(point_ptr->speed != NULL)
        {
            FREEMEM(point_ptr->speed);
        }
        
        if(point_ptr->posAccuracy != NULL)
        {
            DF_PositionConfidenceSet_free(point_ptr->posAccuracy);
            FREEMEM(point_ptr->posAccuracy);
        }
        
        if(point_ptr->heading != NULL)
        {
            FREEMEM(point_ptr->heading);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(point_ptr, 0, sizeof(*point_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Free extention routine for DF_PathHistoryPoint. Diff with DF_PathHistoryPoint_free() and this will free the point itself. */
int DF_PathHistoryPoint_freeExt(PathHistoryPoint_t *point_ptr)
{
    if(point_ptr != NULL)
    {
        /* Free the path history point content and point itself. */
        DF_PathHistoryPoint_free(point_ptr);
        FREEMEM(point_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_PathHistoryPoint. */
int DF_PathHistoryPoint_allocate(PathHistoryPoint_t *point_ptr, DF_PathHistoryPoint_st_ptr Point_ptr)
{
    int result = 0;
    /* Error detect. */
    if((point_ptr == NULL) || (Point_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(point_ptr, 0, sizeof(*point_ptr));

    /* PositionOffset LLV*/
    if((result = DF_PositionOffsetLLV_allocate(&point_ptr->llvOffset, &Point_ptr->llvOffset)) != 0)
    {
        goto ERR_EXIT;
    }

    /* TimeOffset. */
    point_ptr->timeOffset = bsctyp_encode(DE_TimeOffset, Point_ptr->timeoffset);

    /* Speed. */
    if(Point_ptr->opt.PathHistoryPoint_opt_speed == MSG_OPTIONAL_YES)
    {
        if((point_ptr->speed = CALLOC(1, sizeof(*point_ptr->speed))) != NULL)
        {
            * point_ptr->speed = bsctyp_encode(DE_Speed, Point_ptr->speed);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        point_ptr->speed = NULL;
    }

    /* PositionAccuracy. */
    if(Point_ptr->opt.PathHistoryPoint_opt_posAccuracy == MSG_OPTIONAL_YES)
    {
        if((point_ptr->posAccuracy = CALLOC(1, sizeof(*point_ptr->posAccuracy))) != NULL)
        {
            if((result = DF_PositionConfidenceSet_allocate(point_ptr->posAccuracy, &(Point_ptr->posaccuracy))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        point_ptr->posAccuracy = NULL;
    }

    /* CoarseHeading. */
    if(Point_ptr->opt.PathHistoryPoint_opt_heading == MSG_OPTIONAL_YES)
    {
        if((point_ptr->heading = CALLOC(1, sizeof(*point_ptr->heading))) != NULL)
        {
            * point_ptr->heading = bsctyp_encode(DE_CoarseHeading, Point_ptr->heading);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        point_ptr->heading = NULL;
    }

    return result;

ERR_EXIT:

    DF_PathHistoryPoint_free(point_ptr);
    return result;
}


/* Parse routine for DF_PathHistoryPoint. */
int DF_PathHistoryPoint_parse(PathHistoryPoint_t *point_ptr, DF_PathHistoryPoint_st_ptr Point_ptr)
{
    int result = 0;


    /* Error detect. */
    if((point_ptr == NULL) || (Point_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Point_ptr, 0, sizeof(*Point_ptr));

    /* PositionOffset LLV*/
    if((result = DF_PositionOffsetLLV_parse(&point_ptr->llvOffset, &Point_ptr->llvOffset)) != 0)
    {
        goto ERR_EXIT;
    }

    /* TimeOffset. */
    Point_ptr->timeoffset = bsctyp_decode(DE_TimeOffset, point_ptr->timeOffset);

    /* Speed. */
    if(point_ptr->speed != NULL)
    {
        Point_ptr->opt.PathHistoryPoint_opt_speed = MSG_OPTIONAL_YES;
        Point_ptr->speed = bsctyp_decode(DE_Speed, * point_ptr->speed);
    }

    /* PositionAccuracy. */
    if(point_ptr->posAccuracy != NULL)
    {
        Point_ptr->opt.PathHistoryPoint_opt_posAccuracy = MSG_OPTIONAL_YES;
        if((result = DF_PositionConfidenceSet_parse(point_ptr->posAccuracy, &(Point_ptr->posaccuracy))) != 0)
        {
            goto ERR_EXIT;
        }
    }

    /* CoarseHeading. */
    if(point_ptr->heading != NULL)
    {
        Point_ptr->opt.PathHistoryPoint_opt_heading = MSG_OPTIONAL_YES;
        Point_ptr->heading = bsctyp_decode(DE_CoarseHeading, * point_ptr->heading);
    }
   
    return result;

ERR_EXIT:
    
    return result;
}


/* Free routine for DF_PathPointList. */
int DF_PathPointList_free(PathPointList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_PathPointList. */
int DF_PathPointList_allocate(PathPointList_t *list_ptr, DF_PathPointList_st_ptr List_ptr)
{
    int                         result = 0;
    uint8_t             point_index = 0;
    PositionOffsetLLV_t  * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* PathPointList. */
    list_ptr->list.free = (void (*)(PositionOffsetLLV_t *))DF_PositionOffsetLLV_freeExt;
    
    for(point_index = 0; point_index < List_ptr->pointNum; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if((result = DF_PositionOffsetLLV_allocate(elem_ptr, &(List_ptr->array[point_index]))) == 0)
            {
                /* Add point element to sequence. */
                if((result = asn_sequence_add(&(list_ptr->list), elem_ptr)) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_PositionOffsetLLV_free(elem_ptr);
                    FREEMEM(elem_ptr);
                    
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);
                
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_PathPointList_free(list_ptr);
    return result;
}


/* Parse routine for DF_PathPointList. */
int DF_PathPointList_parse(PathPointList_t *list_ptr, DF_PathPointList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* PathPointList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_PositionOffsetLLV_parse(list_ptr->list.array[point_index], &(List_ptr->array[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->pointNum = point_index;

ERR_EXIT:

    return result;
}


/* Free routine for DF_PathPrediction. */
int DF_PathPrediction_free(PathPrediction_t *path_ptr)
{
    if(path_ptr != NULL)
    {
        /* Must clear all the zone and avoid repeat free operation. */
        memset(path_ptr, 0, sizeof(*path_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_PathPrediction. */
int DF_PathPrediction_allocate(PathPrediction_t *path_ptr, DF_PathPrediction_st_ptr Path_ptr)
{
    int result = 0;

    
    /* Error detect. */
    if((path_ptr == NULL) || (Path_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(path_ptr, 0, sizeof(*path_ptr));

    /* Radius of curvature. */
    path_ptr->radiusOfCurve = bsctyp_encode(DE_RadiusOfCurvature, Path_ptr->radiusOfCurve);

    /* Confidence. */
    path_ptr->confidence = bsctyp_encode(DE_Confidence, Path_ptr->confidence);
    
    return result;

ERR_EXIT:

    DF_PathPrediction_free(path_ptr);
    return result;
}


/* Parse routine for DF_PathPrediction. */
int DF_PathPrediction_parse(PathPrediction_t *path_ptr, DF_PathPrediction_st_ptr Path_ptr)
{  
    int result = 0;

    
    /* Error detect. */
    if((path_ptr == NULL) || (Path_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Path_ptr, 0, sizeof(*Path_ptr));

    /* Radius of curvature. */
    Path_ptr->radiusOfCurve = bsctyp_decode(DE_RadiusOfCurvature, path_ptr->radiusOfCurve);

    /* Confidence. */
    Path_ptr->confidence = bsctyp_decode(DE_Confidence, path_ptr->confidence);

ERR_EXIT:

    return result;
}


/* Free routine for DF_PhaseList. */
int DF_PhaseList_free(PhaseList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_PhaseList. */
int DF_PhaseList_allocate(PhaseList_t *list_ptr, DF_PhaseList_st_ptr List_ptr)
{
    int               result = 0;
    uint8_t      point_index = 0;
    Phase_t       * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* PhaseList. */
    list_ptr->list.free = (void (*)(Phase_t *))DF_Phase_freeExt;
    
    for(point_index = 0; point_index < List_ptr->pointNum; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if(DF_Phase_allocate(elem_ptr, &(List_ptr->array[point_index])) == 0)
            {
                /* Add point element to sequence. */
                if(asn_sequence_add(&(list_ptr->list), elem_ptr) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_Phase_free(elem_ptr);
                    FREEMEM(elem_ptr);

                    result = -ERR_SYS;
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);

                result = -ERR_SYS;
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_PhaseList_free(list_ptr);
    return result;
}


/* Parse routine for DF_PhaseList. */
int DF_PhaseList_parse(PhaseList_t *list_ptr, DF_PhaseList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* PhaseList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_Phase_parse(list_ptr->list.array[point_index], &(List_ptr->array[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->pointNum = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


/* Free routine for DF_PhaseState. */
int DF_PhaseState_free(PhaseState_t *ps_ptr)
{
    int result = 0;

    
    if(ps_ptr != NULL)
    {
        /* TimeChangeDetails. */
        if(ps_ptr->timing != NULL)
        {
            DF_TimeChangeDetails_free(ps_ptr->timing);
            FREEMEM(ps_ptr->timing);
        }
        
        memset(ps_ptr, 0x00, sizeof(*ps_ptr));
        result = ERR_OK;
    }
    else
    {
        result = -ERR_INVAL;
    }

    return result;
}


/* Free extention routine for DF_PhaseState. Diff with DF_PhaseState_free() and this will free the point itself. */
int DF_PhaseState_freeExt(PhaseState_t *ps_ptr)
{
    if(ps_ptr != NULL)
    {
        /* Free the path history point content and point itself. */
        DF_PhaseState_free(ps_ptr);
        FREEMEM(ps_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_PhaseState.*/
int DF_PhaseState_allocate(PhaseState_t *ps_ptr, DF_PhaseState_st_ptr Ps_ptr)
{
    int result = 0;


    /* Error detection. */
    if((ps_ptr == NULL) || (Ps_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(ps_ptr, 0x00, sizeof(*ps_ptr));

    /* Light. */
    ps_ptr->light = Ps_ptr->light;

    /* Time change details. */
    if(Ps_ptr->opt.PhaseState_opt_timing == MSG_OPTIONAL_YES)
    {
        if((ps_ptr->timing = CALLOC(1, sizeof(*(ps_ptr->timing)))) != NULL)
        {
            if((result = DF_TimeChangeDetails_allocate(ps_ptr->timing, &(Ps_ptr->timing))) != 0)
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        ps_ptr->timing = NULL;
    }

    return result;
    
ERR_EXIT:
    
    DF_PhaseState_free(ps_ptr);
    return result;
}


/* Parse routine for DF_PhaseState. */
int DF_PhaseState_parse(PhaseState_t *ps_ptr, DF_PhaseState_st_ptr Ps_ptr)
{
    int result = 0;


    /* Error detection. */
    if((ps_ptr == NULL) || (Ps_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* clear Ps_ptr memory */
    memset(Ps_ptr, 0x00, sizeof(*Ps_ptr));

    /* Light. */
    Ps_ptr->light = ps_ptr->light;

    /* Time change details. */
    if(ps_ptr->timing != NULL)
    {
        if((result = DF_TimeChangeDetails_parse(ps_ptr->timing, &(Ps_ptr->timing))) != 0)
        {
            goto ERR_EXIT;
        }
        Ps_ptr->opt.PhaseState_opt_timing = MSG_OPTIONAL_YES;
    }

ERR_EXIT:

    return result;
}


/* Free routine for DF_PhaseStateList. */
int DF_PhaseStateList_free(PhaseStateList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_PhaseStateList. */
int DF_PhaseStateList_allocate(PhaseStateList_t *list_ptr, DF_PhaseStateList_st_ptr List_ptr)
{
    int               result = 0;
    uint8_t      point_index = 0;
    PhaseState_t  * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* PhaseStateList. */
    list_ptr->list.free = (void (*)(PhaseState_t *))DF_PhaseState_freeExt;
    
    for(point_index = 0; point_index < List_ptr->pointNum; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if(DF_PhaseState_allocate(elem_ptr, &(List_ptr->array[point_index])) == 0)
            {
                /* Add point element to sequence. */
                if(asn_sequence_add(&(list_ptr->list), elem_ptr) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_PhaseState_free(elem_ptr);
                    FREEMEM(elem_ptr);

                    result = -ERR_SYS;
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);

                result = -ERR_SYS;
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_PhaseStateList_free(list_ptr);
    return result;
}


/* Parse routine for DF_PhaseStateList. */
int DF_PhaseStateList_parse(PhaseStateList_t *list_ptr, DF_PhaseStateList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* PhaseStateList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_PhaseState_parse(list_ptr->list.array[point_index], &(List_ptr->array[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->pointNum = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


/* Free routine for DF_Phase. */
int DF_Phase_free(Phase_t *phase_ptr)
{
    int result = 0;
    
    
    if(phase_ptr != NULL)
    {
        /* Phase state list. */
         if((result = DF_PhaseStateList_free(&(phase_ptr->phaseStates))) != 0)
        {
            goto ERR_EXIT; 
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(phase_ptr, 0, sizeof(*phase_ptr));
    }
    else
    {
        result = -ERR_INVAL;
    }
    
ERR_EXIT:

    return result;
}


/* Free extention routine for DF_Phase. Diff with DF_Phase_free() and this will free the point itself. */
int DF_Phase_freeExt(Phase_t *phase_ptr)
{
    if(phase_ptr != NULL)
    {
        /* Free the path history point content and point itself. */
        DF_Phase_free(phase_ptr);
        FREEMEM(phase_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_Phase. */
int DF_Phase_allocate(Phase_t *phase_ptr, DF_Phase_st_ptr Phase_ptr)
{
    int result = 0;

    
    /* Error detect. */
    if((phase_ptr == NULL) || (Phase_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(phase_ptr, 0, sizeof(*phase_ptr));

    /* PhaseID. */
    phase_ptr->id = bsctyp_encode(DE_PhaseID, Phase_ptr->id);
    
    /* Phase state list. */
     if((result = DF_PhaseStateList_allocate(&(phase_ptr->phaseStates), &(Phase_ptr->phaseStates))) != 0)
    {
        goto ERR_EXIT; 
    }

    return result;

ERR_EXIT:

    DF_Phase_free(phase_ptr);
    return result;
}


/* Parse routine for DF_Phase. */
int DF_Phase_parse(Phase_t *phase_ptr, DF_Phase_st_ptr Phase_ptr)
{
    int result = 0;

    /* Error detect. */
    if((phase_ptr == NULL) || (Phase_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Phase_ptr, 0, sizeof(*Phase_ptr));

    /* PhaseID. */
    Phase_ptr->id = bsctyp_decode(DE_PhaseID, phase_ptr->id);
    
    /* Phase state list. */
     if((result = DF_PhaseStateList_parse(&(phase_ptr->phaseStates), &(Phase_ptr->phaseStates))) != 0)
    {
        goto ERR_EXIT; 
    }

ERR_EXIT:
    
    return result;
}


/* allocate routine for DF_Position_LL_24B. */
int DF_Position_LL_24B_allocate(Position_LL_24B_t *pos_ll_24b_ptr, DF_Position_LL_24B_st_ptr Pos_LL_24B_ptr)
{
    int result = 0;
    
    if((pos_ll_24b_ptr == NULL) || (Pos_LL_24B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(pos_ll_24b_ptr, 0x00, sizeof(*pos_ll_24b_ptr));

    pos_ll_24b_ptr->lat = bsctyp_encode(DE_OffsetLL_B12, Pos_LL_24B_ptr->lat);
    
    pos_ll_24b_ptr->lon = bsctyp_encode(DE_OffsetLL_B12, Pos_LL_24B_ptr->lon);
    
ERR_EXIT:
    
    return result;
}

/* parse routine for DF_Position_LL_24B. */
int DF_Position_LL_24B_parse(Position_LL_24B_t *pos_ll_24b_ptr, DF_Position_LL_24B_st_ptr Pos_LL_24B_ptr)
{
    int result = 0;
    
    if((pos_ll_24b_ptr == NULL) || (Pos_LL_24B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_24b_ptr, 0x00, sizeof(*pos_ll_24b_ptr));

    Pos_LL_24B_ptr->lat = bsctyp_decode(DE_OffsetLL_B12, pos_ll_24b_ptr->lat);
    
    Pos_LL_24B_ptr->lon = bsctyp_decode(DE_OffsetLL_B12, pos_ll_24b_ptr->lon);

ERR_EXIT:
    
    return result;
}

/* allocate routine for DF_Position_LL_28B. */
int DF_Position_LL_28B_allocate(Position_LL_28B_t *pos_ll_28b_ptr, DF_Position_LL_28B_st_ptr Pos_LL_28B_ptr)
{
    int result = 0;
    
    if((pos_ll_28b_ptr == NULL) || (Pos_LL_28B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_28b_ptr, 0x00, sizeof(*pos_ll_28b_ptr));
    
    pos_ll_28b_ptr->lat = bsctyp_encode(DE_OffsetLL_B14, Pos_LL_28B_ptr->lat);
    
    pos_ll_28b_ptr->lon = bsctyp_encode(DE_OffsetLL_B14, Pos_LL_28B_ptr->lon);
    
ERR_EXIT:
    
    return result;
}

/* parse routine for DF_Position_LL_28B. */
int DF_Position_LL_28B_parse(Position_LL_28B_t *pos_ll_28b_ptr, DF_Position_LL_28B_st_ptr Pos_LL_28B_ptr)
{
    int result = 0;
    
    if((pos_ll_28b_ptr == NULL) || (Pos_LL_28B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Reset all the zone. */
    memset(pos_ll_28b_ptr, 0x00, sizeof(*pos_ll_28b_ptr));
    
    Pos_LL_28B_ptr->lat = bsctyp_decode(DE_OffsetLL_B14, pos_ll_28b_ptr->lat);
        
    Pos_LL_28B_ptr->lon = bsctyp_decode(DE_OffsetLL_B14, pos_ll_28b_ptr->lon);


ERR_EXIT:
    
    return result;
}

/* allocate routine for DF_Position_LL_32B. */
int DF_Position_LL_32B_allocate(Position_LL_32B_t *pos_ll_32b_ptr, DF_Position_LL_32B_st_ptr Pos_LL_32B_ptr)
{
    int result = 0;
    
    if((pos_ll_32b_ptr == NULL) || (Pos_LL_32B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_32b_ptr, 0x00, sizeof(*pos_ll_32b_ptr));
    
    pos_ll_32b_ptr->lat = bsctyp_encode(DE_OffsetLL_B16, Pos_LL_32B_ptr->lat);
    
    pos_ll_32b_ptr->lon = bsctyp_encode(DE_OffsetLL_B16, Pos_LL_32B_ptr->lon);
    
ERR_EXIT:
    
    return result;
}

/* parse routine for DF_Position_LL_32B. */
int DF_Position_LL_32B_parse(Position_LL_32B_t *pos_ll_32b_ptr, DF_Position_LL_32B_st_ptr Pos_LL_32B_ptr)
{
    int result = 0;
    
    if((pos_ll_32b_ptr == NULL) || (Pos_LL_32B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_32b_ptr, 0x00, sizeof(*pos_ll_32b_ptr));
    
    Pos_LL_32B_ptr->lat = bsctyp_decode(DE_OffsetLL_B16, pos_ll_32b_ptr->lat);
        
    Pos_LL_32B_ptr->lon = bsctyp_decode(DE_OffsetLL_B16, pos_ll_32b_ptr->lon);


ERR_EXIT:
    
    return result;
}

/* allocate routine for DF_Position_LL_36B. */
int DF_Position_LL_36B_allocate(Position_LL_36B_t *pos_ll_36b_ptr, DF_Position_LL_36B_st_ptr Pos_LL_36B_ptr)
{
    int result = 0;
    
    if((pos_ll_36b_ptr == NULL) || (Pos_LL_36B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_36b_ptr, 0x00, sizeof(*pos_ll_36b_ptr));
    
    pos_ll_36b_ptr->lat = bsctyp_encode(DE_OffsetLL_B18, Pos_LL_36B_ptr->lat);
    
    pos_ll_36b_ptr->lon = bsctyp_encode(DE_OffsetLL_B18, Pos_LL_36B_ptr->lon);
    
ERR_EXIT:
    
    return result;
}

/* parse routine for DF_Position_LL_36B. */
int DF_Position_LL_36B_parse(Position_LL_36B_t *pos_ll_36b_ptr, DF_Position_LL_36B_st_ptr Pos_LL_36B_ptr)
{
    int result = 0;
    
    if((pos_ll_36b_ptr == NULL) || (Pos_LL_36B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_36b_ptr, 0x00, sizeof(*pos_ll_36b_ptr));
    
    Pos_LL_36B_ptr->lat = bsctyp_decode(DE_OffsetLL_B18, pos_ll_36b_ptr->lat);
        
    Pos_LL_36B_ptr->lon = bsctyp_decode(DE_OffsetLL_B18, pos_ll_36b_ptr->lon);


ERR_EXIT:
    
    return result;
}

/* allocate routine for DF_Position_LL_44B. */
int DF_Position_LL_44B_allocate(Position_LL_44B_t *pos_ll_44b_ptr, DF_Position_LL_44B_st_ptr Pos_LL_44B_ptr)
{
    int result = 0;
    
    if((pos_ll_44b_ptr == NULL) || (Pos_LL_44B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_44b_ptr, 0x00, sizeof(*pos_ll_44b_ptr));
    
    pos_ll_44b_ptr->lat = bsctyp_encode(DE_OffsetLL_B22, Pos_LL_44B_ptr->lat);
    
    pos_ll_44b_ptr->lon = bsctyp_encode(DE_OffsetLL_B22, Pos_LL_44B_ptr->lon);
    
ERR_EXIT:
    
    return result;
}

/* parse routine for DF_Position_LL_44B. */
int DF_Position_LL_44B_parse(Position_LL_44B_t *pos_ll_44b_ptr, DF_Position_LL_44B_st_ptr Pos_LL_44B_ptr)
{
    int result = 0;
    
    if((pos_ll_44b_ptr == NULL) || (Pos_LL_44B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_44b_ptr, 0x00, sizeof(*pos_ll_44b_ptr));
    
    Pos_LL_44B_ptr->lat = bsctyp_decode(DE_OffsetLL_B22, pos_ll_44b_ptr->lat);
        
    Pos_LL_44B_ptr->lon = bsctyp_decode(DE_OffsetLL_B22, pos_ll_44b_ptr->lon);


ERR_EXIT:
    
    return result;
}

/* allocate routine for DF_Position_LL_48B. */
int DF_Position_LL_48B_allocate(Position_LL_48B_t *pos_ll_48b_ptr, DF_Position_LL_48B_st_ptr Pos_LL_48B_ptr)
{
    int result = 0;
    
    if((pos_ll_48b_ptr == NULL) || (Pos_LL_48B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_48b_ptr, 0x00, sizeof(*pos_ll_48b_ptr));
    
    pos_ll_48b_ptr->lat = bsctyp_encode(DE_OffsetLL_B24, Pos_LL_48B_ptr->lat);
    
    pos_ll_48b_ptr->lon = bsctyp_encode(DE_OffsetLL_B24, Pos_LL_48B_ptr->lon);
    
ERR_EXIT:
    
    return result;
}

/* parse routine for DF_Position_LL_48B. */
int DF_Position_LL_48B_parse(Position_LL_48B_t *pos_ll_48b_ptr, DF_Position_LL_48B_st_ptr Pos_LL_48B_ptr)
{
    int result = 0;
    
    if((pos_ll_48b_ptr == NULL) || (Pos_LL_48B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_48b_ptr, 0x00, sizeof(*pos_ll_48b_ptr));
    
    Pos_LL_48B_ptr->lat = bsctyp_decode(DE_OffsetLL_B24, pos_ll_48b_ptr->lat);
        
    Pos_LL_48B_ptr->lon = bsctyp_decode(DE_OffsetLL_B24, pos_ll_48b_ptr->lon);


ERR_EXIT:
    
    return result;
}

/* allocate routine for DF_Position_LLmD_64B. */
int DF_Position_LLmD_64B_allocate(Position_LLmD_64b_t *pos_ll_64b_ptr, DF_Position_LLmD_64b_st_ptr Pos_LL_64B_ptr)
{
    int result = 0;
    
    if((pos_ll_64b_ptr == NULL) || (Pos_LL_64B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_64b_ptr, 0x00, sizeof(*pos_ll_64b_ptr));
    
    pos_ll_64b_ptr->lat = bsctyp_encode(DE_Latitude, Pos_LL_64B_ptr->lat);
    
    pos_ll_64b_ptr->lon = bsctyp_encode(DE_Longitude, Pos_LL_64B_ptr->lon);
    
ERR_EXIT:
    
    return result;
}

/* parse routine for DF_Position_LL_64B. */
int DF_Position_LL_64B_parse(Position_LLmD_64b_t *pos_ll_64b_ptr, DF_Position_LLmD_64b_st_ptr Pos_LL_64B_ptr)
{
    int result = 0;
    
    if((pos_ll_64b_ptr == NULL) || (Pos_LL_64B_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ll_64b_ptr, 0x00, sizeof(*pos_ll_64b_ptr));
    
    Pos_LL_64B_ptr->lat = bsctyp_decode(DE_Latitude, pos_ll_64b_ptr->lat);
        
    Pos_LL_64B_ptr->lon = bsctyp_decode(DE_Longitude, pos_ll_64b_ptr->lon);


ERR_EXIT:
    
    return result;
}


/* Free routine for DF_Position3D. */
int DF_Position3D_free(Position3D_t *pos_ptr)
{
    int result = 0;

    
    if(pos_ptr != NULL)
    {
        if(pos_ptr->elevation != NULL)
         {
             FREEMEM(pos_ptr->elevation);
         }

        memset(pos_ptr, 0x00, sizeof(*pos_ptr));
    }

    return result;
}


/* Allocate routine for DF_Position3D. */
int DF_Position3D_allocate(Position3D_t *pos_ptr, DF_Position3D_st_ptr Pos_ptr)
{
    int     result = 0;

    
    /* Error detect. */
    if((pos_ptr == NULL) || (Pos_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ptr, 0, sizeof(*pos_ptr));

    /* latitude . */
    pos_ptr->lat = bsctyp_encode(DE_Latitude, Pos_ptr->latitude);
    
    /* longitude . */
    pos_ptr->Long = bsctyp_encode(DE_Longitude, Pos_ptr->longitude);
    
    /* elevation . */
    if(Pos_ptr->opt.Position3D_opt_elev == MSG_OPTIONAL_YES)
    {
        pos_ptr->elevation = CALLOC(1, sizeof(*pos_ptr->elevation));
        if(pos_ptr->elevation == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        *(pos_ptr->elevation) = bsctyp_encode(DE_Elevation, Pos_ptr->elevation);
    }else{
        pos_ptr->elevation = NULL;
    }
    
    return result;

ERR_EXIT:

    DF_Position3D_free(pos_ptr);
    
    return result;

}

/* Parse routine for DF_Position3D. */
int DF_Position3D_parse(Position3D_t *pos_ptr, DF_Position3D_st_ptr Pos_ptr)
{
    int     result = 0;
    /* Error detect. */
    if((pos_ptr == NULL) || (Pos_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Pos_ptr, 0, sizeof(*Pos_ptr));

    /* latitude . */
    Pos_ptr->latitude = bsctyp_decode(DE_Latitude, pos_ptr->lat);
    /* longitude . */
    Pos_ptr->longitude = bsctyp_decode(DE_Longitude, pos_ptr->Long);
    /* elevation . */
    if(pos_ptr->elevation != NULL)
    {
        Pos_ptr->opt.Position3D_opt_elev = MSG_OPTIONAL_YES;
        Pos_ptr->elevation = bsctyp_decode(DE_Elevation, *(pos_ptr->elevation));
    }
    
    return result;
ERR_EXIT:

    return result;

}

#if 0
/* Free routine for DF_PositionalAccuracy. */
int DF_PositionalAccuracy_free(PositionalAccuracy_t *pos_ptr)
{
    if(pos_ptr != NULL)
    {
        /* Must clear all the zone and avoid repeat free operation. */
        memset(pos_ptr, 0, sizeof(*pos_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}

/* Allocate routine for DF_PositionalAccuracy. */
int DF_PositionalAccuracy_allocate(PositionalAccuracy_t *pos_ptr, DF_PositionalAccuracy_st_ptr Pos_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((pos_ptr == NULL) || (Pos_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ptr, 0, sizeof(*pos_ptr));

    pos_ptr->semiMajor = bsctyp_encode(DE_SemiMajorAxisAccuracy, Pos_ptr->semi_major_accu);
    pos_ptr->semiMinor = bsctyp_encode(DE_SemiMinorAxisAccuracy, Pos_ptr->semi_minor_accu);
    pos_ptr->orientation = bsctyp_encode(DE_SemiMajorAxisOrientation, Pos_ptr->semi_major_orientation);
    
    return result;

ERR_EXIT:

    DF_PositionalAccuracy_free(pos_ptr);
    return result;
}


/* Parse routine for DF_PositionalAccuracy. */
int DF_PositionalAccuracy_parse(PositionalAccuracy_t *pos_ptr, DF_PositionalAccuracy_st_ptr Pos_ptr)
{
    int            result = 0;
    /* Error detect. */
    if((pos_ptr == NULL) || (Pos_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Pos_ptr, 0, sizeof(*Pos_ptr));

    Pos_ptr->semi_major_accu = bsctyp_decode(DE_SemiMajorAxisAccuracy, pos_ptr->semiMajor);
    Pos_ptr->semi_minor_accu = bsctyp_decode(DE_SemiMinorAxisAccuracy, pos_ptr->semiMinor);
    Pos_ptr->semi_major_orientation = bsctyp_decode(DE_SemiMajorAxisOrientation, pos_ptr->orientation);
    
    return result;

ERR_EXIT:
    
    return result;
}
#endif

/* Free routine for DF_PositionConfidenceSet. */
int DF_PositionConfidenceSet_free(PositionConfidenceSet_t *pos_ptr)
{
    if(pos_ptr != NULL)
    {
        if(pos_ptr->elevation != NULL)
        {
            FREEMEM(pos_ptr->elevation);
        }
        /* Must clear all the zone and avoid repeat free operation. */
        memset(pos_ptr, 0, sizeof(*pos_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_PositionConfidenceSet. */
int DF_PositionConfidenceSet_allocate(PositionConfidenceSet_t *pos_ptr, DF_PositionConfidenceSet_st_ptr Pos_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((pos_ptr == NULL) || (Pos_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(pos_ptr, 0, sizeof(*pos_ptr));

    pos_ptr->pos = Pos_ptr->pos;
    if(Pos_ptr->opt.PositionConfidenceSet_opt_confidence_elev == MSG_OPTIONAL_YES)
    {
        pos_ptr->elevation = CALLOC(1, sizeof(*pos_ptr->elevation));
        *(pos_ptr->elevation) = Pos_ptr->elevation;
    }else{
        pos_ptr->elevation = NULL;
    }
    
    return result;

ERR_EXIT:

    DF_PositionConfidenceSet_free(pos_ptr);
    return result;
}


/* Parse routine for DF_PositionConfidenceSet. */
int DF_PositionConfidenceSet_parse(PositionConfidenceSet_t *pos_ptr, DF_PositionConfidenceSet_st_ptr Pos_ptr)
{
    int            result = 0;
    /* Error detect. */
    if((pos_ptr == NULL) || (Pos_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Pos_ptr, 0, sizeof(*Pos_ptr));

    Pos_ptr->pos = pos_ptr->pos;
    if(pos_ptr->elevation != NULL)
    {
        Pos_ptr->opt.PositionConfidenceSet_opt_confidence_elev = MSG_OPTIONAL_YES;
        Pos_ptr->elevation = *pos_ptr->elevation;
    }
    
    return result;

ERR_EXIT:
    
    return result;
}


/* Allocate routine for DF_PositionOffsetLL. */
int DF_PositionOffsetLL_allocate(PositionOffsetLL_t *posoffsetll_ptr, DF_PositionOffsetLL_st_ptr PosOffsetLL_ptr)
{
    int        result = 0;

    
    /* Error detect. */
    if((posoffsetll_ptr == NULL) || (PosOffsetLL_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(posoffsetll_ptr, 0, sizeof(*posoffsetll_ptr));

    /* PositionOffsetLL. */
    switch(PosOffsetLL_ptr->choiceid)
    {
    case PositionOffsetLL_NOTHING:
    {
        posoffsetll_ptr->present = PositionOffsetLL_PR_NOTHING;
        break;
    }
    case PositionOffsetLL_position_LL1:
    {
        posoffsetll_ptr->present = PositionOffsetLL_PR_position_LL1;
        result = DF_Position_LL_24B_allocate(&posoffsetll_ptr->choice.position_LL1, &PosOffsetLL_ptr->choice.position_LL1);
        break;
    }
    case PositionOffsetLL_position_LL2:
    {
        posoffsetll_ptr->present = PositionOffsetLL_PR_position_LL2;
        result = DF_Position_LL_28B_allocate(&posoffsetll_ptr->choice.position_LL2, &PosOffsetLL_ptr->choice.position_LL2);
        break;
    }
    case PositionOffsetLL_position_LL3:
    {
        posoffsetll_ptr->present = PositionOffsetLL_PR_position_LL3;
        result = DF_Position_LL_32B_allocate(&posoffsetll_ptr->choice.position_LL3, &PosOffsetLL_ptr->choice.position_LL3);
        break;
    }
    case PositionOffsetLL_position_LL4:
    {
        posoffsetll_ptr->present = PositionOffsetLL_PR_position_LL4;
        result = DF_Position_LL_36B_allocate(&posoffsetll_ptr->choice.position_LL4, &PosOffsetLL_ptr->choice.position_LL4);
        break;
    }
    case PositionOffsetLL_position_LL5:
    {
        posoffsetll_ptr->present = PositionOffsetLL_PR_position_LL5;
        result = DF_Position_LL_44B_allocate(&posoffsetll_ptr->choice.position_LL5, &PosOffsetLL_ptr->choice.position_LL5);
        break;
    }
    case PositionOffsetLL_position_LL6:
    {
        posoffsetll_ptr->present = PositionOffsetLL_PR_position_LL6;
        result = DF_Position_LL_48B_allocate(&posoffsetll_ptr->choice.position_LL6, &PosOffsetLL_ptr->choice.position_LL6);
        break;
    }
    case PositionOffsetLL_position_LatLon:
    {
        posoffsetll_ptr->present = PositionOffsetLL_PR_position_LatLon;
        result = DF_Position_LLmD_64B_allocate(&posoffsetll_ptr->choice.position_LatLon, &PosOffsetLL_ptr->choice.position_LatLon);
        break;
    }
    }
    
    return result;

ERR_EXIT:

    return result;
}


/* Parse routine for DF_PositionOffsetLL. */
int DF_PositionOffsetLL_parse(PositionOffsetLL_t *posoffsetll_ptr, DF_PositionOffsetLL_st_ptr PosOffsetLL_ptr)
{  
    int            result = 0;

    
    /* Error detect. */
    if((posoffsetll_ptr == NULL) || (PosOffsetLL_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(PosOffsetLL_ptr, 0, sizeof(*PosOffsetLL_ptr));

    /* PositonOffsetLL. */
    /* PositionOffsetLL. */
    switch(posoffsetll_ptr->present)
    {
    case PositionOffsetLL_PR_NOTHING:
    {
        PosOffsetLL_ptr->choiceid = PositionOffsetLL_NOTHING;
        break;
    }
    case PositionOffsetLL_PR_position_LL1:
    {
        PosOffsetLL_ptr->choiceid= PositionOffsetLL_position_LL1;
        result = DF_Position_LL_24B_parse(&posoffsetll_ptr->choice.position_LL1, &PosOffsetLL_ptr->choice.position_LL1);
        break;
    }
    case PositionOffsetLL_PR_position_LL2:
    {
        PosOffsetLL_ptr->choiceid = PositionOffsetLL_position_LL2;
        result = DF_Position_LL_28B_parse(&posoffsetll_ptr->choice.position_LL2, &PosOffsetLL_ptr->choice.position_LL2);
        break;
    }
    case PositionOffsetLL_PR_position_LL3:
    {
        PosOffsetLL_ptr->choiceid = PositionOffsetLL_position_LL3;
        result = DF_Position_LL_32B_parse(&posoffsetll_ptr->choice.position_LL3, &PosOffsetLL_ptr->choice.position_LL3);
        break;
    }
    case PositionOffsetLL_PR_position_LL4:
    {
        PosOffsetLL_ptr->choiceid = PositionOffsetLL_position_LL4;
        result = DF_Position_LL_36B_parse(&posoffsetll_ptr->choice.position_LL4, &PosOffsetLL_ptr->choice.position_LL4);
        break;
    }
    case PositionOffsetLL_PR_position_LL5:
    {
        PosOffsetLL_ptr->choiceid = PositionOffsetLL_position_LL5;
        result = DF_Position_LL_44B_parse(&posoffsetll_ptr->choice.position_LL5, &PosOffsetLL_ptr->choice.position_LL5);
        break;
    }
    case PositionOffsetLL_PR_position_LL6:
    {
        PosOffsetLL_ptr->choiceid = PositionOffsetLL_position_LL6;
        result = DF_Position_LL_48B_parse(&posoffsetll_ptr->choice.position_LL6, &PosOffsetLL_ptr->choice.position_LL6);
        break;
    }
    case PositionOffsetLL_PR_position_LatLon:
    {
        PosOffsetLL_ptr->choiceid = PositionOffsetLL_position_LatLon;
        result = DF_Position_LL_64B_parse(&posoffsetll_ptr->choice.position_LatLon, &PosOffsetLL_ptr->choice.position_LatLon);
        break;
    }
    }
    
    return result;

ERR_EXIT:

    return result;
}


/* Free extention routine for DF_PositionOffsetLLV. Diff with DF_PositionOffsetLLV_free() and this will free the point itself. */
int DF_PositionOffsetLLV_freeExt(PositionOffsetLLV_t *posoffsetllv_ptr)
{
    if(posoffsetllv_ptr != NULL)
    {
        /* Free the path offset llv point content and point itself. */
        DF_PositionOffsetLLV_free(posoffsetllv_ptr);
        FREEMEM(posoffsetllv_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}


/* Free routine for DF_PositionOffsetLLV*/
int DF_PositionOffsetLLV_free(PositionOffsetLLV_t *posoffsetllv_ptr)
{
    int result = 0;

    
    if(posoffsetllv_ptr != NULL)
    {
        if(posoffsetllv_ptr->offsetV != NULL)
        {
            result = DF_VerticalOffset_free(posoffsetllv_ptr->offsetV);
            if(result)
                return result;
            
            FREEMEM(posoffsetllv_ptr->offsetV);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(posoffsetllv_ptr, 0, sizeof(*posoffsetllv_ptr));
    }

      return result;
}


/* Allocate routine for DF_PositionOffsetLLV. */
int DF_PositionOffsetLLV_allocate(PositionOffsetLLV_t *posoffsetllv_ptr, DF_PositionOffsetLLV_st_ptr PosOffsetLLV_ptr)
{
    int        result = 0;

    
    /* Error detect. */
    if((posoffsetllv_ptr == NULL) || (PosOffsetLLV_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(posoffsetllv_ptr, 0, sizeof(*posoffsetllv_ptr));

    /* position offset LL . */
    if((result = DF_PositionOffsetLL_allocate(&posoffsetllv_ptr->offsetLL, &PosOffsetLLV_ptr->offsetLL)) != 0)
    {
        goto ERR_EXIT;
    }

    /* vertical offset LL . */
    if(PosOffsetLLV_ptr->opt.PositionOffsetLLV_opt_VerticalOffset == MSG_OPTIONAL_YES)
    {
    
        posoffsetllv_ptr->offsetV = CALLOC(1, sizeof(*posoffsetllv_ptr->offsetV));
        if(posoffsetllv_ptr->offsetV == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        if((result = DF_VerticalOffset_allocate(posoffsetllv_ptr->offsetV, &PosOffsetLLV_ptr->offsetV)) != 0)
        {
            goto ERR_EXIT;
        }
        
    }
    else
    {
        posoffsetllv_ptr->offsetV = NULL;
    }
    return result;

ERR_EXIT:

    DF_PositionOffsetLLV_free(posoffsetllv_ptr);
    return result;
}


/* Parse routine for DF_PositionOffsetLLV. */
int DF_PositionOffsetLLV_parse(PositionOffsetLLV_t *posoffsetllv_ptr, DF_PositionOffsetLLV_st_ptr PosOffsetLLV_ptr)
{  
    int            result = 0;

    
    /* Error detect. */
    if((posoffsetllv_ptr == NULL) || (PosOffsetLLV_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(PosOffsetLLV_ptr, 0, sizeof(*PosOffsetLLV_ptr));

    /* Position Offset LL. */
    if((result = DF_PositionOffsetLL_parse(&(posoffsetllv_ptr->offsetLL), &(PosOffsetLLV_ptr->offsetLL))) != 0)
    {
        goto ERR_EXIT;
    }

    /* Vertical Offset LL. */
    if(posoffsetllv_ptr->offsetV != NULL)
    {
    
        PosOffsetLLV_ptr->opt.PositionOffsetLLV_opt_VerticalOffset = MSG_OPTIONAL_YES;
        
        if((result = DF_VerticalOffset_parse(posoffsetllv_ptr->offsetV, &(PosOffsetLLV_ptr->offsetV))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    
    return result;

ERR_EXIT:

    return result;
}

/* Free routine for DF_PointList. */
int DF_PointList_free(PointList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_PointList. */
int DF_PointList_allocate(PointList_t *list_ptr, DF_PointList_st_ptr List_ptr)
{
    int               result = 0;
    uint8_t      point_index = 0;
    RoadPoint_t   * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* PhaseStateList. */
    list_ptr->list.free = (void (*)(RoadPoint_t *))DF_RoadPoint_freeExt;
    
    for(point_index = 0; point_index < List_ptr->PointList_size; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if(DF_RoadPoint_allocate(elem_ptr, &(List_ptr->PointList_data[point_index])) == 0)
            {
                /* Add point element to sequence. */
                if(asn_sequence_add(&(list_ptr->list), elem_ptr) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_RoadPoint_free(elem_ptr);
                    FREEMEM(elem_ptr);

                    result = -ERR_SYS;
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);

                result = -ERR_SYS;
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_PointList_free(list_ptr);
    return result;
}


/* Parse routine for DF_PointList. */
int DF_PointList_parse(PointList_t *list_ptr, DF_PointList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* PhaseStateList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_RoadPoint_parse(list_ptr->list.array[point_index], &(List_ptr->PointList_data[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->PointList_size = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}

/* Free routine for DF_RegulatorySpeedLimit*/
int DF_RegulatorySpeedLimit_free(RegulatorySpeedLimit_t *speedlimit_ptr)
{
    int result = 0;
    
    if(speedlimit_ptr != NULL)
    {
        /* Must clear all the zone and avoid repeat free operation. */
        memset(speedlimit_ptr, 0, sizeof(*speedlimit_ptr));
    }

      return result;
}

/* Free extention routine for DF_RegulatorySpeedLimit. Diff with DF_RegulatorySpeedLimit_free() and this will free the point itself. */
int DF_RegulatorySpeedLimit_freeExt(RegulatorySpeedLimit_t *speedlimit_ptr)
{
    if(speedlimit_ptr != NULL)
    {
        /* Free the path  point content and point itself. */
        DF_RegulatorySpeedLimit_free(speedlimit_ptr);
        FREEMEM(speedlimit_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}

/* Allocate routine for DF_RegulatorySpeedLimit. */
int DF_RegulatorySpeedLimit_allocate(RegulatorySpeedLimit_t *speedlimit_ptr, DF_RegulatorySpeedLimit_st_ptr Speedlimit_ptr)
{
    int        result = 0;

    
    /* Error detect. */
    if((speedlimit_ptr == NULL) || (Speedlimit_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(speedlimit_ptr, 0, sizeof(*speedlimit_ptr));

    speedlimit_ptr->type = Speedlimit_ptr->type;

    speedlimit_ptr->speed = bsctyp_encode(DE_Speed, Speedlimit_ptr->speed);

    return result;

ERR_EXIT:

    DF_RegulatorySpeedLimit_free(speedlimit_ptr);
    return result;
}


/* Parse routine for DF_RegulatorySpeedLimit. */
int DF_RegulatorySpeedLimit_parse(RegulatorySpeedLimit_t *speedlimit_ptr, DF_RegulatorySpeedLimit_st_ptr Speedlimit_ptr)
{  
    int            result = 0;

    /* Error detect. */
    if((speedlimit_ptr == NULL) || (Speedlimit_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Speedlimit_ptr, 0, sizeof(*Speedlimit_ptr));

    Speedlimit_ptr->type = speedlimit_ptr->type;

    Speedlimit_ptr->speed = bsctyp_decode(DE_Speed, speedlimit_ptr->speed);
    
    return result;

ERR_EXIT:

    return result;
}


/* Free routine for DF_RoadPoint*/
int DF_RoadPoint_free(RoadPoint_t *roadpoint_ptr)
{
    int result = 0;

    
    if(roadpoint_ptr != NULL)
    {
        result = DF_PositionOffsetLLV_free(&(roadpoint_ptr->posOffset));
        if(result)
            return result;

        /* Must clear all the zone and avoid repeat free operation. */
        memset(roadpoint_ptr, 0, sizeof(*roadpoint_ptr));
    }

    return result;
}

/* Free extention routine for DF_RoadPoint. Diff with DF_RoadPoint_free() and this will free the point itself. */
int DF_RoadPoint_freeExt(RoadPoint_t *roadpoint_ptr)
{
    if(roadpoint_ptr != NULL)
    {
        /* Free the path  point content and point itself. */
        DF_RoadPoint_free(roadpoint_ptr);
        FREEMEM(roadpoint_ptr);

        return 0;
    }
    else
    {
        return -1;
    }
}

/* Allocate routine for DF_RoadPoint. */
int DF_RoadPoint_allocate(RoadPoint_t *roadpoint_ptr, DF_RoadPoint_st_ptr Roadpoint_ptr)
{
    int        result = 0;

    
    /* Error detect. */
    if((roadpoint_ptr == NULL) || (Roadpoint_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(roadpoint_ptr, 0, sizeof(*roadpoint_ptr));

    /* PositionOffsetLLV. */
    if((result = DF_PositionOffsetLLV_allocate(&(roadpoint_ptr->posOffset), &(Roadpoint_ptr->posOffset))) != 0)
    {
        goto ERR_EXIT;
    }

    return result;

ERR_EXIT:

    DF_RoadPoint_free(roadpoint_ptr);
    return result;
}


/* Parse routine for DF_RoadPoint. */
int DF_RoadPoint_parse(RoadPoint_t *roadpoint_ptr, DF_RoadPoint_st_ptr Roadpoint_ptr)
{  
    int            result = 0;

    /* Error detect. */
    if((roadpoint_ptr == NULL) || (Roadpoint_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Roadpoint_ptr, 0, sizeof(*Roadpoint_ptr));

    /* Position Offset LL. */
    if((result = DF_PositionOffsetLLV_parse(&(roadpoint_ptr->posOffset), &(Roadpoint_ptr->posOffset))) != 0)
    {
        goto ERR_EXIT;
    }
    
    return result;

ERR_EXIT:

    return result;
}

/* Free routine for DF_SpeedLimitList. */
int DF_SpeedLimitList_free(SpeedLimitList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}

/* Allocate routine for DF_SpeedLimitList. */
int DF_SpeedLimitList_allocate(SpeedLimitList_t *list_ptr, DF_SpeedLimitList_st_ptr List_ptr)
{
    int                          result = 0;
    uint8_t                 point_index = 0;
    RegulatorySpeedLimit_t   * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* PhaseStateList. */
    list_ptr->list.free = (void (*)(RegulatorySpeedLimit_t *))DF_RegulatorySpeedLimit_freeExt;
    
    for(point_index = 0; point_index < List_ptr->SpeedLimitList_size; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            if(DF_RegulatorySpeedLimit_allocate(elem_ptr, &(List_ptr->SpeedLimitList_data[point_index])) == 0)
            {
                /* Add point element to sequence. */
                if(asn_sequence_add(&(list_ptr->list), elem_ptr) != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DF_RegulatorySpeedLimit_free(elem_ptr);
                    FREEMEM(elem_ptr);

                    result = -ERR_SYS;
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);

                result = -ERR_SYS;
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_SpeedLimitList_free(list_ptr);
    return result;
}


/* Parse routine for DF_SpeedLimitList. */
int DF_SpeedLimitList_parse(SpeedLimitList_t *list_ptr, DF_SpeedLimitList_st_ptr List_ptr)
{  
    int           result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* PhaseStateList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        result = DF_RegulatorySpeedLimit_parse(list_ptr->list.array[point_index], &(List_ptr->SpeedLimitList_data[point_index]));
        if(result)
            goto ERR_EXIT;
    }
    List_ptr->SpeedLimitList_size = point_index;
    
    result = ERR_OK;

ERR_EXIT:

    return result;
}


#if 0
/* Free routine for DF_RTCMheader. */
int DF_RTCMheader_free(RTCMheader_t *header_ptr)
{
    if(header_ptr != NULL)
    {
        DE_GNSSstatus_free(&(header_ptr->status));    
        DF_AntennaOffsetSet_free(&(header_ptr->offsetSet));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(header_ptr, 0, sizeof(*header_ptr));
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_RTCMheader. */
int DF_RTCMheader_allocate(RTCMheader_t *header_ptr, DF_RTCMheader_st_ptr Header_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((header_ptr == NULL) || (Header_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(header_ptr, 0, sizeof(*header_ptr));

    /* GNSSstatus. */
    result = DE_GNSSstatus_allocate(&(header_ptr->status), &(Header_ptr->status));
    if(result!= 0)
    {
        goto ERR_EXIT;
    }

    /* AntennaOffsetSet. */
    result = DF_AntennaOffsetSet_allocate(&(header_ptr->offsetSet), &(Header_ptr->offsetSet));
    if(result!= 0)
    {
        goto ERR_EXIT;
    }
    
    return result;

ERR_EXIT:

    DF_RTCMheader_free(header_ptr);
    return result;
}


/* Parse routine for DF_RTCMheader. */
int DF_RTCMheader_parse(RTCMheader_t *header_ptr, DF_RTCMheader_st_ptr Header_ptr)
{  
    int            result = 0;
    /* Error detect. */
    if((header_ptr == NULL) || (Header_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Header_ptr, 0, sizeof(*Header_ptr));

    /* GNSSstatus. */
    result = DE_GNSSstatus_parse(&(header_ptr->status), &(Header_ptr->status));
    if(result!= 0)
    {
        goto ERR_EXIT;
    }

    /* PathHistoryPointList. */
    result = DF_AntennaOffsetSet_parse(&(header_ptr->offsetSet), &(Header_ptr->offsetSet));
    if(result != 0)
    {
        goto ERR_EXIT;
    }
    
    return result;

ERR_EXIT:

    return result;
}


/* Free routine for DF_RTCMmessageList. */
int DF_RTCMmessageList_free(RTCMmessageList_t *list_ptr)
{
    if(list_ptr != NULL)
    {
        /* Free the contents of the sequence,not the sequence itself. */
        asn_sequence_empty(&(list_ptr->list));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(list_ptr, 0, sizeof(*list_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_RTCMmessageList. */
int DF_RTCMmessageList_allocate(RTCMmessageList_t *list_ptr, DF_RTCMmessageList_st_ptr List_ptr)
{
    int            result = 0;
    uint8_t       point_index = 0;
    RTCMmessage_t  * elem_ptr = NULL;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL) 
    || ((sizeof(List_ptr->array) / sizeof(List_ptr->array[0])) < List_ptr->messageNum))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(list_ptr, 0, sizeof(*list_ptr));

    /* PathHistoryPointList. */
    list_ptr->list.free = (void (*)(RTCMmessage_t *))DE_RTCMmessage_freeExt;
    
    for(point_index = 0; point_index < List_ptr->messageNum; point_index ++)
    {
        if((elem_ptr = CALLOC(1, sizeof(*elem_ptr))) != NULL)
        {
            result = DE_RTCMmessage_allocate(elem_ptr, &(List_ptr->array[point_index]));
            if(result == 0)
            {
                /* Add point element to sequence. */
                result = asn_sequence_add(&(list_ptr->list), elem_ptr);
                if(result != 0)
                {
                    /* Free the point's content and point itself when add failure. */
                    DE_RTCMmessage_free(elem_ptr);
                    FREEMEM(elem_ptr);
                    
                    goto ERR_EXIT;
                }   
            }
            else
            {
                /* Free the point itself when allocate point's content failure. */
                FREEMEM(elem_ptr);
                
                goto ERR_EXIT;
            }
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }

    return result;

ERR_EXIT:

    DF_RTCMmessageList_free(list_ptr);
    return result;
}


/* Parse routine for DF_RTCMmessageList. */
int DF_RTCMmessageList_parse(RTCMmessageList_t *list_ptr, DF_RTCMmessageList_st_ptr List_ptr)
{  
    int        result = 0;
    uint8_t  point_index = 0;
  

    /* Error detect. */
    if((list_ptr == NULL) || (List_ptr == NULL) 
    || ((sizeof(List_ptr->array) / sizeof(List_ptr->array[0])) < list_ptr->list.count))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(List_ptr, 0, sizeof(*List_ptr));

    /* PathHistoryPointList. */
    for(point_index = 0; point_index < list_ptr->list.count; point_index ++)
    {
        DE_RTCMmessage_parse(list_ptr->list.array[point_index], &(List_ptr->array[point_index]));
    }
    List_ptr->messageNum = point_index;
    
    return result;

ERR_EXIT:

    return result;
}

/* Free routine for DF_SpecialVehicleExtensions. */
int DF_SpecialVehicleExtensions_free(SpecialVehicleExtensions_t * vecext_ptr)
{
    return -1;    
}


/* Allocate routine for DF_SpecialVehicleExtensions. */
int DF_SpecialVehicleExtensions_allocate(SpecialVehicleExtensions_t * vecext_ptr, DF_SpecialVehicleExtensions_st_ptr vecExt_ptr)
{
    return -1;
}


/* Parse routine for DF_SpecialVehicleExtensions. */
int DF_SpecialVehicleExtensions_parse(SpecialVehicleExtensions_t * vecext_ptr, DF_SpecialVehicleExtensions_st_ptr vecExt_ptr)
{  
    return -1;
}


/* Free routine for DF_SpeedHeadingThrottleConfidence. */
int DF_SpeedHeadingThrottleConfidence_free(SpeedandHeadingandThrottleConfidence_t *conf_ptr)
{
    if(conf_ptr != NULL)
    {
        /* Must clear all the zone and avoid repeat free operation. */
        memset(conf_ptr, 0, sizeof(*conf_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_SpeedHeadingThrottleConfidence. */
int DF_SpeedHeadingThrottleConfidence_allocate(SpeedandHeadingandThrottleConfidence_t *conf_ptr, DF_SpeedHeadingThrottleConf_st_ptr Conf_ptr)
{
    int result = 0;
    /* Error detect. */
    if((conf_ptr == NULL) || (Conf_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(conf_ptr, 0, sizeof(*conf_ptr));

    conf_ptr->heading = Conf_ptr->heading;
    conf_ptr->speed = Conf_ptr->speed;
    conf_ptr->throttle = Conf_ptr->throttle;    

    return result;

ERR_EXIT:

    DF_SpeedHeadingThrottleConfidence_free(conf_ptr);
    return result;
}


/* Parse routine for DF_SpeedHeadingThrottleConfidence. */
int DF_SpeedHeadingThrottleConfidence_parse(SpeedandHeadingandThrottleConfidence_t *conf_ptr, DF_SpeedHeadingThrottleConf_st_ptr Conf_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((conf_ptr == NULL) || (Conf_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Conf_ptr, 0, sizeof(*Conf_ptr));

    Conf_ptr->heading = conf_ptr->heading;
    Conf_ptr->speed = conf_ptr->speed;
    Conf_ptr->throttle = conf_ptr->throttle;    
     
    return result;

ERR_EXIT:
    
    return result;
}


/* Free routine for DF_SupplementalVehicleExtensions. */
int DF_SupplementalVehicleExtensions_free(VehicleDummyStruct_t * vecext_ptr)
{
    return -1;
}


/* allocate routine for DF_SupplementalVehicleExtensions. */
int DF_SupplementalVehicleExtensions_allocate(VehicleDummyStruct_t * vecext_ptr, DF_SupplementalVehicleExtensions_st_ptr vecExt_ptr)
{
    return -1;
}


/* Parse routine for DF_SupplementalVehicleExtensions. */
int DF_SupplementalVehicleExtensions_parse(VehicleDummyStruct_t * vecext_ptr, DF_SupplementalVehicleExtensions_st_ptr vecExt_ptr)
{  
    return -1;
}


/* Free routine for DF_TransmissionAndSpeed. */
int DF_TransmissionAndSpeed_free(TransmissionAndSpeed_t *trans_ptr)
{
    if(trans_ptr != NULL)
    {
        /* Must clear all the zone and avoid repeat free operation. */
        memset(trans_ptr, 0, sizeof(*trans_ptr));
        
        return 0;
    }
    else
    {
        return -1;
    }
}



/* Allocate routine for DF_TransmissionAndSpeed. */
int DF_TransmissionAndSpeed_allocate(TransmissionAndSpeed_t *trans_ptr, DF_TransmissionAndSpeed_st_ptr Trans_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((trans_ptr == NULL) || (Trans_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(trans_ptr, 0, sizeof(*trans_ptr));

    trans_ptr->transmisson = Trans_ptr->transmission;
    trans_ptr->speed = bsctyp_encode(DE_Velocity, Trans_ptr->speed);
    
    return result;

ERR_EXIT:

    DF_TransmissionAndSpeed_free(trans_ptr);
    return result;
}


/* Parse routine for DF_TransmissionAndSpeed. */
int DF_TransmissionAndSpeed_parse(TransmissionAndSpeed_t *trans_ptr, DF_TransmissionAndSpeed_st_ptr Trans_ptr)
{
    int        result = 0;
    /* Error detect. */
    if((trans_ptr == NULL) || (Trans_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(Trans_ptr, 0, sizeof(*Trans_ptr));

    Trans_ptr->transmission = trans_ptr->transmisson;
    Trans_ptr->speed = bsctyp_decode(DE_Velocity, trans_ptr->speed);
    
    return result;

ERR_EXIT:
    
    return result;
}
#endif


/* Free routine for DF_TimeChangeDetails. */
int DF_TimeChangeDetails_free(TimeChangeDetails_t *tcd_ptr)
{
    int    result = 0;


    if(tcd_ptr != NULL)
    {
        if(tcd_ptr->minEndTime != NULL)
        {
            FREEMEM(tcd_ptr->minEndTime);
        }

        if(tcd_ptr->maxEndTime != NULL)
        {
            FREEMEM(tcd_ptr->maxEndTime);
        }

        if(tcd_ptr->timeConfidence != NULL)
        {
            FREEMEM(tcd_ptr->timeConfidence);
        }

        if(tcd_ptr->nextStartTime != NULL)
        {
            FREEMEM(tcd_ptr->nextStartTime);
        }

        if(tcd_ptr->nextDuration != NULL)
        {
            FREEMEM(tcd_ptr->nextDuration);
        }

        memset(tcd_ptr, 0x00, sizeof(*tcd_ptr));

        result = 0;
    }
    else
    {
        result = 1;
    }

    return result;
}


/* Allocate routine for DF_TimeChangeDetails.*/
int DF_TimeChangeDetails_allocate(TimeChangeDetails_t *tcd_ptr, DF_TimeChangeDetials_st_ptr Tcd_ptr)
{
    int    result = 0;


    /* Error detection. */
    if((tcd_ptr == NULL) || (Tcd_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(tcd_ptr, 0x00, sizeof(*tcd_ptr));

    /* Start time. */
    tcd_ptr->startTime = bsctyp_encode(DE_TimeMark, Tcd_ptr->startTime);

    /* Min end time. */
    if(Tcd_ptr->opt.minEndTime == MSG_OPTIONAL_YES)
    {
        if((tcd_ptr->minEndTime = CALLOC(1, sizeof(*(tcd_ptr->minEndTime)))) != NULL)
        {
            *(tcd_ptr->minEndTime) = bsctyp_encode(DE_TimeMark, Tcd_ptr->minEndTime);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        tcd_ptr->minEndTime = NULL;
    }

    /* Max end time. */
    if(Tcd_ptr->opt.maxEndTime == MSG_OPTIONAL_YES)
    {
        if((tcd_ptr->maxEndTime = CALLOC(1, sizeof(*(tcd_ptr->maxEndTime)))) != NULL)
        {
            *(tcd_ptr->maxEndTime) = bsctyp_encode(DE_TimeMark, Tcd_ptr->maxEndTime);
        }
        else
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        tcd_ptr->maxEndTime = NULL;
    }

    /* Likely end time. */
    tcd_ptr->likelyEndTime = bsctyp_encode(DE_TimeMark, Tcd_ptr->likelyEndTime);

    /* Time confidence. */
    if(Tcd_ptr->opt.TimeConfidence == MSG_OPTIONAL_YES)
    {
        if((tcd_ptr->timeConfidence = CALLOC(1, sizeof(*(tcd_ptr->timeConfidence)))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *(tcd_ptr->timeConfidence) = bsctyp_encode(DE_Confidence, Tcd_ptr->timeConfidence);
    }
    else
    {
        tcd_ptr->timeConfidence = NULL;
    }

    /* Next start time. */
    if(Tcd_ptr->opt.nextStartTime == MSG_OPTIONAL_YES)
    {
        if((tcd_ptr->nextStartTime = CALLOC(1, sizeof(*(tcd_ptr->nextStartTime)))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *(tcd_ptr->nextStartTime) = bsctyp_encode(DE_TimeMark, Tcd_ptr->nextStartTime);
    }
    else
    {
        tcd_ptr->nextStartTime = NULL;
    }

    /* Next duration. */
    if(Tcd_ptr->opt.nextDuration == MSG_OPTIONAL_YES)
    {
        if((tcd_ptr->nextDuration = CALLOC(1, sizeof(*(tcd_ptr->nextDuration)))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }

        *(tcd_ptr->nextDuration) = bsctyp_encode(DE_TimeMark, Tcd_ptr->nextDuration);
    }
    else
    {
        tcd_ptr->nextDuration = NULL;
    }
    
    return result;
    
ERR_EXIT:
    
    DF_TimeChangeDetails_free(tcd_ptr);
    return result;
}


/* Parse routine for DF_TimeChangeDetails. */
int DF_TimeChangeDetails_parse(TimeChangeDetails_t *tcd_ptr, DF_TimeChangeDetials_st_ptr Tcd_ptr)
{
    int    result = 0;


    /* Error detection. */
    if((tcd_ptr == NULL) || (Tcd_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* clear  Tcd_ptr memory */
    memset(Tcd_ptr, 0x00, sizeof(*Tcd_ptr));

    /* Start time. */
    Tcd_ptr->startTime = bsctyp_decode(DE_TimeMark, tcd_ptr->startTime);

    /* Min end time. */
    if(tcd_ptr->minEndTime != NULL)
    {
        Tcd_ptr->opt.minEndTime = MSG_OPTIONAL_YES;
        Tcd_ptr->minEndTime = bsctyp_decode(DE_TimeMark, *(tcd_ptr->minEndTime));
    }

    /* Max end time. */
    if(tcd_ptr->maxEndTime != NULL)
    {
        Tcd_ptr->opt.maxEndTime = MSG_OPTIONAL_YES;
        Tcd_ptr->maxEndTime = bsctyp_decode(DE_TimeMark, *(tcd_ptr->maxEndTime));
    }

    /* Likely end time. */
    Tcd_ptr->likelyEndTime = bsctyp_decode(DE_TimeMark, tcd_ptr->likelyEndTime);

    /* Time confidence. */
    if(tcd_ptr->timeConfidence != NULL)
    {
        Tcd_ptr->opt.TimeConfidence = MSG_OPTIONAL_YES;
        Tcd_ptr->timeConfidence = bsctyp_decode(DE_Confidence, *(tcd_ptr->timeConfidence));    
    }

    /* Next start time. */
    if(tcd_ptr->nextStartTime != NULL)
    {
        Tcd_ptr->opt.nextStartTime = MSG_OPTIONAL_YES;
        Tcd_ptr->nextStartTime = bsctyp_decode(DE_TimeMark, *(tcd_ptr->nextStartTime));    
    }

    /* Next duration. */
    if(tcd_ptr->nextDuration != NULL)
    {
        Tcd_ptr->opt.nextDuration = MSG_OPTIONAL_YES;
        Tcd_ptr->nextDuration = bsctyp_decode(DE_TimeMark, *(tcd_ptr->nextDuration));
    }
    
ERR_EXIT:

    return result;
}


/* Free routine for DF_VehicleClassification. */
int DF_VehicleClassification_free(VehicleClassification_t* vehicleclass_ptr)
{
    int result = 0;

    
    if(vehicleclass_ptr != NULL)
    {
        /* Must clear all the zone and avoid repeat free operation. */
        memset(vehicleclass_ptr, 0, sizeof(*vehicleclass_ptr));    
    }  

    return result;
}


/* Allocate routine for DF_VehicleClassification. */
int DF_VehicleClassification_allocate(VehicleClassification_t* vehicleclass_ptr , DF_VehicleClassification_st_ptr VehicleClass_ptr)
{
    int result = 0;

    
    /* Error detect. */
    if((vehicleclass_ptr == NULL) || (VehicleClass_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(vehicleclass_ptr, 0, sizeof(*vehicleclass_ptr));

    /* BasicVehicleClass. */
    vehicleclass_ptr->classification = VehicleClass_ptr->classification;

    return result;
    
ERR_EXIT:

    DF_VehicleClassification_free(vehicleclass_ptr);
    
    return result;
}


/* Parse routine for DF_VehicleClassification. */
int DF_VehicleClassification_parse(VehicleClassification_t* vehicleclass_ptr , DF_VehicleClassification_st_ptr VehicleClass_ptr)
{  
    int result = 0;

    
    /* Error detect. */
    if((vehicleclass_ptr == NULL) || (VehicleClass_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(VehicleClass_ptr, 0, sizeof(*VehicleClass_ptr));

    /* BasicVehicleClass. */
    VehicleClass_ptr->classification = vehicleclass_ptr->classification;
    
ERR_EXIT:

    return result;
}


/* Free routine for DF_VehicleSafetyExtensions. */
int DF_VehicleSafetyExtensions_free(VehicleSafetyExtensions_t * vecsafety_ptr)
{
    int result = 0;

    if(vecsafety_ptr != NULL)
    {
        /* Events. */
        if(vecsafety_ptr->events != NULL)
        {
            DE_VehicleEventFlags_free(vecsafety_ptr->events); 
            FREEMEM(vecsafety_ptr->events);
        }

        /* Path history. */
        if(vecsafety_ptr->pathHistory != NULL)
        {
            DF_PathHistory_free(vecsafety_ptr->pathHistory);
            FREEMEM(vecsafety_ptr->pathHistory);
        }

        /* Path prediction. */
        if(vecsafety_ptr->pathPrediction != NULL)
        {
            DF_PathPrediction_free(vecsafety_ptr->pathPrediction);
            FREEMEM(vecsafety_ptr->pathPrediction);
        }
        
        /* Lights. */
        if(vecsafety_ptr->lights != NULL)
        {
            DE_ExteriorLights_free(vecsafety_ptr->lights);
            FREEMEM(vecsafety_ptr->lights);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(vecsafety_ptr, 0, sizeof(*vecsafety_ptr));

       
    }  

      return result;
}



/* Allocate routine for DF_VehicleSafetyExtensions. */
int DF_VehicleSafetyExtensions_allocate(VehicleSafetyExtensions_t * vecsafety_ptr, DF_VehicleSafetyExtensions_st_ptr vecSafety_ptr)
{
    int result = 0;
    
    /* Error detect. */
    if((vecsafety_ptr == NULL) || (vecSafety_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(vecsafety_ptr, 0, sizeof(*vecsafety_ptr));

    /* Events. */
    if(vecSafety_ptr->opt.VehicleSafetyExternsions_opt_events == MSG_OPTIONAL_YES)
    {
        if((vecsafety_ptr->events = CALLOC(1, sizeof(* vecsafety_ptr->events))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        if((result = DE_VehicleEventFlags_allocate(vecsafety_ptr->events, &(vecSafety_ptr->events))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        vecsafety_ptr->events = NULL;
    }

    /* Path history. */
    if(vecSafety_ptr->opt.VehicleSafetyExternsions_opt_pathHistory == MSG_OPTIONAL_YES)
    {
        if((vecsafety_ptr->pathHistory = CALLOC(1, sizeof(* vecsafety_ptr->pathHistory))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        if((result = DF_PathHistory_allocate(vecsafety_ptr->pathHistory, &(vecSafety_ptr->pathHistory))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        vecsafety_ptr->pathHistory = NULL;
    }

    /* Path prediction. */
    if(vecSafety_ptr->opt.VehicleSafetyExternsions_opt_pathPrediction == MSG_OPTIONAL_YES)
    {
        if((vecsafety_ptr->pathPrediction = CALLOC(1, sizeof(* vecsafety_ptr->pathPrediction))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        if((result = DF_PathPrediction_allocate(vecsafety_ptr->pathPrediction, &(vecSafety_ptr->pathPrediction))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        vecsafety_ptr->pathPrediction = NULL;
    }

    /* Lights. */
    if(vecSafety_ptr->opt.VehicleSafetyExternsions_opt_lights == MSG_OPTIONAL_YES)
    {
        if((vecsafety_ptr->lights = CALLOC(1, sizeof(* vecsafety_ptr->lights))) == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        if((result = DE_ExteriorLights_allocate(vecsafety_ptr->lights, &(vecSafety_ptr->lights))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        vecsafety_ptr->lights = NULL;
    }

    return result;
    
ERR_EXIT:

    DF_VehicleSafetyExtensions_free(vecsafety_ptr);
    
    return result;
}


/* Parse routine for DF_VehicleSafetyExtensions. */
int DF_VehicleSafetyExtensions_parse(VehicleSafetyExtensions_t * vecsafety_ptr, DF_VehicleSafetyExtensions_st_ptr vecSafety_ptr)
{  
    int result = 0;
        
    /* Error detect. */
    if((vecsafety_ptr == NULL) || (vecSafety_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(vecSafety_ptr, 0, sizeof(*vecSafety_ptr));

    /* Events. */
    if(vecsafety_ptr->events != NULL)
    {
        if((result = DE_VehicleEventFlags_parse(vecsafety_ptr->events, &(vecSafety_ptr->events))) == 0)
        {
            /* Set optional bit. */
            vecSafety_ptr->opt.VehicleSafetyExternsions_opt_events = MSG_OPTIONAL_YES;
        }
        else
        {
            goto ERR_EXIT;
        }
    }

    /* Path history. */
    if(vecsafety_ptr->pathHistory != NULL)
    {
        if((result = DF_PathHistory_parse(vecsafety_ptr->pathHistory, &(vecSafety_ptr->pathHistory))) == 0)
        {
            /* Set optional bit. */
            vecSafety_ptr->opt.VehicleSafetyExternsions_opt_pathHistory = MSG_OPTIONAL_YES;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    
    /* Path prediction. */
    if(vecsafety_ptr->pathPrediction != NULL)
    {
        if((result = DF_PathPrediction_parse(vecsafety_ptr->pathPrediction, &(vecSafety_ptr->pathPrediction))) == 0)
        {
            /* Set optional bit. */
            vecSafety_ptr->opt.VehicleSafetyExternsions_opt_pathPrediction = MSG_OPTIONAL_YES;
        }
        else
        {
            goto ERR_EXIT;
        }
    }

    /* Lights. */
    if(vecsafety_ptr->lights != NULL)
    {
        if((result = DE_ExteriorLights_parse(vecsafety_ptr->lights, &(vecSafety_ptr->lights))) == 0)
        {
            /* Set optional bit. */
            vecSafety_ptr->opt.VehicleSafetyExternsions_opt_lights = MSG_OPTIONAL_YES;
        }
        else
        {
            goto ERR_EXIT;
        }
    }

    return result;
    
ERR_EXIT:

    return result;
}


/* Free routine for DF_VehicleSize. */
int DF_VehicleSize_free(VehicleSize_t* vehiclesize_ptr)
{
    int result = 0;

    
    if(vehiclesize_ptr != NULL)
    {
        /* Heights. */
        if(vehiclesize_ptr->height != NULL)
        {
            FREEMEM(vehiclesize_ptr->height);
        }

        /* Must clear all the zone and avoid repeat free operation. */
        memset(vehiclesize_ptr, 0, sizeof(*vehiclesize_ptr));    
    }  

    return result;
}


/* Allocate routine for DF_VehicleSize. */
int DF_VehicleSize_allocate(VehicleSize_t* vehiclesize_ptr , DF_VehicleSize_st_ptr VehicleSize_ptr)
{
    int result = 0;
    /* Error detect. */
    if((vehiclesize_ptr == NULL) || (VehicleSize_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(vehiclesize_ptr, 0, sizeof(*vehiclesize_ptr));

    vehiclesize_ptr->width = bsctyp_encode(DE_VehicleWidth, VehicleSize_ptr->width);
    vehiclesize_ptr->length = bsctyp_encode(DE_VehicleLength, VehicleSize_ptr->length);

    /* Events. */
    if(VehicleSize_ptr->opt.vehiclesize_opt_height == MSG_OPTIONAL_YES)
    {
        vehiclesize_ptr->height = CALLOC(1, sizeof(*vehiclesize_ptr->height));
        if(vehiclesize_ptr->height == NULL)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        *vehiclesize_ptr->height = bsctyp_encode(DE_VehicleHeight, VehicleSize_ptr->height);
    }
    else
    {
        vehiclesize_ptr->height = NULL;
    }

    return result;
    
ERR_EXIT:

    DF_VehicleSize_free(vehiclesize_ptr);
    
    return result;
}


/* Parse routine for DF_VehicleSize. */
int DF_VehicleSize_parse(VehicleSize_t* vehiclesize_ptr , DF_VehicleSize_st_ptr VehicleSize_ptr)
{  
    int result = 0;

    
    /* Error detect. */
    if((vehiclesize_ptr == NULL) || (VehicleSize_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(VehicleSize_ptr, 0, sizeof(*VehicleSize_ptr));
    
    VehicleSize_ptr->width = bsctyp_decode(DE_VehicleWidth, vehiclesize_ptr->width);
    VehicleSize_ptr->length = bsctyp_decode(DE_VehicleLength, vehiclesize_ptr->length);
    /* Events. */
    if(vehiclesize_ptr->height != NULL)
    {
        VehicleSize_ptr->opt.vehiclesize_opt_height = MSG_OPTIONAL_YES;
        VehicleSize_ptr->height = bsctyp_decode(DE_VehicleHeight, *vehiclesize_ptr->height);
    }

    
ERR_EXIT:

    return result;
}


/* Free routine for DF_VerticalOffset. */
int DF_VerticalOffset_free(VerticalOffset_t *vecoffset_ptr)
{
    if(vecoffset_ptr != NULL)
    {
        /* Must clear all the zone and avoid repeat free operation. */
        memset(vecoffset_ptr, 0, sizeof(*vecoffset_ptr));
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate routine for DF_VerticalOffset. */
int DF_VerticalOffset_allocate(VerticalOffset_t *vecoffset_ptr, DF_VerticalOffset_st_ptr VecOffset_ptr)
{
    int        result = 0;

    
    /* Error detect. */
    if((vecoffset_ptr == NULL) || (VecOffset_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(vecoffset_ptr, 0, sizeof(*vecoffset_ptr));

    /* VehicleOffset. */
    switch(VecOffset_ptr->choiceid)
    {
        case VerticalOffset_NOTHING:
        {
            vecoffset_ptr->present = VerticalOffset_PR_NOTHING;
            break;
        }
        case VerticalOffset_offset1:
        {
            vecoffset_ptr->present = VerticalOffset_PR_offset1;
            vecoffset_ptr->choice.offset1 = bsctyp_encode(DE_VertOffset_B07, VecOffset_ptr->choice.offset1);
            break;
        }
        case VerticalOffset_offset2:
        {
            vecoffset_ptr->present = VerticalOffset_PR_offset2;
            vecoffset_ptr->choice.offset2 = bsctyp_encode(DE_VertOffset_B08, VecOffset_ptr->choice.offset2);
            break;
        }
        case VerticalOffset_offset3:
        {
            vecoffset_ptr->present = VerticalOffset_PR_offset3;
            vecoffset_ptr->choice.offset3 = bsctyp_encode(DE_VertOffset_B09, VecOffset_ptr->choice.offset3);
            break;
        }
        case VerticalOffset_offset4:
        {
            vecoffset_ptr->present = VerticalOffset_PR_offset4;
            vecoffset_ptr->choice.offset4 = bsctyp_encode(DE_VertOffset_B10, VecOffset_ptr->choice.offset4);
            break;
        }
        case VerticalOffset_offset5:
        {
            vecoffset_ptr->present = VerticalOffset_PR_offset5;
            vecoffset_ptr->choice.offset5 = bsctyp_encode(DE_VertOffset_B11, VecOffset_ptr->choice.offset5);
            break;
        }
        case VerticalOffset_offset6:
        {
            vecoffset_ptr->present = VerticalOffset_PR_offset6;
            vecoffset_ptr->choice.offset6 = bsctyp_encode(DE_VertOffset_B12, VecOffset_ptr->choice.offset6);
            break;
        }
        case VerticalOffset_elevation:
        {
            vecoffset_ptr->present = VerticalOffset_PR_elevation;
            vecoffset_ptr->choice.elevation = bsctyp_encode(DE_Elevation, VecOffset_ptr->choice.elevation);
            break;
        }
    }
    
    return result;

ERR_EXIT:

    DF_VerticalOffset_free(vecoffset_ptr);
    return result;
}


/* Parse routine for DF_VerticalOffset. */
int DF_VerticalOffset_parse(VerticalOffset_t *vecoffset_ptr, DF_VerticalOffset_st_ptr VecOffset_ptr)
{  
    int            result = 0;

    
    /* Error detect. */
    if((vecoffset_ptr == NULL) || (VecOffset_ptr == NULL))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(VecOffset_ptr, 0, sizeof(*VecOffset_ptr));

    /* VehicleOffset. */
    switch(vecoffset_ptr->present)
    {
        case VerticalOffset_PR_NOTHING:
        {
            VecOffset_ptr->choiceid = VerticalOffset_NOTHING;
            break;
        }
        case VerticalOffset_PR_offset1:
        {
            VecOffset_ptr->choiceid = VerticalOffset_offset1;
            VecOffset_ptr->choice.offset1 = bsctyp_decode(DE_VertOffset_B07, vecoffset_ptr->choice.offset1);
            break;
        }
        case VerticalOffset_PR_offset2:
        {
            VecOffset_ptr->choiceid = VerticalOffset_offset2;
            VecOffset_ptr->choice.offset2 = bsctyp_decode(DE_VertOffset_B08, vecoffset_ptr->choice.offset2);
            break;
        }
        case VerticalOffset_PR_offset3:
        {
            VecOffset_ptr->choiceid = VerticalOffset_offset3;
            VecOffset_ptr->choice.offset3 = bsctyp_decode(DE_VertOffset_B09, vecoffset_ptr->choice.offset3);
            break;
        }
        case VerticalOffset_PR_offset4:
        {
            VecOffset_ptr->choiceid = VerticalOffset_offset4;
            VecOffset_ptr->choice.offset4 = bsctyp_decode(DE_VertOffset_B10, vecoffset_ptr->choice.offset4);
            break;
        }
        case VerticalOffset_PR_offset5:
        {
            VecOffset_ptr->choiceid = VerticalOffset_offset5;
            VecOffset_ptr->choice.offset5 = bsctyp_decode(DE_VertOffset_B11, vecoffset_ptr->choice.offset5);
            break;
        }
        case VerticalOffset_PR_offset6:
        {
            VecOffset_ptr->choiceid = VerticalOffset_offset6;
            VecOffset_ptr->choice.offset6 = bsctyp_decode(DE_VertOffset_B12, vecoffset_ptr->choice.offset6);
            break;
        }
        case VerticalOffset_PR_elevation:
        {
            VecOffset_ptr->choiceid = VerticalOffset_elevation;
            VecOffset_ptr->choice.elevation = bsctyp_decode(DE_Elevation, vecoffset_ptr->choice.elevation);
            break;
        }
    }
    
    return result;

ERR_EXIT:

    return result;
}






