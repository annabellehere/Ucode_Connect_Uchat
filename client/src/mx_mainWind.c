#include "../inc/uchat.h"


t_create_chat new_gchat;
void on_scrollDown_clicked(GtkButton *b);

char *mx_chat_names_shorten(t_chat_panel *chat_panel){
    int name_len = strlen(chat_panel->chat_shown_name);
    if (name_len >= MAX_CHAT_VISIBLE_NAME){
        char *shorten_name = mx_strnew(MAX_CHAT_VISIBLE_NAME);
        strncpy(shorten_name, chat_panel->chat_shown_name, MAX_CHAT_VISIBLE_NAME - 3);
        for (int i = 0; i < 3; i++){
            shorten_name[MAX_CHAT_VISIBLE_NAME - 3 + i] = '.';
        }
        //log_error("DA");
        return shorten_name;
    }
    //log_error("NET");
    return chat_panel->chat_shown_name;
}


int mx_get_usr_id_from_username(char *username) {
    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db_client, "SELECT user_id FROM user WHERE user_nick = ?;", -1, &st, NULL);
    sqlite3_bind_text(st, 1, username, -1, SQLITE_STATIC);
    int res = 0;
    if ((res = sqlite3_step(st)) == SQLITE_DONE){
        int id = sqlite3_column_int64(st,0);
        sqlite3_finalize(st);
        return id;
    }
    else{
        sqlite3_finalize(st);
        return -1;
    }
}

void mx_mainWind(void) {
    fixed = GTK_WIDGET( gtk_builder_get_object(builder, "fixed"));
    fixed2 = GTK_WIDGET( gtk_builder_get_object(builder, "fixed2"));
	Scrolled = GTK_WIDGET( gtk_builder_get_object(builder, "Scrolled"));
    Scrolled2 = GTK_WIDGET( gtk_builder_get_object(builder, "Scrolled2"));
	Viewport = GTK_WIDGET( gtk_builder_get_object(builder, "Viewport"));
    Viewport2 = GTK_WIDGET( gtk_builder_get_object(builder, "Viewport2"));

    labelName = GTK_WIDGET(gtk_builder_get_object(builder, "labelName")); 

    scrollDown = GTK_WIDGET(gtk_builder_get_object(builder, "scrollDown"));
    Search = GTK_WIDGET( gtk_builder_get_object(builder, "Search"));
    InputMessage =GTK_WIDGET( gtk_builder_get_object(builder, "InputMessage"));
    SendMessage = GTK_WIDGET( gtk_builder_get_object(builder, "SendMessage"));
    SendMessage1 = GTK_WIDGET( gtk_builder_get_object(builder, "SendMessage1"));
    Settings = GTK_WIDGET( gtk_builder_get_object(builder, "Settings"));
    inputFriend = GTK_WIDGET( gtk_builder_get_object(builder, "inputFriend"));
    info = GTK_WIDGET( gtk_builder_get_object(builder, "info"));
    Ava = GTK_WIDGET( gtk_builder_get_object(builder, "Ava"));
    Nick = GTK_WIDGET( gtk_builder_get_object(builder, "Nick"));
    GroupChat = GTK_WIDGET( gtk_builder_get_object(builder, "GroupChat"));
    scrollDown = GTK_WIDGET(gtk_builder_get_object(builder, "ScrollDown"));
    WaitingGif = GTK_WIDGET(gtk_builder_get_object(builder, "WaitingGif"));
	/*gtk_widget_add_class(GTK_WIDGET(inputPassword), "inputs");
	gtk_widget_add_class(GTK_WIDGET(inputUsername), "inputs");
	gtk_widget_add_class(GTK_WIDGET(LogIn), "button");
	gtk_widget_add_class(GTK_WIDGET(Register), "button");
	gtk_widget_add_class(GTK_WIDGET(labelUsername), "label");
	gtk_widget_add_class(GTK_WIDGET(labelPassword), "label");
	*/gtk_widget_add_class(window, "window-main");
    gtk_widget_add_class(GTK_WIDGET(InputMessage), "input-message");
    gtk_widget_add_class(GTK_WIDGET(SendMessage), "button");
    gtk_widget_add_class(GTK_WIDGET(SendMessage1), "button");
    gtk_widget_add_class(GTK_WIDGET(Settings), "button");
    Grid = GTK_WIDGET( gtk_builder_get_object(builder, "Grid"));
    Grid2 = GTK_WIDGET( gtk_builder_get_object(builder, "Grid2"));
    mainGrid = GTK_WIDGET( gtk_builder_get_object(builder, "mainGrid"));
    gtk_widget_add_class(GTK_WIDGET(inputFriend), "inputs");
    gtk_widget_add_class(GTK_WIDGET(Search), "button");
    gtk_widget_add_class(GTK_WIDGET(Nick), "label");
    gtk_widget_add_class(GTK_WIDGET(GroupChat), "button");

    css_set(provider, GroupChat);
    css_set(provider, Search);
    css_set(provider, inputFriend);
	css_set(provider, window);
    css_set(provider, SendMessage);
    css_set(provider, SendMessage1);
    css_set(provider, InputMessage);
    css_set(provider, Settings);
    css_set(provider, Nick);
    gtk_widget_hide(Ava);
    gtk_widget_hide(Nick);
    gtk_widget_hide(InputMessage);
    gtk_widget_hide(SendMessage);
    gtk_widget_hide(SendMessage1);
    gtk_widget_hide(Scrolled2);
	/*css_set(provider, inputPassword);
	css_set(provider, inputUsername);
	css_set(provider, LogIn);
	css_set(provider, Register);
	css_set(provider, labelUsername);
	css_set(provider, labelPassword);
*/
    //int list_size = mx_list_size(names);
    //GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    //guint timeout_id = g_timeout_add(2000, mx_chat_updating, NULL);
    //g_main_loop_run(loop);
    //g_source_remove(timeout_id);
    g_signal_connect(SendMessage1, "clicked", G_CALLBACK(on_SendMessage1_clicked), NULL);
    g_signal_connect(Settings, "clicked", G_CALLBACK(on_Settings_clicked), NULL);
    g_signal_connect(SendMessage, "clicked", G_CALLBACK(on_SendMessage_clicked), InputMessage);
    g_signal_connect(InputMessage, "changed", G_CALLBACK(on_InputMessage_changed), NULL);
    g_signal_connect(inputFriend, "changed", G_CALLBACK(on_inputFriend_changed), NULL);
    g_signal_connect(Search, "clicked", G_CALLBACK(on_Search_clicked), inputFriend);
    //g_signal_connect(Search, "clicked", G_CALLBACK(on_Block_clicked), Block);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(GroupChat, "clicked", G_CALLBACK(on_GroupChat_clicked), inputFriend);
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), SendMessage);
    GdkPixbufAnimation *Waiting = gdk_pixbuf_animation_new_from_file("./client/src/img/smaller_waiting.gif", NULL);
   // GdkPixBufAnimation *Waiting_scaled = gdk_pixbuf_animation_scale(WaitingGif, gdk_pixbuf_get_width(Waiting) * 0.5, gdk_pixbuf_get_height(Waiting) * 0.5, true);
    gtk_image_set_from_animation(GTK_IMAGE(WaitingGif), Waiting );
   
  
    mx_start_chat_message_threading();


}
void on_scrollDown_clicked(GtkButton *b){
    messages_need_to_updata = true;
}

