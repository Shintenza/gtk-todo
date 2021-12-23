#include <gtk/gtk.h>
#include <sqlite3.h>
#include "include/structs.h"
#include "include/task_handling.h"

int load_tasks_from_db_callback (void *args, int argc, char **argv, char**col_name) {
    struct LoadTasksFromDbParams *load_tasks_from_db_args = args;
    GtkWidget *tasks_box = load_tasks_from_db_args->tasks_box;
    int id = strtol(argv[0], NULL, 10);
    int importance = strtol(argv[5], NULL, 10);
    static struct CreateNewTaskBoxParams new_task_box_params;
    new_task_box_params.task_name = argv[1];
    new_task_box_params.task_desc = argv[2];
    new_task_box_params.tasks_box = tasks_box;
    new_task_box_params.date_string = argv[3];
    new_task_box_params.db_elements = load_tasks_from_db_args->db_elements;
    new_task_box_params.finished = load_tasks_from_db_args->finished;
    new_task_box_params.importance = importance;
    create_new_task_box(&new_task_box_params, id);
    return 0;
};
void load_tasks_from_db (struct DbElements *db_elements, GtkWidget *tasks_box, char *importance, int finished) {
    char *sql;
    int rc = db_elements->rc;
    sqlite3 *db = db_elements->db;

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
    load_tasks_from_db_callback_args.db_elements = db_elements;
    load_tasks_from_db_callback_args.finished = finished;

    rc = sqlite3_exec(db, sql, load_tasks_from_db_callback, &load_tasks_from_db_callback_args, &db_elements->err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", db_elements->err_msg);

        sqlite3_free(db_elements->err_msg);
        sqlite3_close(db);
    }
}
void load_archived_tasks(GtkWidget *button, gpointer user_data) {
    struct LoadTasksFromDbParams *load_tasks_params = user_data;
    load_tasks_from_db(load_tasks_params->db_elements, load_tasks_params->tasks_box, "normal", 1);
}
void load_active_tasks(GtkWidget *button, gpointer user_data) {
    struct LoadTasksFromDbParams *load_tasks_params = user_data;
    load_tasks_from_db(load_tasks_params->db_elements, load_tasks_params->tasks_box, "normal", 0);
}
