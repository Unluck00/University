#include "sem.h"

int sem_set(int sem_id, int sem_num) {

    union semun arg;
    arg.val = 1;

    return semctl(sem_id, sem_num, SETVAL, arg);
}

int sem_set_all(int sem_id, int sem_num) {

    union semun arg;
    unsigned short values[sem_num];
    for(int i=0; i<sem_num; i++) {
        values[i] = 1; 
    }
    arg.array = values;

    return semctl(sem_id, sem_num, SETALL, arg);  // sem_num dovrebbe essere un argomento ignorato
}

int sem_reserve(int sem_id, int sem_num) {

    struct sembuf sops;

    sops.sem_num = sem_num;
    sops.sem_op = -1;
    sops.sem_flg = 0;

    return semop(sem_id, &sops, 1);
}

int sem_release(int sem_id, int sem_num) {

    struct sembuf sops;

    sops.sem_num = sem_num;
    sops.sem_op = 1;
    sops.sem_flg = 0;

    return semop(sem_id, &sops, 1);    
}