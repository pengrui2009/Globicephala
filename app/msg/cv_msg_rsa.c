/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_msg_rsa.c
 @brief  : MSG_RoadSideAlert(RSA) routine set.
 @author : wangxianwen
 @history:
           2016-09-12    wangxianwen    Created file
           ...
******************************************************************************/
#include "cv_msg_rsa.h"

#include "DSRCmsgIDEnum.h"
#include "RoadSideAlert.h"
#include "J2735.h"


#if 0
uint16_t itiscode[RSA_TYPE_MAX+1] = 
{
#undef xx
#define xx(SEQ, TYPE, ITISCODE) (ITISCODE),
    VAM_RSA_TYPE_2_ITISCODE
};
#endif

/* Key for rsa tx message structure and UPER data printf. */
static uint8_t KeyPrintfRsaTxStruct = 1;
static uint8_t KeyPrintfRsaTxData = 1;

/* Key for rsa rx message structure and UPER data printf. */
static uint8_t KeyPrintfRsaRxStruct = 1;
static uint8_t KeyPrintfRsaRxData = 1;

#if 0
static uint16_t encode_itiscode(uint16_t rsa_mask, itis_codes_t *p_des)
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


static void itiscode_2_rsa_mask(uint16_t type, uint16_t *rsa_mask)
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


uint16_t decode_itiscode(uint16_t typeEvent, uint16_t *p_des)
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

#endif

/* Printf rsa message structure data. */
static void rsa_printf_msgstruct(const char *str, RoadSideAlert_t *rsa_ptr)
{
    RsaBody_t *body_ptr = &(rsa_ptr->rsaBody);


    /* User specific strings. */
    printf(str);
    
    /* Message tag. */
    printf("\nRSA message structure------------------->\n");
        
    /* Msg id. */
    printf("rsa_ptr->dsrcMsgId = 0x%lx. \n", rsa_ptr->dsrcMsgId);
    
    /* Message count. */
    printf("body_ptr->msgCnt = 0x%lx. \n", body_ptr->msgCnt);

    /* MinuteOfTheYear. */
    if(body_ptr->timeStamp != NULL)
    {
        printf("body_ptr->timeStamp = 0x%lx. \n", *(body_ptr->timeStamp));
    }

    /* continued........... */

}


/* Printf rsa message PER data. */
static void rsa_printf_msgdata(const char *str, uint8_t *buffer_ptr, uint16_t valid_bit)
{
    uint16_t index = 0;


    /* User specific strings. */
    printf(str);
    
    printf("\nRSA message PER data %d bit------------->\n", valid_bit);
    
    for(index = 0; index < (valid_bit+7)/8; index++)
    {
        printf("0x%02x ", buffer_ptr[index]);
        if((index+1) % 10 == 0)
        {
            printf("\n");
        }
    }
}


/* Free rsa description element main routine. */
static void rsa_free_description_element(ITIScodes_t *element_ptr)
{
    /* Free the description element. */
    if(element_ptr != NULL)
    {
        free(element_ptr);
    }   
}


/* Free rsa decription structure. */
static int rsa_free_decription_structure(RoadSideAlert_t *rsa_ptr)
{
    if(rsa_ptr != NULL)
    {
        /* Decription structure domain. */
        if(rsa_ptr->rsaBody.description != NULL)
        {
            /* Free the contents of the sequence,do not free the sequence itself. */
            asn_sequence_empty(&(rsa_ptr->rsaBody.description->list));
            
            free(rsa_ptr->rsaBody.description);
            rsa_ptr->rsaBody.description = NULL;
        }
        
        return 0;
    }
    else
    {
        return -1;
    }
}


