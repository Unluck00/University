#ifndef SIMULAZIONE_POSTE_MSG_H
#define SIMULAZIONE_POSTE_MSG_H

#include "../headers/common.h"  // dovrebbe andare sto collegamento (nel caso controlla la disposizione dei folder)

int send_message(int queueID, void *msg,int size, int waitFlag);
int receive_message(int queueID, void *msg, int size, int mtype, int flag);

#endif /* SIMULAZIONE_POSTE_MSH_H */