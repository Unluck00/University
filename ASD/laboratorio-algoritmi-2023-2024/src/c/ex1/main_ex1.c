//Macro che abilitano estensioni GNU e POSIX, necessarie per funzioni come getline e strsep
#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "headers/comparing_functions.h"
#include "headers/sorting.h"

// ---- Windows polyfills for getline/strsep Blocco compilato solo su Windows per fornire funzioni assenti (getline, strsep). ----
#ifdef _WIN32
    // Definisce ssize_t se non presente (tipico di Linux, ma mancante su Windows).
  #include <stddef.h>
  #ifndef _SSIZE_T_DEFINED
    typedef long ssize_t;
    #define _SSIZE_T_DEFINED
  #endif
  static char* my_strsep(char **stringp, const char *delim) { // Implementazione manuale di strsep, che divide una stringa in token separati da delimitatori (qui la virgola del CSV).
      if (stringp == NULL || *stringp == NULL || delim == NULL) return NULL;
      char *s = *stringp;
      char *p = s;
      // cerca il primo carattere in delim
      while (*p) {
          const char *d = delim;
          while (*d) { if (*p == *d++) { *p = '\0'; *stringp = p + 1; return s; } }
          p++;
      }
      // fine stringa: restituisci l'ultimo token e imposta *stringp = NULL
      *stringp = NULL;
      return s;
  }
  static ssize_t my_getline(char **lineptr, size_t *n, FILE *stream) { // Implementazione manuale di getline: legge una riga dal file in modo dinamico, riallocando il buffer se necessario.
      if (!lineptr || !n || !stream) return -1;
      if (*lineptr == NULL || *n == 0) {
          *n = 128;
          *lineptr = (char*)malloc(*n);
          if (!*lineptr) return -1;
      }
      size_t pos = 0;
      for (;;) {
          int c = fgetc(stream);
          if (c == EOF) {
              if (pos == 0) return -1;     // nessun carattere letto
              else break;                  // restituisci la riga parziale
          }
          // assicurati spazio
          if (pos + 1 >= *n) {
              size_t newn = (*n) * 2;
              char *newp = (char*)realloc(*lineptr, newn);
              if (!newp) return -1;
              *lineptr = newp; *n = newn;
          }
          (*lineptr)[pos++] = (char)c;
          if (c == '\n') break;
      }
      (*lineptr)[pos] = '\0';
      return (ssize_t)pos;
  }
  #define strsep  my_strsep
  #define getline my_getline
#endif
// ---- end polyfills ----


// ---- helpers ----
static inline void chomp(char *s) { // Rimuove \n o \r finali da una stringa → utile per processare correttamente linee CSV.
    if (!s) return;
    size_t n = strlen(s);
    if (n && (s[n-1] == '\n' || s[n-1] == '\r')) s[--n] = '\0';
    if (n && (s[n-1] == '\r')) s[--n] = '\0';
}

static Record* parse_line(const char *line) { // Converte una riga del CSV in una struct Record
    if (!line || !*line) return NULL;
    char *buf = strdup(line); // Duplica la stringa (strdup) per poterla modificare
    if (!buf) return NULL; 
    chomp(buf);

    char *p = buf;
    char *tok;
    Record *rec = calloc(1, sizeof(Record)); // creazione di una memoria dinamina del Record e inizializzata
    if (!rec) { free(buf); return NULL; }

    // Divide i campi con strsep
    // Converte i campi nei rispettivi tipi (int, string, double)
    // Alloca dinamicamente un Record e ritorna il puntatore

    tok = strsep(&p, ",");         // id
    if (!tok) { free(buf); free(rec); return NULL; }
    rec->id = atoi(tok);

    tok = strsep(&p, ",");         // field1 (string)
    if (!tok) { free(buf); free(rec); return NULL; }
    rec->field1 = malloc(strlen(tok) + 1);
    if (!rec->field1) { free(buf); free(rec); return NULL; }
    strcpy(rec->field1, tok);

    tok = strsep(&p, ",");         // field2 (int)
    if (!tok) { free(buf); free(rec->field1); free(rec); return NULL; }
    rec->field2 = atoi(tok);

    tok = p;                       // field3 (double) - fino a fine riga
    if (!tok) { free(buf); free(rec->field1); free(rec); return NULL; }
    chomp(tok);
    rec->field3 = atof(tok);

    free(buf);
    return rec;
}

