#include "include/structs.h"
#include "include/messages.h"
#define MAX_ENTRIES 2

struct CliLoadTaskParams {
    char **response;
    int *entries_counter;
};

static int cli_load_tasks_callback(void *args,int argc, char **argv, char **col_name ) {
    struct CliLoadTaskParams *params = args;
    int *entries_counter = params->entries_counter;
    char **response = params->response;
    char *task_name = argv[1];
    char *task_desc = argv[2];
    char *string_date = argv[3];
    char final_message[strlen(task_name)+strlen(task_desc)+strlen(string_date)+200];
    if (*entries_counter == MAX_ENTRIES-1) {
        sprintf(final_message, "Zadanie nr: %d\nNazwa zadania: %s\nOpis zadania: %s\n%s\n", *entries_counter, task_name, task_desc, string_date);
    } else {
        sprintf(final_message, "Zadanie nr: %d\nNazwa zadania: %s\nOpis zadania: %s\n%s\n\n", *entries_counter, task_name, task_desc, string_date);
    }
    strcpy(response[*entries_counter], final_message);
    if (*entries_counter > MAX_ENTRIES) {
        return 0;
    }
    *entries_counter = *entries_counter + 1;
    return 0;
}
static char** cli_load_tasks(sqlite3 *db, char *err_msg, int rc, char *status, char **response, int *entries_counter){
    char *sql; 
    struct CliLoadTaskParams params;
    if(strcmp(status, "active")==0) {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 0 ORDER BY date ASC;"; 
    } else {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 1 ORDER BY date ASC;"; 
    }
    params.response = response;
    params.entries_counter = entries_counter;
    rc = sqlite3_exec(db, sql, cli_load_tasks_callback, &params, &err_msg);
    return response;
}

int cli_handling (int argc, char **argv, struct DbElements *db_elements) {
    sqlite3 *db = db_elements->db;
    char *err_msg = db_elements->err_msg;
    int rc = db_elements->rc;
    char *response[MAX_ENTRIES] = {NULL};
    int entries_counter = 0;
    int i = 0;
    while (i<MAX_ENTRIES){
        response[i] = malloc(10000);
        i++;
    }
    i = 0;
    if (strcmp(argv[1], "--help")==0 || strcmp(argv[1], "-h")==0) {
        printf("%s", help_message());
    } else if(strcmp(argv[1], "-L")==0 || strcmp(argv[1], "-l")==0) {
        printf("[Lista aktywnych zadań:]\n\n");
        cli_load_tasks(db, err_msg, rc, "active", response, &entries_counter);
        while (i<MAX_ENTRIES && response[i]!=NULL) {
            printf("%s", response[i]);
            free(response[i]);
            i++;
        }
    } else {
        printf("Niepoprawna składnia polecenia! Wpisz --help, aby zapoznać się z dozwolonymi poleceniami!\n");
    }
    return 1;
}
