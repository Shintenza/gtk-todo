#include <stdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

#include "include/structs.h"
#include "include/activate.h"

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("todo.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot connect to the database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    char *sql = "CREATE TABLE IF NOT EXISTS tasks (task_name TEXT, \
                                                   task_desc TEXT, \
                                                   date_string TEXT,\
                                                   date INT,       \
                                                   importance TEXT,\
                                                   finished BOOLEAN NOT NULL CHECK (finished IN (0, 1)));";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL err, %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    
    static struct DbElements db_elements;
    db_elements.db = db;
    db_elements.err_msg = err_msg;
    db_elements.rc = rc;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), &db_elements);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);


    return status;
}
