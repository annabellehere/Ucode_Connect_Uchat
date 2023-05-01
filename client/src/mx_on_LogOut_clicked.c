#include "../inc/uchat.h"

void on_LogOut_clicked (GtkButton *b) {
    mx_terminate_requesting_thread();
    mx_t_db_user_free(user_info);
    //mx_close_g_thread(message_panel_thread, message_timeout);
   // mx_close_g_thread(chat_panel_thread, chat_timeout);
    user_info = NULL;
    //mx_t_free_chat_panel(chats_panel);
    //mx_t_free_message_panel(messages_panel);
   
    gtk_widget_hide(window);
    current_chat.chat_id = -1;
    chats_need_to_update = true;
    messages_need_to_updata = true;
    builder = gtk_builder_new_from_file ("client/src/login.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_widget_show_all(window);
    mx_login();
} 



