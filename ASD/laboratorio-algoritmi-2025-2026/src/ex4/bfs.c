#include "headers/bfs.h"
#include "../ex2/headers/hash_table.h"

// usando la funzione breadth_first_visit si esegue la visita in ampiezza (BFS) a partire da un nodo 'start' nel grafo 'gr'
void** breadth_first_visit(Graph gr, void* start, int (*compare)(const void*, const void*), unsigned long (*hash)(const void*)) {
    if(!graph_contains_node(gr, start)) return NULL; // se il nodo di partenza non esiste, restituisce NULL

    HashTable* visited = hash_table_create(compare, hash); // tabella hash per tenere traccia dei nodi visitati
    if(visited == NULL) {
        printf("Error allocating memory for visited hash table\n");
        return NULL;
    }

    // array dinamico per la coda della BFS
    void** queue = malloc(sizeof(void*) * (size_t)(graph_num_nodes(gr) + 1)); // coda per la BFS, dato dalla dimensione massima del grafo + 1 (per il terminatore NULL)
    if(queue == NULL) {
        printf("Error allocating memory for BFS queue\n");
        hash_table_free(visited);
        return NULL;
    }

    // array per memorizzare l'ordine di visita
    void** result = malloc(sizeof(void*) * (size_t)(graph_num_nodes(gr) + 1)); // array per memorizzare l'ordine di visita
    if(result == NULL) {
        printf("Error allocating memory for result array\n");
        free(queue);
        hash_table_free(visited);
        return NULL;
    }

    size_t front = 0, rear = 0; // indici per la coda (front = testa, rear = coda)
    size_t result_index = 0; // indice per l'array di risultato

    queue[rear++] = start; // enqueue del nodo di partenza, aggiunta del nodo alla coda (il primo elemento viene inserito in posizione 0, poi rear viene incrementato)
    hash_table_put(visited, start, (void*)1); // marca il nodo di partenza come visitato, usando 1 per indicare che è stato visitato (andava bene qualsiasi valore non NULL)

    while(front < rear) { // finché la coda non è vuota
        void* current = queue[front++]; // dequeue, e come se si rimuovesse l'elemento dalla coda (l'elemento in posizione front viene preso, poi front viene incrementato)
        result[result_index++] = current; // aggiunge il nodo corrente al risultato

        void** neighbors = graph_get_neighbours(gr, current); // ottiene i vicini del nodo corrente, restituisce un array di void* terminato da NULL dove ogni puntatore è un nodo vicino
        if(neighbors != NULL) {
            for(size_t i = 0; neighbors[i] != NULL; i++) {
                if(!hash_table_contains_key(visited, neighbors[i])) { // se il vicino non è stato visitato
                    queue[rear++] = neighbors[i]; // enqueue del vicino, aggiunta del vicino alla coda precisamente in posizione rear, poi rear viene incrementato
                    hash_table_put(visited, neighbors[i], (void*)1); // marca il vicino come visitato
                }
            }
            free(neighbors); // libera la memoria dell'array dei vicini
        }
    }

    result[result_index] = NULL; // termina l'array con NULL

    free(queue);
    hash_table_free(visited);
    return result;
}
