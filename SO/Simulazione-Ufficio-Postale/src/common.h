// common.h - Strutture dati e costanti condivise
#ifndef UFFICIO_POSTALE_COMMON_H
#define UFFICIO_POSTALE_COMMON_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// Servizi disponibili
typedef enum {
    PACCHI,
    LETTERE,
    BANCOPOSTA,
    BOLLETTINI,
    FINANZIARI,
    OROLOGI,
    NUM_SERVIZI
} TipoServizio;

// Struttura per il ticket
typedef struct {
    long mtype;           // Tipo di messaggio per la coda
    int numero_ticket;    // Numero progressivo del ticket
    TipoServizio servizio;// Tipo di servizio richiesto
    pid_t pid_utente;     // PID del processo utente
    time_t timestamp;     // Timestamp della richiesta
} Ticket;

// Struttura per la memoria condivisa delle statistiche
typedef struct {
    int servizi_erogati[NUM_SERVIZI];
    int servizi_non_erogati[NUM_SERVIZI];
    int utenti_in_attesa[NUM_SERVIZI];
    double tempo_attesa_totale[NUM_SERVIZI];
    int num_ticket_totali;
} StatisticheUfficio;

// Chiavi per IPC
#define KEY_BASE 1234
#define SHM_KEY (KEY_BASE + 1)
#define SEM_KEY (KEY_BASE + 2)
#define MSG_KEY (KEY_BASE + 3)

// Indici semafori
#define SEM_MUTEX_STATS 0
#define SEM_TICKET_DISP 1
#define NUM_SEMAFORI 2

// Probabilità di servizio
#define P_SERV_MIN 0.2
#define P_SERV_MAX 0.8

#endif // COMMON_H