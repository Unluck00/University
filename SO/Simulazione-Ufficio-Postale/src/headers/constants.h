#ifndef SIMULAZIONE_POSTE_CONSTANTS_H
#define SIMULAZIONE_POSTE_CONSTANTS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

/* -- IPC OBJECTS -- */
#define SHM_PARAMETERS 1337
#define SHM_STATISTICS 1338
#define SHM_DESKS_ARRAY 1339
#define SHM_TIME_SIMULATED 1340
#define SEM_STATISTICS 420
#define SEM_DESKS 421
#define SEM_TIME_SIMULATED 422
#define QUEUE_TICKETS 841
#define QUEUE_WAITING 842

#define MAX_BUFFER 128
#define MAX_CHARACTERS 64 

#define MINUTES_WORK_DAY 300 //valgono a 5 ore di lavoro al giorno

#define CONFIG_FILE "config.conf"
#define TICKET_NAME "/.ticket"
#define OPERATOR_NAME "/.operator"
#define USER_NAME "/.user"


#endif  // SIMULAZIONE_POSTE_CONSTANTS_H