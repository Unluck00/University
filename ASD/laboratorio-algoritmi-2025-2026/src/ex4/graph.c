#include "headers/graph.h"
#include "../ex2/headers/hash_table.h"

// definizione privata della struct di graph in c 
struct graph { // permette di accedere ai membri della struct solo all'interno di questo file (graph.c) 
    int labelled;
    int directed;
    HashTable* nodes;             // per cui nodes è una tabella hash che mappa i nodi del grafo (key = nodo (void*), value = struct adjacency*)
    int num_edges;
    int (*compare)(const void*, const void*);
    unsigned long (*hash)(const void*);
};

typedef struct adjacency {
    HashTable* neighbors; // l'hash table mappa i nodi adiacenti al nodo corrente; key = nodo adiacente (void*), value = etichetta dell'arco (void*) o NULL se il grafo non è etichettato
} Adjacency;

Graph graph_create(int labelled, int directed, int (*compare)(const void*, const void*), unsigned long (*hash)(const void*)) {
    Graph gr = malloc(sizeof(*gr)); // Graph è un typedef per struct graph*, in cui gr è già un puntatore (a struct); sizeof(*gr) restituisce la dimensione della struct graph (dereferenziando il puntatore)
    if(gr == NULL) {
        printf("Error allocating memory for graph\n");
        return NULL;
    } // aggiungere al main il controllo di errore su questa funzione
    gr->labelled = labelled;
    gr->directed = directed;
    gr->num_edges = 0;
    gr->compare = compare;
    gr->hash = hash;
    gr->nodes = hash_table_create(compare, hash); // creazione della tabella hash per i nodi del grafo
    if(gr->nodes == NULL) {
        free(gr);
        return NULL;
    }
    return gr;
}

int graph_is_directed(const Graph gr) {
    if(gr == NULL) return 0;
    return gr->directed;
}

int graph_is_labelled(const Graph gr) {
    if(gr == NULL) return 0;
    return gr->labelled;
}

int graph_add_node(Graph gr, const void* node) {
    if(gr == NULL || node == NULL) return 0;
    if(graph_contains_node(gr, node)) return 0; // controlla che il nodo non sia già presente nel grafo

    Adjacency* adj = malloc(sizeof(Adjacency)); // si alloca memoria per la struct adjacency del nuovo nodo rappresentato
    if(adj == NULL) {
        printf("Error allocating memory for adjacency\n");
        return 0;
    }

    adj->neighbors = hash_table_create(gr->compare, gr->hash); // creazione di un hashtable per i nodi adiacenti
    if(adj->neighbors == NULL) {
        free(adj);
        printf("Error allocating memory for neighbors hash table\n");
        return 0;
    }

    hash_table_put(gr->nodes, node, adj); // lo si aggiunge alla tabella dei nodi del grafo
    return 1;
}

int graph_contains_node(const Graph gr, const void* node) {
    if(gr == NULL) return 0;
    return hash_table_contains_key(gr->nodes, node);
}

int graph_remove_node(Graph gr, const void* node) {
    if(gr == NULL) return 0;
    if(!graph_contains_node(gr, node)) return 0; // controlla che il nodo non sia presente nel grafo

    // Rimuove tutti gli archi entranti verso questo nodo (quindi rimuove il nodo dalle liste di adiacenza degli altri nodi
    void** all_nodes = graph_get_nodes(gr); // si ottiene la lista di tutti i nodi del grafo
    if(all_nodes == NULL) {
        printf("Error getting all nodes or there's not node on the graph\n");
        return 0;
    }

    int removed_edges = 0;
    for(size_t i=0; all_nodes[i] != NULL; i++) {
        if(gr->compare(all_nodes[i], node) == 0) continue; // salta se è lo stesso nodo
        void** neighbors = graph_get_neighbours(gr, all_nodes[i]); // si ottiene la lista dei nodi adiacenti a questo nodo
        if(neighbors == NULL) continue; // nessun vicino o errore, quindi salta
        for(size_t j=0; neighbors[j] != NULL; j++) {
            if(gr->compare(neighbors[j], node) == 0) { // se trova il nodo da rimuovere tra i vicini
                graph_remove_edge(gr, all_nodes[i], node); // rimuove l'arco dall'altro nodo verso questo nodo
                removed_edges++;
                break;
            }
        }
        free(neighbors);
    }
    free(all_nodes);

    // Rimuove l'arco uscente (liberando la memoria della hash table neighbors), quindi rimuove il nodo dalla tabella dei nodi
    Adjacency* adj = (Adjacency*) hash_table_get(gr->nodes, node); // ottiene la struct adjacency del nodo (la value del hashNode) che contiene la tabella dei nodi adiacenti da rimuovere
    if(adj != NULL) { // controlla che ci sia l'adiacenza del nodo
        if(adj->neighbors != NULL) {
            hash_table_free(adj->neighbors); // libera la memoria della hash table dei nodi adiacenti
            adj->neighbors = NULL; // evita dangling pointer, cioè puntatori che puntano a memoria già liberata
        }
        free(adj); // rimuove la struct adjacency allocata in graph_add_node
    }

    // Infine si rimuove il nodo dalla tabella dei nodi
    hash_table_remove(gr->nodes, node);

    gr->num_edges -= removed_edges;

    return 1;
}

