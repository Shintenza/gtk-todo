#include <stdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

#include "include/structs.h"
#include "include/activate.h"
#include "include/db_init.h"
#include "include/cli_handling.h"

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    int cli_response = 0;
    bool is_add_task_active = false;
    bool sent_wrong_date_alert = false; 
    static struct DbElements db_elements;
    struct ActivateParams activate_params;

    db_init(&db_elements);
    activate_params.db_elements = &db_elements;
    activate_params.is_add_task_active = &is_add_task_active;
    activate_params.sent_wrong_date_alert = &sent_wrong_date_alert;

    if (argc >= 2) {
        cli_response = cli_handling(argc, argv, &db_elements);
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
