/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_msg_bsm.c
 @brief  : MSG_BasicSafetyMessage(BSM) routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/
#include "cv_msg_bsm.h"
#include "DSRCmsgIDEnum.h"
#include "BasicSafetyMessage.h"
#include "J2735.h"


/* Key for bsm tx message structure and UPER data printf. */
static uint8_t KeyPrintfBsmTxStruct = 0;
static uint8_t KeyPrintfBsmTxData = 0;

/* Key for bsm rx message structure and UPER data printf. */
static uint8_t KeyPrintfBsmRxStruct = 1;
static uint8_t KeyPrintfBsmRxData = 1;



/* Printf bsm message structure data. */
static void bsm_printf_msgstruct(const char *str, BasicSafetyMessage_t *bsm_ptr)
{
    BSMcoreData_t *coredata_ptr = &(bsm_ptr->bsmBody.coreData);


    /* User specific strings. */
    printf(str);
    
    /* Message tag. */
    printf("\nBSM message structure------------------->\n");
        
    /* Msg id. */
    printf("bsm_ptr->dsrcMsgId = 0x%lx. \n", bsm_ptr->dsrcMsgId);
    
    /* Message count. */
    printf("coredata_ptr->msgCnt = 0x%lx. \n", coredata_ptr->msgCnt);

    /* Id. */
    printf("coredata_ptr->id.buf = 0x%x,0x%x,0x%x,0x%x. \n", coredata_ptr->id.buf[0],coredata_ptr->id.buf[1],coredata_ptr->id.buf[2],coredata_ptr->id.buf[3]);

    /* Dsecond. */
    printf("coredata_ptr->secMark = 0x%lx. \n", coredata_ptr->secMark);

    /* Postion. */
    printf("coredata_ptr->lat,Long,elev = 0x%lx,0x%lx,0x%lx. \n", coredata_ptr->lat,coredata_ptr->Long,coredata_ptr->elev);

    /* Position accuracy. */
    printf("coredata_ptr->accuracy.semiMajor,semiMinor,orientation = 0x%lx,0x%lx,0x%lx. \n", coredata_ptr->accuracy.semiMajor,coredata_ptr->accuracy.semiMinor,coredata_ptr->accuracy.orientation);

    /* Speed angle. */
    printf("coredata_ptr->transmission,speed,heading,angle = 0x%lx,0x%lx,0x%lx,0x%lx. \n", coredata_ptr->transmission,coredata_ptr->speed,coredata_ptr->heading,coredata_ptr->angle);

    /* Acceleration set. */
    printf("coredata_ptr->accelSet.Long,lat,vert,yaw = 0x%lx,0x%lx,0x%lx,0x%lx. \n", coredata_ptr->accelSet.Long,coredata_ptr->accelSet.lat,coredata_ptr->accelSet.vert,coredata_ptr->accelSet.yaw);

    /* Brakes status. */
    printf("coredata_ptr->brakes.wheelBrakes.buf = 0x%x. \n", coredata_ptr->brakes.wheelBrakes.buf[0]);
    printf("coredata_ptr->brakes.traction = 0x%lx. \n", coredata_ptr->brakes.traction);
    printf("coredata_ptr->brakes.abs = 0x%lx. \n", coredata_ptr->brakes.abs);
    printf("coredata_ptr->brakes.scs = 0x%lx. \n", coredata_ptr->brakes.scs);
    printf("coredata_ptr->brakes.brakeBoost = 0x%lx. \n", coredata_ptr->brakes.brakeBoost);
    printf("coredata_ptr->brakes.auxBrakes = 0x%lx. \n", coredata_ptr->brakes.auxBrakes);

    /* Vehicle size. */
    printf("coredata_ptr->size.width,length = 0x%lx,0x%lx. \n", coredata_ptr->size.width,coredata_ptr->size.length);
}


/* Printf bsm message PER data. */
static void bsm_printf_msgdata(const char *str, uint8_t *buffer_ptr, uint16_t valid_bit)
{
    uint16_t index = 0;


    /* User specific strings. */
    printf(str);
    
    printf("\nBSM message PER data %d bit------------->\n", valid_bit);
    
    for(index = 0; index < (valid_bit+7)/8; index++)
    {
        printf("0x%02x ", buffer_ptr[index]);
        if((index+1) % 10 == 0)
        {
            printf("\n");
        }
    }
}


