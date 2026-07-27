#ifndef RECORD_IO_H
#define RECORD_IO_H

#include "stdio.h"
#include "stddef.h"
#include "record.h"

/**
Reads one record from file into *record
*/
int read_record(FILE* file, Record* record);

/**
Write one record to file 
*/
int write_record(FILE* file, const Record* record);

/**
Loads all record from file into a new allocated array
*/
int load_records(FILE* file, Record** records, size_t* count);

/**
Saves all records in array to file
*/
int save_records(FILE* file, const Record* records, size_t count);

/**
Prints a record
*/
void print_record(const Record* record);

#endif // RECORD_IO_H