/* Allocate rsa description structure. */
static int rsa_allocate_description_structure(RoadSideAlert_t *rsa_ptr, rsa_msg_st_ptr msg_ptr) 
{
    ITIScodes_t *element_ptr = NULL;
    uint8_t            index = 0;

    
    /* Allocate decription structure pointer. */
    rsa_ptr->rsaBody.description = calloc(1, sizeof(*(rsa_ptr->rsaBody.description)));
    if(rsa_ptr->rsaBody.description == NULL)
    {
        return -1;
    }
    
    /* Set description element free routine. */
    rsa_ptr->rsaBody.description->list.free = rsa_free_description_element;

    /* Allocate description element. */
    if(msg_ptr->decription_num <= (sizeof(msg_ptr->description_ele) / sizeof(msg_ptr->description_ele[0])))
    {
        for(index = 0; index < msg_ptr->decription_num; index ++)
        {
            element_ptr = calloc(1, sizeof(*element_ptr));
            if(element_ptr != NULL)
            {
                *element_ptr = msg_ptr->description_ele[index];
                
                /* Add description element to structure pointer. */
                if(asn_sequence_add(&(rsa_ptr->rsaBody.description->list), element_ptr) != 0)
                {
                    goto ERR_EXIT;
                } 
            }
            else
            {
                goto ERR_EXIT;
            }
        }
    }
    else
    {
        goto ERR_EXIT;
    }

    return 0;

ERR_EXIT:
    
   rsa_free_decription_structure(rsa_ptr);
    
   return -1;
}


/* Parse rsa message into vam status structure. */
static int rsa_parse_msg(RoadSideAlert_t *rsa_ptr, rsa_msg_st_ptr msg_ptr)
{

    /* Message count. */
    msg_ptr->msgCnt = rsa_ptr->rsaBody.msgCnt;

    /* Time stamp. */
    if(rsa_ptr->rsaBody.timeStamp != NULL)
    {
        msg_ptr->opt.timeStamp = MSG_OPTIONAL_YES;
        msg_ptr->timeStamp = *(rsa_ptr->rsaBody.timeStamp);
    }


    return 0; 
}


/* Free rsa message. */
static int rsa_free_msg(RoadSideAlert_t *rsa_ptr)
{
    /* Error detection. */
    if(rsa_ptr == NULL)
    {
        return -1;
    }
    else
    {
        if(rsa_ptr->rsaBody.timeStamp != NULL)
        {
            free(rsa_ptr->rsaBody.timeStamp);
        }
        if(rsa_ptr->rsaBody.description != NULL)
        {
            rsa_free_decription_structure(rsa_ptr);
        }
        if(rsa_ptr->rsaBody.priority != NULL)
        {
            if(rsa_ptr->rsaBody.priority->buf != NULL)
            {
                free(rsa_ptr->rsaBody.priority->buf);
            }
            free(rsa_ptr->rsaBody.priority);
        }  
        if(rsa_ptr->rsaBody.heading != NULL)
        {
            if(rsa_ptr->rsaBody.heading->buf != NULL)
            {
                free(rsa_ptr->rsaBody.heading->buf);
            }
            free(rsa_ptr->rsaBody.heading);
        }
        if(rsa_ptr->rsaBody.extent != NULL)
        {
            free(rsa_ptr->rsaBody.extent);
        } 
        if(rsa_ptr->rsaBody.position != NULL)
        {
            DF_FullPositionVector_free(rsa_ptr->rsaBody.position);   
        }
        if(rsa_ptr->rsaBody.furtherInfoID != NULL)
        {
            if(rsa_ptr->rsaBody.furtherInfoID->buf != NULL)
            {
                free(rsa_ptr->rsaBody.furtherInfoID->buf);
            }
            free(rsa_ptr->rsaBody.furtherInfoID);
        }
            
        /* Free message itself domain. */
        free(rsa_ptr);
    }
   
    return 0;
}


