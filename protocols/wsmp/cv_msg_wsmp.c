/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_msg_bsm.c
 @brief  : Message WSMP(DSMP + SEC + LLC) routine set.
 @author : wangxianwen
 @history:
           2017-06-12    wangxianwen    Created file
           ...
******************************************************************************/
#include "cv_msg_wsmp.h"
#include "string.h"


/* Build dsmp message into the specific buffer. */
static int wsmp_build_dsmp(wsmp_infor_st_ptr infor_ptr, wsmp_buf_st_ptr buffer_ptr)
{   
    int                     ret = ERR_OK;
    dsmp_header_st_ptr dsmp_ptr = NULL;
    uint32_t           data_u32 = 0;


    /* Add dsmp header only when buffer room valid. */
    if((buffer_ptr->data_len + DSMP_HEADER_ST_LEN) <= buffer_ptr->buffer_size)
    {
        /* Move room for dsmp header. */
    	buffer_ptr->buffer_ptr -= DSMP_HEADER_ST_LEN;
        dsmp_ptr = (dsmp_header_st_ptr)buffer_ptr->buffer_ptr;
    	memset(dsmp_ptr, 0x00, DSMP_HEADER_ST_LEN);

        /* Set dsmp header. */
    	dsmp_ptr->version = DSMP_VERSION_V1;
        
        data_u32 = infor_ptr->dsmp_aid;
    	dsmp_ptr->aid = cv_ntohl(data_u32);
        
    	dsmp_ptr->element_id = infor_ptr->dsmp_element_id;
    	dsmp_ptr->length = cv_ntohs(buffer_ptr->data_len);

        /* Set data length. */
        buffer_ptr->data_len += DSMP_HEADER_ST_LEN;

        ret = ERR_OK;
    }
    else
    {
        ret = ERR_NOMEM;
    }

    return ret;
}


