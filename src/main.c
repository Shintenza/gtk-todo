#include <stdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

#include "include/structs.h"
#include "include/activate.h"
#include "include/db_init.h"
#include "include/cli_handling.h"
#include "include/utils_h/first_time_launch.h"

int main(int argc, char **argv) {
    sqlite3 *db;
    GtkApplication *app;
    int status;
    int cli_response = 0;

    /* UIStates elements is_add_task_active, sent_wrong_date_alert, appended_inform_label, edit_mode, first_launch */
    struct UIStates ui_states = { 0, 0, 0, 0 };
    struct ActivateParams activate_params = { NULL, &ui_states }; 
    
    /*db initialization*/
    db_init(&db);
    activate_params.db = db;

    /*running cli version of the program*/
    if (argc >= 2) {
        cli_response = cli_handling(argc, argv, db);
    }

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), &activate_params);
    status = g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref (app);


    return status;
}
