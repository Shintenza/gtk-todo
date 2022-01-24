#include <gtk/gtk.h>
#include <sqlite3.h>

struct LoadTasksFromDbParams {
    GtkWidget *tasks_box;
    sqlite3 *db;
    int entries_counter;
    struct UIStates *ui_states;
    struct Task *tasks;
};
void load_archived_tasks(GtkWidget *button, gpointer user_data);
void load_active_tasks(GtkWidget *button, gpointer user_data);
void task_drawing (struct Task *tasks, GtkWidget *tasks_box, struct UIStates *ui_states, sqlite3 *db, int finished, int entries_counter);
