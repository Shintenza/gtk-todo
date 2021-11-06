#ifndef STRUCTS_H
#define STRUCTS_H
#include <gtk/gtk.h>
struct TaskDataParams {
    GtkEntryBuffer *task_name_buffer;
    GtkEntryBuffer *task_desc_buffer;
    GtkWidget *window;
    GtkWidget *tasks_box;
    GtkWidget *add_task_box;
    GtkWidget *date_label;
    GtkWidget *time_label;
    char * string_date;
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
struct HandleDate {
    GtkWidget *popover;
    GtkWidget *popover_box;
    GtkWidget *popover_sub_box;
    GtkWidget *add_task_box;
    GtkWidget *desc_entry;
    GtkWidget *calendar;
    GtkWidget *hour_input;
    GtkWidget *min_input;
    GtkWidget *add_date_button;
    struct TaskDataParams *params;
    bool sent_warning;
};
struct ResetWrongDateAlertBoxParams {
    GtkWidget *warning_label;
    GtkWidget *popover_box;
};
#endif
