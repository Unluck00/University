#ifndef SORT_RECORDS_H
#define SORT_RECORDS_H

#include <stdio.h>
#include <stddef.h>

/**
Sorts all records from infile and writes them to outfile
@param infile  Input binary file with unsorted records
@param outfile  Output binary file
@param field  
@param k Threshold for hybrid sort
*/

void sort_records(FILE *input_file, FILE *output_file, size_t field, size_t k);

#endif // SORT_RECORDS_H