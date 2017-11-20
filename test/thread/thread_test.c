/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : spinlock_test.c
 @brief  : test the thead api
 @author : pengrui
 @history:
           2017-10-24    pengrui    Created file
           ...
******************************************************************************/
#include "osal_thread.h"

#define TASK_TEST_STACK_SIZE (1024*8192)
#define TASK_TEST_PRIORITY   (TK_PRIO_DEFAULT + 11)  

void *thread_entry(void *arg)
{
    while(1)
    {
        sleep(1);
        fprintf(stderr, "%s %d\n", __FUNCTION__, __LINE__);
    }

    return (void *)NULL;
}

int main()
{
    int ret = 0;
    osal_task_t task_test;

    ret = osal_task_create(&task_test, "tk-test", thread_entry, NULL, TASK_TEST_STACK_SIZE, TASK_TEST_PRIORITY);
    if(ret)
    {
        fprintf(stderr, "osal_task_create() error\n");
    }
    
    while(1)
    {
        sleep(1000);
    }
}

