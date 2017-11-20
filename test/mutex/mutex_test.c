/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : msg_recv.c
 @brief  : test the api function of osal mutex.
 @author : pengrui
 @history:
           2017-10-24    pengrui    Created file
           ...
******************************************************************************/

#include "osal_mutex.h"

int main()
{
    int result = 0;
    osal_mutex_t mutex;

        result = osal_mutex_create(&mutex);
    if(result)
    {
        fprintf(stderr, "osal_mutex_create() error:%d\n", result);
        goto ERR_EXIT;
    }
    
    result = osal_mutex_lock(&mutex);
    if(result)
    {
        fprintf(stderr, "osal_mutex_trylock() error:%d\n", result);
        goto ERR_EXIT;
    }
    
    result = osal_mutex_trylock(&mutex);
    if(result)
    {
        fprintf(stderr, "osal_mutex_trylock() error:%d\n", result);
        goto ERR_EXIT;
    }

    result = osal_mutex_unlock(&mutex);
    if(result)
    {
        fprintf(stderr, "osal_mutex_unlock() error:%d\n", result);
        goto ERR_EXIT;
    }

    result = osal_mutex_delete(&mutex);
    if(result)
    {
        fprintf(stderr, "osal_mutex_delete() error:%d\n", result);
        goto ERR_EXIT;
    }
        
#if 0
    result = pthread_mutex_init(&mutex, NULL);
    if(result)
    {
        fprintf(stderr, "pthread_mutex_init() error:%d\n", result);
        goto ERR_EXIT;
    }

    result = pthread_mutex_lock(&mutex);
    if(result)
    {
        fprintf(stderr, "pthread_mutex_lock() error:%d\n", result);
        goto ERR_EXIT;
    }

    result = pthread_mutex_trylock(&mutex);
    if(result)
    {
        fprintf(stderr, "pthread_mutex_trylock() error:%d\n", result);
        goto ERR_EXIT;
    }

    result = pthread_mutex_unlock(&mutex);
    if(result)
    {
        fprintf(stderr, "pthread_mutex_unlock() error:%d\n", result);
        goto ERR_EXIT;
    }

#endif
ERR_EXIT:

    return result;
}