void on_GroupChat_clicked(GtkButton *b) {
    //gtk_widget_hide(window);
    builder1 = gtk_builder_new_from_file ("client/src/group.glade");
    window1 = GTK_WIDGET(gtk_builder_get_object(builder1, "window1"));
    g_signal_connect(window1, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window1);
    mx_init_new_gchat();
    mx_reset_new_gchat();
    mx_group();
}

void mx_group(void) {
    fix = GTK_WIDGET( gtk_builder_get_object(builder1, "fix"));
    scroll = GTK_WIDGET( gtk_builder_get_object(builder1, "scroll"));
	viewport = GTK_WIDGET( gtk_builder_get_object(builder1, "viewport"));
    grid = GTK_WIDGET( gtk_builder_get_object(builder1, "grid"));
    addMan = GTK_WIDGET( gtk_builder_get_object(builder1, "addMan"));
	inputName = GTK_WIDGET( gtk_builder_get_object(builder1, "inputName"));
    CreateGroup = GTK_WIDGET( gtk_builder_get_object(builder1, "CreateGroup"));
    GroupName = GTK_WIDGET( gtk_builder_get_object(builder1, "GroupName"));
    LabelGroup = GTK_WIDGET( gtk_builder_get_object(builder1, "LabelGroup"));
    ChangeAvaGroup = GTK_WIDGET( gtk_builder_get_object(builder1, "ChangeAvaGroup"));
    ImageSettingGroup = GTK_WIDGET( gtk_builder_get_object(builder1, "ImageSettingGroup"));

    gtk_widget_add_class(window1, "window-group");
    gtk_widget_add_class(GTK_WIDGET(CreateGroup), "button")
    ;
    gtk_widget_add_class(GTK_WIDGET(inputName), "inputs");
    gtk_widget_add_class(GTK_WIDGET(addMan), "button");
    gtk_widget_add_class(GTK_WIDGET(ChangeAvaGroup), "button");
    gtk_widget_add_class(GTK_WIDGET(GroupName), "inputs");
    gtk_widget_add_class(GTK_WIDGET(LabelGroup), "label");

    css_set(provider, CreateGroup);
    css_set(provider, inputName);
    css_set(provider, addMan);
    css_set(provider, window1);
    css_set(provider, ChangeAvaGroup);
    css_set(provider, GroupName);
    css_set(provider, LabelGroup);

    g_signal_connect(CreateGroup, "clicked", G_CALLBACK(on_CreateGroup_clicked), NULL);
    g_signal_connect(addMan, "clicked", G_CALLBACK(on_addMan_clicked), NULL);
    g_signal_connect(ChangeAvaGroup, "clicked", G_CALLBACK(on_ChangeAvaGroup_clicked), NULL);
    g_signal_connect(GroupName, "changed", G_CALLBACK(on_GroupName_changed), NULL);
}

