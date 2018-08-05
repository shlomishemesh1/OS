#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <semaphore.h>

#include "circ_q.h"
#include "utils.h"

#define THREAD_NUM			(100)
#define NUM_OF_ELEMENTS		(50)

enum
{
	OFF,
	ON
};

void *WriteFunc(void *arg);
void *ReadFunc(void *arg);
void Handler(int signal_num);

sem_t is_empty_sem;
sem_t is_full_sem;

pthread_mutex_t mutex;
pthread_t reader[THREAD_NUM];
pthread_t writer[THREAD_NUM];

size_t counter = 0;
volatile int g_flag = 0;

int main(void)
{
	int status = 0;
	struct sigaction act = {0};
	queue_t *queue = NULL;
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
	
	status = sem_init(&is_empty_sem, 0, 0);
	if (0 != status)
	{
		PRINT_ERROR("sem_intit error");
		return (1);
	}
	
	status = sem_init(&is_full_sem, 0, NUM_OF_ELEMENTS);
	if (0 != status)
	{
		PRINT_ERROR("sem_intit error");
		return (1);
	}
	
	queue = CircQCreate(NUM_OF_ELEMENTS);
	if (NULL == queue)
	{
		PRINT_ERROR("CircQCreate error");
		return (1);
	}
	
	for (i = 0; i < THREAD_NUM; ++i)
	{
		status = pthread_create(&writer[i], NULL, WriteFunc, queue);
		if (0 != status)
		{
			PRINT_ERROR("pthread_create error");
			return (1);
		}	
	}
	
	for (i = 0; i < THREAD_NUM; ++i)
	{
		status = pthread_create(&reader[i], NULL, ReadFunc, queue);
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
	
	CircQDestroy(queue);
	sem_destroy(&is_empty_sem);
	sem_destroy(&is_full_sem);
	
	pthread_mutex_destroy(&mutex);
	
	printf("exit ok\n");
	return (0);
}

void *WriteFunc(void *arg)
{
	queue_t *queue = (queue_t *)arg;
	
	while (1)
	{
		sem_wait(&is_full_sem);
		pthread_mutex_lock(&mutex);
		CircQEnqueue(queue, counter);
		printf("writer tid: %02lu, count = %lu\n", pthread_self()%50, counter);
		++counter;
		sem_post(&is_empty_sem);
		
		pthread_mutex_unlock(&mutex);

		if (g_flag == 1)
		{
			return (NULL);
		}
	}
	return (NULL);
}

void *ReadFunc(void *arg)
{
	queue_t *queue = (queue_t *)arg;
	
	while (1)
	{
		sem_wait(&is_empty_sem);
		pthread_mutex_lock(&mutex);
		printf("\t\t\t\treader tid: %lu, data = %u\n", 
		pthread_self()%50, CircQDequque(queue));
		
		sem_post(&is_full_sem);
		pthread_mutex_unlock(&mutex);
		
		if (g_flag == 1)
		{
			return (NULL);
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
