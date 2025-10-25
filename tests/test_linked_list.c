#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <windows.h>

#include "linked_list.h"
#include "utils.h"

static int test_failures = 0;
static int total_tests = 0;
static int data_a = 10, data_b = 20, data_c = 30;

/**
 * @brief Executes a single test function and reports its status.
 * * @param test_func The function pointer to the test to be run.
 * @param test_name The name of the test for reporting.
 */
#define RUN_TEST(test_func, test_name)                           \
  do {                                                           \
    total_tests++;                                               \
    int initial_failures = test_failures;                        \
    test_func();                                                 \
    if (test_failures == initial_failures) {                     \
      printf("[SUCCESS] Test %d: %s\n", total_tests, test_name); \
    }                                                            \
  } while (0)

#define ASSERT_TRUE(condition, message)                                     \
  if (!(condition)) {                                                       \
    printf("[FAILURE] Test '%s' FAILED: %s (Line %d)\n", __func__, message, \
           __LINE__);                                                       \
    test_failures++;                                                        \
    return;                                                                 \
  }

#define ASSERT_FALSE(condition, message) ASSERT_TRUE(!(condition), message)

#define ASSERT_NULL(pointer, message) ASSERT_TRUE((pointer) == NULL, message)

#define ASSERT_NOT_NULL(pointer, message) \
  ASSERT_TRUE((pointer) != NULL, message)

#define ASSERT_EQUALS(expected, actual, message)                             \
  if ((expected) != (actual)) {                                              \
    printf(                                                                  \
        "[FAILURE] Test '%s' FAILED: %s (Expected: %zu, Actual: %zu) (Line " \
        "%d)\n",                                                             \
        __func__, message, (uint64_t)(expected), (uint64_t)(actual),         \
        __LINE__);                                                           \
    test_failures++;                                                         \
    return;                                                                  \
  }

#define ASSERT_IS_ERROR(condition, expected_errno, message)                  \
  if ((condition) || (errno != expected_errno)) {                            \
    printf(                                                                  \
        "[FAILURE] Test '%s' FAILED: %s (Expected errno: %d, Actual errno: " \
        "%d) (Line %d)\n",                                                   \
        __func__, message, expected_errno, errno, __LINE__);                 \
    test_failures++;                                                         \
    return;                                                                  \
  }

/**
 * @brief Tests list creation and basic properties.
 */
void test_list_creation_and_push() {
  List* list = list_create();

  // 1. Basic Creation
  ASSERT_NOT_NULL(list, "List creation should not return NULL.");
  ASSERT_EQUALS((uint64_t)0, (uint64_t)list_length(list),
                "New list length must be zero.");
  ASSERT_TRUE(list_is_empty(list), "New list must be reported as empty.");

  // 2. Simple Push Operation
  bool success = list_push(list, &data_a);  // List: [10]
  ASSERT_TRUE(success, "list_push should succeed with valid input.");
  ASSERT_EQUALS(1, list_length(list), "Length should be 1 after one push.");
  ASSERT_FALSE(list_is_empty(list), "List should not be empty after push.");

  // 3. Push NULL data (Invalid Input)
  errno = 0;
  success = list_push(list, NULL);
  ASSERT_FALSE(success, "Pushing NULL data should fail.");
  ASSERT_EQUALS(EINVAL, errno, "Pushing NULL data must set EINVAL.");

  // Assuming list_destroy exists to clean up memory
  // list_destroy(list);
}

/**
 * @brief Validates index_from_zero with all edge cases including the fallback
 * logic. Assumes index_from_zero(index, max) exists.
 */
void test_safe_index_behavior() {
  uint64_t max = 3;  // List of 3 elements (Indices 0, 1, 2)

  // 1. Valid positives
  ASSERT_EQUALS(0, index_from_zero(0, max), "Index 0 should be 0.");
  ASSERT_EQUALS(2, index_from_zero(2, max), "Index max-1 should be 2.");

  // 2. Invalid positives (Out of bounds for access: index 3)
  errno = 0;
  uint64_t out_of_bounds_pos = index_from_zero(3, max);
  ASSERT_EQUALS(max, out_of_bounds_pos,
                "Out of bounds positive index (3) should return max (3).");
  ASSERT_EQUALS(EINVAL, errno, "Out of bounds positive index must set EINVAL.");

  // 3. Invalid Negative
  errno = 0;
  ASSERT_EQUALS(2, index_from_zero(-1, max), "Index -1 should be 2.");
  ASSERT_EQUALS(0, index_from_zero(-3, max), "Index -max should be 0 (head).");
  ASSERT_EQUALS(0, errno, "Valid negative index should NOT set errno.");

  // 4. Extrem Negative (*Fallback* for 0 + Error)
  errno = 0;
  uint64_t extreme_neg = index_from_zero(-10, max);  // -10 > max=3
  ASSERT_EQUALS(0, extreme_neg, "Extreme negative index should FALLBACK to 0.");
  ASSERT_EQUALS(EINVAL, errno, "Extreme negative index must set EINVAL.");

  // 5. Empty List
  errno = 0;
  index_from_zero(1, 0);
  ASSERT_EQUALS(EINVAL, errno, "Empty list (max=0) should set EINVAL.");
}

