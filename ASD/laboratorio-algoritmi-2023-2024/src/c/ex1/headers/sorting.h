#ifndef LABORATORIO_DI_ALGORITMI_2023_2024_SORTING_H
#define LABORATORIO_DI_ALGORITMI_2023_2024_SORTING_H

#include <stdio.h>

void merge_sort(void **base, size_t nitems, int (*compar)(const void *, const void *));
void quick_sort(void **base, size_t nitems, int (*compar)(const void*, const void*));

#endif