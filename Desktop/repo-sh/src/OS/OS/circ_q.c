#include <stdlib.h>
#include "circ_q.h"
#include "../utils/utils.h"

struct queue
{
	unsigned int *start;
	unsigned int *end;
	unsigned int *reader;
	unsigned int *writer;
};

queue_t *CircQCreate(size_t size)
{
	queue_t *queue = malloc(sizeof(queue_t));
	if (NULL == queue)
	{
		PRINT_ERROR("create error");
		return (NULL);
	}
	
	queue->start = malloc(sizeof(unsigned int) * size);
	if (NULL == queue->start)
	{
		PRINT_ERROR("create error");
		free(queue);
		return (NULL);
	}
	
	queue->end = queue->start + size;
	queue->reader = queue->writer = queue->start;
	
	return (queue);
}

void CircQEnqueue(queue_t *queue, unsigned int data)
{
	*queue->writer = data;
	
	++queue->writer;
	if (queue->writer == queue->end)
	{
		queue->writer = queue->start;
	}
	
	return;
}

unsigned int CircQDequque(queue_t *queue)
{
	unsigned int return_value = *queue->reader;
	
	++queue->reader;
	if (queue->reader == queue->end)
	{
		queue->reader = queue->start;
	}
	
	return (return_value);
}

void CircQDestroy(queue_t *queue)
{
	free(queue->start);
	free(queue);
	return;
}


