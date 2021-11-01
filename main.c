#include <stdio.h>
#include <gtk/gtk.h>
#include <time.h>
/* #include "include/connect_db.h" */
/* #include "include/add_data.h" */
#include "include/structs.h"

bool is_add_task_active = false;
void get_data(GtkButton *button, gpointer data) {
    struct TaskDataParams *params = data; 
    const gchar *task_name = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(params->task_name_buffer));
    printf("%s\n", task_name);
}
void create_new_task_box(const struct CreateNewTaskBoxParams *params) {
    GtkWidget *tasks_box = params->tasks_box;
    const char *task_name = params->task_name;
    const char *task_desc = params->task_desc;
    const char *date_string = params->date_string;

    GtkWidget *single_task_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(GTK_WIDGET(single_task_box), "single_task_box");

    GtkWidget *task_name_label = gtk_label_new(task_name);
    GtkWidget *task_date_label = gtk_label_new(date_string);
    GtkWidget *task_desc_label = gtk_label_new(task_desc);
    GtkWidget *task_done_button = gtk_button_new_with_label("Ukończone");
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
        static struct CreateNewTaskBoxParams fn_params;
        fn_params.tasks_box = tasks_box;
        fn_params.task_name = task_name;
        fn_params.task_desc = task_desc;
        fn_params.date_string = "30/10/2021";

        create_new_task_box(&fn_params);
        is_add_task_active = false;
        gtk_box_remove(GTK_BOX(tasks_box), add_task_box);
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
    GDateTime *date_now = g_date_time_new_now(time_zone);
    int day_now = g_date_time_get_day_of_month(date_now);
    int month_now = g_date_time_get_month(date_now);
    printf("%d %d\n", day_now, month_now);
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
        
        gtk_menu_button_set_popover(GTK_MENU_BUTTON(add_date_button), GTK_WIDGET(popover));
        gtk_menu_button_set_label(GTK_MENU_BUTTON(add_date_button), "Ustaw datę");
        gtk_menu_button_set_always_show_arrow(GTK_MENU_BUTTON(add_date_button), FALSE);

        g_signal_connect(add_button, "clicked", G_CALLBACK(data_handler), &params);
        

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
        g_signal_connect(button, "clicked", G_CALLBACK(date_handler), &date_params);
    }

}


static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *vbox;
    GtkWidget *main_box;
    GtkWidget *side_menu;
    GtkWidget *tasks_box;
    GtkWidget *task_box;
    GtkWidget *header_bar = gtk_header_bar_new();
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *scrolled_window;
    GtkWidget *add_task_button;
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();


    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "style.css");
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    gtk_window_set_title(GTK_WINDOW(window), "TODO APP");

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header_bar), true);
    gtk_widget_set_name(GTK_WIDGET(header_bar), "header");
    gtk_box_append(GTK_BOX(vbox), header_bar);
    add_task_button = gtk_button_new_with_label("Dodaj nowe zadanie");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), add_task_button);

    main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_append(GTK_BOX(vbox), main_box);
    
    side_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(side_menu, 200, -1);
    gtk_widget_set_name(GTK_WIDGET(side_menu), "side_menu");
    
    button = gtk_button_new_with_label("Dzisiaj");
    gtk_box_append(GTK_BOX(side_menu), button);
    gtk_widget_set_margin_bottom(button, 10);
    
    button = gtk_button_new_with_label("Nadchodzące");
    gtk_box_append(GTK_BOX(side_menu), button);
    gtk_widget_set_margin_bottom(button, 10);
    
    button = gtk_button_new_with_label("Archiwum");
    gtk_box_append(GTK_BOX(side_menu), button);
    gtk_widget_set_margin_bottom(button, 10);

    gtk_box_append(GTK_BOX(main_box), side_menu);

    scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(GTK_WIDGET(scrolled_window), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(scrolled_window), TRUE);
    
    tasks_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(GTK_WIDGET(tasks_box), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(tasks_box), TRUE);
    gtk_widget_set_name(GTK_WIDGET(tasks_box), "tasks_box");
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), tasks_box);
    
    gtk_box_append(GTK_BOX(main_box), scrolled_window);
    static struct AddNewTaskParams params;
    params.window = window;
    params.tasks_box = tasks_box;


    
    g_signal_connect(add_task_button, "clicked", G_CALLBACK(add_new_task), &params);
    

    gtk_window_present (GTK_WINDOW (window));
}

int main(int argc, char **argv) {
    /* mongoc_collection_t *collection; */
    /* mongoc_client_t *client; */
    /* client = db_connect(); */
    /* collection = mongoc_client_get_collection(client, "gtk-todo", "tasks"); */

    /* add_data(collection); */
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);


    return status;
}
