/**
 * @file linked_list.c
 * @brief Implementation of a dynamic singly linked list.
 *
 * This file contains the implementation of the linked list operations,
 * including insertion, removal, swapping, sorting, and iteration.
 */

#include "utils.h"
#include "linked_list.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @struct ListNode
 * @brief Represents a a node in the linked list.
 *
 * Each node stores a reference to the next node and holds a pointer
 * to the associated data.
 */
typedef struct ListNode {
  /**
   * @var ListNode::next
   * @brief Pointer to the next node in the linked list
   *
   * If `NULL`, it indicates the end of the list.
   */
  struct ListNode* next;

  /**
   * @var ListNode::data
   * @brief Pointer to the stored data.
   *
   * The data type is void* to support generic data storage.
   */
  void* data;
} ListNode;

/**
 * @struct LinkedList
 * @brief Represents a dynamic singly linked list.
 *
 * This structure maintains a reference to the first node (`head`) and tracks
 * the number of elements (`length`) stored in the list.
 */
struct LinkedList {
  /**
   * @var LinkedList::head
   * @brief Pointer to the first node in the list.
   *
   * If `NULL`, the list is empty.
   */
  ListNode* head;

  /**
   * @var LinkedList::length
   * @brief The number of elements in the list.
   */
  uint64_t length;
};

/**
 * @brief Creates a new list node with the given data.
 *
 * This function is private and only used internally for node allocation.
 *
 * @param data Pointer to the data to be stored in the node.
 * @param next Pointer to the next node in the list.
 * @return Pointer to the newly allocated list node, or NULL on memory failure.
 */
