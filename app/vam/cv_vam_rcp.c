/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_vam_rcp.c
 @brief  : this file realize the vehicle Remote Communicat Protocol
 @author : wangyifeng
 @history:
           2014-6-17    wangyifeng    Created file
           2014-7-29    wanglei       modified file: process evam msg 
           ...
******************************************************************************/
#include "cv_osal.h"
#define OSAL_MODULE_DEBUG
#define OSAL_MODULE_DEBUG_LEVEL OSAL_DEBUG_INFO
#define MODULE_NAME "rcp"
#include "cv_osal_dbg.h"
#include "cv_vam_api.h"
#include "cv_vam.h"
#include "cv_cms_def.h"
#include "cv_wnet.h"
#include "J2735.h"
#include "app_msg_format.h"


#include "MessageModuleSet.h"
#include "BasicSafetyMessage.h"



/*****************************************************************************
 * declaration of variables and functions                                    *
*****************************************************************************/
itis_codes_t itiscode[RSA_TYPE_MAX+1] = 
{
#undef xx
#define xx(SEQ, TYPE, ITISCODE) (ITISCODE),
    VAM_RSA_TYPE_2_ITISCODE
};

float rcp_dbg_distance = 0;




/*---------------------------------------MSG_BasicSafetyMessage(BSM)---------------------------------------*/


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

/* Build bsm message into the specific buffer. */
static int bsm_build_msg(bsm_msg_optional_st_ptr optional_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *valid_bit_ptr);

/* Parse bsm message from the specific buffer. */
static int bsm_analyse_msg(vam_envar_t *vam_ptr, uint8_t *buffer_ptr, uint16_t buffer_size);

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
static int bsm_build_msg(bsm_msg_optional_st_ptr optional_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *valid_bit_ptr)
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
static int bsm_analyse_msg(vam_envar_t *vam_ptr, uint8_t *buffer_ptr, uint16_t buffer_size)
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
    coredata_ptr->speed = encode_absolute_velocity(vamstatus_ptr->speed);;
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
    if(optional_ptr->vse_vehicle_event_flags == BSM_PARTII_OPTIONAL_YES)
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
    if(optional_ptr->vse_path_history == BSM_PARTII_OPTIONAL_YES)
    {
        element_ptr->choice.vehicleSafetyExt.pathHistory = NULL;
    }
    else
    {
        element_ptr->choice.vehicleSafetyExt.pathHistory = NULL;
    }

    /* Set path prediction. */
    if(optional_ptr->vse_path_prediction == BSM_PARTII_OPTIONAL_YES)
    {
        element_ptr->choice.vehicleSafetyExt.pathPrediction = NULL;
    }
    else
    {
        element_ptr->choice.vehicleSafetyExt.pathPrediction = NULL;
    }

    /* set lights. */
    if(optional_ptr->vse_exterior_lights == BSM_PARTII_OPTIONAL_YES)
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




/*---------------------------------------MSG_RoadSideAlert(RSA)---------------------------------------*/



/*---------------------------------------Remote Communicate Protocol----------------------------------*/


/*****************************************************************************
 * implementation of functions                                               *
*****************************************************************************/

/* BEGIN: Added by wanglei, 2015/1/4. for rsa test */
__COMPILE_INLINE__ uint16_t encode_itiscode(uint16_t rsa_mask, itis_codes_t *p_des)
{
    uint16_t r = 0;
    int bit;

    for(bit=0; bit<9; bit++)
    {
        if(rsa_mask & (1<<bit)){
            if (0 == bit){
                r = cv_ntohs(itiscode[bit]);
            }
            else{
                p_des[bit-1] = cv_ntohs(itiscode[bit]);
            }
        }
    }
    
    return r;
}

static void itiscode_2_rsa_mask(itis_codes_t type, uint16_t *rsa_mask)
{
    int i = 0;
    for (i=0; i<RSA_TYPE_MAX; i++)
    {
        if (itiscode[i] == type){
           *rsa_mask |= 1<<i;
           break;
        }
    }
}
__COMPILE_INLINE__ uint16_t decode_itiscode(itis_codes_t typeEvent, itis_codes_t *p_des)
{
    uint16_t k = 0;
	uint16_t rsa_mask = 0;
	uint16_t r;
    r = cv_ntohs(typeEvent);
    itiscode_2_rsa_mask(r, &rsa_mask);    
    for(k=0; k<8; k++)
    {
        r = cv_ntohs(p_des[k]);
        itiscode_2_rsa_mask(r, &rsa_mask);    
    }
    return rsa_mask;

}


