#include <stdlib.h>
#include <stdint.h>

#include "headers/priority_queue.h"
#include "../ex2/headers/hash_table.h"

static int (*g_cmp_for_map)(const void*, const void*) = NULL; // funzione di confronto globale per la mappa delle posizioni, si usa questa invece che passare la funzione ad ogni chiamata

static int map_equals_from_cmp(const void* a, const void* b) { // funzione di uguaglianza basata sulla funzione di confronto globale
    if (!g_cmp_for_map) return a == b; // fallback se la funzione non è stata impostata, quindi g_cmp_for_map è NULL
    return (g_cmp_for_map(a, b) == 0) ? 1 : 0; // restituisce 1 se uguali, 0 altrimenti; uso di questa condizione per adattarsi alla funzione di uguaglianza richiesta dalla hash table
}

static inline const void* idx_to_ptr(int idx) { 
    return (const void*)(intptr_t)(idx + 1);
    // si aggiunge 1 per evitare di restituire NULL per l'indice 0
    // si converte l'indice in intptr_t (intero con la stessa dimensione di un puntatore) prima di fare il cast a void*
    // per evitare warning di conversione diretta da int a void*
    // esempio: indice 0 → puntatore (void*)1, indice 1 → puntatore (void*)2, ecc.
    // quindi se l'indice è 0, nell'hash table viene memorizzato il puntatore (void*)1, che non è NULL e che punterà all'indice 0 quando verrà recuperato
}

static inline int ptr_to_idx(const void* p) {
    return (int)(intptr_t)p - 1;
    // si converte il puntatore in intptr_t (intero con la stessa dimensione di un puntatore) prima di fare il cast a int
    // per evitare warning di conversione diretta da void* a int
    // si sottrae 1 per ottenere l'indice originale
}

struct PriorityQueue {
    void   **nodes;   // array dinamico di puntatori ai nodi
    size_t   size;
    size_t   capacity;

    int (*cmp)(const void*, const void*);
    unsigned long (*hash)(const void*);

    HashTable *pos; // mappa delle posizioni: key = elemento (void*), value = indice nell'array nodes (memorizzato come puntatore usando idx_to_ptr)
    // si è aggiunto le tabelle hash invece che fare un ciclo della priority queue per cercare un elemento, perché così si ha una complessità di O(1) per la ricerca dell'elemento nella coda con priorità
    // infatti la tabella hash mantiene la mappatura tra l'elemento e il suo indice nell'array nodes, mentre un ciclo lineare avrebbe una complessità di O(n)

    // quindi il PriorityQueue mantiene un array dinamico di nodi (void** nodes) che rappresentano gli elementi della coda con priorità e una tabella hash (HashTable* pos) per tracciare le posizioni di ciascun elemento nell'array nodes, permettendo operazioni efficienti di inserimento, accesso e rimozione basate sulla priorità definita dalla funzione di confronto cmp.
};

#define PQ_INITIAL_CAPACITY 128

static int parent_idx(int i) { return (i - 1) / 3; }

static void map_set_index(PriorityQueue *pq, const void *key, int idx) { // aggiorna la mappa delle posizioni
    hash_table_put(pq->pos, key, idx_to_ptr(idx)); // memorizza l'indice + 1 come puntatore
}

static int map_get_index(const PriorityQueue *pq, const void *key) { // ottiene l'indice dell'elemento dalla mappa delle posizioni
    const void *v = hash_table_get(pq->pos, key);
    if (v == NULL) return -1;
    return ptr_to_idx(v);
}

static void swap_nodes(PriorityQueue *pq, int i, int j) {
    void *tmp = pq->nodes[i];
    pq->nodes[i] = pq->nodes[j];
    pq->nodes[j] = tmp;

    map_set_index(pq, pq->nodes[i], i); 
    map_set_index(pq, pq->nodes[j], j);
}

static void heapify_up(PriorityQueue *pq, int i) {
    while (i > 0) {
        int p = parent_idx(i);
        if (pq->cmp(pq->nodes[i], pq->nodes[p]) > 0) {
            swap_nodes(pq, i, p);
            i = p;
        } else break;
    }
}

