#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "headers/edit_distance.h"

dictionary *load_dictionary(const char* file_name, int *list_words) {
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    printf("Loading dictionary from file %s\n", file_name);

    dictionary *dict = NULL;
    char buffer[MAX_CHARACTERS];
    int words_count = 0;

    for (int i=0; fgets(buffer, sizeof(buffer), fp) != NULL; i++) {
        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline, insert termination
        dict = realloc(dict, sizeof(dictionary) * (size_t)(i+1));
        if(dict == NULL) {
            fclose(fp);
            return NULL;
        }

        strncpy(dict[i].word, buffer, sizeof(dict[i].word)-1);
        dict[i].word[sizeof(dict[i].word) - 1] = '\0';
        words_count++;
    }

    fclose(fp);
    *list_words = words_count;
    return dict;
}

// Using array of stringer
char** load_array(const char* file_name, int *list_words) {
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    printf("Loading file to be corrected %s\n", file_name);

    char** words_file = NULL;
    char* single_word = NULL;
    char buffer[MAX_CHARACTERS];
    int size_array = 0;
    
    while(fscanf(fp, "%50s", buffer) == 1) {
        if(ispunct(buffer[(int)strlen(buffer)-1])) buffer[(int)strlen(buffer)-1] = '\0'; // eliminate special character
        
        words_file = realloc(words_file, sizeof(char*) * (size_t)(size_array+1));
        if(words_file == NULL) {
            fclose(fp);
            return NULL;
        }

        single_word = malloc(strlen(buffer)+1); // +1 for character termination '\0'
        words_file[size_array] = strcpy(single_word, buffer);
        size_array++;
    }

    fclose(fp);
    *list_words = size_array;
    return words_file;
}

int main(int argc, char const *argv[])
{
    if(argc < 3) {
        printf("Usage: %s <dictionary.txt> <correctme.txt>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Load dictionary
    int list_words_dict = 0;
    dictionary *dict = load_dictionary(argv[1], &list_words_dict);
    if(dict == NULL) {
        printf("Error loading dictionary\n");
        exit(EXIT_FAILURE);
    }

    // Load file input
    int list_words_file = 0;
    char** words_file = load_array(argv[2], &list_words_file);
    if(words_file == NULL) {
        printf("Error loading file input\n");
        free(dict);
        exit(EXIT_FAILURE);
    }

    int min_distance;
    int* best_index = NULL; // array dynamic for index
    int best_count; // for define size array best_index

    for(int i=0; i<list_words_file; i++) {

        min_distance = MAX_CHARACTERS;
        int word_len = (int)strlen(words_file[i]);
        best_count = 0;

        printf("\n");
        printf("The word is: %s\n", words_file[i]);

        // Minime distance words with the functions edit_distance_dyn
        for(int j=0; j<list_words_dict; j++) {
            if(abs((int)strlen(dict[j].word)-word_len) > min_distance) continue; // avoid waste of time, skip if the word is further the minimum
            int dist = edit_distance_dyn(dict[j].word, words_file[i]);
            
            if(dist < min_distance) {
                min_distance = dist;
                best_index = realloc(best_index, sizeof(int));
                best_index[0] = j;
                best_count = 1;
            }
            else if(dist == min_distance) {
                best_index = realloc(best_index, sizeof(int) * (size_t)(best_count+1));
                best_index[best_count] = j;
                best_count++;
            }
        }

        // Search the minime distance words with the functions edit_distance_dyn
        printf("Suggest words with minimum distance %d\n", min_distance);
        for(int k=0; k<best_count; k++) {
            printf(" %s\n", dict[best_index[k]].word);
        }
        printf("\n");
    }

    // free memory allocatate
    free(best_index);
    free(dict);
    for(int i=0; i<list_words_file; i++) {
        free(words_file[i]);
    }
    free(words_file);

    return EXIT_SUCCESS;
}