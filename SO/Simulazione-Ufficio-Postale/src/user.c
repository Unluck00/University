#include "headers/common.h"
#include "include/user.h"

#include "utils/msg.h"
#include "utils/sem.h"

// IPC Shared Memory
struct parameters *par;
statistic *stats;
desk *desks;
time_sim *time_data;

// IPC Queue
struct msgbuf_tickets ticketMessage;
struct msgbuf_waiting_users waitMessage;

struct timespec ts;

sigset_t setMask; // permette di contenere più segnali che verranno gestiti (bloccati/sbloccati)
int sig;
// variabile globale usati per comunicare tra il codice standard e il signal handler che è arrivato il segnale
// sig_atomic_t accesso sicuro da signal handler (non interrompibile a metà da un segnale)
// volatile impedisce ottimizzazioni che nasconderebbero cambiamenti async
volatile sig_atomic_t wait_ticket = 0;
volatile sig_atomic_t wait_user = 0;

int shmPar_ID;
int shmStats_ID;
int shmDesks_ID;
int shmTime_ID;
int semStats_ID;
int semDesks_ID;
int semTime_ID;
int queueTickets_ID;
int queueWaiting_ID;

unsigned int day_current;
// unsigned int start_poste;

void attach_ipc() {
    shmPar_ID = shmget(ftok("director.c", SHM_PARAMETERS), 0, 0);  // si ottiene l'identificatore del IPC
    shmStats_ID = shmget(ftok("director.c", SHM_STATISTICS), 0, 0);
    shmDesks_ID = shmget(ftok("director.c", SHM_DESKS_ARRAY), 0, 0);
    shmTime_ID = shmget(ftok("director.c", SHM_TIME_SIMULATED), 0, 0);

    par = (struct parameters *)shmat(shmPar_ID, NULL, 0);
    stats = (statistic *)shmat(shmStats_ID, NULL, 0);
    desks = (desk *)shmat(shmDesks_ID, NULL, 0);
    time_data = (time_sim *)shmat(shmTime_ID, NULL, 0);

    semStats_ID = semget(ftok("director.c", SEM_STATISTICS), 0, 0); 
    semDesks_ID = semget(ftok("director.c", SEM_DESKS), 0, 0);
    semTime_ID = semget(ftok("director.c", SEM_TIME_SIMULATED), 0, 0);

    queueTickets_ID = msgget(ftok("director.c", QUEUE_TICKETS), 0);
    queueWaiting_ID = msgget(ftok("director.c", QUEUE_WAITING), 0);
}

void probability_visiting(float p_serv) {

    /* https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c */

    float p_min = par->P_SERV_MIN;
    float p_max = par->P_SERV_MAX;
    p_serv = p_min + (((float)rand() / (float)RAND_MAX) * (p_max - p_min)); 
    // ((float)rand() / (float)RAND_MAX) permette di generare un numero compreso tra 0.0 e 1.0; 
    // successivamente p_min + (((float)rand() / (float)RAND_MAX) * (p_max - p_min)) permette di generare un numero compreso nel range tra min e max
}

/* PER STABILIRE PIU' SERVIZI
void services_user(int service_user) {
    service_user = rand() % TOT_SERVICES;
}
*/

void wait_user_time(int time_user) {
    while(time_data->min_sim < time_user) {
        nanosleep(&ts, NULL);
    }
}

bool check_seats_available(int service_user) {

    int counterSeats;

    for(counterSeats = 0; counterSeats < par->NOF_WORKER_SEATS; counterSeats++) {

        sem_reserve(semDesks_ID, counterSeats);
        if(desks[counterSeats].status == occupied && desks[counterSeats].service_type == service_user) {
            return true;
        }
        else {
            return false;
        }
        sem_release(semDesks_ID, counterSeats);
    }
}

int choose_best_desk(int service_user) {

    int counterSeats;
    sem_reserve(semDesks_ID, 0);
    int minQueue = desks[0].wait_users;
    sem_release(semDesks_ID, 0);
    int bestDesk;
    
    sigprocmask(SIG_BLOCK, &setMask, NULL);

    for(counterSeats = 0; counterSeats < par->NOF_WORKER_SEATS; counterSeats++) {
        sem_reserve(semDesks_ID, counterSeats);
        if(desks[counterSeats].service_type == service_user && desks[counterSeats].wait_users < minQueue) {
            minQueue = desks[counterSeats].wait_users;
            bestDesk = counterSeats;
        }
        sem_release(semDesks_ID, counterSeats);
    }

    sigprocmask(SIG_UNBLOCK, &setMask, NULL);

    return bestDesk;
}

