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
    GtkWidget *right_box;
    GtkWidget *floating_add_button;
    struct DbElements *db_elements;
    char * string_date;
    gint64 unix_datetime;
};
struct ExistingElements {
    const char *existing_task_name;
    const char *existing_task_desc;
};
struct AddNewTaskParams {
    GtkWidget *window;
    GtkWidget *tasks_box;
    GtkWidget *right_box;
    GtkWidget *floating_add_button;
    int edit_mode;
    struct DbElements *db_elements;
    struct ExistingElements *existing_elements;
};
struct CancelAddingNewTaskParams {
    GtkWidget *floating_add_button;
    GtkWidget *add_task_box;
    GtkWidget *tasks_box;
    GtkWidget *existing_box;
    struct AddNewTaskParams *add_task_params;
};
struct CreateNewTaskBoxParams {
    GtkWidget *tasks_box;
    struct DbElements *db_elements;
    struct AddNewTaskParams *add_params;
    const char *task_name;
    const char *task_desc;
    const char *date_string;
    int importance;
    int *id;
    int finished;
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
struct MoveTaskParams {
    struct DbElements *db_elements;
    GtkWidget *tasks_box;
    int operation;
};
struct LoadTasksFromDbParams {
    GtkWidget *tasks_box;
    struct DbElements *db_elements;
    struct AddNewTaskParams *add_new_task_params;
    int finished;
};
struct ToggleTaskImportance {
    sqlite3 *db;
    const char *widget_name;
};
#endif
