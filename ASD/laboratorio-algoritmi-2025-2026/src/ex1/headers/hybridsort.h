#ifndef HYBRIDSORTH_H
#define HYBRIDSORTH_H

#include <stddef.h>

/**

@param base Pointer to first element of the array
@param nitems Number of items in the array
@param size Size (in bytes) of each element 
@param k Threshold:
            -if k==0 use only QuickSort
            -if k < nitmes use only SelectionSort
            -else QuickSort for subarrays >= k, SelectionSort for subarrays < k
@param compar Comparison function
*/

void hybridsort(void *base, size_t nitems, size_t size, size_t k,
                int (*compar)(const void *, const void *));

#endif /* HYBRIDSORTH_H */