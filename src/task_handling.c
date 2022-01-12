#include <gtk/gtk.h>
#include "include/structs.h"
#include "include/task_handling.h"

void task_move(GtkWidget *button, gpointer data) {
    struct MoveTaskParams *move_task_params = data;
    GtkWidget *task_box = gtk_widget_get_parent(gtk_widget_get_parent(button));
    GtkWidget *tasks_box = gtk_widget_get_parent(task_box);
    int rc;
    int operation = move_task_params->operation;
    sqlite3 *db = move_task_params->db;
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
    sqlite3 *db = data;
    GtkWidget *task_box = gtk_widget_get_parent(gtk_widget_get_parent(button));
    GtkWidget *tasks_box = gtk_widget_get_parent(task_box);
    int rc;
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
    struct UIStates *ui_states = params->ui_states;
    if(ui_states->sent_wrong_date_alert) {
        ui_states->sent_wrong_date_alert = 0;
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
    GtkWidget *edit_button = gtk_button_new_with_label("");
    GtkWidget *name_and_important_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkStyleContext *context;
    static struct MoveTaskParams move_task_params;
    struct UIStates *ui_states = params->ui_states;

    sprintf(string_id, "%d", id);

    context = gtk_widget_get_style_context(single_task_box);
    gtk_style_context_add_class(context, "single_task_box");

    sprintf(date_string_label, "Zaplanowano na: %s", date_string);
    task_date_label=gtk_label_new(date_string_label);
    
    move_task_params.db = params->db;
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
    gtk_widget_set_name(GTK_WIDGET(edit_button), "edit_button");

    gtk_widget_set_halign(button_box, GTK_ALIGN_END);
    gtk_widget_set_valign(button_box, GTK_ALIGN_END);
    gtk_widget_set_halign(task_date_label, GTK_ALIGN_START);
    gtk_widget_set_halign(task_desc_label, GTK_ALIGN_START);
    gtk_widget_set_halign(task_name_label, GTK_ALIGN_START);
    gtk_widget_set_halign(importatnt_button, GTK_ALIGN_END);
    gtk_widget_set_halign(edit_button, GTK_ALIGN_START);
    gtk_widget_set_hexpand(task_name_label, TRUE);
    gtk_widget_set_hexpand(name_and_important_box, TRUE);


    gtk_box_append(GTK_BOX(single_task_box), name_and_important_box);
    gtk_box_append(GTK_BOX(single_task_box), task_date_label);
    gtk_box_append(GTK_BOX(single_task_box), task_desc_label);
    gtk_box_append(GTK_BOX(single_task_box), button_box);
    gtk_box_append(GTK_BOX(name_and_important_box), task_name_label);
    gtk_box_append(GTK_BOX(name_and_important_box), importatnt_button);
    gtk_box_append(GTK_BOX(button_box), edit_button);
    gtk_box_append(GTK_BOX(button_box), task_remove_button);
    gtk_box_append(GTK_BOX(button_box), task_done_button);
    gtk_box_append(GTK_BOX(tasks_box), single_task_box);  
    
    params->date_string = 0;
    ui_states->appended_inform_label = 0;
    static struct AddNewTaskParams add_params;
    add_params.tasks_box = tasks_box;
    add_params.db = params->db;
    add_params.ui_states = ui_states;
    g_signal_connect(task_remove_button, "clicked", G_CALLBACK(task_remove), params->db);
    g_signal_connect(importatnt_button, "clicked", G_CALLBACK(toggle_task_importance), params->db);
    g_signal_connect(edit_button, "clicked", G_CALLBACK(add_new_task), &add_params);
}
void data_handler(GtkWidget *button, gpointer data) {
    struct TaskDataParams *add_task_params = data; 
    GtkWidget *dialog; 
    GtkEntryBuffer *task_name_buffer = add_task_params->task_name_buffer;
    GtkEntryBuffer *task_desc_buffer = add_task_params->task_desc_buffer;
    GtkWidget *window = add_task_params->window;
    GtkWidget *tasks_box = add_task_params->tasks_box;
    GtkWidget *add_task_box = add_task_params->add_task_box;
    GtkWidget *parent_box = gtk_widget_get_parent(gtk_widget_get_parent(button));
    GtkWidget *floating_add_button = add_task_params->floating_add_button;
    GtkWidget *child = gtk_widget_get_first_child(tasks_box);
    GtkWidget *parent_box_child = gtk_widget_get_first_child(parent_box);
    const gchar *task_name = gtk_entry_buffer_get_text(task_name_buffer);
    const gchar *task_desc = gtk_entry_buffer_get_text(task_desc_buffer);
    sqlite3 *db = add_task_params->db;
    static struct AddNewTaskParams add_new_task_params;
    struct UIStates *ui_states = add_task_params->ui_states;
    /*TODO pass proper amount of bytes*/
    char *sql = malloc(10000);
    int rc = 0;
    char *err_msg;

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
        int element_id = sqlite3_last_insert_rowid(db) + 1;

        static struct CreateNewTaskBoxParams fn_params;
        fn_params.tasks_box = tasks_box;
        fn_params.task_name = task_name;
        fn_params.task_desc = task_desc;
        fn_params.finished = 0;
        fn_params.date_string = add_task_params->string_date;
        fn_params.db = db;
        fn_params.ui_states = ui_states;
        if (strchr(gtk_widget_get_name(parent_box), 'e')!=NULL) {
            char rowid[100];
            sprintf(rowid, "%s", gtk_widget_get_name(parent_box));
            /*removes first letter from char array*/
            memmove(rowid, rowid+1, strlen(rowid));

            gtk_widget_set_name(parent_box, rowid);

            gtk_box_remove(GTK_BOX(parent_box), add_task_box);
            GtkWidget *name_label = gtk_widget_get_first_child(parent_box_child);
            GtkWidget *date_label = gtk_widget_get_next_sibling(gtk_widget_get_first_child(parent_box));
            GtkWidget *desc_label = gtk_widget_get_next_sibling(date_label);
            char string_date_formated[40];
            sprintf(string_date_formated, "Zaplanowano na: %s", add_task_params->string_date);
            gtk_label_set_text(GTK_LABEL(name_label), task_name);
            gtk_label_set_text(GTK_LABEL(desc_label), task_desc);
            gtk_label_set_text(GTK_LABEL(date_label), string_date_formated);

            while(gtk_widget_get_next_sibling(parent_box_child)!=NULL){
                gtk_widget_set_visible(parent_box_child, true);
                parent_box_child = gtk_widget_get_next_sibling(parent_box_child);
            }
            gtk_widget_set_visible(gtk_widget_get_last_child(parent_box), true);
            sprintf(sql, "UPDATE tasks SET task_name = '%s', task_desc = '%s', date_string = '%s', date = '%ld' WHERE rowid = %s",\
                    task_name, task_desc, add_task_params->string_date, add_task_params->unix_datetime, rowid);
        } else {
            create_new_task_box(&fn_params, element_id);
            sprintf(sql, "INSERT INTO tasks VALUES ('%s', '%s', '%s', %lu, 0, 0);", task_name, task_desc, add_task_params->string_date, add_task_params->unix_datetime);
        }
        ui_states->is_add_task_active = 0;
        gtk_box_remove(GTK_BOX(add_task_params->right_box), add_task_box);
        gtk_widget_set_sensitive(GTK_WIDGET(tasks_box), true);

        gtk_widget_set_sensitive(child, true);
        while(gtk_widget_get_next_sibling(child)!=NULL) {
            child = gtk_widget_get_next_sibling(child);
            gtk_widget_set_sensitive(child, true);
        }

    
        rc = sqlite3_exec(db, sql, 0,0, &err_msg);
        gtk_button_set_label(GTK_BUTTON(floating_add_button), "+");
        gtk_widget_set_name(floating_add_button, "f_add_button");
        add_new_task_params.tasks_box = tasks_box;
        add_new_task_params.db  = db;
        add_new_task_params.ui_states = ui_states;
        g_signal_connect(floating_add_button, "clicked", G_CALLBACK(add_new_task), &add_new_task_params);
        free(sql);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL err, %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
        }
    }
}
void date_handler (GtkMenuButton *button, gpointer data) {
    struct HandleDate *params = data;
    static struct ResetWrongDateAlertBoxParams callback_params;
    struct UIStates *ui_states = params->ui_states;
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
        if (!ui_states->sent_wrong_date_alert) {
            gtk_box_insert_child_after(GTK_BOX(popover_box), warning_label, params->popover_sub_box);
            callback_params.popover_box = popover_box;
            callback_params.warning_label = warning_label;
            callback_params.ui_states = ui_states;
            g_signal_connect(params->popover, "closed", G_CALLBACK(rest_wrong_date_alert), &callback_params);

            ui_states->sent_wrong_date_alert = 1;
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
        
        if (!ui_states->appended_inform_label) {
            gtk_box_insert_child_after(GTK_BOX(params->add_task_box), date_box, params->desc_entry);
            params->params->date_label = date_label;
            params->params->time_label = time_label;
            ui_states->appended_inform_label = 1;
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
    GtkWidget *child = gtk_widget_get_first_child(add_task_box_parent);
    if (cancel_params->edit_mode>0) {
        while(gtk_widget_get_next_sibling(child)!=NULL){
            gtk_widget_set_visible(child, true);
            child = gtk_widget_get_next_sibling(child);
        }
        char rowid[100];
        sprintf(rowid, "%s", gtk_widget_get_name(add_task_box_parent));
        /*removes first letter from char array*/
        memmove(rowid, rowid+1, strlen(rowid));
        gtk_widget_set_name(add_task_box_parent, rowid);
        gtk_widget_set_visible(gtk_widget_get_parent(button), true);
    }
    gtk_box_remove(GTK_BOX(add_task_box_parent), add_task_box);
    gtk_widget_set_name(floating_add_button, "f_add_button");
    gtk_button_set_label(GTK_BUTTON(floating_add_button), "+");
    cancel_params->add_task_params->ui_states->is_add_task_active = 0;

    gtk_widget_set_sensitive(existing_task, true);
    while(gtk_widget_get_next_sibling(existing_task)!=NULL) {
        existing_task = gtk_widget_get_next_sibling(existing_task);
        gtk_widget_set_sensitive(existing_task, true);
    }
}
void add_new_task(GtkWidget *button, gpointer data) {
    struct AddNewTaskParams *add_new_task_params = data;
    GtkWidget *tasks_box = add_new_task_params->tasks_box;
    GtkWidget *right_box = gtk_widget_get_parent(tasks_box);
    GtkWidget *add_date_button = gtk_menu_button_new();
    GtkWidget *overlay = gtk_widget_get_parent(gtk_widget_get_parent(right_box));
    GtkWidget *window = gtk_widget_get_parent(gtk_widget_get_parent(gtk_widget_get_parent(gtk_widget_get_parent(overlay))));
    GtkWidget *floating_add_button = gtk_widget_get_next_sibling(overlay);
    GtkWidget *popover = gtk_popover_new(), *popover_box, *popover_sub_box;
    GtkWidget *calendar, *spin_button_hour, *spin_button_min, *btn;
    GtkWidget *fields_box, *task_name_entry, *task_desc_entry, *add_button, *label;
    GtkWidget *existing_task = gtk_widget_get_first_child(tasks_box);
    GtkWidget *existing_box;
    GtkEntryBuffer *task_name_buffer = gtk_entry_buffer_new(NULL, 0) , *task_desc_buffer = gtk_entry_buffer_new(NULL, 0);
    sqlite3 *db = add_new_task_params->db;
    static struct TaskDataParams params;
    static struct HandleDate date_params;
    struct UIStates *ui_states = add_new_task_params->ui_states;

    if(ui_states->is_add_task_active==1) return;

    gtk_button_set_label(GTK_BUTTON(floating_add_button), "X");
    gtk_widget_set_name(floating_add_button, "f_add_button_red");

    gtk_widget_set_sensitive(existing_task, false);
    while(gtk_widget_get_next_sibling(existing_task)!=NULL) {
        existing_task = gtk_widget_get_next_sibling(existing_task);
        gtk_widget_set_sensitive(existing_task, false);
    }
    fields_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    if(strcmp(gtk_widget_get_name(button), "edit_button")==0) {
        existing_box = gtk_widget_get_parent(gtk_widget_get_parent(button));

        char temp_name[100];
        sprintf(temp_name, "e%s", gtk_widget_get_name(existing_box));
        gtk_widget_set_name(existing_box, temp_name);
        gtk_widget_set_sensitive(existing_box, true);
        GtkWidget *child = gtk_widget_get_first_child(existing_box);
        GtkWidget *name_label = gtk_widget_get_first_child(child);
        GtkWidget *date_label = gtk_widget_get_next_sibling(gtk_widget_get_first_child(existing_box));
        GtkWidget *desc_label = gtk_widget_get_next_sibling(date_label);
        const char *task_name = gtk_label_get_text(GTK_LABEL(name_label));
        const char *task_desc = gtk_label_get_text(GTK_LABEL(desc_label));
        /* const char *task_date = gtk_label_get_text(GTK_LABEL(desc_label)); */

        while(gtk_widget_get_next_sibling(child)!=NULL){
            gtk_widget_set_visible(child, false);
            child = gtk_widget_get_next_sibling(child);
        }
        gtk_widget_set_visible(gtk_widget_get_parent(button), false);

        gtk_widget_set_visible(child, false);
        gtk_entry_buffer_set_text(task_name_buffer, task_name, strlen(task_name));
        gtk_entry_buffer_set_text(task_desc_buffer, task_desc, strlen(task_desc));

        gtk_box_append(GTK_BOX(existing_box), fields_box);

        label = gtk_label_new("Edytowanie zadania");
        add_new_task_params->ui_states->edit_mode=1;
    } else {
        label = gtk_label_new("Dodaj nowe zadanie");
    }
    gtk_widget_set_name(GTK_WIDGET(fields_box), "add_task_box");
    
    gtk_widget_set_name(GTK_WIDGET(label), "add_new_task_label");

    task_name_entry = gtk_entry_new_with_buffer(task_name_buffer);
    gtk_entry_set_placeholder_text(GTK_ENTRY(task_name_entry), "Nazwa zadania");

    task_desc_entry = gtk_entry_new_with_buffer(task_desc_buffer);
    gtk_entry_set_placeholder_text(GTK_ENTRY(task_desc_entry), "Opis zadania");
    

    add_button = gtk_button_new_with_label("Dodaj");
    params.task_name_buffer = task_name_buffer;
    params.task_desc_buffer = task_desc_buffer;
    params.window = window;
    params.tasks_box = tasks_box;
    params.add_task_box = fields_box;
    params.db = db;
    params.right_box = right_box;
    params.floating_add_button = floating_add_button;
    params.string_date = 0;
    params.unix_datetime = 0;
    params.ui_states = ui_states;

    gtk_menu_button_set_popover(GTK_MENU_BUTTON(add_date_button), GTK_WIDGET(popover));
    gtk_menu_button_set_label(GTK_MENU_BUTTON(add_date_button), "Ustaw datę");
    gtk_menu_button_set_always_show_arrow(GTK_MENU_BUTTON(add_date_button), FALSE);

    gtk_box_append(GTK_BOX(fields_box), label);
    gtk_box_append(GTK_BOX(fields_box), task_name_entry);
    gtk_box_append(GTK_BOX(fields_box), task_desc_entry);
    gtk_box_append(GTK_BOX(fields_box), add_date_button);
    gtk_box_append(GTK_BOX(fields_box), add_button);
   
    /*locks others tasks interactions while adding a new one*/


    if(strcmp(gtk_widget_get_name(button), "edit_button")!=0) gtk_box_prepend(GTK_BOX(right_box), fields_box);
    ui_states->is_add_task_active = 1;

    popover_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    calendar = gtk_calendar_new();
    label = gtk_label_new("Ustaw godzinę/minutę");
    spin_button_hour = gtk_spin_button_new_with_range(0, 24, 1);
    popover_sub_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    spin_button_min= gtk_spin_button_new_with_range(0, 60, 1);
    btn = gtk_button_new_with_label("Potwierdź");

    gtk_popover_set_has_arrow(GTK_POPOVER(popover), FALSE);
    gtk_popover_set_child(GTK_POPOVER(popover), GTK_WIDGET(popover_box));
    gtk_box_append(GTK_BOX(popover_box), calendar);
    gtk_box_append(GTK_BOX(popover_box), GTK_WIDGET(label));
    gtk_box_append(GTK_BOX(popover_sub_box), GTK_WIDGET(spin_button_hour));
    gtk_box_append(GTK_BOX(popover_sub_box), GTK_WIDGET(spin_button_min));
    gtk_box_append(GTK_BOX(popover_box), GTK_WIDGET(popover_sub_box));
    gtk_box_append(GTK_BOX(popover_box), GTK_WIDGET(btn));

    date_params.add_task_box = fields_box;
    date_params.calendar = calendar;
    date_params.hour_input = spin_button_hour;
    date_params.min_input = spin_button_min;
    date_params.popover_box = popover_box;
    date_params.popover_sub_box = popover_sub_box;
    date_params.popover = popover;
    date_params.params = &params;
    date_params.desc_entry = task_desc_entry;
    date_params.add_date_button = add_date_button;
    date_params.ui_states = ui_states;

    g_signal_connect(btn, "clicked", G_CALLBACK(date_handler), &date_params);
    g_signal_connect(add_button, "clicked", G_CALLBACK(data_handler), &params);
}
