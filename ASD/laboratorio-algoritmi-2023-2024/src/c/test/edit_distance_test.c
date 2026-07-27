#include "unity/unity.h"
#include "../ex2/headers/edit_distance.h"

void setUp(void) {}

void tearDown(void) {}

void test_edit_distance_empty_string(void) {
    // Test with pure recursion
    TEST_ASSERT_EQUAL_INT(0, edit_distance("", ""));

    // Test with memoization
    TEST_ASSERT_EQUAL_INT(0, edit_distance_dyn("", ""));
}

void test_edit_distance_one_empty_string(void) {
    // Test with pure recursion
    TEST_ASSERT_EQUAL_INT(4, edit_distance("casa", ""));
    TEST_ASSERT_EQUAL_INT(4, edit_distance("", "vino"));

    // Test with memoization
    TEST_ASSERT_EQUAL_INT(4, edit_distance_dyn("casa", ""));
    TEST_ASSERT_EQUAL_INT(4, edit_distance_dyn("", "vino"));
}

void test_edit_distance_symbols(void) {
    // Test with pure recursion
    TEST_ASSERT_EQUAL_INT(1, edit_distance("ciao!", "ciao"));

    // Test with memoization
    TEST_ASSERT_EQUAL_INT(1, edit_distance_dyn("ciao!", "ciao"));
}

void test_edit_distance_equal(void) {
    // Test with pure recursion
    TEST_ASSERT_EQUAL_INT(0, edit_distance("pioppo", "pioppo"));

    // Test with memoization
    TEST_ASSERT_EQUAL_INT(0, edit_distance_dyn("pioppo", "pioppo"));
}

void test_edit_distance_insertion(void) {
    // Test with pure recursion
    TEST_ASSERT_EQUAL_INT(2, edit_distance("vinaio", "vino"));

    // Test with memoization
    TEST_ASSERT_EQUAL_INT(2, edit_distance_dyn("vinaio", "vino"));
}

void test_edit_distance_deletion(void) {
    // Test with pure recursion
    TEST_ASSERT_EQUAL_INT(1, edit_distance("casa", "cassa"));

    // Test with memoization
    TEST_ASSERT_EQUAL_INT(1, edit_distance_dyn("casa", "cassa"));
}

void test_edit_distance_insertion_deletion(void) {
    // Test with pure recursion
    TEST_ASSERT_EQUAL_INT(2, edit_distance("casa", "cara"));
    TEST_ASSERT_EQUAL_INT(4, edit_distance("tassa", "passato"));

    // Test with memoization
    TEST_ASSERT_EQUAL_INT(2, edit_distance_dyn("casa", "cara"));
    TEST_ASSERT_EQUAL_INT(4, edit_distance_dyn("tassa", "passato"));
}

int main(void) {

    UNITY_BEGIN();

    RUN_TEST(test_edit_distance_empty_string);
    RUN_TEST(test_edit_distance_one_empty_string);
    RUN_TEST(test_edit_distance_symbols);
    RUN_TEST(test_edit_distance_equal);
    RUN_TEST(test_edit_distance_insertion);
    RUN_TEST(test_edit_distance_deletion);
    RUN_TEST(test_edit_distance_insertion_deletion);

    return UNITY_END();
}