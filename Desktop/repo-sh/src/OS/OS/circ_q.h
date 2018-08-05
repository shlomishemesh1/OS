
#ifndef __CIRC_Q_H
#define __CIRC_Q_H

typedef struct queue queue_t;

queue_t *CircQCreate(size_t size);

void CircQEnqueue(queue_t *queue, unsigned int data);

unsigned int CircQDequque(queue_t *queue);

void CircQDestroy(queue_t *queue);


#endif
