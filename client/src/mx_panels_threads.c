#include "../inc/uchat.h"


bool is_first = 0;
bool chat_pannels_running;
void mx_start_chat_message_threading(void){
    if (chat_pannels_running == false) 
        chat_panel_thread = g_thread_new(NULL, (GThreadFunc)update_chats, NULL);

    chat_pannels_running = true;
}

gpointer update_chats(void){
    mx_chat_updating(NULL);
    chat_timeout = g_timeout_add(100, mx_chat_updating, NULL);
    return NULL;
}

void mx_close_g_thread(GThread *th, guint timeout){
    chat_pannels_running = false;
    if (timeout > 0){
    if (!g_source_is_destroyed(g_main_context_find_source_by_id(NULL, timeout))){
        if(g_source_get_id(g_main_context_find_source_by_id(NULL, timeout)) == timeout){
            g_source_remove(timeout);
        }
    }
    }
    if(th){
   // g_thread_join(th);
    g_thread_unref(th);
    }

}

gboolean mx_chat_updating(void *ptr){
    if (chats_need_to_update == false){
        return G_SOURCE_CONTINUE;
    }
    chats_need_to_update = false;
    mx_clean_grid(1);
    //mx_clean_grid(2);
    gdk_threads_enter();

    //mx_t_free_message_panel(messages_panel);
    mx_t_free_chat_panel(chats_panel);
    chats_panel = mx_get_chat_panels_to_show(user_info->user_id);
    //log_info("List size %d",mx_list_size(chats_panel));
    //gtk_grid_insert_row(GTK_GRID(Grid2), 0);
  
    gtk_label_set_text(GTK_LABEL(labelName), user_info->nickname); // PUT HERE NAME OF USER


    css_set(provider, GTK_WIDGET(labelName));
    gtk_widget_show(labelName);
    
    //gtk_grid_attach(GTK_GRID(Grid2), labelName, 1, 0, 1, 1);
    int row = 0;
    for(t_list *i = chats_panel; i != NULL; i = i->next) {
        char *tmp_name = mx_chat_names_shorten((t_chat_panel *)(i->data));
        gtk_grid_insert_row(GTK_GRID(Grid), row);
        button[row] = gtk_button_new_with_label(tmp_name);
        gtk_widget_set_size_request(button[row], 250, 25);
        css_set(provider, button[row]);
        gtk_button_set_alignment(GTK_BUTTON(button[row]), 0.0, 0.5);
        gtk_grid_attach(GTK_GRID(Grid), button[row], 1, row, 1, 1);
        int *row_itter = (int *)malloc(sizeof(int));
        (*row_itter) = ((t_chat_panel *)(i->data))->chat_id;
        g_signal_connect_data(button[row], "clicked", G_CALLBACK(on_ChatPanel_clicked), row_itter, NULL, 0);
        row++;
            //free(tmp_name);
    }
        //mx_printstr("YES\n");
    gtk_widget_show_all(window);
       //mx_printstr("Pokazal\n");
        
    gdk_flush();
    gdk_threads_leave();
       
       // }
    return G_SOURCE_CONTINUE;
}

void mx_clean_grid(int number_of_grid){
    // 1 is for grid; 2 -grid2; 3-grid3; 4 -info
    GList *children, *i;
    switch(number_of_grid){
        case 1:
            children = gtk_container_get_children(GTK_CONTAINER(Grid));
            break;
        case 2:
            children = gtk_container_get_children(GTK_CONTAINER(Grid2));
            break;
        case 3:
            children = gtk_container_get_children(GTK_CONTAINER(mainGrid));
            break;
        case 4:
            children = gtk_container_get_children(GTK_CONTAINER(info));
        default:
            return;
    }
        for (i = children; i != NULL; i = g_list_next(i))
            gtk_widget_destroy(GTK_WIDGET(i->data));
        g_list_free(children);
}