static void heapify_down(PriorityQueue *pq, int i) {
    while (1) {
        int largest = i;
        int c1 = 3*i + 1;
        int c2 = 3*i + 2;
        int c3 = 3*i + 3;
        int s = (int)pq->size;

        if (c1 < s && pq->cmp(pq->nodes[c1], pq->nodes[largest]) > 0) largest = c1;
        if (c2 < s && pq->cmp(pq->nodes[c2], pq->nodes[largest]) > 0) largest = c2;
        if (c3 < s && pq->cmp(pq->nodes[c3], pq->nodes[largest]) > 0) largest = c3;

        if (largest != i) {
            swap_nodes(pq, i, largest);
            i = largest;
        } else break;
    }
}

PriorityQueue* priority_queue_create(int (*f1)(const void*,const void*),
                                     unsigned long (*f2)(const void*)) {
    if (!f1 || !f2) return NULL;

    PriorityQueue *pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!pq) return NULL;

    pq->capacity = PQ_INITIAL_CAPACITY; 
    pq->size = 0;
    pq->nodes = (void**)malloc(sizeof(void*) * pq->capacity); 
    pq->cmp = f1;
    pq->hash = f2;

    g_cmp_for_map = f1; // impostazione della funzione di confronto globale per la funzione di uguaglianza della mappa
    pq->pos = hash_table_create(map_equals_from_cmp, f2); // creazione della tabella hash per la mappa delle posizioni

    if (!pq->nodes || !pq->pos) {
        if (pq->nodes) free(pq->nodes);
        if (pq->pos) hash_table_free(pq->pos);
        free(pq);
        return NULL;
    }
    return pq;
}

int priority_queue_contains(const PriorityQueue* pq, const void* data) {
    if (!pq || !data) return -1;
    return hash_table_contains_key(pq->pos, data);
}

int priority_queue_push(PriorityQueue* pq, void* data) {
    if (!pq || !data) return -1;

    if (priority_queue_contains(pq, data)) return 0;

    if (pq->size == pq->capacity) {
        size_t new_cap = pq->capacity * 2;
        void **new_nodes = (void**)realloc(pq->nodes, sizeof(void*) * new_cap); // reallocazione della memoria per i nodi, raddoppiando la capacità e copia i vecchi dati nei nuovi spazi di memoria
        if (!new_nodes) return 0;
        pq->nodes = new_nodes; // aggiornamento del puntatore ai nodi
        pq->capacity = new_cap; // aggiornamento della capacità
    }

    int i = (int)pq->size; // indice dell'ultimo elemento
    pq->nodes[i] = data; // inserimento del nuovo elemento alla fine dell'heap

    map_set_index(pq, data, i); // aggiornamento della mappa delle posizioni nell'hash table
    pq->size++;

    heapify_up(pq, i); // ripristino della proprietà di heap salendo l'elemento appena inserito
    return 1;
}

void* priority_queue_top(const PriorityQueue* pq) {
    if (!pq || pq->size == 0) return NULL;
    return pq->nodes[0];
}

int priority_queue_remove(PriorityQueue* pq, con st void* data) {
    if (!pq || !data) return -1;

    int i = map_get_index(pq, data);
    if (i == -1) return 0;

    hash_table_remove(pq->pos, data);

    int last = (int)pq->size - 1;

    if (i == last) {
        pq->nodes[i] = NULL;
        pq->size--;
        return 1;
    }

    void *last_elem = pq->nodes[last];
    pq->nodes[i] = last_elem;
    pq->nodes[last] = NULL;
    pq->size--;

    map_set_index(pq, last_elem, i);

    heapify_up(pq, i); 
    heapify_down(pq, i);

    return 1;
}

void priority_queue_pop(PriorityQueue* pq) {
    if (!pq || pq->size == 0) return;
    priority_queue_remove(pq, pq->nodes[0]);
}

int priority_queue_size(const PriorityQueue* pq) {
    return pq ? (int)pq->size : -1;
}

void priority_queue_free(PriorityQueue* pq) {
    if (!pq) return;
    hash_table_free(pq->pos);
    free(pq->nodes);
    free(pq);
}
