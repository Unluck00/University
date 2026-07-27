#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/priority_queue.h"
#include "headers/task.h"

static int parse_line_task(const char* line, Task* out) {
    if (sscanf(line, " %d , %d , %d , %d", &out->id, &out->start, &out->length, &out->priority) == 4) {
        return 1;
    }
    if (sscanf(line, " %d ; %d ; %d ; %d", &out->id, &out->start, &out->length, &out->priority) == 4) {
        return 1;
    }
    return 0;
}

static int read_tasks_csv(const char* path, Task** out_tasks, int* out_n) {
    FILE* f = fopen(path, "r");
    if (!f) return 0;

    int cap = 128;
    int n = 0;
    Task* tasks = malloc(sizeof(Task) * (size_t)cap);
    if (!tasks) { fclose(f); return 0; }

    char buf[256];
    while (fgets(buf, sizeof(buf), f)) {
        
        if (buf[0] == '\n' || buf[0] == '\r') continue;

        if (strstr(buf, "ID") && strstr(buf, "Start") && strstr(buf, "Length")) continue;

        Task t;
        if (!parse_line_task(buf, &t)) continue;

        if (n == cap) { 
            cap *= 2;
            Task* tmp = realloc(tasks, sizeof(Task) * (size_t)cap);
            if (!tmp) { free(tasks); fclose(f); return 0; }
            tasks = tmp;
        }

        tasks[n++] = t;
    }

    fclose(f);
    *out_tasks = tasks;
    *out_n = n;
    return 1;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    Task* tasks = NULL;
    int n = 0;
    if (!read_tasks_csv(argv[1], &tasks, &n)) {
        fprintf(stderr, "Errore: impossibile leggere il file %s\n", argv[1]);
        return 1;
    }

    FILE* out = fopen(argv[2], "w");
    if (!out) {
        fprintf(stderr, "Errore: impossibile aprire output %s\n", argv[2]);
        free(tasks);
        return 1;
    }

    fprintf(out, "ID,StartTime,EndTime\n");

    PriorityQueue* pq = priority_queue_create(compare_task, hash_task);
    if (!pq) {
        fprintf(stderr, "Errore: impossibile creare la PriorityQueue\n");
        fclose(out);
        free(tasks);
        return 1;
    }

    int t = 0;      
    int i = 0;      
    int done = 0;   

    while (done < n) {
        
        while (i < n && tasks[i].start <= t) {
            
            (void)priority_queue_push(pq, &tasks[i]);
            i++;
        }

        if (priority_queue_size(pq) == 0) {
            t += 1;
            continue;
        }

        Task* next = (Task*)priority_queue_top(pq);
        if (!next) {
            t += 1;
            continue;
        }

        int start_time = t;
        int end_time = t + next->length;

        fprintf(out, "%d,%d,%d\n", next->id, start_time, end_time);

        priority_queue_pop(pq);
        done++;

        t = end_time;
    }

    priority_queue_free(pq);
    fclose(out);
    free(tasks);
    return 0;
}
