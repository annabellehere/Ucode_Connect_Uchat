#include "../inc/uchat.h"
char *db_key = NULL;

char *mx_read_full_file(char *file, long *len) {
    FILE *f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    long i = ftell(f);
    *len = i;
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(i);
    fread(buf, i, 1, f);
    fclose(f);
    return buf;
}

char *mx_get_server_db_key(void) {
    if (db_key != NULL) return db_key;
    long len = 0;
    db_key = mx_read_full_file("./serverDBKey", &len);
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
    int res = sqlite3_open(DB_PATH, &db);
    if (!db){
        sqlite3_close(db);
        db = NULL;
        return false; 
    }
    sqlite3_key(db, mx_get_server_db_key(), strlen(mx_get_server_db_key()));
    res = sqlite3_exec(db, "CREATE TABLE user (" \
        "user_id INTEGER PRIMARY KEY AUTOINCREMENT, " \
        "user_nick TEXT NOT NULL, "\
        "user_avathar TEXT, "\
        "user_login TEXT NOT NULL UNIQUE, "\
        "password TEXT NOT NULL, "\
        "create_time DATETIME NOT NULL DEFAULT (DATETIME('now' )),"\
        "edit_time DATETIME NOT NULL DEFAULT (DATETIME('now' ))"\
        ");", NULL, NULL, NULL);
    if(res != SQLITE_OK) {
        sqlite3_close(db);
        db = NULL;
        return false;
    }
    res = sqlite3_exec(db, "CREATE TABLE chat ( " \
        "chat_id INTEGER PRIMARY KEY AUTOINCREMENT , " \
        "chat_type INTEGER NOT NULL ," \
        "chat_name TEXT ," \
        "chat_avathar TEXT ," \
        "member_count INTEGER NOT NULL ," \
        "create_time DATETIME NOT NULL DEFAULT (DATETIME(NULL )) ," \
        ");", NULL, NULL, NULL);
    if(res != SQLITE_OK) {
        sqlite3_close(db);
        db = NULL;
        return false;
    }
    res = sqlite3_exec(db, "CREATE TABLE chat_user (" \
        "chat_id INTEGER NOT NULL," \
        "user_id INTEGER NOT NULL," \
        "FOREIGN KEY (chat_id) REFERENCES chat (chat_id)," \
        "FOREIGN KEY (user_id) REFERENCES user (user_id)" \
        ");", NULL, NULL, NULL);
    if(res != SQLITE_OK) {
        sqlite3_close(db);
        db = NULL;
        return false;
    }
    res = sqlite3_exec(db, "CREATE TABLE message (" \
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
        sqlite3_close(db);
        db = NULL;
        return false;
    }
    sqlite3_close(db);
    db = NULL;
    return true;
}

char *mx_login_form_handle( const char* message_json){ 
	t_login_form *form = mx_t_login_form_unjsonify(message_json);
	sqlite3_stmt *st = NULL;
	t_login_result *res = malloc(sizeof(t_login_result));
	memset(res, 0, sizeof(t_login_result));

    char *hash = (char *)SHA256((unsigned char *)form->password, strlen(form->password), NULL);
    int b64len = Base64encode_len(strlen(hash));
    char *b64hash = malloc(b64len + 1);
    memset(b64hash, 0, b64len + 1);
    Base64encode(b64hash, hash, strlen(hash));
	sqlite3_prepare_v2(db, "SELECT user_login, password FROM user WHERE (user_login = ? AND password = ? );", -1, &st, NULL);
	sqlite3_bind_text(st, 1, form->user_login, -1, SQLITE_STATIC);
	sqlite3_bind_text(st, 2, b64hash, -1, SQLITE_STATIC);
	if (sqlite3_step(st) == SQLITE_ROW) {
		res->success = true;
		res->details = mx_strnew(0);
		char *res_str = (char *)mx_t_login_result_jsonify(*res);
		mx_t_login_result_free(res);
		sqlite3_finalize(st);
        free(b64hash);
		return res_str;
	}
	sqlite3_finalize(st);
	res->success = false;
	res->details = mx_strnew(0);
	char *res_str = (char *)mx_t_login_result_jsonify(*res);
    free(b64hash);
	mx_t_login_result_free(res);
	return res_str;
}

