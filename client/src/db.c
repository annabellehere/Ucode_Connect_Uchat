#include "../inc/uchat.h"

char *db_key = NULL;

char *mx_get_client_db_key(void) {
    if (db_key != NULL) return db_key;
    long len = 0;
    db_key = mx_read_full_file("./clientDBKey", &len);
    char *tmp = mx_strnew(len);
    mx_strncpy(tmp, db_key, len);
    db_key = tmp;
    return db_key;
} 


bool mx_checkForBD(void){
    struct stat zhopa;
    if(stat(DB_PATH, &zhopa) == 0){
        return true;
    }
    int res = sqlite3_open(DB_PATH, &db_client);
    if (!db_client){
        sqlite3_close(db_client);
        db_client = NULL;
        return false; 
    }
    sqlite3_key(db_client, mx_get_client_db_key(), strlen(mx_get_client_db_key()));
    res = sqlite3_exec(db_client, "CREATE TABLE user (" \
        "user_id INTEGER PRIMARY KEY AUTOINCREMENT, " \
        "user_nick TEXT NOT NULL, "\
        "user_avathar TEXT, "\
        "user_login TEXT NOT NULL UNIQUE, "\
        "password TEXT NOT NULL, "\
        "create_time DATETIME NOT NULL DEFAULT (DATETIME('now' )), "\
        "edit_time DATETIME NOT NULL DEFAULT (DATETIME('now' )) "\
        ");", NULL, NULL, NULL);
    if(res != SQLITE_OK) {
        sqlite3_close(db_client);
        db_client = NULL;
        return false;
    }
    res = sqlite3_exec(db_client, "CREATE TABLE chat ( " \
        "chat_id INTEGER PRIMARY KEY AUTOINCREMENT , " \
        "chat_type INTEGER NOT NULL ," \
        "chat_name TEXT ," \
        "chat_avathar TEXT ," \
        "member_count INTEGER NOT NULL ," \
        "create_time DATETIME NOT NULL "\
        ");", NULL, NULL, NULL);
    if(res != SQLITE_OK) {
        sqlite3_close(db_client);
        db_client = NULL;
        log_error("chat");
        log_error("%i",res);
        return false;
    }
    res = sqlite3_exec(db_client, "CREATE TABLE chat_user (" \
        "chat_id INTEGER NOT NULL," \
        "user_id INTEGER NOT NULL," \
        "FOREIGN KEY (chat_id) REFERENCES chat (chat_id)," \
        "FOREIGN KEY (user_id) REFERENCES user (user_id)" \
        ");", NULL, NULL, NULL);
    if(res != SQLITE_OK) {
        sqlite3_close(db_client);
        db_client = NULL;
        return false;
    }
    res = sqlite3_exec(db_client, "CREATE TABLE message (" \
        "chat_id INTEGER NOT NULL," \
        "sendtime DATETIME NOT NULL," \
        "text TEXT NOT NULL," \
        "author_id INTEGER NOT NULL," \
        "message_id INTEGER PRIMARY KEY AUTOINCREMENT," \
        "message_type INTEGER NOT NULL," \
        "media TEXT," \
        "edit_time DATETIME NOT NULL," \
        "FOREIGN KEY (chat_id) REFERENCES chat (chat_id)," \
        "FOREIGN KEY (author_id) REFERENCES user (user_id)" \
        ");", NULL, NULL, NULL);
    if(res != SQLITE_OK) {
        sqlite3_close(db_client);
        db_client = NULL;
        return false;
    }
    sqlite3_close(db_client);
    db_client = NULL;
    return true;
}

/*bool mx_add_db_user_to_client(t_db_user usr) {
    sqlite3_stmt *st = NULL;
    int code = 0;

    sqlite3_prepare_v2(db_client, "INSERT INTO user "\
    "(user_id, user_nick, user_avathar, user_login, password)"\
    " VALUES( ? , ? , ? , ? , ? );", -1, &st, NULL);
    sqlite3_bind_int64(st, 1, usr.user_id);
    sqlite3_bind_text(st, 2, usr.nickname, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 3, usr.avathar, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 4, usr.user_login, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 5, usr.password_hash, -1, SQLITE_STATIC);
    
    while((code = sqlite3_step(st)) == SQLITE_BUSY){
        usleep(10000);
    }

    if (code == SQLITE_DONE) return true;
    else return false;
}*/


