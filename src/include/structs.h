#ifndef STRUCTS_H
#define STRUCTS_H
#include <gtk/gtk.h>
#include <sqlite3.h>
#include "global.h"

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
    GtkTextBuffer *desc_buffer;
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
    struct UIStates *ui_states;
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
    GtkWidget *desc_box;
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
struct LoadTasksFromDbParams {
    GtkWidget *tasks_box;
    sqlite3 *db;
    int finished;
    int *tasks_counter;
    struct UIStates *ui_states;
};
struct ToggleTaskImportance {
    sqlite3 *db;
    const char *widget_name;
};
struct CliTask {
    int call_id;
    int rowid;
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESC_LENGTH];
    char string_time[MAX_NAME_LENGTH];
    long long unix_time;
    int finished;
    char response_msg[1000];
};
#endif
