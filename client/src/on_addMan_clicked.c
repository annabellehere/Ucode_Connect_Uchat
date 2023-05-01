#include "../inc/uchat.h"

void on_addMan_clicked(GtkButton *b) {
    bool exist = false;
    char *find_log = mx_strdup(gtk_entry_get_text(GTK_ENTRY(inputName)));
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
        if (found != NULL){
            exist = true;
        }
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
        int row = get_num_filled_rows(grid);
        gtk_grid_insert_row(GTK_GRID(Grid), row);
        button[row] = gtk_button_new_with_label(find_log);
        gtk_widget_set_size_request(button[row], 200, 25);
        css_set(provider, button[row]);
        gtk_button_set_alignment(GTK_BUTTON(button[row]), 0.0, 0.5);
        gtk_grid_attach(GTK_GRID(grid), button[row], 1, row, 1, 1);
        gtk_entry_set_text(GTK_ENTRY(inputName), "");
        gtk_widget_show_all(window1);


        mx_init_new_gchat();
        new_gchat.member_count++;
        new_gchat.user_ids = realloc(new_gchat.user_ids, sizeof(long) * new_gchat.member_count);
        new_gchat.user_ids[new_gchat.member_count - 1] = found->user_id;
        //log_info("Added user %i total %i", found->user_id, new_gchat.member_count);
    }
    else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "No such user, check once more");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    free(find_log);
}

void mx_init_new_gchat(void) {
    static bool done = 0;
    if (done == 1) return;
    memset(&new_gchat, 0, sizeof(t_create_chat));
    done = 1;
    new_gchat.chat_avathar = strdup("");
    new_gchat.chat_name = strdup("");
    new_gchat.user_ids = NULL;
}

void mx_reset_new_gchat(void) {
    if (new_gchat.chat_avathar != NULL) free(new_gchat.chat_avathar);
    if (new_gchat.chat_name != NULL) free(new_gchat.chat_name);
    if (new_gchat.user_ids != NULL) free(new_gchat.user_ids);
    memset(&new_gchat, 0, sizeof(t_create_chat));
    new_gchat.chat_avathar = strdup("");
    new_gchat.chat_name = strdup("");
    new_gchat.user_ids = NULL;
}
