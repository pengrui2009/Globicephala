/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_vam_bsm.c
 @brief  : MSG_BasicSafetyMessage(BSM) routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/
#include "cv_vam_bsm.h"
#include "BasicSafetyMessage.h"
#include "J2735.h"


/* Key for bsm tx message structure and UPER data printf. */
static uint8_t KeyPrintfBsmTxStruct = 1;
static uint8_t KeyPrintfBsmTxData = 1;

/* Key for bsm rx message structure and UPER data printf. */
static uint8_t KeyPrintfBsmRxStruct = 1;
static uint8_t KeyPrintfBsmRxData = 1;


/* Printf bsm message structure data. */
static void bsm_printf_msgstruct(const char *str, BasicSafetyMessage_t *bsm_ptr);

/* Printf bsm message PER data. */
static void bsm_printf_msgdata(const char *str, uint8_t *buffer_ptr, uint16_t valid_bit);

/* Allocate bsm message. */
static int bsm_allocate_msg(BasicSafetyMessage_t **bsm_ptr_ptr, bsm_msg_optional_st_ptr optional_ptr); 

/* Free bsm message. */
static int bsm_free_msg(BasicSafetyMessage_t *bsm_ptr);

/* Parse bsm message into vam status structure. */
static int bsm_parse_msg(BasicSafetyMessage_t *bsm_ptr, vam_stastatus_t *vamstatus_ptr);

/* Add bsm parti BSMcoreData into message. */
static int bsm_allocate_parti_BSMcoreData(BSMcoreData_t *coredata_ptr, vam_stastatus_t *vamstatus_ptr);

/* Free bsm parti BSMcoreData from message. */
static int bsm_free_parti_BSMcoreData(BSMcoreData_t *coredata_ptr);

/* Parse bsm parti BSMcoreData from message. */
static int bsm_parse_parti_BSMcoreData(BSMcoreData_t *coredata_ptr, vam_stastatus_t *vamstatus_ptr);

/* Allocate bsm partii structure. */
static int bsm_allocate_partii_structure(BasicSafetyMessage_t *bsm_ptr, bsm_msg_optional_st_ptr optional_ptr, vam_stastatus_t *vamstatus_ptr);

/* Free bsm partii structure. */
static int bsm_free_partii_structure(BasicSafetyMessage_t *bsm_ptr);

/* Allocate bsm partii element vehicleSafetyExt. */
static int bsm_allocate_partii_vehicleSafetyExt(BSMpartIIExtension_t **ext_ptr_ptr, bsm_msg_optional_st_ptr optional_ptr, vam_stastatus_t *vamstatus_ptr);

/* Free bsm partii element vehicleSafetyExt. */
static int bsm_free_partii_vehicleSafetyExt(BSMpartIIExtension_t *element_ptr);

/* Allocate partii element specialVehicleExt. */
static int bsm_allocate_partii_specialVehicleExt(BSMpartIIExtension_t **ext_ptr_ptr, bsm_msg_optional_st_ptr optional_ptr);

/* Free partii element specialVehicleExt. */
static int bsm_free_partii_specialVehicleExt(BSMpartIIExtension_t *element_ptr);

/* Allocate partii element supplementalVehicleExt. */
static int bsm_allocate_partii_supplementalVehicleExt(BSMpartIIExtension_t **ext_ptr_ptr, bsm_msg_optional_st_ptr optional_ptr);

/* Free partii element supplementalVehicleExt. */
static int bsm_free_partii_supplementalVehicleExt(BSMpartIIExtension_t *element_ptr);

/* Free bsm partii element main routine. */
static void bsm_free_partii_element(BSMpartIIExtension_t *element_ptr);

/* Parse bsm partii element main routine. */
static void bsm_parse_partii_element(BSMpartIIExtension_t *element_ptr, vam_stastatus_t *vamstatus_ptr);


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


