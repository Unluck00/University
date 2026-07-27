#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct PriorityQueue PriorityQueue;

/**
Creates a new priority queue
    -f1: compare function
    -f2: hash function for the elements
*/

PriorityQueue* priority_queue_create(int (*f1)(const void*, const void*), 
                                     unsigned long (*f2)(const void*)); // returns NULL on error

int priority_queue_push(PriorityQueue*, void*); // returns 1 if inserted, 0 if already present, -1 on error
int priority_queue_contains(const PriorityQueue*, const void*); // returns 1 if present, 0 if not present, -1 on error
void* priority_queue_top(const PriorityQueue*); // returns the top element or NULL if empty or on error
void priority_queue_pop(PriorityQueue*); // removes the top element
int priority_queue_remove(PriorityQueue*, const void*); // removes the specified element, returns 1 if removed, 0 if not found, -1 on error
int priority_queue_size(const PriorityQueue*); // returns the number of elements, -1 on error
void priority_queue_free(PriorityQueue*); // frees the priority queue

#endif // PRIORITY_QUEUE_H

