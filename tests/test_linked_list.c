#include <criterion/criterion.h>
#include <stdlib.h>
#include "linked_list.h"

Test(linked_list, create_list) {
  List* list = list_create();
  cr_assert_not_null(list, "List should be created successfully");
  cr_assert_eq(list_length(list), 0, "New list should be empty");
  cr_assert_true(list_is_empty(list), "list_is_empty should return true");
  list_delete(list);
}

Test(linked_list, push_single_element) {
  List* list = list_create();
  int value = 42;

  bool result = list_push(list, &value);
  cr_assert_true(result, "Push should succeed");
  cr_assert_eq(list_length(list), 1, "List should have 1 element");

  int retrieved;
  list_peek(list, 0, &retrieved);
  cr_assert_eq(retrieved, 42, "Value should be 42");

  list_delete(list);
}

Test(linked_list, push_multiple_elements) {
  List* list = list_create();
  int values[] = {10, 20, 30};

  for (int i = 0; i < 3; i++) {
    bool result = list_push(list, &values[i]);
    cr_assert_true(result, "Push should succeed");
  }

  cr_assert_eq(list_length(list), 3, "List should have 3 elements");
  list_delete(list);
}

Test(linked_list, append_element) {
  List* list = list_create();
  int value1 = 10;
  int value2 = 20;

  list_push(list, &value1);
  list_append(list, &value2);

  cr_assert_eq(list_length(list), 2, "List should have 2 elements");
  list_delete(list);
}

Test(linked_list, pop_element) {
  List* list = list_create();
  int value = 42;

  list_push(list, &value);
  int popped;
  bool result = list_pop(list, &popped);

  cr_assert_true(result, "Pop should succeed");
  cr_assert_eq(popped, 42, "Popped value should be 42");
  cr_assert_true(list_is_empty(list), "List should be empty");

  list_delete(list);
}

Test(linked_list, get_by_index) {
  List* list = list_create();
  int values[] = {10, 20, 30};

  for (int i = 0; i < 3; i++) {
    list_push(list, &values[i]);
  }

  int retrieved;
  bool result = list_get(list, 1, &retrieved);
  cr_assert_true(result, "Get should succeed");
  cr_assert_eq(retrieved, 20, "Value at index 1 should be 20");

  list_delete(list);
}

Test(linked_list, insert_at_index) {
  List* list = list_create();
  int v1 = 10, v2 = 30, v_insert = 20;

  list_push(list, &v1);
  list_push(list, &v2);
  list_insert(list, 1, &v_insert);

  cr_assert_eq(list_length(list), 3, "List should have 3 elements");

  int retrieved;
  list_get(list, 1, &retrieved);
  cr_assert_eq(retrieved, 20, "Inserted value should be at index 1");

  list_delete(list);
}

Test(linked_list, list_from) {
  int values[] = {5, 10, 15, 20};
  const void* argv[] = {&values[0], &values[1], &values[2], &values[3]};

  List* list = list_from(4, argv);
  cr_assert_not_null(list, "list_from should create a list");
  cr_assert_eq(list_length(list), 4, "List should have 4 elements");

  list_delete(list);
}

Test(linked_list, list_copy) {
  List* original = list_create();
  int values[] = {1, 2, 3};

  for (int i = 0; i < 3; i++) {
    list_push(original, &values[i]);
  }

  List* copy = list_copy(original);
  cr_assert_not_null(copy, "Copy should be created");
  cr_assert_eq(list_length(copy), list_length(original), "Copy should have same length");

  list_delete(original);
  list_delete(copy);
}
