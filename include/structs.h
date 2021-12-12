#ifndef STRUCTS_H
#define STRUCTS_H
#include <gtk/gtk.h>
#include <sqlite3.h>

struct DbElements {
    char *err_msg;
    sqlite3 *db; 
    int rc;
};
struct ActivateParams {
    struct DbElements *db_elements;
    bool *is_add_task_active;
    bool *sent_wrong_date_alert; 
};
struct TaskDataParams {
    GtkEntryBuffer *task_name_buffer;
    GtkEntryBuffer *task_desc_buffer;
    GtkWidget *window;
    GtkWidget *tasks_box;
    GtkWidget *add_task_box;
    GtkWidget *date_label;
    GtkWidget *time_label;
    struct DbElements *db_elements;
    char * string_date;
    gint64 unix_datetime;
};
struct AddNewTaskParams {
   GtkWidget *window;
   GtkWidget *tasks_box;
   GtkWidget *right_box;
   struct DbElements *db_elements;
};
struct CreateNewTaskBoxParams {
    GtkWidget *tasks_box;
    struct DbElements *db_elements;
    const char *task_name;
    const char *task_desc;
    const char *date_string;
    int *id;
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
struct ArchiveTaskParams {
    struct DbElements *db_elements;
    GtkWidget *tasks_box;
};
struct LoadTasksFromDbParams {
    GtkWidget *tasks_box;
    struct DbElements *db_elements;
};
#endif
