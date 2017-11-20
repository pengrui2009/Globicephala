/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_msg_wsmp.h
 @brief  : Message WSMP(DSMP + SEC + LLC) routine set.
 @author : wangxianwen
 @history:
           2017-06-12    wangxianwen    Created file
           ...
******************************************************************************/

#ifndef __CV_MSG_WSMP_H__
#define __CV_MSG_WSMP_H__

#include <stdio.h>
#include <stdint.h>
#include "osal_cmn.h"
#include "error.h"


/* Wsmp build buffer and analyse buffer header room. */
#define WSMP_BUILD_BUFFER_HEADER    (100)
#define WSMP_ANALYSE_BUFFER_HEADER  (0)


/* WSMP buffer structure. */
typedef    struct _wsmp_buf_st
{
    /* Buffer start address for user data. */
    uint8_t  * buffer_ptr;

    /* Buffer size that user can use. */
    uint16_t  buffer_size;

    /* Actual data length that user use. */
    uint16_t     data_len; 
    
}wsmp_buf_st, * wsmp_buf_st_ptr;

#define WSMP_BUF_ST_LEN    (sizeof(wsmp_buf_st))


/* DSMP aid. */
typedef enum _dsmp_aid_en 
{
    WAVE_AID_system                        = 0,
    WAVE_AID_automatic_fee_collection      = 1,
    WAVE_AID_freight_fleet_management      = 2,
    WAVE_AID_public_transport              = 3,
    WAVE_AID_traffic_traveler_information  = 4,
    WAVE_AID_traffic_control               = 5,
    WAVE_AID_parking_management            = 6,
    WAVE_AID_geographic_road_database      = 7,
    WAVE_AID_medium_range_preinformation   = 8,
    WAVE_AID_man_machine_interface         = 9,
    WAVE_AID_intersystem_interface         = 10,
    WAVE_AID_automatic_vehicle_identification = 11,
    WAVE_AID_emergency_warning             = 12,
    WAVE_AID_private                       = 13,
    WAVE_AID_multi_purpose_payment         = 14,
    WAVE_AID_dsrc_resource_manager         = 15,
    WAVE_AID_after_theft_systems           = 16,
    WAVE_AID_cruise_assist_highway_system  = 17,
    WAVE_AID_multi_purpose_information_system = 18, 
    WAVE_AID_public_safety                 = 19,
    WAVE_AID_vehicle_safety                = 20,
    WAVE_AID_general_purpose_internet_access = 21,
    WAVE_AID_onboard_diagnostics           = 22,
    WAVE_AID_security_manager              = 23,
    WAVE_AID_signed_WSA                    = 24
    
} dsmp_aid_en, *dsmp_aid_en_ptr;

#define DSMP_AID_EN_LEN    (sizeof(dsmp_aid_en))


/* DSMP element_id. */
typedef enum _dsmp_element_id_en 
{
    WAVE_ELEMENT_ID_RSV          = 0,
    WAVE_ELEMENT_ID_Service_info = 1,
    WAVE_ELEMENT_ID_channel_info = 2,
    WAVE_ELEMENT_ID_WRA          = 3,    
    WAVE_ELEMENT_ID_Trans_Power  = 4,
    WAVE_ELEMENT_ID_2D           = 5,
    WAVE_ELEMENT_ID_3D           = 6,
    WAVE_ELEMENT_ID_Advertiser_id = 7,
    WAVE_ELEMENT_ID_PSC          = 8,
    WAVE_ELEMENT_ID_IPv6_Addr    = 9,

    WAVE_ELEMENT_ID_WSM          = 128,
    WAVE_ELEMENT_ID_WSMP_S       = 129,
    WAVE_ELEMENT_ID_WSMP_I       = 130
    
}dsmp_element_id_en, *dsmp_element_id_en_ptr;

#define DSMP_ELEMENT_ID_EN_LEN    (sizeof(dsmp_element_id_en))


/* SEC security type. */
typedef enum _sec_security_type_en 
{
    SEC_SECURITY_TYPE_NONE    = 0x00,
    SEC_SECURITY_TYPE_DES     = 0x01
    
    
}sec_security_type_en, *sec_security_type_en_ptr;

