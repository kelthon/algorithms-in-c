#ifndef AIC_STACK_H_INCLUDED
#define AIC_STACK_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>

typedef struct LinkedQueue Queue;

Queue *stack_create();

void stack_push(Queue *stack, int index, void *value);

bool stack_peek(Queue *stack, int index, void *value_copy);

bool stack_pop(Queue *stack, void *value_copy);

size_t stack_length(Queue *stack);

Queue *stack_copy(Queue *stack);

void stack_clear(Queue *stack, void *(*clear_function)(void *item));
void stack_delete(Queue *stack, void *(*clear_function)(void *item));

#endif  // AIC_STACK_H_INCLUDED