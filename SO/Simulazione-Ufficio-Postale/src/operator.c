#include "headers/common.h"
#include "include/operator.h"

struct parameters *par;
statistic *stats;
desk *desks;
time_sim *time_data;

// struct msgbuf_waiting_user waitMessage;

sigset_t setMask, setWait; // setMask -> permette di contenere più segnali che verranno gestiti (bloccati/sbloccati); setWait -> permette di contenere il segnale che verrà assegnato per essere ricevuto e liberare il sigwait
int sig; // utile per i comandi tipo sigwait() che richiedono il secondo attributo

int shmPar_ID;
int shmStats_ID;
int shmDesks_ID;
int shmTime_ID;
int semStats_ID;
int semDesks_ID;
int semTime_ID;
// int queueWaiting_ID;

unsigned int day_current;
// unsigned int seats_operator; PER DARE AL PROCESSO OPERATORE IL NUMERO DELLA POSTAZIONE ASSEGNATO (MA VERREBBE USATO PER LIBERARE QUELLA POSTAZIONE SE L'OPERATORE FA LA PAUSA)

bool check_seats = false; // flag di controllo per controllare che questo processo operatore ha uno sportello selezionato o meno
// bool waiting_seats = false; // flag di controllo per segnalare che l'operatore è in attesa

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

    // queueWaiting_ID = msgget(ftok("director.c", QUEUE_WAITING), 0);
}

void seats_search(int sevice_type_operator) { // per controllare se gli sportelli sono liberi

    int counterSeats;

    for(counterSeats = 0; counterSeats < par->NOF_WORKER_SEATS; counterSeats++) {
        sem_reserve(semDesks_ID, counterSeats);

        if(desks[counterSeats].status == available && desks[counterSeats].service_type == sevice_type_operator) {
            
            sigprocmask(SIG_BLOCK, &setMask, NULL); // blocca i segnali (SIGUSR1 e SIGINT) evitando cosi di bloccarsi se sta facendo un aggiornamento dei dati in memoria condivisa
            
            desks[counterSeats].operator_PID = getpid();
            desks[counterSeats].status = occupied;
            sem_release(semDesks_ID, counterSeats);
            // seats_operator = counterSeats;
            printf("Sportello %d occupato dall'operatore %d per il servizio di %d\n", counterSeats/*+1 se si vuole che parta da 1*/, getpid(), sevice_type_operator);

            sem_reserve(semStats_ID, 1);
            stats->daily_operators_active++;
            stats->total_operators_active++;
            sem_release(semStats_ID, 1);

            sigprocmask(SIG_UNBLOCK, &setMask, NULL); // sblocca i segnali (SIGUSR1 e SIGINT) evitando cosi di bloccarsi se sta facendo un aggiornamento dei dati in memoria condivisa

            check_seats = true;
            return; // evita il pause() se ha occupato uno sportello
        }
        sem_release(semDesks_ID, counterSeats);
    }
    
    printf("Nessun sportello libero, l'operatore %d rimane in attesa\n", getpid());
    
    //waiting_seats = true;
    //sigwait(&setWait, )

    sigprocmask(SIG_UNBLOCK, &setWait, NULL); // si toglie il blocco del segnale SIGUSR2 visto che ora l'operatore è in attesa di uno sportello
    pause();  // si può usare sigwait(), ma ho deciso per il momento di no (controllare cosa sia meglio)
    // rimane in attesa di un segnale che lo liberi (SIGUSR2 -> si libera uno sportello (altrimenti usare dei semafori); SIGINT -> finisce la simulazione; SIGUSR1 -> finisce la giornata)
    sigprocmask(SIG_BLOCK, &setWait, NULL); // si rimette il blocco temporaneamente in caso in cui c'è uno sportello libero
}

void take_break() {
    
    int counterSeats;
    int counterOperator;

    for(counterSeats = 0; counterSeats < par->NOF_WORKER_SEATS; counterSeats++) {
        sem_reserve(semDesks_ID, counterSeats); // se l'utente si trova allo sportello per la richiesta servizio allora l'operatore rimane in attesa prima di andare in pausa

        if(desks[counterSeats].operator_PID == getpid()) {
            desks[counterSeats].operator_PID = 0; // valore per indicare nessun processo (USARE QUESTO METODO O TOGLIERLO E IGNORARE L'ULTIMO OPERATORE ASSEGNATO ALLO SPORTELLO)
            desks[counterSeats].status = available;
            sem_release(semDesks_ID, counterSeats);
            printf("Sportello %d liberato dall'operatore %d che va in pausa\n", counterSeats/*+1 se si vuole che parta da 1*/, getpid());
            

            // mandare il segnale SIGUSR2
            killpg(0, SIGUSR2); // il segnale viene inviato a tutti i processi operatori che fanno parte dello stesso gruppo (vedi setpgid in director.c), inoltre sono solo i processi che sono in attesa di uno sportello libero
            
            return;
        }

        sem_release(semDesks_ID, counterSeats);
    }
}

