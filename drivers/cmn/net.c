/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : net.c
 @brief  : This file contain NET module driver fucntions.
 @author : pengrui
 @history:
           2016-10-25    pengrui      Created file
           2017-10-26    wangxianwen  Optimization file structure.
           2017-11-30    pengrui      Add the checksum flag
           2017-11-30    pengrui      Modify the param of net_send function
           ...
******************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "os_core.h"
#include "net.h"


/******************************************************************************
*    Function: net_config
*    Descriptions: config the specific net fd driver.
*    Paramters:
            fd                -    the file descriptor.
            cfg_ptr            -    the config paramters pointer.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
int net_config(int fd, net_config_st_ptr cfg_ptr)
{
    int                     ret = 0x00;
    net_st_ptr      net_ptr = (net_st_ptr)fd;


    /* Error detection. */
    if((net_ptr == NULL) || (cfg_ptr == NULL))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Host port should different from remote port. */
    if(cfg_ptr->host_port == cfg_ptr->remote_port)
    {
        printf("[%s %d]: The host port should be different from remote port. \n", __FUNCTION__, __LINE__);

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Set configuration. */
    memcpy(&(net_ptr->config), cfg_ptr, sizeof(net_ptr->config));

    ret = ERR_OK;

ERR_EXIT:
    
    return ret;
}


