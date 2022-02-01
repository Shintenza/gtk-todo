#include "include/structs.h"
#include "include/utils_h/messages.h"
#include "include/utils_h/db_error.h"
#include "include/global.h"
#include "include/utils_h/dump_to_file.h"
#include "include/utils_h/date_validator.h"
#include "include/utils_h/get_main_dir.h"
#include <sys/stat.h>

struct CliLoadTaskParams {
    struct CliTask *tasks_arr;
    int *entries_counter;
};

int cli_load_tasks_callback(void *args,int argc, char **argv, char **col_name ) {
    struct CliLoadTaskParams *params = args;
    int *entries_counter = params->entries_counter;
    long long time = strtoll(argv[4], NULL, 10);
    unsigned int finished = strtoll(argv[6], NULL, 10);
    int rowid = strtol(argv[0], NULL, 10);

    if(*entries_counter>MAX_ENTRIES) {
        return 0;
    }

    strcpy(params->tasks_arr[*entries_counter].name, argv[1]);
    strcpy(params->tasks_arr[*entries_counter].description, argv[2]);
    strcpy(params->tasks_arr[*entries_counter].string_time, argv[3]);
    params->tasks_arr[*entries_counter].rowid = rowid;
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
            printf("Zadanie nr: %d\nNazwa zadania: %s\nOpis zadania: %s\nZaplanowano na: %s\nId z bazy danych: %d\nZaplanowano na (UNIX time): %lld\n\n",\
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
void delete_handling (int argc, char **argv, int tasks_array_len, int first_finished_task, struct CliTask *tasks_arr, sqlite3 *db) {
    int db_user_given_id = -1;
    int user_given_id = -1;
    int rc;
    char *error;
    char *sql = malloc(500);
    char user_input[4];
    char *return_message = malloc(1000);
    char *db_error_msg = 0;
    int i;
    struct CliTask chosen_task = {0};
    if (argc < 3) printf("Musisz podać numer zadania lub jego id z bazy danych po fladze -b\n");
    if (check_if_flag_exists(argc, argv, "-b") || check_if_flag_exists(argc, argv, "-n")){
        db_user_given_id = check_if_flag_exists(argc, argv, "-b") ? strtol(argv[3], &error, 10) : -1;
        user_given_id = check_if_flag_exists(argc, argv, "-n") ? strtol(argv[3], &error, 10) : -1;
        if (*error!='\0') {
            printf("Podana wartość musi być liczbą!\n");
            free(sql);
            free(return_message);
            return;
        }
    } else if (argc == 3 && check_if_flag_exists(argc, argv, "-x")) {
        for (i = 0; i<tasks_array_len; i++) {
            if (tasks_arr[i].finished == 1) {
                sprintf(sql, "DELETE FROM tasks WHERE rowid=%d", tasks_arr[i].rowid);
                rc = sqlite3_exec(db, sql, NULL, NULL, &db_error_msg);
                if (rc!=SQLITE_OK) {
                    free(sql);
                    free(return_message);
                    db_error_handling(&db, &db_error_msg);
                }
            }
        }
        sprintf(return_message, "Pomyślnie usunięto zadanie: %s z bazy danych!\n", tasks_arr[i].name);
        free(sql);
        free(return_message);
        return;
    } else if (argc == 3) {
        user_given_id = strtol(argv[2], &error, 10);
        if (*error!='\0') {
            printf("Podana wartość musi być liczbą!\n");
            free(sql);
            free(return_message);
            return;
        }
    } else {
        printf("Niepoprawne użycie komendy! Użyj --help aby otrzymać pomoc.\n");
        free(sql);
        free(return_message);
        return;
    }
    if (db_user_given_id>=0) {
        for (i = 0; i<tasks_array_len; i++) {
            if (db_user_given_id == tasks_arr[i].rowid) {
                chosen_task = tasks_arr[i];
            }
        }
    } else {
        for (i = 0; i< tasks_array_len; i++) {
            if (user_given_id == tasks_arr[i].call_id) {
                chosen_task = tasks_arr[i];
            }
        }
    }
    if (chosen_task.unix_time == 0) {
        printf("Nie znaleziono takiego zadania w naszej bazie danych!\n");
        free(sql);
        free(return_message);
        return;
    }
    if (chosen_task.finished==0) {
        sprintf(return_message, "Czy chcesz przenieść zadanie %s, do zadań ukończoncyh?\nWpisz tak/nie: ", chosen_task.name);
        sprintf(sql, "UPDATE tasks SET finished = 1 WHERE rowid=%d", chosen_task.rowid);
    }
    if (chosen_task.finished==1) {
        sprintf(return_message, "Wybrane zadanie: %s, ma status ukończonego. Czy chcesz usunąć dany wpis z bazy danych?\nWpisz tak/nie: ", chosen_task.name);
        sprintf(sql, "DELETE FROM tasks WHERE rowid=%d", chosen_task.rowid);
    }
    printf("%s", return_message);
    fgets(user_input, 4, stdin);

    if (strcmp(user_input, "tak")!=0) {
        free(sql);
        free(return_message);
        return;
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &db_error_msg);
    if (rc!=SQLITE_OK) {
        free(sql);
        free(return_message);
        db_error_handling(&db, &db_error_msg);
    }
    free(sql);
    free(return_message);
    printf("Operacja przebiegła pomyślnie!!!\n");
}
void init_load(sqlite3 *db, struct CliTask *tasks_arr, int *entries_counter) {
    char *sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks ORDER BY finished, date ASC;"; 
    char *db_error_msg;
    struct CliLoadTaskParams params;
    int rc;

    params.tasks_arr = tasks_arr;
    params.entries_counter = entries_counter;
    rc = sqlite3_exec(db, sql, cli_load_tasks_callback, &params, &db_error_msg);
    if (rc!=SQLITE_OK) {
        db_error_handling(&db, &db_error_msg);
    }
}
void adding_handling (int argc, char **argv, sqlite3 *db){
    int found_flags[3] = {0};
    int rc;
    char task_name[MAX_NAME_LENGTH];
    char task_desc[MAX_DESC_LENGTH];
    char task_date[17];
    char user_input[4];
    char day_string[3], month_string[3], year_string[5], hours_string[3], minutes_string[3];
    char *error;
    int err_counter = 0;
    int day, month, year, hours, minutes;
    int i;
    struct tm time_stamp = {0};
    time_t epoch;
    char *sql = malloc(MAX_NAME_LENGTH+MAX_DESC_LENGTH+17+15);
    char *db_error_msg = 0;

    if (argc != 8) {
        printf("Sprawdź składnię komendy! Aby to zrobić sprawdź --help!\n");
        return;
    }

    for (i=2; i<argc; i++) {
        if (strcmp(argv[i], "-t") == 0) {
            found_flags[0] = i;
        } else if (strcmp(argv[i], "-d") == 0) {
            found_flags[1] = i;
        } else if (strcmp(argv[i], "-tm") == 0) {
            found_flags[2] = i;
        }
    }
    for (i = 0; i<3; i++) {
        if (found_flags[i]==0) {
            printf("Nie podano wszystkich informacji wymaganych do uruchomienia tego polecenia. Sprawdź --help\n");
            return;
        }
    }
    if (strlen(argv[found_flags[0]+1])>MAX_NAME_LENGTH) {
        printf("Podano za długą nazwę zadania, pamiętaj że limit to %d znaków!\n", MAX_NAME_LENGTH);
        return;
    }
    strcpy(task_name, argv[found_flags[0]+1]);
    if (strlen(argv[found_flags[0]+1])>MAX_DESC_LENGTH) {
        printf("Podano za długą nazwę zadania, pamiętaj że limit to %d znaków!\n", MAX_NAME_LENGTH);
        return;
    }
    strcpy(task_desc, argv[found_flags[1]+1]);
    if (strlen(argv[found_flags[2]+1])!=16 ||\
            argv[found_flags[2]+1][2]!='/'||\
            argv[found_flags[2]+1][5]!='/'||\
            argv[found_flags[2]+1][10]!='/'||\
            argv[found_flags[2]+1][13]!=':')  {
        printf("Podano niepoprawną datę! Sprawdź poprawny format komendą --help\n");
        return;
    }
    strcpy(task_date, argv[found_flags[2]+1]);
    extract_from_datestring(task_date, day_string, month_string, year_string, hours_string, minutes_string);

    day = strtol(day_string, &error, 10);
    if (*error != '\0') err_counter++;
    month = strtol(month_string, &error, 10);
    if (*error != '\0') err_counter++;
    year = strtol(year_string, &error, 10);
    if (*error != '\0') err_counter++;
    hours = strtol(hours_string, &error, 10);
    if (*error != '\0') err_counter++;
    minutes = strtol(minutes_string, &error, 10);
    if (*error != '\0') err_counter++;
    
    if (err_counter > 0) {
        fprintf(stderr, "Wprowadzono nieprawidłową datę!\n");
        exit(1);
    }

    date_validator(day, month, year, hours, minutes);
    time_stamp.tm_mday = day; time_stamp.tm_mon = month-1; time_stamp.tm_year = year-1900; time_stamp.tm_hour = hours; time_stamp.tm_min = minutes; time_stamp.tm_sec = 0;
    epoch = mktime(&time_stamp);
    
    if (epoch < time(NULL)) {
        fprintf(stderr, "Nie potrafię podróżować w czasie, a w szczególności w przeszłość!\n");
        exit(1);
    }
    printf("Podsumowanie dodawania nowego zadania:\n\nNazwa zadania: %s\nOpis zadania: %s\nWprowadzony czas: %s\nCzy podane informacje się zgadzają? Wpisz tak lub nie: ", task_name, task_desc, task_date);
    fgets(user_input, 4, stdin);
    
    if (strcmp(user_input, "tak")!=0) {
        return;
    } 

    sprintf(sql, "INSERT INTO tasks VALUES ('%s', '%s', '%s', %lu, 'normal', 0);", task_name, task_desc, task_date, epoch);
    rc = sqlite3_exec(db, sql, NULL, NULL, &db_error_msg);
    if (rc!=SQLITE_OK) {
        free(sql);
        db_error_handling(&db, &db_error_msg);
    }
    printf("Pomyślnie dodano nowe zadanie!!!\n");
    free(sql);
}
void write_to_file (int argc, int arr_len, char **argv, struct CliTask *tasks_arr){
    int mode = 0;
    char path[500];
    if (check_if_flag_exists(argc, argv, "-o")) {
        mode = 1;
    }
    if ((argc-mode)<3 || argc > 3+mode) {
        fprintf(stderr, "Niepoprawna składnia polecenia! Zapoznaj się z --help!\n");
        exit(-1);
    }
    realpath(argv[argc-1], path);
    dump_to_file(tasks_arr, mode, arr_len, path);
}
void change_default_db_dest (int argc, char **argv) {
    char path_to_db[500];
    char new_location[500];
    char path_to_cache[500];
    char buffer[500];
    int mode = 0;
    int rc;
    FILE *file;
    char username[30];
    struct stat st = {0};
    sqlite3 *test_db;
    
    if (check_if_flag_exists(argc, argv, "-n")) {
        printf("Obecne położenie bazy danych: %s\n", get_db_path());
        exit(0);
    }
    if (check_if_flag_exists(argc, argv, "-c")) {
        mode = 1;
    }
    if (argc!=4) {
        fprintf(stderr, "Niepoprawna składnia polecenia! Zapoznaj się z treścią polecenia --help!\n");
        exit(1);
    }
    getlogin_r(username, sizeof(username));
    if (strlen(username) <= 0) {
        fprintf(stderr, "Coś poszło nie tak!\n");
        exit(-1);
    }
    sprintf(path_to_cache, "/home/%s/.cache", username);

    if (stat(path_to_cache, &st) == -1) {
        mkdir(path_to_cache, 0755);
    }
    realpath(argv[2+mode], path_to_db);

    if (!mode || (mode==1 && fopen(path_to_db, "r"))){
        rc = sqlite3_open(path_to_db, &test_db);
        if (rc!=SQLITE_OK) {
            fprintf(stderr, "Podano niepoprawny plik\n");
            sqlite3_close(test_db);
            exit(1);
        }
        rc = sqlite3_exec(test_db, "SELECT * FROM tasks", 0, 0, NULL); 
        if (rc!=SQLITE_OK) {
            if (mode == 0) {
                fprintf(stderr, "Podano niepoprawny plik");
                sqlite3_close(test_db);
                exit(1);
            }
        }
        sqlite3_close(test_db);
    }

    strcat(path_to_cache, "/c_todo_location");
    if (mode == 0) {
        realpath(argv[3+mode], new_location);
        fgets(buffer, sizeof(buffer), fopen(path_to_cache, "r"));
        if (strcmp(path_to_db, buffer) != 0) {
            fprintf(stderr, "To nie jest stary plik bazy danych!\n");
            exit(1);
        }
        rc = rename(path_to_db, new_location);
        if (rc < 0) {
            fprintf(stderr, "Nie udało mi się przenieść bazy danych! Sprawdź czy podany plik już nie istnieje lub czy masz odpowiedni dostęp\n");
            exit(1);
        }

        file = fopen(path_to_cache, "w+");
        fputs(new_location, file);
    } else {
        file = fopen(path_to_cache, "w+");
        fputs(path_to_db, file);
    }
}
/* core of cli */
int cli_handling (int argc, char **argv, sqlite3 *given_db) {
    sqlite3 *db = given_db;
    char *main_flag = argv[1];
    int entries_counter = 0;
    int tasks_array_len = 0;
    int first_finished_task = 0;

    struct CliTask tasks_arr[MAX_ENTRIES] = {{0}};

    init_load(db, tasks_arr, &entries_counter);
    tasks_arr_length(tasks_arr, &tasks_array_len, &first_finished_task);

    if (strcmp(main_flag, "--help") == 0 || strcmp(main_flag, "-h")==0) {
        printf("%s", help_message());
    } else if(strcmp(main_flag, "-L")==0 || strcmp(main_flag, "-l")==0) {
        listing_handling(&entries_counter, tasks_arr, argc, argv, &tasks_array_len, &first_finished_task);
    } else if(strcmp(main_flag, "-D")==0 || strcmp(main_flag, "-d")==0) {
        delete_handling(argc, argv, tasks_array_len, first_finished_task, tasks_arr, db);
    } else if(strcmp(main_flag, "-A")==0 || strcmp(main_flag, "-a")==0){
        adding_handling(argc, argv, db);
    } else if(strcmp(main_flag, "-W")==0 || strcmp(main_flag, "-w")==0) {
        write_to_file(argc, tasks_array_len, argv, tasks_arr);
    } else if (strcmp(main_flag, "-F")==0 || strcmp(main_flag, "-f")==0) {
        change_default_db_dest(argc, argv);
    } else {
        printf("Niepoprawna składnia polecenia! Wpisz --help, aby zapoznać się z dozwolonymi poleceniami!\n");
        exit(1);
    }
    exit(0);
}
