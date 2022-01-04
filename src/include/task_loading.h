#ifndef TASK_LOADING
#define TASK_LOADING
#include <gtk/gtk.h>
#include "../include/structs.h"
int load_tasks_from_db_callback (void *args, int argc, char **argv, char**col_name);
void load_tasks_from_db (sqlite3 *db, GtkWidget *tasks_box, char *importance, int finished);
void load_archived_tasks(GtkWidget *button, gpointer user_data);
void load_active_tasks(GtkWidget *button, gpointer user_data);
#endif
