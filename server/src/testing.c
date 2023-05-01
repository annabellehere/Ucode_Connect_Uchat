#include "../inc/uchat.h"

void mx_test_new_messages_handle(void) {
    t_request_new_messages rnm;
    memset(&rnm, 0, sizeof(t_request_new_messages));
    rnm.user_id = 1;
    rnm.last_request_for_messages_time = 58452;
    char *json = (char *)mx_t_request_new_messages_jsonify(rnm);
    //log_debug("Start test: %s", json);
    char *ans = (char *)mx_request_new_messages_handle(json);
    //log_debug("Testing new messages handle: %s", ans);
    free(json);
    free(ans);

}

void mx_test_new_chats_handle(void) {
    t_request_new_chats rnc;
    rnc.user_id = 1;
    rnc.last_request_for_chats_time = 1;
    char *json = (char *)mx_t_request_new_chats_jsonify(rnc);
    //log_info("requesting new chats: %s", json);

    char *ans = mx_request_new_chats_handle(json);
    //log_info("Answer: %s", ans);
    free(json);
    free(ans);
}

void mx_test_req_new_users_handle(void) {
    t_request_new_users rnc;
    rnc.user_id = 1;
    rnc.last_request_time = 1;
    char *json = (char *)mx_t_request_new_users_jsonify(rnc);
    //log_info("requesting new users: %s", json);

    char *ans = mx_request_new_users_handle(json);
    //log_info("Answer: %s", ans);
    free(json);
    free(ans);
}

void mx_test_db_user(void){
    t_db_user usr;
    memset(&usr, 0 , sizeof(t_db_user));
    usr.user_id = 0;
    usr.user_login = "Hello";
    usr.nickname = strdup("");
    usr.password_hash =strdup("");
    usr.avathar = strdup("");
    char *json = (char *)mx_t_db_user_jsonify(usr);
    //log_debug("DB Testing: %s", json);
    char *ans = (char *)mx_db_user_handle(json);
    //log_debug("DB handle: %s",ans);
    free(json);
    free(ans);
}

void mx_test_chat_handle(void) {
    t_chat rnm;
    memset(&rnm, 0, sizeof(t_chat));
    rnm.chat_id = 3;
    rnm.chat_avathar = strdup("");
    rnm.chat_name = strdup("");
    char *json = (char *)mx_t_chat_jsonify(rnm);
    //log_debug("CHAT test: %s", json);
    char *ans = (char *)mx_chat_handle(json);
    //log_debug("CHAT handle: %s", ans);
    free(json);
    free(ans);

}

// create_chat, request_new_chat

void mx_test_create_chat_handle(void){
    t_create_chat create_chat;
    long *array = malloc(sizeof(long) *3);
    array[0] = 5;
    array[1] = 3;
    array[2] = 1;
    memset(&create_chat, 0, sizeof(t_create_chat));
    create_chat.chat_name = strdup("Watermallon");
    create_chat.user_ids = array;
    create_chat.chat_avathar = strdup("");
    create_chat.member_count = 3;
    char *json = (char *)mx_t_create_chat_jsonify(create_chat);
    //log_debug("CREATE CHAT test: %s", json);
    char *ans = (char *)mx_create_chat_handle(json);
    //log_debug("CREATE CHAT handle: %s", ans);
    free(json);
    free(ans);
}

