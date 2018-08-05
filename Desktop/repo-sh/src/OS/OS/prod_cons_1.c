#define _POSIX_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include "utils.h"

enum
{
	OFF,
	ON
};

void *WriteFunc(void *arg);
void *ReadFunc(void *arg);
void Handler(int signal_num);

int g_resource = 0;
int g_flag = ON;
pthread_t reader;
pthread_t writer;

int main(void)
{
	
	struct sigaction act;
	int status = 0;
	
	act.sa_handler = Handler;
	act.sa_flags = SIGINT;
	sigaction(SIGINT, &act, NULL);
	
	status = pthread_create(&writer, NULL, WriteFunc, NULL);
	if (0 != status)
	{
		PRINT_ERROR("pthread_create error");
	}	
	
	status = pthread_create(&reader, NULL, ReadFunc, NULL);
	if (0 != status)
	{
		PRINT_ERROR("pthread_create error");
	}

	pthread_join(writer, NULL);
	if (0 != status)
	{
		PRINT_ERROR("pthread_join error");
	}
	pthread_join(reader, NULL);
	if (0 != status)
	{
		PRINT_ERROR("pthread_join error");
	}
	
	printf("exit ok\n");
	return (0);
}

void *WriteFunc(void *arg)
{
	while (1)
	{
		if (g_flag)
		{
			++g_resource;
			g_flag = OFF;
		}
	pthread_testcancel();
	}
	return (NULL);
}

void *ReadFunc(void *arg)
{
	int prev;
	
	while (1)
	{
		if (!g_flag)
		{
			if (prev + 1  != g_resource) printf("error");
			prev = g_resource;
			g_flag = ON;
		}
		pthread_testcancel();
	}
	return (NULL);
}

void Handler(int signal_num)
{
	pthread_cancel(reader);
	pthread_cancel(writer);
	
	return;
}
