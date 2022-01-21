#include <stdio.h>
#include <sqlite3.h>
#include <gtk/gtk.h>

void first_time_launch (GtkWidget *tasks_box, int mode) {
    char label_message[200];
    GtkWidget *welcome_label;
    if (mode == 1) {
        strcpy(label_message, "Trochę tu pusto!\nAby dodać nowe zadanie do listy kliknij przycisk \"Dodaj nowe zadanie\".\nEwentualnie możesz użyć tego plusika w prawym dolnym rogu ;)");
    } else {
        strcpy(label_message, "Archiwa spłonęły w Kopenhadze ;).\nTutaj trafiają zakończone zadania");
    }
    welcome_label = gtk_label_new(label_message);
    gtk_widget_set_name(welcome_label, "welcome_msg");
    gtk_widget_set_hexpand(welcome_label, TRUE);
    gtk_widget_set_vexpand(welcome_label, TRUE);
    gtk_box_append(GTK_BOX(tasks_box), welcome_label);
}
void hide_unhide_welcome_msg (GtkWidget *tasks_box, int mode) {
    GtkWidget *welcome_lbl = gtk_widget_get_first_child(tasks_box);
    if (welcome_lbl == NULL) return;
    if (strcmp(gtk_widget_get_name(welcome_lbl), "welcome_msg" )!=0) return;
    if (mode == 1)  {
        gtk_widget_set_visible(welcome_lbl, FALSE);
    } else {
        gtk_widget_set_visible(welcome_lbl, TRUE);
    }
}

void destroy_welcome_msg (GtkWidget *tasks_box) {
    GtkWidget *welcome_lbl = gtk_widget_get_first_child(tasks_box);
    if (welcome_lbl!=NULL && strcmp(gtk_widget_get_name(welcome_lbl), "welcome_msg" )==0)
        gtk_box_remove(GTK_BOX(tasks_box), welcome_lbl);
}
