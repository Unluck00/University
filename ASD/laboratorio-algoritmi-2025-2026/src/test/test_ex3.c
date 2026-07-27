#include <stdio.h>
#include <stdlib.h>

#include "../ex3/headers/priority_queue.h"
#include "../ex3/headers/task.h"

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("FAIL (line %d): %s\n", __LINE__, #cond); \
        return 1; \
    } \
} while(0)

static Task T(int id, int start, int length, int priority) {
    Task t;
    t.id = id;
    t.start = start;
    t.length = length;
    t.priority = priority;
    return t;
}

int main(void) {
    PriorityQueue* pq = priority_queue_create(compare_task, hash_task);
    ASSERT_TRUE(pq != NULL);
    ASSERT_TRUE(priority_queue_size(pq) == 0);

    Task a = T(1, 0, 5, 10);
    Task b = T(2, 0, 3, 20);
    Task c = T(3, 0, 2, 15);

    ASSERT_TRUE(priority_queue_push(pq, &a) == 1);
    ASSERT_TRUE(priority_queue_push(pq, &b) == 1);
    ASSERT_TRUE(priority_queue_push(pq, &c) == 1);

    ASSERT_TRUE(priority_queue_size(pq) == 3);

    Task* top = (Task*)priority_queue_top(pq);
    ASSERT_TRUE(top != NULL);
    ASSERT_TRUE(top->id == 2);

    ASSERT_TRUE(priority_queue_contains(pq, &a) == 1);
    ASSERT_TRUE(priority_queue_contains(pq, &b) == 1);
    ASSERT_TRUE(priority_queue_contains(pq, &c) == 1);

    Task b_dup = T(2, 999, 999, 999); 
    ASSERT_TRUE(priority_queue_push(pq, &b_dup) == 0);
    ASSERT_TRUE(priority_queue_size(pq) == 3);

    priority_queue_pop(pq);
    ASSERT_TRUE(priority_queue_size(pq) == 2);

    top = (Task*)priority_queue_top(pq);
    ASSERT_TRUE(top != NULL);
    ASSERT_TRUE(top->id == 3);

    ASSERT_TRUE(priority_queue_remove(pq, &a) == 1);
    ASSERT_TRUE(priority_queue_contains(pq, &a) == 0);
    ASSERT_TRUE(priority_queue_size(pq) == 1);

    Task x = T(99, 0, 1, 1);
    ASSERT_TRUE(priority_queue_remove(pq, &x) == 0);

    priority_queue_free(pq);
    printf("OK\n");
    return 0;
}
