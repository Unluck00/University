#include <stdlib.h>
#include <string.h>
#include "unity/unity.h"
#include "../ex2/headers/hash_table.h"

unsigned long hash_int(const void* key) {
    return (unsigned long)(*(const int*) key);
}

int equals_int(const void* a, const void* b) {
    return *(const int*) a == *(const int*) b;
}

int str_equals(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b) == 0;
}

unsigned long str_hash(const void* s) {
    const unsigned char* str = s;
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (unsigned char)c;
    return hash;
}

static HashTable* table_int;
static HashTable* table_str;


void setUp(void) {
    table_int = hash_table_create(equals_int, hash_int);
    table_str = hash_table_create(str_equals, str_hash);
}

void tearDown(void) {
    hash_table_free(table_int);
    hash_table_free(table_str);
}

void test_hash_table_insert_and_research(void) {
    int keys[] = {1, 2, 3, 4, 5};
    int values[] = {10, 20, 30, 40, 50};

    for(int i=0; i<5; i++) {
        hash_table_put(table_int, &keys[i], &values[i]);
    }

    hash_table_put(table_str, "chiave1", "val1");
    hash_table_put(table_str, "chiave2", "val2");

    for(int i=0; i<5; i++) {
        int* value = hash_table_get(table_int, &keys[i]);
        TEST_ASSERT_NOT_NULL(value);
        TEST_ASSERT_EQUAL_PTR(&values[i], value);
    }

    printf("get(chiave1) = %s\n", (char*)hash_table_get(table_str, "chiave1"));
    printf("get(chiave2) = %s\n", (char*)hash_table_get(table_str, "chiave2"));
    printf("get(chiaveX) = %p\n", hash_table_get(table_str, "chiaveX")); // deve essere NULL
}

void test_hash_table_update_value(void) {
    int key = 1;
    int value1 = 10;
    int value2 = 20;

    hash_table_put(table_int, &key, &value1);
    int* value = hash_table_get(table_int, &key);
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_PTR(&value1, value);

    hash_table_put(table_int, &key, &value2);
    value = hash_table_get(table_int, &key);
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_PTR(&value2, value);
}

void test_hash_table_remove(void) {
    int keys[] = {1, 2, 3, 4, 5};
    int values[] = {10, 20, 30, 40, 50};

    for(int i=0; i<5; i++) {
        hash_table_put(table_int, &keys[i], &values[i]);
    }

    for(int i=0; i<5; i++) {
        hash_table_remove(table_int, &keys[i]);
        TEST_ASSERT_FALSE(hash_table_contains_key(table_int, &keys[i]));
    }
}

void test_hash_table_keyset(void) {
    int keys[] = {1, 2, 3, 4, 5};
    int values[] = {10, 20, 30, 40, 50};

    for(int i=0; i<5; i++) {
        hash_table_put(table_int, &keys[i], &values[i]);
    }

    void** keyset = hash_table_keyset(table_int);
    TEST_ASSERT_NOT_NULL(keyset);
    int count = 0;

    for(int i=0; i<5; i++) {
        int found = 0;
        for(size_t j=0; keyset[j] != NULL; j++) {
            if(table_int->equals(keyset[j], &keys[i])) {
                found = 1;
                break;
            }
        }
        count++;
        TEST_ASSERT_TRUE(found);
    }
    TEST_ASSERT_EQUAL_INT(5, count);
    free(keyset);
}

void test_hash_table_size(void) {
    int keys[] = {1, 2, 3, 4, 5};
    int values[] = {10, 20, 30, 40, 50};

    for(int i=0; i<5; i++) {
        hash_table_put(table_int, &keys[i], &values[i]);
    }

    TEST_ASSERT_EQUAL_INT(5, hash_table_size(table_int));
}

void test_hash_table_contains_key(void) {
    int keys[] = {1, 2, 3};
    int values[] = {10, 20, 30};

    for (int i = 0; i < 3; i++) {
        hash_table_put(table_int, &keys[i], &values[i]);
    }

    // Chiavi esistenti → devono risultare presenti
    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_TRUE(hash_table_contains_key(table_int, &keys[i]));
    }

    // Chiavi non presenti → devono risultare assenti
    int missing = 999;
    TEST_ASSERT_FALSE(hash_table_contains_key(table_int, &missing));
}


void test_hash_table_null_value(void) {
    int key = 7;
    hash_table_put(table_int, &key, NULL);

    TEST_ASSERT_TRUE(hash_table_contains_key(table_int, &key));
    TEST_ASSERT_NULL(hash_table_get(table_int, &key));
}

// Test: recupero di elementi esistenti
void test_get_existing_elements(void) {
    int keys[] = {10, 20, 30};
    int values[] = {100, 200, 300};

    for (int i = 0; i < 3; i++)
        hash_table_put(table_int, &keys[i], &values[i]);

    for (int i = 0; i < 3; i++) {
        int* val = hash_table_get(table_int, &keys[i]);
        TEST_ASSERT_NOT_NULL_MESSAGE(val, "Errore: elemento esistente non trovato");
        TEST_ASSERT_EQUAL_INT(values[i], *val);
    }
}

// Test: recupero di elementi inesistenti
void test_get_non_existing_elements(void) {
    int keys_inserted[] = {1, 2, 3};
    int values[] = {10, 20, 30};

    for (int i = 0; i < 3; i++)
        hash_table_put(table_int, &keys_inserted[i], &values[i]);

    int keys_missing[] = {4, 5, 6, 999};
    for (int i = 0; i < 4; i++) {
        int* val = hash_table_get(table_int, &keys_missing[i]);
        TEST_ASSERT_NULL_MESSAGE(val, "Errore: elemento inesistente restituito");
    }
}

// Test: verifica contains_key separatamente (anche con valore NULL)
void test_contains_key_with_null_value(void) {
    int key = 42;
    hash_table_put(table_int, &key, NULL);

    TEST_ASSERT_TRUE_MESSAGE(hash_table_contains_key(table_int, &key),
        "Errore: chiave presente con valore NULL non rilevata");
}

// Test: collisioni controllate
void test_collision_handling(void) {
    int values[5] = {10, 20, 30, 40, 50};

    // Chiavi che collidono nello stesso bucket per capacity = 128
    int keys[] = {1, 1 + 128, 1 + 2*128, 1 + 3*128, 1 + 4*128};

    for (int i = 0; i < 5; i++) {
        hash_table_put(table_int, &keys[i], &values[i]);
    }

    for (int i = 0; i < 5; i++) {
        int* val = hash_table_get(table_int, &keys[i]);
        TEST_ASSERT_NOT_NULL_MESSAGE(val, "Errore: collisione non gestita correttamente");
        TEST_ASSERT_EQUAL_INT(values[i], *val);
    }
}

int main(void) {

    UNITY_BEGIN();

    RUN_TEST(test_hash_table_insert_and_research);
    RUN_TEST(test_hash_table_update_value);
    RUN_TEST(test_hash_table_remove);
    RUN_TEST(test_hash_table_keyset);
    RUN_TEST(test_hash_table_size);
    RUN_TEST(test_hash_table_contains_key);
    RUN_TEST(test_hash_table_null_value);

    RUN_TEST(test_get_existing_elements);
    RUN_TEST(test_get_non_existing_elements);
    RUN_TEST(test_contains_key_with_null_value);
    RUN_TEST(test_collision_handling);
    
    return UNITY_END();
}