#ifndef RECORD_H
#define RECORD_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint64_t id;         /* 8 bytes */
    float    field1;     /* 4 bytes */
    int64_t  field2;     /* 8 bytes */
    char     field3[16]; /* 16 bytes */
} Record;

_Static_assert(sizeof(Record) == 36, "Record must be exactly 36 bytes");

#endif
