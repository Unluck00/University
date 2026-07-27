#ifndef SIMULAZIONE_POSTE_USER_H
#define SIMULAZIONE_POSTE_USER_H

#include "../utils/msg.h"
#include "../utils/sem.h"

void attach_ipc();

void probability_visiting(float p_serv);
void wait_user_time(int time_user);
bool check_seats_available(int service_user);
int choose_best_desk(int service_user);
void free_desk(int current_desk);

void time_handle(int signum);
void interrupt_handle(int signum);
void signal_handlers_init(struct sigaction *saINT, struct sigaction *saUSR1);

#endif // SIMULAZIONE_POSTE_USER_H