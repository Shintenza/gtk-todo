#ifndef FIRST_TIME_LAUNCH_H
#define FIRST_TIME_LAUNCH_H 
#include <gtk/gtk.h>
int first_time_launch (GtkWidget *tasks_box, int mode);
void hide_unhide_welcome_msg (GtkWidget *tasks_box, int mode);
void destroy_welcome_msg (GtkWidget *tasks_box);
#endif