char *mx_registration_form_handle(const char *form_json){
    t_registration_form *form = mx_t_registration_form_unjsonify(form_json);
    sqlite3_stmt *st = NULL;
	t_registration_result *res = malloc(sizeof(t_registration_result));
    memset(res, 0, sizeof(t_registration_result));

    char *hash = (char *)SHA256((unsigned char *)form->password, strlen(form->password), NULL);
    int b64len = Base64encode_len(strlen(hash));
    char *b64hash = malloc(b64len + 1);
    memset(b64hash, 0, b64len + 1);
    Base64encode(b64hash, hash, strlen(hash));


    sqlite3_prepare_v2(db, "INSERT INTO user (user_nick, user_avathar, user_login, password) VALUES ( ? , ? , ?  , ?  );", -1, &st, NULL);
    sqlite3_bind_text(st, 1, form->nickname, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 2, form->avathar, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 3, form->user_login, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 4, b64hash, -1, SQLITE_STATIC);
    int result;
    while((result = sqlite3_step(st)) == SQLITE_BUSY){
        usleep(10000);
    }
    if (result == SQLITE_DONE){
        sqlite3_stmt *user_st = NULL;
        sqlite3_prepare_v2(db, "SELECT user_id FROM user WHERE (user_login = ?);", -1, &user_st, NULL);
        sqlite3_bind_text(user_st, 1, form->user_login, -1, SQLITE_STATIC);
        sqlite3_step(user_st);
        res->success = true;
        res->details = strdup("ty ne churka");
        res->user_id = sqlite3_column_int64(user_st, 0);
        sqlite3_finalize(user_st);
    }
    else if(result == SQLITE_CONSTRAINT){
        res->success = false;
        res->details = strdup("User with such login already exists!");
        res->user_id = -1;
    }
    else {
        res->success = false;
        res->details = strdup("ty churka");
        res->user_id = -1;
    }
    char *str_res = (char *)mx_t_registration_result_jsonify(*res);
    mx_t_registration_result_free(res);
    mx_t_registration_form_free(form);
    sqlite3_finalize(st);
    free(b64hash);
    return str_res;
}
char *mx_chat_handle(char *json_str){
    t_chat *chat = mx_t_chat_unjsonify(json_str);
    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db, "SELECT *,CAST(strftime('%s', create_time ) AS INTEGER) FROM chat WHERE (chat_id = ? );", -1, &st, NULL);
    sqlite3_bind_int64(st, 1, chat->chat_id);
    int result;
    if((result = sqlite3_step(st)) == SQLITE_ROW){
        chat->chat_type = sqlite3_column_int64(st, 1);
        char *tmp = (char *)sqlite3_column_text(st, 2);
        if (tmp == NULL)
            chat->chat_name = strdup("");
        else
            chat->chat_name = strdup((char *)sqlite3_column_text(st, 2));
        tmp = (char *)sqlite3_column_text(st, 3);
        if (tmp == NULL)
            chat->chat_avathar = strdup("");
        else
            chat->chat_avathar = strdup((char *)sqlite3_column_text(st, 3));
        chat->member_count = sqlite3_column_int64(st, 4);
        chat->create_time = sqlite3_column_int64(st,6);
        sqlite3_finalize(st);
        char *res = (char *) mx_t_chat_jsonify(*chat);
        mx_t_chat_free(chat);
        return res;
    }
    chat->chat_id = 0;
    chat->chat_avathar = strdup("");
    chat->chat_name = strdup("");
    char *res = (char *) mx_t_chat_jsonify(*chat);
    mx_t_chat_free(chat);
    return res;
}

