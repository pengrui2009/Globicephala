/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_msg.h
 @brief  : the head file of msg.
 @author : pengrui
 @history:
           2017-10-19    pengrui    Created file
           ...
******************************************************************************/
#ifndef _OSAL_MSG_H_
#define _OSAL_MSG_H_ 

#include "os_core.h"
#include "osal_cmn.h"

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
int osal_msg_open(osal_msg_t *msgfd_ptr, const char * key_name_ptr, int key_id);

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
int osal_msg_close(osal_msg_t msgfd);

/******************************************************************************
*    Function: osal_msg_send
*    Descriptions: config pcm and mixer device.
*    Paramters:
            fd                       - the handle fd of the msg
            type                     - the msg type
            data_ptr                 - the msg data 
            data_len                 - the length of msg data
            timeout                  - wait time 0:BLOCK 1:NOBLOCK
*    Return:
            = 0                      - success
            -ERR_FULLMSG             - msg queue is full
            < 0                      - failed
*    Comments: 
******************************************************************************/
int osal_msg_send(osal_msg_t msgfd, long type, void *data_ptr, uint32_t data_len, uint16_t timeout);

/******************************************************************************
*    Function: osal_msg_recv
*    Descriptions: config pcm and mixer device.
*    Paramters:
            fd                     - the handle fd of the XSI msg
            type                   - the msg type we need recv 0: recv all 
            data_ptr               - the msg data recv
            data_len               - the length of msg data
            timeout                - wait time 0:BLOCK 1:NOBLOCK
*    Return:
            = 0                    - the length of the data
            -ERR_NOMSG             - there is no such type msg int the msg queue 
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_msg_recv(osal_msg_t msgfd, long *type, void *data_ptr, uint32_t *data_len, uint16_t timeout);

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
int osal_msg_clear(osal_msg_t msgfd);

#endif /* _OSAL_MSG_H_ */
