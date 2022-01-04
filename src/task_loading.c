#include <gtk/gtk.h>
#include <sqlite3.h>
#include "include/structs.h"
#include "include/task_handling.h"

int load_tasks_from_db_callback (void *args, int argc, char **argv, char**col_name) {
    struct LoadTasksFromDbParams *load_tasks_from_db_args = args;
    struct UIStates *ui_states = load_tasks_from_db_args->ui_states;
    GtkWidget *tasks_box = load_tasks_from_db_args->tasks_box;
    int id = strtol(argv[0], NULL, 10);
    int importance = strtol(argv[5], NULL, 10);
    static struct CreateNewTaskBoxParams new_task_box_params;
    new_task_box_params.task_name = argv[1];
    new_task_box_params.task_desc = argv[2];
    new_task_box_params.tasks_box = tasks_box;
    new_task_box_params.date_string = argv[3];
    new_task_box_params.db = load_tasks_from_db_args->db;
    new_task_box_params.finished = load_tasks_from_db_args->finished;
    new_task_box_params.importance = importance;
    new_task_box_params.ui_states = ui_states;
    create_new_task_box(&new_task_box_params, id);
    return 0;
};
void load_tasks_from_db (sqlite3 *given_db, GtkWidget *tasks_box, char *importance, int finished, struct UIStates *ui_states) {
    char *sql;
    int rc;
    sqlite3 *db = given_db;
    char *err_msg;

    if( strcmp(importance, "normal") == 0 && finished == 0 ) {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 0"; 
    } else if ( finished == 1 ) {
       sql = "SELECT rowid, task_name, task_desc, date_string, date, importance, finished FROM tasks WHERE finished = 1"; 
    }

    struct LoadTasksFromDbParams load_tasks_from_db_callback_args;
    
    while (gtk_widget_get_last_child(tasks_box)!=NULL) {
        GtkWidget *last_child = gtk_widget_get_last_child(tasks_box);
        gtk_box_remove(GTK_BOX(tasks_box), last_child);
    }
    load_tasks_from_db_callback_args.tasks_box = tasks_box;
    load_tasks_from_db_callback_args.db= db;
    load_tasks_from_db_callback_args.finished = finished;
    load_tasks_from_db_callback_args.ui_states = ui_states;

    rc = sqlite3_exec(db, sql, load_tasks_from_db_callback, &load_tasks_from_db_callback_args, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
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
