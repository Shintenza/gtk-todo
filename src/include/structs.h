#ifndef STRUCTS_H
#define STRUCTS_H
#include <gtk/gtk.h>
#include <sqlite3.h>

struct UIStates {
    int is_add_task_active;
    int sent_wrong_date_alert; 
    int appended_inform_label;
    int edit_mode;
    int first_launch;
};
struct ActivateParams {
    sqlite3 *db;
    struct UIStates *ui_states;
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
    sqlite3 *db;
    char * string_date;
    gint64 unix_datetime;
    struct UIStates *ui_states;
};
struct AddNewTaskParams {
   GtkWidget *tasks_box;
   struct UIStates *ui_states;
   sqlite3 *db;
};
struct CancelAddingNewTaskParams {
    GtkWidget *floating_add_button;
    GtkWidget *add_task_box;
    GtkWidget *tasks_box;
    int edit_mode;
    gint handler_id;
    struct AddNewTaskParams *add_task_params;
};
struct CreateNewTaskBoxParams {
    GtkWidget *tasks_box;
    sqlite3 *db;
    const char *task_name;
    const char *task_desc;
    const char *date_string;
    int importance;
    int *id;
    int finished;
    int mode;
    struct UIStates *ui_states;
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
    struct UIStates *ui_states;
    struct TaskDataParams *params;
    bool sent_warning;
};
struct ResetWrongDateAlertBoxParams {
    GtkWidget *warning_label;
    GtkWidget *popover_box;
    struct UIStates *ui_states;
};
struct MoveTaskParams {
    sqlite3 *db;
    GtkWidget *tasks_box;
    int operation;
};
struct LoadTasksFromDbParams {
    GtkWidget *tasks_box;
    sqlite3 *db;
    int finished;
    struct UIStates *ui_states;
};
struct ToggleTaskImportance {
    sqlite3 *db;
    const char *widget_name;
};
#endif
