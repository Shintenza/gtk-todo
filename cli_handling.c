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
    int long_listing;
};

static int cli_load_tasks_callback(void *args,int argc, char **argv, char **col_name ) {
    struct CliLoadTaskParams *params = args;
    int *entries_counter = params->entries_counter;
    int long_listing = params->long_listing;
    char final_message[strlen(argv[1])+strlen(argv[2])+strlen(argv[3])+100];
    char *error;
    long long time = strtoll(argv[4], &error, 10);
    if(*entries_counter>MAX_ENTRIES) {
        return 0;
    }
    if (long_listing == 1) {
        sprintf(final_message, "Zadanie nr: %d\nNazwa zadania: %s\nOpis zadania: %s\nZaplanowano na: %s\nId z bazy danych: %s\nZaplanowano na (UNIX time): %lld\n\n", *entries_counter, argv[1], argv[2], argv[3], argv[0], time);
    } else {
        sprintf(final_message, "Zadanie nr: %d\nNazwa zadania: %s\nOpis zadania: %s\nZaplanowano na: %s\n\n", *entries_counter, argv[1], argv[2], argv[3]);
    }
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
static void cli_load_tasks(sqlite3 *db, char *err_msg, int rc, char *status, struct CliTask *tasks_arr, int *entries_counter, int long_listing){
    char *sql; 
    struct CliLoadTaskParams params;
    if(strcmp(status, "active")==0) {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 0 ORDER BY date ASC;"; 
    } else {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 1 ORDER BY date ASC;"; 
    }
    params.tasks_arr = tasks_arr;
    params.entries_counter = entries_counter;
    params.long_listing = long_listing;
    rc = sqlite3_exec(db, sql, cli_load_tasks_callback, &params, &err_msg);
    return;
}
int check_if_flag_exists(int argc, char **argv, char *flag) {
    int i;
    for (i=2; i<argc; i++) {
        if(strcmp(argv[i], flag)==0) return 1;
    }
    return 0;
}
int cli_handling (int argc, char **argv, struct DbElements *db_elements) {
    sqlite3 *db = db_elements->db;
    char *err_msg = db_elements->err_msg;
    int rc = db_elements->rc;
    struct CliTask tasks_arr[MAX_ENTRIES] = {{0}};

    char *main_flag = argv[1];
    int entries_counter = 0;
    int i = 0;
    i = 0;
    if (strcmp(main_flag, "--help") ==0 || strcmp(main_flag, "-h")==0) {
        printf("%s", help_message());
    } else if(strcmp(main_flag, "-L")==0 || strcmp(main_flag, "-l")==0) {
        if (check_if_flag_exists(argc, argv, "-o")==1) {
            printf("[Lista archiwalnych zadań:]\n\n");
            if (check_if_flag_exists(argc, argv, "-v")==1) {
                cli_load_tasks(db, err_msg, rc, "archived", tasks_arr, &entries_counter, 1);
            } else {
                cli_load_tasks(db, err_msg, rc, "archived", tasks_arr, &entries_counter, 0);
            }
        } else {
            printf("[Lista aktywnych zadań:]\n\n");
            if (check_if_flag_exists(argc, argv, "-v")==1) {
                cli_load_tasks(db, err_msg, rc, "active", tasks_arr, &entries_counter, 1);
            } else {
                cli_load_tasks(db, err_msg, rc, "active", tasks_arr, &entries_counter, 0);
            }
        }
        while (i<MAX_ENTRIES && tasks_arr[i].unix_time !=0) {
            printf("%s", tasks_arr[i].response_msg);
            i++;
        }
    } else {
        printf("Niepoprawna składnia polecenia! Wpisz --help, aby zapoznać się z dozwolonymi poleceniami!\n");
    }
    return 1;
}