void on_ChangeAvaGroup_clicked(GtkButton *b) {
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
  if (res == GTK_RESPONSE_ACCEPT) {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    long len = 0;
    //char *tmp = strdup(filename);
    filename = mx_compress_image(filename);
    char *bytebuf = mx_read_full_file(filename, &len);
    binaryb64 *b64buf = malloc(Base64encode_len(len) + 1);
    b64buf[Base64encode_len(len) + 1] = 0;
    Base64encode(b64buf, bytebuf, len);
    free(bytebuf);
    mx_init_new_gchat();
    gtk_image_set_from_file(GTK_IMAGE(ImageSettingGroup), filename);
    scale_image(ImageSettingGroup, 100, 100);
    free(new_gchat.chat_avathar);
    new_gchat.chat_avathar = b64buf;

    //gtk_image_set_from_file(GTK_IMAGE(imageReg), filename);
    //scale_image(imageReg, 50, 50);
    mx_delete_tmp_file(filename);
  }
  gtk_widget_destroy(dialog);
    
}

void on_GroupName_changed(GtkEntry *e) {
    mx_init_new_gchat();
    free(new_gchat.chat_name);
    new_gchat.chat_name = strdup(gtk_entry_get_text(e));
}

void on_CreateGroup_clicked(GtkButton *b) {
    gtk_window_close(GTK_WINDOW(window1));

    //new_gchat.chat_avathar = strdup("");
    //new_gchat.chat_name = strdup("SomeGroup");
    new_gchat.creation_time = time(NULL);

    bool is_already_in = false;
    for(long i = 0; i < new_gchat.member_count; i++) {
        if (new_gchat.user_ids[i] == user_info->user_id) is_already_in = true;
    }

    if (!is_already_in) {
        mx_init_new_gchat();
        new_gchat.member_count++;
        new_gchat.user_ids = realloc(new_gchat.user_ids, sizeof(long) * new_gchat.member_count);
        new_gchat.user_ids[new_gchat.member_count - 1] = user_info->user_id;
    }
    
    char *create_gchat_json = (char *)mx_t_create_chat_jsonify(new_gchat);

    mx_begin_io_transaction();
    mx_ssl_send_package(create_gchat_json);
    free(create_gchat_json);
    char *resp = mx_ssl_read_package();
    mx_end_io_transaction();

    t_create_chat *create_resp = mx_t_create_chat_unjsonify(resp);
    if (create_resp->chat_id > 0){
        //log_info("Created group chat %i, %s, %i", create_resp->chat_id, create_resp->chat_name, create_resp->member_count);
        mx_chats_inserter(create_resp);
    }
    chats_need_to_update = true;

    mx_t_create_chat_free(create_resp);

    mx_reset_new_gchat();



}




void on_SendMessage1_clicked(GtkWidget *widget, gpointer data){
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
  if (res == GTK_RESPONSE_ACCEPT) {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    long len = 0;
    char *tmp = strdup(filename);
    filename = mx_compress_image(filename);
    char *bytebuf = mx_read_full_file(filename, &len);
    binaryb64 *b64buf = malloc(Base64encode_len(len) + 1);
    b64buf[Base64encode_len(len) + 1] = 0;
    Base64encode(b64buf, bytebuf, len);
    free(bytebuf);
    t_message *ms = malloc(sizeof(t_message));
    memset(ms, 0, sizeof(t_message));
    ms->author_id = user_info->user_id;
    ms->chat_id = current_chat.chat_id;
    ms->media = b64buf;
    ms->message_type = photo;
    ms->sendtime = time(NULL);
    ms->text = tmp;
    ms->edit_time = time(NULL);
    
    char *ms_json = (char *)mx_t_message_jsonify(*ms);
    free(ms->media);
    free(ms->text);
    free(ms);

    mx_begin_io_transaction();
    mx_ssl_send_package(ms_json);
    free(ms_json);
    char *resp = mx_ssl_read_package();
    mx_end_io_transaction();

    t_message *resp_ms = mx_t_message_unjsonify(resp);
    free(resp);
    mx_message_inserter(resp_ms);
    mx_t_message_free(resp_ms);
    messages_need_to_updata = true;

    //gtk_image_set_from_file(GTK_IMAGE(imageReg), filename);
    //scale_image(imageReg, 50, 50);
    mx_delete_tmp_file(filename);
  }
  gtk_widget_destroy(dialog);
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Return) { 
        on_SendMessage_clicked((GtkButton *)SendMessage, InputMessage);
        return TRUE;
    }
    return FALSE;
}