char *mx_db_user_handle(const char *json_str){
    t_db_user *db_usr = mx_t_db_user_unjsonify(json_str);
    
    sqlite3_stmt *st = NULL;
    if (db_usr->user_id == 0 ){
        sqlite3_prepare_v2(db, "SELECT * FROM user WHERE (user_login = ?);", -1 , &st, NULL);
        sqlite3_bind_text(st, 1, db_usr->user_login, -1, SQLITE_STATIC);
    }
    else{
        sqlite3_prepare_v2(db, "SELECT * FROM user WHERE (user_id = ?);", -1 , &st, NULL);
        sqlite3_bind_int64(st, 1, db_usr->user_id);
        
    }
    int res = sqlite3_step(st);
    if (res == SQLITE_ROW){
        db_usr->user_id = sqlite3_column_int64(st, 0);
        if (sqlite3_column_text(st, 1) != NULL)
            db_usr->nickname = strdup((char *)sqlite3_column_text(st, 1));
        else
            db_usr->nickname = strdup("");
        if (sqlite3_column_text(st, 2) != NULL)
            db_usr->avathar = strdup((char *)sqlite3_column_text(st, 2));
        else
            db_usr->avathar = strdup("");
        if (sqlite3_column_text(st, 3) != NULL)
            db_usr->user_login = strdup((char *) sqlite3_column_text(st, 3));
        else
            db_usr->user_login = strdup("");
        if (sqlite3_column_text(st, 4) != NULL)
            db_usr->password_hash = strdup((char *) sqlite3_column_text(st, 4));
        else
            db_usr->password_hash = strdup("");
        sqlite3_finalize(st);
        char *str_res = (char *)mx_t_db_user_jsonify(*db_usr);
        //log_error("STR: %s",str_res);
       // mx_t_db_user_free(db_usr);
        return str_res;
    }
    db_usr->user_id = 0;
    char *str_res = (char *)mx_t_db_user_jsonify(*db_usr);
   // mx_t_db_user_free(db_usr);
    return str_res;
}

char *mx_message_handle(const char *message_json){
    t_message *message = mx_t_message_unjsonify(message_json);
    sqlite3_stmt *st = NULL;
    sqlite3_prepare_v2(db, "INSERT INTO message (chat_id, sendtime, text, author_id, message_type, media) VALUES ( ? , datetime( ? ,'unixepoch' ), ? , ? , ? , ? );", -1, &st, NULL);
    sqlite3_bind_int64(st, 1, message->chat_id);
    sqlite3_bind_int64(st, 2, message->sendtime);
    sqlite3_bind_text(st, 3, message->text, -1, SQLITE_STATIC);
    sqlite3_bind_int64(st, 4, message->author_id);
    sqlite3_bind_int64(st, 5, message->message_type);
    sqlite3_bind_text(st, 6, message->media, -1, SQLITE_STATIC);
    int result;
    while((result = sqlite3_step(st)) == SQLITE_BUSY){
        usleep(10000);
    }
    if (result == SQLITE_DONE){
        sqlite3_stmt *st_msg = NULL;
        sqlite3_prepare_v2(db, "SELECT message_id FROM message WHERE (chat_id = ? AND sendtime = datetime( ? ,'unixepoch' ) AND author_id = ? );", -1, &st_msg, NULL);
        sqlite3_bind_int64(st_msg, 1, message->chat_id);
        sqlite3_bind_int64(st_msg, 2, message->sendtime);
        sqlite3_bind_int64(st_msg, 3, message->author_id);
        sqlite3_step(st_msg);
        message->message_id = sqlite3_column_int64(st_msg, 0);
        sqlite3_finalize(st_msg);
    }
    else {
        message->message_id = -1;
    }
    char *str_res = (char *)mx_t_message_jsonify(*message);
    mx_t_message_free(message);
    sqlite3_finalize(st);
    return str_res;
}