void mx_t_chat_panel_free(t_chat_panel *p) {
    free(p->chat_avathar);
    free(p->chat_shown_name);
    free(p->user_ids);
    free(p);
}

t_db_user *mx_ask_for_user_data_by_login(char *login){
    t_db_user *usr = malloc(sizeof(t_db_user));
    memset(usr, 0, sizeof(t_db_user));
    usr->user_id = 0;
    usr->nickname = strdup("");
    usr->user_login = strdup(login);
    usr->password_hash = strdup("");
    usr->avathar = strdup("");
    const char *req = mx_t_db_user_jsonify(*usr);
    mx_t_db_user_free(usr);
    mx_begin_io_transaction();
    mx_ssl_send_package(req);
    char *resp = mx_ssl_read_package();
    mx_end_io_transaction();
    
    usr = mx_t_db_user_unjsonify(resp);
    
    bool isInserted = mx_user_inserter(usr);
    free(resp);
    if(isInserted){
        return usr;
    }
    else {
        mx_t_db_user_free(usr);
        return NULL;
    }
    return usr;
}

t_db_user *mx_ask_for_client_db_user(char *login){
    t_db_user *usr = malloc(sizeof(t_db_user));
    memset(usr, 0, sizeof(t_db_user));
    usr->user_login = strdup(login);
    usr->password_hash = strdup("");
    usr->avathar = strdup("");
    usr->nickname = strdup("");
    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db_client, "SELECT user_id, user_nick, user_avathar FROM user WHERE ( user_login = ? );", -1, &st, NULL);
    sqlite3_bind_text(st, 1, login, -1, SQLITE_STATIC);
    int res = sqlite3_step(st);
    if (res == SQLITE_ROW){
        usr->user_id = sqlite3_column_int64(st, 0);

        char *tmp = (char *)sqlite3_column_text(st, 1);
        if (tmp != NULL)
            usr->nickname = strdup(tmp);

        tmp = (char *)sqlite3_column_text(st, 2);
        if (tmp != NULL)
            usr->avathar = strdup(tmp);
        sqlite3_finalize(st);
        return usr;
    }
    else {
        mx_t_db_user_free(usr);
        sqlite3_finalize(st);
        return mx_ask_for_user_data_by_login(login);
    }
}


