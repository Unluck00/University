#include "headers/common.h"
#include "include/ticket.h"

struct parameters *par;
statistic *stats;
// desk *desks; NON DOVREBBE SERVIRE
// time_sim *time_data; NON DOVREBBE SERVIRE

struct msgbuf_tickets ticketMessage;

sigset_t setMask; // permette di contenere più segnali che verranno gestiti (bloccati/sbloccati)

int shmPar_ID;
int shmStats_ID;
int semStats_ID;
int queueTickets_ID;

void attach_ipc() {
    shmPar_ID = shmget(ftok("director.c", SHM_PARAMETERS), 0, 0);  // si ottiene l'identificatore del IPC
    shmStats_ID = shmget(ftok("director.c", SHM_STATISTICS), 0, 0);

    par = (struct parameters *)shmat(shmPar_ID, NULL, 0);
    stats = (statistic *)shmat(shmStats_ID, NULL, 0);

    semStats_ID = semget(ftok("director.c", SEM_STATISTICS), 0, 0); 
    queueTickets_ID = msgget(ftok("director.c", QUEUE_TICKETS), 0);
}

int generate_time_service(int time_avarage) {
    /* https://www.hosthello.com/posts/how-rand-function-works */

    /* IL CALCOLO +-50% DOVREBBE ESSERE APPROPRIATO SCRITTO COSI, NEL CASO CONTROLLARE */ 
    int variation = time_avarage / 2;
    return variation + (rand() % (time_avarage + 1));
}

void interrupt_handle(int signum) { // gestore di segnale per chiusura del processo dovuta alla fine della simulazione
    
    printf("Chiusura del processo erogatore ticket\n");

    shmdt(par);
    shmdt(stats);
    
    exit(0);
}

void time_handle(int signum) { // gestore di segnale per fine e inizio giornata (in modo da sincronizzare il tempo simulato con il processo direttore)

    /*
    struct msqid_ds buf;
    msgctl(queueTickets_ID, IPC_STAT, &buf);  // Ottieni il numero di messaggi
    printf("There are %d messages in queue\n", buf.msg_qnum);
    */

    while(1) {
        if(receive_message(queueTickets_ID, &ticketMessage, sizeof(ticketMessage) - sizeof(long), 0, IPC_NOWAIT) == -1) { // nel caso ci siano problemi con la coda IPC_NOWAIT almeno restituira un errore immediato
            break;
        }
    }
}

void signal_handlers_init(struct sigaction *saINT, struct sigaction *saUSR1) {
    // POSSIBILE MIGLIORAMENTO DA VEDERE
    saINT.sa_handler = interrupt_handle; // segnale di fine simulazione, che interrompe e chiude il processo corrente
    sigemptyset(&saINT.sa_mask);
    saINT.sa_flags = 0;
    
    saUSR1.sa_handler = time_handle; // segnale di fine giornata, che permette di interrompere temporaneamente l'erogatore affinchè si prepari per la nuova giornata
    sigemptyset(&saUSR1.sa_mask);
    saUSR1.sa_flags = SA_RESTART;  // in questo modo se arriva il segnale di fine giornata è si sta leggendo il messaggio (msgrcv) allora il kernel permette di riavviare il segnale (senza restituire errore EINTR e quindi evitando il -1)
    
    sigaction(SIGINT, saINT, NULL);
    sigaction(SIGUSR1, saUSR1, NULL);

    // BLOCCO/SBLOCCO DI UNA MASCHERA DI SEGNALI NEL CASO DI INTERFERENZA CON SEZIONI CRITICHE
    sigemptyset(&setMask); // inizializzazione del set
    sigaddset(&setMask, SIGINT); // aggiunge SIGINT al set
    sigaddset(&setMask, SIGUSR1); // aggiunge SIGUSR1 al set
}

int main(int argc, char *argv[]) {
   
    struct sigaction saINT;
    struct sigaction saUSR1;
   
    attach_ipc(); 

    signal_handlers_init(&saINT, &saUSR1);

    printf("Attivazione erogatore ticket\n");

    while(1) {    
        if(receive_message(queueTickets_ID, &ticketMessage, sizeof(struct msgbuf_ticket) - sizeof(long), 1, 0) == -1) {
            exit(EXIT_FAILURE);
        }
        
        int service_time_avarage = par->SERVICE[ticketMessage.service_type];
        int service_time_random = generate_time_service(service_time_avarage);

        sigprocmask(SIG_BLOCK, &setMask, NULL); // aggiunge i segnali di set alla maschera segnali (bloccandoli)
        
        /* AGGIORNAMENTO NELLE STATISICHE */
        sem_reserve(semStats_ID, 1);
        stats->servList[ticketMessage.service_type].avg_daily_services_provided += service_time_random;
        stats->servList[ticketMessage.service_type].avg_total_services_provided += service_time_random;
        
        stats->servTot.avg_daily_services_provided +=service_time_random;
        stats->servTot.avg_total_services_provided +=service_time_random;
        sem_release(semStats_ID, 1);

        sigprocmask(SIG_UNBLOCK, &setMask, NULL); // Uso di sblocco della maschera con SIG_UNBLOCK invece che con SIG_SETMASK, perché tanto non ci sono altri segnali che erano bloccati già prima (ogni processo ha la propria maschera di segnali, per cui non c'è pericolo di concorrenza con gli altri processi) SE VUOI FAI IL CONTRARIO (A TE LA SCELTA)    

        ticketMessage.mtype = ticketMessage.usersPID;
        ticketMessage.service_time_provide = service_time_random;

        if(send_message(queueTickets_ID, &ticketMessage, sizeof(struct msgbuf_ticket) - sizeof(long), 0) == -1) {
            exit(EXIT_FAILURE);
        }
        
        printf("Ticket inviato all'utente con PID %d\n", ticketMessage.usersPID);    
    }
}