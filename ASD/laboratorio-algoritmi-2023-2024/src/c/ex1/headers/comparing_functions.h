#ifndef LABORATORIO_DI_ALGORITMI_2023_2024_COMPARING_FUNCTIONS_H
#define LABORATORIO_DI_ALGORITMI_2023_2024_COMPARING_FUNCTIONS_H

typedef struct {
    int    id;
    char  *field1;
    int    field2;
    double field3;
} Record;

int compareInt   (const void *a, const void *b);
int compareDouble(const void *a, const void *b);
int compareString(const void *a, const void *b);

#endif
