#include "headers/task.h"

int compare_task(const void *a, const void *b) {
    const Task *t1 = (const Task *)a;
    const Task *t2 = (const Task *)b;

    if (t1->id == t2->id) return 0;

    if (t1->priority > t2->priority) return 1;
    if (t1->priority < t2->priority) return -1;

    if (t1->start < t2->start) return 1;
    if (t1->start > t2->start) return -1;

    if (t1->id < t2->id) return 1;
    return -1;
}

unsigned long hash_task(const void *task) {
    const Task *t = (const Task *)task;
    return (unsigned long)t->id;
}
