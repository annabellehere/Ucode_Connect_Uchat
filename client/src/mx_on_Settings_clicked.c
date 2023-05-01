#include "../inc/uchat.h"

void on_Settings_clicked (GtkButton *b) {
    mx_close_g_thread(chat_panel_thread, chat_timeout);
    mx_close_g_thread(message_panel_thread, message_timeout);
    gtk_widget_hide(window);
    builder = gtk_builder_new_from_file ("client/src/settings.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_widget_show_all(window);
    mx_settings();
}






