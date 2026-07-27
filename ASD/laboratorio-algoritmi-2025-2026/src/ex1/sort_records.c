#include "headers/sort_records.h"
#include "headers/record_io.h"
#include "headers/hybridsort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int record_cmp_by_id(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;
    
    if(ra->id < rb->id) return -1;
    if(ra->id > rb->id) return 1;
    return 0;
}

static int record_cmp_by_field1(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;
    
    if(ra->field1 < rb->field1) return -1;
    if(ra->field1 > rb->field1) return 1;
    return 0;
}

static int record_cmp_by_field2(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;
    
    if(ra->field2 < rb->field2) return -1;
    if(ra->field2 > rb->field2) return 1;       
    return 0;
}

static int record_cmp_by_field3(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;
    
    int res = strncmp(ra->field3, rb->field3, sizeof(ra->field3));
    if (res < 0) return -1;
    if (res > 0) return 1;
    return 0;
}

void sort_records(FILE *infile, FILE *outfile, size_t field, size_t k) {
    if(!infile || !outfile) {
        fprintf(stderr, "File pointer invalido.\n");
        return;
    }

    Record *records = NULL;
    size_t count = 0;
    if(!load_records(infile, &records, &count)) {
        fprintf(stderr, "Errore durante il caricamento dei record dal file di input.\n");
        return;
    }

    if(count == 0) {
        fprintf(stderr, "Nessun record da ordinare.\n");
        free(records);
        return;
    }

    int (*cmp)(const void *, const void *) = NULL;

    switch(field) {
        case 1:
            cmp = record_cmp_by_id;
            break;
        case 2:
            cmp = record_cmp_by_field1;
            break;
        case 3:
            cmp = record_cmp_by_field2;
            break;
        case 4:
            cmp = record_cmp_by_field3;
            break;
        default:
            fprintf(stderr, "Campo di ordinamento non valido.\n");
            free(records);
            return;
    }

    hybridsort(records, count, sizeof(Record), k, cmp);
    
    if(!save_records(outfile, records, count)) {
        fprintf(stderr, "Errore durante il salvataggio dei record nel file di output.\n");
    }

    free(records);
}