int graph_num_nodes(const Graph gr) {
    if(gr == NULL) return 0;
    return hash_table_size(gr->nodes);
}

void** graph_get_nodes(const Graph gr) {
    if(gr == NULL) return NULL;
    return hash_table_keyset(gr->nodes);
}

int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label) {
    if(!gr->labelled && label != NULL) return 0; // se il grafo non è etichettato, non si possono aggiungere etichette
    if(!graph_contains_node(gr, node1) || !graph_contains_node(gr, node2) || graph_contains_edge(gr, node1, node2)) return 0; // controlla che entrambi i nodi siano già presenti nel grafo e che l'arco non esista già

    // Inserisce l'arco diretto da node1 a node2
    Adjacency* adj1 = (Adjacency*) hash_table_get(gr->nodes, node1); // si ottiene la struct adjacency del nodo1 (la value del hashNode) che contiene la tabella dei nodi adiacenti
    if(adj1 == NULL) return 0; // controllo di sicurezza
    hash_table_put(adj1->neighbors, (void*)node2, (void*)label); // si aggiunge alla tabella dei nodi adiacenti di node1 il nodo2 con l'etichetta dell'arco come value
    gr->num_edges++;

    // Se il grafo è non diretto, allora si inserisce anche l'arco inverso (da node2 a node1)
    if(!gr->directed) {
        Adjacency* adj2 = (Adjacency*) hash_table_get(gr->nodes, node2); // si ottiene la struct adjacency del nodo2 (la value del hashNode) che contiene la tabella dei nodi adiacenti
        if(adj2 == NULL) return 0; // controllo di sicurezza
        hash_table_put(adj2->neighbors, (void*)node1, (void*)label); // si aggiunge alla tabella dei nodi adiacenti di node2 il nodo1 con l'etichetta dell'arco come value
    }

    return 1; 
}

int graph_contains_edge(const Graph gr, const void* node1, const void* node2) {
    if(gr == NULL) return 0;
    if(!graph_contains_node(gr, node1)) return 0; // se il nodo1 non esiste, l'arco non può esistere

    Adjacency* adj1 = (Adjacency*) hash_table_get(gr->nodes, node1); // si ottiene la struct adjacency del nodo1 (la value del hashNode) che contiene la tabella dei nodi adiacenti
    if(adj1 == NULL) return 0; // controllo di sicurezza
    return hash_table_contains_key(adj1->neighbors, node2); // controlla se nella tabella dei nodi adiacenti di node1 esiste il nodo2
}

int graph_remove_edge(Graph gr, const void* node1, const void* node2) {
    if(gr == NULL || node1 == NULL || node2 == NULL) return 0;
    if(!graph_contains_edge(gr, node1, node2)) return 0; // controlla che l'arco non sia presente nel grafo

    Adjacency* adj1 = (Adjacency*) hash_table_get(gr->nodes, node1); // si ottiene la struct adjacency del nodo1 (la value del hashNode) che contiene la tabella dei nodi adiacenti
    if(adj1 == NULL) return 0; // controllo di sicurezza
    hash_table_remove(adj1->neighbors, node2); // rimuove dalla tabella dei nodi adiacenti di node1 il nodo2
    gr->num_edges--;

    // Se il grafo è non diretto, allora si rimuove anche l'arco inverso (da node2 a node1)
    if(!gr->directed) {
        Adjacency* adj2 = (Adjacency*) hash_table_get(gr->nodes, node2); // si ottiene la struct adjacency del nodo2 (la value del hashNode) che contiene la tabella dei nodi adiacenti
        if(adj2 == NULL) return 0; // controllo di sicurezza
        hash_table_remove(adj2->neighbors, (void*)node1); // rimuove dalla tabella dei nodi adiacenti di node2 il nodo1
    }

    return 1; 
}

int graph_num_edges(const Graph gr) {
    if(gr == NULL) return 0;
    return gr->num_edges;
}

