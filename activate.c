#include <gtk/gtk.h>
#include "include/structs.h"
#include "include/task_handling.h"
#include "include/task_loading.h"

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *vbox;
    GtkWidget *main_box;
    GtkWidget *side_menu;
    GtkWidget *tasks_box;
    GtkWidget *header_bar = gtk_header_bar_new();
    GtkWidget *button;
    GtkWidget *scrolled_window;
    GtkWidget *add_task_button;
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    struct DbElements *db_elements = user_data;
    static struct AddNewTaskParams params;
    static struct LoadTasksFromDbParams load_tasks_params;


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
    
    tasks_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(GTK_WIDGET(tasks_box), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(tasks_box), TRUE);
    gtk_widget_set_name(GTK_WIDGET(tasks_box), "tasks_box");
    
    params.window = window;
    params.tasks_box = tasks_box;
    params.db_elements = db_elements;
    load_tasks_params.tasks_box = tasks_box;
    load_tasks_params.db_elements = db_elements;
    
    button = gtk_button_new_with_label("Aktywne");
    gtk_box_append(GTK_BOX(side_menu), button);
    gtk_widget_set_margin_bottom(button, 10);
    g_signal_connect(button, "clicked", G_CALLBACK(load_active_tasks), &load_tasks_params);
    
    /* button = gtk_button_new_with_label("Nadchodzące"); */
    /* gtk_box_append(GTK_BOX(side_menu), button); */
    /* gtk_widget_set_margin_bottom(button, 10); */
    
    button = gtk_button_new_with_label("Archiwum");
    gtk_box_append(GTK_BOX(side_menu), button);
    gtk_widget_set_margin_bottom(button, 10);
    g_signal_connect(button, "clicked", G_CALLBACK(load_archived_tasks), &load_tasks_params);

    gtk_box_append(GTK_BOX(main_box), side_menu);

    scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(GTK_WIDGET(scrolled_window), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(scrolled_window), TRUE);
    
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), tasks_box);
    
    gtk_box_append(GTK_BOX(main_box), scrolled_window);

    g_signal_connect(add_task_button, "clicked", G_CALLBACK(add_new_task), &params);
    load_tasks_from_db(db_elements, tasks_box, "normal", 0);

    gtk_window_present (GTK_WINDOW (window));
}