static size_t read_records(FILE *infile, Record ***out_records) { // Legge tutti i record da infile e li salva in un array di puntatori a Record
    if (!infile || !out_records) return 0;

    // Inizia con capacità 1024, e raddoppia con realloc quando serve
    // Per ogni riga letta con getline, usa parse_line per creare il Record

    size_t cap = 1024, n = 0;
    Record **records = malloc(cap * sizeof(Record*));
    if (!records) return 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t r;
    while ((r = getline(&line, &len, infile)) != -1) {
        if (r == 0) continue;
        Record *rec = parse_line(line);
        if (!rec) continue;
        if (n == cap) {
            cap <<= 1; // shift of 1 bit to the left for increase the cap of malloc
            Record **tmp = realloc(records, cap * sizeof(Record*));
            if (!tmp) break;
            records = tmp;
        }
        records[n++] = rec;
    }
    free(line);
    //Ritorna il numero di record letti e imposta *out_records
    *out_records = records;
    return n;
}

static void write_records(FILE *outfile, Record **records, size_t n) { // Scrive l’array di record su file CSV (outfile), rispettando il formato richiesto (id,field1,field2,field3)
    if (!outfile || !records) return;
    for (size_t i = 0; i < n; ++i) {
        if (!records[i]) continue;
        // CSV standard (virgole) come da specifica
        fprintf(outfile, "%d,%s,%d,%lf\n",
                records[i]->id,
                records[i]->field1 ? records[i]->field1 : "",
                records[i]->field2,
                records[i]->field3);
    }
    fflush(outfile); // vengono scritti i file sul disco senza problemi di interruzione immprovisa
}

// funzione richiesta dall'esercizio
void sort_records(FILE *infile, FILE *outfile, size_t field, size_t algo) { // Legge tutti i record da infile (read_records)
    if (!infile || !outfile) {
        fprintf(stderr, "Provide a valid input and output file\n");
        return;
    }

    Record **records = NULL;
    size_t n = read_records(infile, &records);
    if (n == 0) {
        fprintf(stderr, "No records to sort or read error.\n");
        free(records);
        return;
    }

    // Sceglie il comparator giusto in base al campo (field1=string, field2=int, field3=double)
    int (*cmp)(const void*, const void*) = NULL;
    switch (field) {
        case 1: cmp = compareString; break;
        case 2: cmp = compareInt;    break;
        case 3: cmp = compareDouble; break;
        default:
            fprintf(stderr, "Invalid field %zu (use 1=string,2=int,3=double)\n", field);
            write_records(outfile, records, n);
            for (size_t i = 0; i < n; ++i) { if (records[i]) { free(records[i]->field1); free(records[i]); } }
            free(records);
            return;
    }

    // 1=MergeSort, 2=QuickSort (come da README)
    // Applica l’algoritmo richiesto (merge_sort o quick_sort).
    if (algo == 1)      merge_sort((void**)records, n, cmp);
    else if (algo == 2) quick_sort((void**)records, n, cmp);
    else {
        fprintf(stderr, "Invalid algo %zu (use 1=merge, 2=quick)\n", algo);
        merge_sort((void**)records, n, cmp);
    }

    // Scrive i record ordinati su outfile
    write_records(outfile, records, n);

    // deallocazioni (non obbligatorie per il CLI, ma corrette)
    for (size_t i = 0; i < n; ++i) { if (records[i]) { free(records[i]->field1); free(records[i]); } }
    free(records);
}

int main(int argc, char **argv){
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <input.csv> <output.csv> <field:1|2|3> <algo:1=merge|2=quick>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_file_path  = argv[1];
    const char *output_file_path = argv[2];
    const size_t field = (size_t)atoi(argv[3]);
    const size_t algo  = (size_t)atoi(argv[4]);

    FILE *inputfile  = fopen(input_file_path, "r");
    if (!inputfile) { perror("error opening input file"); return EXIT_FAILURE; }
    FILE *outputfile = fopen(output_file_path, "w");
    if (!outputfile) { perror("error opening output file"); fclose(inputfile); return EXIT_FAILURE; }

    sort_records(inputfile, outputfile, field, algo);

    fclose(inputfile);
    fclose(outputfile);
    return EXIT_SUCCESS;
}