Edge** graph_get_edges(const Graph gr) {
    if(gr == NULL) return NULL;

    void** all_nodes = graph_get_nodes(gr); // si ottiene la lista di tutti i nodi del grafo
    if(all_nodes == NULL) {
        printf("Error getting all nodes or there's not node on the graph\n");
        return NULL;
    }

    /* Prima passata: conta gli archi che verranno restituiti */
    size_t count = 0; // indice per l'array di Edge*
    for (size_t i = 0; all_nodes[i] != NULL; i++) {
        Adjacency* adj = (Adjacency*) hash_table_get(gr->nodes, all_nodes[i]); // si ottiene la struct adjacency del nodo corrente
        if (adj == NULL || adj->neighbors == NULL) continue; // controllo di sicurezza
        void** neighbors = hash_table_keyset(adj->neighbors); // si ottiene la lista dei nodi adiacenti a questo nodo
        if (neighbors == NULL) continue; // nessun vicino o errore, quindi salta
        for (size_t j = 0; neighbors[j] != NULL; j++) {
            // Per i grafi non diretti, si evita di aggiungere l'arco inverso già considerato
            if (!gr->directed && gr->compare(all_nodes[i], neighbors[j]) > 0) continue;// confronta i nodi per evitare duplicati
            count++;
        }
        free(neighbors);
    }

    /* Alloca array di Edge* (terminato da NULL) */
    Edge** edges = malloc(sizeof(Edge*) * (count + 1));
    if (edges == NULL) {
        printf("Error allocating memory for edges array\n");
        free(all_nodes);
        return NULL;
    }

    /* Seconda passata: popola l'array */
    size_t idx = 0;
    for (size_t i = 0; all_nodes[i] != NULL; i++) {
        Adjacency* adj = (Adjacency*) hash_table_get(gr->nodes, all_nodes[i]);
        if (adj == NULL || adj->neighbors == NULL) continue;
        void** neighbors = hash_table_keyset(adj->neighbors);
        if (neighbors == NULL) continue;
        for (size_t j = 0; neighbors[j] != NULL; j++) {
            if (!gr->directed && gr->compare(all_nodes[i], neighbors[j]) > 0) continue;

            Edge* e = malloc(sizeof(Edge));
            if (e == NULL) {
                printf("Error allocating memory for edge\n");
                for (size_t k = 0; k < idx; k++) free(edges[k]);
                free(edges);
                free(neighbors);
                free(all_nodes);
                return NULL;
            }
            e->source = all_nodes[i];
            e->dest = neighbors[j];
            e->label = hash_table_get(adj->neighbors, neighbors[j]);
            edges[idx++] = e;
        }
        free(neighbors);
    }

    edges[idx] = NULL;
    free(all_nodes);
    return edges;
}

void* graph_get_label(const Graph gr, const void* node1, const void* node2) { 
    if(gr == NULL) return NULL;
    if(!graph_contains_node(gr, node1) || !graph_contains_edge(gr, node1, node2)) return NULL; // se il nodo 1 o l'arco non esiste, restituisce NULL

    Adjacency* adj1 = (Adjacency*) hash_table_get(gr->nodes, node1); // si ottiene la struct adjacency del nodo1 (la value del hashNode) che contiene la tabella dei nodi adiacenti
    if(adj1 == NULL) return NULL; // controllo di sicurezza
    return hash_table_get(adj1->neighbors, node2); // restituisce l'etichetta dell'arco dalla tabella dei nodi adiacenti di node1
}

void** graph_get_neighbours(const Graph gr, const void* node) {
    if(gr == NULL) return NULL;
    if(!graph_contains_node(gr, node)) return NULL; // se il nodo non esiste, restituisce NULL

    Adjacency* adj = (Adjacency*) hash_table_get(gr->nodes, node); // si ottiene la struct adjacency del nodo (la value del hashNode) che contiene la tabella dei nodi adiacenti
    if(adj == NULL) return NULL; // controllo di sicurezza
    return hash_table_keyset(adj->neighbors); // restituisce la lista dei nodi adiacenti a questo nodo
}

int graph_num_neighbours(const Graph gr, const void* node) {
    if(gr == NULL) return 0;
    if(!graph_contains_node(gr, node)) return 0; // se il nodo non esiste, restituisce 0

    Adjacency* adj = (Adjacency*) hash_table_get(gr->nodes, node); // si ottiene la struct adjacency del nodo (la value del hashNode) che contiene la tabella dei nodi adiacenti
    if(adj == NULL) return 0; // controllo di sicurezza
    return hash_table_size(adj->neighbors); // restituisce il numero di nodi adiacenti a questo nodo
}

void graph_free(Graph gr) {
    if(gr == NULL) return;
    void** all_nodes = graph_get_nodes(gr); // si ottiene la lista di tutti i nodi del grafo
    if(all_nodes == NULL) {
        hash_table_free(gr->nodes);
        free(gr);
        return;
    }
    
    for(size_t i=0; all_nodes[i] != NULL; i++) {
        Adjacency* adj = (Adjacency*) hash_table_get(gr->nodes, all_nodes[i]); // si ottiene la struct adjacency del nodo corrente
        if(adj != NULL) { // controlla che ci sia l'adiacenza del nodo
            if(adj->neighbors != NULL) {
                hash_table_free(adj->neighbors); // libera la memoria della hash table dei nodi adiacenti
                adj->neighbors = NULL; // evita dangling pointer, cioè puntatori che puntano a memoria già liberata
            }
            free(adj); // rimuove la struct adjacency allocata in graph_add_node
        }
    }
    
    free(all_nodes);
    hash_table_free(gr->nodes); // libera la memoria della tabella dei nodi del grafo
    free(gr); // libera la memoria della struct del grafo 
}