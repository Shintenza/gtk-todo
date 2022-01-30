#ifndef TASK_HANDLING_H
#define TASK_HANDLING_H
#include <gtk/gtk.h>
#include "../include/structs.h"

void archive_task (GtkWidget *button, gpointer data);
void rest_wrong_date_alert(GtkPopover *popover, gpointer data);
void create_new_task_box(struct CreateNewTaskBoxParams *params, int id);
void data_handler(GtkWidget *button, gpointer data);
void date_handler (GtkMenuButton *button, gpointer data);
void add_new_task(GtkWidget *button, gpointer data);
void cancel_adding_new_task(GtkWidget *button, gpointer data);
#endif
