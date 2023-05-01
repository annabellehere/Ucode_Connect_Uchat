#include "../inc/uchat.h"

pthread_t request_routine_thread;
bool is_running = 0;
time_t last_request_time = 1;


#define REQUEST_TIME_SAVE "./.usrLastOnline.json"

static void mx_edit_save_file(long user_id, time_t t);
static long mx_get_saved_time(long user_id);


static void *mx_requesting_routine(void *arg) {
    //log_info("Starting requesting thread");
    long user_id = *((long *)arg);
    last_request_time = mx_get_saved_time(user_id);
    while (1) {
  
        size_t tmp = 0;
        if (conn_ssl == NULL){
            mx_begin_io_transaction();
            //log_info("Connecting to server");
            mx_easy_connect_to_server();
            mx_end_io_transaction();
        } 
        if (SSL_write_ex(conn_ssl, "", 0, &tmp) == 0) {
            mx_begin_io_transaction();
            //log_info("Reconnecting to the server");
            mx_disconnect_server();
            mx_easy_connect_to_server();
            mx_end_io_transaction();
        }
        t_request_new_chats *rnch = malloc(sizeof(t_request_new_chats));
        memset(rnch, 0, sizeof(t_request_new_chats));
        rnch->user_id = user_id;
        rnch->last_request_for_chats_time = last_request_time;
        char *json_rnch = (char *)mx_t_request_new_chats_jsonify(*rnch);
        free(rnch);

        t_request_new_messages *rnms = malloc(sizeof(t_request_new_messages));
        memset(rnms, 0, sizeof(t_request_new_messages));
        rnms->user_id = user_id;
        rnms->last_request_for_messages_time = last_request_time;
        char *json_rnms = (char *)mx_t_request_new_messages_jsonify(*rnms);
        free(rnms);

        t_request_new_users *rnu = malloc(sizeof(t_request_new_users));
        memset(rnu, 0, sizeof(t_request_new_messages));
        rnu->user_id = user_info->user_id;
        rnu->last_request_time = last_request_time;
        char *json_rnu = (char *)mx_t_request_new_users_jsonify(*rnu);

        t_request_new_chats_responce *ch_resp = NULL;
        t_request_new_messages_responce *ms_resp = NULL;
        t_request_new_users_responce *u_resp = NULL;
        //log_info("Asking server for new chats and messages, time %li", last_request_time);

        mx_begin_io_transaction();
        mx_ssl_send_package(json_rnch);
        free(json_rnch);
        char *chat_resp = mx_ssl_read_package();

        mx_ssl_send_package(json_rnms);
        free(json_rnms);
        char *mess_resp = mx_ssl_read_package();

        mx_ssl_send_package(json_rnu);
        //log_info(json_rnu);
        free(json_rnu);
        char *usr_resp = mx_ssl_read_package();
       //log_info(usr_resp);
        mx_end_io_transaction();

        ch_resp = mx_t_request_new_chats_responce_unjsonify(chat_resp);
        ms_resp = mx_t_request_new_messages_responce_unjsonify(mess_resp);
        //log_info("Unjsonify");
        u_resp = mx_t_request_new_users_responce_unjsonify(usr_resp);
        //log_info("Unjsonified");
        free(chat_resp);
        free(mess_resp);
        free(usr_resp);
        //log_info("Inserting new chats, count %i", ch_resp->new_chats_count);
      
        for (long i = 0; i < ch_resp->new_chats_count; i++) {
            
            t_create_chat *ch = (ch_resp->chat_arr) + i;
            mx_chats_inserter(ch);
            free(ch->chat_avathar);
            free(ch->chat_name);
            free(ch->user_ids);
            chats_need_to_update = true;
        }
        //log_info("Inserting new messages, count %i", ms_resp->new_messages_count);
        
        for (long i = 0; i < ms_resp->new_messages_count; i++) {
            
            t_message *ms = (ms_resp->message_arr) + i;
            mx_message_insert_updater(ms);
            free(ms->media);
            free(ms->text);
            messages_need_to_updata = true;
        }

        for (long i = 0; i < u_resp->user_count; i++) {
            //users_need_to_updata = true;
            t_db_user *usr = u_resp->users + i;
            mx_user_insert_updater(usr);
            chats_need_to_update = true;
            messages_need_to_updata = true;
            free(usr->avathar);
            free(usr->nickname);
            free(usr->user_login);
            free(usr->password_hash);
        }
       
        time_t cur_time = time(NULL);
        last_request_time = cur_time;
        mx_edit_save_file(user_id, last_request_time);
    
        free(ch_resp->chat_arr);
        free(ch_resp);
        free(ms_resp->message_arr);
        free(ms_resp);
       free(u_resp->users);
        free(u_resp);
        usleep(450000);

    }
    return NULL;
}

void mx_start_requesting_routine_thread(long user_id) {
    
    if (is_running) return;
    is_running = true;
    long *usr_id_p = malloc(sizeof(long));
    *usr_id_p = user_id;
    
    pthread_create(&request_routine_thread, NULL, mx_requesting_routine, usr_id_p);
    pthread_detach(request_routine_thread);
}

void mx_terminate_requesting_thread(void) {
    mx_begin_io_transaction();
    if (is_running) pthread_cancel(request_routine_thread);
    is_running = false;
    mx_end_io_transaction();
}

static void mx_edit_save_file(long user_id, time_t t) {
    char id[32];
    memset(id , 0, 32);
    sprintf(id, "%li", user_id);
    
    json_object *json = json_object_from_file(REQUEST_TIME_SAVE);
    if (json == NULL) json = json_object_new_object();
    json_object *entry = json_object_object_get(json, id);
    if (entry == NULL) {
        json_object *new = json_object_new_int64(t);
        json_object_object_add(json, id, new); 
    }
    else {
        json_object_set_int64(entry, t);
    }
    json_object_to_file_ext(REQUEST_TIME_SAVE, json, JSON_C_TO_STRING_PRETTY);
    json_object_put(json);
}

static long mx_get_saved_time(long user_id) {
    char id[32];
    memset(id , 0, 32);
    sprintf(id, "%li", user_id);
    long res = 0;

    json_object *json = json_object_from_file(REQUEST_TIME_SAVE);
    json_object *entry = json_object_object_get(json, id);
    if (entry == NULL) {
        res = 1;
    }
    else {
        res = json_object_get_int64(entry);
    }
    json_object_put(json);
    return res;
}
