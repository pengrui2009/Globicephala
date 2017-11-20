/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_msg_frame.c
 @brief  : MSG_MessageFrame(MMF) routine set.
 @author : pengrui
 @history:
           2017-09-08    pengrui    Created file
           ...
******************************************************************************/
#include "cv_msg_frame.h"
#include "cv_data_element.h"
#include "cv_data_frame.h"
#include "error.h"
#include "MessageFrame.h"


/* Free routine for MSG_BasicSafetyMessage. */
static int MSG_BasicSafetyMessage_free(BasicSafetyMessage_t *bsm_ptr)
{
	int		result = 0;

    
    if(bsm_ptr != NULL)
    {
        /* Free id. */
		if(bsm_ptr->id.buf != NULL)
		{
			FREEMEM(bsm_ptr->id.buf);
		}

		if(bsm_ptr->plateNo != NULL)
		{
			if(bsm_ptr->plateNo->buf != NULL)
			{
				FREEMEM(bsm_ptr->plateNo->buf);
			}
			FREEMEM(bsm_ptr->plateNo);
		}

		DF_Position3D_free(&bsm_ptr->pos);

		DF_PositionConfidenceSet_free(&bsm_ptr->accuracy);

		if(bsm_ptr->angle != NULL)
		{
			FREEMEM(bsm_ptr->angle);
		}
		
		if(bsm_ptr->motionCfd != NULL)
		{
			DF_MotionConfidenceSet_free(bsm_ptr->motionCfd);
			FREEMEM(bsm_ptr->motionCfd);
		}

		DF_BrakeSystemStatus_free(&bsm_ptr->brakes);

		DF_VehicleSize_free(&bsm_ptr->size);

		if(bsm_ptr->safetyExt != NULL)
		{
			DF_VehicleSafetyExtensions_free(bsm_ptr->safetyExt);
			FREEMEM(bsm_ptr->safetyExt);
		}
		
        /* Must clear all the zone and avoid repeat free operation. */
        memset(bsm_ptr, 0, sizeof(*bsm_ptr));
        
        return result;
    }
    else
    {
    	result = -ERR_INVAL;
        return result;
    }
}


