#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/hash_table.h"

static void hash_table_resize(HashTable* table) {
    size_t new_capacity = table->capacity * 2;
    HashNode** new_buckets = calloc(new_capacity, sizeof(HashNode*));

    for (size_t i = 0; i < table->capacity; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            HashNode* next = node->next;
            unsigned long hash_val = table->hash(node->key);
            size_t new_index = hash_val % new_capacity;
            node->next = new_buckets[new_index];
            new_buckets[new_index] = node;
            node = next;
        }
    }

    free(table->buckets);
    table->buckets = new_buckets;
    table->capacity = new_capacity;
}

static size_t get_index(const HashTable* table, const void* key) {
    unsigned long hash_value = table->hash(key);
    return hash_value % table->capacity;
}

HashTable* hash_table_create(int (*equals)(const void*,const void*), unsigned long (*hash)(const void*)) {

    HashTable* table = malloc(sizeof(HashTable));
    if(table == NULL) {
        printf("Error allocating memory for hash table\n");
        return NULL;
    }

    // inizializzazione della tabella hash
    table->equals = equals;
    table->hash = hash; // assegna la funzione di hashing passata come parametro, permettendo con la funzione hash di trovare l'indice del bucket corrispondente per una data chiave
    table->capacity = MAX_CAPACITY;
    table->size = 0;
    table->buckets = calloc(table->capacity, sizeof(HashNode*)); // si è deciso di usare calloc invece della malloc perché è una tabella hash e quindi il numero di bucket è predeterminato (fisso) e tutti i puntatori (buckets) devono essere inizializzati a NULL
    if (table->buckets == NULL) {
        printf("Error allocating memory for buckets\n");
        free(table);
        return NULL;
    }

    return table;
}

void hash_table_put(HashTable* table, const void* key, const void* value) {

    if (table->size / table->capacity > LOAD_FACTOR) hash_table_resize(table);

    size_t index = get_index(table, key);
    HashNode* current_node = table->buckets[index]; // puntatore al primo nodo nella lista collegata del bucket corrispondente

    // Controllo della chiave esistente
    while(current_node != NULL) {
        if(table->equals(current_node->key, key)) {
            current_node->value = (void*) value; // Aggiorna il valore associato alla chiave esistente; inoltre con (void*) si effettua un cast esplicito del tipo di dato ed evita errori di conversione
            return;
        }
        current_node = current_node->next;
    }

    // Aggiunge il nuovo nodo all'inizio della lista concatenata (in testa)
    HashNode* new_node = malloc(sizeof(HashNode));
    new_node->key = (void*) key;
    new_node->value = (void*) value;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->size++;
}

void* hash_table_get(const HashTable* table, const void* key) {

    size_t index = get_index(table, key); // calcola l'indice del bucket corrispondente
    HashNode* current_node = table->buckets[index]; // puntatore al primo nodo nella lista collegata del bucket corrispondente

    // Controllo della chiave esistente
    while(current_node != NULL) {
        if(table->equals(current_node->key, key)) {
            return current_node->value;
        }
        current_node = current_node->next;
    }

    return NULL; // Chiave non trovata
}

int hash_table_contains_key(const HashTable* table, const void* key) {

    if (!table) return 0;

    size_t idx = get_index(table, key);
    HashNode* cur = table->buckets[idx];
    while (cur) {
        if (table->equals(cur->key, key)) {
            return 1;  // Chiave trovata, valore può essere NULL
        }
        cur = cur->next;
    }
    return 0;  // Chiave non trovata
}

void hash_table_remove(HashTable* table, const void* key) {

    size_t index = get_index(table, key);
    HashNode* current_node = table->buckets[index]; // puntatore al primo nodo nella lista collegata del bucket corrispondente
    HashNode* prev_node = NULL; // puntatore al nodo precedente; in questo modo si ricollega la lista quando si elimina un nodo

    while(current_node != NULL) {
        if(table->equals(current_node->key, key)) {
            if(prev_node == NULL) table->buckets[index] = current_node->next; // rimuove il nodo dalla testa della lista
            else prev_node->next = current_node->next; // rimuove il nodo dalla lista e collega il nodo precedente con il successivo
            
            free(current_node); // si libera dalla memoria il nodo rimosso, altrimenti si crea un memory leak
            table->size--;
            return; // non c'è bisogno di continuare la ricerca; esce dalla funzione
        }

        prev_node = current_node;
        current_node = current_node->next;
    }
}

int hash_table_size(const HashTable* table) {
    return (int)table->size;
}

void** hash_table_keyset(const HashTable* table) {
    
    void** keys = malloc(sizeof(void*) * (table->size + 1)); // +1 per il terminatore NULL
    if (keys == NULL) return NULL;
    size_t key_index = 0;

    for(size_t i=0; i<table->capacity; i++) {
        HashNode* current_node = table->buckets[i];
        while(current_node != NULL) {
            keys[key_index] = current_node->key;
            current_node = current_node->next;
            key_index++;
        }
    }

    keys[key_index] = NULL; // termina all'ultimo indice della lista
    return keys;
}

void hash_table_free(HashTable* table) {

    for(size_t i=0; i<table->capacity; i++) {
        HashNode* current_node = table->buckets[i];
        while(current_node != NULL) {
            HashNode* next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }
    }

    free(table->buckets);
    free(table);
}