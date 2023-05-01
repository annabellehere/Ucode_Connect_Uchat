#include "../inc/uchat.h"

void on_GoBack_clicked (GtkButton *b) {
    gtk_widget_hide(window);
    builder = gtk_builder_new_from_file ("client/src/main_chat.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    
    gtk_widget_show_all(window);

    messages_need_to_updata = true;
    chats_need_to_update = true;
    mx_mainWind();
}

