#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "headers/edit_distance.h"

int min_three_operations(int no_op, int canc, int ins) {
    int min = no_op;

    if(canc < min) min = canc;
    if(ins < min) min = ins;

    return min;
}

int edit_distance(const char *s1, const char *s2) {
    if(*s1 == '\0') return (int)strlen(s2); // if s1 is empty then need |s2| removes
    if(*s2 == '\0') return (int)strlen(s1); // if s2 is empty then need |s1| insertions

    // no_op only if the first chars are equal
    int no_op;

    if(*s1 == *s2) {
        no_op = edit_distance(s1+1, s2+1);
    }
    else {
        no_op = MAX_SIZE;
    }

    // canc remove s2[0]
    // ins insert s1[0] after s2[0]
    int canc, ins;

    canc = 1 + edit_distance(s1, s2+1);
    ins = 1 + edit_distance(s1+1, s2);

    // min between the three operations
    return min_three_operations(no_op, canc, ins);
}

int edit_distance_dyn_index(const char *s1, const char *s2, int i, int j, int mem_bid[][MAX_SIZE]) {
    if(mem_bid[i][j] != -1) return mem_bid[i][j];
    
    if(s1[i] == '\0') return mem_bid[i][j] = (int)strlen(s2+j); // if s1[i] is empty then need |s2+j| removes
    if(s2[j] == '\0') return mem_bid[i][j] = (int)strlen(s1+i); // if s2[j] is empty then need |s1+i| insertions

    // no_op only if the first chars are equal
    int no_op = INT_MAX;

    if(s1[i] == s2[j]) no_op = edit_distance_dyn_index(s1, s2, i+1, j+1, mem_bid);

    // canc remove s2[j]
    // ins insert s1[i] after s2[j]
    int canc, ins;

    canc = 1 + edit_distance_dyn_index(s1, s2, i, j+1, mem_bid);
    ins = 1 + edit_distance_dyn_index(s1, s2, i+1, j, mem_bid);

    // min between the three operations
    return mem_bid[i][j] = min_three_operations(no_op, canc, ins);
}

int edit_distance_dyn(const char *s1, const char *s2) {
    // table results in bidimensional
    int mem_bid[MAX_SIZE][MAX_SIZE];
    memset(mem_bid, -1, sizeof(mem_bid)); // the value -1 is not calculated
    // function for recursion with memoization
    return edit_distance_dyn_index(s1, s2, 0, 0, mem_bid); 
}