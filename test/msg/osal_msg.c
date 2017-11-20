/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_msg.c
 @brief  : the function of the systemV msg.
 @author : pengrui
 @history:
           2017-10-19    pengrui    Created file
           ...
******************************************************************************/
#include "osal_msg.h"

/******************************************************************************
*    Function: osal_msg_open
*    Descriptions: open XSI msg.
*    Paramters:
            fd                         - the handle of te msg(returned)
            key_name_ptr               - the name of key
            key_id                     - the id of key
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_msg_open(osal_msg_t *msgfd_ptr, const char * key_name_ptr, int key_id)
{
    int ret = 0;

    ret = os_msg_init(msgfd_ptr, key_name_ptr, key_id);
    if(ret)
    {
        goto ERR_EXIT;
    }
    
ERR_EXIT:
    
    return ret;
}

/******************************************************************************
*    Function: osal_msg_close
*    Descriptions: close the XSI msg.
*    Paramters:
            msgfd                     - the fd of the XSI msg
*    Return:
            = 0                       - success
            < 0                       - failed
*    Comments: 
******************************************************************************/
int osal_msg_close(osal_msg_t msgfd)
{
    int ret = 0;

    ret = os_msg_deinit(msgfd);

    return ret;
}

/******************************************************************************
*    Function: osal_msg_send
*    Descriptions: send msg to msg queue.
*    Paramters:
            fd                       - the handle fd of the msg
            type                     - the msg type
            data_ptr                 - the msg data 
            data_len                 - the length of msg data
            timeout                  - wait time 0:BLOCK 1:NOBLOCK
*    Return:
            = 0                      - success
            < 0                      - failed
*    Comments: 
******************************************************************************/
int osal_msg_send(osal_msg_t msgfd, long type, void *data_ptr, uint32_t data_len, uint16_t timeout)
{
    int ret = 0;

    ret = os_msg_snd(msgfd, type, (const void *)data_ptr, data_len, timeout);
    
    return ret;
}

/******************************************************************************
*    Function: osal_msg_recv
*    Descriptions: recv msg from msg queue.
*    Paramters:
            fd                     - the handle fd of the XSI msg
            type                   - the msg type we need recv 0: recv all 
            data_ptr               - the msg data recv
            data_len               - the length of msg data
            timeout                - wait time 0:BLOCK 1:NOBLOCK
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_msg_recv(osal_msg_t msgfd, long *type, void *data_ptr, uint32_t *data_len, uint16_t timeout)
{
    int ret = 0;

    ret = os_msg_rcv(msgfd, type, (void *)data_ptr, (size_t *)data_len, timeout);

    return ret;
}

/******************************************************************************
*    Function: osal_msg_clear
*    Descriptions: clear msg queue.
*    Paramters:
            fd                     - the handle fd of the XSI msg
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_msg_clear(osal_msg_t msgfd)
{
    int ret = 0;
    uint8_t msgbuf[MSGBUF_SIZE] = {0};
    uint32_t msglen = sizeof(msgbuf);
    long msgtype = 0;

    ret = os_msg_rcv(msgfd, &msgtype, (void *)&msgbuf, &msglen, OSAL_NO_WAIT);
    if(ret < 0 && (-ERR_NOMSG != ret))
    {
        goto ERR_EXIT;
    }
    
    while(-ERR_NOMSG != ret)
    {
        msglen = sizeof(msgbuf);
        msgtype = 0;
        ret = os_msg_rcv(msgfd, &msgtype, (void *)&msgbuf, &msglen, OSAL_NO_WAIT);
        if(ret < 0 && (-ERR_NOMSG != ret))
        {
            goto ERR_EXIT;
        }
    }

    ret = 0;
ERR_EXIT:
    
    return ret;
}

