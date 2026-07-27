#ifndef BFS_H
#define BFS_H

#include "graph.h"

/* === Algoritmo di Visita in Ampiezza (BFS) === */

/**
 * Esegue la visita in ampiezza (BFS) a partire da un nodo 'start'.
 * @param gr Il grafo.
 * @param start Il nodo di partenza.
 * @param compare Funzione di confronto per i nodi (usata internamente per strutture ausiliarie).
 * @param hash Funzione di hash per i nodi (usata internamente per strutture ausiliarie).
 * @return Array di void* con i nodi nell'ordine di visita, terminato da NULL. NULL se 'start' non esiste.
 */
void** breadth_first_visit(Graph gr, void* start, int (*compare)(const void*, const void*), unsigned long (*hash)(const void*));

#endif