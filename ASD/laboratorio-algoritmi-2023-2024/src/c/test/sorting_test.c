#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>                      // per NAN
#include "unity/unity.h"
#include "../ex1/headers/comparing_functions.h"
#include "../ex1/headers/sorting.h"

/* ------- helpers alloc ------- */
static Record* rec_i(int id, int v) {
    Record* r = calloc(1, sizeof(Record));
    r->id = id; r->field2 = v; return r;
}
static Record* rec_d(int id, double v) {
    Record* r = calloc(1, sizeof(Record));
    r->id = id; r->field3 = v; return r;
}
static Record* rec_s(int id, const char* s) {
    Record* r = calloc(1, sizeof(Record));
    r->id = id;
    if (s) { r->field1 = malloc(strlen(s)+1); strcpy(r->field1, s); }
    return r;
}
static void free_arr(Record** a, size_t n) {
    for (size_t i=0;i<n;i++){ if(!a[i]) continue; free(a[i]->field1); free(a[i]); }
}

/* Unity hooks (qui non servono setup/teardown specifici) */
void setUp(void) {}
void tearDown(void) {}

/* ------- INT ------- */
static void check_sorted_int(Record** a, size_t n) {
    for (size_t i=1;i<n;i++) {
        TEST_ASSERT_LESS_OR_EQUAL(a[i]->field2, a[i]->field2); // no-op
        TEST_ASSERT_TRUE(a[i-1]->field2 <= a[i]->field2);
    }
}
void test_merge_sort_int_basic(void) {
    Record* a[5] = {rec_i(1,10),rec_i(2,5),rec_i(3,20),rec_i(4,15),rec_i(5,30)};
    merge_sort((void**)a, 5, compareInt);
    int exp[] = {5,10,15,20,30};
    for (int i=0;i<5;i++) TEST_ASSERT_EQUAL_INT(exp[i], a[i]->field2);
    free_arr(a,5);
}
void test_quick_sort_int_basic(void) {
    Record* a[5] = {rec_i(1,10),rec_i(2,5),rec_i(3,20),rec_i(4,15),rec_i(5,30)};
    quick_sort((void**)a, 5, compareInt);
    int exp[] = {5,10,15,20,30};
    for (int i=0;i<5;i++) TEST_ASSERT_EQUAL_INT(exp[i], a[i]->field2);
    free_arr(a,5);
}
void test_int_edge_empty_and_singleton(void) {
    /* n=0 non deve crashare (guard-clause) */
    merge_sort(NULL, 0, compareInt);
    quick_sort(NULL, 0, compareInt);
    /* singleton */
    Record* a[1] = {rec_i(1, 42)};
    merge_sort((void**)a, 1, compareInt);
    quick_sort((void**)a, 1, compareInt);
    TEST_ASSERT_EQUAL_INT(42, a[0]->field2);
    free_arr(a,1);
}
void test_int_duplicates_sorted_reverse(void) {
    /* duplicati */
    Record* a[6] = {rec_i(1,2),rec_i(2,2),rec_i(3,2),rec_i(4,1),rec_i(5,1),rec_i(6,3)};
    merge_sort((void**)a, 6, compareInt); check_sorted_int(a,6);
    quick_sort((void**)a, 6, compareInt); check_sorted_int(a,6);
    free_arr(a,6);

    /* già ordinato e inverso */
    Record* b[4] = {rec_i(1,1),rec_i(2,2),rec_i(3,3),rec_i(4,4)};
    merge_sort((void**)b,4,compareInt); check_sorted_int(b,4);
    quick_sort((void**)b,4,compareInt); check_sorted_int(b,4);
    free_arr(b,4);

    Record* c[4] = {rec_i(1,4),rec_i(2,3),rec_i(3,2),rec_i(4,1)};
    merge_sort((void**)c,4,compareInt); check_sorted_int(c,4);
    quick_sort((void**)c,4,compareInt); check_sorted_int(c,4);
    free_arr(c,4);
}

