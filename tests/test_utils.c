#include <criterion/criterion.h>
#include <stdlib.h>
#include <errno.h>
#include "utils.h"

TestSuite(index_from_zero, .init = setup, .fini = teardown);

void setup(void) {
    errno = 0;
}

void teardown(void) {
}

Test(index_from_zero, negative_index_valid) {
    errno = 0;
    cr_assert_eq(index_from_zero(-1, 6), 5, "");
    cr_assert_eq(errno, 0, "");
}

Test(index_from_zero, negative_index_at_boundary) {
    errno = 0;
    cr_assert_eq(index_from_zero(-6, 6), 0, "");
    cr_assert_eq(errno, 0, "");
}

Test(index_from_zero, negative_index_out_of_bounds) {
    errno = 0;
    cr_assert_eq(index_from_zero(-7, 6), 0, "");
    cr_assert_eq(errno, EINVAL, "");
}

Test(index_from_zero, positive_index_valid) {
    errno = 0;
    cr_assert_eq(index_from_zero(0, 6), 0, "");
    cr_assert_eq(errno, 0, "");
}

Test(index_from_zero, positive_index_middle) {
    errno = 0;
    cr_assert_eq(index_from_zero(3, 6), 3, "");
    cr_assert_eq(errno, 0, "");
}

Test(index_from_zero, positive_index_at_boundary) {
    errno = 0;
    cr_assert_eq(index_from_zero(5, 6), 5, "");
    cr_assert_eq(errno, 0, "");
}

Test(index_from_zero, positive_index_out_of_bounds) {
    errno = 0;
    cr_assert_eq(index_from_zero(6, 6), 6, "");
    cr_assert_eq(errno, EINVAL, "");
}

Test(index_from_zero, max_zero) {
    errno = 0;
    cr_assert_eq(index_from_zero(0, 0), 0, "");
    cr_assert_eq(errno, EINVAL, "");
}

Test(index_from_zero, negative_max_zero) {
    errno = 0;
    cr_assert_eq(index_from_zero(-1, 0), 0, "");
    cr_assert_eq(errno, EINVAL, "");
}