/*
typedef struct s_chat_panel {
    //usernick if personal, chat_name if group
    char *chat_shown_name;
    binaryb64 *chat_avathar;
    long chat_id;
    //ids of users
    long *user_ids;
    long member_count;
    long chat_type;
    time_t last_message_time;
}              t_chat_panel;
*/
//returns NULL-terminated array of t_chat_panel objects
//they are used to show available chats on the left of main window
t_list *mx_get_chat_panels_to_show(long user_id) {
    t_list *res = NULL;
    sqlite3_stmt *f_st = NULL;
    sqlite3_prepare_v2(db_client, "SELECT chat_user.chat_id, chat.member_count, chat.chat_type FROM chat JOIN chat_user ON chat.chat_id = chat_user.chat_id WHERE chat_user.user_id = ?;", -1, &f_st, NULL);
    sqlite3_bind_int64(f_st, 1, user_id);
    int st_res;
    while ((st_res = sqlite3_step(f_st)) == SQLITE_ROW){
        //log_info("ROW");
        t_chat_panel *new_n = malloc(sizeof(t_chat_panel));
        memset(new_n, 0, sizeof(t_chat_panel));
        new_n->chat_id = sqlite3_column_int64(f_st, 0);
        new_n->member_count = sqlite3_column_int64(f_st, 1);
        new_n->chat_type = sqlite3_column_int64(f_st, 2);
        new_n->user_ids = malloc(sizeof(long) * new_n->member_count);
        sqlite3_stmt *p_st = NULL;
        sqlite3_prepare_v2(db_client, "SELECT MAX(CAST(strftime('%s', sendtime ) AS INTEGER)) FROM message WHERE chat_id = ?;", -1, &p_st, NULL);
        sqlite3_bind_int64(p_st, 1, new_n->chat_id);
        new_n->last_message_time = sqlite3_column_int64(p_st, 0);
        //log_info("chat_type: %d",new_n->chat_type);
        if (new_n->chat_type == personal){
            sqlite3_stmt *s_st = NULL;
            //log_info("Chat_id %d",new_n->chat_id);
            sqlite3_prepare_v2(db_client, "SELECT chat_user.user_id, user.user_nick, user.user_avathar FROM chat_user JOIN user ON chat_user.user_id = user.user_id WHERE chat_id = ? AND user.user_id != ?;", -1, &s_st, NULL);
            sqlite3_bind_int64(s_st, 1, new_n->chat_id);
            sqlite3_bind_int64(s_st, 2, user_id);
            if (sqlite3_step(s_st) != SQLITE_ROW){
               
                sqlite3_stmt *four_st = NULL;
                sqlite3_prepare_v2(db_client, "SELECT user_id FROM chat_user WHERE chat_id = ? AND user_id != ?;", -1, &four_st, NULL);
                sqlite3_bind_int64(four_st, 1, new_n->chat_id);
                sqlite3_bind_int64(four_st, 2, user_id);
                sqlite3_step(four_st);
                t_db_user *user = mx_ask_for_user_data(sqlite3_column_int64(four_st, 0));
                 //log_info("sobesednik: %d", sqlite3_column_int64(four_st, 0));
                sqlite3_finalize(four_st);
                if (user == NULL){
                    sqlite3_finalize(s_st);
                    free(new_n->user_ids);
                    free(new_n);
                    //log_info("ERROR");
                    continue;
                }
                else {
                    new_n->chat_avathar = strdup(user->avathar);
                    new_n->chat_shown_name = strdup(user->nickname);
                    new_n->user_ids[0] = user_id;
                    new_n->user_ids[1] = user->user_id;
                    free(user);
                }
            }
            else {
                new_n->chat_avathar = strdup((char *)sqlite3_column_text(s_st, 2));
                new_n->chat_shown_name = strdup((char *)sqlite3_column_text(s_st, 1));
                new_n->user_ids[0] = user_id;
                new_n->user_ids[1] = sqlite3_column_int64(s_st, 0);
            }
            sqlite3_finalize(s_st);
            
        }
        else if (new_n->chat_type == group){

            sqlite3_stmt *s_st = NULL;
            sqlite3_prepare_v2(db_client, "SELECT chat_avathar, chat_name FROM chat WHERE chat_id = ?;", -1, &s_st, NULL);
            sqlite3_bind_int64(s_st, 1, new_n->chat_id);
            sqlite3_step(s_st);
            if (sqlite3_column_text(s_st, 0) != NULL)
                new_n->chat_avathar = strdup((char *)sqlite3_column_text(s_st, 0));
            else
                new_n->chat_avathar = strdup("");
            if (sqlite3_column_text(s_st, 1) != NULL)
                new_n->chat_shown_name = strdup((char *)sqlite3_column_text(s_st, 1));
            else
                new_n->chat_shown_name = strdup("");
            sqlite3_finalize(s_st);
            sqlite3_prepare_v2(db_client, "SELECT user_id FROM chat_user WHERE chat_id = ?;", -1, &s_st, NULL);
            sqlite3_bind_int64(s_st, 1, new_n->chat_id);
            int res;
            int count = 0;

            while ((res = sqlite3_step(s_st)) == SQLITE_ROW && count < new_n->member_count){
                long user_id_ask = sqlite3_column_int64(s_st, 0);
                new_n->user_ids[count] = user_id_ask;
                count++;
                t_db_user *usr = mx_ask_for_user_data(user_id_ask);
                if (usr == NULL) continue;
                else mx_t_db_user_free(usr);
            }
            sqlite3_finalize(s_st);
        }
        //log_info("PUSHING: %d", mx_list_size(res));
        mx_push_front(&res, (void *)new_n);
    }
    //log_info("BEFORE SORTING: %d",mx_list_size(res));
    sqlite3_finalize(f_st);
    t_list *sorted_list = mx_sort_list(res, time_comparator);
    //log_info("AFTER SORTING: %d", mx_list_size(sorted_list));
    return sorted_list;
}
t_list *mx_get_chat_panels_to_show_by_chat_id(long user_id, long chat_id) {
    t_list *res = NULL;
    sqlite3_stmt *f_st = NULL;
    sqlite3_prepare_v2(db_client, "SELECT chat_user.chat_id, chat.member_count, chat.chat_type FROM chat JOIN chat_user ON chat.chat_id = chat_user.chat_id WHERE chat_user.user_id = ? and chat_user.chat_id = ? ;", -1, &f_st, NULL);
    sqlite3_bind_int64(f_st, 1, user_id);
    sqlite3_bind_int64(f_st, 2, chat_id);
    int st_res;
    while ((st_res = sqlite3_step(f_st)) == SQLITE_ROW){
        t_chat_panel *new_n = malloc(sizeof(t_chat_panel));
        memset(new_n, 0, sizeof(t_chat_panel));
        new_n->chat_id = sqlite3_column_int64(f_st, 0);
        new_n->member_count = sqlite3_column_int64(f_st, 1);
        new_n->chat_type = sqlite3_column_int64(f_st, 2);
        new_n->user_ids = malloc(sizeof(long) * new_n->member_count);
        sqlite3_stmt *p_st = NULL;
        sqlite3_prepare_v2(db_client, "SELECT MAX(CAST(strftime('%s', sendtime ) AS INTEGER)) FROM message WHERE chat_id = ?;", -1, &p_st, NULL);
        sqlite3_bind_int64(p_st, 1, new_n->chat_id);
        new_n->last_message_time = sqlite3_column_int64(p_st, 0);

        if (new_n->chat_type == personal){
            sqlite3_stmt *s_st = NULL;
            sqlite3_prepare_v2(db_client, "SELECT chat_user.user_id, user.user_nick, user.user_avathar FROM chat_user JOIN user ON chat_user.user_id = user.user_id WHERE chat_id = ? AND user.user_id != ?;", -1, &s_st, NULL);
            sqlite3_bind_int64(s_st, 1, new_n->chat_id);
            sqlite3_bind_int64(s_st, 2, user_id);
            if (sqlite3_step(s_st) != SQLITE_ROW){
                sqlite3_stmt *four_st = NULL;
                sqlite3_prepare_v2(db_client, "SELECT user_id FROM chat_user WHERE chat_id = ? AND user_id != ?;", -1, &four_st, NULL);
                sqlite3_bind_int64(four_st, 1, new_n->chat_id);
                sqlite3_bind_int64(four_st, 2, user_id);
                sqlite3_step(four_st);
                t_db_user *user = mx_ask_for_user_data(sqlite3_column_int64(four_st, 0));
                if (user == NULL){
                    sqlite3_finalize(s_st);
                    free(new_n->user_ids);
                    free(new_n);
                    
                    continue;
                }
                else {
                    new_n->chat_avathar = strdup(user->avathar);
                    new_n->chat_shown_name = strdup(user->nickname);
                    new_n->user_ids[0] = user_id;
                    new_n->user_ids[1] = user->user_id;
                    free(user);
                }
            }
            else {
                new_n->chat_avathar = strdup((char *)sqlite3_column_text(s_st, 2));
                new_n->chat_shown_name = strdup((char *)sqlite3_column_text(s_st, 1));
                new_n->user_ids[0] = user_id;
                new_n->user_ids[1] = sqlite3_column_int64(s_st, 0);
            }
            sqlite3_finalize(s_st);
            
        }
        else if (new_n->chat_type == group){

            sqlite3_stmt *s_st = NULL;
            sqlite3_prepare_v2(db_client, "SELECT chat_avathar, chat_name FROM chat WHERE chat_id = ?;", -1, &s_st, NULL);
            sqlite3_bind_int64(s_st, 1, new_n->chat_id);
            sqlite3_step(s_st);
            if (sqlite3_column_text(s_st, 0) != NULL)
                new_n->chat_avathar = strdup((char *)sqlite3_column_text(s_st, 0));
            else
                new_n->chat_avathar = strdup("");
            if (sqlite3_column_text(s_st, 1) != NULL)
                new_n->chat_shown_name = strdup((char *)sqlite3_column_text(s_st, 1));
            else
                new_n->chat_shown_name = strdup("");
            sqlite3_finalize(s_st);
            sqlite3_prepare_v2(db_client, "SELECT user_id FROM chat_user WHERE chat_id = ?;", -1, &s_st, NULL);
            sqlite3_bind_int64(s_st, 1, new_n->chat_id);
            int res;
            int count = 0;

            while ((res = sqlite3_step(s_st)) == SQLITE_ROW && count < new_n->member_count){
                long user_id_ask = sqlite3_column_int64(s_st, 0);
                new_n->user_ids[count] = user_id_ask;
                count++;
                t_db_user *usr = mx_ask_for_user_data(user_id_ask);
                if (usr == NULL) continue;
                else mx_t_db_user_free(usr);
            }
        }

        mx_push_front(&res, (void *)new_n);
    }

    sqlite3_finalize(f_st);
    t_list *sorted_list = mx_sort_list(res, time_comparator);
    return sorted_list;
}

