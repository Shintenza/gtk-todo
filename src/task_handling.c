#include <gtk/gtk.h>
#include "include/structs.h"
#include "include/task_handling.h"

bool is_add_task_active = false;
bool sent_wrong_date_alert = false; 
bool appended_inform_label = false;

void task_move(GtkWidget *button, gpointer data) {
    struct MoveTaskParams *move_task_params = data;
    GtkWidget *task_box = gtk_widget_get_parent(gtk_widget_get_parent(button));
    GtkWidget *tasks_box = gtk_widget_get_parent(task_box);
    int rc;
    int operation = move_task_params->operation;
    sqlite3 *db = move_task_params->db_elements->db;
    char *err_msg = 0;
    char *sql = malloc(100);
    const char *element_id = gtk_widget_get_name(task_box);
    switch (operation) {
        case 1:
            sprintf(sql, "UPDATE tasks SET finished = 1 WHERE rowid = %s", element_id);
            break;
        case 2:
            sprintf(sql, "UPDATE tasks SET finished = 0 WHERE rowid = %s", element_id);
            break;
        default:
            sprintf(sql, "DELETE FROM tasks WHERE rowid = %s", element_id);
    }
    gtk_box_remove(GTK_BOX(tasks_box), task_box);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Operation failed\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }
    free(sql);
}
void task_remove(GtkWidget *button, gpointer data) {
    struct DbElements *db_elements= data;
    GtkWidget *task_box = gtk_widget_get_parent(gtk_widget_get_parent(button));
    GtkWidget *tasks_box = gtk_widget_get_parent(task_box);
    int rc;
    sqlite3 *db =db_elements->db;
    char *err_msg = 0;
    char *sql = malloc(100);
    const char *element_id = gtk_widget_get_name(task_box);
    sprintf(sql, "DELETE FROM tasks WHERE rowid = %s", element_id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Operation failed\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }
    gtk_box_remove(GTK_BOX(tasks_box), task_box);
    free(sql);
}
void rest_wrong_date_alert(GtkPopover *popover, gpointer data) {
    struct ResetWrongDateAlertBoxParams *params = data;
    if(sent_wrong_date_alert) {
        sent_wrong_date_alert = false;
        gtk_box_remove(GTK_BOX(params->popover_box), params->warning_label);
    }
}
void toggle_task_importance (GtkWidget *button, gpointer data) {
    GtkWidget *task_box = gtk_widget_get_parent(gtk_widget_get_parent(button));
    const char *button_name = gtk_widget_get_name(button);
    const char *element_id = gtk_widget_get_name(task_box);
    sqlite3 *db = data;
    char *sql = malloc(100);
    int rc;
    char *err_msg =0;
    if (strcmp(button_name, "important_button") == 0) {
        sprintf(sql, "UPDATE tasks SET importance = 1 WHERE rowid = %s", element_id);
        gtk_widget_set_name(button, "important_button_toggled");
    } else {
        sprintf(sql, "UPDATE tasks SET importance = 0 WHERE rowid = %s", element_id);
        gtk_widget_set_name(button, "important_button");
    }

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Operation failed\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }
    return; 
}
void create_new_task_box(struct CreateNewTaskBoxParams *params, int id) {
    const char *task_name = params->task_name;
    const char *task_desc = params->task_desc;
    const char *date_string = params->date_string;
    int finished = params->finished;
    char *date_string_label = malloc(40);
    char string_id[1000];
    GtkWidget *tasks_box = params->tasks_box;
    GtkWidget *task_name_label = gtk_label_new(task_name);
    GtkWidget *task_desc_label = gtk_label_new(task_desc);
    GtkWidget *task_date_label;
    GtkWidget *task_done_button;
    GtkWidget *task_remove_button = gtk_button_new_with_label("Usuń");
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *single_task_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *importatnt_button = gtk_button_new_with_label("");
    GtkWidget *name_and_important_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkStyleContext *context;
    static struct MoveTaskParams move_task_params;

    sprintf(string_id, "%d", id);

    context = gtk_widget_get_style_context(single_task_box);
    gtk_style_context_add_class(context, "single_task_box");

    sprintf(date_string_label, "Zaplanowano na: %s", date_string);
    task_date_label=gtk_label_new(date_string_label);
    
    move_task_params.db_elements = params->db_elements;
    move_task_params.tasks_box = params->tasks_box;

    if (finished == 0) {
        move_task_params.operation = 1;
        task_done_button = gtk_button_new_with_label("Ukończone");
        g_signal_connect(task_done_button, "clicked", G_CALLBACK(task_move), &move_task_params);
    } else {
        move_task_params.operation = 2;
        task_done_button = gtk_button_new_with_label("Przywróć");
        g_signal_connect(task_done_button, "clicked", G_CALLBACK(task_move), &move_task_params);
    } 
    
    gtk_widget_set_name(task_remove_button, "task_remove_button");
    gtk_widget_set_name(task_done_button, "task_add_button");
    gtk_widget_set_name(task_name_label, "task_name_label");
    gtk_widget_set_name(name_and_important_box, "ni_box");
    if (params->importance == 0) {
        gtk_widget_set_name(importatnt_button, "important_button");
    } else {
        gtk_widget_set_name(importatnt_button, "important_button_toggled");
    }
    gtk_widget_set_name(GTK_WIDGET(single_task_box), string_id);

    gtk_widget_set_halign(button_box, GTK_ALIGN_END);
    gtk_widget_set_valign(button_box, GTK_ALIGN_END);
    gtk_widget_set_halign(task_date_label, GTK_ALIGN_START);
    gtk_widget_set_halign(task_desc_label, GTK_ALIGN_START);
    gtk_widget_set_halign(task_name_label, GTK_ALIGN_START);
    gtk_widget_set_halign(importatnt_button, GTK_ALIGN_END);
    gtk_widget_set_hexpand(task_name_label, TRUE);
    gtk_widget_set_hexpand(name_and_important_box, TRUE);

    gtk_box_append(GTK_BOX(single_task_box), name_and_important_box);
    gtk_box_append(GTK_BOX(single_task_box), task_date_label);
    gtk_box_append(GTK_BOX(single_task_box), task_desc_label);
    gtk_box_append(GTK_BOX(single_task_box), button_box);
    gtk_box_append(GTK_BOX(name_and_important_box), task_name_label);
    gtk_box_append(GTK_BOX(name_and_important_box), importatnt_button);
    gtk_box_append(GTK_BOX(button_box), task_remove_button);
    gtk_box_append(GTK_BOX(button_box), task_done_button);
    gtk_box_append(GTK_BOX(tasks_box), single_task_box);  
    
    params->date_string = 0;
    appended_inform_label = false;
    g_signal_connect(task_remove_button, "clicked", G_CALLBACK(task_remove), params->db_elements);
    g_signal_connect(importatnt_button, "clicked", G_CALLBACK(toggle_task_importance), params->db_elements->db);
}
void data_handler(GtkWidget *button, gpointer data) {
    GtkWidget *dialog; 
    struct TaskDataParams *add_task_params = data; 
    GtkEntryBuffer *task_name_buffer = add_task_params->task_name_buffer;
    GtkEntryBuffer *task_desc_buffer = add_task_params->task_desc_buffer;
    GtkWidget *window = add_task_params->window;
    GtkWidget *tasks_box = add_task_params->tasks_box;
    GtkWidget *add_task_box = add_task_params->add_task_box;
    GtkWidget *floating_add_button = add_task_params->floating_add_button;
    const gchar *task_name = gtk_entry_buffer_get_text(task_name_buffer);
    const gchar *task_desc = gtk_entry_buffer_get_text(task_desc_buffer);
    struct DbElements *db_elements = add_task_params->db_elements;
    static struct AddNewTaskParams add_new_task_params;

    int task_name_len = strlen(task_name);
    int task_desc_len = strlen(task_desc);
    if(task_name_len < 1 || task_desc_len < 1 || add_task_params->string_date == 0) {
        char *err_msg;
        if(task_name_len < 1 && task_desc_len < 1) {
            err_msg = "Nie wprowadzono żadnych danych";
        } else if (task_name_len < 1 ) {
            err_msg = "Nie podano nazy zadania";
        } else if (add_task_params->string_date == 0) {
            err_msg = "Nie podano żadnej daty!";
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
        fn_params.finished = 0;
        fn_params.date_string = add_task_params->string_date;
        fn_params.db_elements = db_elements;

        create_new_task_box(&fn_params, element_id);
        is_add_task_active = false;
        gtk_box_remove(GTK_BOX(add_task_params->right_box), add_task_box);
        gtk_widget_set_sensitive(GTK_WIDGET(tasks_box), true);

        char *sql = malloc(sizeof(char)*1000);
        sprintf(sql, "INSERT INTO tasks VALUES ('%s', '%s', '%s', %lu, 0, 0);", task_name, task_desc, add_task_params->string_date, add_task_params->unix_datetime);
    
        db_elements->rc = sqlite3_exec(db_elements->db, sql, 0,0, &db_elements->err_msg);
        gtk_button_set_label(GTK_BUTTON(floating_add_button), "+");
        gtk_widget_set_name(floating_add_button, "f_add_button");
        add_new_task_params.window = window;
        add_new_task_params.floating_add_button = floating_add_button;
        add_new_task_params.tasks_box = tasks_box;
        add_new_task_params.right_box = add_task_params->right_box;
        add_new_task_params.db_elements  = db_elements;
        g_signal_connect(floating_add_button, "clicked", G_CALLBACK(add_new_task), &add_new_task_params);
        free(sql);
        if( db_elements->rc != SQLITE_OK ) {
            fprintf(stderr, "SQL err, %s\n", sqlite3_errmsg(db_elements->db));
            sqlite3_close(db_elements->db);
        }
    }
}
void date_handler (GtkMenuButton *button, gpointer data) {
    struct HandleDate *params = data;
    static struct ResetWrongDateAlertBoxParams callback_params;
    GDateTime *date = gtk_calendar_get_date(GTK_CALENDAR(params->calendar));
    int day = g_date_time_get_day_of_month(date);
    int month = g_date_time_get_month(date);
    int year = g_date_time_get_year(date);
    int hour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(params->hour_input));
    int min = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(params->min_input));
    GTimeZone *time_zone = g_time_zone_new_local();

    char str_day[3];
    char str_month[3];
    char str_year[5];
    char str_hour[3];
    char str_minutes[3];
    
    char date_string[12];
    char time_string[6];
    static char complete_date[20];
    
    GDateTime *selected_date = g_date_time_new(time_zone, year, month, day, hour, min, 0);
    GDateTime *date_now = g_date_time_new_now(time_zone);
    GtkWidget *warning_label = gtk_label_new("Ustaw poprawną datę!");
    GtkWidget *date_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    GtkWidget *inform_label = gtk_label_new("Ustawiono datę: ");
    GtkWidget *date_label = gtk_label_new(date_string);
    GtkWidget *time_label = gtk_label_new(time_string);
    GtkWidget *popover_box = params->popover_box;


    if(g_date_time_compare(selected_date, date_now) == -1) {
        if (!sent_wrong_date_alert) {
            gtk_box_insert_child_after(GTK_BOX(popover_box), warning_label, params->popover_sub_box);
            callback_params.popover_box = popover_box;
            callback_params.warning_label = warning_label;
            g_signal_connect(params->popover, "closed", G_CALLBACK(rest_wrong_date_alert), &callback_params);

            sent_wrong_date_alert = true;
        }
    } else {

        sprintf(str_year,"%d",year);
        if (month<10) sprintf(str_month, "0%d", month);
        else sprintf(str_month, "%d", month);
        if (day < 10) sprintf(str_day, "0%d", day);
        else sprintf(str_day, "%d", day);
        if(hour<10) sprintf(str_hour, "0%d", hour);
        else sprintf(str_hour, "%d", hour); 
        if (min<10) sprintf(str_minutes, "0%d", min);
        else sprintf(str_minutes, "%d", min);

        sprintf(date_string, "%s/%s/%s", str_day, str_month, str_year);

        sprintf(time_string, "%s:%s", str_hour, str_minutes);

        date_label = gtk_label_new(date_string);
        
        gtk_box_append(GTK_BOX(date_box), GTK_WIDGET(inform_label));
        
        inform_label = gtk_label_new("Ustawiono godzinę: ");
        time_label = gtk_label_new(time_string);
        gtk_box_append(GTK_BOX(date_box), GTK_WIDGET(date_label));
        gtk_box_append(GTK_BOX(date_box), GTK_WIDGET(inform_label));
        gtk_box_append(GTK_BOX(date_box), GTK_WIDGET(time_label));
        
        if (!appended_inform_label) {
            gtk_box_insert_child_after(GTK_BOX(params->add_task_box), date_box, params->desc_entry);
            params->params->date_label = date_label;
            params->params->time_label = time_label;
            appended_inform_label = true;
        } else {
            gtk_label_set_label(GTK_LABEL(params->params->date_label), date_string);
            gtk_label_set_label(GTK_LABEL(params->params->time_label), time_string);
        }

        sprintf(complete_date, "%s %s", date_string, time_string);
        params->params->string_date =  complete_date;
        params->params->unix_datetime = g_date_time_to_unix(selected_date);
        gtk_popover_popdown(GTK_POPOVER(params->popover));
        gtk_menu_button_set_label(GTK_MENU_BUTTON(params->add_date_button), "Zmień datę");
    } 
}
void cancel_adding_new_task(GtkWidget *button, gpointer data){
    struct CancelAddingNewTaskParams *cancel_params = data; 
    GtkWidget *floating_add_button = cancel_params->floating_add_button;
    GtkWidget *add_task_box = cancel_params->add_task_box;
    GtkWidget *add_task_box_parent = gtk_widget_get_parent(add_task_box);
    GtkWidget *existing_task = gtk_widget_get_first_child(cancel_params->tasks_box);

    gtk_box_remove(GTK_BOX(add_task_box_parent), add_task_box);
    gtk_widget_set_name(floating_add_button, "f_add_button");
    gtk_button_set_label(GTK_BUTTON(floating_add_button), "+");
    is_add_task_active = !is_add_task_active;

    gtk_widget_set_sensitive(existing_task, true);
    while(gtk_widget_get_next_sibling(existing_task)!=NULL) {
        existing_task = gtk_widget_get_next_sibling(existing_task);
        gtk_widget_set_sensitive(existing_task, true);
    }

    g_signal_connect(floating_add_button, "clicked", G_CALLBACK(add_new_task), cancel_params->add_task_params);
}
void add_new_task(GtkWidget *button, gpointer data) {
    if(!is_add_task_active) {
        struct AddNewTaskParams *add_new_task_params = data;
        GtkWidget *tasks_box = add_new_task_params->tasks_box;
        GtkWidget *right_box = add_new_task_params->right_box;
        GtkWidget *parent_window = add_new_task_params->window;
        GtkWidget *add_date_button = gtk_menu_button_new();
        GtkWidget *floating_add_button = add_new_task_params->floating_add_button;
        GtkWidget *popover = gtk_popover_new(), *popover_box, *popover_sub_box;
        GtkWidget *calendar, *spin_button_hour, *spin_button_min, *button;
        GtkWidget *add_task_box, *task_name_entry, *task_desc_entry, *add_button, *label;
        GtkWidget *existing_task = gtk_widget_get_first_child(tasks_box);
        GtkEntryBuffer *task_name_buffer, *task_desc_buffer;

        struct DbElements *db_elements = add_new_task_params->db_elements;
        static struct TaskDataParams params;
        static struct HandleDate date_params;
        static struct CancelAddingNewTaskParams cancel_params;

        gtk_button_set_label(GTK_BUTTON(floating_add_button), "X");
        gtk_widget_set_name(floating_add_button, "f_add_button_red");
        

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
        params.task_name_buffer = task_name_buffer;
        params.task_desc_buffer = task_desc_buffer;
        params.window = parent_window;
        params.tasks_box = tasks_box;
        params.add_task_box = add_task_box;
        params.db_elements = db_elements;
        params.right_box = right_box;
        params.floating_add_button = floating_add_button;
        params.string_date = 0;
        params.unix_datetime = 0;

        gtk_menu_button_set_popover(GTK_MENU_BUTTON(add_date_button), GTK_WIDGET(popover));
        gtk_menu_button_set_label(GTK_MENU_BUTTON(add_date_button), "Ustaw datę");
        gtk_menu_button_set_always_show_arrow(GTK_MENU_BUTTON(add_date_button), FALSE);

        gtk_box_append(GTK_BOX(add_task_box), label);
        gtk_box_append(GTK_BOX(add_task_box), task_name_entry);
        gtk_box_append(GTK_BOX(add_task_box), task_desc_entry);
        gtk_box_append(GTK_BOX(add_task_box), add_date_button);
        gtk_box_append(GTK_BOX(add_task_box), add_button);
       
        /*locks others tasks interactions while adding a new one*/

        gtk_widget_set_sensitive(existing_task, false);
        while(gtk_widget_get_next_sibling(existing_task)!=NULL) {
            existing_task = gtk_widget_get_next_sibling(existing_task);
            gtk_widget_set_sensitive(existing_task, false);
        }

        gtk_box_prepend(GTK_BOX(right_box), add_task_box);
        is_add_task_active= !is_add_task_active;

        popover_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        calendar = gtk_calendar_new();
        label = gtk_label_new("Ustaw godzinę/minutę");
        spin_button_hour = gtk_spin_button_new_with_range(0, 24, 1);
        popover_sub_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        spin_button_min= gtk_spin_button_new_with_range(0, 60, 1);
        button = gtk_button_new_with_label("Potwierdź");

        gtk_popover_set_has_arrow(GTK_POPOVER(popover), FALSE);
        gtk_popover_set_child(GTK_POPOVER(popover), GTK_WIDGET(popover_box));
        gtk_box_append(GTK_BOX(popover_box), calendar);
        gtk_box_append(GTK_BOX(popover_box), GTK_WIDGET(label));
        gtk_box_append(GTK_BOX(popover_sub_box), GTK_WIDGET(spin_button_hour));
        gtk_box_append(GTK_BOX(popover_sub_box), GTK_WIDGET(spin_button_min));
        gtk_box_append(GTK_BOX(popover_box), GTK_WIDGET(popover_sub_box));
        gtk_box_append(GTK_BOX(popover_box), GTK_WIDGET(button));

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

        cancel_params.add_task_box = add_task_box;
        cancel_params.floating_add_button = floating_add_button;
        cancel_params.add_task_params = add_new_task_params;
        cancel_params.tasks_box = tasks_box;

        g_signal_connect(button, "clicked", G_CALLBACK(date_handler), &date_params);
        g_signal_connect(add_button, "clicked", G_CALLBACK(data_handler), &params);
        g_signal_connect(floating_add_button, "clicked", G_CALLBACK(cancel_adding_new_task), &cancel_params);
    }
}
