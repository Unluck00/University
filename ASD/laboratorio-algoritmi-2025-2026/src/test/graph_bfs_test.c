#include <stdlib.h>
#include <string.h>
#include "unity/unity.h"
#include "../ex2/headers/hash_table.h"
#include "../ex4/headers/graph.h"
#include "../ex4/headers/bfs.h"

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

static Graph graph;
static Graph graph_iniz;

void setUp(void) {
    graph_iniz = graph_create(1, 0, str_equals, str_hash); // grafo etichettato e non diretto
    graph_add_node(graph_iniz, "A");
    graph_add_node(graph_iniz, "B");
    graph_add_node(graph_iniz, "C");
    graph_add_node(graph_iniz, "D");
    graph_add_node(graph_iniz, "E");

    float* dist_ab = malloc(sizeof(float)); *dist_ab = 1.0;
    float* dist_ac = malloc(sizeof(float)); *dist_ac = 2.0;
    float* dist_bd = malloc(sizeof(float)); *dist_bd = 3.0;
    float* dist_ce = malloc(sizeof(float)); *dist_ce = 4.0;
    float* dist_de = malloc(sizeof(float)); *dist_de = 5.0;

    graph_add_edge(graph_iniz, "A", "B", dist_ab);
    graph_add_edge(graph_iniz, "A", "C", dist_ac);
    graph_add_edge(graph_iniz, "B", "D", dist_bd);
    graph_add_edge(graph_iniz, "C", "E", dist_ce);
    graph_add_edge(graph_iniz, "D", "E", dist_de);
}

void tearDown(void) {
    if (graph != NULL) {
        graph_free(graph);
        graph = NULL;
    }
    if (graph_iniz != NULL) {
        graph_free(graph_iniz);
        graph_iniz = NULL;
    }
}

void test_graph_create(void) {
    graph = graph_create(1, 1, str_equals, str_hash);
    TEST_ASSERT_NOT_NULL(graph);
    TEST_ASSERT_EQUAL(1, graph_is_directed(graph));
    TEST_ASSERT_EQUAL(1, graph_is_labelled(graph)); 
}

void test_graph_create_unlabelled_undirected(void) {
    graph = graph_create(0, 0, str_equals, str_hash);
    TEST_ASSERT_NOT_NULL(graph);
    TEST_ASSERT_EQUAL(0, graph_is_directed(graph));
    TEST_ASSERT_EQUAL(0, graph_is_labelled(graph));
}

void test_graph_nodes_basic(void) {
    graph = graph_create(1, 1, str_equals, str_hash);

    TEST_ASSERT_EQUAL(0, graph_num_nodes(graph));

    TEST_ASSERT_EQUAL(1, graph_add_node(graph, "a"));
    TEST_ASSERT_EQUAL(1, graph_add_node(graph, "b"));
    TEST_ASSERT_EQUAL(1, graph_add_node(graph, "c"));

    TEST_ASSERT_EQUAL(3, graph_num_nodes(graph));

    TEST_ASSERT_EQUAL(1, graph_contains_node(graph, "a"));
    TEST_ASSERT_EQUAL(0, graph_contains_node(graph, "z"));

    TEST_ASSERT_EQUAL(1, graph_remove_node(graph, "b"));
    TEST_ASSERT_EQUAL(2, graph_num_nodes(graph));
}

void test_graph_edges_directed_labelled(void) {
    graph = graph_create(1, 1, str_equals, str_hash);

    graph_add_node(graph, "a");
    graph_add_node(graph, "b");

    TEST_ASSERT_EQUAL(1, graph_add_edge(graph, "a", "b", "lab"));
    TEST_ASSERT_EQUAL(1, graph_contains_edge(graph, "a", "b"));
    TEST_ASSERT_EQUAL(0, graph_contains_edge(graph, "b", "a")); // diretto

    TEST_ASSERT_EQUAL_STRING("lab", graph_get_label(graph, "a", "b"));

    TEST_ASSERT_EQUAL(1, graph_remove_edge(graph, "a", "b"));
    TEST_ASSERT_EQUAL(0, graph_contains_edge(graph, "a", "b"));
}

