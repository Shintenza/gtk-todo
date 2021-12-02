#include "include/structs.h"
#include "include/messages.h"
#define MAX_ENTRIES 1000
#define MAX_NAME_LENGTH 125
#define MAX_DESC_LENGTH 2000

struct CliTask {
    int call_id;
    char rowid[100];
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESC_LENGTH];
    char string_time[MAX_NAME_LENGTH];
    long long unix_time;
    int finished;
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
    char *error;
    long long time = strtoll(argv[4], &error, 10);
    unsigned int finished = strtoll(argv[6], &error, 10);

    if(*entries_counter>MAX_ENTRIES) {
        return 0;
    }

    strcpy(params->tasks_arr[*entries_counter].name, argv[1]);
    strcpy(params->tasks_arr[*entries_counter].description, argv[2]);
    strcpy(params->tasks_arr[*entries_counter].string_time, argv[3]);
    strcpy(params->tasks_arr[*entries_counter].rowid, argv[0]);
    params->tasks_arr[*entries_counter].unix_time=time;
    params->tasks_arr[*entries_counter].call_id=*entries_counter;
    params->tasks_arr[*entries_counter].finished=finished;

    *entries_counter = *entries_counter + 1;
    return 0;
}
int check_if_flag_exists(int argc, char **argv, char *flag) {
    int i;

    for (i=2; i<argc; i++) {
        if(strcmp(argv[i], flag)==0) return 1;
    }
    return 0;
}
void tasks_arr_length (struct CliTask *tasks_arr, int *tasks_array_len, int *first_finished_task) {
    int found = 0; 
    while (*tasks_array_len<MAX_ENTRIES && tasks_arr[*tasks_array_len].unix_time!=0){
        *tasks_array_len+=1;
        if (found==0 && tasks_arr[*tasks_array_len].finished == 1) {
            found = 1;
            *first_finished_task = tasks_arr[*tasks_array_len].call_id;
        }
    }
    return;
}
void listing_handling_output (int finished, int *tasks_arr_len, int *first_finished_task, struct CliTask *tasks_arr, int rich_output) {
    int i = 0;
    while (i<*tasks_arr_len) {
        if (tasks_arr[i].finished == finished && rich_output == 1) {
            printf("Zadanie nr: %d\nNazwa zadania: %s\nOpis zadania: %s\nZaplanowano na: %s\nId z bazy danych: %s\nZaplanowano na (UNIX time): %lld\n\n",\
                    finished == 1 ? tasks_arr[i].call_id - *first_finished_task : tasks_arr[i].call_id,\
                    tasks_arr[i].name,\
                    tasks_arr[i].description,\
                    tasks_arr[i].string_time,
                    tasks_arr[i].rowid,
                    tasks_arr[i].unix_time);
        } 
        if (tasks_arr[i].finished == finished && rich_output == 0) {
            printf("Zadanie nr: %d\nNazwa zadania: %s\nOpis zadania: %s\nZaplanowano na: %s\n\n",\
                    finished == 1 ? tasks_arr[i].call_id - *first_finished_task : tasks_arr[i].call_id,\
                    tasks_arr[i].name,\
                    tasks_arr[i].description,\
                    tasks_arr[i].string_time);
        }
        i++;
    }
}
void listing_handling (int *entries_counter, struct CliTask *tasks_arr, int argc, char **argv, int *tasks_arr_len, int *first_finished_task) {

    if (check_if_flag_exists(argc, argv, "-o")==1) {
        printf("[Lista archiwalnych zadań:]\n\n");
        if (check_if_flag_exists(argc, argv, "-v")==1) {
            listing_handling_output(1, tasks_arr_len, first_finished_task, tasks_arr, 1);
        } else {
            listing_handling_output(1, tasks_arr_len, first_finished_task, tasks_arr, 0);
        }
    } else {
        printf("[Lista aktywnych zadań:]\n\n");
        if (check_if_flag_exists(argc, argv, "-v")==1) {
            listing_handling_output(0, tasks_arr_len, first_finished_task, tasks_arr, 1);
        } else {
            listing_handling_output(0, tasks_arr_len, first_finished_task, tasks_arr, 0);
        }
    }
}
void delete_handling (int argc, char **argv, int tasks_array_len, int first_finished_task, struct CliTask *tasks_arr) {
    int user_given_id;
    char *error;
    if (argc < 3) printf("Musisz podać numer zadania lub jego id z bazy danych po fladze -b\n");
    if (check_if_flag_exists(argc, argv, "-b")){
        user_given_id = strtol(argv[3], &error, 10);
        if (*error!='\0') {
            printf("Podana wartość musi być liczbą!\n");
            return;
        }
    }
}
void init_load(sqlite3 *db, char *err_msg, struct CliTask *tasks_arr, int *entries_counter) {
    char *sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks ORDER BY finished, date ASC;"; 
    struct CliLoadTaskParams params;
    int rc;

    params.tasks_arr = tasks_arr;
    params.entries_counter = entries_counter;
    rc = sqlite3_exec(db, sql, cli_load_tasks_callback, &params, &err_msg);
}

int cli_handling (int argc, char **argv, struct DbElements *db_elements) {
    sqlite3 *db = db_elements->db;
    char *err_msg = db_elements->err_msg;
    char *main_flag = argv[1];
    struct CliTask tasks_arr[MAX_ENTRIES] = {{0}};
    int entries_counter = 0;
    int tasks_array_len = 0;
    int rc = db_elements->rc;
    int first_finished_task = 0;

    init_load(db, err_msg, tasks_arr, &entries_counter);
    tasks_arr_length(tasks_arr, &tasks_array_len, &first_finished_task);

    if (strcmp(main_flag, "--help") ==0 || strcmp(main_flag, "-h")==0) {
        printf("%s", help_message());
    } else if(strcmp(main_flag, "-L")==0 || strcmp(main_flag, "-l")==0) {
        listing_handling(&entries_counter, tasks_arr, argc, argv, &tasks_array_len, &first_finished_task);
    } else if(strcmp(main_flag, "-D")==0 || strcmp(main_flag, "-d")==0) {
        delete_handling(argc, argv, tasks_array_len, first_finished_task, tasks_arr);
    } else {
        printf("Niepoprawna składnia polecenia! Wpisz --help, aby zapoznać się z dozwolonymi poleceniami!\n");
    }
    return 1;
}
