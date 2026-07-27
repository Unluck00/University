#ifndef SIMULAZIONE_POSTE_SEM_H
#define SIMULAZIONE_POSTE_SEM_H

#include "../headers/common.h"  // dovrebbe andare sto collegamento (nel caso controlla la disposizione dei folder)


union semun {
	int val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
				    (Linux-specific) */
};

int sem_set(int sem_id, int sem_num);
int sem_set_all(int sem_id, int sem_num);
int sem_reserve(int sem_id, int sem_num);
int sem_release(int sem_id, int sem_num);

#endif /* SIMULAZIONE_POSTE_SEM_H */