/* Free bsm parti BSMcoreData from message. */
static int bsm_free_parti_BSMcoreData(BSMcoreData_t *coredata_ptr)
{
    if(coredata_ptr != NULL)
    {
        /* Id domain. */
        if(coredata_ptr->id.buf != NULL)
        {
            free(coredata_ptr->id.buf);
        }

        /* Brakes domain. */
        if(coredata_ptr->brakes.wheelBrakes.buf != NULL)
        {
            free(coredata_ptr->brakes.wheelBrakes.buf);
        }

        return 0;
    }
    else
    {
        return -1;
    }    
}


/* Add bsm parti BSMcoreData into message. */
static int bsm_allocate_parti_BSMcoreData(BSMcoreData_t *coredata_ptr, DF_BSMcoreData_st_ptr coreData_ptr)
{
    /* Message count. */
    if(127 < coreData_ptr->msgCnt)
    {
        goto ERR_EXIT;
    }
    coredata_ptr->msgCnt = coreData_ptr->msgCnt;

    /* Id. */
    coredata_ptr->id.size = sizeof(coreData_ptr->id);
    coredata_ptr->id.buf = calloc(1, coredata_ptr->id.size);
    if(coredata_ptr->id.buf == NULL)
    {
        goto ERR_EXIT;                  
    }
    memcpy(coredata_ptr->id.buf, coreData_ptr->id, sizeof(coreData_ptr->id));

    /* Dsecond. */
    coredata_ptr->secMark = coreData_ptr->secMark;

    /* Postion. */
    coredata_ptr->lat = encode_latitude(coreData_ptr->latitude);
    coredata_ptr->Long = encode_longitude(coreData_ptr->longitude);
    coredata_ptr->elev = encode_elevation(coreData_ptr->elevation);

    /* Position accuracy. */
    coredata_ptr->accuracy.semiMajor = encode_semimajor_axis_accuracy(coreData_ptr->accuracy.semi_major_accu);
    coredata_ptr->accuracy.semiMinor = encode_semiminor_axis_accuracy(coreData_ptr->accuracy.semi_minor_accu);
    coredata_ptr->accuracy.orientation = encode_semimajor_axis_orientation(coreData_ptr->accuracy.semi_major_orientation);

    /* Speed angle. */
    coredata_ptr->transmission = coreData_ptr->trans;
    coredata_ptr->speed = encode_absolute_velocity(coreData_ptr->speed);
    coredata_ptr->heading = encode_angle(coreData_ptr->heading);
    coredata_ptr->angle = encode_steer_wheel_angle(coreData_ptr->angle);

    /* Acceleration set. */
    coredata_ptr->accelSet.Long = encode_acceleration(coreData_ptr->acceSet.longitudinal);;
    coredata_ptr->accelSet.lat = encode_acceleration(coreData_ptr->acceSet.lateral);;
    coredata_ptr->accelSet.vert = encode_vertical_acceleration(coreData_ptr->acceSet.vertical);;
    coredata_ptr->accelSet.yaw = encode_yawrate(coreData_ptr->acceSet.yaw_rate);;

    /* Brakes status. */
    coredata_ptr->brakes.wheelBrakes.size = 1;
    coredata_ptr->brakes.wheelBrakes.bits_unused = 3;
    coredata_ptr->brakes.wheelBrakes.buf = calloc(1, coredata_ptr->brakes.wheelBrakes.size);
    if(coredata_ptr->brakes.wheelBrakes.buf == NULL)
    {
        goto ERR_EXIT;
    }
    (*coredata_ptr->brakes.wheelBrakes.buf) = (coreData_ptr->brakes.wheelBrakes.brake_status_unavailable << 7) 
                                                 | (coreData_ptr->brakes.wheelBrakes.brake_status_leftFront << 6) 
                                                 | (coreData_ptr->brakes.wheelBrakes.brake_status_leftRear << 5)
                                                 | (coreData_ptr->brakes.wheelBrakes.brake_status_rightFront << 4) 
                                                 | (coreData_ptr->brakes.wheelBrakes.brake_status_rightRear << 3);
    
    coredata_ptr->brakes.traction = coreData_ptr->brakes.traction;
    coredata_ptr->brakes.abs = coreData_ptr->brakes.abs;
    coredata_ptr->brakes.scs = coreData_ptr->brakes.scs;
    coredata_ptr->brakes.brakeBoost = coreData_ptr->brakes.brakeBoost;
    coredata_ptr->brakes.auxBrakes = coreData_ptr->brakes.auxBrakes;

    /* Vehicle size. */
    coredata_ptr->size.width = encode_vehicle_width(coreData_ptr->size.width);
    coredata_ptr->size.length = encode_vehicle_length(coreData_ptr->size.length);

    return 0;
    

ERR_EXIT:

    bsm_free_parti_BSMcoreData(coredata_ptr);

    return -1;
}


