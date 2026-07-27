#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/sorting.h"

static void merge(void **base, size_t left, size_t middle, size_t right,
                  int (*compar)(const void *, const void *)) {
    size_t i = 0, j = 0, k = left;
    const size_t n1 = middle - left + 1;
    const size_t n2 = right - middle;

    void **L = (void **)malloc(n1 * sizeof(void *));
    void **R = (void **)malloc(n2 * sizeof(void *));
    if (!L || !R) { free(L); free(R); return; }

    memcpy(L, &base[left],      n1 * sizeof(void *));
    memcpy(R, &base[middle+1],  n2 * sizeof(void *));

    while (i < n1 && j < n2) {
        if (compar(L[i], R[j]) <= 0) base[k++] = L[i++];
        else                         base[k++] = R[j++];
    }
    while (i < n1) base[k++] = L[i++];
    while (j < n2) base[k++] = R[j++];

    free(L); free(R);
}

static void merge_sort_rec(void **base, size_t left, size_t right,
                           int (*compar)(const void *, const void *)) {
    if (left >= right) return;
    size_t mid = left + (right - left)/2;
    merge_sort_rec(base, left, mid, compar);
    merge_sort_rec(base, mid + 1, right, compar);
    merge(base, left, mid, right, compar);
}

void merge_sort(void **base, size_t nitems, int (*compar)(const void *, const void *)) {
    if (!base || nitems < 2 || !compar) return;
    merge_sort_rec(base, 0, nitems - 1, compar);
}

static inline void ptr_swap(void **a, void **b) {
    void *t = *a; *a = *b; *b = t;
}

static long partition(void **base, long low, long high,
                      int (*compar)(const void*, const void*)) {
    void *pivot = base[high];
    long i = low - 1;
    for (long j = low; j < high; ++j) {
        if (compar(base[j], pivot) <= 0) { // <= per ridurre gli swap inutili
            ++i;
            ptr_swap(&base[i], &base[j]);
        }
    }
    ptr_swap(&base[i + 1], &base[high]);
    return i + 1;
}

static void quick_sort_rec(void **base, long low, long high,
                           int (*compar)(const void*, const void*)) {
    while (low < high) {
        long pi = partition(base, low, high, compar);
        // Tail recursion elimination: ordina prima il lato più corto
        if (pi - 1 - low < high - (pi + 1)) {
            quick_sort_rec(base, low, pi - 1, compar);
            low = pi + 1;
        } else {
            quick_sort_rec(base, pi + 1, high, compar);
            high = pi - 1;
        }
    }
}

void quick_sort(void **base, size_t nitems, int (*compar)(const void*, const void*)) {
    if (!base || nitems < 2 || !compar) return;
    quick_sort_rec(base, 0, (long)nitems - 1, compar);
}