/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : sem_unlock.c
 @brief  : test the named sem api
 @author : pengrui
 @history:
           2017-10-24   pengrui    create file
           ...
******************************************************************************/
#include "osal_sem.h"

int main(void)
{
	int result = 0;
	osal_sem_t *sem_ptr;
	int value = 0;

	result = osal_sem_open(&sem_ptr, "sem-test",  0);
	if(result)
	{
		fprintf(stderr, "osal_sem_open() error\n");
		goto ERR_EXIT;
	}

	sem_getvalue(sem_ptr, &value);
	printf("sem value:%d\n", value);	

	result = osal_sem_release(sem_ptr);
	if(result)
	{
		fprintf(stderr, "osal_sem_take() ERROR\n");
		goto ERR_EXIT;
	}
	
	sem_getvalue(sem_ptr, &value);
	printf("sem value:%d\n", value);	

	//result = osal_sem_close("test", semfd);
	if(result)
	{
		fprintf(stderr, "osal_sem_close()\n");
		goto ERR_EXIT;
	}
ERR_EXIT:

	return result;
}