bool time_comparator(void *a, void *b){
    return ((t_chat_panel *)a)->last_message_time < ((t_chat_panel *)b)->last_message_time;
}

t_db_user *mx_ask_for_user_data(long user_id){
    t_db_user *usr = malloc(sizeof(t_db_user));
    memset(usr, 0, sizeof(t_db_user));
    usr->user_id = user_id;
    usr->nickname = strdup("");
    usr->user_login = strdup("");
    usr->password_hash = strdup("");
    usr->avathar = strdup("");
    const char *req = mx_t_db_user_jsonify(*usr);
    mx_t_db_user_free(usr);
    mx_begin_io_transaction();
    mx_ssl_send_package(req);
    char *resp = mx_ssl_read_package();
    mx_end_io_transaction();
    
    usr = mx_t_db_user_unjsonify(resp);
    usr->password_hash = strdup("");
    bool isInserted = mx_user_inserter(usr);
    
    free(resp);
    if(isInserted){
        return usr;
    }
    else {
        mx_t_db_user_free(usr);
        return NULL;
    }
    return usr;
}

bool mx_user_inserter(t_db_user *user){
    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db_client, "INSERT INTO user (user_id, user_nick, user_avathar, user_login, password) VALUES ( ? , ? , ? , ? , '');", -1, &st, NULL);
    sqlite3_bind_int64(st, 1, user->user_id);
    sqlite3_bind_text(st, 2, user->nickname, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 3, user->avathar, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 4, user->user_login, -1, SQLITE_STATIC);
    int res = 0;
    while ((res = sqlite3_step(st)) == SQLITE_BUSY){
        usleep(10000);
    }
    if (res == SQLITE_DONE){
        sqlite3_finalize(st);
        return true;
    }
    else {
        sqlite3_finalize(st);
        return false;
    }

}
bool mx_user_insert_updater(t_db_user *user) {
    if (mx_user_inserter(user)) return true;
    sqlite3_stmt *st = NULL;
    //log_info("Try updating user");
    int code = 0;
    code = sqlite3_prepare_v2(db_client, "UPDATE user SET user_nick = ? , user_avathar = ? WHERE user_id = ?;", -1, &st, NULL);
    //log_info("Prepase statement %i", code);
    sqlite3_bind_text(st, 1, user->nickname, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 2, user->avathar, -1, SQLITE_STATIC);
    sqlite3_bind_int64(st, 3, user->user_id);
    //log_info("Stepping");
    while ((code = sqlite3_step(st)) == SQLITE_BUSY) {
        usleep(10000);
        //log_info("Step");
    }
    //log_info("Out Stepping %i", code);
    sqlite3_finalize(st);
    if (code == SQLITE_DONE) return true;
    else return false;
}