#define SEC_SECURITY_TYPE_EN_LEN    (sizeof(sec_security_type_en))


/* LLC ether type. */
typedef enum _llc_ether_type_en 
{
    LLC_ETHERTYPE_IPv4    = 0x0800,
    LLC_ETHERTYPE_IPv6    = 0x86DD,
    LLC_ETHERTYPE_WSMPv1  = 0x88DC,
    LLC_ETHERTYPE_DSMPv1  = 0x88E2,
    LLC_ETHERTYPE_DSAv1   = 0x88E4
    
}llc_ether_type_en, *llc_ether_type_en_ptr;

#define LLC_ETHER_TYPE_EN_LEN    (sizeof(llc_ether_type_en))


/* WSMP frame infor structure. */
typedef struct _wsmp_infor_st
{   
    /* Application ID for dsmp header. */
    dsmp_aid_en                   dsmp_aid;

    /* Element ID for dsmp header. */
    dsmp_element_id_en     dsmp_element_id;

    /* Length for dsmp header. */
    uint16_t                   dsmp_length;

    /* Sec security type. */
    sec_security_type_en sec_security_type;

    /* Ether type for llc header. */
    llc_ether_type_en       llc_ether_type;
        
}wsmp_infor_st, * wsmp_infor_st_ptr;

#define WSMP_INFOR_ST_LEN    (sizeof(wsmp_infor_st))


/* Save all the compiler settings and off the optimization. */
#pragma pack(push)
#pragma pack(1)


/* DSMP frame header,same as WSMP frame defined by IEEE1609.3. */
typedef struct _dsmp_header_st 
{
    /* Dsmp version. */
    uint8_t    version;

    /* Application ID. */
    uint32_t       aid;

    /* Element ID. */
    uint8_t element_id;

    /* Data length. */
    uint16_t    length;
    
}dsmp_header_st, *dsmp_header_st_ptr;

#define DSMP_HEADER_ST_LEN    (sizeof(dsmp_header_st))

/* DSMP version. */
#define DSMP_VERSION_V1    1


/* Security header,defined from 1609.2 frame header */
typedef struct _sec_header_st 
{
    /* Sec version. */
    uint8_t       version;

    /* Security type. */
    uint8_t security_type;

    /* Reseved. */
    uint8_t   reserved[2];
    
}sec_header_st, *sec_header_st_ptr;

#define SEC_HEADER_ST_LEN    (sizeof(sec_header_st))

/* SEC version. */
#define SEC_VERSION    2


/* Security DES header structure. */
typedef struct _sec_des_header_st 
{
    /* Dummy data. */
    uint8_t enc_type;  
    uint8_t   key[3];
    
}sec_des_header_st, *sec_des_header_st_ptr;

#define SEC_DES_HEADER_ST_LEN    (sizeof(sec_des_header_st))


/* LLC and SNAP headers structure. */
typedef struct _llc_header_st 
{   
    /* LLC header. */
    uint8_t        dsap;
    uint8_t        ssap;
    uint8_t     control;

    /* SNAP header. */
    uint8_t    ouiid[3];
    uint16_t ether_type;
    
}llc_header_st, *llc_header_st_ptr;

#define LLC_HEADER_ST_LEN    (sizeof(llc_header_st))

/* Llc DSAP and SSAP. */
#define LLC_DSAP       0xAA
#define LLC_SSAP       0xAA

/* Llc control. */
#define LLC_CONTROL    0x03


/* restore all compiler settings in stacks. */
#pragma pack(pop)


/* Build wsmp message into the specific buffer. */
extern int wsmp_build_msg(wsmp_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *data_len_ptr);

/*FUNCTION*****************************************************************
* 
* Returned : ERR_OK when analyse succeed Or negative error code when failure.
* Comments : Analyse wsmp message from the specific buffer.
* Caution  : data_len_ptr return the rest valid data length in buffer except to wsmp 
*            mesage head.
*
*END*********************************************************************/
extern int wsmp_analyse_msg(wsmp_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t *data_len_ptr);


#endif

