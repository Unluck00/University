#include "headers/hybridsort.h"
#include <stdlib.h>
#include <string.h>

static void swap_bytes(char *a, char *b, size_t size) {
    if (a == b) {
        return;
    }

    for (size_t i = 0; i < size; i++) {
        char temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}

static void selection_sort_subarray(char *base, size_t left, size_t right, size_t size,
                                    int (*compar)(const void *, const void *)) {
    for (size_t i = left; i < right; i++) {
        size_t min_index = i;
        for (size_t j = i + 1; j <= right; j++) {
            if (compar(base + j * size, base + min_index * size) < 0) {
                min_index = j;
            }
        }
        if (min_index != i) {
            swap_bytes(base + i * size, base + min_index * size, size);
        }
    }
}

static void quicksort_hybrid(char *base, size_t left, size_t right, size_t size, size_t k,
                             int (*compar)(const void *, const void *)) {
    if (right <= left) {
        return;
    }

    size_t length = right - left + 1;

    if (k > 0 && length < k) {
        selection_sort_subarray(base, left, right, size, compar);
        return;
    }

    size_t mid = left + (right - left) / 2; // scelta del pivot come elemento centrale
    char *pivot = malloc(size); // allocazione dinamica per il pivot
    if (!pivot) {
        selection_sort_subarray(base, left, right, size, compar);
        return;
    }
    memcpy(pivot, base + mid * size, size); // copia del pivot in memoria, in modo da non perderlo durante le operazioni di swap

    size_t lt = left; // indice per il confine degli elementi < del pivot
    size_t i  = left; // indice corrente che scorre l'array
    size_t gt = right; // indice per il confine degli elementi > del pivot

    while (i <= gt) {
        int cmp = compar(base + i * size, pivot); // confronto elemento con il pivot
        if (cmp < 0) { // elemento < pivot
            swap_bytes(base + lt * size, base + i * size, size); // scambia l'elemento corrente con l'elemento alla posizione lt
            lt++; // sposta in avanti il confine degli elementi < pivot
            i++; // avanza l'indice corrente
        } else if (cmp > 0) { // elemento > pivot
            swap_bytes(base + i * size, base + gt * size, size); // scambia l'elemento corrente con l'elemento alla posizione gt
            if (gt == 0) {  // prevenzione underflow
                break;
            }
            gt--; // sposta indietro il confine degli elementi > pivot
        } else {
            i++; // elemento == pivot, solo avanza l'indice corrente
        }
    }

    free(pivot);

    if (lt > left) { // ricorsione sulla parte sinistra
        quicksort_hybrid(base, left, lt - 1, size, k, compar); 
    }
    if (gt < right) { // ricorsione sulla parte destra
        quicksort_hybrid(base, gt + 1, right, size, k, compar);
    }
}

void hybridsort(void *base, size_t nitems, size_t size, size_t k,
                int (*compar)(const void *, const void *)) {
    if (!base || nitems <= 1 || !compar || size == 0) {
        return;
    }

    char *cbase = (char *)base;
    quicksort_hybrid(cbase, 0, nitems - 1, size, k, compar);
}