void test_graph_edges_undirected_unlabelled(void) {
    graph = graph_create(0, 0, str_equals, str_hash);

    graph_add_node(graph, "a");
    graph_add_node(graph, "b");

    TEST_ASSERT_EQUAL(1, graph_add_edge(graph, "a", "b", NULL));

    // doppia adiacenza
    TEST_ASSERT_EQUAL(1, graph_contains_edge(graph, "a", "b"));
    TEST_ASSERT_EQUAL(1, graph_contains_edge(graph, "b", "a"));

    TEST_ASSERT_NULL(graph_get_label(graph, "a", "b"));
    TEST_ASSERT_NULL(graph_get_label(graph, "b", "a"));
}

void test_graph_neighbours(void) {
    graph = graph_create(1, 1, str_equals, str_hash);

    graph_add_node(graph, "a");
    graph_add_node(graph, "b");
    graph_add_node(graph, "c");

    graph_add_edge(graph, "a", "b", "lab1");
    graph_add_edge(graph, "a", "c", "lab2");

    TEST_ASSERT_EQUAL(2, graph_num_neighbours(graph, "a"));

    void** neigh = graph_get_neighbours(graph, "a");
    TEST_ASSERT_NOT_NULL(neigh);

    int found_b = 0, found_c = 0;
    for (int i = 0; i < 2; i++) {
        if (strcmp(neigh[i], "b") == 0) found_b = 1;
        if (strcmp(neigh[i], "c") == 0) found_c = 1;
    }

    TEST_ASSERT_TRUE(found_b);
    TEST_ASSERT_TRUE(found_c);

    free(neigh);
}

void test_graph_get_nodes_and_edges(void) {
    graph = graph_create(1, 0, str_equals, str_hash);

    graph_add_node(graph, "a");
    graph_add_node(graph, "b");
    graph_add_node(graph, "c");

    graph_add_edge(graph, "a", "b", "L1");
    graph_add_edge(graph, "b", "c", "L2");

    void** nodes = graph_get_nodes(graph);
    TEST_ASSERT_NOT_NULL(nodes);

    int count = 0;
    for (int i = 0; i < 3; i++)
        if (nodes[i] != NULL) count++;

    TEST_ASSERT_EQUAL(3, count);

    Edge** edges = graph_get_edges(graph);
    TEST_ASSERT_NOT_NULL(edges);

    // per non-diretto ci sono 4 archi (a,b + b,a + b,c + c,b)
    int edge_count = 0;
    while (edges[edge_count] != NULL)
        edge_count++;

    TEST_ASSERT_EQUAL(4, edge_count);

    for (int i = 0; i < edge_count; i++)
        free(edges[i]);
    free(edges);
}

void test_bfs_simple(void) {
    graph = graph_create(0, 0, str_equals, str_hash);

    graph_add_node(graph, "A");
    graph_add_node(graph, "B");
    graph_add_node(graph, "C");
    graph_add_node(graph, "D");

    graph_add_edge(graph, "A", "B", NULL);
    graph_add_edge(graph, "A", "C", NULL);
    graph_add_edge(graph, "B", "D", NULL);

    void** res = breadth_first_visit(graph, "A", str_equals, str_hash);

    // risultato atteso: A B C D  (o A C B D, entrambi validi)
    TEST_ASSERT_NOT_NULL(res);

    TEST_ASSERT_EQUAL_STRING("A", res[0]);
    TEST_ASSERT_TRUE(
        strcmp(res[1], "B") == 0 || strcmp(res[1], "C") == 0
    );
    TEST_ASSERT_TRUE(
        strcmp(res[2], "B") == 0 || strcmp(res[2], "C") == 0
    );
    TEST_ASSERT_EQUAL_STRING("D", res[3]);

    free(res);
}

void test_check_bfs_complete(void) {
    printf("Controllo 1\n");
    void** res = breadth_first_visit(graph_iniz, "A", str_equals, str_hash);

    // risultato atteso: A B C D E (in questo ordine specifico)
    TEST_ASSERT_NOT_NULL(res);

    printf("Controllo 2\n");
    TEST_ASSERT_EQUAL_STRING("A", res[0]);
    TEST_ASSERT_EQUAL_STRING("B", res[1]);
    TEST_ASSERT_EQUAL_STRING("C", res[2]);
    TEST_ASSERT_EQUAL_STRING("D", res[3]);
    TEST_ASSERT_EQUAL_STRING("E", res[4]);

    printf("Controllo 3\n");

    free(res);

    printf("Controllo 4\n");
}

