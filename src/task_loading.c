#include <gtk/gtk.h>
#include <sqlite3.h>
#include "include/structs.h"
#include "include/task_handling.h"
#include "include/utils_h/first_time_launch.h"

int load_tasks_from_db_callback (void *args, int argc, char **argv, char**col_name) {
    struct LoadTasksFromDbParams *given_data = args;
    struct UIStates *ui_states = given_data->ui_states;
    GtkWidget *tasks_box = given_data->tasks_box;
    int *tasks_counter = given_data->tasks_counter;
    int id = strtol(argv[0], NULL, 10);
    int importance = strtol(argv[5], NULL, 10);
    static struct CreateNewTaskBoxParams new_task_box_params;
    
    ui_states->first_launch = 0;

    new_task_box_params.task_name = argv[1];
    new_task_box_params.task_desc = argv[2];
    new_task_box_params.tasks_box = tasks_box;
    new_task_box_params.date_string = argv[3];
    new_task_box_params.db = given_data->db;
    new_task_box_params.finished = given_data->finished;
    new_task_box_params.importance = importance;
    new_task_box_params.ui_states = ui_states;
    new_task_box_params.mode = 0;
    create_new_task_box(&new_task_box_params, id);
    (*tasks_counter)++;
    return 0;
};
void load_tasks_from_db (sqlite3 *given_db, GtkWidget *tasks_box, char *importance, int finished, struct UIStates *ui_states) {
    char *sql;
    int rc;
    sqlite3 *db = given_db;
    char *err_msg;
    int tasks_counter = 0;

    if( strcmp(importance, "normal") == 0 && finished == 0 ) {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 0 ORDER BY date ASC"; 
    } else if ( finished == 1 ) {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 1 ORDER BY date ASC"; 
    }

    struct LoadTasksFromDbParams callback_args;
    
    while (gtk_widget_get_last_child(tasks_box)!=NULL) {
        GtkWidget *last_child = gtk_widget_get_last_child(tasks_box);
        gtk_box_remove(GTK_BOX(tasks_box), last_child);
    }

    callback_args.tasks_box = tasks_box;
    callback_args.db= db;
    callback_args.finished = finished;
    callback_args.ui_states = ui_states;
    callback_args.tasks_counter = &tasks_counter;

    rc = sqlite3_exec(db, sql, load_tasks_from_db_callback, &callback_args, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
    if (tasks_counter == 0) {
        ui_states->first_launch = 1;
        if (finished == 0)
            first_time_launch(tasks_box, 1);
        else 
            first_time_launch(tasks_box, 0);
    }
} 
void load_archived_tasks(GtkWidget *button, gpointer user_data) {
    struct LoadTasksFromDbParams *load_tasks_params = user_data;
    load_tasks_from_db(load_tasks_params->db, load_tasks_params->tasks_box, "normal", 1, load_tasks_params->ui_states);
}
void load_active_tasks(GtkWidget *button, gpointer user_data) {
    struct LoadTasksFromDbParams *load_tasks_params = user_data;
    load_tasks_from_db(load_tasks_params->db, load_tasks_params->tasks_box, "normal", 0, load_tasks_params->ui_states);
}
