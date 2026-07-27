#ifndef SIMULAZIONE_POSTE_OPERATOR_H
#define SIMULAZIONE_POSTE_OPERATOR_H

#include "../utils/msg.h"
#include "../utils/sem.h"

void attach_ipc();

void seats_search(int sevice_type_operator);
void take_break();

void time_handle(int signum);
void interrupt_handle(int signum);
void signal_handlers_init(struct sigaction *saINT, struct sigaction *saUSR1);

#endif // SIMULAZIONE_POSTE_OPERATOR_H