/* Parse bsm parti BSMcoreData from message. */
static int bsm_parse_parti_BSMcoreData(BSMcoreData_t *coredata_ptr, DF_BSMcoreData_st_ptr coreData_ptr)
{
    /* MsgCount. */
    coreData_ptr->msgCnt = coredata_ptr->msgCnt;

    /* id. */
    memcpy(coreData_ptr->id, coredata_ptr->id.buf, sizeof(coreData_ptr->id));
    
    /* Dsecond. */    
    coreData_ptr->secMark = coredata_ptr->secMark;

    /* Positon. */
    coreData_ptr->latitude = decode_latitude(coredata_ptr->lat);
    coreData_ptr->longitude = decode_longitude(coredata_ptr->Long);
    coreData_ptr->elevation = decode_elevation(coredata_ptr->elev);

    /* Position accuracy. */
    coreData_ptr->accuracy.semi_major_accu = decode_semimajor_axis_accuracy(coredata_ptr->accuracy.semiMajor);
    coreData_ptr->accuracy.semi_minor_accu = decode_semiminor_axis_accuracy(coredata_ptr->accuracy.semiMinor);
    coreData_ptr->accuracy.semi_major_orientation = decode_semimajor_axis_orientation(coredata_ptr->accuracy.orientation);

    /* Speed angle. */
    coreData_ptr->trans = coredata_ptr->transmission;
    coreData_ptr->speed = decode_absolute_velocity(coredata_ptr->speed);
    coreData_ptr->heading = decode_angle(coredata_ptr->heading);
    coreData_ptr->angle = decode_steer_wheel_angle(coredata_ptr->angle);

    /* Acceleration set. */
    coreData_ptr->acceSet.longitudinal = decode_acceleration(coredata_ptr->accelSet.Long);
    coreData_ptr->acceSet.lateral = decode_acceleration(coredata_ptr->accelSet.lat);
    coreData_ptr->acceSet.vertical = decode_vertical_acceleration(coredata_ptr->accelSet.vert);
    coreData_ptr->acceSet.yaw_rate = decode_yawrate(coredata_ptr->accelSet.yaw);

    /* Brakes status. */
    coreData_ptr->brakes.wheelBrakes.brake_status_unavailable = (*coredata_ptr->brakes.wheelBrakes.buf) & 0x80; 
    coreData_ptr->brakes.wheelBrakes.brake_status_leftFront =  (*coredata_ptr->brakes.wheelBrakes.buf) & 0x40; 
    coreData_ptr->brakes.wheelBrakes.brake_status_leftRear = (*coredata_ptr->brakes.wheelBrakes.buf) & 0x20; 
    coreData_ptr->brakes.wheelBrakes.brake_status_rightFront = (*coredata_ptr->brakes.wheelBrakes.buf) & 0x10; 
    coreData_ptr->brakes.wheelBrakes.brake_status_rightRear = (*coredata_ptr->brakes.wheelBrakes.buf) & 0x08; 
    
    coreData_ptr->brakes.traction = coredata_ptr->brakes.traction;
    coreData_ptr->brakes.abs = coredata_ptr->brakes.abs;
    coreData_ptr->brakes.scs = coredata_ptr->brakes.scs;
    coreData_ptr->brakes.brakeBoost = coredata_ptr->brakes.brakeBoost;
    coreData_ptr->brakes.auxBrakes = coredata_ptr->brakes.auxBrakes;

    /* Vehicle size. */
    coreData_ptr->size.length = decode_vehicle_length(coredata_ptr->size.length);
    coreData_ptr->size.width = decode_vehicle_width(coredata_ptr->size.width);
    
    return 0;
}


