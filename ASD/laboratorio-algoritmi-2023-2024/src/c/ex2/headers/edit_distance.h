#ifndef EDIT_DISTANCE_H
#define EDIT_DISTANCE_H

// Max size of the distance or capacity
#define MAX_SIZE 500
#define MAX_CHARACTERS 50

typedef struct dictionary_t {
    char word[MAX_CHARACTERS];
} dictionary;

int min_three_operations(int no_op, int canc, int ins);
int edit_distance(const char *s1, const char *s2);
int edit_distance_dyn_index(const char *s1, const char *s2, int i, int j, int mem_bid[][MAX_SIZE]);
int edit_distance_dyn(const char *s1, const char *s2);

#endif