#include <gtk/gtk.h>
#include "include/structs.h"
#include "include/task_handling.h"

bool is_add_task_active = false;
bool sent_wrong_date_alert = false; 

void archive_task (GtkWidget *button, gpointer data) {
    struct ArchiveTaskParams *archive_task_params = data;
    struct DbElements *db_elements = archive_task_params->db_elements; 
    GtkWidget *tasks_box = archive_task_params->tasks_box;
    GtkWidget *parent = gtk_widget_get_parent(button);
    const char *string_id = gtk_widget_get_name(parent);
    int rc = db_elements->rc;
    sqlite3 *db = db_elements->db;
    char *sql = malloc(sizeof(char)*100);
    char *error;
    int id = strtol(string_id, &error, 10);

    sprintf(sql, "UPDATE tasks SET finished = 1 WHERE rowid = %d", id);


    rc = sqlite3_exec(db, sql, 0, 0, &db_elements->err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to remove data from db\n");
        fprintf(stderr, "SQL error: %s\n", db_elements->err_msg);

        sqlite3_free(db_elements->err_msg);
        sqlite3_close(db);
    }
    gtk_box_remove(GTK_BOX(tasks_box), parent);
    free(sql);
}

void rest_wrong_date_alert(GtkPopover *popover, gpointer data) {
    struct ResetWrongDateAlertBoxParams *params = data;
    if(sent_wrong_date_alert) {
        sent_wrong_date_alert = false;
        gtk_box_remove(GTK_BOX(params->popover_box), params->warning_label);
    }
}
void create_new_task_box(struct CreateNewTaskBoxParams *params, int id) {
    GtkWidget *tasks_box = params->tasks_box;
    const char *task_name = params->task_name;
    const char *task_desc = params->task_desc;
    const char *date_string = params->date_string;
    GtkStyleContext *context;


    char string_id[1000];
    sprintf(string_id, "%d", id);

    GtkWidget *single_task_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    context = gtk_widget_get_style_context(single_task_box);
    gtk_style_context_add_class(context, "single_task_box");

    gtk_widget_set_name(GTK_WIDGET(single_task_box), string_id);

    GtkWidget *task_name_label = gtk_label_new(task_name);
    GtkWidget *task_date_label = gtk_label_new(date_string);
    GtkWidget *task_desc_label = gtk_label_new(task_desc);
    GtkWidget *task_done_button = gtk_button_new_with_label("Ukończone");

    static struct ArchiveTaskParams archive_task_params;
    archive_task_params.db_elements = params->db_elements;
    archive_task_params.tasks_box = params->tasks_box;

    g_signal_connect(task_done_button, "clicked", G_CALLBACK(archive_task), &archive_task_params);
    
    gtk_box_append(GTK_BOX(single_task_box), task_name_label);
    gtk_box_append(GTK_BOX(single_task_box), task_desc_label);
    gtk_box_append(GTK_BOX(single_task_box), task_date_label);
    gtk_box_append(GTK_BOX(single_task_box), task_done_button);
    gtk_box_append(GTK_BOX(tasks_box), single_task_box);  
}
void data_handler(GtkWidget *button, gpointer data) {
    GtkWidget *dialog; 
    struct TaskDataParams *params = data; 
    GtkEntryBuffer *task_name_buffer = params->task_name_buffer;
    GtkEntryBuffer *task_desc_buffer = params->task_desc_buffer;
    GtkWidget *window = params->window;
    GtkWidget *tasks_box = params->tasks_box;
    GtkWidget *add_task_box = params->add_task_box;
    const gchar *task_name = gtk_entry_buffer_get_text(task_name_buffer);
    const gchar *task_desc = gtk_entry_buffer_get_text(task_desc_buffer);
    struct DbElements *db_elements = params->db_elements;

    int task_name_len = strlen(task_name);
    int task_desc_len = strlen(task_desc);

    if(task_name_len < 1 || task_desc_len < 1) {
        char *err_msg;
        if(task_name_len < 1 && task_desc_len < 1) {
            err_msg = "Nie wprowadzono żadnych danych";
        } else if (task_name_len < 1 ) {
            err_msg = "Nie podano nazy zadania";
        } else {
            err_msg = "Nie podano opisu zadania";
        }
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL;
        dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
                                                flags, 
                                                GTK_MESSAGE_ERROR, 
                                                GTK_BUTTONS_CLOSE, 
                                                err_msg);
        gtk_widget_set_name(GTK_WIDGET(dialog), "dialog");
        g_signal_connect (dialog, "response", G_CALLBACK (gtk_window_destroy), NULL);
        gtk_widget_show(GTK_WIDGET(dialog));
    } else {
        /* This static is usless ig */
        int element_id = sqlite3_last_insert_rowid(db_elements->db) + 1;

        static struct CreateNewTaskBoxParams fn_params;
        fn_params.tasks_box = tasks_box;
        fn_params.task_name = task_name;
        fn_params.task_desc = task_desc;
        fn_params.date_string = params->string_date;
        fn_params.db_elements = db_elements;

        create_new_task_box(&fn_params, element_id);
        is_add_task_active = false;
        gtk_box_remove(GTK_BOX(tasks_box), add_task_box);

        char *sql = malloc(sizeof(char)*1000);
        sprintf(sql, "INSERT INTO tasks VALUES ('%s', '%s', '%s', %lu, 'normal', 0);", task_name, task_desc, params->string_date, params->unix_datetime);
    
        db_elements->rc = sqlite3_exec(db_elements->db, sql, 0,0, &db_elements->err_msg);

        free(sql);
        if( db_elements->rc != SQLITE_OK ) {
            fprintf(stderr, "SQL err, %s\n", sqlite3_errmsg(db_elements->db));
            sqlite3_close(db_elements->db);
        }

    }
}
void date_handler (GtkMenuButton *button, gpointer data) {
    struct HandleDate *params = data;
    GDateTime *date = gtk_calendar_get_date(GTK_CALENDAR(params->calendar));
    int day = g_date_time_get_day_of_month(date);
    int month = g_date_time_get_month(date);
    int year = g_date_time_get_year(date);
    int hour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(params->hour_input));
    int min = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(params->min_input));
    GTimeZone *time_zone = g_time_zone_new_local();
    GtkWidget *popover_box = params->popover_box;
    
    GDateTime *selected_date = g_date_time_new(time_zone, year, month, day, hour, min, 0);
    GDateTime *date_now = g_date_time_new_now(time_zone);

    if(g_date_time_compare(selected_date, date_now) == -1) {
        GtkWidget *warning_label = gtk_label_new("Ustaw poprawną datę!");
        if (!sent_wrong_date_alert) {
            gtk_box_insert_child_after(GTK_BOX(popover_box), warning_label, params->popover_sub_box);
            static struct ResetWrongDateAlertBoxParams callback_params;
            callback_params.popover_box = popover_box;
            callback_params.warning_label = warning_label;
            g_signal_connect(params->popover, "closed", G_CALLBACK(rest_wrong_date_alert), &callback_params);

            sent_wrong_date_alert = true;
        }
    } else {
        char str_day[10];
        char str_month[10];
        char str_year[10];
        char str_hour[5];
        char str_minutes[5];

        sprintf(str_day,"%d/",day);
        sprintf(str_month,"%d/",month);
        sprintf(str_year,"%d",year);
        if(hour<10) {
            sprintf(str_hour, " 0%d:", hour);
        } else {
            sprintf(str_hour, "%d:", hour);
        }
        if (min<10) {
            sprintf(str_minutes, "0%d", min);
        } else {
            sprintf(str_minutes, "%d", min);
        }
        /* TODO free memory */
        int date_size = strlen(str_day) + strlen(str_month) + strlen(str_year) + 1;
        char *date_string = malloc(date_size);

        int time_size = strlen(str_hour) + strlen(str_minutes) + 1;
        char *time_string = malloc(time_size);

        date_string[0] = '\0';
        strcat(date_string, str_day);
        strcat(date_string, str_month);
        strcat(date_string, str_year);
        
        time_string[0] = '\0';
        strcat(time_string, str_hour);
        strcat(time_string, str_minutes);

        GtkWidget *date_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
        GtkWidget *inform_label = gtk_label_new("Ustawiono datę: ");
        GtkWidget *date_label = gtk_label_new(date_string);
        
        gtk_box_append(GTK_BOX(date_box), GTK_WIDGET(inform_label));
        
        inform_label = gtk_label_new("Ustawiono godzinę: ");
        GtkWidget *time_label = gtk_label_new(time_string);
        gtk_box_append(GTK_BOX(date_box), GTK_WIDGET(date_label));
        gtk_box_append(GTK_BOX(date_box), GTK_WIDGET(inform_label));
        gtk_box_append(GTK_BOX(date_box), GTK_WIDGET(time_label));
        
        if (!gtk_widget_get_parent(params->params->date_label)) {
            gtk_box_insert_child_after(GTK_BOX(params->add_task_box), date_box, params->desc_entry);
            params->params->date_label = date_label;
            params->params->time_label = time_label;
        } else {
            gtk_label_set_label(GTK_LABEL(params->params->date_label), date_string);
            gtk_label_set_label(GTK_LABEL(params->params->time_label), time_string);
        }
        /* TODO free memory here as well */

        char *complete_date = malloc(strlen("Zaplanowano na:  ") + strlen(date_string)+strlen(time_string)+strlen(" ")+1);
        complete_date[0] = '\0';
        strcat(complete_date, "Zaplanowano na:  ");
        strcat(complete_date, date_string);
        strcat(complete_date, " ");
        strcat(complete_date, time_string);
        params->params->string_date =  complete_date;
        params->params->unix_datetime = g_date_time_to_unix(selected_date);
        gtk_popover_popdown(GTK_POPOVER(params->popover));
        gtk_menu_button_set_label(GTK_MENU_BUTTON(params->add_date_button), "Zmień datę");
    } 
}
void add_new_task(GtkWidget *button, gpointer data) {
    if(!is_add_task_active) {
        struct AddNewTaskParams *add_new_task_params = data;
        GtkWidget *tasks_box = add_new_task_params->tasks_box;
        GtkWidget *parent_window = add_new_task_params->window;
        GtkWidget *add_date_button = gtk_menu_button_new();
        GtkWidget *popover = gtk_popover_new();
        GtkWidget *popover_box;
        GtkWidget *popover_sub_box;
        GtkWidget *calendar;
        GtkWidget *spin_button_hour;
        GtkWidget *spin_button_min;
        GtkWidget *button;
        GtkWidget *add_task_box, *task_name_entry, *task_desc_entry, *add_button, *label;
        GtkEntryBuffer *task_name_buffer, *task_desc_buffer;
        struct DbElements *db_elements = add_new_task_params->db_elements;
        

        add_task_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_widget_set_name(GTK_WIDGET(add_task_box), "add_task_box");
        
        label = gtk_label_new("Dodaj nowe zadanie");
        gtk_widget_set_name(GTK_WIDGET(label), "add_new_task_label");

        task_name_buffer = gtk_entry_buffer_new(NULL, 0);
        task_name_entry = gtk_entry_new_with_buffer(task_name_buffer);
        gtk_entry_set_placeholder_text(GTK_ENTRY(task_name_entry), "Nazwa zadania");

        task_desc_buffer = gtk_entry_buffer_new(NULL, 0);
        task_desc_entry = gtk_entry_new_with_buffer(task_desc_buffer);
        gtk_entry_set_placeholder_text(GTK_ENTRY(task_desc_entry), "Opis zadania");
        

        add_button = gtk_button_new_with_label("Dodaj");
        static struct TaskDataParams params;
        params.task_name_buffer = task_name_buffer;
        params.task_desc_buffer = task_desc_buffer;
        params.window = parent_window;
        params.tasks_box = tasks_box;
        params.add_task_box = add_task_box;
        params.db_elements = db_elements;

        gtk_menu_button_set_popover(GTK_MENU_BUTTON(add_date_button), GTK_WIDGET(popover));
        gtk_menu_button_set_label(GTK_MENU_BUTTON(add_date_button), "Ustaw datę");
        gtk_menu_button_set_always_show_arrow(GTK_MENU_BUTTON(add_date_button), FALSE);

        gtk_box_append(GTK_BOX(add_task_box), label);
        gtk_box_append(GTK_BOX(add_task_box), task_name_entry);
        gtk_box_append(GTK_BOX(add_task_box), task_desc_entry);
        gtk_box_append(GTK_BOX(add_task_box), add_date_button);

        gtk_box_append(GTK_BOX(add_task_box), add_button);

        gtk_box_prepend(GTK_BOX(tasks_box), add_task_box);
        is_add_task_active= !is_add_task_active;
        
        gtk_popover_set_has_arrow(GTK_POPOVER(popover), FALSE);
        popover_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_popover_set_child(GTK_POPOVER(popover), GTK_WIDGET(popover_box));
        calendar = gtk_calendar_new();
        gtk_box_append(GTK_BOX(popover_box), calendar);
        label = gtk_label_new("Ustaw godzinę/minutę");
        gtk_box_append(GTK_BOX(popover_box), GTK_WIDGET(label));
        spin_button_hour = gtk_spin_button_new_with_range(0, 24, 1);
        popover_sub_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_box_append(GTK_BOX(popover_sub_box), GTK_WIDGET(spin_button_hour));
        spin_button_min= gtk_spin_button_new_with_range(0, 60, 1);
        gtk_box_append(GTK_BOX(popover_sub_box), GTK_WIDGET(spin_button_min));
        gtk_box_append(GTK_BOX(popover_box), GTK_WIDGET(popover_sub_box));
        button = gtk_button_new_with_label("Potwierdź");
        gtk_box_append(GTK_BOX(popover_box), GTK_WIDGET(button));

        
        static struct HandleDate date_params;
        date_params.add_task_box = add_task_box;
        date_params.calendar = calendar;
        date_params.hour_input = spin_button_hour;
        date_params.min_input = spin_button_min;
        date_params.popover_box = popover_box;
        date_params.popover_sub_box = popover_sub_box;
        date_params.popover = popover;
        date_params.params = &params;
        date_params.desc_entry = task_desc_entry;
        date_params.add_date_button = add_date_button;

        g_signal_connect(button, "clicked", G_CALLBACK(date_handler), &date_params);
        g_signal_connect(add_button, "clicked", G_CALLBACK(data_handler), &params);
    }

}
