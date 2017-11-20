/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_queue.c
 @brief  : mqueue api adapt
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           2017-10-24   pengrui    Modify the api function and add the comments
           ...
******************************************************************************/
#include "osal_queue.h"

/******************************************************************************
*    Function: osal_queue_create
*    Descriptions: open the queue.
*    Paramters:
            queue_ptr            - the handle of the queue
            qname                - the name of queue.
            max_msgs             - the max msgs of the queue can coming in.
            msg_size             - the max msg size of each queue.
*    Return:
            = 0                  - success
            < 0                  - failed
*    Comments:    
******************************************************************************/
int osal_queue_create(osal_queue_t *queue_ptr, const char *qname, uint32_t max_msgs, uint32_t msg_size)
{
    int ret = 0;
    osal_mqd_t mqd;

    if(queue_ptr == NULL)
   	{
   	    ret = -ERR_INVAL;
	    goto ERR_EXIT;
    }

    /* flags = 0, block */
    ret = os_queue_create(&mqd, qname, max_msgs, msg_size, 0);
    if (ret)
    {    
        goto ERR_EXIT;
    }
    
    queue_ptr->mq_des = mqd;
    queue_ptr->msg_size = msg_size;
    
    ret = 0;

ERR_EXIT:

    return ret;
}

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
int osal_queue_delete(osal_queue_t queuefd)
{
    int ret = 0;
    
    ret = os_queue_delete(queuefd.mq_des);

    return ret;
}

/******************************************************************************
*    Function: osal_queue_recv
*    Descriptions: recv msg from queue.
*    Paramters:
            queuefd                - the handle of the queue.
            data                   - the data recv from queue.
            len                    - the len of the data.
            timeout                - wait time 0xFFFFFFFF : wait forerver
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_queue_recv(osal_queue_t queuefd, void *data, uint32_t *len, uint32_t timeout)
{
    int ret = 0;
    
    /* timeout 0xFFFFFFFF:block   0:  noblock */
    if (OSAL_BLOCK == timeout) 
    {
        ret = os_queue_recv(queuefd.mq_des, data, queuefd.msg_size, len);
    }else{
        ret = os_mq_timedrecv(queuefd.mq_des, data, queuefd.msg_size, len, timeout);
    }

    return ret;
}

/******************************************************************************
*    Function: osal_queue_send
*    Descriptions: send msg to the queue.
*    Paramters:
            queuefd                    - the handle of the queue.
            data                       - the data send to the queue.
            len                        - the length of the data.
            priority                   - the priority 
            timeout                    - the wait time: 0xFFFFFFFF wait forever 
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_queue_send(osal_queue_t queuefd, void *data, uint32_t len, uint32_t priority, uint32_t timeout)
{
    int ret = 0;

    if(len > queuefd.msg_size)
	{
	    ret = -ERR_INVAL;
	    goto ERR_EXIT;
	}
	
    /* timeout 0xFFFFFFFF:block   0:  noblock */
    if (OSAL_BLOCK == timeout) 
    {
        ret = os_queue_send(queuefd.mq_des, data, len, priority);
    }else{
        ret = os_mq_timedsend(queuefd.mq_des, data, len, priority, timeout);    
    }

ERR_EXIT:
	
    return ret;
}


