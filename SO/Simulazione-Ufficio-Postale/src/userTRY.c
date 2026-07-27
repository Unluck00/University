#include "include/common.h"
#include "include/user.h"

/* Parametri di simulazione */
struct parametri {
    double P_SERV_MIN;  // Probabilità minima di servizio
    double P_SERV_MAX;  // Probabilità massima di servizio
    double P_SERV;      // Probabilità specifica di servizio dell'utente
    int SO_RETRY;      // Numero massimo di tentativi
    int SO_SERVICES;   // Numero di servizi disponibili
};

/* Stato dell'utente */
typedef enum {
    in_attesa,
    in_ufficio,
    servito,
    abbandonato
} stato_utente;

/* Strutture memoria condivisa */
struct parametri *par;
struct utente {
    pid_t pid;
    stato_utente stato;
    int servizi_tentati;
} *utentiPID;

/* Variabili globali */
pid_t mioPID;
int semUtentiPID_ID;

/* Prototipi di funzione */
void aggancia_oggetti_ipc(char **argv);
void inizializza_gestori_segnali(struct sigaction *saINT);
void aggiorna_stato(stato_utente statoImpostare);
int ottieni_indice_pid_utente(int PID_daRicercare);

/* Gestore segnale di interruzione */
void gestore_interruzione_utente(int segnale)
{
    #ifdef DEBUG
    (void)!write(2, "::UTENTE:: Segnale SIGINT ricevuto\n", 35);
    #endif

    aggiorna_stato(abbandonato);
    exit(0);
}

/* Aggiornamento sicuro dello stato dell'utente */
void aggiorna_stato(stato_utente statoImpostare)
{
    int i = ottieni_indice_pid_utente(mioPID);
    if (i == -1)
    {
        TRACCIA(("[UTENTE %d] Impossibile trovare me stesso in utentiPID[]\n", mioPID));
        return;
    }

    // Simula prenotazione semaforo
    utentiPID[i].stato = statoImpostare;
    if (statoImpostare == abbandonato)
    {
        TRACCIA(("[UTENTE] Utente ha abbandonato il servizio\n"));
    }
}

/* Restituisce l'indice dell'utente con PID specifico */
int ottieni_indice_pid_utente(int PID_daRicercare)
{
    unsigned int i;
    for (i = 0; i < par->SO_RETRY; i++)
    {
        if (utentiPID[i].pid == PID_daRicercare)
            return i;
    }
    return ERRORE;
}

/* Aggancia oggetti IPC */
void aggancia_oggetti_ipc(char **argv)
{
    par = shmat(atoi(argv[1]), NULL, 0);
    if (par == (void *) -1)
    {
        perror("Errore nell'agganciare i parametri");
        exit(1);
    }

    utentiPID = shmat(atoi(argv[2]), NULL, 0);
    if (utentiPID == (void *) -1)
    {
        perror("Errore nell'agganciare utentiPID");
        exit(1);
    }

    semUtentiPID_ID = atoi(argv[3]);
}

/* Inizializza gestori segnali */
void inizializza_gestori_segnali(struct sigaction *saINT)
{
    saINT->sa_handler = gestore_interruzione_utente;
    sigaction(SIGINT, saINT, NULL);
}