static ListNode* list_node_create(void* data, ListNode* next) {
  ListNode* item = (ListNode*)malloc(sizeof(ListNode));

  if (item == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  item->data = data;
  item->next = next;

  return item;
}

/**
 * @brief Validates if a list and a value pointer are non-NULL.
 *
 * This function is private and used internally to check function input
 * integrity.
 *
 * @param list Pointer to the list.
 * @param value Pointer to the data to be stored/used.
 * @return true if both list and value are valid (non-NULL), false otherwise.
 */
static bool list_input_validate(const List* list, void* value) {
  if (list == NULL || value == NULL) {
    errno = EINVAL;
    return false;
  }

  return true;
}

/**
 * @brief Retrieves the node at the specified index.
 *
 * This function assumes the 'index' is within the valid range [0, list->length
 * - 1]. It is meant for internal use after index_from_zero validation.
 *
 * @param list Pointer to the source list. Assumed non-NULL and non-empty.
 * @param index The zero-based index of the target node (must be valid).
 * @return Pointer to the indexed ListNode, or NULL if an unexpected error
 * occurs.
 */
static ListNode* list_node_get(const List* list, uint64_t index) {
  if (list_is_empty(list)) {
    errno = EINVAL;
    return NULL;
  }

  if (index >= list->length) {
    errno = EINVAL;
    return NULL;
  }

  uint64_t current_index = 0;
  ListNode* node = list->head;

  while (current_index < index && node != NULL) {
    node = node->next;
    current_index++;
  }

  if (node == NULL || current_index != index) {
    errno = EINVAL;
    return NULL;
  }

  return node;
}

static int list_node_find(const List* list, void* value,
                          int* (*compare_function)(void* target_value,
                                                   void* current_value),
                          ListNode* previous_node) {
  if (list_is_empty(list)) {
    errno = EINVAL;
    return -1;
  }

  int current_index = 0;
  ListNode* node = list->head;
  ListNode* prev_node = NULL;

  while (compare_function(value, node->data) != 0 && node != NULL) {
    prev_node = node;
    node = node->next;
    current_index++;
  }

  if (node == NULL) {
    errno = EINVAL;
    return -1;
  }

  if (previous_node != NULL) {
    *(ListNode*)previous_node = *prev_node;
  }

  return current_index;
}

static void* list_node_remove(ListNode** node) {
  void* value_ptr;
  if (*node != NULL) {
    value_ptr = (*node)->data;
    free(*node);
    *node = NULL;
  }

  return value_ptr;
}

/**
 * @brief Allocates and initializes a new linked list.
 *
 * Creates an empty list with no elements and sets the head pointer to `NULL`.
 *
 * @return Pointer to the newly allocated linked list.
 */
List* list_create() {
  List* list = (List*)malloc(sizeof(List));

  if (list == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  list->head = NULL;
  list->length = 0;

  return list;
}

/**
 * @brief Inserts a value at the beginning of the linked list.
 *
 * This function creates a new node with the given data and places
 * it at the front of the list, updating the head pointer accordingly.
 *
 * @param list Pointer to the linked list. Must not be `NULL`.
 * @param value Pointer to the data to be stored in the new node. Must not be
 * `NULL`.
 * @return `true` if the operation was successful, `false` if the list is
 * invalid or memory allocation failed.
 */
bool list_push(List* list, void* value) {
  ListNode* new_node;

  if (!list_input_validate(list, value)) {
    return false;
  }

  new_node = list_node_create(value, list->head);

  if (new_node == NULL) {
    return false;
  }

  list->head = new_node;
  list->length++;

  return true;
}

/**
 * @brief Inserts a value at a specified index in the linked list.
 *
 * @param list Pointer to the linked list.
 * @param position The zero-based position for insertion.
 * @param value Pointer to the data to be stored.
 * @return true if successful, false if input is invalid, index is out of
 * bounds, or memory allocation fails.
 */
bool list_insert(List* list, int position, void* value) {
  ListNode *new_node, *prev_node;
  if (!list_input_validate(list, value)) return false;

  uint64_t index = index_from_zero(position, list->length);

  if (index == 0) {
    return list_push(list, value);
  }

  if (index == list->length) {
    return list_append(list, value);
  }

  prev_node = list_node_get(list, index_from_zero(position - 1, list->length));
  if (prev_node == NULL) {
    errno = EINVAL;
    return false;
  }

  new_node = list_node_create(value, prev_node->next);
  if (new_node == NULL) {
    return false;
  }

  prev_node->next = new_node;

  return true;
}

/**
 * @brief Appends a value to the end (tail) of the linked list.
 *
 * @param list Pointer to the linked list.
 * @param value Pointer to the data to be stored.
 * @return true if successful, false otherwise.
 */
bool list_append(List* list, void* value) {
  uint64_t index;
  ListNode *new_node, *prev_node;
  int last_item_index = -1;

  if (!list_input_validate(list, value)) return false;

  new_node = list_node_create(value, NULL);
  if (new_node == NULL) {
    return false;
  }

  index = index_from_zero(last_item_index - 1, list->length);

  prev_node = list_node_get(list, index);
  if (prev_node == NULL) {
    return false;
  }

  prev_node->next = new_node;

  return true;
}

/**
 * @brief Retrieves the data pointer from the node at the specified index.
 *
 * The caller is responsible for casting the returned void* to the correct type.
 *
 * @param list Pointer to the list.
 * @param position The zero-based position of the node.
 * @param peeked_value A pointer to the stored data
 * @return true if value was found, or false if the index is out
 * of bounds or the list is invalid.
 */
bool list_peek(const List* list, int position, void* peeked_value) {
  ListNode* node;

  if (list_is_empty(list) || peeked_value == NULL) {
    errno = EINVAL;
    return false;
  }

  uint64_t index = index_from_zero(position, list->length);

  node = list_node_get(list, index);
  if (node == NULL) {
    errno = EINVAL;
    return false;
  }

  *(void**)peeked_value = node->data;
  return true;
}

bool list_remove(List* list, void* target_value,
                 int* (*compare_function)(void* target_value,
                                          void* current_value),
                 void* removed_value) {
  if (list_is_empty(list)) {
    errno = EINVAL;
    return false;
  }

  ListNode* previous_node = list_node_create(NULL, NULL);
  if (previous_node == NULL) {
    return false;
  }

  if (list_node_find(list, target_value, compare_function, previous_node) ==
      -1) {
    return false;
  }

  ListNode* node = previous_node->next;
  previous_node->next = node->next;
  node->next = NULL;

  *(void**)removed_value = list_node_remove(&node);

  return true;
}

/**
 * @brief Gets the number of elements in a list.
 *
 * @param list Pointer to the linked list (non-NULL).
 * @return the number of elements in a list.
 */
uint64_t list_length(const List* list) {
  if (list == NULL) {
    errno = EINVAL;
    return 0;
  }

  return list->length;
}

/**
 * @brief Checks if the linked list is either NULL or contains no elements.
 *
 * @param list Pointer to the linked list (can be NULL).
 * @return true if the list pointer is NULL or the list length is zero, false
 * otherwise.
 */
bool list_is_empty(const List* list) {
  return list == NULL || list->head == NULL;
}