/* Allocate rsa message. */
static int rsa_allocate_msg(RoadSideAlert_t **rsa_ptr_ptr, rsa_msg_st_ptr msg_ptr) 
{
    RoadSideAlert_t    *rsa_ptr = NULL;


    /* Allocate the rsa message. */
    rsa_ptr = calloc(1, sizeof(*rsa_ptr));
    if(rsa_ptr == NULL)
    {
        goto ERR_EXIT;
    }

    /* Mesage id. */
    rsa_ptr->dsrcMsgId = DSRCmsgIDEnum_roadSideAlert;

    /* Message count. */
    if(127 < msg_ptr->msgCnt)
    {
        goto ERR_EXIT;
    }
    rsa_ptr->rsaBody.msgCnt = msg_ptr->msgCnt;

    /* Time stamp. */
    if(msg_ptr->opt.timeStamp == MSG_OPTIONAL_YES)
    {
        rsa_ptr->rsaBody.timeStamp = calloc(1, sizeof(*(rsa_ptr->rsaBody.timeStamp)));
        if(rsa_ptr->rsaBody.timeStamp != NULL)
        {
            * rsa_ptr->rsaBody.timeStamp = msg_ptr->timeStamp;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        rsa_ptr->rsaBody.timeStamp = NULL;
    }

    /* Type event. */
    rsa_ptr->rsaBody.typeEvent = 0;//encode_itiscode(vam_local.alert_mask, rsa_ptr->rsaBody.description);

    /* Description. */
    if(msg_ptr->opt.description == MSG_OPTIONAL_YES)
    {
        if(rsa_allocate_description_structure(rsa_ptr, msg_ptr) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        rsa_ptr->rsaBody.description = NULL;
    }

    /* Priority. */
    if(msg_ptr->opt.priority == MSG_OPTIONAL_YES)
    {
        rsa_ptr->rsaBody.priority = calloc(1, sizeof(Priority_t));
        if(rsa_ptr->rsaBody.priority != NULL)
        {
            /* Allocate room for priority data. */
            rsa_ptr->rsaBody.priority->size = 1;
            rsa_ptr->rsaBody.priority->buf = calloc(1, rsa_ptr->rsaBody.priority->size);
            if(rsa_ptr->rsaBody.priority->buf != NULL)
            {
                rsa_ptr->rsaBody.priority->buf[0] = 0x00;
            }
            else
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        rsa_ptr->rsaBody.priority = NULL;
    }

    /* Heading. */
    if(msg_ptr->opt.heading == MSG_OPTIONAL_YES)
    {
        rsa_ptr->rsaBody.heading = calloc(1, sizeof(HeadingSlice_t));
        if(rsa_ptr->rsaBody.heading != NULL)
        {
            /* Allocate room for heading bit. */
            rsa_ptr->rsaBody.heading->size = 2;
            rsa_ptr->rsaBody.heading->bits_unused = 0;
            
            rsa_ptr->rsaBody.heading->buf = calloc(1, rsa_ptr->rsaBody.heading->size);
            if(rsa_ptr->rsaBody.heading->buf != NULL)
            {
                rsa_ptr->rsaBody.heading->buf[0] = 0x00;
                rsa_ptr->rsaBody.heading->buf[1] = 0x01;
            }
            else
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        rsa_ptr->rsaBody.heading = NULL;
    }

    /* Extend. */
    if(msg_ptr->opt.extent == MSG_OPTIONAL_YES)
    {
        rsa_ptr->rsaBody.extent = calloc(1, sizeof(Extent_t));
        if(rsa_ptr->rsaBody.extent != NULL)
        {
            * rsa_ptr->rsaBody.extent = Extent_useFor50meters;
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        rsa_ptr->rsaBody.heading = NULL;
    }

    /* Position. */
    if(msg_ptr->opt.position == MSG_OPTIONAL_YES)
    {
        if((DF_FullPositionVector_allocate(&(rsa_ptr->rsaBody.position), &(msg_ptr->position))) != 0)
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        rsa_ptr->rsaBody.position = NULL;
    }
    
    /* Further info id. */
    if(msg_ptr->opt.furtherInforID == MSG_OPTIONAL_YES)
    {
        rsa_ptr->rsaBody.furtherInfoID = calloc(1, sizeof(FurtherInfoID_t));
        if(rsa_ptr->rsaBody.furtherInfoID != NULL)
        {
            /* Allocate room for further info id data. */
            rsa_ptr->rsaBody.furtherInfoID->size = 2;
            rsa_ptr->rsaBody.furtherInfoID->buf = calloc(1, rsa_ptr->rsaBody.furtherInfoID->size);
            if(rsa_ptr->rsaBody.furtherInfoID->buf != NULL)
            {
                rsa_ptr->rsaBody.furtherInfoID->buf[0] = 0x00;
                rsa_ptr->rsaBody.furtherInfoID->buf[1] = 0x01;
            }
            else
            {
                goto ERR_EXIT;
            }
        }
        else
        {
            goto ERR_EXIT;
        }
    }
    else
    {
        rsa_ptr->rsaBody.furtherInfoID = NULL;
    }

    /* Regional. */


    /* Set rsa pointer. */
    *rsa_ptr_ptr = rsa_ptr;
    
    return 0;


ERR_EXIT:

    rsa_free_msg(rsa_ptr);
    *rsa_ptr_ptr = NULL;
    
    return -1;  
 
}


/* Build rsa message into the specific buffer. */
int rsa_build_msg(rsa_msg_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *valid_bit_ptr)
{
    int                       result = 0;
    RoadSideAlert_t         *rsa_ptr = NULL;
    asn_enc_rval_t       encode_rval = { 0 };


    /* Error detection. */
    if( (msg_ptr == NULL) || (buffer_ptr == NULL) || (buffer_size == 0) || (valid_bit_ptr == NULL) )
    {
        printf(" rsa_build_msg() check error. \n ");
        result = -1;
    }
    else
    {
        /* Allocate rsa message. */
        result = rsa_allocate_msg(&rsa_ptr, msg_ptr);
        if(result != 0)
        {
            printf(" rsa_allocate_msg() is faild. \n ");
            result = -1;
        }
        else
        {
            /* Encode message to UPER format. */
            encode_rval = uper_encode_to_buffer(&asn_DEF_RoadSideAlert, rsa_ptr, buffer_ptr, buffer_size);
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

            /* Printf the rsa message structure when key on. */
            if(KeyPrintfRsaTxStruct != 0)
            {
                rsa_printf_msgstruct("\nRSA Tx-------------------->", rsa_ptr);
            }       
            
            /* Printf rsa messge UPER data when key on. */
            if(KeyPrintfRsaTxData != 0)
            {
                rsa_printf_msgdata("\nRSA Tx-------------------->", buffer_ptr, *valid_bit_ptr);
            } 

            /* Free message. */
            rsa_free_msg(rsa_ptr);
        }
    }
  
    return result; 
}


/* Parse rsa message from the specific buffer. */
int rsa_analyse_msg(rsa_msg_st_ptr msg_ptr, uint8_t *buffer_ptr, uint16_t buffer_size)
{
    int                    result = 0;
    RoadSideAlert_t      *rsa_ptr = NULL;
    
    asn_dec_rval_t    decode_rval = { 0 };
    asn_codec_ctx_t    decode_ctx = { 0 };


    /* Error detection. */
    if((msg_ptr == NULL) || (buffer_ptr == NULL) || (buffer_size == 0))
    {
        printf(" rsa_analyse_msg() check error. \n ");
        result = -1;
    }
    else
    {
        /* Dncode message from UPER format. */
        decode_rval = uper_decode(&decode_ctx, &asn_DEF_RoadSideAlert,(void **)&rsa_ptr, buffer_ptr, buffer_size, 0, 0);
        if(decode_rval.code != RC_OK)
        {
            printf(" uper_decode() is faild. \n ");
            result = -1;
        }
        else
        {
            /* Parse the rsa message. */
            result = rsa_parse_msg(rsa_ptr, msg_ptr); 
        }

        /* Printf the rsa message structure when key on. */
        if(KeyPrintfRsaRxStruct != 0)
        {
            rsa_printf_msgstruct("\nBSM Rx-------------------->", rsa_ptr);
        }       

        /* Printf rsa messge UPER data when key on. */
        if(KeyPrintfRsaRxData != 0)
        {
            rsa_printf_msgdata("\nBSM Rx-------------------->", buffer_ptr, buffer_size * 8);
        }
    }

    return result; 
}


