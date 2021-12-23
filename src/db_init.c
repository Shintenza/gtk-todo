#include "include/structs.h"
#include <gtk/gtk.h>
#include <sqlite3.h>

int db_init(struct DbElements *db_elements) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("./todo.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot connect to the database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    char *sql = "CREATE TABLE IF NOT EXISTS tasks (task_name TEXT, \
                                                   task_desc TEXT, \
                                                   date_string TEXT,\
                                                   date INT,       \
                                                   importance BOOLEAN NOT NULL CHECK (finished IN (0, 1)),\
                                                   finished BOOLEAN NOT NULL CHECK (finished IN (0, 1)));";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL err, %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    db_elements->db = db;
    db_elements->err_msg = err_msg;
    db_elements->rc = rc; 
    return 0;
}