char *mx_request_new_messages_handle(char *json_str) {
    t_request_new_messages *req = mx_t_request_new_messages_unjsonify(json_str);
    sqlite3_stmt *st = NULL;
    int code;
    t_request_new_messages_responce *responce = malloc(sizeof(t_request_new_messages_responce));
    memset(responce, 0, sizeof(t_request_new_messages_responce));
    long user_id = req->user_id;
    long timestamp = req->last_request_for_messages_time;
    code = sqlite3_prepare_v2(db, "SELECT chat_id, CAST(strftime('%s', sendtime ) AS INTEGER), text, author_id, message_id, message_type, media, sendtime FROM message WHERE (chat_id IN (SELECT chat_id FROM chat_user WHERE user_id = ? ) AND sendtime >= DATETIME( ? , 'unixepoch' ));" , -1, &st, NULL);
    sqlite3_bind_int64(st, 1, user_id);
    sqlite3_bind_int64(st, 2, timestamp);
    
    while ((code = sqlite3_step(st)) == SQLITE_ROW) {
        responce->new_messages_count++;
        responce->message_arr = realloc(responce->message_arr, responce->new_messages_count * sizeof(t_message));
        t_message tmp_mes;
        memset(&tmp_mes, 0, sizeof(t_message));
        char * tmp;
        tmp_mes.chat_id = sqlite3_column_int64(st, 0);
        tmp_mes.sendtime = sqlite3_column_int64(st, 1);

        tmp = (char *)sqlite3_column_text(st, 2);
        if (tmp == NULL) tmp_mes.text = strdup("");
        else tmp_mes.text = strdup(tmp);

        tmp_mes.author_id = sqlite3_column_int64(st, 3);
        tmp_mes.message_id = sqlite3_column_int64(st, 4);
        tmp_mes.message_type = sqlite3_column_int64(st, 5);

        tmp = (char *)sqlite3_column_text(st, 6);
        if (tmp == NULL) tmp_mes.media = strdup("");
        else tmp_mes.media = strdup(tmp);

        responce->message_arr[responce->new_messages_count - 1] = tmp_mes;
    }
    sqlite3_finalize(st);
    char *res = (char *)mx_t_request_new_messages_responce_jsonify(*responce);
    for (int i = 0; i < responce->new_messages_count; i++) {
        free(((responce->message_arr)[i]).text);
        free(((responce->message_arr)[i]).media);
    }
    
    free(responce->message_arr);
    free(responce);
    mx_t_request_new_messages_free(req);
    return res;

}

char *mx_request_new_chats_handle(char *json_str) {
    t_request_new_chats *req = mx_t_request_new_chats_unjsonify(json_str);
    long user_id = req->user_id;
    long timestamp = req->last_request_for_chats_time;
    mx_t_request_new_chats_free(req);

    sqlite3_stmt *st = NULL;
    t_request_new_chats_responce *resp = malloc(sizeof(t_request_new_chats_responce));
    memset(resp, 0, sizeof(t_request_new_chats_responce));

    sqlite3_prepare_v2(db, "SELECT chat_id, chat_type, chat_name, chat_avathar, member_count "\
    "FROM chat WHERE create_time >= DATETIME( ? , 'unixepoch' ) AND "\
    "chat_id IN (SELECT chat_id FROM chat_user WHERE user_id = ? );", -1, &st, NULL);
    sqlite3_bind_int64(st, 2 ,user_id);
    sqlite3_bind_int64(st, 1, timestamp);
    while (sqlite3_step(st) == SQLITE_ROW) {
        resp->new_chats_count++;
        resp->chat_arr = realloc(resp->chat_arr, resp->new_chats_count * sizeof(t_chat));
        t_create_chat ch;
        char *tmp;
        memset(&ch, 0, sizeof(t_create_chat));
        ch.chat_id = sqlite3_column_int64(st, 0);
        ch.member_count = sqlite3_column_int64(st, 4);

        tmp = (char *)sqlite3_column_text(st, 2);
        if (tmp == NULL) ch.chat_name = strdup("");
        else ch.chat_name = strdup(tmp);

        tmp = (char *)sqlite3_column_text(st, 3);
        if (tmp == NULL) ch.chat_avathar = strdup("");
        else ch.chat_avathar = strdup(tmp);

        ch.user_ids = malloc(sizeof(long) * ch.member_count);
        memset(ch.user_ids, 0, sizeof(long) * ch.member_count);
        //fill in user_ids
        sqlite3_stmt *s_st = NULL;
        sqlite3_prepare_v2(db, "SELECT user_id FROM chat_user WHERE chat_id = ? ;", -1, &s_st, NULL);
        sqlite3_bind_int64(s_st, 1, ch.chat_id);
        int count = 0;
        while (sqlite3_step(s_st) == SQLITE_ROW && count < ch.member_count) {
            ch.user_ids[count] = sqlite3_column_int64(s_st, 0);
            count++;
        }
        sqlite3_finalize(s_st);

        
        resp->chat_arr[resp->new_chats_count - 1] = ch;


    }
    sqlite3_finalize(st);
    char *res = (char *)mx_t_request_new_chats_responce_jsonify(*resp);
    for (int i = 0; i < resp->new_chats_count; i++) {
        free(((resp->chat_arr)[i]).chat_name);
        free(((resp->chat_arr)[i]).chat_avathar);
    }
    free(resp->chat_arr);
    free(resp);
    return res;

}


