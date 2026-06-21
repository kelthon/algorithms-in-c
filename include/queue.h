#ifndef AIC_QUEUE_H_INCLUDED
#define AIC_QUEUE_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>

typedef struct LinkedQueue Queue;

Queue *queue_create();

void queue_add(Queue *queue, int index, void *value);

bool queue_peek(Queue *queue, int index, void *value_copy);

bool queue_remove(Queue *queue, void *value_copy);

size_t queue_length(Queue *queue);

Queue *queue_copy(Queue *queue);

void queue_clear(Queue *queue, void *(*clear_function)(void *item));
void queue_delete(Queue *queue, void *(*clear_function)(void *item));

#endif  // AIC_QUEUE_H_INCLUDED