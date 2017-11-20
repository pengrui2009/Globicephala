/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : drv_ltev.c
 @brief  : This file contain LTE-V module driver fucntions.
 @author : pengrui
 @history:
           2017-05-30    pengrui      Created file
           2017-06-09    wangxianwen  Optimization file structure.
           ...
******************************************************************************/

#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "drv_ltev.h"
#include "os_core.h"
#include "errno.h"


/*FUNCTION*****************************************************************
* 
* Returned : Negative error code when failure or data length been sended. 
* Comments : Send data to ltev module in device mode. 
*
*END*********************************************************************/
static int drv_ltev_device_send_data(drv_ltev_st_ptr ltev_ptr, ltev_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t data_len)
{
    int                      ret = 0x00;
    uint8_t           *frame_ptr = NULL;
    ltev_dheader_st_ptr head_ptr = NULL;
    uint16_t         payload_len = 0;
    

    /* Error detection. */
    if((ltev_ptr == NULL) || (ltev_ptr->fd_app < 0) || (infor_ptr == NULL) || (buffer_ptr == NULL) 
    || (data_len < LTEV_DATALENTH_MIN) || (LTEV_DATALENTH_MAX < data_len))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Format data payload length and calloc memory for ltev frame.  */
    payload_len = ((data_len % 4) != 0) ? (((data_len / 4) + 1) * 4) : data_len;
    if((frame_ptr = calloc(1, payload_len + LTEV_DHEADER_ST_LEN)) == NULL)
    {
        printf("[%s %d]: Calloc data buffer error. \n", __FUNCTION__, __LINE__);
        
        ret = ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Init frame head. */
    head_ptr = (ltev_dheader_st_ptr)frame_ptr;

    memcpy(head_ptr->ueid, infor_ptr->ueid, sizeof(head_ptr->ueid));
    memcpy(head_ptr->psid, infor_ptr->psid, sizeof(head_ptr->psid));

    head_ptr->pw = infor_ptr->pw;
    head_ptr->pri = infor_ptr->pri;
    head_ptr->pi = infor_ptr->pi;
    head_ptr->ci = infor_ptr->ci;

    head_ptr->length_l = (payload_len & 0x00FF);
    head_ptr->length_h = ((payload_len >> 8) & 0x00FF);

    head_ptr->tm = LTEV_DHEADER_TM_DATA;
    head_ptr->ts = LTEV_DHEADER_TS_DATA;

    /* Init payload. */
    memcpy(frame_ptr + LTEV_DHEADER_ST_LEN, buffer_ptr, data_len);

    /* Send data to baseband. */
    if((ret = write(ltev_ptr->fd_app, frame_ptr, payload_len + LTEV_DHEADER_ST_LEN)) < 0)
    {
            printf("[%s %d]: Send data to ltev device file error. ret = %d. errno = %d(%s). \n", \
                    __FUNCTION__, __LINE__, ret, errno, strerror(errno));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }
    
ERR_EXIT:
    
    if(frame_ptr != NULL)
    {
        free(frame_ptr);
    }    
    
    return ret;
}


/*FUNCTION*****************************************************************
* 
* Returned : ERR_OK when succeed or negative error code when failure. The data
*            length will be 0 when received my own machine's ltev data.
* Comments : Receive data through ltev module. Caution: User should give enough 
*            data length value to store data.
*
*END*********************************************************************/
static int drv_ltev_device_receive_data(drv_ltev_st_ptr ltev_ptr, ltev_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t data_len)
{
    int              ret = 0x00;
    ltev_dheader_st head = { { 0 }, { 0 }, 0 };
    uint16_t payload_len = 0x00;
    
    uint16_t payload_act = 0;


    /* Error detection. */
    if((ltev_ptr == NULL) || (ltev_ptr->fd_app < 0) || (infor_ptr == NULL) 
    || (buffer_ptr == NULL) || (data_len == 0))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Read the head data from baseband. */
    if((ret = read(ltev_ptr->fd_app, (uint8_t*)&head, sizeof(head))) != sizeof(head))
    {
            printf("[%s %d]: Read data header in ltev device file error. ret = %d. errno = %d(%s).\n", \
                    __FUNCTION__, __LINE__, ret, errno, strerror(errno));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    /* Init payload length. */
    payload_len = head.length_h;
    payload_len = ((payload_len << 8) & 0xFF00) + head.length_l;
    
    /* Read the payload data. */
    if(payload_len <= data_len)
    {            
        /* Read the payload data from baseband for while. */
        while(payload_act < payload_len)
        {
            ret = read(ltev_ptr->fd_app, buffer_ptr + payload_act, payload_len - payload_act);
            if(ret < 0)
            {
                printf("[%s %d]: Read data payload in ltev device file error. ret = %d. errno = %d(%s).\n", \
                            __FUNCTION__, __LINE__, ret, errno, strerror(errno));
                ret = -ERR_SYS;
                goto ERR_EXIT;
            }
            else
            {
                payload_act += ret;
            }
        }
        
        ret = payload_len;
    }
    else
    {   
        /* No frame edge code for current mode,so do nothting. */
        printf("[%s %d]: Data payload length over then user buffer. Payload = %d, Userbuff = %d. \n", \
                                                    __FUNCTION__, __LINE__, payload_len, data_len);
        ret = ERR_NODISK;
        goto ERR_EXIT;
    }

    /* Set other infor structure. */
    memcpy(infor_ptr->ueid, head.ueid, sizeof(infor_ptr->ueid));
    memcpy(infor_ptr->psid, head.psid, sizeof(infor_ptr->psid));

    infor_ptr->pw = head.pw;
    infor_ptr->pri = head.pri;
    infor_ptr->pi = head.pi;
    infor_ptr->ci = head.ci;

ERR_EXIT:

    return ret;
}


/*FUNCTION*****************************************************************
* 
* Returned : Negative error code when failure or data length been sended. 
* Comments : Send data to ltev module in socket mode. 
*
*END*********************************************************************/
static int drv_ltev_socket_send_data(drv_ltev_st_ptr ltev_ptr, ltev_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t data_len)
{
    int                      ret = 0x00;
    uint8_t           *frame_ptr = NULL;
    ltev_dheader_st_ptr head_ptr = NULL;
    uint16_t         payload_len = 0;

    struct sockaddr_in         dest_addr;
    uint8_t      ip_broadcast[4] = { 0 };


    /* Error detection. */
    if((ltev_ptr == NULL) || (ltev_ptr->fd_app < 0) || (infor_ptr == NULL) || (buffer_ptr == NULL) 
    || (data_len < LTEV_DATALENTH_MIN) || (LTEV_DATALENTH_MAX < data_len))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Format data payload length and calloc memory for ltev frame.  */
    payload_len = ((data_len % 4) != 0) ? (((data_len / 4) + 1) * 4) : data_len;
    if((frame_ptr = calloc(1, payload_len + LTEV_DHEADER_ST_LEN)) == NULL)
    {
        printf("[%s %d]: Calloc data buffer error. \n", __FUNCTION__, __LINE__);
        
        ret = ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Init frame head. */
    head_ptr = (ltev_dheader_st_ptr)frame_ptr;

    memcpy(head_ptr->ueid, infor_ptr->ueid, sizeof(head_ptr->ueid));
    memcpy(head_ptr->psid, infor_ptr->psid, sizeof(head_ptr->psid));

    head_ptr->pw = infor_ptr->pw;
    head_ptr->pri = infor_ptr->pri;
    head_ptr->pi = infor_ptr->pi;
    head_ptr->ci = infor_ptr->ci;

    head_ptr->length_l = (payload_len & 0x00FF);
    head_ptr->length_h = ((payload_len >> 8) & 0x00FF);

    head_ptr->tm = LTEV_DHEADER_TM_DATA;
    head_ptr->ts = LTEV_DHEADER_TS_DATA;

    /* Init payload. */
    memcpy(frame_ptr + LTEV_DHEADER_ST_LEN, buffer_ptr, data_len);
    
    /* Set destination address. */
    bzero(&dest_addr, sizeof(struct sockaddr_in));
    dest_addr.sin_family = AF_INET;

    /* Set boardcast destination ip to host ip's network segment ip. Do not use "dest_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST)", 
    or the frame will output from eth0 not the network bridge. */
    memcpy(ip_broadcast, &(ltev_ptr->local_ip.word), sizeof(ip_broadcast));
    ip_broadcast[3] = 0xFF;
    memcpy(&(dest_addr.sin_addr.s_addr), ip_broadcast, sizeof(ip_broadcast));
    
    dest_addr.sin_port = htons(ltev_ptr->config.socket_cfg.remote_port);

    /* Send data to destination. */
    if((ret = (sendto(ltev_ptr->fd_app, frame_ptr, payload_len+LTEV_DHEADER_ST_LEN, 0, (struct sockaddr*)&dest_addr, sizeof(struct sockaddr)))) < 0)
    {
            printf("[%s %d]: Send data to ltev socket file error. ret = %d. errno = %d(%s). \n", \
                    __FUNCTION__, __LINE__, ret, errno, strerror(errno));
        if((errno == EPIPE) || (errno == EDESTADDRREQ) || (errno == EBADF))
        {
            ret = -ERR_DISCONNECT;
        }
        else
        {
            ret = -ERR_TIMEOUT;
        }
        goto ERR_EXIT;
    }   
    
ERR_EXIT:
    
    if(frame_ptr != NULL)
    {
        free(frame_ptr);
    }
    
    return ret;
}


/*FUNCTION*****************************************************************
* 
* Returned : ERR_OK when succeed or negative error code when failure. The data
*            length will be 0 when received my own machine's ltev data.
* Comments : Receive data through ltev module. Caution: User should give enough 
*            data length value to store data.
*
*END*********************************************************************/
static int drv_ltev_socket_receive_data(drv_ltev_st_ptr ltev_ptr, ltev_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t data_len)
{
    int              ret = 0x00;
    ltev_dheader_st head = { { 0 }, { 0 }, 0 };
    uint16_t payload_len = 0x00;
    
    uint8_t   *frame_ptr = NULL;
    struct  sockaddr_in src_addr;
    socklen_t        len = 0; 
    int       actual_len = 0;


    /* Error detection. */
    if((ltev_ptr == NULL) || (ltev_ptr->fd_app < 0) || (infor_ptr == NULL) 
    || (buffer_ptr == NULL) || (data_len == 0))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Read the head data from socket in peek mode and the data frame will not been delete from receive list. */
    bzero(&src_addr, sizeof(struct sockaddr_in));
    len = sizeof(struct sockaddr);
    if(recvfrom(ltev_ptr->fd_app, (uint8_t*)&head, sizeof(head), MSG_PEEK, (struct sockaddr *)&src_addr, &len) <= 0)
    {
            printf("[%s %d]: Read data header in ltev socket file error. ret = %d. errno = %d(%s).\n", \
                    __FUNCTION__, __LINE__, ret, errno, strerror(errno));
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    /* Init payload length. */
    payload_len = head.length_h;
    payload_len = ((payload_len << 8) & 0xFF00) + head.length_l;

    /* Read the payload data. */
    if(payload_len <= data_len)
    {        
        /* Calloc memory for ltev frame.  */
        if((frame_ptr = calloc(1, payload_len + LTEV_DHEADER_ST_LEN)) == NULL)
        {
            printf("[%s %d]: Calloc data buffer error. \n", __FUNCTION__, __LINE__);
            ret = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        /* Read the frame data from socket. */
        bzero(&src_addr, sizeof(struct sockaddr_in));
        len = sizeof(struct sockaddr);
        actual_len = recvfrom(ltev_ptr->fd_app, frame_ptr, payload_len + LTEV_DHEADER_ST_LEN, 0, (struct sockaddr *)&src_addr, &len);
        if(actual_len != (payload_len + LTEV_DHEADER_ST_LEN))
        {
            printf("[%s %d]: Read data payload in ltev socket file error. ret = %d. errno = %d(%s).\n",\
                        __FUNCTION__, __LINE__, ret, errno, strerror(errno));
            ret = -ERR_SYS;
            goto ERR_EXIT;
        }

        /* Copy head and payload data. */
        memcpy(&head, frame_ptr, LTEV_DHEADER_ST_LEN);
        memcpy(buffer_ptr, frame_ptr + LTEV_DHEADER_ST_LEN, payload_len);
    
        ret = payload_len; 
    }
    else
    {
        /* Read the head data from socket and the data frame will been delete from receive list. */
        bzero(&src_addr, sizeof(struct sockaddr_in));
        len = sizeof(struct sockaddr);
        recvfrom(ltev_ptr->fd_app, (uint8_t*)&head, sizeof(head), 0, (struct sockaddr *)&src_addr, &len);
        
        printf("[%s %d]: Data payload length over then user buffer. Payload = %d, Userbuff = %d. \n", \
                                                    __FUNCTION__, __LINE__, payload_len, data_len);

        ret = ERR_NODISK;
        goto ERR_EXIT;
    }
    
#if 0
    /* Set data length to 0 when received my own's frame. */
    if(memcmp(&(ltev_ptr->local_ip.word), &(src_addr.sin_addr.s_addr), sizeof(ltev_ptr->local_ip.word)) == 0)
    {
        * data_len_ptr = 0;
    }
#endif    

    /* Set other infor structure. */
    memcpy(infor_ptr->ueid, head.ueid, sizeof(infor_ptr->ueid));
    memcpy(infor_ptr->psid, head.psid, sizeof(infor_ptr->psid));

    infor_ptr->pw = head.pw;
    infor_ptr->pri = head.pri;
    infor_ptr->pi = head.pi;
    infor_ptr->ci = head.ci;

ERR_EXIT:

    if(frame_ptr != NULL)
    {
        free(frame_ptr);
    }

    return ret;
}


/******************************************************************************
*    Function: drv_ltev_send
*    Descriptions: Send data to the specific ltev driver.
*    Paramters:
            fd                -    the file descriptor.
            infor_ptr       -   data information.
            buff_ptr        -   buffer head address for data storage.
            data_len        -   the data length that user want to send.
*    Return:
            >= 0            -    the data count that have sended.
            < 0                -    failed
*    Comments: 
******************************************************************************/
int drv_ltev_send(int fd, ltev_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t data_len)
{
    int                  ret = 0x00;
    drv_ltev_st_ptr ltev_ptr = (drv_ltev_st_ptr)fd;
    

    /* Error detection. */
    if(ltev_ptr == NULL)
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Send data based on ltev mode. */
    if(ltev_ptr->config.dev_mode == LTEV_MODE_DEVICE)
    {
        ret = drv_ltev_device_send_data(ltev_ptr, infor_ptr, buffer_ptr, data_len);
    }
    else if(ltev_ptr->config.dev_mode == LTEV_MODE_SOCKET)
    {
        ret = drv_ltev_socket_send_data(ltev_ptr, infor_ptr, buffer_ptr, data_len);
    }
    else
    {
        printf("[%s %d]: Do not support letv mode. Ltev mode = %d. \n", __FUNCTION__, __LINE__, ltev_ptr->config.dev_mode);
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

ERR_EXIT:

    return ret;
}


/******************************************************************************
*    Function: drv_ltev_receive
*    Descriptions: Receive data from the specific ltev driver.
*    Paramters:
            fd                -    the file descriptor.
            infor_ptr       -   data information.
            buff_ptr        -   buffer head address for data storage.
            data_len        -   the data count that user want to receive.
*    Return:
            >= 0            -    the data count that have received.
            < 0                -    failed
*    Comments: 
******************************************************************************/
int drv_ltev_receive(int fd, ltev_infor_st_ptr infor_ptr, uint8_t *buffer_ptr, uint16_t data_len)
{
    int          ret = 0x00;
    drv_ltev_st_ptr ltev_ptr = (drv_ltev_st_ptr)fd;


    /* Error detection. */
    if(ltev_ptr == NULL)
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Send data based on ltev mode. */
    if(ltev_ptr->config.dev_mode == LTEV_MODE_DEVICE)
    {
        ret = drv_ltev_device_receive_data(ltev_ptr, infor_ptr, buffer_ptr, data_len);
    }
    else if(ltev_ptr->config.dev_mode == LTEV_MODE_SOCKET)
    {
        ret = drv_ltev_socket_receive_data(ltev_ptr, infor_ptr, buffer_ptr, data_len);
    }
    else
    {
        printf("[%s %d]: Do not support letv mode. Ltev mode = %d. \n", __FUNCTION__, __LINE__, ltev_ptr->config.dev_mode);
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
ERR_EXIT:

    return ret;
}


/* Init ltev in device mode. */
static int drv_ltev_device_fd_init(drv_ltev_st_ptr ltev_ptr)
{
    int    ret = 0x00;
    

    /* Application file descriptor. */
    if((ltev_ptr->fd_app = open(ltev_ptr->config.device_cfg.if_name, O_RDWR)) < 0)
    {
           printf("[%s %d]: Open ltev device file error. ret = %d. errno = %d(%s). \n", \
                       __FUNCTION__, __LINE__, ltev_ptr->fd_app, errno, strerror(errno));

        ret = -ERR_NODEV;
        goto ERR_EXIT;
    }

    ret = ERR_OK;
    
ERR_EXIT:

    return ret;
}


/* Init ltev local ip domain. */
static int drv_ltev_local_ip_init(drv_ltev_st_ptr ltev_ptr)
{
    int                  ret = 0;
    int            socket_fd = 0;
    struct             ifreq ifr;
    struct sockaddr_in *addr_ptr;


    /* Create raw socket. */
    if((socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) 
    {
        printf("[%s %d]: Create local_ip socket error. \n", __FUNCTION__, __LINE__);

        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    /* Get interface address. */
    bzero(&ifr, sizeof(ifr));
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, ltev_ptr->config.socket_cfg.if_name, sizeof(ltev_ptr->config.socket_cfg.if_name));
    
    if(ioctl(socket_fd, SIOCGIFADDR, &ifr) == 0)
    {
        addr_ptr = (struct sockaddr_in *)&ifr.ifr_addr;
        memcpy(&(ltev_ptr->local_ip.word), &(addr_ptr->sin_addr.s_addr), sizeof(ltev_ptr->local_ip.word));

        ret = ERR_OK;
    }
    else
    {
           printf("[%s %d]: Get local ip error. \n", __FUNCTION__, __LINE__);
        ret = -ERR_SYS;
    }
    
ERR_EXIT:

    /* Close socket. */
    if(socket_fd != 0)
    {
        close(socket_fd);
    }
    
    return ret;
}


/* Init ltev in socket mode. */
static int drv_ltev_socket_fd_init(drv_ltev_st_ptr ltev_ptr)
{
    int                  ret = 0x00;
    int               optval = 1;
    struct sockaddr_in sockaddr_udp;
    int           nochecksum = 0;
 

    /* Application socket. */
    if((ltev_ptr->fd_app = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("[%s %d]: Create app socket error. \n", __FUNCTION__, __LINE__);
        ret = ERR_NODEV;
        goto ERR_EXIT;
    }

    /* Set socket broadcast mode. */
    if(setsockopt(ltev_ptr->fd_app, SOL_SOCKET, SO_BROADCAST,(char *)&optval, sizeof(optval)) !=  0)
    {
        printf("[%s %d]: Set app socket broadcast mode error. \n", __FUNCTION__, __LINE__);
        
        /* Close socket. */
        close(ltev_ptr->fd_app);
        ltev_ptr->fd_app = -1;

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Set socket no checksum mode. */
    nochecksum = !(ltev_ptr->config.socket_cfg.checksum);
    if(setsockopt(ltev_ptr->fd_app, SOL_SOCKET, SO_NO_CHECK, (char *)&nochecksum, sizeof(nochecksum)) != 0)
    {
        printf("[%s %d]: Set app socket no checksum mode error. \n", __FUNCTION__, __LINE__);

        /* Close socket. */
        close(ltev_ptr->fd_app);
        ltev_ptr->fd_app = -1;

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Init udp socket address. */
    bzero(&sockaddr_udp, sizeof(struct sockaddr_in));
    sockaddr_udp.sin_family = AF_INET;
    sockaddr_udp.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr_udp.sin_port = htons(ltev_ptr->config.socket_cfg.host_port);

    /* Bind application socket. */
    if(bind(ltev_ptr->fd_app, (struct sockaddr *)&sockaddr_udp, sizeof(struct sockaddr)) < 0)
    {
        printf("[%s %d]: Bind app socket error. \n", __FUNCTION__, __LINE__);
        
        /* Close socket. */
        close(ltev_ptr->fd_app);
        ltev_ptr->fd_app = -1;
        
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    ret = ERR_OK;
    
ERR_EXIT:

    return ret;
}


/******************************************************************************
*    Function: drv_ltev_init
*    Descriptions: open the specific ltev device.
*    Paramters:
            fd_ptr          -   the pointer that hold file descriptor.
            cfg_ptr            -    the config paramters pointer.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
int drv_ltev_init(int *fd_ptr, ltev_config_st_ptr cfg_ptr)
{
    int                  ret = 0x00;
    drv_ltev_st_ptr ltev_ptr = NULL;


    /* Error detection. */
    if((fd_ptr == NULL) || (cfg_ptr == NULL))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Host port should different from remote port. */
    if(cfg_ptr->dev_mode == LTEV_MODE_SOCKET)
    {
        if(cfg_ptr->socket_cfg.host_port == cfg_ptr->socket_cfg.remote_port)
        {
            printf("[%s %d]: The host port should be different from remote port. \n", __FUNCTION__, __LINE__);

            ret = -ERR_INVAL;
            goto ERR_EXIT;
        }
    }

    /* Allocate memory for driver. */
    if((ltev_ptr = os_calloc(1, sizeof(*ltev_ptr))) == NULL)
    {
        printf("[%s %d]: Allocate memory for ltev driver error. \n", __FUNCTION__, __LINE__);

        ret = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Reset the ltev structure. */
    ltev_ptr->fd_app = -1;
    ltev_ptr->fd_ctrl = -1;
    memset(&(ltev_ptr->config), 0, sizeof(ltev_ptr->config));
    
    /* Set configuration. */
    memcpy(&(ltev_ptr->config), cfg_ptr, sizeof(ltev_ptr->config));
    
    /* Init the specific device based on user configuration. */
    if(ltev_ptr->config.dev_mode == LTEV_MODE_DEVICE)
    {
        if((ret = drv_ltev_device_fd_init(ltev_ptr)) != ERR_OK)
        {
            printf("[%s %d]: Init ltev device file error. ret = %d. \n", __FUNCTION__, __LINE__, ret);
            ret = -ERR_NODEV;
            goto ERR_EXIT;
        }
    }
    else if(ltev_ptr->config.dev_mode == LTEV_MODE_SOCKET)
    {
        if((ret = drv_ltev_local_ip_init(ltev_ptr)) != ERR_OK)
        {
            printf("[%s %d]: Init ltev local ip error. ret = %d. \n", __FUNCTION__, __LINE__, ret);
            ret = -ERR_SYS;
            goto ERR_EXIT;
        }

        if((ret = drv_ltev_socket_fd_init(ltev_ptr)) != ERR_OK)
        {
            printf("[%s %d]: Init ltev socket file error. ret = %d. \n", __FUNCTION__, __LINE__, ret);
            ret = -ERR_NODEV;
            goto ERR_EXIT;
        } 
    }
    else
    {
        printf("[%s %d]: Do not support letv mode. Ltev mode = %d. \n", __FUNCTION__, __LINE__, ltev_ptr->config.dev_mode);
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }


    /* return the file descriptor. */
    * fd_ptr = (int)ltev_ptr;
    ret = ERR_OK;
    
    return ret;
    
ERR_EXIT:

    /* Deinit the ltev driver when error. */
    if(ltev_ptr != NULL)
    {
        drv_ltev_deinit((int)ltev_ptr);
    }
    
    return ret;
}


/******************************************************************************
*    Function: drv_ltev_deinit
*    Descriptions: Deinit the ltev fd device.
*    Paramters:
            fd                -    the file descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
int drv_ltev_deinit(int fd)
{
    int                  ret = ERR_OK;
    drv_ltev_st_ptr ltev_ptr = (drv_ltev_st_ptr)fd;


    /* Error detection. */
    if(ltev_ptr == NULL)
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        ret = -ERR_INVAL;
    }
    else
    {
        /* Close application file descriptors. */
        if(0 < ltev_ptr->fd_app)
        {
            close(ltev_ptr->fd_app);
        }

        /* Close control file descriptors. */
        if(0 < ltev_ptr->fd_ctrl)
        {
            close(ltev_ptr->fd_ctrl);
        }

        /* Reset the file descriptors. */
        ltev_ptr->fd_app = -1;
        ltev_ptr->fd_ctrl = -1;
        memset(&(ltev_ptr->config), 0, sizeof(ltev_ptr->config));

        /* Free the driver memory. */
        os_free(ltev_ptr);
        
        ret = ERR_OK;
    }

    return ret;
}


