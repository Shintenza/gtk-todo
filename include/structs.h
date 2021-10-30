#ifndef STRUCTS_H
#define STRUCTS_H
#include <gtk/gtk.h>
struct TaskDataParams {
    GtkEntryBuffer *task_name_buffer;
    GtkEntryBuffer *task_desc_buffer;
    GtkWidget *window;
    GtkWidget *tasks_box;
    GtkWidget *add_task_box;
};
struct AddNewTaskParams {
   GtkWidget *window;
   GtkWidget *tasks_box;
};
struct CreateNewTaskBoxParams {
    GtkWidget *tasks_box;
    const char *task_name;
    const char *task_desc;
    const char *date_string;
};
#endif
