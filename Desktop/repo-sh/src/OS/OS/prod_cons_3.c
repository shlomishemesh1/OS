#include <pthread.h>
#include <stdio.h>
#include <signal.h>

#include "doubly_linked_list.h"
#include "utils.h"

#define THREAD_NUM		(50)
enum
{
	OFF,
	ON
};

void *WriteFunc(void *arg);
void *ReadFunc(void *arg);
void Handler(int signal_num);

pthread_mutex_t mutex;
pthread_t reader[THREAD_NUM];
pthread_t writer[THREAD_NUM];

int main(void)
{
	int status = 0;
	struct sigaction act;
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
	
	printf("exit ok\n");
	return (0);
}

void *WriteFunc(void *arg)
{
	size_t counter = 0;
	
	dlist_t *dlist = (dlist_t *)arg;
	while (1)
	{
		pthread_mutex_lock(&mutex);
		DlistPushFront(dlist, (void *)counter);
		printf("writer tid: %lu, count = %lu\n", pthread_self()%50, counter);
		++counter;

		pthread_mutex_unlock(&mutex);
		
		if (1 == g_flag)
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
		pthread_mutex_lock(&mutex);
		if (!DlistIsEmpty(dlist))
		{
			printf("\t\t\t\treader tid: %lu, data = %d, size = %lu\n",
					pthread_self() % 50, (int)(size_t)DlistPopFront(dlist), 
															DlistCount(dlist));
		}
		
		if (1 == g_flag)
		{
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&mutex);
	}
	return (NULL);
}

void Handler(int signal_num)
{
	g_flag = 1;

	return;
}