int main(int argc, char *argv[])
{
    struct sigaction saINT;
    struct timespec tempoSonnoRand = {0};
    struct timespec tempoSonnoRimanente = {0};

    AZZERA(&saINT, sizeof(saINT));
    mioPID = getpid();

    if (argc < 4)
    {
        printf("[UTENTE %d] Argomenti insufficienti\n", mioPID);
        return ERRORE;
    }

    srand(mioPID);

    aggancia_oggetti_ipc(argv);
    inizializza_gestori_segnali(&saINT);

    // Decide casualmente se recarsi all'ufficio postale
    float probabilita_servizio = par->P_SERV_MIN + 
        ((float)rand() / RAND_MAX) * (par->P_SERV_MAX - par->P_SERV_MIN);

    while (1)
    {
        // Sonno casuale per simulare il ciclo giornaliero
        tempoSonnoRand.tv_sec = CASUALE(1, 24 * 3600);  // Dorme fino a 24 ore
        nanosleep(&tempoSonnoRand, &tempoSonnoRimanente);

        if ((float)rand() / RAND_MAX <= probabilita_servizio)
        {
            aggiorna_stato(in_ufficio);

            // Sceglie un servizio
            int servizio_scelto = CASUALE(0, par->SO_SERVICES - 1);

            // Prova a ottenere un biglietto ed essere servito
            // Simula la logica di servizio dell'ufficio qui
            int servizio_ottenuto = 0;  // Segnaposto per la logica di servizio dell'ufficio
            int contatore_tentativi = 0;

            while (!servizio_ottenuto && contatore_tentativi < par->SO_RETRY)
            {
                // Simula il controllo della disponibilità del servizio e l'ottenimento del biglietto
                // Questo interagirebbe con memoria condivisa o code di messaggi in un'implementazione reale
                servizio_ottenuto = (rand() % 2 == 0);
                contatore_tentativi++;
            }

            if (servizio_ottenuto)
            {
                aggiorna_stato(servito);
                TRACCIA(("[UTENTE %d] Servizio %d ottenuto\n", mioPID, servizio_scelto));
            }
            else
            {
                aggiorna_stato(abbandonato);
                TRACCIA(("[UTENTE %d] Servizio abbandonato dopo %d tentativi\n", mioPID, contatore_tentativi));
            }
        }
    }

    return 0;
}






















#include "headers/common.h"
#include "include/user.h"

static Utente utente;
static int msg_id = -1;
static int sem_id = -1;

void init_utente(void) {
    srand(time(NULL) ^ getpid());
    
    utente.pid = getpid();
    utente.prob_servizio = P_SERV_MIN + 
        ((double)rand() / RAND_MAX) * (P_SERV_MAX - P_SERV_MIN);
    utente.servizi_richiesti = 0;
    utente.servizi_ottenuti = 0;

    // Accede alla coda messaggi
    msg_id = msgget(MSG_KEY, 0666);
    if (msg_id == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // Accede ai semafori
    sem_id = semget(SEM_KEY, NUM_SEMAFORI, 0666);
    if (sem_id == -1) {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }
}

int decidi_richiesta_servizio(void) {
    double random = (double)rand() / RAND_MAX;
    return random < utente.prob_servizio;
}

TipoServizio scegli_servizio(void) {
    return rand() % NUM_SERVIZI;
}

int richiedi_ticket(TipoServizio servizio) {
    struct sembuf op = {SEM_TICKET_DISP, -1, 0};
    
    // Attende che l'erogatore sia disponibile
    if (semop(sem_id, &op, 1) == -1) {
        perror("semop down failed");
        return -1;
    }

    Ticket ticket_request;
    ticket_request.mtype = 1;
    ticket_request.servizio = servizio;
    ticket_request.pid_utente = utente.pid;
    ticket_request.timestamp = time(NULL);

    // Invia richiesta ticket
    if (msgsnd(msg_id, &ticket_request, sizeof(Ticket) - sizeof(long), 0) == -1) {
        perror("msgsnd failed");
        return -1;
    }

    utente.servizi_richiesti++;

    // Rilascia l'erogatore
    op.sem_op = 1;
    if (semop(sem_id, &op, 1) == -1) {
        perror("semop up failed");
        return -1;
    }

    return 0;
}

int attendi_turno(Ticket* ticket) {
    // Implementazione dell'attesa del proprio turno
    // Questa è una versione semplificata che attende il tempo di erogazione
    int tempo_attesa = calcola_tempo_erogazione(ticket->servizio);
    sleep(tempo_attesa);
    
    utente.servizi_ottenuti++;
    return 0;
}

void termina_utente(void) {
    // Cleanup delle risorse se necessario
}