/******************************************************************************
*    Function: net_send
*    Descriptions: Send data to the specific net driver.
*    Paramters:
            fd                - the file descriptor.
            flag              - send flag:BLOCK or NOBLOCK
            send_ipaddr         the ipaddr we need send to
            buff_ptr          - buffer head address for data storage.
            data_len          - the data length that user want to send.
*    Return:
            >= 0              - the data count that have sended.
            < 0               - failed
*    Comments: 
******************************************************************************/
int net_send(int fd, uint16_t flag, uint8_t *ipaddr, uint8_t *buff_ptr, uint16_t data_len)
{
    int                     ret = 0x00;
    struct sockaddr_in        dest_addr;
    //uint8_t     ip_broadcast[4] = { 0 };
    net_st_ptr      net_ptr = (net_st_ptr)fd;


    /* Error detection. */
    if((net_ptr == NULL) || (NULL == ipaddr) || (net_ptr->fd < 0) || (buff_ptr == NULL))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Set destination address. */
    bzero(&dest_addr, sizeof(struct sockaddr_in));
    dest_addr.sin_family = AF_INET;

    /* Set boardcast destination ip to host ip's network segment ip. Do not use "dest_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST)", 
    or the frame will output from eth0 not the network bridge. */
    //memcpy(ip_broadcast, send_ipaddr, sizeof(ip_broadcast));
    memcpy(&(dest_addr.sin_addr.s_addr), ipaddr, 4);
    dest_addr.sin_port = htons(net_ptr->config.remote_port);

    /* Send data to destination. */
    
    if(NET_NOBLOCK == flag)
    {
        ret = sendto(net_ptr->fd, buff_ptr, data_len, MSG_DONTWAIT, (struct sockaddr*)&dest_addr, sizeof(struct sockaddr));
    }else{
        ret = sendto(net_ptr->fd, buff_ptr, data_len, 0, (struct sockaddr*)&dest_addr, sizeof(struct sockaddr));
    }
    if(ret < 0)
    {
        printf("[%s %d]: Send data to net socket error. ret = %d. \n", __FUNCTION__, __LINE__, ret);
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
    
    return ret;
}


/******************************************************************************
*    Function: net_receive
*    Descriptions: Receive data from the specific net driver.
*    Paramters:
            fd                   - the file descriptor.
            flag                 - the flag of recv:BLOCK or NOBLOCK
            buff_ptr             - buffer head address for data storage.
            data_len             - the data count that user want to receive.
*    Return:
            >= 0                 - the data count that have received.
            < 0                  - failed
*    Comments: 
******************************************************************************/
int net_receive(int fd, uint16_t flag, uint8_t *ipaddr, uint8_t *buff_ptr, uint16_t data_len)
{
    int               ret = 0x00;
    struct  sockaddr_in src_addr;
    socklen_t         len = 0; 
    int        actual_len = 0;
    net_st_ptr net_ptr = (net_st_ptr)fd;


    /* Error detection. */
    if((net_ptr == NULL) || (NULL == ipaddr) || (net_ptr->fd < 0) || (buff_ptr == NULL))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Read the frame data from socket. */
    bzero(&src_addr, sizeof(struct sockaddr_in));
    len = sizeof(struct sockaddr);
    if(NET_NOBLOCK == flag)
    {
        actual_len = recvfrom(net_ptr->fd, buff_ptr, data_len, MSG_DONTWAIT, (struct sockaddr *)&src_addr, &len);
    }else{
        actual_len = recvfrom(net_ptr->fd, buff_ptr, data_len, 0, (struct sockaddr *)&src_addr, &len);
    }
    
    if(actual_len < 0)
    {
        printf("[%s %d]: Read data in net socket error. \n", __FUNCTION__, __LINE__);
        
        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    memcpy(ipaddr, &(src_addr.sin_addr.s_addr), 4);
    /* Return the actual data length. */
    ret = actual_len;

ERR_EXIT:

    return ret;
}


/******************************************************************************
*    Function: net_local_ip_init
*    Descriptions: Get local ip address.
*    Paramters:
            name_ptr        -    the net name.
            net_ptr         -   net driver structure pointer.
*    Return:
            = 0                -    success.
            < 0                -    failed
*    Comments: 
******************************************************************************/
static int net_local_ip_init(const char *name_ptr, net_st_ptr net_ptr)
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
    strncpy(ifr.ifr_name, name_ptr, strlen(name_ptr)+1);
    
    if(ioctl(socket_fd, SIOCGIFADDR, &ifr) == 0)
    {
        addr_ptr = (struct sockaddr_in *)&ifr.ifr_addr;
        memcpy(&(net_ptr->local_ip.word), &(addr_ptr->sin_addr.s_addr), sizeof(net_ptr->local_ip.word));

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


/******************************************************************************
*    Function: net_socket_fd_init
*    Descriptions: Init net socket file.
*    Paramters:
            net_ptr         -   net driver structure pointer.
*    Return:
            = 0                -    success.
            < 0                -    failed
*    Comments: 
******************************************************************************/
static int net_socket_fd_init(net_st_ptr net_ptr)
{
    int                  ret = 0x00;
    int               optval = 1;
    int               nochecksum = 0;
    struct sockaddr_in sockaddr_udp;


    /* Application socket. */
    if((net_ptr->fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("[%s %d]: Create net socket error. \n", __FUNCTION__, __LINE__);
        ret = -ERR_NODEV;
        goto ERR_EXIT;
    }

    /* Set socket broadcast mode. */
    if(setsockopt(net_ptr->fd, SOL_SOCKET, SO_BROADCAST,(char *)&optval, sizeof(optval)) !=  0)
    {
        printf("[%s %d]: Set net socket broadcast mode error. \n", __FUNCTION__, __LINE__);
        
        /* Close socket. */
        close(net_ptr->fd);
        net_ptr->fd = -1;

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Set reuse mode. */
    if(setsockopt(net_ptr->fd, SOL_SOCKET, SO_REUSEADDR,(char *)&optval,sizeof(optval)) !=  0)
    {
        printf("[%s %d]: Set net socket reuse mode error. \n", __FUNCTION__, __LINE__);
        
        /* Close socket. */
        close(net_ptr->fd);
        net_ptr->fd = -1;

        ret = -ERR_INVAL;
        goto ERR_EXIT;

    }

    /* Set socket no checksum mode. */
    nochecksum = !(net_ptr->config.checksum);
    if(setsockopt(net_ptr->fd, SOL_SOCKET, SO_NO_CHECK, (char *)&nochecksum, sizeof(nochecksum)) != 0)
    {
        printf("[%s %d]: Set app socket no checksum mode error. \n", __FUNCTION__, __LINE__);

        /* Close socket. */
        close(net_ptr->fd);
        net_ptr->fd = -1;

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Init udp socket address. */
    bzero(&sockaddr_udp, sizeof(struct sockaddr_in));
    sockaddr_udp.sin_family = AF_INET;
    sockaddr_udp.sin_addr.s_addr = htonl(INADDR_ANY);
    //memcpy(&sockaddr_udp.sin_addr.s_addr, net_ptr->local_ip.byte, sizeof(net_ptr->local_ip.byte));
    sockaddr_udp.sin_port = htons(net_ptr->config.host_port);
    /* Bind net socket. */
    if(bind(net_ptr->fd, (struct sockaddr *)&sockaddr_udp, sizeof(struct sockaddr)) < 0)
    {
        printf("[%s %d]: Bind net socket error. \n", __FUNCTION__, __LINE__);
        
        /* Close socket. */
        close(net_ptr->fd);
        net_ptr->fd = -1;
        
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    ret = ERR_OK;
    
ERR_EXIT:

    return ret;
}


/******************************************************************************
*    Function: net_init
*    Descriptions: open the specific net device.
*    Paramters:
            fd_ptr          -   the pointer that hold net file descriptor.
            name_ptr        -    the device character pointer.
            cfg_ptr            -    the config paramters pointer.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
int net_init(int *fd_ptr, const char *name_ptr, net_config_st_ptr cfg_ptr)
{
    int            ret = ERR_OK;
    net_st_ptr net_ptr = NULL;


    /* Error detection. */
    if((fd_ptr == NULL) || (name_ptr == NULL) || (cfg_ptr == NULL))
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Host port should different from remote port. */
    if(cfg_ptr->host_port == cfg_ptr->remote_port)
    {
        printf("[%s %d]: The host port should be different from remote port. \n", __FUNCTION__, __LINE__);

        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Allocate memory for driver. */
    if((net_ptr = os_calloc(1, sizeof(*net_ptr))) == NULL)
    {
        printf("[%s %d]: Allocate memory for net driver error. \n", __FUNCTION__, __LINE__);

        ret = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Set configuration. */
    memcpy(&(net_ptr->config), cfg_ptr, sizeof(net_ptr->config));

    /* Init local ip. */
    if((ret = net_local_ip_init(name_ptr, net_ptr)) != ERR_OK)
    {
        printf("[%s %d]: Init net local ip error. ret = %d. \n", __FUNCTION__, __LINE__, ret);

        ret = -ERR_SYS;
        goto ERR_EXIT;
    }

    /* Init socket fd. */
    if((ret = net_socket_fd_init(net_ptr)) != ERR_OK)
    {
        printf("[%s %d]: Init net socket error. ret = %d. \n", __FUNCTION__, __LINE__, ret);

        ret = -ERR_NODEV;
        goto ERR_EXIT;
    } 

    /* return the file descriptor. */
    * fd_ptr = (int)net_ptr;
    ret = ERR_OK;
    
    return ret;
    
ERR_EXIT:

    /* Deinit the net driver when error. */
    if(net_ptr != NULL)
    {
        net_deinit((int)net_ptr);
    }

    return ret;
}


/******************************************************************************
*    Function: net_deinit
*    Descriptions: Deinit the net fd device.
*    Paramters:
            fd                -    the file descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
int net_deinit(int fd)
{
    int                ret = ERR_OK;
    net_st_ptr net_ptr = (net_st_ptr)fd;
    
    
    /* Error detection. */
    if(net_ptr == NULL)
    {
        printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);
        ret = -ERR_INVAL;
    }
    else
    {
        /* Close net file descriptors. */
        if(0 < net_ptr->fd)
        {
            close(net_ptr->fd);
        }
        net_ptr->fd = -1;
        memset(&(net_ptr->config), 0, sizeof(net_ptr->config));
        memset(&(net_ptr->local_ip.word), 0, sizeof(net_ptr->local_ip.word));

        /* Free the driver memory. */
        os_free(net_ptr);
        
        ret = ERR_OK;
    }

    return ret;
}


