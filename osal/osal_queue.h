/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_queue.h
 @brief  : osal_queue.c header file
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           2017-10-24   pengrui    Add the comments
           ...
******************************************************************************/

#ifndef _OSAL_QUEUE_H_
#define _OSAL_QUEUE_H_


#include "os_core.h"
#include "osal_cmn.h"


typedef struct _osal_queue {
    osal_mqd_t mq_des;
    uint32_t msg_size;
} osal_queue_t;

/******************************************************************************
*	Function: osal_queue_create
*	Descriptions: open the queue.
*	Paramters:
			queue_ptr			- the handle of the queue
			qname 				- the name of queue.
			max_msgs			- the max msgs of the queue can coming in.
			msg_size			- the max msg size of each queue.
*	Return:
			NOT NULL			- the handle of queue
			NULL				- failed
*	Comments: 
******************************************************************************/
int osal_queue_create(osal_queue_t *queue_ptr, const char *qname, uint32_t max_msgs, uint32_t msg_size);

/******************************************************************************
*    Function: osal_queue_delete
*    Descriptions: close the queue.
*    Paramters:
            queuefd                - the handle of the queue.
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_queue_delete(osal_queue_t  queuefd);


/******************************************************************************
*	Function: osal_queue_recv
*	Descriptions: recv msg from queue.
*	Paramters:
			queuefd  			- the handle of the queue.
			data				- the data recv from queue.
			len					- the len of the data.
			timeout				- wait time
*	Return:
			= 0					- success
			< 0					- failed
*	Comments: 
******************************************************************************/
int osal_queue_recv(osal_queue_t queuefd, void *data, uint32_t *len, uint32_t timeout);

/******************************************************************************
*	Function: osal_queue_send
*	Descriptions: send msg to the queue.
*	Paramters:
			queuefd 				- the handle of the queue.
			data					- the data send to the queue.
			len						- the length of the data.
			priority				- the priority 
			timeout					- the wait time: -1 wait forever 
*	Return:
			= 0						- success
			< 0						- failed
*	Comments: 
******************************************************************************/
int osal_queue_send(osal_queue_t queuefd, void *data, uint32_t len, uint32_t priority, uint32_t timeout);


#endif
