#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

#define MAX_CAPACITY 128
#define MAX_CHARACTER 256
#define LOAD_FACTOR 0.75


typedef struct HashNode_t {
    void* key;
    void* value;
    struct HashNode_t* next;
} HashNode;

typedef struct HashTable_t {
    int (*equals)(const void*, const void*);       // f1: Function pointer for key comparison, assignment in create hash table
    unsigned long (*hash)(const void*);     // f2: Function pointer for hashing keys
    size_t capacity;        // Number of buckets in the hash table
    size_t size;        // Number of elements (keys-values) in the hash table
    HashNode** buckets;     // Array of pointers to hashNodes (buckets = index in the table)
} HashTable;

HashTable* hash_table_create(int (*equals)(const void*,const void*), unsigned long (*hash)(const void*));
void hash_table_put(HashTable* table, const void* key, const void* value);
void* hash_table_get(const HashTable* table, const void* key);        // Con la funzione void* si intende che la funzione può accettare e restituire un puntatore a qualsiasi tipo di dato
int hash_table_contains_key(const HashTable* table, const void* key);
void hash_table_remove(HashTable* table, const void* key);
int hash_table_size(const HashTable* table);
void** hash_table_keyset(const HashTable* table);     // Con la funzione void** si intende che la funzione restituisce un array di puntatori a qualsiasi tipo di dato
void hash_table_free(HashTable* table);

#endif