/* Allocate routine for MSG_BasicSafetyMessage. */
static int MSG_BasicSafetyMessage_allocate(BasicSafetyMessage_t *bsm_ptr, MSG_BasicSafetyMessage_st_ptr msg_bsm_ptr) 
{
	int		result = 0;


    /* Error detect. */
    if((bsm_ptr == NULL) || (msg_bsm_ptr == NULL))
    {
    	result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(bsm_ptr, 0, sizeof(*bsm_ptr));
    
    /* Message count. */
    bsm_ptr->msgCnt = bsctyp_encode(DE_MsgCount, msg_bsm_ptr->msgCnt);

    /* Id. */
	result = DE_TemporaryID_allocate(&(bsm_ptr->id), &(msg_bsm_ptr->id));
    if(result!= 0)
    {
        goto ERR_EXIT;
    }

	/* plateNo */
	if(msg_bsm_ptr->opt.msg_BasicSafetyMessage_opt_plateno == MSG_OPTIONAL_YES)
	{
		bsm_ptr->plateNo = CALLOC(1, sizeof(*(bsm_ptr->plateNo)));
		if(bsm_ptr->plateNo == NULL)
		{
			result = -ERR_NOMEM;
			goto ERR_EXIT;
		}
		
		result= DE_PlateNo_allocate(bsm_ptr->plateNo, &(msg_bsm_ptr->plateno));
		if(result != 0)
		{
			goto ERR_EXIT;
		}
		
	}else{
		bsm_ptr->plateNo = NULL;
	}
	
	
    /* Dsecond. */
    bsm_ptr->secMark = bsctyp_encode(DE_DSecond, msg_bsm_ptr->secMark);

    /* Postion. */
	result = DF_Position3D_allocate(&(bsm_ptr->pos), &(msg_bsm_ptr->pos));
	if(result != 0)
	{
		goto ERR_EXIT;
	}
    
    /* Position accuracy. */
	result = DF_PositionConfidenceSet_allocate(&(bsm_ptr->accuracy), &(msg_bsm_ptr->accuracy));
	if(result != 0)
	{
		goto ERR_EXIT;
	}
	
    /* Speed angle. */
    bsm_ptr->transmission = msg_bsm_ptr->trans;
    bsm_ptr->speed = bsctyp_encode(DE_Speed, msg_bsm_ptr->speed);
    bsm_ptr->heading = bsctyp_encode(DE_Heading, msg_bsm_ptr->heading);
	
	if(msg_bsm_ptr->opt.msg_BasicSafetyMessage_opt_angle == MSG_OPTIONAL_YES)
	{
		bsm_ptr->angle = CALLOC(1, sizeof(*bsm_ptr->angle));
		if(bsm_ptr->angle == NULL)
		{
			result = -ERR_NOMEM;
			goto ERR_EXIT;
		}
		*bsm_ptr->angle = bsctyp_encode(DE_SteeringWheelAngle, msg_bsm_ptr->angle);
	}else{
		bsm_ptr->angle = NULL;
	}
	
	/* motion cfd*/
	if(msg_bsm_ptr->opt.msg_BasicSafetyMessage_opt_motioncfd == MSG_OPTIONAL_YES)
	{
		bsm_ptr->motionCfd = CALLOC(1, sizeof(*(bsm_ptr->motionCfd)));
		if(bsm_ptr->motionCfd == NULL)
		{
			goto ERR_EXIT;
		}

		result = DF_MotionConfidenceSet_Allocate(bsm_ptr->motionCfd, &msg_bsm_ptr->motioncfd);
		if(result != 0)
		{
			goto ERR_EXIT;
		}
	}else{
		bsm_ptr->motionCfd = NULL;
	}
	
    /* Acceleration set. */
	
    bsm_ptr->accelSet.Long = bsctyp_encode(DE_Acceleration, msg_bsm_ptr->acceSet.longitudinal);;
    bsm_ptr->accelSet.lat = bsctyp_encode(DE_Acceleration, msg_bsm_ptr->acceSet.lateral);;
    bsm_ptr->accelSet.vert = bsctyp_encode(DE_VerticalAcceleration, msg_bsm_ptr->acceSet.vertical);;
    bsm_ptr->accelSet.yaw = bsctyp_encode(DE_YawRate, msg_bsm_ptr->acceSet.yaw_rate);;

    /* Brakes status. */
	result = DF_BrakeSystemStatus_allocate(&bsm_ptr->brakes, &msg_bsm_ptr->brakes);
	if(result != 0)
    {
        goto ERR_EXIT;
	}

    /* Vehicle size. */
	result = DF_VehicleSize_allocate(&bsm_ptr->size, &msg_bsm_ptr->size);
	if(result != 0)
    {
        goto ERR_EXIT;
	}
    
	/* vehicle class */
	bsm_ptr->vehicleClass.classification = msg_bsm_ptr->vehicleclass.classification;
	
	/* safetyExt */
	if(msg_bsm_ptr->opt.msg_BasicSafetyMessage_opt_safetyExt == MSG_OPTIONAL_YES)
	{
		bsm_ptr->safetyExt = CALLOC(1, sizeof(*(bsm_ptr->safetyExt)));
		if(bsm_ptr->safetyExt == NULL)
		{
			result = -ERR_NOMEM;
			goto ERR_EXIT;
		}

		result = DF_VehicleSafetyExtensions_allocate(bsm_ptr->safetyExt, &msg_bsm_ptr->safetyExt);
		if(result != 0)
		{
			goto ERR_EXIT;
		}
	}
	
    return result;    
    
ERR_EXIT:

    MSG_BasicSafetyMessage_free(bsm_ptr);
    return result;    
}


/* Parse bsm message into message control structure. */
static int MSG_BasicSafetyMessage_parse(BasicSafetyMessage_t *bsm_ptr, MSG_BasicSafetyMessage_st_ptr msg_bsm_ptr)
{
	int result = 0;


    /* Error detect. */
    if((bsm_ptr == NULL)||(msg_bsm_ptr == NULL))
    {
    	result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(msg_bsm_ptr, 0, sizeof(*msg_bsm_ptr));

	msg_bsm_ptr->msgCnt = bsctyp_decode(DE_MsgCount, bsm_ptr->msgCnt);

	/* id. */
	result = DE_TemporaryID_parse(&(bsm_ptr->id), &(msg_bsm_ptr->id));
	if(result != 0)
	{
		goto ERR_EXIT;
	}

	/* plateNo */
	if(bsm_ptr->plateNo != NULL)
	{
		result = DE_PlateNo_parse(bsm_ptr->plateNo, &(msg_bsm_ptr->plateno));
		if(result != 0)
		{
			goto ERR_EXIT;
		}

		msg_bsm_ptr->opt.msg_BasicSafetyMessage_opt_plateno = MSG_OPTIONAL_YES;
	}
	
	/* Dsecond. */	  
	msg_bsm_ptr->secMark = bsctyp_decode(DE_DSecond, bsm_ptr->secMark);

	/* Positon. */
	result = DF_Position3D_parse(&(bsm_ptr->pos), &(msg_bsm_ptr->pos));
	if(result != 0)
	{
		goto ERR_EXIT;
	}

	/* Position confidenceset. */
	result = DF_PositionConfidenceSet_parse(&(bsm_ptr->accuracy), &(msg_bsm_ptr->accuracy));
	if(result != 0)
	{
		goto ERR_EXIT;
	}

	/*transmission*/
	msg_bsm_ptr->trans = bsm_ptr->transmission;
	
	/* Speed . */
	msg_bsm_ptr->speed = bsctyp_decode(DE_Speed, bsm_ptr->speed);
	/* Heading. */
	msg_bsm_ptr->heading = bsctyp_decode(DE_Heading, bsm_ptr->heading);
	/* Angle. */
	if(bsm_ptr->angle != NULL)
	{
		msg_bsm_ptr->angle = bsctyp_decode(DE_SteeringWheelAngle, *bsm_ptr->angle);
		msg_bsm_ptr->opt.msg_BasicSafetyMessage_opt_angle = MSG_OPTIONAL_YES;
	}
	/* motion cfd */
	if(bsm_ptr->motionCfd != NULL)
	{
		result = DF_MotionConfidenceSet_parse(bsm_ptr->motionCfd, &(msg_bsm_ptr->motioncfd));
		if(result != 0)
		{
			goto ERR_EXIT;
		}
		msg_bsm_ptr->opt.msg_BasicSafetyMessage_opt_motioncfd = MSG_OPTIONAL_YES;
	}
	
	/* Acceleration set. */
	result = DF_AccelerationSet4Way_parse(&(bsm_ptr->accelSet), &(msg_bsm_ptr->acceSet));

	/* Brakes status. */
	result = DF_BrakeSystemStatus_parse(&(bsm_ptr->brakes), &(msg_bsm_ptr->brakes));
	if(result != 0)
	{
		goto ERR_EXIT;
	}

	/* vehicle size */
	result = DF_VehicleSize_parse(&bsm_ptr->size, &(msg_bsm_ptr->size));
	if(result != 0)
	{
		goto ERR_EXIT;
	}
	
	/* vehicle class */
	msg_bsm_ptr->vehicleclass.classification = bsm_ptr->vehicleClass.classification;

	if(bsm_ptr->safetyExt != NULL)
	{
		/*Vehicle Safety Extensions*/
	    result = DF_VehicleSafetyExtensions_parse(bsm_ptr->safetyExt, &(msg_bsm_ptr->safetyExt));
		if(result != 0)
		{
			goto ERR_EXIT;
		}
		msg_bsm_ptr->opt.msg_BasicSafetyMessage_opt_safetyExt = MSG_OPTIONAL_YES;
	}
    return result; 

ERR_EXIT:

    return result;
}


/* Free routine for MSG_MapData. */
static int MSG_MapData_free(MapData_t *mapData_ptr)
{
	int    result = 0;

    /* Error detection. */
    if(mapData_ptr != NULL)
    {
        /* TimeStamp. */
        if(mapData_ptr->timeStamp != NULL)
        {
            FREEMEM(mapData_ptr->timeStamp);
        }

        result = DF_NodeList_free(&(mapData_ptr->nodes));
		if(result)
			goto ERR_EXIT;

        /* Must clear all the zone and avoid repeat free operation. */
        memset(mapData_ptr, 0, sizeof(*mapData_ptr));

        result = ERR_OK;
    }
    else
    {
        result = -ERR_INVAL;
    }
ERR_EXIT:
	return result;
}


/* Allocate routine for MSG_MapData. */
static int MSG_MapData_allocate(MapData_t *mapData_ptr, MSG_MapData_st_ptr MapData_ptr) 
{
	int    result = 0;


    /* Error detect. */
    if((mapData_ptr == NULL) || (MapData_ptr == NULL))
    {
    	result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(mapData_ptr, 0, sizeof(*mapData_ptr));

    /* Message count. */
    mapData_ptr->msgCnt = bsctyp_encode(DE_MsgCount, MapData_ptr->msgCnt);

    /* TimeStamp. */
    if(MapData_ptr->opt.msg_MapData_opt_timestamp == MSG_OPTIONAL_YES)
    {
        if((mapData_ptr->timeStamp = CALLOC(1, sizeof(*(mapData_ptr->timeStamp)))) != NULL)
        {
            *(mapData_ptr->timeStamp) = MapData_ptr->timestamp;
        }
        else
        {
        	result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        mapData_ptr->timeStamp = NULL;
    }

    result = DF_NodeList_allocate(&(mapData_ptr->nodes), &(MapData_ptr->nodes));
	if(result)
		goto ERR_EXIT;

	return result;
  
ERR_EXIT:
 
    MSG_MapData_free(mapData_ptr);
    return result;   

}


/* Parse mapData message into message control structure. */
static int MSG_MapData_parse(MapData_t *mapData_ptr, MSG_MapData_st_ptr MapData_ptr)
{
	int 	result = 0;


    /* Error detect. */
    if((mapData_ptr == NULL) || (MapData_ptr == NULL))
    {
    	result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(MapData_ptr, 0, sizeof(*MapData_ptr));

    /* Message count. */
    MapData_ptr->msgCnt = bsctyp_decode(DE_MsgCount, mapData_ptr->msgCnt);

    /* TimeStamp. */
    if(mapData_ptr->timeStamp != NULL)
    {
        MapData_ptr->opt.msg_MapData_opt_timestamp = MSG_OPTIONAL_YES;
        MapData_ptr->timestamp = *(mapData_ptr->timeStamp);
    }
    else
    {
        MapData_ptr->opt.msg_MapData_opt_timestamp = MSG_OPTIONAL_NO;
    }

	result = DF_NodeList_parse(&(mapData_ptr->nodes), &(MapData_ptr->nodes));
	if(result)
		goto ERR_EXIT;

	result = 0;  

ERR_EXIT:

    return result;  
}


/* Free routine for MSG_RoadSideInformation. */
static int MSG_RoadSideInformation_free(RoadSideInformation_t *rsi_ptr)
{
	int    result = 0;

	
    /* Error detection. */
    if(rsi_ptr != NULL)
    {
        /* TimeStamp. */
        if(rsi_ptr->timeStamp != NULL)
        {
            FREEMEM(rsi_ptr->timeStamp);
        }

        /* Id. */
        DE_TemporaryID_free(&(rsi_ptr->id));
        
        /* Description. */
        if(rsi_ptr->description != NULL)
        {
            if(rsi_ptr->description->buf != NULL)
            {
                FREEMEM(rsi_ptr->description->buf);
            }

            FREEMEM(rsi_ptr->description);
        }

        /* Priority. */
        if(rsi_ptr->priority != NULL)
        {
            if(rsi_ptr->priority->buf != NULL)
            {
                FREEMEM(rsi_ptr->priority->buf);
            }

            FREEMEM(rsi_ptr->priority);
        }

        /* Postion3D. */
        DF_Position3D_free(&(rsi_ptr->refPos));

        /* AlertPath. */
        DF_PathPointList_free(&(rsi_ptr->alertPath));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(rsi_ptr, 0, sizeof(*rsi_ptr));

        result = ERR_OK;
    }
    else
    {
        result = -ERR_INVAL;
    }

	return result;
}


/* Allocate routine for MSG_RoadSideInformation. */
static int MSG_RoadSideInformation_allocate(RoadSideInformation_t *rsi_ptr, MSG_RoadSideInformation_st_ptr msg_rsi_ptr) 
{
	int    result = 0;


    /* Error detect. */
    if((rsi_ptr == NULL) || (msg_rsi_ptr == NULL))
    {
    	result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(rsi_ptr, 0, sizeof(*rsi_ptr));

    /* Message count. */
    rsi_ptr->msgCnt = bsctyp_encode(DE_MsgCount, msg_rsi_ptr->msgCnt);

    /* TimeStamp. */
    if(msg_rsi_ptr->opt.msg_RoadSideInformation_opt_timestamp == MSG_OPTIONAL_YES)
    {
        if((rsi_ptr->timeStamp = CALLOC(1, sizeof(*rsi_ptr->timeStamp))) != NULL)
        {
            *(rsi_ptr->timeStamp) = msg_rsi_ptr->timestamp;
        }
        else
        {
        	result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        rsi_ptr->timeStamp = NULL;
    }

    /* Id. */
    if((result = DE_TemporaryID_allocate(&(rsi_ptr->id), &(msg_rsi_ptr->id))) != 0)
    {
        goto ERR_EXIT;
    }
    
    /* RsiId. */
    rsi_ptr->rsiId = msg_rsi_ptr->rsiId;

    /* AlertType. */
    rsi_ptr->alertType = msg_rsi_ptr->alertType;

    /* Description. */
    if(msg_rsi_ptr->opt.msg_RoadSideInformation_opt_description == MSG_OPTIONAL_YES)
    {
        if((rsi_ptr->description = CALLOC(1, sizeof(*rsi_ptr->description))) != NULL)
        {
            rsi_ptr->description->size = msg_rsi_ptr->description.Description_size;
            if((rsi_ptr->description->buf = CALLOC(1, rsi_ptr->description->size)) != NULL)
            {
                memcpy(rsi_ptr->description->buf, msg_rsi_ptr->description.Description_data, rsi_ptr->description->size);
            }
            else
            {
                result = -ERR_NOMEM;
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
        rsi_ptr->description = NULL;
    }

    /* Priority. */
    if(msg_rsi_ptr->opt.msg_RoadSideInformation_opt_priority == MSG_OPTIONAL_YES)
    {
        if((rsi_ptr->priority = CALLOC(1, sizeof(*rsi_ptr->priority))) != NULL)
        {
            rsi_ptr->priority->size = DE_Priority_BUFSIZE;
            if((rsi_ptr->priority->buf = CALLOC(1, rsi_ptr->priority->size)) != NULL)
            {
                memcpy(rsi_ptr->priority->buf, &(msg_rsi_ptr->priority[0]), rsi_ptr->priority->size);
            }
            else
            {
                result = -ERR_NOMEM;
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
        rsi_ptr->priority = NULL;
    }

    /* Postion3D. */
	if((result = DF_Position3D_allocate(&(rsi_ptr->refPos), &(msg_rsi_ptr->refPos))) != 0)
	{
		goto ERR_EXIT;
	}

    /* AlertPath. */
	if((result = DF_PathPointList_allocate(&(rsi_ptr->alertPath), &(msg_rsi_ptr->alertPath))) != 0)
	{
		goto ERR_EXIT;
	}

    /* AlertRadius. */
    rsi_ptr->alertRadius = bsctyp_encode(DE_Radius, msg_rsi_ptr->alertRadius);

	return result;
  
ERR_EXIT:
 
    MSG_RoadSideInformation_free(rsi_ptr);
    return result;   

}


/* Parse rsi message into message control structure. */
static int MSG_RoadSideInformation_parse(RoadSideInformation_t *rsi_ptr, MSG_RoadSideInformation_st_ptr msg_rsi_ptr)
{
	int 	result = 0;


    /* Error detect. */
    if((rsi_ptr == NULL) || (msg_rsi_ptr == NULL))
    {
    	result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(msg_rsi_ptr, 0, sizeof(*msg_rsi_ptr));

    /* Message count. */
    msg_rsi_ptr->msgCnt = bsctyp_decode(DE_MsgCount, rsi_ptr->msgCnt);

    /* TimeStamp. */
    if(rsi_ptr->timeStamp != NULL)
    {
        msg_rsi_ptr->opt.msg_RoadSideInformation_opt_timestamp = MSG_OPTIONAL_YES;
        msg_rsi_ptr->timestamp = *(rsi_ptr->timeStamp);
    }
    else
    {
        msg_rsi_ptr->opt.msg_RoadSideInformation_opt_timestamp = MSG_OPTIONAL_NO;
    }

    /* Id. */
    if((result = DE_TemporaryID_parse(&(rsi_ptr->id), &(msg_rsi_ptr->id))) != 0)
    {
        goto ERR_EXIT;
    }
    
    /* RsiId. */
    msg_rsi_ptr->rsiId = rsi_ptr->rsiId;

    /* AlertType. */
    msg_rsi_ptr->alertType = rsi_ptr->alertType;

    /* Description. */
    if(rsi_ptr->description != NULL)
    {
        msg_rsi_ptr->opt.msg_RoadSideInformation_opt_description = MSG_OPTIONAL_YES;
        
        msg_rsi_ptr->description.Description_size = rsi_ptr->description->size;
        memcpy(msg_rsi_ptr->description.Description_data, rsi_ptr->description->buf, rsi_ptr->description->size);
    }
    else
    {
        msg_rsi_ptr->opt.msg_RoadSideInformation_opt_description = MSG_OPTIONAL_NO;
    }

    /* Priority. */
    if(rsi_ptr->priority != NULL)
    {
        msg_rsi_ptr->opt.msg_RoadSideInformation_opt_priority = MSG_OPTIONAL_YES;
        memcpy(&(msg_rsi_ptr->priority[0]), rsi_ptr->priority->buf, rsi_ptr->priority->size);
    }
    else
    {
        msg_rsi_ptr->opt.msg_RoadSideInformation_opt_priority = MSG_OPTIONAL_NO;
    }

    /* Postion3D. */
	if((result = DF_Position3D_parse(&(rsi_ptr->refPos), &(msg_rsi_ptr->refPos))) != 0)
	{
		goto ERR_EXIT;
	}

    /* AlertPath. */
	if((result = DF_PathPointList_parse(&(rsi_ptr->alertPath), &(msg_rsi_ptr->alertPath))) != 0)
	{
		goto ERR_EXIT;
	}

    /* AlertRadius. */
    msg_rsi_ptr->alertRadius = bsctyp_decode(DE_Radius, rsi_ptr->alertRadius);

	result = 0;  

ERR_EXIT:

    return result;  
}


/* Free routine for MSG_RoadsideSafetyMessage. */
static int MSG_RoadsideSafetyMessage_free(RoadsideSafetyMessage_t *rsm_ptr)
{
	int    result = 0;

	
    /* Error detection. */
    if(rsm_ptr != NULL)
    {
        /* Id. */
        DE_TemporaryID_free(&(rsm_ptr->id));

        /* Postion3D. */
    	DF_Position3D_free(&(rsm_ptr->refPos));
        
        /* ParticipantList. */
        DF_ParticipantList_free(&(rsm_ptr->participants));

        /* Must clear all the zone and avoid repeat free operation. */
        memset(rsm_ptr, 0, sizeof(*rsm_ptr));

        result = ERR_OK;
    }
    else
    {
        result = -ERR_INVAL;
    }

	return result;
}


/* Allocate routine for MSG_RoadSideSafetyMessage. */
static int MSG_RoadSideSafetyMessage_allocate(RoadsideSafetyMessage_t *rsm_ptr, MSG_RoadSideSafetyMessage_st_ptr msg_rsm_ptr) 
{
	int    result = 0;


    /* Error detect. */
    if((rsm_ptr == NULL) || (msg_rsm_ptr == NULL))
    {
    	result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(rsm_ptr, 0, sizeof(*rsm_ptr));

    /* Message count. */
    rsm_ptr->msgCnt = bsctyp_encode(DE_MsgCount, msg_rsm_ptr->msgCnt);

    /* Id. */
    if((result = DE_TemporaryID_allocate(&(rsm_ptr->id), &(msg_rsm_ptr->id))) != 0)
    {
        goto ERR_EXIT;
    }

    /* Postion3D. */
	if((result = DF_Position3D_allocate(&(rsm_ptr->refPos), &(msg_rsm_ptr->refPos))) != 0)
	{
		goto ERR_EXIT;
	}
    
    /* ParticipantList. */
    if((result = DF_ParticipantList_allocate(&(rsm_ptr->participants), &(msg_rsm_ptr->participants))) != 0)
	{
		goto ERR_EXIT;
	}
    
	return result;
  
ERR_EXIT:
 
    MSG_RoadsideSafetyMessage_free(rsm_ptr);
    return result;   

}


/* Parse rsm message into message control structure. */
static int MSG_RoadSideSafetyMessage_parse(RoadsideSafetyMessage_t *rsm_ptr, MSG_RoadSideSafetyMessage_st_ptr msg_rsm_ptr)
{
	int 	result = 0;


    /* Error detect. */
    if((rsm_ptr == NULL) || (msg_rsm_ptr == NULL))
    {
    	result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(msg_rsm_ptr, 0, sizeof(*msg_rsm_ptr));
  
    /* Message count. */
    msg_rsm_ptr->msgCnt = bsctyp_decode(DE_MsgCount, rsm_ptr->msgCnt);

    /* Id. */
    if((result = DE_TemporaryID_parse(&(rsm_ptr->id), &(msg_rsm_ptr->id))) != 0)
    {
        goto ERR_EXIT;
    }

    /* Postion3D. */
	if((result = DF_Position3D_parse(&(rsm_ptr->refPos), &(msg_rsm_ptr->refPos))) != 0)
	{
		goto ERR_EXIT;
	}
    
    /* ParticipantList. */
    if((result = DF_ParticipantList_parse(&(rsm_ptr->participants), &(msg_rsm_ptr->participants))) != 0)
	{
		goto ERR_EXIT;
	}

    result = 0;

ERR_EXIT:

    return result;  

}


/* Free routine for MSG_SignalPhaseAndTiming. */
static int MSG_SignalPhaseAndTiming_free(SPAT_t *spat_ptr)
{
	int    result = 0;


    /* Error detection. */
	if(spat_ptr == NULL)
	{
		result = -ERR_INVAL;
		goto ERR_EXIT;
	}

    /* Minute of the year. */
	if(spat_ptr->timeStamp != NULL)
	{
	    FREEMEM(spat_ptr->timeStamp);
	}
		
    /* Descriptive name. */
	if(spat_ptr->name != NULL)
	{
                if((result = DE_DescriptiveName_free(spat_ptr->name)) != ERR_OK)
		{
                    goto ERR_EXIT;
		}
		FREEMEM(spat_ptr->name);
	}

    /* Intersection state list. */
	if((result = DF_IntersectionStateList_free(&(spat_ptr->intersections))) != ERR_OK)
	{
	    goto ERR_EXIT;
	}
		
    /* Must clear all the zone and avoid repeat free operation. */
	memset(spat_ptr, 0x00, sizeof(*spat_ptr));

ERR_EXIT:

	return result;
}


/* Allocate routine for MSG_SignalPhaseAndTiming. */
static int MSG_SignalPhaseAndTiming_allocate(SPAT_t *spat_ptr, MSG_SPAT_st_ptr msg_spat_ptr)
{
	int    result = 0;


    /* Error detect. */
	if((spat_ptr == NULL) || (msg_spat_ptr == NULL))
	{
		result = -ERR_INVAL;
		goto ERR_EXIT;
	}

    /* Reset all the zone. */
    memset(spat_ptr, 0, sizeof(*spat_ptr));

    /* Message count. */
    spat_ptr->msgCnt = bsctyp_encode(DE_MsgCount, msg_spat_ptr->msgCnt);

    /* TimeStamp. */
    if(msg_spat_ptr->opt.timestamp == MSG_OPTIONAL_YES)
    {
        if((spat_ptr->timeStamp = CALLOC(1, sizeof(*spat_ptr->timeStamp))) != NULL)
        {
            *(spat_ptr->timeStamp) = bsctyp_encode(DE_MinuteOfTheYear, msg_spat_ptr->timestamp);  
        }
        else
        {
        	result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
    }
    else
    {
        spat_ptr->timeStamp = NULL;
    }

    /* Descriptive name. */
    if(msg_spat_ptr->opt.name == MSG_OPTIONAL_YES)
    {
        if((spat_ptr->name = CALLOC(1, sizeof(*spat_ptr->name))) != NULL)
        {
			if((DE_DescriptiveName_allocate(spat_ptr->name, &msg_spat_ptr->name)) != 0)
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
        spat_ptr->name = NULL;
    }

    /* IntersectionStateList. */
    if((result = DF_IntersectionStateList_allocate(&(spat_ptr->intersections), &(msg_spat_ptr->intersections))) != 0)
	{
		goto ERR_EXIT;
	}
    
	return result;
  
ERR_EXIT:
 
    MSG_SignalPhaseAndTiming_free(spat_ptr);
    return result;  
}


/* Parse routine for MSG_SignalPhaseAndTiming. */
int MSG_SignalPhaseAndTiming_parse(SPAT_t *spat_ptr, MSG_SPAT_st_ptr msg_spat_ptr)
{
	int 	result = 0;


    /* Error detect. */
    if((spat_ptr == NULL) || (msg_spat_ptr == NULL))
    {
    	result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Reset all the zone. */
    memset(msg_spat_ptr, 0, sizeof(*msg_spat_ptr));
  
    /* Message count. */
    msg_spat_ptr->msgCnt = bsctyp_decode(DE_MsgCount, spat_ptr->msgCnt);

	/* timestamp. */
	if(spat_ptr->timeStamp != NULL)
	{
		msg_spat_ptr->timestamp = bsctyp_decode(DE_MinuteOfTheYear, *(spat_ptr->timeStamp));
		msg_spat_ptr->opt.timestamp = MSG_OPTIONAL_YES;
	}

	/* Descriptive name. */
	if(spat_ptr->name != NULL)
	{
		if((result = DE_DescriptiveName_parse(spat_ptr->name, &(msg_spat_ptr->name))) == ERR_OK)
		{
		    msg_spat_ptr->opt.name = MSG_OPTIONAL_YES;
		}
        else
        {
            goto ERR_EXIT;
        }	
	}

	/* Intersection state list. */
	if((result = DF_IntersectionStateList_parse(&(spat_ptr->intersections), &(msg_spat_ptr->intersections))) != ERR_OK)
	{
	    goto ERR_EXIT;
	}
	
    return result;

ERR_EXIT:

	return result;
}


/* free frame msg. */
static int frame_free_msg(MessageFrame_t *frame_ptr)
{
	int result = 0;

	if(frame_ptr == NULL)
	{
		result = -ERR_INVAL;
		goto ERR_EXIT;
	}

	switch(frame_ptr->present)
	{
	case MessageFrame_PR_NOTHING:
		break;
	case MessageFrame_PR_bsmFrame:
		result = MSG_BasicSafetyMessage_free(&frame_ptr->choice.bsmFrame);
		break;
	case MessageFrame_PR_mapFrame:
		result = MSG_MapData_free(&frame_ptr->choice.mapFrame);
		break;
	case MessageFrame_PR_rsiFrame:
		result = MSG_RoadSideInformation_free(&frame_ptr->choice.rsiFrame);
		break;
	case MessageFrame_PR_rsmFrame:
		result = MSG_RoadsideSafetyMessage_free(&frame_ptr->choice.rsmFrame);
		break;
	case MessageFrame_PR_spatFrame:
		result = MSG_SignalPhaseAndTiming_free(&frame_ptr->choice.spatFrame);
		break;
	}

ERR_EXIT:

	return result;
}

/* decode frame msg. return 0: success, others:error. */
int frame_decode_msg(MSG_MessageFrame_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size)
{
	int                            result = 0;
    MessageFrame_t          *frame_ptr = NULL;
    
    asn_dec_rval_t        decode_rval = { 0 };
    asn_codec_ctx_t        decode_ctx = { 0 };


    /* Error detection. */
    if((msg_ptr == NULL) || (buffer_ptr == NULL) || (buffer_size == 0))
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Dncode message from UPER format. */
    decode_rval = uper_decode(&decode_ctx, &asn_DEF_MessageFrame, (void **)&frame_ptr, buffer_ptr, buffer_size, 0, 0);
    if(decode_rval.code != RC_OK)
    {
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

	switch(frame_ptr->present)
	{
	case MessageFrame_PR_bsmFrame:
		{
			msg_ptr->messageid = MSG_MessageFrame_ID_BSM;
			result = MSG_BasicSafetyMessage_parse(&(frame_ptr->choice.bsmFrame), &(msg_ptr->msg.msg_bsm));
			break;
		}
	case MessageFrame_PR_mapFrame:
		{
			msg_ptr->messageid = MSG_MessageFrame_ID_MAPDATA;
			result = MSG_MapData_parse(&(frame_ptr->choice.mapFrame), &(msg_ptr->msg.msg_mapdata));
			break;
		}
	case MessageFrame_PR_rsmFrame:
		{
			msg_ptr->messageid = MSG_MessageFrame_ID_RSM;
			result = MSG_RoadSideSafetyMessage_parse(&(frame_ptr->choice.rsmFrame), &(msg_ptr->msg.msg_rsm));
			break;
		}
	case MessageFrame_PR_spatFrame:
		{
			msg_ptr->messageid = MSG_MessageFrame_ID_SPAT;
			result = MSG_SignalPhaseAndTiming_parse(&(frame_ptr->choice.spatFrame), &(msg_ptr->msg.msg_spat)); 
			break;
		}
	case MessageFrame_PR_rsiFrame:
		{
			msg_ptr->messageid = MSG_MessageFrame_ID_RSI;
			result = MSG_RoadSideInformation_parse(&(frame_ptr->choice.rsiFrame), &(msg_ptr->msg.msg_rsi));
		}
	default:
		{

			result = -ERR_INVAL;
			break;
		}
	}

ERR_EXIT:

    /* Free the message structure allocated by routine uper_decode(); */
    if(frame_ptr != NULL)
    {
        asn_DEF_MessageFrame.op->free_struct(&asn_DEF_MessageFrame, frame_ptr, 0);
    }
    
    return result; 
}

/*encode frame msg, return >0: encode bytes, <=0:error. */
int frame_encode_msg(MSG_MessageFrame_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size)
{
	int result = 0;
	MessageFrame_t           msg_frame = { 0 };
    asn_enc_rval_t         encode_rval = { 0 };

	if((msg_ptr == NULL) || (buffer_ptr == NULL) )
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    switch(msg_ptr->messageid)
    {
	    case MSG_MessageFrame_ID_BSM:
		{
			msg_frame.present = MessageFrame_PR_bsmFrame;
	        result = MSG_BasicSafetyMessage_allocate(&msg_frame.choice.bsmFrame, &msg_ptr->msg.msg_bsm);
	        break;
	    }
	    case MSG_MessageFrame_ID_MAPDATA:
		{
			msg_frame.present = MessageFrame_PR_mapFrame;
	        result = MSG_MapData_allocate(&msg_frame.choice.mapFrame, &msg_ptr->msg.msg_mapdata);
	        break;
		}
	    case MSG_MessageFrame_ID_RSI:
		{
			msg_frame.present = MessageFrame_PR_rsiFrame;
	        result = MSG_RoadSideInformation_allocate(&msg_frame.choice.rsiFrame, &msg_ptr->msg.msg_rsi);
	        break;
		}
	    case MSG_MessageFrame_ID_RSM:
		{
			msg_frame.present = MessageFrame_PR_rsmFrame;
	        result = MSG_RoadSideSafetyMessage_allocate(&msg_frame.choice.rsmFrame, &msg_ptr->msg.msg_rsm);
			break;
		}
	    case MSG_MessageFrame_ID_SPAT:
		{
			msg_frame.present = MessageFrame_PR_spatFrame;
	        result = MSG_SignalPhaseAndTiming_allocate(&msg_frame.choice.spatFrame, &msg_ptr->msg.msg_spat);
	        break;
		}
    }
	
    /* Encode message to UPER format. */
    encode_rval = uper_encode_to_buffer(&asn_DEF_MessageFrame, 0, &msg_frame, buffer_ptr, (size_t)buffer_size);
    if(encode_rval.encoded == -1)
    {
        result = -ERR_OTHER;
    }
    else
    {
        result = (encode_rval.encoded + 7)/8;
    }
    
	
ERR_EXIT:
	
    frame_free_msg(&msg_frame);

    return result;
}


