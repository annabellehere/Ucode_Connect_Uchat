#include "../inc/uchat.h"

int last_chat_id = -1;



gboolean mx_vadjust_idle(void *ptr);

gboolean show_gif_idle(gpointer data){
 /*
       GdkPixbuf *waiting = gdk_pixbuf_new_from_file("./client/src/img/waiting.gif", NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(WaitingGif), waiting);
    gtk_widget_show(WaitingGif);
    scale_image(WaitingGif, 200, 200);*/
    gtk_widget_show(WaitingGif);
    return false;
}

void on_ChatPanel_clicked (GtkButton *b, gpointer data) {
    int id = *(int *)data;
    if (current_chat.chat_id == id && !messages_need_to_updata){
        return;
    }
    else{
        mx_clean_grid(3);
        if (message_timeout != 0)
            mx_close_g_thread(message_panel_thread, message_timeout);
        g_idle_add(show_gif_idle, NULL);
        
        current_chat.chat_id = id;
        t_list *tmp =mx_get_chat_panels_to_show_by_chat_id(user_info->user_id, current_chat.chat_id);
        if (tmp != NULL)
            current_chat =*( t_chat_panel*)(tmp->data);
        //log_info("chat name: %s, chat_id: %d", current_chat.chat_shown_name, current_chat.chat_id);
        message_panel_thread = g_thread_new (NULL, (GThreadFunc)update_messages, NULL);
        

    }
}

gpointer update_messages(void){

    message_timeout = g_timeout_add(100, mx_render_messages, NULL);

    return NULL;
}

char *mx_resize_data_format(long int_time){
    long cur_time = time(NULL);
    if (labs(cur_time - int_time) > 86400){ // longer than 24 hours
        return mx_strndup(ctime(&int_time) + 4, 12); 
    }
    else{
        return mx_strndup(ctime(&int_time) + 11, 5); 
    }
}

