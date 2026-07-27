#ifndef SIMULAZIONE_POSTE_DIRECTOR_H
#define SIMULAZIONE_POSTE_DIRECTOR_H

#include "../utils/msg.h"
#include "../utils/sem.h"

void shared_memory_init();
void semaphores_init();
void queue_init();
void cleanup_IPC();

void spawn_operator(pid_t *operators, int oCounter);
void spawn_user(pid_t *users, int uCounter);

void ratio_operator_desk();

void print_daily_statistics(int day);
void print_total_statistics(int type_termination);

void signal_handle(int signum);

#endif // SIMULAZIONE_POSTE_DIRECTOR_H