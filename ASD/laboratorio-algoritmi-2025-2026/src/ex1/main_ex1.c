#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "headers/sort_records.h"

static void print_usage(const char *prog_name) {
    fprintf(stderr,
            "Usage: %s <input_file> <output_file> <field> <k>\n"
            "  <field>: 1=id, 2=field1, 3=field2, 4=field3\n"
            "  <k>: threshold for hybrid sort (0 = QuickSort only)\n",
            prog_name);
}

static int parse_size_t(const char *s, size_t *out) {
    if (!s || !*s) return 0;
    char *end = NULL;
    unsigned long long v = strtoull(s, &end, 10);
    if (!end || *end != '\0') return 0;
    *out = (size_t)v;
    return 1;
}

static int parse_field(const char *s, size_t *out_field) {
    if (!s || !*s) return 0;
    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (!end || *end != '\0') return 0;
    if (v < 1 || v > 4) return 0;
    *out_field = (size_t)v;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        print_usage(argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    if (strcmp(input_filename, output_filename) == 0) {
        fprintf(stderr, "Error: output_file must be different from input_file\n");
        return 1;
    }

    size_t field = 0;
    if (!parse_field(argv[3], &field)) {
        fprintf(stderr, "Invalid field: %s\n", argv[3]);
        print_usage(argv[0]);
        return 1;
    }

    size_t k = 0;
    if (!parse_size_t(argv[4], &k)) {
        fprintf(stderr, "Invalid k: %s\n", argv[4]);
        print_usage(argv[0]);
        return 1;
    }

    FILE *infile = fopen(input_filename, "rb");
    if (!infile) {
        perror("Error opening input file");
        return 1;
    }

    FILE *outfile = fopen(output_filename, "wb");
    if (!outfile) {
        perror("Error opening output file");
        fclose(infile);
        return 1;
    }

    clock_t start_time = clock();
    sort_records(infile, outfile, field, k);
    clock_t end_time = clock();

    fclose(infile);
    fclose(outfile);

    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Sorting completed in %.3f seconds\n", time_taken);

    return 0;
}
