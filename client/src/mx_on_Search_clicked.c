#include "../inc/uchat.h"

void on_Search_clicked(GtkButton *b, gpointer data)
{
    bool exist = false;
    const gchar *find_log = gtk_entry_get_text(GTK_ENTRY(data));
    char *trimmed = mx_strtrim((char *)find_log);
    if (mx_strcmp(trimmed, "") == 0){
        return;
    }
    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db_client, "SELECT user_id FROM user WHERE user_login = ?;", -1, &st, NULL);
    sqlite3_bind_text(st, 1, (const char *)find_log, -1, SQLITE_STATIC);
    int result = sqlite3_step(st);
    t_db_user *found = NULL;
    if (result != SQLITE_ROW)
    {
        found = mx_ask_for_user_data_by_login((char *)find_log);
        if (found != NULL)
            exist = true;
    }
    else
    {
        found = malloc(sizeof(t_db_user));
        memset(found, 0, sizeof(t_db_user));
        found->user_id = sqlite3_column_int64(st, 0);
        exist = true;
    }
    if (exist)
    {
        t_create_chat *chat = malloc(sizeof(t_create_chat));
        memset(chat, 0, sizeof(t_create_chat));
        long *user_ids = malloc(sizeof(long) * 2);
        //log_info("TI VOOBSHE CHETO VIVODISH??2");
        user_ids[0] = user_info->user_id;
        user_ids[1] = found->user_id;
        chat->user_ids = user_ids;
        chat->chat_name = strdup("");
        chat->member_count = 2;
        chat->chat_avathar = strdup("");
        const char *req_str = mx_t_create_chat_jsonify(*chat);
        mx_begin_io_transaction();
        mx_ssl_send_package(req_str);
        const char *resp_str = mx_ssl_read_package();
        mx_end_io_transaction();
        t_create_chat *response = mx_t_create_chat_unjsonify(resp_str);
        if (response->chat_id == -1)
        {
            gtk_label_set_text(GTK_LABEL(errorMessage), "Something went wrong!");
            gtk_dialog_run(GTK_DIALOG(errorDialog));
            mx_t_create_chat_free(chat);
            mx_t_db_user_free(found);
            return;
        }
        if (response->chat_id == -2)
        {
             gtk_label_set_text(GTK_LABEL(errorMessage), "Chat with such person already exists!");
            gtk_dialog_run(GTK_DIALOG(errorDialog));
            mx_t_create_chat_free(chat);
            mx_t_db_user_free(found);
            return;
        }
        mx_t_create_chat_free(chat);
        mx_t_db_user_free(found);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(errorMessage), "User doesn't exist!");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
    }
}

void on_SendMessage_clicked (GtkButton *b, gpointer data) { // buttom for send message
    if (strcmp(mx_strtrim(message_to_send.text), "") == 0 || current_chat.chat_id < 0){
        //log_info("Not sending it");
        return;
        }
    //log_info("Sending it");
    message_to_send.author_id = user_info->user_id;
    message_to_send.message_type = e_message;
    message_to_send.chat_id = current_chat.chat_id;
    message_to_send.edit_time = 0;
    message_to_send.sendtime = time(NULL);
    message_to_send.media = strdup("");

    const char *req = mx_t_message_jsonify(message_to_send);

    mx_begin_io_transaction();
    mx_ssl_send_package(req);
    const char *resp = mx_ssl_read_package();
    mx_end_io_transaction();

    free((char *)req);
    free(message_to_send.text);
    free(message_to_send.media);
    free((char *)resp);
    messages_need_to_updata = true;
    gtk_entry_set_text(GTK_ENTRY(InputMessage), "");
    return;


}

/*void on_DelAcc_clicked(GtkButton *b)
{ // buttom for dell acc
    // dell acc
    gtk_widget_hide(window);
    builder = gtk_builder_new_from_file("client/src/login.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_widget_show_all(window);
    mx_login();
}*/

