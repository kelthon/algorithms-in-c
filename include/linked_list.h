/**
 * @file linked_list.h
 * @brief Module for a dynamic singly linked list.
 *
 * This module provides basic operations for handling a linked list,
 * including insertion, removal, swapping, sorting, and iteration.
 *
 * Functions allow dynamic memory management and flexible element manipulation,
 * making it suitable for various use cases requiring sequential storage.
 *
 * @author Kelthon
 * @date 2025-06-02
 */

#ifndef AIC_LINKED_LIST_H
#define AIC_LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct LinkedList List;

List* list_create();

bool list_push(List* list, void* value);
bool list_insert(List* list, int index, void* value);
bool list_append(List* list, void* value);

bool list_peek(const List* list, int position, void* peeked_value);

bool list_pop(const List* list, void* popped_value);
bool list_get(const List* list, int index, void* got_value);
bool list_remove(List* list, void* target_value,
                 int* (*compare_function)(void* target_value,
                                          void* current_value),
                 void* removed_value);

bool list_swap(const List* list, int occurrence_index, int destination_index);

uint64_t list_length(const List* list);

bool list_is_empty(const List* list);

void list_reverse(const List* list);

void list_sort(const List* list,
               const void* (*sort_function)(void* previous, void* next,
                                            uint64_t index));

void list_foreach(const List* list,
                  const void* (*iterator_function)(void* previous, void* next,
                                                   uint64_t index));

void* list_reduce(List* list,
                  void* (*reduce_function)(void* previous, void* next,
                                           uint64_t index));

List* list_filter(const List* list,
                  const void* (*filter_function)(void* previous, void* next,
                                                 uint64_t index));

List* list_map(const List* list,
               const void* (*map_function)(void* previous, void* next,
                                           uint64_t index));

List* list_copy(const List* list);
List* list_from(uint64_t argc, const void* argv[]);

void list_clear(const List* list, void* (*clear_function)(void* item));
void list_delete(List* list, void* (*clear_function)(void* item));

#endif  // AIC_LINKED_LIST_H