void free_desk(int current_desk) {
    sem_reserve(semDesks_ID, current_desk);
    desks[current_desk].status_service = service_OFF;
    sem_release(semDesks_ID, current_desk);

    if(receive_message(queueWaiting_ID, &waitMessage, sizeof(struct msgbuf_waiting_users) - struct(long), current_desk) == -1) {
        // nessun utente in attesa nella coda
        return;
    }

    waitMessage.mtype = waitMessage.user_PID;
    if(send_message(queueWaiting_ID, &waitMessage, sizeof(struct msgbuf_waiting_users) - struct(long), 0) == -1) {
        exit(EXIT_FAILURE);
    }
}

void time_handle(int signum) {

    if(day_current == time_data->day_sim) { // giornata finita

        sem_reserve(semStats_ID, 1);
        // svuotamento degli utenti in attesa del ticket e svuotamento delle code degli utenti in attesa nei vari sportelli (se erano in attesa)
        if(wait_ticket || wait_user) { // 2 tipi di possibilità con utente in attesa della risposta nel queue
            // utente ancora in attesa del ticket, quindi non è stato servito
            // utente ancora in attesa del proprio turno di erogare il servizio, quindi non è stato servito  
            stats->servList[ticketMessage.service_type].daily_services_unprovided++;
            stats->servList[ticketMessage.service_type].total_services_unprovided++;
            stats->servTot.daily_services_unprovided++;
            stats->servTot.total_services_unprovided++;
        }
        sem_release(semStats_ID, 1);

        day_current--; // in modo da non iniziare subito una giornata (NEL CASO RICONTROLLARE)
    }
    else { // inizio di una nuova giornata
        sem_reserve(semTime_ID, 1); // mettere il sem reserve e release prima o dopo l'operazione if????
        day_current = time_data->day_sim;
        sem_release(semTime_ID, 1);
    }
}

void interrupt_handle(int signum) {

    printf("Chiusura del processo utente %d", getpid());

    shmdt(par);
    shmdt(stats);
    shmdt(desks);
    shmdt(time_data);

    exit(0);
}

void signal_handlers_init(struct sigaction *saINT, struct sigaction *saUSR1) {

    saINT.sa_handler = interrupt_handle; // segnale di fine simulazione, che interrompe e chiude il processo corrente
    sigemptyset(&saINT.sa_mask);
    saINT.sa_flags = 0;
    
    saUSR1.sa_handler = time_handle; // segnale di inizio/fine giornata, che permette di interrompere temporaneamente l'utente affinchè si prepari per la nuova giornata
    sigemptyset(&saUSR1.sa_mask);
    saUSR1.sa_flags = 0;

    sigaction(SIGINT, saINT, NULL);
    sigaction(SIGUSR1, saUSR1, NULL);

    // BLOCCO/SBLOCCO DI UNA MASCHERA DI SEGNALI NEL CASO DI INTERFERENZA CON SEZIONI CRITICHE
    sigemptyset(&setMask); // inizializzazione del setMask
    //sigemptyset(&setWait); // inizializzazione del setWait
    sigaddset(&setMask, SIGINT); // aggiunge SIGINT al setMask
    sigaddset(&setMask, SIGUSR1); // aggiunge SIGUSR1 al setMask
}

