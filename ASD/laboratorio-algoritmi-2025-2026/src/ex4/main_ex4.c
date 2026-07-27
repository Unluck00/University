#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/graph.h"
#include "headers/bfs.h"
#include "../ex2/headers/hash_table.h"

#define MAX_CHARACTER 256

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

int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("Usage: %s <file.csv> <startcity> <output.txt>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* file_name = argv[1];
    const char* start_city = argv[2];
    const char* output_file = argv[3];

    FILE* fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    
    Graph gr = graph_create(1, 0, string_equals, hash_string); // grafo etichettato e non diretto
    if(gr == NULL) {
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    char place1[MAX_CHARACTER], place2[MAX_CHARACTER];
    float distance;

    // lettura del file CSV e costruzione del grafo
    while(fscanf(fp, "%255[^,],%255[^,],%f\n", place1, place2, &distance) == 3) {

        // METODO SENZA STRDUP
        char* place1_copy = malloc(strlen(place1) + 1); // +1 per il terminatore null
        char* place2_copy = malloc(strlen(place2) + 1); // +1 per il terminatore null
        if(place1_copy == NULL || place2_copy == NULL) {
            printf("Error allocating memory for place1/place2\n");
            graph_free(gr);
            fclose(fp);
            continue;
        }
        strcpy(place1_copy, place1);
        strcpy(place2_copy, place2);

        graph_add_node(gr, place1_copy);
        graph_add_node(gr, place2_copy);
        float* dist_ptr = malloc(sizeof(float));
        *dist_ptr = distance; // si alloca memoria per l'etichetta (la distanza)
        graph_add_edge(gr, place1_copy, place2_copy, dist_ptr);

        // METODO USANDO STRDUP, cambiato per via dei test su moodle
        // graph_add_node(gr, strdup(place1)); // si usa strdup per allocare memoria per la stringa
        // graph_add_node(gr, strdup(place2));
        // float* dist_ptr = malloc(sizeof(float));
        // *dist_ptr = distance; // si alloca memoria per l'etichetta (la distanza)
        // graph_add_edge(gr, strdup(place1), strdup(place2), dist_ptr); // si usa strdup per allocare memoria per la stringa
    }

    fclose(fp);

    // esecuzione della BFS
    void** bfs_order = breadth_first_visit(gr, (void*)start_city, string_equals, hash_string);
    if(bfs_order == NULL) {
        printf("Start city not found in the graph.\n");
        graph_free(gr);
        exit(EXIT_FAILURE);
    }

    // scrittura dell'output su file
    FILE* out_fp = fopen(output_file, "w");
    if(out_fp == NULL) {
        printf("Error opening output file\n");
        free(bfs_order);
        graph_free(gr);
        exit(EXIT_FAILURE);
    }

    for(size_t i = 0; bfs_order[i] != NULL; i++) {
        fprintf(out_fp, "%s\n", (char*)bfs_order[i]);
    }

    fclose(out_fp);

    free(bfs_order);
    graph_free(gr);

    return 0;
}