void test_graph_add_edge_invalid_nodes(void) {
    graph = graph_create(1, 1, str_equals, str_hash);

    graph_add_node(graph, "A");

    // Nodo B NON esiste → dovrebbe restituire 0, NON crashare
    TEST_ASSERT_EQUAL(0, graph_add_edge(graph, "A", "B", "x"));
    TEST_ASSERT_EQUAL(0, graph_add_edge(graph, "B", "A", "x"));
}

void test_graph_remove_edge_invalid_nodes(void) {
    graph = graph_create(1, 0, str_equals, str_hash);

    graph_add_node(graph, "A");

    TEST_ASSERT_EQUAL(0, graph_remove_edge(graph, "A", "B"));
    TEST_ASSERT_EQUAL(0, graph_remove_edge(graph, "B", "A"));
}

void test_graph_get_label_invalid_nodes(void) {
    graph = graph_create(1, 1, str_equals, str_hash);

    graph_add_node(graph, "A");
    graph_add_node(graph, "B");
    graph_add_edge(graph, "A", "B", "lab");

    TEST_ASSERT_NULL(graph_get_label(graph, "A", "Z"));
    TEST_ASSERT_NULL(graph_get_label(graph, "Z", "A"));
}

void test_graph_contains_edge_invalid_nodes(void) {
    graph = graph_create(0, 0, str_equals, str_hash);

    graph_add_node(graph, "A");

    TEST_ASSERT_EQUAL(0, graph_contains_edge(graph, "A", "B"));
    TEST_ASSERT_EQUAL(0, graph_contains_edge(graph, "B", "A"));
}

void test_graph_neighbours_invalid_node(void) {
    graph = graph_create(1, 0, str_equals, str_hash);

    graph_add_node(graph, "A");

    // Nodo inesistente → deve ritornare NULL, NON segfault
    void** n = graph_get_neighbours(graph, "B");
    TEST_ASSERT_NULL(n);
}

void test_graph_get_nodes_empty_graph(void) {
    graph = graph_create(1, 0, str_equals, str_hash);

    void** nodes = graph_get_nodes(graph);
    TEST_ASSERT_NOT_NULL(nodes);
    TEST_ASSERT_NULL(nodes[0]); // nessun nodo

    free(nodes);
}

void test_graph_get_edges_empty_graph(void) {
    graph = graph_create(1, 0, str_equals, str_hash);

    Edge** edges = graph_get_edges(graph);
    TEST_ASSERT_NOT_NULL(edges);
    TEST_ASSERT_NULL(edges[0]); // nessun arco

    free(edges);
}

void test_bfs_from_nonexistent_node(void) {
    graph = graph_create(0, 0, str_equals, str_hash);

    graph_add_node(graph, "A");
    graph_add_node(graph, "B");

    // BFS da nodo NON presente → deve ritornare NULL
    void** res = breadth_first_visit(graph, "Z", str_equals, str_hash);
    TEST_ASSERT_NULL(res);
}

void test_bfs_empty_graph(void) {
    graph = graph_create(0, 0, str_equals, str_hash);

    void** res = breadth_first_visit(graph, "A", str_equals, str_hash);
    TEST_ASSERT_NULL(res);
}

int main(void) {

    UNITY_BEGIN();

    RUN_TEST(test_graph_create);
    RUN_TEST(test_graph_create_unlabelled_undirected);
    RUN_TEST(test_graph_nodes_basic);
    RUN_TEST(test_graph_edges_directed_labelled);
    RUN_TEST(test_graph_edges_undirected_unlabelled);
    RUN_TEST(test_graph_neighbours);
    RUN_TEST(test_graph_get_nodes_and_edges);
    
    RUN_TEST(test_bfs_simple);
    RUN_TEST(test_check_bfs_complete);

    RUN_TEST(test_graph_add_edge_invalid_nodes);
    RUN_TEST(test_graph_remove_edge_invalid_nodes);
    RUN_TEST(test_graph_get_label_invalid_nodes);
    RUN_TEST(test_graph_contains_edge_invalid_nodes);
    RUN_TEST(test_graph_neighbours_invalid_node);
    RUN_TEST(test_graph_get_nodes_empty_graph);
    RUN_TEST(test_graph_get_edges_empty_graph);

    RUN_TEST(test_bfs_from_nonexistent_node);
    RUN_TEST(test_bfs_empty_graph);
    
    return UNITY_END();
}