#ifndef SIMULAZIONE_POSTE_TICKET_H
#define SIMULAZIONE_POSTE_TICKET_H

#include "../utils/msg.h"
#include "../utils/sem.h"

void attach_ipc();

int generate_time_service(int time_avarage);

void interrupt_handle(int signum);
void time_handle(int signum);
void signal_handlers_init(struct sigaction *saINT, struct sigaction *saUSR1);


#endif // SIMULAZIONE_POSTE_TICKET_H