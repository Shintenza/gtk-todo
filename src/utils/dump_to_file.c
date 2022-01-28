#include <stdlib.h>
#include <stdio.h>
#include "../include/structs.h"
#include "../include/global.h"
#include "ctype.h"

void file_extension_handler (char *filename) {
    int dot_index = -1;
    char *p = filename;  
    char *c;
    c = strchr(filename, '.');
    if (c) {
        dot_index = (int)(c-filename);
        p[dot_index] = 0;
    } 
    strcat(filename, ".md");
}

void dump_to_file (struct CliTask *tasks, int mode, int arr_len, char *path) {
    char filename[FILENAME_MAX];
    char *p = path;
    char *p2 = path;
    int index;
    FILE *file; 
    char user_input[4];
    char buffer[MAX_DESC_LENGTH + MAX_DESC_LENGTH];
    int i;

    index = strrchr(path, '/') ? (int) (strrchr(path, '/') - path) : -1;

    if (index<0) {
        fprintf(stderr, "Coś poszło nie tak!\n");
        exit(-1);
    }
    p+=index+1;
    if (strlen(p)<1) {
        fprintf(stderr, "Nazwa pliku jest za krótka!\n");
        exit(-1);
    }
    strcpy(filename, p);
    
    file_extension_handler(filename);
    p2[index] = 0;
    strcat(path, "/");
    strcat(path, filename);
    if (fopen(path, "r")) {
        printf("Plik o podanej nazwie już istnieje. Czy chcesz kontynuować? Wpisz tak/nie!\n");
        fgets(user_input, 4, stdin);
        if (strcmp(user_input, "tak")) {
            exit(1);
        }
    }
    file = fopen(path, "w+");
    if (!file) {
        fprintf(stderr, "Nie mam uprawnień żeby to zrobić!\n");
        exit(1);
    }
    for (i = 0; i<arr_len; i++) {
        if (tasks[i].finished == mode) {
            sprintf(buffer, "## %s\n", tasks[i].name);
            fputs(buffer, file);
            sprintf(buffer, "# Zaplanowano na: %s\n", tasks[i].string_time);
            fputs(buffer, file);
            sprintf(buffer, "%s\n\n", tasks[i].description);
            fputs(buffer, file);
        }
    }
}
