#include <stdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

#include "include/structs.h"
#include "include/activate.h"
#include "include/db_init.h"
#include "include/cli_handling.h"

int main(int argc, char **argv) {
    sqlite3 *db;
    GtkApplication *app;
    int status;
    int cli_response = 0;
    struct UIStates ui_states = { 0, 0, 0, 0 };
    db_init(&db);
    struct ActivateParams activate_params = { db, &ui_states };

    if (argc >= 2) {
        cli_response = cli_handling(argc, argv, db);
    }
    
    if (cli_response > 0) {
        return 0;
    }
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), &activate_params);
    status = g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref (app);


    return status;
}
