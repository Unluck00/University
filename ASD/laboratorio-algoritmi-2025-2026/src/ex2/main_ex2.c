#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "headers/hash_table.h"

int string_equals(const void* a, const void* b) {
    return strcmp((const char*) a, (const char*) b) == 0; // bisogna fare il cast esplicito del tipo di dato perché le funzioni di confronto accettano puntatori a char 
}

unsigned long hash_string(const void* str) { // Implementazione della funzione di hash djb2 di Dan Bernstein; permette di ottenere una buona distribuzione delle chiavi
    
    const char* s = (const char*) str; // punta al primo carattere della stringa
    unsigned long hash = 5381; // valore iniziale della funzione di hash; È il valore iniziale che dà i risultati migliori quando combinato con il moltiplicatore 33 nel ciclo di hashing
    int c;

    while ((c = *s++)) { // legge ogni carattere della stringa fino al terminatore null; L’operatore * serve per dereferenziare → accedere al contenuto della cella puntata.
        hash = ((hash << 5) + hash) + (unsigned char)c; // hash * 33 + c; hash * 33 viene scritto come (hash << 5) + hash perché è più veloce
        // L'operatore << è uno shift a sinistra dei bit; sposta i bit di 5 posizioni a sinistra, equivalente a moltiplicare per 2^5 (32)
    }

    return hash;
}

void clean_word(char* word) {
    
    char* p = word; // puntatore di lettura
    char* q = word; // puntatore di scrittura
    
    while (*p) { // finché non si raggiunge il terminatore null
        unsigned char c = (unsigned char)*p; // Cast esplicito a unsigned char per evitare valori negativi
        if (isalpha(c)) {
            *q = (char)tolower(c); // se il carattere è alfabetico, lo converte in minuscolo e lo copia
            q++;
        }
        p++;
    }

    if (q > word && ispunct((unsigned char)*(q-1))) {
        q--; // rimuove la punteggiatura finale, se presente
    }
    *q = '\0'; // aggiunge il terminatore null alla fine della parola "pulita"
}

int main(int argc, char const *argv[]) {    // nel test su moodle bisogna inserire senza const
    
    if(argc != 3) {
        printf("Usage: %s <file.txt> <min_length>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* file_name = argv[1];
    int min_len = atoi(argv[2]);
    
    FILE* fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    HashTable* h_table = hash_table_create(string_equals, hash_string);
    if(h_table == NULL) {
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    char buffer[MAX_CHARACTER];

    while(fgets(buffer, MAX_CHARACTER, fp) != NULL) {
        // Rimuove il carattere newline se presente
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Divide la stringa in token usando spazi e punteggiatura come delimitatori
        char* token = strtok(buffer, " \t\n\r\f\v.,;:!?\"'()[]{}-—");
        while(token != NULL) {
            clean_word(token);
            if((int)strlen(token) >= min_len) {
                if(hash_table_contains_key(h_table, token)) {
                    int* count = (int*) hash_table_get(h_table, token);
                    (*count)++;
                }
                else {
                    int* new_count = malloc(sizeof(int));
                    *new_count = 1;
                    
                    // Sostituiamo strdup(token) con allocazione manuale, per via dei test su moodle che non accettavano strdup()
                    char* key_copy = malloc(strlen(token) + 1); // +1 per il terminatore null
                    if (key_copy == NULL) {
                        free(new_count);
                        continue;
                    }
                    strcpy(key_copy, token);
                    
                    hash_table_put(h_table, key_copy, new_count);
                }
                // else {
                //     int* new_count = malloc(sizeof(int)); // alloca memoria per il contatore, perché il nodo è stato definito per contenere puntatori a void e quindi non può contenere direttamente un intero
                //     *new_count = 1;
                //     hash_table_put(h_table, strdup(token), new_count); // strdup() alloca memoria per la stringa e copia il contenuto; evita problemi di puntatori a stringhe temporanee
                // }
            }
            token = strtok(NULL, " \t\n\r\f\v.,;:!?\"'()[]{}-—"); // Chiamata successiva con NULL per continuare dall'ultima posizione
        }
    }

    fclose(fp);

    // Si trova la parola più frequente
    void** keys = hash_table_keyset(h_table); // ottiene l'array di chiavi dalla tabella hash, in cui ogni puntatore in realtà punta alla key presente nella tabella hash
    char* max_word = NULL; // è un puntatore a char perché le chiavi sono stringhe
    int max_count = 0;

    for(size_t i=0; keys[i] != NULL; i++) {
        int* count = (int*) hash_table_get(h_table, keys[i]);
        if(*count > max_count) {
            max_count = *count;
            max_word = (char*) keys[i];
        }
    }

    printf("Most frequent word (length >= %d): '%s' occurred %d times.\n", min_len, max_word, max_count);

    // Si libera la memoria allocata
    for(size_t i=0; keys[i] != NULL; i++) {
        int* count = (int*) hash_table_get(h_table, keys[i]);
        free(count); // libera la memoria allocata per il contatore, cioè la malloc creata in main
        free(keys[i]); // libera la memoria allocata per la stringa (creata con strdup al momento dell'inserimento del nodo); cioè quella usata per memorizzare la chiave nella tabella hash
    }
    free(keys); 
    hash_table_free(h_table);
    
    return 0;
}