/* Free bsm partii element vehicleSafetyExt. */
static int bsm_free_partii_vehicleSafetyExt(BSMpartIIExtension_t *element_ptr)
{
    if(element_ptr != NULL)
    {
        /* Lig`hts domain. */
        if(element_ptr->choice.vehicleSafetyExt.lights != NULL)
        {
            free(element_ptr->choice.vehicleSafetyExt.lights);
        }

        /* Path prediction domain. */
        if(element_ptr->choice.vehicleSafetyExt.pathPrediction != NULL)
        {
            free(element_ptr->choice.vehicleSafetyExt.pathPrediction);
        }

        /* Path history domain. */
        if(element_ptr->choice.vehicleSafetyExt.pathHistory != NULL)
        {
            free(element_ptr->choice.vehicleSafetyExt.pathHistory);
        }

        /* Events domain. */
        if(element_ptr->choice.vehicleSafetyExt.events != NULL)
        {
            if(element_ptr->choice.vehicleSafetyExt.events->buf != NULL)
            {
                free(element_ptr->choice.vehicleSafetyExt.events->buf);
            }
            
            free(element_ptr->choice.vehicleSafetyExt.events);
        }
        
        /* Element itself domain. */
        free(element_ptr);

        return 0;
    }  
    else
    {
        return -1;
    }
}


/* Allocate bsm partii element vehicleSafetyExt. */
static int bsm_allocate_partii_vehicleSafetyExt(BSMpartIIExtension_t **ext_ptr_ptr, DF_VehicleSafetyExtensions_st_ptr vecSafety_ptr)
{
    BSMpartIIExtension_t *element_ptr = NULL;

    
    /* Allocate partii element structure. */
    element_ptr = calloc(1, sizeof(BSMpartIIExtension_t));
    if(element_ptr == NULL)
    {
        goto ERR_EXIT;
    }

    /* Set partii code. */
    element_ptr->present = BSMpartIIExtension_PR_vehicleSafetyExt;

    /* Set events. */
    if(vecSafety_ptr->opt.events == MSG_OPTIONAL_YES)
    {
        uint16_t event_mask = 0;
    
        element_ptr->choice.vehicleSafetyExt.events = calloc(1, sizeof(VehicleEventFlags_t));
        if(element_ptr->choice.vehicleSafetyExt.events == NULL)
        {
            goto ERR_EXIT;
        }
        element_ptr->choice.vehicleSafetyExt.events->size = 2;
        element_ptr->choice.vehicleSafetyExt.events->bits_unused = 3;
        element_ptr->choice.vehicleSafetyExt.events->buf = calloc(1, element_ptr->choice.vehicleSafetyExt.events->size);
        if(element_ptr->choice.vehicleSafetyExt.events->buf == NULL)
        {
            goto ERR_EXIT;
        }

        /* Encode event data. */
        if(vecSafety_ptr->events.eventHazardLights == 1) 
        {
            event_mask |= (0x0001 << (15 - VehicleEventFlags_eventHazardLights));
        }

        if(vecSafety_ptr->events.eventHardBraking == 1) 
        {
            event_mask |= (0x0001 << (15 - VehicleEventFlags_eventHardBraking));
        }

        if(vecSafety_ptr->events.eventDisabledVehicle == 1) 
        {
            event_mask |= (0x0001 << (15 - VehicleEventFlags_eventDisabledVehicle));
        }
        
        element_ptr->choice.vehicleSafetyExt.events->buf[0] = (event_mask >> 8) & 0x00FF;
        element_ptr->choice.vehicleSafetyExt.events->buf[1] = event_mask & 0x00FF;
    }
    else
    {
        element_ptr->choice.vehicleSafetyExt.events = NULL;
    }

    /* Set path history. */
    if(vecSafety_ptr->opt.pathHistory == MSG_OPTIONAL_YES)
    {
        element_ptr->choice.vehicleSafetyExt.pathHistory = NULL;
    }
    else
    {
        element_ptr->choice.vehicleSafetyExt.pathHistory = NULL;
    }

    /* Set path prediction. */
    if(vecSafety_ptr->opt.pathPrediction == MSG_OPTIONAL_YES)
    {
        element_ptr->choice.vehicleSafetyExt.pathPrediction = NULL;
    }
    else
    {
        element_ptr->choice.vehicleSafetyExt.pathPrediction = NULL;
    }

    /* set lights. */
    if(vecSafety_ptr->opt.lights == MSG_OPTIONAL_YES)
    {
        element_ptr->choice.vehicleSafetyExt.lights = NULL;
    }
    else
    {
        element_ptr->choice.vehicleSafetyExt.lights = NULL;
    }

    /* Set element pointer. */
    *ext_ptr_ptr = element_ptr;

    return 0;

    
ERR_EXIT:

    /* Set element pointer. */
    *ext_ptr_ptr = NULL;
    bsm_free_partii_vehicleSafetyExt(element_ptr);
    
    return -1;
}


