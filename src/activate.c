#include <gtk/gtk.h>
#include "include/structs.h"
#include "include/task_handling.h"
#include "include/task_loading.h"
#include "include/utils_h/first_time_launch.h"

void handle_floating_button (GtkWidget *button, gpointer data) {
    struct AddNewTaskParams *a = data;
    struct CancelAddingNewTaskParams cancel_params;
    GtkWidget *tmp_c = gtk_widget_get_first_child(a->tasks_box);
    const char *name;

    if (a->ui_states->is_add_task_active == 0 ) {
        add_new_task(button, a);
    } else {
        cancel_params.add_task_params = a;
        cancel_params.tasks_box = a->tasks_box;
        cancel_params.edit_mode = a->ui_states->edit_mode;
        cancel_params.floating_add_button = button;
        cancel_params.ui_states = a->ui_states;
        
        printf("EDIT MODE %d\n", a->ui_states->edit_mode);
        if (a->ui_states->edit_mode == 1) {
            printf("Hello from handle_floating_button (editing)");
            exit(1);
            do {
                name = gtk_widget_get_name(tmp_c);
                if(name[0]=='e') {
                    gtk_box_append(GTK_BOX(tmp_c), tmp_c);
                    cancel_params.add_task_box = gtk_widget_get_last_child(tmp_c);
                    break;
                }
            } while((tmp_c = gtk_widget_get_next_sibling(tmp_c))!=NULL);
            cancel_adding_new_task(button, &cancel_params);
            a->ui_states->edit_mode = 0;
        } else {
            printf("Hello from handle_floating_button (not editing)");
            cancel_params.add_task_box = gtk_widget_get_first_child(gtk_widget_get_parent(a->tasks_box));
            cancel_adding_new_task(button, &cancel_params);
        }
    }
}
void activate(GtkApplication *app, gpointer user_data) {
    struct ActivateParams *activate_params = user_data;
    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *vbox, *main_box, *side_menu, *tasks_box, *button, *scrolled_window, *add_task_button, *right_box;
    GtkWidget *header_bar = gtk_header_bar_new();
    GtkWidget *overlay = gtk_overlay_new();
    GtkWidget *floating_add_button = gtk_button_new_with_label("+");
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    sqlite3 *db = activate_params->db;
    struct UIStates *ui_states = activate_params->ui_states;

    static struct AddNewTaskParams add_new_task_parms;
    static struct LoadTasksFromDbParams load_tasks_params;
    
    gtk_widget_set_name(floating_add_button, "f_add_button");
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "./style.css");
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    gtk_window_set_title(GTK_WINDOW(window), "TODO APP");

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header_bar), true);
    gtk_widget_set_name(GTK_WIDGET(header_bar), "header");
    gtk_box_append(GTK_BOX(vbox), header_bar); add_task_button = gtk_button_new_with_label("Dodaj nowe zadanie");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), add_task_button);

    main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_append(GTK_BOX(vbox), main_box);
    
    side_menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(side_menu, 200, -1);
    gtk_widget_set_name(GTK_WIDGET(side_menu), "side_menu");
   
    right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(GTK_WIDGET(right_box), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(right_box), TRUE);
    gtk_widget_set_name(GTK_WIDGET(right_box), "right_box");

    tasks_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(GTK_WIDGET(tasks_box), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(tasks_box), TRUE);
    gtk_widget_set_name(GTK_WIDGET(tasks_box), "tasks_box");
    
    add_new_task_parms.tasks_box = tasks_box;
    add_new_task_parms.db = db;
    add_new_task_parms.ui_states = ui_states;
    load_tasks_params.tasks_box = tasks_box;
    load_tasks_params.db = db;
    load_tasks_params.ui_states = ui_states;
    
    button = gtk_button_new_with_label("Aktywne");
    gtk_box_append(GTK_BOX(side_menu), button);
    gtk_widget_set_margin_bottom(button, 10);
    g_signal_connect(button, "clicked", G_CALLBACK(load_active_tasks), &load_tasks_params);

    button = gtk_button_new_with_label("Archiwum");
    gtk_box_append(GTK_BOX(side_menu), button);
    gtk_widget_set_margin_bottom(button, 10);
    g_signal_connect(button, "clicked", G_CALLBACK(load_archived_tasks), &load_tasks_params);

    gtk_box_append(GTK_BOX(main_box), side_menu);

    scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(GTK_WIDGET(scrolled_window), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(scrolled_window), TRUE);
    
    gtk_box_append(GTK_BOX(main_box), overlay);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), scrolled_window);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), right_box);

    gtk_box_append(GTK_BOX(right_box), tasks_box);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), floating_add_button);
    gtk_widget_set_halign(floating_add_button, GTK_ALIGN_END);
    gtk_widget_set_valign(floating_add_button, GTK_ALIGN_END);
    gtk_widget_set_margin_bottom(floating_add_button, 20);
    gtk_widget_set_margin_end(floating_add_button, 20);

    g_signal_connect(add_task_button, "clicked", G_CALLBACK(add_new_task), &add_new_task_parms);
    g_signal_connect(floating_add_button, "clicked", G_CALLBACK(handle_floating_button), &add_new_task_parms);
    load_tasks_from_db(db, tasks_box, "normal", 0, ui_states);

    gtk_window_present (GTK_WINDOW (window));
}
