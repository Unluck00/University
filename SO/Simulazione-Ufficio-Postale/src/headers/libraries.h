#ifndef SIMULAZIONE_POSTE_LIBRARIES_H
#define SIMULAZIONE_POSTE_LIBRARIES_H

#include <stdio.h> //per le printf e per i file
#include <unistd.h> //funzioni per processi (fork, getpid)
#include <stdlib.h> //funzioni per exit(), malloc(), atoi(), NULL; terminazione del programma con un codice di uscita EXIT_FAILURE
#include <errno.h> //funzioni per gestione degli errori (errno, perror, strerror), per l'uso della variavile errno, che memorizza l'ultimo codice di errore memorizzato
#include <string.h> //funzioni che manipolano le stringhe
#include <time.h>  //utilizzo della struct timespec per rappresentare il tempo
#include <signal.h> // funzioni per la gestione dei segnali
#include <stdbool.h> // uso di bool come dato (TRUE, FALSE)

#include <sys/types.h> //dichiarazioni di tipi di dati (key_t, pid_t)
#include <sys/shm.h> //funzioni necessarie per l'uso della memoria condivisa
#include <sys/sem.h> //funzioni necessarie per l'uso dei semafori
#include <sys/msg.h> //funzioni necessarie per l'uso delle code di messaggi
#include <sys/ipc.h> // strutture per l'IPC 


#endif  // SIMULAZIONE_POSTE_LIBRARIES_H