/* Parse bsm partii element vehicleSafetyExt. */
static int bsm_parse_partii_vehicleSafetyExt(BSMpartIIExtension_t *element_ptr, DF_VehicleSafetyExtensions_st_ptr vehicleSafety_ptr)
{  
    /* Get events. */
    if(element_ptr->choice.vehicleSafetyExt.events != NULL)
    {
        if(element_ptr->choice.vehicleSafetyExt.events->buf != NULL)
        {
            uint16_t event_mask = 0;

            event_mask = element_ptr->choice.vehicleSafetyExt.events->buf[0];
            event_mask = (event_mask << 8) | element_ptr->choice.vehicleSafetyExt.events->buf[1]; 

            /* Decode event data to alert mask. */
            if(event_mask & (0x0001 << (15 - VehicleEventFlags_eventHazardLights))) 
            {
                vehicleSafety_ptr->events.eventHazardLights = 1;        
            }

            if(event_mask & (0x0001 << (15 - VehicleEventFlags_eventHardBraking))) 
            {
                vehicleSafety_ptr->events.eventHardBraking = 1;     
            }

            if(event_mask & (0x0001 << (15 - VehicleEventFlags_eventDisabledVehicle))) 
            {
                vehicleSafety_ptr->events.eventDisabledVehicle = 1;     
            }
        }
    }

    /* Get path history. */
    if(element_ptr->choice.vehicleSafetyExt.pathHistory != NULL)
    {
        ;
    }
    else
    {
        ;
    }

    /* Get path prediction. */
    if(element_ptr->choice.vehicleSafetyExt.pathPrediction != NULL)
    {
        ;
    }
    else
    {
        ;
    }

    /* Get lights. */
    if(element_ptr->choice.vehicleSafetyExt.lights != NULL)
    {
        ;
    }
    else
    {
        ;
    }


    return 0;
}


/* Allocate partii element specialVehicleExt. */
static int bsm_allocate_partii_specialVehicleExt(BSMpartIIExtension_t **ext_ptr_ptr, DF_SpecialVehicleExtensions_st_ptr specialVec_ptr)
{
    return -1;
}


/* Free partii element specialVehicleExt. */
static int bsm_free_partii_specialVehicleExt(BSMpartIIExtension_t *element_ptr)
{
    return -1;    
}


/* Parse bsm partii element specialVehicleExt. */
static int bsm_parse_partii_specialVehicleExt(BSMpartIIExtension_t *element_ptr, DF_SpecialVehicleExtensions_st_ptr specialVec_ptr)
{  
    return -1;
}


