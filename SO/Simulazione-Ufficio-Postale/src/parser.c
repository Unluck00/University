#include "headers/common.h"
#include "include/parser.h"

void assign_defaults(struct parameters *par) {
    par->SIM_DURATION = 10;
    par->N_NANO_SECS = 100000000;
    par->NOF_USERS = 50;
    par->P_SERV_MIN = 0.1;
    par->P_SERV_MAX = 0.9;
    par->NOF_WORKER = 8;
    par->NOF_WORKER_SEATS = 4;
    par->NOF_PAUSE = 3;
    par->SERVICE[0] = 10;
    par->SERVICE[1] = 8;
    par->SERVICE[2] = 6;
    par->SERVICE[3] = 8;
    par->SERVICE[4] = 20;
    par->SERVICE[5] = 20;
    par->EXPLODE_THRESHOLD = 25;
}

int parse_parameters(struct parameters *par) {

    assign_defaults(par);

    FILE *fp = fopen(CONFIG_FILE, "r");
    if (fp == NULL) {
        /*
        perror("*** Error reading conf file ***\n");
        exit(1);
        */
        fprintf(stderr, "%s: %d. *** Error reading conf file, defaulting to conf *** #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
        return 0;
    }

    char buffer[MAX_BUFFER]; // buffer in cui viene memorizzata la riga del file conf, con massima dimensione di 100

    char keys[MAX_CHARACTERS];
    char value[MAX_CHARACTERS]; 

    while (fgets(buffer, MAX_BUFFER, fp)) {
        if (sscanf(buffer, "%63s %63s", keys, value) != 2) {
            fprintf(stderr, "%s: %d. *** Error invalid line format *** #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
            continue;
        }

        if (!strcmp(keys, "SIM_DURATION")) {
            par->SIM_DURATION = atoi(value);
        }
        else if (!strcmp(keys, "N_NANO_SECS")) {
            par->N_NANO_SECS = atoi(value);
        }
        else if (!strcmp(keys, "NOF_USERS")) {
            par->NOF_USERS = atoi(value);
        }
        else if (!strcmp(keys, "P_SERV_MIN")) {
            par->P_SERV_MIN = atof(value);
        }
        else if (!strcmp(keys, "P_SERV_MAX")) {
            par->P_SERV_MAX = atof(value);
        }
        else if (!strcmp(keys, "NOF_WORKER")) {
            par->NOF_WORKER = atoi(value);
        }
        else if (!strcmp(keys, "NOF_WORKER_SEATS")) {
            par->NOF_WORKER_SEATS = atoi(value);
        }
        else if (!strcmp(keys, "NOF_PAUSE")) {
            par->NOF_PAUSE = atoi(value);
        }
        else if (!strcmp(keys, "SERVICE_1")) {
            par->SERVICE[0] = atoi(value);
        }
        else if (!strcmp(keys, "SERVICE_2")) {
            par->SERVICE[1] = atoi(value);
        }
        else if (!strcmp(keys, "SERVICE_3")) {
            par->SERVICE[2] = atoi(value);
        }        
        else if (!strcmp(keys, "SERVICE_4")) {
            par->SERVICE[3] = atoi(value);
        }        
        else if (!strcmp(keys, "SERVICE_5")) {
            par->SERVICE[4] = atoi(value);
        }        
        else if (!strcmp(keys, "SERVICE_6")) {
            par->SERVICE[5] = atoi(value);
        }
        else if (!strcmp(keys, "EXPLODE_THRESHOLD")) {
            par->EXPLODE_THRESHOLD = atoi(value);
        }
    }

    if (errno == ERANGE) {
        fprintf(stderr, "%s: %d. *** Error one or multiple values out of bound, resetting defaults *** #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
        assign_defaults(par);
    }
    if (par->P_SERV_MIN > par->P_SERV_MAX) { // Normalizza i valori di P_SERV_MIN e P_SERV_MAX se l'ordine è errato
        printf("P_SERV_MIN greater than P_SERV_MAX, will be normalized\n");
        par->P_SERV_MIN = par->P_SERV_MAX;
    }

    fclose(fp);
    return 0;
}
