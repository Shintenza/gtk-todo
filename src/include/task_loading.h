#ifndef TASK_LOADING_H
#define TASK_LOADING_H
#include <gtk/gtk.h>
#include "../include/structs.h"
int load_tasks_from_db_callback (void *args, int argc, char **argv, char**col_name);
void load_tasks_from_db (sqlite3 *db, GtkWidget *tasks_box, char *importance, int finished, struct UIStates *ui_states);
void load_archived_tasks(GtkWidget *button, gpointer user_data);
void load_active_tasks(GtkWidget *button, gpointer user_data);
#endif
