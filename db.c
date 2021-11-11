#include <stdio.h>
#include <sqlite3.h>

int callback(void *args, int argc, char **argv, char **col_names) {
    for (int i = 0; i < argc; i++) {
        printf("%s - %s\n", col_names[i], argv[i]);
    }
    return 0;
};
// NOTES

// Store data as unix time
/*
tasks
    task_name TEXT
    task_desc TEXT
    date TEXT
    importance TEXT
    finished BOOL
*/
int main() {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("todo.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot connect to the database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    char *sql="CREATE TABLE IF NOT EXISTS tasks (task_name TEXT, task_desc TEXT, date INT, importance TEXT, finished BOOLEAN NOT NULL CHECK (finished IN (0, 1)));"
                      "INSERT INTO tasks VALUES ('Zaliczyć projekt z C', 'xDD', strftime('%s', 'now'), 'normal', 0);";
    /* char *sql = "SELECT * FROM tasks"; */
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL err, %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}

