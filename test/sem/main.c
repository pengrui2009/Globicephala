#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/sem.h>
#include <semaphore.h>

int osal_sem_open(sem_t **sem_ptr)
{
    int ret = 0;
    int value = 0;
    unsigned int init_cnt = 5;
    *sem_ptr = sem_open("test10", O_CREAT, S_IRWXU | S_IRWXG, init_cnt);
    if(SEM_FAILED == *sem_ptr)
    {
        fprintf(stderr, "sem_open error()\n");
        ret = -1;
    }

    sem_getvalue(*sem_ptr, &value);
    printf("value:%d\n", value);

    return ret;
}

int osal_sem_take(sem_t *sem_ptr)
{
	int ret = 0;
	int value = 0;
	ret = sem_wait(sem_ptr);
        if(ret)
	{
		ret = -1;
		goto ERR_EXIT;
	}

	
    	sem_getvalue(sem_ptr, &value);
    	printf("value:%d\n", value);
ERR_EXIT:

	return ret;
}

int main()
{
    int result = 0;
    sem_t *sem = NULL;
    int value = 0;
    
    result = osal_sem_open(&sem);
    if(result)
    {
	fprintf(stderr, "sem_open error()\n");
    }
    
    sem_getvalue(sem, &value);
    printf("value:%d\n", value);

	result = osal_sem_take(sem);
	if(result)
	{
		fprintf(stderr, "sem_take error\n");
	}	
	
    sem_getvalue(sem, &value);
    printf("value:%d\n", value);
    return 0;
}
