#include <string.h>
#include <math.h>
#include "headers/comparing_functions.h"

static int safe_strcmp(const char *s1, const char *s2) {
    if (s1 == s2) return 0;          // include (NULL,NULL)
    if (s1 == NULL) return -1;       // policy: NULL < "qualunque stringa"
    if (s2 == NULL) return 1;
    return strcmp(s1, s2);
}

int compareInt(const void *a, const void *b){
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;

    if (ra->field2 < rb->field2) return -1;
    if (ra->field2 > rb->field2) return  1;
    return 0;
}

int compareDouble(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;

    const double x = ra->field3;
    const double y = rb->field3;

    const int x_nan = isnan(x);
    const int y_nan = isnan(y);
    if (x_nan && y_nan) return 0;
    if (x_nan) return 1;   // x "dopo" i numeri reali
    if (y_nan) return -1;

    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

int compareString(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;
    return safe_strcmp(ra->field1, rb->field1);
}