/* Allocate partii element supplementalVehicleExt. */
static int bsm_allocate_partii_supplementalVehicleExt(BSMpartIIExtension_t ** ext_ptr_ptr, DF_SupplementalVehicleExtensions_st_ptr supplementalVec_ptr)
{
    return -1;
}


/* Free partii element supplementalVehicleExt. */
static int bsm_free_partii_supplementalVehicleExt(BSMpartIIExtension_t *element_ptr)
{
    return -1;
}


/* Parse bsm partii element specialVehicleExt. */
static int bsm_parse_partii_supplementalVehicleExt(BSMpartIIExtension_t *element_ptr, DF_SupplementalVehicleExtensions_st_ptr supplementalVec_ptr)
{  
    return -1;
}


/* Free bsm partii element main routine. */
static void bsm_free_partii_element(BSMpartIIExtension_t *element_ptr)
{
    switch(element_ptr->present)
    {
        case BSMpartIIExtension_PR_vehicleSafetyExt:
        {
            bsm_free_partii_vehicleSafetyExt(element_ptr);        break;
        }
        case BSMpartIIExtension_PR_specialVehicleExt:
        {
            bsm_free_partii_specialVehicleExt(element_ptr);       break;
        }
        case BSMpartIIExtension_PR_vehicleDummySt1:
        {
            bsm_free_partii_supplementalVehicleExt(element_ptr);  break;
        }
        default:
        {
            break;
        }
    }
}


