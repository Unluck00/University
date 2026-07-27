#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

typedef struct graph *Graph; // crea un incapsulamento per la struct graph in modo che l'utente non possa accedere direttamente ai membri della struct e quindi si possa modificare l'implementazione interna senza influenzare il codice che utilizza il grafo
 
typedef struct edge {
    void* source;   // nodo d'origine
    void* dest;     // nodo di destinazione
    void* label;    // etichetta dell'arco
} Edge;

/**
 * Crea un grafo vuoto.
 * @param labelled 1 se il grafo è etichettato, 0 altrimenti.
 * @param directed 1 se il grafo è diretto, 0 altrimenti.
 * @param compare Funzione di confronto per i nodi.
 * @param hash Funzione di hash per i nodi.
 * Complessità: O(1)
 */
Graph graph_create(int labelled, int directed, int (*compare)(const void*, const void*), unsigned long (*hash)(const void*));

/**
 * Dice se il grafo è diretto o meno.
 * Complessità: O(1)
 */
int graph_is_directed(const Graph gr);

/**
 * Dice se il grafo è etichettato o meno.
 * Complessità: O(1)
 */
int graph_is_labelled(const Graph gr);


/* === Gestione Nodi === */

/**
 * Aggiunge un nodo al grafo.
 * @return 1 se aggiunto, 0 se era già presente.
 * Complessità: O(1)
 */
int graph_add_node(Graph gr, const void* node);

/**
 * Controlla se un nodo è presente nel grafo.
 * Complessità: O(1)
 */
int graph_contains_node(const Graph gr, const void* node);

/**
 * Rimuove un nodo dal grafo.
 * @return 1 se rimosso, 0 se non trovato.
 * Complessità: O(N)
 */
int graph_remove_node(Graph gr, const void* node);

/**
 * Restituisce il numero di nodi nel grafo.
 * Complessità: O(1)
 */
int graph_num_nodes(const Graph gr);

/**
 * Recupera tutti i nodi del grafo.
 * Restituisce un array di void* terminato da NULL.
 * Complessità: O(N)
 */
void** graph_get_nodes(const Graph gr);


/* === Gestione Archi === */

/**
 * Aggiunge un arco dati gli estremi e l'etichetta.
 * @param label Etichetta dell'arco (può essere ignorata se il grafo non è labelled).
 * @return 1 se aggiunto, 0 se l'arco era già presente o se i nodi non esistono.
 * Complessità: O(1) (*)
 */
int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label);

/**
 * Controlla se un arco è presente nel grafo.
 * Complessità: O(1) (*)
 */
int graph_contains_edge(const Graph gr, const void* node1, const void* node2);

/**
 * Rimuove un arco dal grafo.
 * @return 1 se rimosso, 0 se non trovato o se i nodi non esistono.
 * Complessità: O(1) (*)
 */
int graph_remove_edge(Graph gr, const void* node1, const void* node2);

/**
 * Restituisce il numero di archi nel grafo.
 * Complessità: O(N) (N = numero di archi)
 */
int graph_num_edges(const Graph gr);

/**
 * Recupera tutti gli archi del grafo.
 * Restituisce un array di Edge* terminato da NULL.
 * Complessità: O(N) (N = numero di archi)
 */
Edge** graph_get_edges(const Graph gr);

/**
 * Recupera l'etichetta di un arco.
 * Restituisce NULL se l'arco non esiste.
 * Complessità: O(1) (*)
 */
void* graph_get_label(const Graph gr, const void* node1, const void* node2);


/* === Adiacenza === */

/**
 * Recupera i nodi adiacenti a un dato nodo.
 * Restituisce un array di void* terminato da NULL.
 * Complessità: O(1) (*) (assumendo grado del nodo in O(1))
 */
void** graph_get_neighbours(const Graph gr, const void* node);

/**
 * Recupera il numero di nodi adiacenti a un dato nodo.
 * Complessità: O(1)
 */
int graph_num_neighbours(const Graph gr, const void* node);


/* === Distruzione === */

/**
 * Libera la memoria allocata per il grafo.
 */
void graph_free(Graph gr);

#endif