/* Build bsm message into the specific buffer. */
int bsm_build_msg(bsm_msg_optional_st_ptr optional_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *valid_bit_ptr)
{
    int                       result = 0;
    BasicSafetyMessage_t    *bsm_ptr = NULL;
    asn_enc_rval_t       encode_rval = { 0 };


    /* Error detection. */
    if( (optional_ptr == NULL) || (buffer_ptr == NULL) || (buffer_size == 0) || (valid_bit_ptr == NULL) )
    {
        printf(" bsm_build_msg() check error. \n ");
        result = -1;
    }
    else
    {
        /* Allocate bsm message. */
        result = bsm_allocate_msg(&bsm_ptr, optional_ptr);
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
int bsm_analyse_msg(vam_envar_t *vam_ptr, uint8_t *buffer_ptr, uint16_t buffer_size)
{
    int                    result = 0;
    BasicSafetyMessage_t *bsm_ptr = NULL;
    
    asn_dec_rval_t    decode_rval = { 0 };
    asn_codec_ctx_t    decode_ctx = { 0 };

    vam_sta_node_t  *vam_node_ptr = NULL;


    /* Error detection. */
    if((buffer_ptr == NULL) || (buffer_size == 0))
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
            /* Only when not my own's bsm message and find the specific vam node. */
            if( (0 == memcmp(bsm_ptr->bsmBody.coreData.id.buf, vam_ptr->local.pid, RCP_TEMP_ID_LEN))
              && ((vam_node_ptr = vam_find_sta(vam_ptr, bsm_ptr->bsmBody.coreData.id.buf)) != NULL) )
            {
                /* Parse the bsm message. */
                result = bsm_parse_msg(bsm_ptr, &(vam_node_ptr->s)); 
            }
            else
            {
                result = -1;
            }
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


/* Allocate bsm message. */
static int bsm_allocate_msg(BasicSafetyMessage_t **bsm_ptr_ptr, bsm_msg_optional_st_ptr optional_ptr) 
{
    BasicSafetyMessage_t *bsm_ptr = NULL;
    vam_stastatus_t     vam_local = { 0 };

    
    /* Allocate the bsm message. */
    bsm_ptr = calloc(1, sizeof(BasicSafetyMessage_t));
    if(bsm_ptr == NULL)
    {
        goto ERR_MSG_EXIT;
    }

    /* Mesage id. */
    bsm_ptr->dsrcMsgId = RCP_MSG_ID_BSM;

    /* Get local current vam status. */
    vam_get_local_current_status(&vam_local);

    /* Allocate message parti. */
    if(bsm_allocate_parti_BSMcoreData(&(bsm_ptr->bsmBody.coreData), &vam_local) != 0)
    {
        goto ERR_PARTI_EXIT;
    }

    /* Allocate message partii. */
    if(optional_ptr->msg_partii == MSG_OPTIONAL_YES)
    {
        if(bsm_allocate_partii_structure(bsm_ptr, optional_ptr, &vam_local) != 0)
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
static int bsm_parse_msg(BasicSafetyMessage_t *bsm_ptr, vam_stastatus_t *vamstatus_ptr)
{
    uint8_t          ele_index = 0;


    /* Parse parti bsmcoredata. */
    bsm_parse_parti_BSMcoreData(&(bsm_ptr->bsmBody.coreData), vamstatus_ptr);

    /* Parse partii when exist. */
    if(bsm_ptr->bsmBody.partII != NULL)
    {
        /* Parse every element in array. */
        for(ele_index = 0; ele_index < bsm_ptr->bsmBody.partII->list.count; ele_index ++)
        {
            bsm_parse_partii_element(bsm_ptr->bsmBody.partII->list.array[ele_index], vamstatus_ptr);
        }
    }

    return 0; 
}


/* Add bsm parti BSMcoreData into message. */
static int bsm_allocate_parti_BSMcoreData(BSMcoreData_t *coredata_ptr, vam_stastatus_t *vamstatus_ptr)
{
    static uint8_t  msg_count = 0;


    /* Message count. */
    coredata_ptr->msgCnt = msg_count ++;
    if(127 < msg_count)
    {
        msg_count = 0;
    }

    /* Id. */
    coredata_ptr->id.size = RCP_TEMP_ID_LEN;
    coredata_ptr->id.buf = calloc(1, coredata_ptr->id.size);
    if(coredata_ptr->id.buf == NULL)
    {
        goto ERR_EXIT;                  
    }
    memcpy(coredata_ptr->id.buf, vamstatus_ptr->pid, RCP_TEMP_ID_LEN);

    /* Dsecond. */
    coredata_ptr->secMark = osal_get_systime();

    /* Postion. */
    coredata_ptr->lat = encode_latitude(vamstatus_ptr->pos.latitude);
    coredata_ptr->Long = encode_longitude(vamstatus_ptr->pos.longitude);
    coredata_ptr->elev = encode_elevation(vamstatus_ptr->pos.elevation);

    /* Position accuracy. */
    coredata_ptr->accuracy.semiMajor = encode_semimajor_axis_accuracy(vamstatus_ptr->pos_accuracy.semi_major_accu);
    coredata_ptr->accuracy.semiMinor = encode_semiminor_axis_accuracy(vamstatus_ptr->pos_accuracy.semi_minor_accu);
    coredata_ptr->accuracy.orientation = encode_semimajor_axis_orientation(vamstatus_ptr->pos_accuracy.semi_major_orientation);

    /* Speed angle. */
    coredata_ptr->transmission = vamstatus_ptr->transmission_state;
    coredata_ptr->speed = encode_absolute_velocity(vamstatus_ptr->speed);
    coredata_ptr->heading = encode_angle(vamstatus_ptr->dir);
    coredata_ptr->angle = encode_steer_wheel_angle(vamstatus_ptr->steer_wheel_angle);

    /* Acceleration set. */
    coredata_ptr->accelSet.Long = encode_acceleration(vamstatus_ptr->acce_set.longitudinal);;
    coredata_ptr->accelSet.lat = encode_acceleration(vamstatus_ptr->acce_set.lateral);;
    coredata_ptr->accelSet.vert = encode_vertical_acceleration(vamstatus_ptr->acce_set.vertical);;
    coredata_ptr->accelSet.yaw = encode_yawrate(vamstatus_ptr->acce_set.yaw_rate);;

    /* Brakes status. */
    coredata_ptr->brakes.wheelBrakes.size = 1;
    coredata_ptr->brakes.wheelBrakes.bits_unused = 3;
    coredata_ptr->brakes.wheelBrakes.buf = calloc(1, coredata_ptr->brakes.wheelBrakes.size);
    if(coredata_ptr->brakes.wheelBrakes.buf == NULL)
    {
        goto ERR_EXIT;
    }
    (*coredata_ptr->brakes.wheelBrakes.buf) = (vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.unavailable << 7) 
                                                 | (vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.leftfront << 6) 
                                                 | (vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.leftrear << 5)
                                                 | (vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.rightfront << 4) 
                                                 | (vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.rightrear << 3);
    
    coredata_ptr->brakes.traction = vamstatus_ptr->brake_stat.traction;
    coredata_ptr->brakes.abs = vamstatus_ptr->brake_stat.abs;
    coredata_ptr->brakes.scs = vamstatus_ptr->brake_stat.scs;
    coredata_ptr->brakes.brakeBoost = vamstatus_ptr->brake_stat.brakeboost;
    coredata_ptr->brakes.auxBrakes = vamstatus_ptr->brake_stat.auxbrakes;

    /* Vehicle size. */
    coredata_ptr->size.width = encode_vehicle_width(vamstatus_ptr->vec_size.vec_width);
    coredata_ptr->size.length = encode_vehicle_length(vamstatus_ptr->vec_size.vec_length);

    return 0;
    

ERR_EXIT:

    bsm_free_parti_BSMcoreData(coredata_ptr);

    return -1;
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


/* Parse bsm parti BSMcoreData from message. */
static int bsm_parse_parti_BSMcoreData(BSMcoreData_t *coredata_ptr, vam_stastatus_t *vamstatus_ptr)
{
    /* Dsecond. */    
    vamstatus_ptr->dsecond = coredata_ptr->secMark;
    vamstatus_ptr->time = osal_get_systemtime();

    /* Positon. */
    vamstatus_ptr->pos.latitude = decode_latitude(coredata_ptr->lat);
    vamstatus_ptr->pos.longitude = decode_longitude(coredata_ptr->Long);
    vamstatus_ptr->pos.elevation = decode_elevation(coredata_ptr->elev);

    /* Position accuracy. */
    vamstatus_ptr->pos_accuracy.semi_major_accu = decode_semimajor_axis_accuracy(coredata_ptr->accuracy.semiMajor);
    vamstatus_ptr->pos_accuracy.semi_minor_accu = decode_semiminor_axis_accuracy(coredata_ptr->accuracy.semiMinor);
    vamstatus_ptr->pos_accuracy.semi_major_orientation = decode_semimajor_axis_orientation(coredata_ptr->accuracy.orientation);

    /* Speed angle. */
    vamstatus_ptr->transmission_state = coredata_ptr->transmission;
    vamstatus_ptr->speed = decode_absolute_velocity(coredata_ptr->speed);
    vamstatus_ptr->dir = decode_angle(coredata_ptr->heading);
    vamstatus_ptr->steer_wheel_angle = decode_steer_wheel_angle(coredata_ptr->angle);

    /* Acceleration set. */
    vamstatus_ptr->acce_set.longitudinal = decode_acceleration(coredata_ptr->accelSet.Long);
    vamstatus_ptr->acce_set.lateral = decode_acceleration(coredata_ptr->accelSet.lat);
    vamstatus_ptr->acce_set.vertical = decode_vertical_acceleration(coredata_ptr->accelSet.vert);
    vamstatus_ptr->acce_set.yaw_rate = decode_yawrate(coredata_ptr->accelSet.yaw);

    /* Brakes status. */
    vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.unavailable = (*coredata_ptr->brakes.wheelBrakes.buf) & 0x80; 
    vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.leftfront =  (*coredata_ptr->brakes.wheelBrakes.buf) & 0x40; 
    vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.leftrear = (*coredata_ptr->brakes.wheelBrakes.buf) & 0x20; 
    vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.rightfront = (*coredata_ptr->brakes.wheelBrakes.buf) & 0x10; 
    vamstatus_ptr->brake_stat.wheel_brakes.wheel_brake_bit.rightrear = (*coredata_ptr->brakes.wheelBrakes.buf) & 0x08; 
    
    vamstatus_ptr->brake_stat.traction = coredata_ptr->brakes.traction;
    vamstatus_ptr->brake_stat.abs = coredata_ptr->brakes.abs;
    vamstatus_ptr->brake_stat.scs = coredata_ptr->brakes.scs;
    vamstatus_ptr->brake_stat.brakeboost = coredata_ptr->brakes.brakeBoost;
    vamstatus_ptr->brake_stat.auxbrakes = coredata_ptr->brakes.auxBrakes;

    /* Vehicle size. */
    vamstatus_ptr->vec_size.vec_length = decode_vehicle_length(coredata_ptr->size.length);
    vamstatus_ptr->vec_size.vec_width = decode_vehicle_width(coredata_ptr->size.width);
    
    return 0;
}


/* Allocate bsm partii structure. */
static int bsm_allocate_partii_structure(BasicSafetyMessage_t *bsm_ptr, bsm_msg_optional_st_ptr optional_ptr, vam_stastatus_t *vamstatus_ptr) 
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
    if(optional_ptr->partii_vehicle_safety_ext == MSG_OPTIONAL_YES)
    {
        if(bsm_allocate_partii_vehicleSafetyExt(&element_ptr, optional_ptr, vamstatus_ptr) == 0)
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
    if(optional_ptr->partii_special_vehicle_ext == MSG_OPTIONAL_YES)
    {
        if(bsm_allocate_partii_specialVehicleExt(&element_ptr, optional_ptr) == 0)
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
    if(optional_ptr->partii_supplemental_vehicle_ext == MSG_OPTIONAL_YES)
    {
        if(bsm_allocate_partii_supplementalVehicleExt(&element_ptr, optional_ptr) == 0)
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


/* Allocate bsm partii element vehicleSafetyExt. */
static int bsm_allocate_partii_vehicleSafetyExt(BSMpartIIExtension_t **ext_ptr_ptr, bsm_msg_optional_st_ptr optional_ptr, vam_stastatus_t *vamstatus_ptr)
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
    if(optional_ptr->vse_vehicle_event_flags == MSG_OPTIONAL_YES)
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

        /* Encode alert mask to event data. */
        if(vamstatus_ptr->alert_mask & VAM_ALERT_MASK_VBD) 
        {
            event_mask |= (0x0001 << (15 - VehicleEventFlags_eventHazardLights));
        }

        if(vamstatus_ptr->alert_mask & VAM_ALERT_MASK_EBD) 
        {
            event_mask |= (0x0001 << (15 - VehicleEventFlags_eventHardBraking));
        }

        if(vamstatus_ptr->alert_mask & VAM_ALERT_MASK_VOT) 
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
    if(optional_ptr->vse_path_history == MSG_OPTIONAL_YES)
    {
        element_ptr->choice.vehicleSafetyExt.pathHistory = NULL;
    }
    else
    {
        element_ptr->choice.vehicleSafetyExt.pathHistory = NULL;
    }

    /* Set path prediction. */
    if(optional_ptr->vse_path_prediction == MSG_OPTIONAL_YES)
    {
        element_ptr->choice.vehicleSafetyExt.pathPrediction = NULL;
    }
    else
    {
        element_ptr->choice.vehicleSafetyExt.pathPrediction = NULL;
    }

    /* set lights. */
    if(optional_ptr->vse_exterior_lights == MSG_OPTIONAL_YES)
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


/* Parse bsm partii element vehicleSafetyExt. */
static int bsm_parse_partii_vehicleSafetyExt(BSMpartIIExtension_t *element_ptr, vam_stastatus_t *vamstatus_ptr)
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
                vamstatus_ptr->alert_mask |= VAM_ALERT_MASK_VBD;        
            }

            if(event_mask & (0x0001 << (15 - VehicleEventFlags_eventHardBraking))) 
            {
                vamstatus_ptr->alert_mask |= VAM_ALERT_MASK_EBD;        
            }

            if(event_mask & (0x0001 << (15 - VehicleEventFlags_eventDisabledVehicle))) 
            {
                vamstatus_ptr->alert_mask |= VAM_ALERT_MASK_VOT;        
            }
        }
    }
    else
    {
        vamstatus_ptr->alert_mask = 0;
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
static int bsm_allocate_partii_specialVehicleExt(BSMpartIIExtension_t **ext_ptr_ptr, bsm_msg_optional_st_ptr optional_ptr)
{
    return -1;
}


/* Free partii element specialVehicleExt. */
static int bsm_free_partii_specialVehicleExt(BSMpartIIExtension_t *element_ptr)
{
    return -1;    
}


/* Parse bsm partii element specialVehicleExt. */
static int bsm_parse_partii_specialVehicleExt(BSMpartIIExtension_t *element_ptr, vam_stastatus_t *vamstatus_ptr)
{  
    return -1;
}


/* Allocate partii element supplementalVehicleExt. */
static int bsm_allocate_partii_supplementalVehicleExt(BSMpartIIExtension_t ** ext_ptr_ptr, bsm_msg_optional_st_ptr optional_ptr)
{
    return -1;
}


/* Free partii element supplementalVehicleExt. */
static int bsm_free_partii_supplementalVehicleExt(BSMpartIIExtension_t *element_ptr)
{
    return -1;
}


/* Parse bsm partii element specialVehicleExt. */
static int bsm_parse_partii_supplementalVehicleExt(BSMpartIIExtension_t *element_ptr, vam_stastatus_t *vamstatus_ptr)
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


/* Parse bsm partii element main routine. */
static void bsm_parse_partii_element(BSMpartIIExtension_t *element_ptr, vam_stastatus_t *vamstatus_ptr)
{
    switch(element_ptr->present)
    {
        case BSMpartIIExtension_PR_vehicleSafetyExt:
        {
            bsm_parse_partii_vehicleSafetyExt(element_ptr, vamstatus_ptr);        break;
        }
        case BSMpartIIExtension_PR_specialVehicleExt:
        {
            bsm_parse_partii_specialVehicleExt(element_ptr, vamstatus_ptr);       break;
        }
        case BSMpartIIExtension_PR_vehicleDummySt1:
        {
            bsm_parse_partii_supplementalVehicleExt(element_ptr, vamstatus_ptr);  break;
        }
        default:
        {
            break;
        }
    }
}