/* Free bsm partii structure. */
static int bsm_free_partii_structure(BasicSafetyMessage_t *bsm_ptr)
{
    if(bsm_ptr != NULL)
    {
        /* Partii structure domain. */
        if(bsm_ptr->bsmBody.partII != NULL)
        {
            /* Free the contents of the sequence,do not free the sequence itself. */
            asn_sequence_empty(&(bsm_ptr->bsmBody.partII->list));
            
            free(bsm_ptr->bsmBody.partII);

            bsm_ptr->bsmBody.partII = NULL;
        }
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate bsm partii structure. */
static int bsm_allocate_partii_structure(BasicSafetyMessage_t *bsm_ptr, DF_BSMpartIIExtension_st_ptr partII_ptr) 
{
    BSMpartIIExtension_t *element_ptr = NULL;

    
    /* Allocate partii structure pointer. */
    bsm_ptr->bsmBody.partII = calloc(1, sizeof(*(bsm_ptr->bsmBody.partII)));
    if(bsm_ptr->bsmBody.partII == NULL)
    {
        return -1;
    }
    
    /* Set partii element free routine. */
    bsm_ptr->bsmBody.partII->list.free = bsm_free_partii_element;

    /* Allocate partii element vehicleSafetyExt structure. */
    if(partII_ptr->opt.vecSafetyExt == MSG_OPTIONAL_YES)
    {
        if(bsm_allocate_partii_vehicleSafetyExt(&element_ptr, &(partII_ptr->vecSafetyExt)) == 0)
        {
            /* Add partii element to partii structure pointer. */
            if(asn_sequence_add(&(bsm_ptr->bsmBody.partII->list), element_ptr) != 0)
            {
                goto ERR_EXIT;
            }   
        }
        else
        {
            goto ERR_EXIT;
        }
    }

    /* Allocate partii element specialVehicleExt structure. */
    if(partII_ptr->opt.specialVecExt == MSG_OPTIONAL_YES)
    {
        if(bsm_allocate_partii_specialVehicleExt(&element_ptr, &(partII_ptr->specialVecExt)) == 0)
        {
            /* Add partii element to partii structure pointer. */
            if(asn_sequence_add(&(bsm_ptr->bsmBody.partII->list), element_ptr) != 0)
            {
                goto ERR_EXIT;
            }   
        }
        else
        {
            goto ERR_EXIT;
        }
    }

    /* Allocate partii element supplementalVehicleExt structure. */
    if(partII_ptr->opt.supplementalVecExt == MSG_OPTIONAL_YES)
    {
        if(bsm_allocate_partii_supplementalVehicleExt(&element_ptr, &(partII_ptr->supplementalVecExt)) == 0)
        {
            /* Add partii element to partii structure pointer. */
            if(asn_sequence_add(&(bsm_ptr->bsmBody.partII->list), element_ptr) != 0)
            {
                goto ERR_EXIT;
            }   
        }
        else
        {
            goto ERR_EXIT;
        }
    }

    return 0;


ERR_EXIT:
    
   bsm_free_partii_structure(bsm_ptr);
    
   return -1;
}


/* Parse bsm partii element main routine. */
static void bsm_parse_partii_element(BSMpartIIExtension_t *element_ptr, DF_BSMpartIIExtension_st_ptr partII_ptr)
{
    switch(element_ptr->present)
    {
        case BSMpartIIExtension_PR_vehicleSafetyExt:
        {
            partII_ptr->opt.vecSafetyExt = MSG_OPTIONAL_YES;
            bsm_parse_partii_vehicleSafetyExt(element_ptr, &(partII_ptr->vecSafetyExt));        

            break;
        }
        case BSMpartIIExtension_PR_specialVehicleExt:
        {
            partII_ptr->opt.specialVecExt = MSG_OPTIONAL_YES;
            bsm_parse_partii_specialVehicleExt(element_ptr, &(partII_ptr->specialVecExt));       

            break;
        }
        case BSMpartIIExtension_PR_vehicleDummySt1:
        {
            partII_ptr->opt.supplementalVecExt = MSG_OPTIONAL_YES;
            bsm_parse_partii_supplementalVehicleExt(element_ptr, &(partII_ptr->supplementalVecExt));  

            break;
        }
        default:
        {
            break;
        }
    }
}


/* Allocate bsm message. */
static int bsm_allocate_msg(BasicSafetyMessage_t **bsm_ptr_ptr, bsm_msg_st_ptr msg_ptr) 
{
    BasicSafetyMessage_t *bsm_ptr = NULL;

    
    /* Allocate the bsm message. */
    bsm_ptr = calloc(1, sizeof(*bsm_ptr));
    if(bsm_ptr == NULL)
    {
        goto ERR_MSG_EXIT;
    }

    /* Mesage id. */
    bsm_ptr->dsrcMsgId = DSRCmsgIDEnum_basicSafetyMessage;

    /* Allocate message parti. */
    if(bsm_allocate_parti_BSMcoreData(&(bsm_ptr->bsmBody.coreData), &(msg_ptr->coreData)) != 0)
    {
        goto ERR_PARTI_EXIT;
    }

    /* Allocate message partii. */
    if( (msg_ptr->partIIExt.opt.vecSafetyExt == MSG_OPTIONAL_YES) 
     || (msg_ptr->partIIExt.opt.specialVecExt == MSG_OPTIONAL_YES) 
     || (msg_ptr->partIIExt.opt.supplementalVecExt == MSG_OPTIONAL_YES))
    {
        if(bsm_allocate_partii_structure(bsm_ptr, &(msg_ptr->partIIExt)) != 0)
        {
            goto ERR_PARTII_EXIT;
        }
    }
    else
    {
        bsm_ptr->bsmBody.partII = NULL;
    }
    
    /* Set bsm pointer. */
    *bsm_ptr_ptr = bsm_ptr;
    
    return 0;


ERR_PARTII_EXIT:

    bsm_free_parti_BSMcoreData(&(bsm_ptr->bsmBody.coreData));

ERR_PARTI_EXIT:

    /* Free message itself domain. */
    free(bsm_ptr);
    
ERR_MSG_EXIT:

    /* Reset bsm pointer. */
    *bsm_ptr_ptr = NULL;
    
    return -1;    
}


/* Free bsm message. */
static int bsm_free_msg(BasicSafetyMessage_t *bsm_ptr)
{
    if(bsm_ptr != NULL)
    {
        /* Free parti. */
        bsm_free_parti_BSMcoreData(&(bsm_ptr->bsmBody.coreData));

        /* Free partii. */
        bsm_free_partii_structure(bsm_ptr);
        
        /* Message itself domain. */
        free(bsm_ptr);
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Parse bsm message into vam status structure. */
static int bsm_parse_msg(BasicSafetyMessage_t *bsm_ptr, bsm_msg_st_ptr msg_ptr)
{
    uint8_t          ele_index = 0;


    /* Parse parti bsmcoredata. */
    bsm_parse_parti_BSMcoreData(&(bsm_ptr->bsmBody.coreData), &(msg_ptr->coreData));

    /* Parse partii when exist. */
    if(bsm_ptr->bsmBody.partII != NULL)
    {
        /* Parse every element in array. */
        for(ele_index = 0; ele_index < bsm_ptr->bsmBody.partII->list.count; ele_index ++)
        {
            bsm_parse_partii_element(bsm_ptr->bsmBody.partII->list.array[ele_index], &(msg_ptr->partIIExt));
        }
    }

    return 0; 
}


/* Build bsm message into the specific buffer. */
int bsm_build_msg(bsm_msg_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *valid_bit_ptr)
{
    int                       result = 0;
    BasicSafetyMessage_t    *bsm_ptr = NULL;
    asn_enc_rval_t       encode_rval = { 0 };


    /* Error detection. */
    if( (msg_ptr == NULL) || (buffer_ptr == NULL) || (buffer_size == 0) || (valid_bit_ptr == NULL) )
    {
        printf(" bsm_build_msg() check error. \n ");
        result = -1;
    }
    else
    {
        /* Allocate bsm message. */
        result = bsm_allocate_msg(&bsm_ptr, msg_ptr);
        if(result != 0)
        {
            printf(" bsm_allocate_msg() is faild. \n ");
            result = -1;
        }
        else
        {
            /* Encode message to UPER format. */
            encode_rval = uper_encode_to_buffer(&asn_DEF_BasicSafetyMessage, bsm_ptr, buffer_ptr, buffer_size);
            if(encode_rval.encoded == -1)
            {
                printf(" uper_encode_to_buffer() is faild. \n ");

                * valid_bit_ptr = 0;
                result = -1;
            }
            else
            {
                * valid_bit_ptr = encode_rval.encoded;
                result = 0;
            }

            /* Printf the bsm message structure when key on. */
            if(KeyPrintfBsmTxStruct != 0)
            {
                bsm_printf_msgstruct("\nBSM Tx-------------------->", bsm_ptr);
            }       
            
            /* Printf bsm messge UPER data when key on. */
            if(KeyPrintfBsmTxData != 0)
            {
                bsm_printf_msgdata("\nBSM Tx-------------------->", buffer_ptr, *valid_bit_ptr);
            } 

            /* Free message. */
            bsm_free_msg(bsm_ptr);
        }
    }
  
    return result; 
}


/* Parse bsm message from the specific buffer. */
int bsm_analyse_msg(bsm_msg_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size)
{
    int                    result = 0;
    BasicSafetyMessage_t *bsm_ptr = NULL;
    
    asn_dec_rval_t    decode_rval = { 0 };
    asn_codec_ctx_t    decode_ctx = { 0 };


    /* Error detection. */
    if((msg_ptr == NULL) || (buffer_ptr == NULL) || (buffer_size == 0))
    {
        printf(" bsm_analyse_msg() check error. \n ");
        result = -1;
    }
    else
    {
        /* Dncode message from UPER format. */
        decode_rval = uper_decode(&decode_ctx, &asn_DEF_BasicSafetyMessage,(void **)&bsm_ptr, buffer_ptr, buffer_size, 0, 0);
        if(decode_rval.code != RC_OK)
        {
            printf(" uper_decode() is faild. \n ");
            result = -1;
        }
        else
        {
            /* Parse the bsm message. */
            result = bsm_parse_msg(bsm_ptr, msg_ptr); 
        }

        /* Printf the bsm message structure when key on. */
        if(KeyPrintfBsmRxStruct != 0)
        {
            bsm_printf_msgstruct("\nBSM Rx-------------------->", bsm_ptr);
        }       

        /* Printf bsm messge UPER data when key on. */
        if(KeyPrintfBsmRxData != 0)
        {
            bsm_printf_msgdata("\nBSM Rx-------------------->", buffer_ptr, buffer_size * 8);
        }
    }

    return result; 
}