char *mx_create_chat_handle(char *json_str){
    t_create_chat *new_chat = mx_t_create_chat_unjsonify(json_str);
    sqlite3_stmt *st = NULL;
    if (check_for_already_exist_chat_personal(new_chat) && new_chat->member_count < 3){
        new_chat->chat_id = -2;
        char *res_str = (char *)mx_t_create_chat_jsonify(*new_chat);
        mx_t_create_chat_free(new_chat);
        return res_str;
    }
    sqlite3_prepare_v2(db, "INSERT INTO chat (chat_type, chat_name, chat_avathar, member_count) VALUES ( ? , ? , ? , ? );", -1, &st, NULL);
    if (new_chat->member_count > 2){
        sqlite3_bind_int64(st, 1, group);
    }
    else {
        sqlite3_bind_int64(st, 1, personal);
    }
    sqlite3_bind_text(st, 2, new_chat->chat_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(st, 3, new_chat->chat_avathar, -1, SQLITE_STATIC);
    sqlite3_bind_int64(st, 4, new_chat->member_count);
    int result = 0;
    while((result = sqlite3_step(st)) == SQLITE_BUSY){
        usleep(10000);
    }
    sqlite3_finalize(st);
    long chat_id; 
    if (result == SQLITE_DONE){
        sqlite3_stmt *st_id = NULL;
        sqlite3_prepare_v2(db, "SELECT MAX(chat_id), CAST(strftime('%s', create_time ) AS INTEGER) FROM chat;", -1, &st_id, NULL);
        sqlite3_step(st_id);
        chat_id = sqlite3_column_int64(st_id, 0);
        new_chat->creation_time = sqlite3_column_int64(st_id, 1);
        new_chat->chat_id = chat_id;
        //log_info("CHAT ID: %i .STATEMENT int: %i", new_chat->chat_id, test);
        
        sqlite3_finalize(st_id);
        int chus_res = 0;
        for (int i = 0; i < new_chat->member_count; i++){
            sqlite3_stmt *st_chus = NULL;
            sqlite3_prepare_v2(db, "INSERT INTO chat_user (user_id, chat_id) VALUES ( ? , ? );", -1, &st_chus, NULL);
            sqlite3_bind_int64(st_chus, 1, new_chat->user_ids[i]);
            sqlite3_bind_int64(st_chus, 2, chat_id);
            while((chus_res = sqlite3_step(st_chus)) == SQLITE_BUSY){
                usleep(10000);
            }
            sqlite3_finalize(st_chus);
        }
    }
    else {
        chat_id = -1;
    }
    char *res_str = (char *)mx_t_create_chat_jsonify(*new_chat);
    mx_t_create_chat_free(new_chat);
    return res_str;
}

bool check_for_already_exist_chat_personal(t_create_chat *tocheck){
    sqlite3_stmt *st_f = NULL;
    sqlite3_prepare_v2(db, "SELECT chat_id FROM chat_user WHERE user_id = ?;", -1, &st_f, NULL);
    sqlite3_bind_int64(st_f, 1, tocheck->user_ids[0]);
    int result_f = 0;
    while((result_f = sqlite3_step(st_f)) == SQLITE_ROW){
        sqlite3_stmt *st2 = NULL;
        sqlite3_prepare_v2(db, "SELECT chat_id FROM chat_user WHERE user_id = ?;", -1, &st2, NULL);
        sqlite3_bind_int64(st2, 1, tocheck->user_ids[1]);
        int first_chid = sqlite3_column_int64(st_f, 0);
        int result_s = 0;
        while ((result_s = sqlite3_step(st2)) == SQLITE_ROW){
            int second_chid = sqlite3_column_int64(st2, 0);
            if (first_chid == second_chid) return true;
        }
        sqlite3_finalize(st2);
    }
    sqlite3_finalize(st_f);
    return false;
}

char *mx_edit_message_handle(char *json_str){
    t_edit_message *edit_msg = mx_t_edit_message_unjsonify(json_str);
    //t_message *res_msg = malloc(sizeof(t_message));
    sqlite3_stmt *st = NULL;
    if (edit_msg->edit_time < 0){
        sqlite3_prepare_v2(db, "DELETE FROM message WHERE message_id = ?;", -1, &st, NULL);
        sqlite3_bind_int64(st, 1, edit_msg->message_id);
    }
    else {
        sqlite3_prepare_v2(db, "UPDATE message SET edit_time = DATETIME(?, 'unixepoch' ), text = ? WHERE message_id = ?;", -1, &st, NULL);
        sqlite3_bind_int64(st, 1, edit_msg->edit_time);
        sqlite3_bind_text(st, 2, edit_msg->new_text, -1, SQLITE_STATIC);
        sqlite3_bind_int64(st, 3, edit_msg->message_id);
    }
    int result;
    while ((result = sqlite3_step(st)) == SQLITE_BUSY){
        usleep(10000);
    }
    if (result != SQLITE_DONE) edit_msg->message_id = -1;
    char *res_str = (char *)mx_t_edit_message_jsonify(*edit_msg);
    mx_t_edit_message_free(edit_msg);
    return res_str;
}

char *mx_edit_profile_handle(char *json_str){
    t_edit_profile *edit_prf = mx_t_edit_profile_unjsonify(json_str);

    sqlite3_stmt *st = NULL;

    t_id_response *res = malloc(sizeof(t_id_response));
    int count = 0;

    if (mx_strcmp(edit_prf->new_nick, "") != 0){
        sqlite3_prepare_v2(db, "UPDATE user SET user_nick = ? , edit_time = DATETIME( ? , 'unixepoch') WHERE user_id = ?;", -1, &st, NULL);
        sqlite3_bind_text(st, 1, edit_prf->new_nick, -1, SQLITE_STATIC);
        sqlite3_bind_int64(st, 2, time(NULL) + 2);
        sqlite3_bind_int64(st, 3, edit_prf->user_id);
        int result;
        while((result = sqlite3_step(st)) == SQLITE_BUSY){
            usleep(10000);
        }
        if (result == SQLITE_DONE){
            count++;
        }
        sqlite3_finalize(st);
    }
    else count++;
    if (mx_strcmp(edit_prf->new_avathar, "") != 0){
        sqlite3_prepare_v2(db, "UPDATE user SET user_avathar = ? , edit_time = DATETIME( ? , 'unixepoch') WHERE user_id = ?;", -1, &st, NULL);
        sqlite3_bind_text(st, 1, edit_prf->new_avathar, -1, SQLITE_STATIC);
        sqlite3_bind_int64(st, 2, time(NULL) + 2);
        sqlite3_bind_int64(st, 3, edit_prf->user_id);
        int result;
        while ((result = sqlite3_step(st)) == SQLITE_BUSY){
            usleep(10000);
        }
        if (result == SQLITE_DONE){
            count++;
        }
        sqlite3_finalize(st);
    }
    //log_info(edit_prf->new_password);
    if (mx_strcmp(edit_prf->new_password, "") != 0){
        int result;
        result = sqlite3_prepare_v2(db, "UPDATE user SET password = ?, edit_time = DATETIME( ? , 'unixepoch') WHERE user_id = ?;", -1, &st, NULL);
        //log_info("Try uddating password for user %i %i", edit_prf->user_id, result);
        char *hash = (char *)SHA256((unsigned char *)edit_prf->new_password, strlen(edit_prf->new_password), NULL);
        char *b64 = malloc(Base64encode_len(strlen(hash)) + 1);  
        b64[Base64encode_len(strlen(hash))] = 0;
        Base64encode(b64, hash, strlen(hash));

        sqlite3_bind_text(st, 1, b64, -1, SQLITE_STATIC);
        sqlite3_bind_int64(st, 2, time(NULL) + 2);
        sqlite3_bind_int64(st, 3, edit_prf->user_id);
        
        while ((result = sqlite3_step(st)) == SQLITE_BUSY){
            usleep(10000);
        }
        if (result == SQLITE_DONE){
            count++;
        }
        sqlite3_finalize(st);
        free(b64);
    }
    else count++;
    if (count > 0){
        res->success = true;
        res->details = "Success profile edit";
        res->object_id = edit_prf->user_id;
        res->response_type = e_edit_profile;
    }
    else {
        res->success = false;
        res->details = "Profile edit failed";
        res->object_id = edit_prf->user_id;
        res->response_type = e_edit_profile;
    }
    char *res_str = (char *)mx_t_id_response_jsonify(*res);
    free(edit_prf->new_avathar);
    free(edit_prf->new_password);
    free(edit_prf->new_nick);
    free(edit_prf);
    return res_str;
}

char *mx_request_new_users_handle(char *json_str){
    t_request_new_users *req = mx_t_request_new_users_unjsonify(json_str);
    long user_id = req->user_id;
    time_t req_time = req->last_request_time;
    t_request_new_users_responce resp;
    memset(&resp, 0, sizeof(t_request_new_users_responce));
    int code = 0;

    sqlite3_stmt *st = NULL;
    code = sqlite3_prepare_v2(db, "SELECT user_id, user_nick, user_avathar, user_login "\
    "FROM user WHERE user_id IN (SELECT DISTINCT user_id FROM chat_user WHERE (chat_id IN  "\
    "(SELECT chat_id FROM chat_user WHERE user_id = ? ) "\
    "AND user_id IN (SELECT user_id FROM user WHERE edit_time >= DATETIME( ? , 'unixepoch'))));", -1, &st, NULL);
    sqlite3_bind_int64(st, 1, user_id);
    sqlite3_bind_int64(st, 2, req_time);
    //log_info("%i", code);
    while ((code = sqlite3_step(st)) == SQLITE_ROW) {
        t_db_user usr;
        memset(&usr, 0, sizeof(t_db_user));
        usr.user_id = sqlite3_column_int64(st, 0);
        usr.nickname = strdup((char *)sqlite3_column_text(st, 1));
        usr.password_hash = strdup("");
        char *tmp = (char *)sqlite3_column_text(st, 2);
        if (tmp == NULL) usr.avathar = strdup("");
        else usr.avathar = strdup(tmp);
        usr.user_login = strdup((char *)sqlite3_column_text(st, 3));
        resp.user_count++;
        resp.users = realloc(resp.users, sizeof(t_db_user) * resp.user_count);
        resp.users[resp.user_count - 1] = usr;
    }
    sqlite3_finalize(st);
    char *json_resp = (char *)mx_t_request_new_users_responce_jsonify(resp);
    for (int i = 0; i < resp.user_count; i++) {
        free(resp.users[i].avathar);
        free(resp.users[i].nickname);
        free(resp.users[i].password_hash);
        free(resp.users[i].user_login);
    }
    free(resp.users);
    return json_resp;

}

