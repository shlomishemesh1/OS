#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <semaphore.h>
#include <stdlib.h>
#include "utils.h"

enum
{
	OFF,
	ON
};

void *WriteFunc(void *arg);
void *ReadFunc(void *arg);
void Handler(int signal_num);

sem_t sem;
pthread_cond_t cond_read;
pthread_mutex_t mutex;

size_t g_read_counter = 0;
size_t counter = 0;
int num_of_readers = 10;
volatile int g_flag = 1;

int main(int argc, char *argv[])
{
	int status = 0;
	struct sigaction act = {0};
	int i = 0;
	pthread_t *reader;
	pthread_t writer;
	
	if (1 < argc)
	{
		num_of_readers = atoi(argv[1]);
	}
	
	reader = malloc(sizeof(pthread_t *) * num_of_readers);
	if (NULL == reader)
	{
		PRINT_ERROR("malloc error");
		return (1);
	}
	
	act.sa_handler = Handler;
	act.sa_flags = SIGINT;
	sigaction(SIGINT, &act, NULL);
	
	status = pthread_mutex_init(&mutex, NULL);
	if (0 != status)
	{
		PRINT_ERROR("pthread_mutex_intit error");
		return (1);
	}
	
	status = pthread_cond_init(&cond_read, NULL);
	if (0 != status)
	{
		PRINT_ERROR("cond_intit error");
		return (1);
	}

	status = sem_init(&sem, 0, 0);
	if (0 != status)
	{
		PRINT_ERROR("sem_intit error");
		return (1);
	}

	status = pthread_create(&writer, NULL, WriteFunc, NULL);
	if (0 != status)
	{
		PRINT_ERROR("pthread_create error");
		return (1);
	}	

	
	for (i = 0; i < num_of_readers; ++i)
	{
		status = pthread_create(&reader[i], NULL, ReadFunc, NULL);
		if (0 != status)
		{
			PRINT_ERROR("pthread_create error");
			return (1);
		}
	}

	
	pthread_join(writer, NULL);
	if (0 != status)
	{
		PRINT_ERROR("pthread_join error");
		return (1);
	}
	for (i = 0; i < num_of_readers; ++i)
	{
		pthread_join(reader[i], NULL);
		if (0 != status)
		{
			PRINT_ERROR("pthread_join error");
			return (1);
		}
	}
	
	pthread_cond_destroy(&cond_read);
	free(reader);
	
	printf("exit ok\n");
	return (0);
}

void *WriteFunc(void *arg)
{
	int i = 0;
	while (g_flag)
	{
		sem_wait(&sem);
		pthread_mutex_lock(&mutex);	
			g_read_counter = 0;
			++counter;
			printf("writer tid: %lu, data = %lu\n", 
									pthread_self(), counter);
			pthread_cond_broadcast(&cond_read);
		pthread_mutex_unlock(&mutex);

	}

	pthread_cond_broadcast(&cond_read);

	return (NULL);
}

void *ReadFunc(void *arg)
{
	int x = counter;
	
	while (g_flag)
	{
		pthread_mutex_lock(&mutex);
			x = counter;
			++g_read_counter;
			if (num_of_readers == g_read_counter)
			{
				sem_post(&sem);
			}
			
			while (counter == x) 
			{
				pthread_cond_wait(&cond_read, &mutex);
			}
		pthread_mutex_unlock(&mutex);
			
		printf("\t\t\t\treader tid: %lu, data = %lu\n", 
								pthread_self(), counter);
		
	}
	
	x = !counter;
	sem_post(&sem);

	return (NULL);
}


void Handler(int signal_num)
{
	g_flag = 0;
	puts("in handler");
	return;
}