void time_handle(int signum) {
    
    if(day_current == time_data->day_sim) { // giornata finita
        day_current--; // in modo da non iniziare subito una giornata (NEL CASO RICONTROLLARE)
        // check_seats = true; // in modo che quando la gioranta è finita e c'era un processo operatore in attesa di uno sportello, non rimanga dentro il loop
    }
    else { // inizio di una nuova giornata
        sem_reserve(semTime_ID, 1); // mettere il sem reserve e release prima o dopo l'operazione if????
        day_current = time_data->day_sim;
        sem_release(semTime_ID, 1);
        check_seats = false; 
    }
}

void interrupt_handle(int signum) { // gestore di segnale per chiusura del processo dovuta alla fine della simulazione

    printf("Chiusura del processo operatore %d", getpid());

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
    
    saUSR1.sa_handler = time_handle; // segnale di inizio/fine giornata, che permette di interrompere temporaneamente l'operatore affinchè si prepari per la nuova giornata
    sigemptyset(&saUSR1.sa_mask);
    saUSR1.sa_flags = 0;
    
    /* NON NE HA BISOGNO 
    saUSR2.sa_handler = wakes_handle; // segnale che sveglia un operatore che è in attesa di uno sportello libero
    sigemptyset(&saUSR2.sa_mask);
    saUSR2.sa_flags = 0;
    sigaction(SIGUSR2, saUSR2, NULL);
    */

    sigaction(SIGINT, saINT, NULL);
    sigaction(SIGUSR1, saUSR1, NULL);

    // BLOCCO/SBLOCCO DI UNA MASCHERA DI SEGNALI NEL CASO DI INTERFERENZA CON SEZIONI CRITICHE
    sigemptyset(&setMask); // inizializzazione del setMask
    sigemptyset(&setWait); // inizializzazione del setWait
    sigaddset(&setMask, SIGINT); // aggiunge SIGINT al setMask
    sigaddset(&setMask, SIGUSR1); // aggiunge SIGUSR1 al setMask
    sigaddset(&setWait, SIGUSR2); // aggiunge SIGUSR2 al setWait
}

int main(int argc, char *argv[]) {
    
    struct sigaction saINT;
    struct sigaction saUSR1;
    // struct sigaction saUSR2;

    day_current = 0;

    attach_ipc();

    signal_handlers_init(&saINT, &saUSR1);

    int pauses_operator = par->NOF_USERS;
    int service_type_operator = atoi(argv[1]);

    printf("Operatore %d attivato", getpid());

    sigprocmask(SIG_BLOCK, &setWait, NULL); // inizializza che tutti gli operatori ignorino il segnale SIGUSR2 (quindi che non sono in attesa di uno sportello libero)

    // l'operatore continua a lavorare finchè non arriva il segnale di fine giornata SIGUSR1
    while(1) {
        if(day_current == time_data->day_sim) { // nel caso in cui l'inizio della nuova giornata non è ancora iniziato allora sospende l'operatore fino all'arrivo del segnale SIGUSR1
            // inizia il giorno e di conseguenza il lavoro
            if(!check_seats) {
                seats_search(service_type_operator);
            }
            else {
                // if(receive_message(queueWaiting_ID, &waitMessage, sizeof(struct msgbuf_waiting_user) - sizeof(long), 0))

                unsigned int random_break = rand() % 100; // prende un valore casuale tra 0 e 99; viene dato all'operatore per controllare se vuole fare una pausa (in questo modo il valore cambia ad ogni iterazione, cosi può decidere di andare in pausa durante la giornata)
                if(random_break < 15) { // 15% di probabilità che vada in pausa
                    if(pauses_operator != 0) {
                        
                        sigprocmask(SIG_BLOCK, &setMask, NULL); // blocca i segnali (SIGUSR1 e SIGINT) evitando cosi di bloccarsi se sta facendo un aggiornamento dei dati in memoria condivisa
                        
                        take_break();
                        pauses_operator--;
    
                        sem_reserve(semStats_ID, 1);
                        stats->daily_pauses++;
                        stats->total_pauses++;
                        sem_release(semStats_ID, 1);
    
                        sigprocmask(SIG_UNBLOCK, &setMask, NULL); // sblocca i segnali (SIGUSR1 e SIGINT)
    
                        // mandare il processo operatore in pausa e aspettare l'inizio di una nuova giornata (SIGUSR1) o la fine della simulazione (SIGINT)
                        sigwait(&setMask, &sig);
                    }
                    else {
                        printf("L'operatore %d vorrebbe fare una pausa, ma ha raggiunto il limite massimo per la simulazione\n", getpid());
                    }
                }
    
                sleep(1); //aspetta prima 1 secondo prima di riprovare a prendere una pausa
    
            }
        }
        else {
            sigwait(&setMask, &sig);
        }
    }
}