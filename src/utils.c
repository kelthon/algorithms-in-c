/**
 * @file linked_list.c
 * @brief Implementation of utils functions.
 *
 * This file contains the implementation of the linked list operations,
 * including insertion, removal, swapping, sorting, and iteration.
 */

#include <stdint.h>
#include <errno.h>
#include "utils.h"

/**
 * @brief Normalizes a negative index into a positive index relative to the
 * list head.
 *
 * This function returns a zero-based positive index or 0 if the index is
 * out of bounds.
 *
 * @param index The possibly negative index.
 * @param max The maximum index (i.e., the list length).
 * @returns A normalized zero-based index, or 0 if out of bounds (errno
 * set).
 */
uint64_t index_from_zero(int position, uint64_t max) {
  if (position >= 0) {
    return (uint64_t)position;
  }

  if (max == 0) {
    errno = EINVAL;
    return 0;
  }

  uint64_t distance_from_end = (uint64_t)(-position);

  // Prevents wrap around and invalid access.
  if (distance_from_end > max) {
    errno = EINVAL;
    return 0;
  };

  return max - distance_from_end;
}