/* ------- DOUBLE (inclusi NaN policy) ------- */
void test_merge_sort_double_basic(void) {
    Record* a[5] = {rec_d(1,10.3),rec_d(2,5.8),rec_d(3,20.4),rec_d(4,15.2),rec_d(5,30.9)};
    merge_sort((void**)a, 5, compareDouble);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 5.8, a[0]->field3);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 10.3, a[1]->field3);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 15.2, a[2]->field3);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 20.4, a[3]->field3);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 30.9, a[4]->field3);
    for (int i=0;i<5;i++) TEST_ASSERT_DOUBLE_WITHIN(1e-9, exp[i], a[i]->field3);
    free_arr(a,5);
}
void test_quick_sort_double_basic(void) {
    Record* a[5] = {rec_d(1,10.3),rec_d(2,5.8),rec_d(3,20.4),rec_d(4,15.2),rec_d(5,30.9)};
    quick_sort((void**)a, 5, compareDouble);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 5.8, a[0]->field3);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 10.3, a[1]->field3);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 15.2, a[2]->field3);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 20.4, a[3]->field3);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 30.9, a[4]->field3);
    for (int i=0;i<5;i++) TEST_ASSERT_DOUBLE_WITHIN(1e-9, exp[i], a[i]->field3);
    free_arr(a,5);
}
void test_double_with_NaN_goes_last(void) {
    Record* a[4] = {rec_d(1, 2.0), rec_d(2, NAN), rec_d(3, -1.0), rec_d(4, 0.0)};
    merge_sort((void**)a, 4, compareDouble);
    TEST_ASSERT_TRUE(isnan(a[3]->field3));   // NaN in fondo (policy)
    free_arr(a,4);
}

/* ------- STRING (inclusi NULL policy) ------- */
void test_merge_sort_string_basic(void) {
    Record* a[5] = {rec_s(1,"test"),rec_s(2,"ciao"),rec_s(3,"prova"),rec_s(4,"ab"),rec_s(5,"zl")};
    merge_sort((void**)a, 5, compareString);
    const char* exp[] = {"ab","ciao","prova","test","zl"};
    for (int i=0;i<5;i++) TEST_ASSERT_EQUAL_STRING(exp[i], a[i]->field1);
    free_arr(a,5);
}
void test_quick_sort_string_basic(void) {
    Record* a[5] = {rec_s(1,"test"),rec_s(2,"ciao"),rec_s(3,"prova"),rec_s(4,"ab"),rec_s(5,"zl")};
    quick_sort((void**)a, 5, compareString);
    const char* exp[] = {"ab","ciao","prova","test","zl"};
    for (int i=0;i<5;i++) TEST_ASSERT_EQUAL_STRING(exp[i], a[i]->field1);
    free_arr(a,5);
}
void test_string_NULL_policy(void) {
    Record* a[3] = {rec_s(1,"b"), rec_s(2,NULL), rec_s(3,"a")};
    merge_sort((void**)a, 3, compareString);
    TEST_ASSERT_NULL(a[0]->field1);                   // NULL prima di qualunque stringa
    TEST_ASSERT_EQUAL_STRING("a", a[1]->field1);
    TEST_ASSERT_EQUAL_STRING("b", a[2]->field1);
    free_arr(a,3);
}

/* ------- Stabilità (solo MergeSort è stabile) ------- */
void test_mergesort_stability_on_equal_keys(void) {
    /* chiave = field2; ids 1..4 in input */
    Record* a[4] = {rec_i(1, 7), rec_i(2, 7), rec_i(3, 7), rec_i(4, 7)};
    merge_sort((void**)a, 4, compareInt);
    /* ordine input preservato */
    TEST_ASSERT_EQUAL_INT(1, a[0]->id);
    TEST_ASSERT_EQUAL_INT(2, a[1]->id);
    TEST_ASSERT_EQUAL_INT(3, a[2]->id);
    TEST_ASSERT_EQUAL_INT(4, a[3]->id);
    free_arr(a,4);
}

/* ------- Runner ------- */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_merge_sort_int_basic);
    RUN_TEST(test_quick_sort_int_basic);
    RUN_TEST(test_int_edge_empty_and_singleton);
    RUN_TEST(test_int_duplicates_sorted_reverse);

    RUN_TEST(test_merge_sort_double_basic);
    RUN_TEST(test_quick_sort_double_basic);
    RUN_TEST(test_double_with_NaN_goes_last);

    RUN_TEST(test_merge_sort_string_basic);
    RUN_TEST(test_quick_sort_string_basic);
    RUN_TEST(test_string_NULL_policy);

    RUN_TEST(test_mergesort_stability_on_equal_keys);

    return UNITY_END();
}
