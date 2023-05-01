#include "../inc/uchat.h"

char *changed_text;
void on_EditMessageWindow_destroy(void);

void on_userEditMessage_clicked(gpointer *data){
    t_message_panel *msg = (t_message_panel*)data;
    GtkBuilder *popka = gtk_builder_new_from_file("./client/src/main_chat.glade");
    GtkWidget *editMessageClose = GTK_WIDGET(gtk_builder_get_object(popka, "editMessageClose"));
    editMessageWindow = GTK_WIDGET(gtk_builder_get_object(popka, "editMessageWindow"));
    editMessageInput = GTK_WIDGET(gtk_builder_get_object(popka, "editMessageInput"));
    editMessageSendButton = GTK_WIDGET(gtk_builder_get_object(popka, "editMessageSendButton"));
    //log_info("TEXT: %s", msg->text);
    gtk_entry_set_text(GTK_ENTRY(editMessageInput), msg->text);
    
    t_edit_message *edit_msg = malloc(sizeof(edit_msg));
    memset(edit_msg, 0, sizeof(t_edit_message));

    edit_msg->message_id = msg->message_id;
    edit_msg->edit_time = 0;
    edit_msg->new_text = msg->text;
    g_signal_connect(editMessageInput, "changed", G_CALLBACK(on_editMessageInput_changed), editMessageWindow);
    g_signal_connect_data(editMessageSendButton, "clicked", G_CALLBACK(on_userEditMessageSendButton_clicked), edit_msg, NULL, G_CONNECT_SWAPPED);
    g_signal_connect(editMessageClose, "clicked", G_CALLBACK(on_EditMessageWindow_destroy), editMessageWindow);
    
    gtk_widget_show(editMessageWindow);
    gtk_widget_show(editMessageInput);
    gtk_widget_show(editMessageSendButton);

    gtk_widget_add_class(editMessageWindow, "window");
    gtk_widget_add_class(GTK_WIDGET(editMessageInput), "inputs");
    gtk_widget_add_class(GTK_WIDGET(editMessageSendButton), "button");
    css_set(provider, editMessageClose);
    css_set(provider, editMessageWindow);
    css_set(provider, editMessageInput);
    css_set(provider, editMessageSendButton);
    css_set(provider, errorType);
    css_set(provider, errorMessage);
}

void on_editMessageInput_changed(GtkEntry *e) {
	free(changed_text);
	changed_text = strdup(gtk_entry_get_text(e));
    log_info("%s",changed_text);
}

void on_EditMessageWindow_destroy(void){
    gtk_widget_destroy(editMessageWindow);
}

void on_userEditMessageSendButton_clicked(gpointer *data){
    t_edit_message *msg = (t_edit_message *)data;
    if (strcmp(mx_strtrim(changed_text), "") == 0){
        gtk_label_set_text(GTK_LABEL(errorMessage), "You can't send empty message.");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
        gtk_widget_destroy(editMessageWindow);
        return;
    }
    if(strcmp(changed_text, msg->new_text) == 0){
        gtk_widget_destroy(editMessageWindow);
        return;
    }
    gtk_widget_destroy(editMessageWindow);
    msg->new_text = changed_text;
    delete_edit_message = true;
    if (!mx_message_db_edit(msg)){
        gtk_label_set_text(GTK_LABEL(errorMessage), "Something went wrong >:D");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
    }
   
}