/* Analyse bsmp message from the specific buffer. */
static int wsmp_analyse_dsmp(wsmp_infor_st_ptr infor_ptr, wsmp_buf_st_ptr buffer_ptr)
{
    int                     ret = ERR_OK;
	dsmp_header_st_ptr dsmp_ptr = NULL;


    /* Error detection. */
    if(buffer_ptr->data_len < DSMP_HEADER_ST_LEN)
    {
    	ret = ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Analyse dsmp header. */
    dsmp_ptr = (dsmp_header_st_ptr)buffer_ptr->buffer_ptr;
    infor_ptr->dsmp_aid = cv_ntohl(dsmp_ptr->aid); 
    infor_ptr->dsmp_element_id = dsmp_ptr->element_id;
    infor_ptr->dsmp_length = cv_ntohs(dsmp_ptr->length);

    /* Move over dsmp header. */
    buffer_ptr->buffer_ptr += DSMP_HEADER_ST_LEN;
    buffer_ptr->data_len -= DSMP_HEADER_ST_LEN;

    /* Check whether the dsmp's hold length less than buffer reset data length . */
    if(infor_ptr->dsmp_length <= buffer_ptr->data_len)
    {
        ret = ERR_OK;
    }
    else
    {
        ret = ERR_NORECORD;
    }

ERR_EXIT:

    return ret;
}


/* Build sec message into the specific buffer. */
static int wsmp_build_sec(wsmp_infor_st_ptr infor_ptr, wsmp_buf_st_ptr buffer_ptr)
{
    int                       ret = ERR_OK;
    sec_header_st_ptr     sec_ptr = NULL;
    sec_des_header_st_ptr des_ptr = NULL;


	/* Set sec header only when set exit. */
    switch(infor_ptr->sec_security_type)
	{
	    case SEC_SECURITY_TYPE_NONE:
        {
            break;    
        }
        case SEC_SECURITY_TYPE_DES:
        {
            /* Add sec des header only when buffer room valid. */
            if((buffer_ptr->data_len + SEC_DES_HEADER_ST_LEN) <= buffer_ptr->buffer_size)
            {
                /* Move room for sec_des header. */
                buffer_ptr->buffer_ptr -= SEC_DES_HEADER_ST_LEN;
                des_ptr = (sec_des_header_st_ptr)buffer_ptr->buffer_ptr;
                memset(des_ptr, 0x00, SEC_DES_HEADER_ST_LEN);

            	/* Set sec_dex header. */
            	des_ptr->enc_type = 0;
            	des_ptr->key[0] = 0;
                des_ptr->key[1] = 0;
                des_ptr->key[2] = 0;
                
                /* Set data length. */
            	buffer_ptr->data_len += SEC_DES_HEADER_ST_LEN;
            }
            else
            {
                ret = ERR_NOMEM;
                goto ERR_EXIT;
            }

            break;    
        }
        default: 
        {
            ret = ERR_NOTSUPPORT;
            goto ERR_EXIT;
            
            break;    
        }
	}

    /* Add sec header only when buffer room valid. */
    if((buffer_ptr->data_len + SEC_HEADER_ST_LEN) <= buffer_ptr->buffer_size)
    {
        /* Move room for sec header. */
        buffer_ptr->buffer_ptr -= SEC_HEADER_ST_LEN;
        sec_ptr = (sec_header_st_ptr)buffer_ptr->buffer_ptr;
        memset(sec_ptr, 0x00, SEC_HEADER_ST_LEN);

    	/* Set sec header. */
    	sec_ptr->version = SEC_VERSION;
    	sec_ptr->security_type = infor_ptr->sec_security_type;

        /* Set data length. */
    	buffer_ptr->data_len += SEC_HEADER_ST_LEN;

        /* return ok; */
        ret = ERR_OK;
    }
    else
    {
        ret = ERR_NOMEM;
    }

ERR_EXIT:

    return ret;
}


/* Analyse sec message from the specific buffer. */
static int wsmp_analyse_sec(wsmp_infor_st_ptr infor_ptr, wsmp_buf_st_ptr buffer_ptr)
{
    int                   ret = ERR_OK;
	sec_header_st_ptr sec_ptr = NULL;


    /* Error detection. */
    if(buffer_ptr->data_len < SEC_HEADER_ST_LEN)
    {
    	ret = ERR_NOMEM;
        goto ERR_EXIT;
    }
    
	/* Analyse sec header. */
    sec_ptr = (sec_header_st_ptr)buffer_ptr->buffer_ptr;
	switch(sec_ptr->security_type)
	{
    	case SEC_SECURITY_TYPE_NONE:
        {     
            infor_ptr->sec_security_type = sec_ptr->security_type;

            /* Move over sec header. */
            buffer_ptr->buffer_ptr += SEC_HEADER_ST_LEN;
            buffer_ptr->data_len -= SEC_HEADER_ST_LEN;
            
            ret = ERR_OK;
            break;
        }
        case SEC_SECURITY_TYPE_DES:
        {
            infor_ptr->sec_security_type = sec_ptr->security_type;

            /* Move over sec header. */
            buffer_ptr->buffer_ptr += SEC_HEADER_ST_LEN;
            buffer_ptr->data_len -= SEC_HEADER_ST_LEN;

            /* Error detection. */
            if(buffer_ptr->data_len < SEC_DES_HEADER_ST_LEN)
            {
            	ret = ERR_NOMEM;
            }
            else
            {
                /* Move over sec_des header. */
                buffer_ptr->buffer_ptr += SEC_DES_HEADER_ST_LEN;
                buffer_ptr->data_len -= SEC_DES_HEADER_ST_LEN;

                ret = ERR_OK;
            }
            break;
        }
	    default:
        {
            ret = ERR_NOTSUPPORT;
            break;
        }
	}

ERR_EXIT:

    return ret;
}


/* Build llc message into the specific buffer. */
static int wsmp_build_llc(wsmp_infor_st_ptr infor_ptr, wsmp_buf_st_ptr buffer_ptr)
{
    int                   ret = ERR_OK;
    llc_header_st_ptr llc_ptr = NULL;
    uint16_t         data_u16 = 0;

    
	switch(infor_ptr->llc_ether_type)
	{
    	case LLC_ETHERTYPE_DSMPv1:
        {
            /* Add llc header only when buffer room valid. */
            if((buffer_ptr->data_len + LLC_HEADER_ST_LEN) <= buffer_ptr->buffer_size)
            {
                /* Move room for llc header. */
            	buffer_ptr->buffer_ptr -= LLC_HEADER_ST_LEN;
                llc_ptr = (llc_header_st_ptr)buffer_ptr->buffer_ptr;
            	memset(llc_ptr, 0x00, LLC_HEADER_ST_LEN);
                
                /* Set llc header. */
            	llc_ptr->dsap = LLC_DSAP;
            	llc_ptr->ssap = LLC_SSAP;
                llc_ptr->control = LLC_CONTROL;

                data_u16 = infor_ptr->llc_ether_type;
                llc_ptr->ether_type = cv_ntohs(data_u16);

                /* Set data length. */
                buffer_ptr->data_len += LLC_HEADER_ST_LEN;

                ret = ERR_OK;
            }
            else
            {
                ret = ERR_NOMEM;
            }

            break;
        }
	    default:
        {
            ret = ERR_NOTSUPPORT;
            break;
        }
	}

    return ret;
}


/* Analyse llc message from the specific buffer. */
static int wsmp_analyse_llc(wsmp_infor_st_ptr infor_ptr, wsmp_buf_st_ptr buffer_ptr)
{
    int                   ret = ERR_OK;
	llc_header_st_ptr llc_ptr = NULL;


    /* Error detection. */
    if(buffer_ptr->data_len < LLC_HEADER_ST_LEN)
    {
    	ret = ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Analyse llc header. */
    llc_ptr = (llc_header_st_ptr)buffer_ptr->buffer_ptr;
	switch(cv_ntohs(llc_ptr->ether_type))
	{
    	case LLC_ETHERTYPE_DSMPv1:
        {
            infor_ptr->llc_ether_type = LLC_ETHERTYPE_DSMPv1;
            
            ret = ERR_OK;
            break;
        }
	    default:
        {
            ret = ERR_NOTSUPPORT;
            break;
        }
	}

    /* Move over llc header. */
    buffer_ptr->buffer_ptr += LLC_HEADER_ST_LEN;
    buffer_ptr->data_len -= LLC_HEADER_ST_LEN;

ERR_EXIT:

    return ret;
}


/* Build wsmp message into the specific buffer. */
int wsmp_build_msg(wsmp_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t buffer_size, uint16_t *data_len_ptr)
{
    int              ret = ERR_OK;
    wsmp_buf_st  wsmpbuf = { 0 };


    /* Error detection. */
    if((infor_ptr == NULL) || (buffer_ptr == NULL) || (data_len_ptr == NULL))
    {
        ret = ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Check buffer size for new message. */
    if(buffer_size <= *data_len_ptr)
    {
        ret = ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Move payload data to buffer tail. */
    memmove((buffer_ptr + buffer_size - *data_len_ptr), buffer_ptr, *data_len_ptr);

    /* Init wsmp buffer structure. */
    wsmpbuf.buffer_ptr = buffer_ptr + buffer_size - *data_len_ptr;
    wsmpbuf.data_len = *data_len_ptr;
    wsmpbuf.buffer_size = buffer_size;

    /* Build dsmp header. */
    if((ret = wsmp_build_dsmp(infor_ptr, &wsmpbuf)) != ERR_OK)
    {
        goto ERR_RESET;
    }

    /* Build sec header. */
    if((ret = wsmp_build_sec(infor_ptr, &wsmpbuf)) != ERR_OK)
    {
        goto ERR_RESET;
    }

    /* Build llc header. */
    if((ret = wsmp_build_llc(infor_ptr, &wsmpbuf)) != ERR_OK)
    {
        goto ERR_RESET;
    }

    /* Move new payload data to buffer head. */
    memmove(buffer_ptr, wsmpbuf.buffer_ptr, wsmpbuf.data_len);
    *data_len_ptr = wsmpbuf.data_len;
    
    return ret;
    
ERR_RESET:

    /* Reset old payload data to buffer head. */
    memmove(buffer_ptr, (buffer_ptr + buffer_size - *data_len_ptr), *data_len_ptr);
    
ERR_EXIT:

    return ret; 
}


/*FUNCTION*****************************************************************
* 
* Returned : ERR_OK when analyse succeed Or negative error code when failure.
* Comments : Analyse wsmp message from the specific buffer.
* Caution  : data_len_ptr return the rest valid data length in buffer except to wsmp 
*            mesage head.
*
*END*********************************************************************/
int wsmp_analyse_msg(wsmp_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t *data_len_ptr)
{
    int              ret = ERR_OK;
    wsmp_buf_st  wsmpbuf = { 0 };

    
    /* Error detection. */
    if((infor_ptr == NULL) || (buffer_ptr == NULL) || (data_len_ptr == NULL))
    {
        ret = ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Init wsmp buffer structure. */
    wsmpbuf.buffer_ptr = buffer_ptr;
    wsmpbuf.data_len = *data_len_ptr;
    wsmpbuf.buffer_size = *data_len_ptr;

    /* Analyse llc header. */
    if((ret = wsmp_analyse_llc(infor_ptr, &wsmpbuf)) != ERR_OK)
    {
        fprintf(stderr, "[%s %d]: Analyse llc header error. ret = %d. \n", __FUNCTION__, __LINE__, ret);
        goto ERR_EXIT;
    }

    /* Analyse sec header. */
    if((ret = wsmp_analyse_sec(infor_ptr, &wsmpbuf)) != ERR_OK)
    {
        fprintf(stderr, "[%s %d]: Analyse sec header error. ret = %d. \n", __FUNCTION__, __LINE__, ret);
        goto ERR_EXIT;
    }

    /* Analyse dsmp header. */
    if((ret = wsmp_analyse_dsmp(infor_ptr, &wsmpbuf)) != ERR_OK)
    {
        fprintf(stderr, "[%s %d]: Analyse dsmp header error. ret = %d. \n", __FUNCTION__, __LINE__, ret);
        goto ERR_EXIT;
    }

    *data_len_ptr = wsmpbuf.data_len;

ERR_EXIT:

    return ret; 
}


