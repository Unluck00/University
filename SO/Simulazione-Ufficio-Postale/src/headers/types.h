#ifndef SIMULAZIONE_POSTE_TYPES_H
#define SIMULAZIONE_POSTE_TYPES_H

#define TOT_SERVICES 6  //meglio inserire la costante oppure inserire direttamente la dimensione dell'array?

struct parameters {
    int SIM_DURATION;
    long N_NANO_SECS;
    int NOF_USERS;
    float P_SERV_MIN;
    float P_SERV_MAX;
    int NOF_WORKER;
    int NOF_WORKER_SEATS;
    int NOF_PAUSE;
    int SERVICE[TOT_SERVICES];
    int EXPLODE_THRESHOLD;
};

typedef struct service_t {
    int total_users_served; // lo fa il processo users.c
    int daily_users_served; // lo fa il processo users.c
    int total_services_provided; // lo fa il processo users.c
    int total_services_unprovided; // lo fa il processo users.c
    int daily_services_provided; // lo fa il processo users.c
    int daily_services_unprovided; // lo fa il processo users.c
    float avg_total_wait_users; // lo fa il processo users.c
    float avg_daily_wait_users; // lo fa il processo users.c
    float avg_total_services_provided; // lo fa il processo ticket.c
    float avg_daily_services_provided; // lo fa il processo ticket.c
} service;

typedef struct statistic_t {
    service servList[TOT_SERVICES];  // statistiche per tipologia di servizio
    service servTot;  // statistiche totali di tutti i servizi
    int daily_operators_active; // lo fa il processo operator.c
    int total_operators_active; // lo fa il processo operator.c
    int daily_pauses; // lo fa il processo operator.c
    int total_pauses; // lo fa il processo operator.c
    float daily_ratio_operator_desk_available; // lo fa il processo director.c
} statistic;

typedef struct desk_t {
    enum {
        available,
        occupied
    } status; // stato dello sportello che viene occupato dall'operatore
    enum {
        service_ON,
        service_OFF
    } status_service; // stato dello sportello che sta gestendo un servizio per l'utente
    int service_type;
    int wait_users;
    pid_t operator_PID; 
    // pid_t user_PID;
} desk;

typedef struct time_t {
    int day_sim;
    int min_sim;
} time_sim;

struct msgbuf_tickets {
    long mtype;
    int service_type;
    int service_time_provide; //solo per le risposte
    pid_t user_PID;
};

struct msgbuf_waiting_users {
    long mtype;
    pid_t user_PID;
}

#endif  // SIMULAZIONE_POSTE_TYPES_H