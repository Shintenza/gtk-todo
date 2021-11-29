#include "include/structs.h"
#include "include/messages.h"
#define MAX_ENTRIES 10
#define MAX_NAME_LENGTH 125

struct CliTask {
    int call_id;
    char rowid[100];
    char name[MAX_NAME_LENGTH];
    char description[MAX_NAME_LENGTH];
    char string_time[MAX_NAME_LENGTH];
    long long unix_time;
    char response_msg[1000];
};
struct CliLoadTaskParams {
    struct CliTask *tasks_arr;
    int *entries_counter;
};

static int cli_load_tasks_callback(void *args,int argc, char **argv, char **col_name ) {
    struct CliLoadTaskParams *params = args;
    int *entries_counter = params->entries_counter;
    char final_message[strlen(argv[1])+strlen(argv[2])+strlen(argv[3])+30];
    char *error;
    long long time = strtoll(argv[4], &error, 10);

    sprintf(final_message, "Zadanie nr: %d\nNazwa zadania: %s\nOpis zadania: %s\n%s\n", *entries_counter, argv[1], argv[2], argv[3]);
    strcpy(params->tasks_arr[*entries_counter].name, argv[1]);
    strcpy(params->tasks_arr[*entries_counter].description, argv[2]);
    strcpy(params->tasks_arr[*entries_counter].string_time, argv[3]);
    strcpy(params->tasks_arr[*entries_counter].response_msg, final_message);
    strcpy(params->tasks_arr[*entries_counter].rowid, argv[0]);
    params->tasks_arr[*entries_counter].unix_time=time;
    params->tasks_arr[*entries_counter].call_id=*entries_counter;

    *entries_counter = *entries_counter + 1;
    return 0;
}
static void cli_load_tasks(sqlite3 *db, char *err_msg, int rc, char *status, struct CliTask *tasks_arr, int *entries_counter){
    char *sql; 
    struct CliLoadTaskParams params;
    if(strcmp(status, "active")==0) {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 0 ORDER BY date ASC;"; 
    } else {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 1 ORDER BY date ASC;"; 
    }
    params.tasks_arr = tasks_arr;
    params.entries_counter = entries_counter;
    rc = sqlite3_exec(db, sql, cli_load_tasks_callback, &params, &err_msg);
    return;
}

int cli_handling (int argc, char **argv, struct DbElements *db_elements) {
    sqlite3 *db = db_elements->db;
    char *err_msg = db_elements->err_msg;
    int rc = db_elements->rc;
    struct CliTask tasks_arr[MAX_ENTRIES] = {{0}};

    int entries_counter = 0;
    int i = 0;
    i = 0;
    if (strcmp(argv[1], "--help")==0 || strcmp(argv[1], "-h")==0) {
        printf("%s", help_message());
    } else if(strcmp(argv[1], "-L")==0 || strcmp(argv[1], "-l")==0) {
        printf("[Lista aktywnych zadań:]\n\n");
        cli_load_tasks(db, err_msg, rc, "active", tasks_arr, &entries_counter);
        while (i<MAX_ENTRIES && tasks_arr[i].unix_time !=0) {
            printf("Zadanie nr %d: %s\n", tasks_arr[i].call_id, tasks_arr[i].name);
            i++;
        }
    } else {
        printf("Niepoprawna składnia polecenia! Wpisz --help, aby zapoznać się z dozwolonymi poleceniami!\n");
    }
    return 1;
}