/**
 * @brief Tests list_peek to ensure correct index handling and error
 * propagation.
 */
void test_list_peek_logic() {
  List* list = list_create();

  list_push(list, &data_a);  // [10]
  list_push(list, &data_b);  // [20, 10] (Head is 20)

  int* result = NULL;

  // 1. Peek at Head (Index 0)
  bool success = list_peek(list, 0, &result);
  ASSERT_TRUE(success, "Peek at index 0 should succeed.");
  ASSERT_EQUALS(data_b, *result,
                "Peek at index 0 should return the head data (20).");

  // 2. Peek with NULL output parameter
  errno = 0;
  success = list_peek(list, 0, NULL);
  ASSERT_FALSE(success, "Peek with NULL value_copy must fail.");
  ASSERT_EQUALS(EINVAL, errno, "NULL value_copy must set EINVAL.");

  // 3. Peek with Extreme Negative (Fallback to 0)
  errno = 0;
  success = list_peek(list, -10, &result);  // -10 falls back to index 0
  ASSERT_TRUE(success, "Extreme negative index should FALLBACK and succeed.");
  ASSERT_EQUALS(data_b, *result,
                "Fallback to 0 should return the head data (20).");
  ASSERT_EQUALS(
      EINVAL, errno,
      "Fallback must set EINVAL.");  // Propagates error from index_from_zero

  // list_destroy(list);
}

/**
 * @brief Tests insertion logic, particularly the DRY use of list_node_get(index
 * - 1).
 */
void test_list_insert_dry_logic() {
  List* list = list_create();  // []

  list_push(list, &data_c);  // [30]
  list_push(list, &data_a);  // [10, 30] (Length 2)

  int* result = NULL;

  // 1. Insert at Middle (Index 1) -> Must call get(0)
  bool success = list_insert(list, 1, &data_b);  // [10, 20, 30]
  ASSERT_TRUE(success, "Insert at index 1 failed.");
  ASSERT_EQUALS(3, list_length(list), "Length must be 3 after insert.");
  list_peek(list, 1, &result);
  ASSERT_EQUALS(data_b, *result, "Data at index 1 must be 20.");

  // 2. Insert at End (Index 3) -> Must call get(2)
  success = list_insert(list, 3, &data_c);  // [10, 20, 30, 30]
  ASSERT_TRUE(success, "Insert at index 3 (end) failed.");
  ASSERT_EQUALS(4, list_length(list), "Length must be 4 after insert at end.");
  list_peek(list, 3, &result);
  ASSERT_EQUALS(data_c, *result, "Data at index 3 must be 30.");

  // 3. Insert Out of Bounds (Index 5)
  errno = 0;
  success = list_insert(list, 5, &data_a);
  ASSERT_FALSE(success, "Insert out of bounds (5) must fail.");
  ASSERT_EQUALS(EINVAL, errno, "Out of bounds insert must set EINVAL.");
  ASSERT_EQUALS(4, list_length(list),
                "List length must not change after failed insert.");

  // 4. Insert Extreme Negative (Fallback 0)
  errno = 0;
  success =
      list_insert(list, -10, &data_b);  // -10 falls back to 0 -> list_push
  ASSERT_TRUE(success, "Extreme negative insert should fall back to push.");
  ASSERT_EQUALS(EINVAL, errno, "Extreme negative insert must set EINVAL.");
  list_peek(list, 0, &result);
  ASSERT_EQUALS(data_b, *result,
                "Extreme negative insert must put data at head.");

  // list_destroy(list);
}

// ---------------------------------------------------------------------
// CTEST INTERFACE
// ---------------------------------------------------------------------

int main() {
  printf("--- Initiating Linked List CTest Suite ---\n");

  RUN_TEST(test_list_creation_and_push, "Basic Creation and Push");
  RUN_TEST(test_safe_index_behavior, "Safe Index: Normalization and Fallback");
  RUN_TEST(test_list_peek_logic, "List Peek: Boundary and Error Handling");
  RUN_TEST(test_list_insert_dry_logic, "List Insert: DRY Logic and Edge Cases");

  if (test_failures > 0) {
    printf("\n[CTEST SUMMARY] %d/%d test(s) FAILED.\n", test_failures,
           total_tests);
    return EXIT_FAILURE;  // CTest code: 1
  } else {
    printf("\n[CTEST SUMMARY] All %d tests PASSED.\n", total_tests);
    return EXIT_SUCCESS;  // CTest code: 0
  }
}
