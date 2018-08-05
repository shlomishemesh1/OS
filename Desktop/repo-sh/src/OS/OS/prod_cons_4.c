#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <semaphore.h>

#include "doubly_linked_list.h"
#include "utils.h"

#define THREAD_NUM		(100)
enum
{
	OFF,
	ON
};

void *WriteFunc(void *arg);
void *ReadFunc(void *arg);
void Handler(int signal_num);

sem_t sem;
pthread_mutex_t mutex;
pthread_t reader[THREAD_NUM];
pthread_t writer[THREAD_NUM];
size_t counter = 0;

int g_flag = 0;
int main(void)
{
	int status = 0;
	struct sigaction act = {0};
	dlist_t *dlist = NULL;
	int i = 0;
	
	act.sa_handler = Handler;
	act.sa_flags = SIGINT;
	sigaction(SIGINT, &act, NULL);
	
	status = pthread_mutex_init(&mutex, NULL);
	if (0 != status)
	{
		PRINT_ERROR("pthread_mutex_intit error");
		return (1);
	}
	
	status = sem_init(&sem, 0, 0);
	if (0 != status)
	{
		PRINT_ERROR("sem_intit error");
		return (1);
	}
	
	dlist = DlistCreate();
	if (NULL == dlist)
	{
		PRINT_ERROR("DlistCreate error");
		return (1);
	}
	
	for (i = 0; i < THREAD_NUM; ++i)
	{
		status = pthread_create(&writer[i], NULL, WriteFunc, dlist);
		if (0 != status)
		{
			PRINT_ERROR("pthread_create error");
			return (1);
		}	
	}
	
	for (i = 0; i < THREAD_NUM; ++i)
	{
		status = pthread_create(&reader[i], NULL, ReadFunc, dlist);
		if (0 != status)
		{
			PRINT_ERROR("pthread_create error");
			return (1);
		}
	}

	for (i = 0; i < THREAD_NUM; ++i)
	{
		pthread_join(writer[i], NULL);
		if (0 != status)
		{
			PRINT_ERROR("pthread_join error");
			return (1);
		}
		pthread_join(reader[i], NULL);
		if (0 != status)
		{
			PRINT_ERROR("pthread_join error");
			return (1);
		}
	}
	
	DlistDestroy(dlist);
	printf("exit ok\n");
	return (0);
}

void *WriteFunc(void *arg)
{
	dlist_t *dlist = (dlist_t *)arg;
	while (1)
	{
		pthread_mutex_lock(&mutex);
		DlistPushFront(dlist, (void *)counter);
		printf("writer tid: %02lu, count = %lu\n", pthread_self()%50, counter);
		++counter;
		sem_post(&sem);
		pthread_mutex_unlock(&mutex);

		if (g_flag == 1)
		{
			pthread_exit(NULL);
		}
	}
	return (NULL);
}

void *ReadFunc(void *arg)
{
	dlist_t *dlist = (dlist_t *)arg;
	
	while (1)
	{
		sem_wait(&sem);
		pthread_mutex_lock(&mutex);
		printf("\t\t\t\treader tid: %lu, data = %d, size = %lu\n", 
		pthread_self()%50, (int)(size_t)DlistPopBack(dlist), DlistCount(dlist));
		 
		pthread_mutex_unlock(&mutex);
		
		if (g_flag == 1)
		{
			pthread_exit(NULL);
		}
	}
	return (NULL);
}


void Handler(int signal_num)
{
	g_flag = 1;
	puts("in handler");
	return;
}
