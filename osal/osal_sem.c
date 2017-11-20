/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : osal_sem.c
 @brief  : posix Semaphore api(memory-shared and named semaphore)
 @author : wanglei
 @history:
           2015-8-25    wanglei    Created file
           2017-10-24   pengrui    Add the comments
           ...
******************************************************************************/
#include "osal_sem.h"

/******************************************************************************
*    Function: osal_sem_open
*    Descriptions: open named semaphore.
*    Paramters:
            sem_ptr              - the handle of the sem.
            name                 - the name of the sem, such as /semtest
            count                - the init value of sem.
*    Return:
            = 0                  - success
            < 0                  - failed
*    Comments: 
******************************************************************************/
int osal_sem_open(osal_sem_t **sem_ptr, const char* name, uint32_t count)
{
    int result = 0;

    result = os_sem_open(sem_ptr, name, count);
    if(result)
    {
        goto ERR_EXIT;
    }

ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    Function: osal_sem_close
*    Descriptions: close the named sem.
*    Paramters:
            name                   - the name of the sem
            sem_ptr                - the handle of the sem
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_sem_close(const char *name, osal_sem_t *sem_ptr)
{
    int result = 0;
    
    result = os_sem_close(sem_ptr);

    result = os_sem_unlink(name);

    return result; 

}

/******************************************************************************
*    Function: osal_sem_create
*    Descriptions: open the memory-shared sem.
*    Paramters:
            sem_ptr               - the handle of the sem
            count                 - the init value of sem 
*    Return:
            = 0                   - success
            < 0                   - failed
*    Comments: 
******************************************************************************/
int osal_sem_create(osal_sem_t *sem_ptr, uint32_t count)
{
    int result = 0;

    result = os_sem_init(sem_ptr, 0, count);
    if(result)
    {  
        goto ERR_EXIT;
    }
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: osal_sem_delete
*    Descriptions: close the memory-shared sem.
*    Paramters:
            sem_ptr                - the handle of the sem
*    Return:
            = 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int osal_sem_delete(osal_sem_t *sem_ptr)
{
    int result = 0;

    result = os_sem_destroy(sem_ptr);
    
    return result; 
}

/******************************************************************************
*    Function: osal_sem_take
*    Descriptions: take the sem, sem count decrement one.
*    Paramters:
            sem_ptr                    - the handle of the sem
            wait_time                  - the msg type
*    Return:
            = 0                        - success
            < 0                        - failed
*    Comments: 
******************************************************************************/
int osal_sem_take(osal_sem_t *sem_ptr, uint32_t wait_time)
{
    int result = 0;
    
    result = os_sem_wait(sem_ptr, wait_time);

    return result;
}

/******************************************************************************
*    Function: osal_sem_release
*    Descriptions: release the sem.
*    Paramters:
            sem_ptr                   - the handle of the msg
*    Return:
            = 0                       - success
            < 0                       - failed
*    Comments: 
******************************************************************************/
int osal_sem_release(osal_sem_t *sem_ptr)
{
    int result = 0;
    
    result = os_sem_post(sem_ptr);
    
    return result; 
}