gboolean mx_render_messages(void *ptr){

    if (messages_need_to_updata == false && last_chat_id == current_chat.chat_id){
        return G_SOURCE_CONTINUE;

    }
    mx_clean_grid(3);
      

    
    //mx_clean_grid(4);
   
    //log_error("Image: %s", current_chat.chat_avathar);
    gtk_widget_show(GTK_WIDGET(Nick));
    gtk_widget_show(GTK_WIDGET(Ava));
    gtk_widget_show(GTK_WIDGET(InputMessage));
    gtk_widget_show(GTK_WIDGET(SendMessage));
    gtk_widget_show(GTK_WIDGET(SendMessage1));
    gtk_widget_show(GTK_WIDGET(Scrolled2));
    gtk_label_set_text(GTK_LABEL(Nick), (const gchar*)current_chat.chat_shown_name);
    if (strcmp(current_chat.chat_avathar, "") != 0) {
        char *image = mx_b64_to_file(current_chat.chat_avathar);
        gtk_image_set_from_file(GTK_IMAGE(Ava), image);
        mx_delete_tmp_file(image);
    }
    else {
        gtk_image_set_from_file(GTK_IMAGE(Ava), BASIC_AVATHAR_URL);
    }

    scale_image(GTK_WIDGET(Ava), 50, 50);

     
    
    int counter = 0;
    for (t_list *i = messages_panel; i != NULL;){
        mx_t_message_panel_free(i->data);
        t_list *tmp = i->next;
        free(i);
        i = tmp;
    }
    messages_panel = mx_get_message_panels_in_chat(current_chat.chat_id, -1, -1);
    int rrow = 0;
    counter = mx_list_size(messages_panel);
    //log_info("Rendered messages");
 
    for(t_list *i = messages_panel; i != NULL; i = i->next){
        char *tmp_name = ((t_message_panel *)(i->data))->text;//mx_chat_names_shorten((t_chat_panel *)(i->data));
        long l_time = ((t_message_panel *)(i->data))->sendtime;
        char *tmp_time = mx_resize_data_format(l_time);
        
        gtk_grid_insert_row(GTK_GRID(mainGrid), rrow);
        GtkBuilder *tmpbuilder = gtk_builder_new_from_file("./client/src/main_chat.glade");
        if (((t_message_panel *)(i->data))->author_id == user_info->user_id) {
            
            if (((t_message_panel *)(i->data))->message_type == photo){
                userMessage[rrow] = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "userMessageWithImage"));
                GtkWidget *userMessageImage = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "userMessageImage"));
                GtkWidget *UserMessageWithImageTime = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "UserMessageWithImageTime"));
                userDeleteMessage[rrow] = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "userDeleteMessageImage"));
                css_set(provider, userDeleteMessage[rrow]);
                g_signal_connect_data(userDeleteMessage[rrow], "clicked", G_CALLBACK(on_userDeleteMessage_clicked),(i->data), NULL , G_CONNECT_SWAPPED);
            
                gtk_label_set_text(GTK_LABEL(UserMessageWithImageTime), (const gchar*)tmp_time);
                char * image_path = mx_b64_to_file(((t_message_panel *)(i->data))->media);

                gtk_image_set_from_file(GTK_IMAGE(userMessageImage), image_path);
                mx_delete_tmp_file(image_path);
               // gtk_image_set_max_width(GTK_IMAGE(userMessageImage), 460);
                GdkPixbuf *pb = gtk_image_get_pixbuf(GTK_IMAGE(userMessageImage));
                int h = gdk_pixbuf_get_height(pb);
                int w = gdk_pixbuf_get_width(pb);
                if (w > 400 || h > 400){    
                    scale_image(userMessageImage, 400, 400);
                }
                label[rrow] = userMessage[rrow];
                gtk_widget_add_class(label[rrow], "message");
                css_set(provider, label[rrow]);
               
            }
            else{
                userMessage[rrow] = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "userMessage"));
                
                GtkWidget *userMessageText = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "userMessageText"));
                GtkWidget *userMessageTime = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "userMessageTime"));
                userDeleteMessage[rrow] = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "userDeleteMessage"));
                userEditMessage[rrow] = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "userEditMessage"));
                gtk_label_set_text(GTK_LABEL(userMessageText), (const gchar*)tmp_name);
                gtk_label_set_text(GTK_LABEL(userMessageTime), (const gchar*)tmp_time);

                label[rrow] = userMessage[rrow];
                gtk_widget_add_class(label[rrow], "message");
                css_set(provider, label[rrow]);
                css_set(provider, userEditMessage[rrow]);
                css_set(provider, userDeleteMessage[rrow]);
                g_signal_connect_data(userEditMessage[rrow], "clicked", G_CALLBACK(on_userEditMessage_clicked), (i->data), NULL , G_CONNECT_SWAPPED);
                g_signal_connect_data(userDeleteMessage[rrow], "clicked", G_CALLBACK(on_userDeleteMessage_clicked),(i->data), NULL , G_CONNECT_SWAPPED);
            }
            gtk_grid_attach(GTK_GRID(mainGrid), label[rrow], 1, counter, 1, 1);
            
        }
        else{
            if (((t_message_panel *)(i->data))->message_type == photo){
                foreignMessage[rrow] = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "foreignMessageWithImage"));
                messageInfo[rrow] =  GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "messageInfo"));
                GtkWidget *messageNicknameImage = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "messageNicknameImage"));
                GtkWidget *messageAvatharImage = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "messageAvatharImage"));


                GtkWidget *messageImage = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "messageImage"));
                GtkWidget *messageTimeImage = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "messageTimeImage"));

                gtk_label_set_text(GTK_LABEL(messageTimeImage), (const gchar*)tmp_time);
                gtk_label_set_text(GTK_LABEL(messageNicknameImage), (const gchar*)((t_message_panel *)(i->data))->author_nick);
                char * image_path = mx_b64_to_file(((t_message_panel *)(i->data))->media);
               
               
                gtk_image_set_from_file(GTK_IMAGE(messageImage), image_path);
                mx_delete_tmp_file(image_path);


                GdkPixbuf *pb = gtk_image_get_pixbuf(GTK_IMAGE(messageImage));
                int h = gdk_pixbuf_get_height(pb);
                int w = gdk_pixbuf_get_width(pb);
                if (w > 400 || h > 400){    
                 scale_image(messageImage, 400, 400);
                }

                long auth_id = ((t_message_panel *)(i->data))->author_id;
                char *auth_ava = mx_get_user_avathar_by_id_client(auth_id);
            
                if (strcmp(auth_ava, "") == 0){
                    gtk_image_set_from_file(GTK_IMAGE(messageAvatharImage), "client/src/img/basic_avathar.jpeg");
                }
                else {
                    char *ava_f = mx_b64_to_file(auth_ava);
                    gtk_image_set_from_file(GTK_IMAGE(messageAvatharImage), ava_f);
                    mx_delete_tmp_file(ava_f);
                }
                
                scale_image(messageAvatharImage, 50, 50);

            }
            else{
                foreignMessage[rrow] = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "foreignMessage"));
                messageInfo[rrow] =  GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "messageInfo"));
                messageNickname = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "messageNickname"));
                GtkWidget *messageText = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "messageText"));
                GtkWidget *messageTime = GTK_WIDGET(gtk_builder_get_object(tmpbuilder, "messageTime"));
               
                gtk_label_set_text(GTK_LABEL(messageText), (const gchar*)tmp_name);
                gtk_label_set_text(GTK_LABEL(messageTime), (const gchar*)tmp_time);
                gtk_label_set_text(GTK_LABEL(messageNickname), (const gchar*)((t_message_panel *)(i->data))->author_nick);


            /* gtk_grid_attach(GTK_GRID(foreignMessage[rrow]),GTK_WIDGET(messageInfo[rrow]), 1, 1, 1, 1);
                */
                GtkWidget *image = NULL;
                long auth_id = ((t_message_panel *)(i->data))->author_id;
                char *auth_ava = mx_get_user_avathar_by_id_client(auth_id);
                if (strcmp(auth_ava, "") == 0){
                    image = gtk_image_new_from_file("client/src/img/basic_avathar.jpeg");
                }
                else {
                    char *ava_f = mx_b64_to_file(auth_ava);
                    image = gtk_image_new_from_file(ava_f);
                    mx_delete_tmp_file(ava_f);
                }
                
                
                scale_image(image, 50, 50);
                gtk_grid_attach(GTK_GRID(foreignMessage[rrow]), image, 0 ,1 ,1 , 1);
               

                //gtk_widget_set_size_request(label[rrow], MESSAGE_WIDTH/2 + 8, 25);
                //css_set(provider, label[rrow]);
            }
            label[rrow] = foreignMessage[rrow];
            gtk_widget_add_class(label[rrow], "message");
            css_set(provider, label[rrow]);
            gtk_grid_attach(GTK_GRID(mainGrid), label[rrow], 0, counter, 1, 1);
        }
        
        rrow++;
        counter--;
      //  
        //free(tmp_name);

    }
    //log_info("ENDED RENDER");
    gtk_widget_hide(WaitingGif);
    gtk_widget_show_all(window);
   /* GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(Scrolled2));
   // GtkAdjustment *adj = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(Viewport2));
    //log_info("%f",gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));
   // gtk_adjustment_set_value(adj,  gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));
    gtk_adjustment_set_value(adj, gtk_widget_get_allocated_height(GTK_WIDGET(mainGrid)) - 498);
    //log_info("HEIGHT %d", );
    gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(Scrolled2), adj);*/
    messages_need_to_updata = false;
    if (!delete_edit_message){
        g_idle_add(mx_vadjust_idle, NULL);
        
    }
    else{
        delete_edit_message = false;
    }
    last_chat_id = current_chat.chat_id;
    return G_SOURCE_CONTINUE;
}

gboolean mx_vadjust_idle(void *ptr) {
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(Scrolled2));
    gtk_adjustment_set_value(adj,  gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));
    return false;
}

void mx_t_free_chat_panel(t_list *panel){
    if (panel != NULL){
        for (t_list *i = chats_panel; i->next != NULL;){
            mx_t_chat_panel_free(i->data);
            t_list *tmp = i->next;
            free(i);
            i = tmp;
        }
    }
}

void mx_t_free_message_panel(t_list *panel){
    if(panel != NULL){
        for(t_list *i = panel; i->next != NULL;){
                t_message_panel *tmp = (t_message_panel*)(i->data);
                mx_t_message_panel_free(tmp);
                t_list *buff = i->next;
                free(i);
                i = buff;
            }
    }
}

