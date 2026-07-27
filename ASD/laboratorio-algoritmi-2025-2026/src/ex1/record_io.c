#include "headers/record_io.h"
#include <string.h>
#include <stdlib.h>

int read_record(FILE *file, Record *record) {
    if(!file || !record) {
        return 0;
    }

    size_t read_items = fread(record, sizeof(Record), 1, file);
    if(read_items == 1) {
        return 1;
    }
    return 0;
}

int write_record(FILE *file, const Record *record) {
    if(!file || !record) {
        return 0;
    }

    size_t written_items = fwrite(record, sizeof(Record), 1, file);
    if(written_items == 1) {
        return 1;
    }
    return 0;
}

int load_records(FILE *file, Record **records, size_t *count) {
    if(!file || !records || !count) {
        return 0;
    }

    if(fseek(file, 0, SEEK_END) != 0) {
        return 0;
    }

    long file_size = ftell(file);
    if(file_size < 0) {
        return 0;
    }

    if(fseek(file, 0, SEEK_SET) != 0) {
        return 0;
    }

    size_t num_records = (size_t) (file_size / (long) sizeof(Record));
    if(num_records == 0) {
        *records = NULL;
        *count = 0;
        return 1;
    }

    Record *array = malloc(num_records * sizeof(Record));
    if(!array) {
        return 0;
    }

    size_t read_items = fread(array, sizeof(Record), num_records, file);
    if(read_items != num_records) {
        free(array);
        return 0;
    }

    *records = array;
    *count = num_records;   
    return 1;
}

int save_records(FILE *file, const Record *records, size_t count) {
    if(!file || (!records && count > 0)) {
        return 0;
    }

    if(count == 0) {
        return 1;
    }

    size_t written_items = fwrite(records, sizeof(Record), count, file);
    if(written_items != count) {
        return 0;
    }

    return 1;
}

void print_record(const Record *record) {
    if(!record) {
        return;
    }
    printf("Record{id=%llu, field1=%f, field2=%lld, field3=\"%s\"}\n",
           (unsigned long long)record->id,
           (double)record->field1,
           (long long)record->field2,
           record->field3);
}