char *mx_get_user_avathar_by_id_client(long user_id) {
    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db_client, "SELECT user_avathar FROM user WHERE user_id = ? ;", -1, &st, NULL);
    sqlite3_bind_int64(st, 1, user_id);

    if (sqlite3_step(st) == SQLITE_ROW) {
        char *res = (char *)sqlite3_column_text(st, 0);
        if (res == NULL) res = strdup("");
        else res = strdup(res);
        sqlite3_finalize(st);
        return res;
    }
    return strdup("");
}


bool mx_chats_inserter(t_create_chat *chat){
    sqlite3_stmt *st = NULL;
    int chat_type;
    if (chat->member_count > 2){
        chat_type = group;
    }
    else {
        chat_type = personal;
    }
    sqlite3_prepare_v2(db_client, "INSERT INTO chat (chat_id, chat_type, chat_name, chat_avathar, member_count, create_time) VALUES ( ? , ? , ? , ? , ? , datetime(?,'unixepoch' ));", -1 , &st, NULL);
    sqlite3_bind_int64(st, 1, chat->chat_id);
    sqlite3_bind_int64(st, 2, chat_type);
    sqlite3_bind_text(st, 3, chat->chat_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 4, chat->chat_avathar, -1, SQLITE_STATIC);
    sqlite3_bind_int64(st, 5, chat->member_count);
    sqlite3_bind_int64(st, 6, chat->creation_time);
    int result;
    while ((result = sqlite3_step(st)) == SQLITE_BUSY){
        usleep(10000);
    }
    if (result == SQLITE_DONE){
        sqlite3_finalize(st);
        for (int i = 0; i < chat->member_count; i++){
            sqlite3_prepare_v2(db_client, "INSERT INTO chat_user (chat_id, user_id) VALUES (? , ?);", -1, &st, NULL);
            sqlite3_bind_int64(st, 1, chat->chat_id);
            sqlite3_bind_int64(st, 2, chat->user_ids[i]);
            int res;
            while ((res = sqlite3_step(st)) == SQLITE_BUSY){
                usleep(10000);
            }
            sqlite3_finalize(st);
            if (res != SQLITE_DONE){
                return false;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

bool mx_message_inserter(t_message *mess) {
    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db_client, "INSERT INTO message (chat_id, sendtime, author_id, text, "\
    "message_id, message_type, media, edit_time) "\
    "VALUES ( ? , DATETIME( ? , 'unixepoch' ) , ? , ? , ? , ? , ? , DATETIME( ? , 'unixepoch' ));", -1, &st, NULL);
    sqlite3_bind_int64(st, 1, mess->chat_id);
    sqlite3_bind_int64(st, 2, mess->sendtime);
    sqlite3_bind_int64(st, 3, mess->author_id);
    sqlite3_bind_text(st, 4, mess->text, -1, SQLITE_STATIC);
    sqlite3_bind_int64(st, 5, mess->message_id);
    sqlite3_bind_int64(st, 6, mess->message_type);
    sqlite3_bind_text(st, 7, mess->media, -1, SQLITE_STATIC);
    sqlite3_bind_int64(st, 8, mess->edit_time);
    int code = 0;
    while((code = sqlite3_step(st)) == SQLITE_BUSY) {
        usleep(10000);
    }
    sqlite3_finalize(st);
    if (code == SQLITE_DONE) return true;
    else return false;
}

bool mx_message_insert_updater(t_message *mess) {
    if (mx_message_inserter(mess) == true) return true;
    //else update message
    //log_debug("try updating message");
    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db_client, "UPDATE message SET text = ? , edit_time = DATETIME( ? , 'unixepoch ) WHERE message_id = ? AND text != ? ;", -1, &st, NULL);
    sqlite3_bind_text(st, 1, mess->text, -1, SQLITE_STATIC);
    sqlite3_bind_int64(st, 2, mess->edit_time);
    sqlite3_bind_int64(st, 3, mess->message_id);
    sqlite3_bind_text(st, 4, mess->text, -1, SQLITE_STATIC);
    int code = 0;
    while ((code = sqlite3_step(st)) == SQLITE_BUSY) {
        usleep(10000);
    }
    if (code == SQLITE_DONE){
        sqlite3_finalize(st);
        //log_debug("update success");
        return true;
    }
    sqlite3_finalize(st);
    return true;
}

bool mx_message_db_edit(t_edit_message *mess){
    char *req_str = (char *) mx_t_edit_message_jsonify(*mess);
    mx_begin_io_transaction();
    mx_ssl_send_package((const char *)req_str);
    char *res_str = mx_ssl_read_package();
    mx_end_io_transaction();
    t_edit_message *response = mx_t_edit_message_unjsonify(res_str);
    if(response->message_id < 0){
        //OOOO PAVEZLO PAVEZLO
        mx_t_edit_message_free(response);
        return false;
    }
    else {
        sqlite3_stmt *upd = NULL;
        if (response->edit_time < 0){
            sqlite3_prepare_v2(db_client, "DELETE FROM message WHERE message_id = ?;", -1, &upd, NULL);
            sqlite3_bind_int64(upd, 1, response->message_id);
        }
        else {
            sqlite3_prepare_v2(db_client, "UPDATE message SET edit_time = DATETIME(?, 'unixepoch' ), text = ? WHERE message_id = ?;", -1, &upd, NULL);
            sqlite3_bind_int64(upd, 1, response->edit_time);
            sqlite3_bind_text(upd, 2, response->new_text, -1, SQLITE_STATIC);
            sqlite3_bind_int64(upd, 3, response->message_id);
        }
        int result;
        while ((result = sqlite3_step(upd)) == SQLITE_BUSY){
            usleep(10000);
        }
        mx_t_edit_message_free(response);
        sqlite3_finalize(upd);
        if (result != SQLITE_DONE){
            //log_error("misha havays9 v zhyto");
            return false;
        }
        else{
            messages_need_to_updata = true;
            
            return true;
        }

    }
}


/*typedef struct s_message_panel {
    long author_id;
    char *author_nick;
    char *text;
    long message_id;
    long message_type;
    binaryb64 *media;
    long sendtime;
}              t_message_panel;*/

//returns list of t_message_panel
t_list *mx_get_message_panels_in_chat(long chat_id, long start, long end) {
    if (end < 0) end = -1;
    if (start < 0) start = 0;
    t_list *res = NULL;

    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db_client, "SELECT "\
    "author_id, text, message_id, message_type, media, "\
    "CAST(strftime('%s', sendtime ) AS INTEGER), user_nick "\
    " FROM message JOIN user ON message.author_id = user.user_id WHERE chat_id = ? ORDER BY sendtime LIMIT ? OFFSET ? ;", -1, &st, NULL);
    sqlite3_bind_int64(st, 1, chat_id);
    sqlite3_bind_int64(st, 2, end - start);
    sqlite3_bind_int64(st, 3, start);

    while(sqlite3_step(st) == SQLITE_ROW) {
        
        char *tmp = NULL;
        t_message_panel *mp = malloc(sizeof(t_message_panel));
        memset(mp, 0, sizeof(t_message_panel));
        mp->author_id = sqlite3_column_int64(st, 0);
        mp->text = strdup((char *)sqlite3_column_text(st, 1));
        mp->message_id = sqlite3_column_int64(st, 2);
        mp->message_type = sqlite3_column_int64(st, 3);
        tmp = (char *)sqlite3_column_text(st, 4);

        if (tmp == NULL) mp->media = strdup("");
        else mp->media = strdup(tmp);

        mp->sendtime = sqlite3_column_int64(st, 5);
        mp->author_nick = strdup((char *)sqlite3_column_text(st, 6));

        mx_push_back(&res, mp);

    }
    sqlite3_finalize(st);
    mx_sort_list(res, time_comparator);
    return res;
}


void mx_t_message_panel_free(t_message_panel *panel){
    free(panel->media);
    free(panel->text);
    free(panel->author_nick);
}


