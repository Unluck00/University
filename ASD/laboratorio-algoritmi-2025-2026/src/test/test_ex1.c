#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../ex1/headers/hybridsort.h"
#include "../ex1/headers/record.h"

static int int_compare(const void *a, const void *b) {
    int ia = *(const int *) a;
    int ib = *(const int *) b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

static void test_int_empty_array(void) {
    int *arr = NULL;
    hybridsort(arr, 0, sizeof(int), 10, int_compare);
}

static void test_int_single_element(void) {
    int arr[1] = {42};
    hybridsort(arr, 1, sizeof(int), 10, int_compare);
    assert(arr[0] == 42);
}

static void test_int_sorted_array(void) {
    int arr[5] = {1, 2, 3, 4, 5};
    hybridsort(arr, 5, sizeof(int), 10, int_compare);
    for (int i = 0; i < 5; ++i) {
        assert(arr[i] == i + 1);
    }
}

static void test_int_reverse_array(void) {
    int arr[5] = {5, 4, 3, 2, 1};
    hybridsort(arr, 5, sizeof(int), 2, int_compare); // k piccolo
    for (int i = 0; i < 5; ++i) {
        assert(arr[i] == i + 1);
    }
}

static void test_int_duplicates(void) {
    int arr[7] = {3, 1, 2, 3, 1, 2, 3};
    hybridsort(arr, 7, sizeof(int), 3, int_compare);
    int expected[7] = {1, 1, 2, 2, 3, 3, 3};
    for (int i = 0; i < 7; ++i) {
        assert(arr[i] == expected[i]);
    }
}

static void test_int_k_zero_quicksort_only(void) {
    int arr[5] = {5, 4, 3, 2, 1};
    hybridsort(arr, 5, sizeof(int), 0, int_compare);
    for (int i = 0; i < 5; ++i) {
        assert(arr[i] == i + 1);
    }
}

static void test_int_k_bigger_than_nitems(void) {
    int arr[5] = {2, 1, 5, 3, 4};
    hybridsort(arr, 5, sizeof(int), 10, int_compare); // 10 > 5
    int expected[5] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        assert(arr[i] == expected[i]);
    }
}

static int record_cmp_by_id_for_test(const void *a, const void *b) {
    const Record *ra = (const Record *) a;
    const Record *rb = (const Record *) b;
    if (ra->id < rb->id) return -1;
    if (ra->id > rb->id) return 1;
    return 0;
}

static int record_cmp_by_field1_for_test(const void *a, const void *b) {
    const Record *ra = (const Record *) a;
    const Record *rb = (const Record *) b;
    if (ra->field1 < rb->field1) return -1;
    if (ra->field1 > rb->field1) return 1;
    return 0;
}

static int record_cmp_by_field2_for_test(const void *a, const void *b) {
    const Record *ra = (const Record *) a;
    const Record *rb = (const Record *) b;
    if (ra->field2 < rb->field2) return -1;
    if (ra->field2 > rb->field2) return 1;
    return 0;
}

static int record_cmp_by_field3_for_test(const void *a, const void *b) {
    const Record *ra = (const Record *) a;
    const Record *rb = (const Record *) b;
    return strncmp(ra->field3, rb->field3, 16);
}

static void test_record_sort_by_id(void) {
    Record arr[3];

    arr[0].id = 10;
    arr[1].id = 5;
    arr[2].id = 7;

    hybridsort(arr, 3, sizeof(Record), 2, record_cmp_by_id_for_test);

    assert(arr[0].id == 5);
    assert(arr[1].id == 7);
    assert(arr[2].id == 10);
}

static void test_record_sort_by_field1(void) {
    Record arr[3];

    arr[0].field1 = 3.5f;
    arr[1].field1 = -1.0f;
    arr[2].field1 = 3.5f;

    hybridsort(arr, 3, sizeof(Record), 2, record_cmp_by_field1_for_test);

    assert(arr[0].field1 == -1.0f);
    assert(arr[1].field1 == 3.5f);
    assert(arr[2].field1 == 3.5f);
}

static void test_record_sort_by_field2(void) {
    Record arr[3];

    arr[0].field2 = 100;
    arr[1].field2 = -5;
    arr[2].field2 = 0;

    hybridsort(arr, 3, sizeof(Record), 2, record_cmp_by_field2_for_test);

    assert(arr[0].field2 == -5);
    assert(arr[1].field2 == 0);
    assert(arr[2].field2 == 100);
}

static void test_record_sort_by_field3(void) {
    Record arr[3];

    strncpy(arr[0].field3, "zeta", 16);
    strncpy(arr[1].field3, "beta", 16);
    strncpy(arr[2].field3, "alfa", 16);

    hybridsort(arr, 3, sizeof(Record), 2, record_cmp_by_field3_for_test);

    assert(strncmp(arr[0].field3, "alfa", 16) == 0);
    assert(strncmp(arr[1].field3, "beta", 16) == 0);
    assert(strncmp(arr[2].field3, "zeta", 16) == 0);
}

int main(void) {
    test_int_empty_array();
    test_int_single_element();
    test_int_sorted_array();
    test_int_reverse_array();
    test_int_duplicates();
    test_int_k_zero_quicksort_only();
    test_int_k_bigger_than_nitems();

    test_record_sort_by_id();
    test_record_sort_by_field1();
    test_record_sort_by_field2();
    test_record_sort_by_field3();

    return 0; 
}