int rcp_mda_process(uint8_t msg_hops, 
                      uint8_t msg_count,
                      uint8_t *p_temp_id, 
                      uint8_t *p_forward_id,
                      uint8_t * data,
                      uint32_t datalen)
{
    mda_msg_info_t src;
    mda_envar_t * p_mda;
    int ret;

    p_mda = &p_cms_envar->mda;
    src.left_hops = msg_hops;
    src.msg_count = msg_count;
    memcpy(src.temorary_id, p_temp_id, RCP_TEMP_ID_LEN);
    memcpy(src.forward_id, p_forward_id, RCP_TEMP_ID_LEN);
    
    ret = mda_handle(p_mda, &src, NULL, data, datalen);
    return ret;
}


/* Parse bsm message from others. */
int rcp_parse_bsm(vam_envar_t *p_vam, wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{
    return bsm_analyse_msg(p_vam, databuf, datalen);
}


int rcp_parse_evam(vam_envar_t *p_vam, wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{
    vam_sta_node_t *p_sta;
    rcp_msg_emergency_vehicle_alert_t *p_evam;
    uint16_t alert_mask;

    if (datalen < sizeof(rcp_msg_emergency_vehicle_alert_t)){
        return -1;
    }

    p_evam = (rcp_msg_emergency_vehicle_alert_t *)databuf;

    if (0 == memcmp(p_evam->temporary_id, p_vam->local.pid, RCP_TEMP_ID_LEN)){
        return 0;
    }
    
    rcp_mda_process(p_evam->msg_id.hops, p_evam->rsa.msg_count, 
                     p_evam->temporary_id, p_evam->forward_id, databuf, datalen);


    //TBD
    alert_mask = decode_itiscode(p_evam->rsa.typeEvent, p_evam->rsa.description);
    //rt_kprintf("recv evam: alert_mask = 0x%04x\r\n", alert_mask);

    p_sta = vam_find_sta(p_vam, p_evam->temporary_id);
    if(p_sta != NULL)
    {
    	p_sta->s.dsecond = cv_ntohs(p_evam->rsa.time_stamp);
        p_sta->s.time = osal_get_systemtime();

        p_sta->s.pos.longitude = decode_longitude(p_evam->rsa.position.lon);
        p_sta->s.pos.latitude = decode_latitude(p_evam->rsa.position.lat);
        p_sta->s.pos.elevation = decode_elevation(p_evam->rsa.position.elev);

        p_sta->s.dir = decode_angle(p_evam->rsa.position.heading);
        p_sta->s.speed = decode_absolute_velocity(p_evam->rsa.position.speed.speed);
#if 0
        p_sta->s.acce_set.longitudinal = decode_acceleration(p_evam->motion.acce.lon);
        p_sta->s.acce_set.lateral = decode_acceleration(p_evam->motion.acce.lat);
        p_sta->s.acce_set.vertical = decode_vertical_acceleration(p_evam->motion.acce.vert);
        p_sta->s.acce_set.yaw_rate = decode_yawrate(p_evam->motion.acce.yaw);
#endif
        p_sta->s.alert_mask = alert_mask;

        /* inform the app layer once */
        if(p_vam->evt_handler[VAM_EVT_EVA_UPDATE] != NULL)
        {
            (p_vam->evt_handler[VAM_EVT_EVA_UPDATE])(&p_sta->s);
        }
    }
    return 0;
}


int rcp_parse_rsa(vam_envar_t *p_vam, wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{
    rcp_msg_roadside_alert_t *p_rsa;
    vam_rsa_evt_info_t param;
        
    if (datalen < sizeof(rcp_msg_roadside_alert_t)){
        return -1;
    }

    p_rsa = (rcp_msg_roadside_alert_t *)databuf;

    param.dsecond  = cv_ntohs(p_rsa->time_stamp);
    param.rsa_mask = decode_itiscode(p_rsa->typeEvent, p_rsa->description);
    param.pos.longitude = decode_longitude(p_rsa->position.lon);
    param.pos.latitude = decode_latitude(p_rsa->position.lat);

    if(p_vam->evt_handler[VAM_EVT_RSA_UPDATE] != NULL)
    {
        (p_vam->evt_handler[VAM_EVT_RSA_UPDATE])(&param);
    }

    return 0;
}


/*****************************************************************************
 @funcname: vam_rcp_recv
 @brief   : RCP receive data frame from network layer
 @param   : wnet_rxinfo_t *rxinfo  
 @param   : uint8_t *databuf      
 @param   : uint32_t datalen      
 @return  : 
*****************************************************************************/
int vam_rcp_recv(wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{
    vam_envar_t *p_vam = &p_cms_envar->vam;
    //osal_printf("vam_rcp_recv...\r\n");
    //vam_add_event_queue(p_vam, VAM_MSG_RCPRX, datalen, (uint32_t)databuf, rxinfo);
    rcp_parse_msg(p_vam, rxinfo, databuf, datalen);
    return 0;
}


/* Send bsm message to others. */
int rcp_send_bsm(vam_envar_t *p_vam)
{
    int                       result = 0;
    wnet_txbuf_t              *txbuf = NULL;
    bsm_msg_optional_st bsm_optional = { 0 };
    uint16_t               valid_bit = 0;


    /* Get tx buffer from memory. */
    if((txbuf = wnet_get_txbuf()) == NULL) 
    {
        return -1;
    }

    /* Set bsm optional setting. */
    if(p_vam->flag & VAM_FLAG_TX_BSM_ALERT)
    {
        bsm_optional.msg_partii = MSG_OPTIONAL_YES;
        bsm_optional.partii_vehicle_safety_ext = MSG_OPTIONAL_YES;

        bsm_optional.vse_vehicle_event_flags = MSG_OPTIONAL_YES;
    }
    else
    {
        bsm_optional.msg_partii = MSG_OPTIONAL_NO;
    }

    /* Build bsm message. */
    result = bsm_build_msg(&bsm_optional, txbuf->data_ptr, sizeof(txbuf->buffer) - (txbuf->data_ptr - txbuf->buffer), &valid_bit);
    if(result != 0)
    {
        goto EXIT;
    }
    else
    {
 
    }
    
    /* Set tx information structure. */
    memcpy(txbuf->info.dest.dsmp.addr, "\xFF\xFF\xFF\xFF\xFF\xFF", MACADDR_LENGTH);
    txbuf->info.dest.dsmp.aid = 0x00000020;
    txbuf->info.protocol = WNET_TRANS_PROT_DSMP;
    txbuf->info.encryption = WNET_TRANS_ENCRYPT_NONE;
    txbuf->info.prority = WNET_TRANS_RRORITY_NORMAL;
    txbuf->info.timestamp = osal_get_systimestamp();

    /* Send bsm message. */
    result = wnet_send(&(txbuf->info), (uint8_t *)txbuf->data_ptr, (valid_bit+7)/8);

    
EXIT:
    
    wnet_release_txbuf(txbuf);

    return result;
}


int rcp_send_evam(vam_envar_t *p_vam)
{
    int result = 0;
    rcp_msg_emergency_vehicle_alert_t *p_evam;
    vam_stastatus_t *p_local = &p_vam->local;
    wnet_txbuf_t *txbuf;
    wnet_txinfo_t *txinfo;
    vam_stastatus_t current;
	
    txbuf = wnet_get_txbuf();
    if (txbuf == NULL) {
        return -1;
    }

    vam_get_local_current_status(&current);
    p_local = &current;

    p_evam = (rcp_msg_emergency_vehicle_alert_t *)WNET_TXBUF_DATA_PTR(txbuf);

    p_evam->msg_id.hops = p_vam->working_param.evam_hops;
    p_evam->msg_id.id = RCP_MSG_ID_EVAM;
    memcpy(p_evam->temporary_id, p_local->pid, RCP_TEMP_ID_LEN);

    if (p_vam->working_param.evam_hops > 1){
        memcpy(p_evam->forward_id, p_local->pid, RCP_TEMP_ID_LEN);
    }

    p_evam->time_stamp = cv_ntohs(osal_get_systime());
    p_evam->rsa.msg_count = p_vam->tx_evam_msg_cnt++;
    p_evam->rsa.position.lon = encode_longitude(p_local->pos.longitude);
    p_evam->rsa.position.lat = encode_latitude(p_local->pos.latitude);
    p_evam->rsa.position.elev = encode_elevation(p_local->pos.elevation);
    p_evam->rsa.position.heading = encode_angle(p_local->dir);
    p_evam->rsa.position.speed.transmissionState = TRANS_STATE_Forward;
    p_evam->rsa.position.speed.speed = encode_absolute_velocity(p_local->speed);
    //TBD
    p_evam->rsa.typeEvent = encode_itiscode(p_local->alert_mask, p_evam->rsa.description); 
    

    txinfo = WNET_TXBUF_INFO_PTR(txbuf);
//    memset(txinfo, 0, sizeof(wnet_txinfo_t));
    memcpy(txinfo->dest.dsmp.addr, "\xFF\xFF\xFF\xFF\xFF\xFF", MACADDR_LENGTH);
    txinfo->dest.dsmp.aid = 0x00000020;
    txinfo->protocol = WNET_TRANS_PROT_DSMP;
    txinfo->encryption = WNET_TRANS_ENCRYPT_NONE;
    txinfo->prority = WNET_TRANS_RRORITY_EMERGENCY;
    txinfo->timestamp = osal_get_systimestamp();

    result = wnet_send(txinfo, (uint8_t *)p_evam, sizeof(rcp_msg_emergency_vehicle_alert_t));

    wnet_release_txbuf(txbuf);

    return result;
}



int rcp_send_rsa(vam_envar_t *p_vam)
{
    int result = 0;
    rcp_msg_roadside_alert_t *p_rsa;
    vam_stastatus_t *p_local = &p_vam->local;
    
    wnet_txbuf_t *txbuf;
    wnet_txinfo_t *txinfo;

    txbuf = wnet_get_txbuf();
    
    if (txbuf == NULL) {
        osal_printf("get txbuf failed line%d", __LINE__);
        return -1;
    }

    /* The RSU position is fixed */
#if 0
    vam_stastatus_t current;
    vam_get_local_current_status(&current);
    p_local = &current;
#endif

    p_rsa = (rcp_msg_roadside_alert_t *)WNET_TXBUF_DATA_PTR(txbuf);

    p_rsa->msg_id.hops = p_vam->working_param.bsm_hops;
    p_rsa->msg_id.id = RCP_MSG_ID_RSA;
    p_rsa->msg_count = p_vam->tx_rsa_msg_cnt++;
    vam_active_rsa(RSA_TYPE_CURVE);
    p_rsa->typeEvent = encode_itiscode(p_local->alert_mask, p_rsa->description);
    p_rsa->time_stamp = cv_ntohs(osal_get_systime());
#if 0
    p_local->pos.lon = 132.327144*3.1415926/180.0;
    p_local->pos.lat = 40.0*3.1415926/180.0;
#endif
    p_rsa->position.lon = encode_longitude(p_local->pos.longitude);
    p_rsa->position.lat = encode_latitude(p_local->pos.latitude);
    p_rsa->position.elev = encode_elevation(p_local->pos.elevation);
    p_rsa->position.heading = encode_angle(p_local->dir);
    p_rsa->position.speed.speed = encode_absolute_velocity(p_local->speed);

    txinfo = WNET_TXBUF_INFO_PTR(txbuf);
//    memset(txinfo, 0, sizeof(wnet_txinfo_t));
    memcpy(txinfo->dest.dsmp.addr, "\xFF\xFF\xFF\xFF\xFF\xFF", MACADDR_LENGTH);
    txinfo->dest.dsmp.aid = 0x00000020;
    txinfo->protocol = WNET_TRANS_PROT_DSMP;
    txinfo->encryption = WNET_TRANS_ENCRYPT_NONE;
    txinfo->prority = WNET_TRANS_RRORITY_EMERGENCY;
    txinfo->timestamp = osal_get_systimestamp();

    result = wnet_send(txinfo, (uint8_t *)p_rsa, sizeof(rcp_msg_roadside_alert_t));
    if (result) 
    {
        osal_printf("wnet_send failed line%d", __LINE__);
    }
    wnet_release_txbuf(txbuf);
    
    return result;
}


int rcp_parse_msg(vam_envar_t *p_vam, wnet_rxinfo_t *rxinfo, uint8_t *databuf, uint32_t datalen)
{    
    MessageModuleSet_t  *mms_ptr = NULL;
    asn_dec_rval_t   decode_rval = { 0 };
    asn_codec_ctx_t   decode_ctx = { 0 };


    /* Error detection. */
    if (datalen < sizeof(rcp_msg_head_t))
    {
        return -1;
    }

    /* Decode into message module set for message parse. */
    decode_rval = uper_decode(&decode_ctx, &asn_DEF_MessageModuleSet,(void **)&mms_ptr, databuf, datalen, 0, 0);
    if(decode_rval.code != RC_OK)
    {
        printf("\nuper_decode() for MessageModuleSet_t is faild. \n ");
        return -1;
    }
    else
    {
        switch(mms_ptr->dsrcMsgId)
        {
            case RCP_MSG_ID_BSM:  {
                rcp_parse_bsm(p_vam, rxinfo, databuf, datalen);    break;
            } 
            case RCP_MSG_ID_EVAM: {
                rcp_parse_evam(p_vam, rxinfo, databuf, datalen);   break;
            } 
            case RCP_MSG_ID_RSA:  {
                rcp_parse_rsa(p_vam, rxinfo, databuf, datalen);    break;   
            }
            default:              {
                                                                   break; 
            }   
        }
        
        return 0;
    }
}







int rcp_send_forward_msg(wnet_txbuf_t *txbuf)
{
    wnet_txinfo_t *txinfo;
    rcp_msgid_t *p_msgid;
    rcp_msg_basic_safty_t *p_bsm;
    rcp_msg_emergency_vehicle_alert_t *p_evam;

    vam_envar_t *p_vam = &p_cms_envar->vam;
    
    txinfo = WNET_TXBUF_INFO_PTR(txbuf);
    memset(txinfo, 0, sizeof(wnet_txinfo_t));
    memcpy(txinfo->dest.dsmp.addr, "\xFF\xFF\xFF\xFF\xFF\xFF", MACADDR_LENGTH);
    txinfo->dest.dsmp.aid = 0x00000020;
    txinfo->protocol = WNET_TRANS_PROT_DSMP;
    txinfo->encryption = WNET_TRANS_ENCRYPT_NONE;
    txinfo->prority = WNET_TRANS_RRORITY_NORMAL;//WNET_TRANS_RRORITY_EMERGENCY;
    txinfo->timestamp = osal_get_systimestamp();

    /* modify the forward_id of msgdata */
    p_msgid = (rcp_msgid_t *)(WNET_TXBUF_DATA_PTR(txbuf));
    if (RCP_MSG_ID_BSM == p_msgid->id){
        p_bsm = (rcp_msg_basic_safty_t *)WNET_TXBUF_DATA_PTR(txbuf);
        memcpy(p_bsm->forward_id, p_vam->local.pid, RCP_TEMP_ID_LEN);
    }
    else if(RCP_MSG_ID_EVAM == p_msgid->id){
        p_evam = (rcp_msg_emergency_vehicle_alert_t *)WNET_TXBUF_DATA_PTR(txbuf);
        memcpy(p_evam->forward_id, p_vam->local.pid, RCP_TEMP_ID_LEN);    
    }
    else {
        return -1;
    }
    
    return wnet_send(txinfo, WNET_TXBUF_DATA_PTR(txbuf), txbuf->data_len);
}
wnet_txbuf_t *rcp_create_forward_msg(uint8_t left_hops, uint8_t *pdata, uint32_t length)
{
    rcp_msgid_t *p_msg;
    wnet_txbuf_t *txbuf = NULL;

    p_msg = (rcp_msgid_t *)pdata;
    p_msg->hops = left_hops;

    txbuf = wnet_get_txbuf();
    if (!txbuf) {
        return NULL;
    }

    memcpy(WNET_TXBUF_DATA_PTR(txbuf), pdata, length);
    txbuf->data_len = length;
    
    return txbuf;
}







//////////////////////////////////////////////////////////////
//all below just for test
//////////////////////////////////////////////////////////////

void timer_send_rsa_callback(void* parameter)
{
    vam_envar_t *p_vam = (vam_envar_t *)parameter;   
    rcp_send_rsa(p_vam);
}
void test_rsa(int flag)
{
    vam_envar_t *p_vam = &p_cms_envar->vam;
    osal_printf("rsatype = %d , %d\r\n", RSA_TYPE_SPEED_RESTRICTION, RSA_TYPE_MAX);
    OSAL_DBGPRT(OSAL_DEBUG_INFO, "test rsa");
    if(flag && !p_vam->timer_send_rsa){
        vam_stop();  
        p_vam->timer_send_rsa = osal_timer_create("tm_rsa", timer_send_rsa_callback, p_vam, 1000, \
                                        TIMER_INTERVAL|TIMER_STOPPED, TIMER_PRIO_NORMAL);
        osal_timer_start(p_vam->timer_send_rsa);
    }
    else{
        if(p_vam->timer_send_rsa){
            osal_timer_stop(p_vam->timer_send_rsa);
        }
    }   
}


osal_timer_t *timer_test_bsm_rx;
void timer_test_bsm_rx_callback(void* parameter)
{
    vam_stastatus_t sta;
    vam_stastatus_t *p_local;

    rcp_msg_basic_safty_t test_bsm_rx;
    rcp_msg_basic_safty_t *p_bsm;

    vam_envar_t *p_vam = &p_cms_envar->vam;

    p_local = &sta;
    p_local->pos.latitude = 40.0; //39.5427f;
    p_local->pos.longitude = 120.0;//116.2317f;
    p_local->dir = 90.0;//
    p_local->pid[0] = 0x02;
    p_local->pid[1] = 0x04;
    p_local->pid[2] = 0x06;
    p_local->pid[3] = 0x08;


    p_bsm = (rcp_msg_basic_safty_t *)&test_bsm_rx;   
    /* construct a fake message */
    p_bsm->header.msg_id.hops = 1;
    p_bsm->header.msg_id.id = RCP_MSG_ID_BSM;
    p_bsm->header.msg_count = 0;
    memcpy(p_bsm->header.temporary_id, p_local->pid, RCP_TEMP_ID_LEN);
    p_bsm->dsecond = osal_get_systemtime();

    p_bsm->position.lon = encode_longitude(p_local->pos.longitude);
    p_bsm->position.lat = encode_latitude(p_local->pos.latitude);
    p_bsm->position.elev = encode_elevation(p_local->pos.elevation);
    p_bsm->position.accu.semi_major = encode_semimajor_axis_accuracy(p_local->pos_accuracy.semi_major_accu);
    p_bsm->position.accu.semi_major_orientation = encode_semimajor_axis_orientation(p_local->pos_accuracy.semi_major_orientation);
    p_bsm->position.accu.semi_minor = encode_semiminor_axis_accuracy(p_local->pos_accuracy.semi_minor_accu);

    p_bsm->motion.transmission_state = p_local->transmission_state;
    p_bsm->motion.heading = encode_angle(p_local->dir);
    p_bsm->motion.speed = encode_absolute_velocity(p_local->speed);
    p_bsm->motion.steering_wheel_angle = encode_steer_wheel_angle(p_local->steer_wheel_angle);
    p_bsm->motion.acce.lon = encode_acceleration(p_local->acce_set.longitudinal);
    p_bsm->motion.acce.lat = encode_acceleration(p_local->acce_set.lateral);
    p_bsm->motion.acce.vert = encode_vertical_acceleration(p_local->acce_set.vertical);
    p_bsm->motion.acce.yaw = encode_yawrate(p_local->acce_set.yaw_rate);
    
    encode_brake_sytem_status(&p_local->brake_stat, &p_bsm->brakes);

    p_bsm->size.length = encode_vehicle_length(p_local->vec_size.vec_length);
    p_bsm->size.width = encode_vehicle_width(p_local->vec_size.vec_width);

    rcp_parse_bsm(p_vam, NULL, (uint8_t *)p_bsm, (sizeof(rcp_msg_basic_safty_t) - sizeof(vehicle_safety_ext_t)));
}

void tb1(void)
{
    timer_test_bsm_rx = osal_timer_create("tm-tb",timer_test_bsm_rx_callback,NULL,\
        2400, TIMER_INTERVAL|TIMER_STOPPED, TIMER_PRIO_NORMAL);

    osal_timer_start(timer_test_bsm_rx);
}

void stop_tb1(void)
{
	osal_timer_stop(timer_test_bsm_rx);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(test_rsa, test sending rsa);
FINSH_FUNCTION_EXPORT(tb1, test bsm receiving);
FINSH_FUNCTION_EXPORT(stop_tb1, stop test bsm receiving);
#endif