void on_ChangeName_clicked(GtkButton *b)
{ // buttom for change name
  // change name
   
    t_edit_profile ed;
    memset(&ed, 0, sizeof(t_edit_profile));
    ed.user_id = user_info->user_id;
    if (change_nick == NULL) return;
    ed.new_nick = strdup(change_nick);
    ed.new_avathar = strdup("");
    ed.new_password = strdup("");
    user_info->nickname = strdup(change_nick);
    chats_need_to_update = true;
    char *ed_json = (char *)mx_t_edit_profile_jsonify(ed);
    mx_begin_io_transaction();
    mx_ssl_send_package(ed_json);
    free(ed_json);
    const char * resp = mx_ssl_read_package();

    mx_end_io_transaction();
    t_id_response * id_resp = mx_t_id_response_unjsonify(resp);
    if (id_resp->success){
        gtk_label_set_text(GTK_LABEL(errorType),"Success");
        gtk_label_set_text(GTK_LABEL(errorMessage), "You changed your nickname!");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
    }
    mx_t_id_response_free(id_resp);
    free((char *)resp);
    free(ed.new_avathar);
    free(ed.new_password);
    free(ed.new_nick);
    gtk_entry_set_text(GTK_ENTRY(inputChangeName), "");
}

void on_ChangePassword_clicked(GtkButton *b)
{ // buttom for change password
  // change name
    gtk_entry_set_text(GTK_ENTRY(inputChangePassword), "");
    //log_info("Try changing password to %s", change_password_str);
    t_edit_profile ed;
    memset(&ed, 0, sizeof(t_edit_profile));
    ed.user_id = user_info->user_id;
    ed.new_nick = strdup("");
    ed.new_avathar = strdup("");
    if (change_password_str == NULL) return;
    ed.new_password = strdup(change_password_str);
    char *ed_json = (char *)mx_t_edit_profile_jsonify(ed);
    
    mx_begin_io_transaction();
    mx_ssl_send_package(ed_json);
    free(ed_json);
    const char *resp = mx_ssl_read_package();
    mx_end_io_transaction();
    t_id_response *rsp = mx_t_id_response_unjsonify(resp);
    if(rsp->success){
        gtk_label_set_text(GTK_LABEL(errorType),"Succes");
        gtk_label_set_text(GTK_LABEL(errorMessage), "You changed your password!");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
    }
    free((char *)resp);
    mx_t_id_response_free(rsp);
    free(ed.new_avathar);
    free(ed.new_password);
    free(ed.new_nick);
}

void on_ChangeAvatar_clicked(GtkWidget *widget, gpointer data)
{
    // change avatar
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File", NULL, action, "_Cancel",
                                         GTK_RESPONSE_CANCEL, "_Open",
                                         GTK_RESPONSE_ACCEPT, NULL);
    GtkFileFilter *filter;
    filter = gtk_file_filter_new();
    gtk_file_filter_new();
    gtk_file_filter_add_pixbuf_formats(filter);
    gtk_file_filter_set_name(filter, "images");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);                                     

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        long len = 0;
        filename = mx_compress_image(filename);
        char *buff = mx_read_full_file(filename, &len);
        char *b64 = malloc(Base64encode_len(len) + 1);
        memset(b64, 0, Base64encode_len(len) + 1);
        Base64encode(b64, buff, len);
        free(buff);

        t_edit_profile *ed = malloc(sizeof(t_edit_profile));
        memset(ed, 0, sizeof(t_edit_profile));

        ed->user_id = user_info->user_id;
        ed->new_nick = strdup("");
        ed->new_password = strdup("");
        ed->new_avathar = b64;
        user_info->avathar = b64;
        char *ed_json = (char *)mx_t_edit_profile_jsonify(*ed);

        //log_info("Began transaction");
        mx_begin_io_transaction();
        mx_ssl_send_package(ed_json);
        free(ed_json);
        const char *resp = mx_ssl_read_package();
        mx_end_io_transaction();
        //log_info("ended transaction");
        t_id_response *rsp = mx_t_id_response_unjsonify(resp);
       /* if(rsp->success){
            gtk_label_set_text(GTK_LABEL(errorType),"Success");
            gtk_label_set_text(GTK_LABEL(errorMessage), "You changed your avathar.");
            gtk_dialog_run(GTK_DIALOG(errorDialog));
        }*/
        free((char *)resp);
        mx_t_id_response_free(rsp);
        free(ed->new_avathar);
        free(ed->new_nick);
        free(ed->new_password);
        free(ed);

        gtk_image_set_from_file(GTK_IMAGE(ImageSettings), filename);
        scale_image(ImageSettings, 100, 100);
    }
    gtk_widget_destroy(dialog);
}
