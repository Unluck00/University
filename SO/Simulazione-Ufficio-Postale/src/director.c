#include "headers/common.h"
#include "include/director.h"
#include "include/parser.h"

struct parameters *par;
statistic *stats;
desk *desks;
time_sim *time_data;

struct timespec ts;

int semStats_ID;
int semDesks_ID;
int semTime_ID;
int shmPar_ID;
int shmDesks_ID;
int shmStats_ID;
int shmTime_ID;
int queueTicket_ID;
int queueWaiting_ID;

pid_t *operatorPIDs;
pid_t *usersPIDs;

pid_t group_op_leader;  // pid del primo processo operatore creato che andrà a divenatare il leader del gruppo di tutti i processi operatori (in modo che poi successivamente si possa inviare il segnale di cambio sportello (dovuto alla pausa di uno dei operatori) solo hai processi operatore)

void shared_memory_init() {

    shmPar_ID = shmget(ftok("director.c", SHM_PARAMETERS), sizeof(struct parameters), IPC_CREAT | IPC_EXCL | 0600);
    /* DA AGGIUNGERE IN QUALCHE MODO 
    if(errno == EEXIST) {
        fprintf(stderr, "%s, %d. *** Error one or more shared memory objects couldn't be created, ipcrm the leftovers *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
        exit(EXIT_FAILURE);
    } 
    */
    par = (struct parameters *)shmat(shmPar_ID, NULL, 0);
    parse_parameters(par);


    shmDesks_ID = shmget(ftok("director.c", SHM_DESKS_ARRAY), sizeof(desk) * (par->NOF_WORKER_SEATS), IPC_CREAT | IPC_EXCL | 0600);
    shmStats_ID = shmget(ftok("director.c", SHM_STATISTICS), sizeof(statistic), IPC_CREAT | IPC_EXCL | 0600);
    shmTime_ID = shmget(ftok("director.c", SHM_TIME_SIMULATED), sizeof(time_sim), IPC_CREAT | IPC_EXCL | 0600);

    if(errno == EEXIST) {
        fprintf(stderr, "%s, %d. PID=%5d *** Error one or more shared memory objects couldn't be created, ipcrm the leftovers *** #%03d: %s\n",__FILE__, __LINE__, getpid(), errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    desks = (desk *)shmat(shmDesks_ID, NULL, 0);
    stats = (statistic *)shmat(shmStats_ID, NULL, 0);
    time_data = (time_sim *)shmat(shmTime_ID, NULL, 0);

    if(!desks || !stats || !time_data) {
        fprintf(stderr, "%s, %d. PID=%5d *** Error one or more shared memory objects couldn't be attachment *** #%03d: %s\n",__FILE__, __LINE__, getpid(), errno, strerror(errno));
        exit(EXIT_FAILURE); 
    }

    printf("Printing of all shared memory IDs\n");
    printf("shmPar_ID is %d\n", shmPar_ID);
    printf("shmDesks_ID is %d\n", shmDesks_ID);
    printf("shmStats_ID is %d\n", shmStats_ID);
    printf("shmTime_ID is %d\n", shmTime_ID);
}

void semaphores_init() {

    semStats_ID = semget(ftok("director.c", SEM_STATISTICS), 1, IPC_CREAT | IPC_EXCL | 0600);
    semDesks_ID = semget(ftok("director.c", SEM_DESKS), par->NOF_WORKER_SEATS, IPC_CREAT | IPC_EXCL | 0600);
    semTime_ID = semget(ftok("director.c", SEM_TIME_SIMULATED), 1, IPC_CREAT | IPC_EXCL | 0600);

    if(semStats_ID == -1 || semDesks_ID == -1 || semTime_ID == -1) {
        switch(errno) {
            case EEXIST:
                fprintf(stderr, "%s, %d. *** Error one or more semaphores couldn't be created, ipcrm the leftovers *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
                exit(EXIT_FAILURE);

            case ENOSPC:
                fprintf(stderr, "%s, %d. *** Error too many semaphores already in the system *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
                exit(EXIT_FAILURE);
            
            case ENOMEM:
                fprintf(stderr, "%s, %d. *** Error the system does not have enough memory for the new data structure *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
                exit(EXIT_FAILURE);

            default:
                fprintf(stderr, "%s, %d. *** Error for semget *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
                exit(EXIT_FAILURE);
        }
    }

    // inizializzazione dei semafori e dei loro valori
    sem_set(semTime_ID, 1);
    sem_set(semStats_ID, 1);
    sem_set_all(semDesks_ID, par->NOF_WORKER_SEATS); // inizializza a 1 (sportelli disponibili) tutto il set di semafori assegnati ai vari sportelli

    // stampa degli ID dei semafori
    printf("Printing of all semaphores IDs\n");
    printf("semStats_ID is %d\n", semStats_ID);
    printf("semDesks_ID is %d\n", semDesks_ID);
    printf("semTime_ID is %d\n", semTime_ID);

}

void queue_init() {

    queueTicket_ID = msgget(ftok("director.c", QUEUE_TICKETS), IPC_CREAT | IPC_EXCL | 0600);
    queueWaiting_ID = msgget(ftok("director.c", QUEUE_WAITING), IPC_CREAT | IPC_EXCL | 0600);

    if(queueTicket_ID == -1 || queueWaiting_ID == -1) {
        switch(errno) {
            case EIDRM:
                fprintf(stderr, "%s, %d. *** Error one or more queue was removed *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
                exit(EXIT_FAILURE);

            case EINVAL:
                fprintf(stderr, "%s, %d. *** Error one or more queue invalid value for cmd or msqid*** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
                exit(EXIT_FAILURE);

            default:
                fprintf(stderr, "%s, %d. *** Error for queue *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
                exit(EXIT_FAILURE);
        }
    }

    printf("Printing of all queue IDs\n");
    printf("queueTicket_ID is %d\n", queueTicket_ID);
    printf("queueWaiting_ID is %d\n", queueWaiting_ID);
}

void cleanup_IPC() {

    printf("IPC resource cleanup\n");

    shmctl(shmPar_ID, IPC_RMID, NULL);
    shmctl(shmDesks_ID, IPC_RMID, NULL);
    shmctl(shmStats_ID, IPC_RMID, NULL);
    shmctl(shmTime_ID, IPC_RMID, NULL);
    semctl(semStats_ID, 1, IPC_RMID);
    semctl(semDesks_ID, 1, IPC_RMID);
    semctl(semTime_ID, 1, IPC_RMID);
    msgctl(queueTicket_ID, IPC_RMID, NULL);
    msgctl(queueWaiting_ID, IPC_RMID, NULL);
}

void spawn_operator(pid_t *operators, int oCounter) {

    operators[oCounter] = fork();

    switch(operators[oCounter]) {
        case -1:
            fprintf(stderr, "%s, %d. *** Error forking for operator *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
            exit(EXIT_FAILURE);

        case 0:
            if(oCounter == 0) {
                setpgid(0, 0); // il primo operatore diventa il leader del nuovo gruppo che avrà come Process Group ID il PID del primo processo operatore creato
            }
            else {
                setpgid(0, group_op_leader); // gli altri processi operatore si uniscono al gruppo del primo operatore (grazie al suo PID salvato in group_op_leader)
            }

            int service_type = oCounter % TOT_SERVICES; //assegna ad un operatore uno dei 6 servizi
            char service_str[10];
            snprintf(service_str, sizeof(service_str), "%d", service_type);
            execl(OPERATOR_NAME, OPERATOR_NAME, service_str, NULL);
            fprintf(stderr, "%s, %d. *** Error executing operator *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
            exit(EXIT_FAILURE);  

        default :
            if(oCounter == 0) {
                group_op_leader = operators[oCounter]; // salva il leader che sarà il primo processo operatore creato
            }
            // potrebbe bastare anche cosi, ma per maggior sicurezza, il setpgid lo mettiamo anche al padre 
            setpgid(operators[oCounter], group_op_leader); // il padre imposta il gruppo per i processi figli
            
            return;
    }
}

void spawn_user(pid_t *users, int uCounter) {

    users[uCounter] = fork();

    switch(users[uCounter]) {
        case -1:
            fprintf(stderr, "%s, %d. *** Error forking for user *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
            exit(EXIT_FAILURE);

        case 0:
            execl(USER_NAME, USER_NAME, NULL);
            fprintf(stderr, "%s, %d. *** Error executing user *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
            exit(EXIT_FAILURE);  

        default :
            return;
    }
}

void ratio_operator_desk() {

    unsigned int countSeats;
    int daily_operators_available = par->NOF_WORKER - stats->daily_operators_active;
    int daily_available_desks = 0;
    
    for(countSeats = 0; countSeats < par->NOF_WORKER_SEATS; countSeats++) {
        if(desks[countSeats].status == available) {
            daily_available_desks = daily_available_desks + 1; 
        }
    }

    if(daily_available_desks > 0) {
        stats->daily_ratio_operator_desk_available = (float) daily_operators_available / daily_available_desks;
    }
    else {
        stats->daily_ratio_operator_desk_available = 0.0;
    }
}

void print_daily_statistics(int day) {
    
    int i;

    printf("Day: %d\n", day);

    for(i = 0; i < TOT_SERVICES; i++) {

        char *service;

        switch (i) {
            case 0:
                service = "Sending and receiving parcels";
                break;
            case 1:
                service = "Sending and receiving letters and registered";
                break;
            case 2:
                service = "Bancoposta withdrawals and deposits";
                break;
            case 3:
                service = "Payment of postal bills";
                break;
            case 4:
                service = "Purchase of financial products";
                break;
            case 5:
                service = "Purchase of watches and bracelets";
                break;
        }

        printf("Daily users served of the service %s: %d\n", service, stats->servList[i].daily_users_served);
        printf("Daily services provided of the service %s: %d\n", service, stats->servList[i].daily_services_provided);
        printf("Daily services unprovided of the service %s: %d\n", service, stats->servList[i].daily_services_unprovided);
        printf("Average daily wait time of the users of the service %s: %.2f minutes\n", service, stats->servList[i].avg_daily_wait_users);
        printf("Average daily time of the service provided %s: %.2f minutes\n", service, stats->servList[i].avg_daily_services_provided);
    }

    printf("Daily users served of all services: %d\n", stats->servTot.daily_users_served);
    printf("Daily provided of all services: %d\n", stats->servTot.daily_services_provided);
    printf("Daily unprovided of all services: %d\n", stats->servTot.daily_services_unprovided);
    printf("Average daily wait time of the users of all services: %.2f minutes\n", stats->servTot.avg_daily_wait_users);
    printf("Average daily time of all services provided: %.2f minutes\n", stats->servTot.avg_daily_services_provided);

    printf("Daily operators activated: %d\n", stats->daily_operators_active);
    printf("Daily pauses taken: %d\n", stats->daily_pauses);

    ratio_operator_desk();
    printf("Daily ratio between operators available and desks existing: %.2f\n", stats->daily_ratio_operator_desk_available);
}

void print_total_statistics(int type_termination) {
    switch (type_termination) {
        case 0:
            printf("Simulation terminated timeout\n");
            break;
    
        case 1:
            printf("Simulation terminated explode\n");
            break; 
    }

    int i;

    for(i = 0; i < TOT_SERVICES; i++) {

        char *service;

        switch (i) {
            case 0:
                service = "Sending and receiving parcels";
                break;
            case 1:
                service = "Sending and receiving letters and registered";
                break;
            case 2:
                service = "Bancoposta withdrawals and deposits";
                break;
            case 3:
                service = "Payment of postal bills";
                break;
            case 4:
                service = "Purchase of financial products";
                break;
            case 5:
                service = "Purchase of watches and bracelets";
                break;
        }

        printf("Total users served in simulation of the service %s, %d\n", service, stats->servList[i].total_users_served);
        printf("Total services provided in simulation of the service %s: %d\n", service, stats->servList[i].total_services_provided);
        printf("Total services unprovided in simulation of the service %s: %d\n", service, stats->servList[i].total_services_unprovided);
        printf("Average total wait time of the users in simulation of the service %s: %.2f minutes\n", service, stats->servList[i].avg_total_wait_users);
        printf("Average total time in simulation of the service provided %s: %.2f minutes\n", service, stats->servList[i].avg_total_services_provided);
    }
    
    printf("Total users served in simulation: %d\n", stats->servTot.total_users_served);
    printf("Total provided in simulation: %d\n", stats->servTot.total_services_provided);
    printf("Total unprovided in simulation: %d\n", stats->servTot.total_services_unprovided);
    printf("Average total wait time of the users in simulation: %.2f minutes\n", stats->servTot.avg_total_wait_users);
    printf("Average total time in simulation: %.2f minutes\n", stats->servTot.avg_total_services_provided);

    printf("Total operators activated in simulation: %d\n", stats->total_operators_active);
    printf("Total pauses taken in simulation: %d\n", stats->total_pauses);
}

void reset_daily_statistics() {
    
    sem_reserve(semStats_ID, 1);
    for(int i=0; i<TOT_SERVICES; i++) {
        stats->servList[i].daily_services_provided = 0;
        stats->servList[i].daily_services_unprovided = 0;
        stats->servList[i].daily_users_served = 0;
        stats->servList[i].avg_daily_services_provided = 0;
        stats->servList[i].avg_daily_wait_users = 0;
    }

    stats->servTot.daily_services_provided = 0
    stats->servTot.daily_services_unprovided = 0;
    stats->servTot.daily_users_served = 0;
    stats->servTot.avg_daily_services_provided = 0;
    stats->servTot.avg_daily_wait_users = 0;
    stats->daily_operators_active = 0;
    stats->daily_pauses = 0;
    stats->daily_ratio_operator_desk_available = 0;
    sem_release(semStats_ID, 1);
}

void signal_handle(int signum) {  //gestore di segnale per chiusura della simulazione
    
    killpg(0, SIGINT); //facendo cosi se i processi sono ancora attivi, gli manda un segnale SIGINT per indicare anche agli altri processi che è stato inserito l'interrupt oppure semplicemente per chiudere gli altri processi prima della fine della simulazione
    
    cleanup_IPC();
    
    free(operatorPIDs);
    free(usersPIDs);
    
    exit(0);
}

int main(int argc, char *argv[]) { 

    unsigned int dCounter, oCounter, uCounter;
    unsigned int day, minute;
    int service_desk;

    struct sigaction sa;
    sa.sa_handler = signal_handle;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL); 

    shared_memory_init();
    semaphores_init();
    queue_init();

    // inizializzazione di tutti i dati presenti nella memoria condivisa statistiche in runtime
    memset(*stats, 0, sizeof(statistic));

    //creazione del processo erogatore ticket (ticket)
    switch(fork()) {
        case -1:
            fprintf(stderr, "%s, %d. *** Error forking for ticket dispenser *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
            exit(EXIT_FAILURE);

        case 0:
            execl(TICKET_NAME, TICKET_NAME, NULL);
            fprintf(stderr, "%s, %d. *** Error executing ticket *** #%03d: %s\n",__FILE__, __LINE__, errno, strerror(errno));
            exit(EXIT_FAILURE);       

        default:
            break;
    }

    operatorPIDs = malloc(sizeof(pid_t) * par->NOF_WORKER);
    for(oCounter = 0; oCounter < par->NOF_WORKER; oCounter++) {
        spawn_operator(operatorPIDs, oCounter);
    }

    usersPIDs = malloc(sizeof(pid_t) * par->NOF_USERS);
    for(uCounter = 0; uCounter < par->NOF_USERS; uCounter++) {
        spawn_user(usersPIDs, uCounter);
    }

    // inizializzazione della memoria condivisa per il tempo simulato
    sem_reserve(semTime_ID, 1);
    time_data->day_sim = 1;
    time_data->min_sim = 0;
    sem_release(semTime_ID, 1);

    // simulazione dei giorni
    ts.tv_nsec = 0;
    ts.tv_sec = par->N_NANO_SECS;

    printf("Start simuation\n");

    for(day = 1; day <= par->SIM_DURATION; day++) {
        printf("Day %d\n", day);

        // inizializzazione degli sportelli ogni volta che inizia una giornata   MEGLIO FARLO FARE AD OGNI PROCESSO OPERATORE CHE HA OCCUPATO LO SPORTELLO IN operator.c
        for(dCounter = 0; dCounter < par->NOF_WORKER_SEATS; dCounter++) {
            sem_reserve(semDesks_ID, dCounter);
            desks[dCounter].status = available;
            service_desk = rand() % TOT_SERVICES;
            desks[dCounter].status_service = service_OFF;
            desks[dCounter].wait_users = 0;
            desks[dCounter].service_type = service_desk;
            sem_release(semDesks_ID, dCounter);
        }

        sleep(2); // lascia che gli altri processi si inizializzino prima di iniziare la simulazione
        killpg(0, SIGUSR1);  // manda un segnale a tutti i processi per indicare che inizia una nuova giornata  
        // sleep(2); // lascia che gli altri processi si inizializzino per la giornata prima di iniziarla
               

        for(minute = 0; minute < MINUTES_WORK_DAY; minute++) {
            nanosleep(&ts, NULL);
            printf("It's been %d minutes since the day began", minute);

            sem_reserve(semTime_ID, 1);
            time_data->min_sim++;
            sem_release(semTime_ID, 1);
        }

        killpg(0, SIGUSR1);  // manda un segnale a tutti i processi per indicare che è finita la giornata
        sleep(2); // aspetta che tutti i processi abbiano finito la giornata e aggioranto eventualmente la memoria condivisa

        print_daily_statistics(day);

        //controlla se devono essere reinizializzate i valori delle statistiche per ogni giorno se da qui o dai altri processi
    
        if(stats->servTot.daily_services_unprovided > par->EXPLODE_THRESHOLD) {
            print_total_statistics(1); // terminazione dovuto al explode
            // capire le risorse ipc se eliminarle o meno e vedere se liberare delle eventuali malloc
            exit(0);
        }

        reset_daily_statistics(); // in questo faccio un reset solo una volta PRIMA AVEVO MESSO IL RESET NEI VARI PROCESSI MA RISULTAVA RINDONDANTE PERCHE' LO FACEVA PER OGNI PROCESSO

        sem_reserve(semTime_ID, 1);
        time_data->day_sim++;
        time_data->min_sim = 0;
        sem_release(semTime_ID, 1);                                                           
    }

    print_total_statistics(0);
    // controllare i segnali

    // signal_handle(); // per concludere la simulazione??    
    return 0;
}
