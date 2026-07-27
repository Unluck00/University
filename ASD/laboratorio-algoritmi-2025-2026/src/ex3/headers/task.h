#ifndef TASK_H
#define TASK_H

typedef struct {
    int id;
    int start;
    int length;
    int priority;
} Task;

int compare_task(const void *a, const void *b);

unsigned long hash_task(const void *task);

#endif // TASK_H