int main(int argc, char *argv[]) {

    struct sigaction saINT;
    struct sigaction saUSR1;

    float p_serv;
    float decision_user;
    int service_user;
    int time_user;

    int best_desk;

    day_current = 0;
    //start_poste = 0;

    ts.tv_nsec = 0;
    ts.tv_sec = par->N_NANO_SECS;

    attach_ipc();

    signal_handlers_init(&saINT, &saUSR1);

    probability_visiting(p_serv);

    printf("Utente %d attivato", getpid());

    while(1) {
        if(day_current == time_data->day_sim) {
            
            decision_user = (float)rand() / (float)RAND_MAX; // numero casuale scelto nell'intervallo [0,1]
            if(decision_user < p_serv) {
                // services_user(service_user); // stabilisce il/i servizio/servizi da usufruire nella giornata
                service_user = rand() % TOT_SERVICES;
                time_user = rand() % MINUTES_WORK_DAY; // stabilisce l'orario in cui l'utente va all'ufficio postale
            
                wait_user_time(time_user); // il processo utente rimane in attesa finchè l'orario scelto non coincide con quello generale (nel processo direttore)
            
                if(check_seats_available(service_user)) {
                    
                    ticketMessage.mtype = 1;
                    ticketMessage.service_type = service_user;
                    ticketMessage.user_PID = getpid();

                    if(send_message(queueTickets_ID, &ticketMessage, sizeof(struct msgbuf_ticket) - sizeof(long), 0) == -1) {
                        exit(EXIT_FAILURE);
                    }

                    sigprocmask(SIG_BLOCK, &setMask, NULL);
                    wait_ticket = 1; // il messaggio non è stato ricevuto
                    sigprocmask(SIG_UNBLOCK, &setMask, NULL);
                    if(receive_message(queueTickets_ID, &ticketMessage, sizeof(struct msgbuf_ticket) - sizeof(long), getpid(), 0) == -1) {
                        exit(EXIT_FAILURE);
                    }
                    wait_ticket = 0; // il messaggio è ricevuto o fallito

                    sigprocmask(SIG_BLOCK, &setMask, NULL);

                    sem_reserve(semStats_ID, 1);
                    stats->servList[ticketMessage.service_type].avg_daily_wait_users += ticketMessage.service_time_provide;
                    stats->servList[ticketMessage.service_type].avg_total_wait_users += ticketMessage.service_time_provide;
                    stats->servTot.avg_daily_wait_users += ticketMessage.service_time_provide;
                    stats->servTot.avg_total_wait_users += ticketMessage.service_time_provide;
                    sem_release(semStats_ID, 1);

                    sigprocmask(SIG_UNBLOCK, &setMask, NULL);

                    // Ricevuto il ticket per il servizio richiesto ora è il momento di andare allo sportello (oppure alla coda se c'è fila)
                    // Per ottimizzare il processo, l'utente controlla prima quale sportello gestisce il suo servizio, se c'è ne più di uno allora controlla qual'è lo sportello con meno utenti in coda
                    best_desk = choose_best_desk(service_user);
                    
                    sem_reserve(semDesks_ID, best_desk);
                    if(desks[best_desk].status_service == service_ON) { // vuol dire che è occupato lo sportello
                        sem_release(semDesks_ID, best_desk);

                        waitMessage.mtype = best_desk;
                        waitMessage.user_PID = getpid();

                        if(send_message(queueWaiting_ID, &waitMessage, sizeof(struct msgbuf_waiting_users) - sizeof(long), 0) == -1) {
                            exit(EXIT_FAILURE);
                        }

                        sigprocmask(SIG_BLOCK, &setMask, NULL);
                        wait_user = 1; // il messaggio non è stato ricevuto
                        sigprocmask(SIG_UNBLOCK, &setMask, NULL);
                        if(receive_message(queueWaiting_ID, &waitMessage, sizeof(struct msgbuf_waiting_users) - sizeof(long), getpid(), 0) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        wait_user = 0; // il messaggio è stato ricevuto o fallito

                        sem_reserve(semDesks_ID, best_desk);
                    }

                    sigprocmask(SIG_BLOCK, &setMask, NULL);

                    desks[best_desk].status_service = service_ON;

                    // quando l'utente si trova allo sportello lo prenota con il semaforo affinchè non venga usato da altri e in modo che l'operatore (prima di andare in pausa) finisca prima di servirlo
                    for(int minute_service_provide = 0; minute_service_provide < ticketMessage.service_time_provide; minute_service_provide++) {
                        nanosleep(&ts, NULL);
                    }
                    sem_release(semDesks_ID, best_desk);
                    
                    // utente servito 
                    sem_reserve(semStats_ID, 1);
                    stats->servList[ticketMessage.service_type].daily_users_served++;
                    stats->servList[ticketMessage.service_type].total_users_served++;
                    stats->servList[ticketMessage.service_type].daily_services_provided++;
                    stats->servList[ticketMessage.service_type].total_services_provided++;

                    stats->servTot.daily_users_served++;
                    stats->servTot.total_users_served++;
                    stats->servTot.daily_services_provided++;
                    stats->servTot.total_services_provided++;
                    sem_release(semStats_ID, 1);

                    free_desk(best_desk);

                    sigprocmask(SIG_UNBLOCK, &setMask, NULL);

                    // successivamente l'utente va a casa
                    sigwait(&setMask, &sig);
                }
            }
        }
        else {
            sigwait(&setMask, &sig);
        }
    }
}