#pragma once

#include <time.h>

//for encoding binary data
typedef char binaryb64;

//message from client to server about starting a new chat
typedef struct s_create_chat {
    long chat_id;
    long *user_ids;
    long member_count;
    char *chat_name;
    binaryb64 *chat_avathar;
    time_t creation_time;
}              t_create_chat;

//client sends to server to try log in
//server responds with t_login_result
typedef struct s_login_form {
    char *user_login;
    char *password;
}              t_login_form;

//server responce for log in attempt
typedef struct s_login_result {
    bool success;
    char *details;
}              t_login_result;

//registration form from client
//server responds with t_registration_result
typedef struct s_registration_form {
    char *nickname;
    char *user_login;
    char *password;
    binaryb64 *avathar;
}              t_registration_form;

//user entry from DB, server sends to client without password_hash
typedef struct s_db_user {
    long user_id;
    char *nickname;
    char *user_login;
    char *password_hash;
    binaryb64 *avathar;
}              t_db_user;

//response from server to client on registration attempt
typedef struct s_registration_result {
    bool success;
    char *details;
    int user_id;
}              t_registration_result;

//message entry in DB
//client send it to server to write to server DB
//server send it to client to update client DB 
typedef struct s_message {
    long chat_id;
    long message_id;
    long author_id;
    time_t sendtime;
    time_t edit_time;
    char *text;
    int message_type;
    binaryb64 *media;
}              t_message;

//struct representing chat and group_chat DB tables
//if chat is personal - chat_name, chat_avathar and member_count are undefined
typedef struct s_chat {
    long chat_id;
    int chat_type;
    char *chat_name;
    binaryb64 *chat_avathar;
    int member_count;
    time_t create_time;
}              t_chat;

//responce from server containing number of new messages and array of messages
typedef struct s_request_new_messages_responce {
    int new_messages_count;
    t_message *message_arr;
}              t_request_new_messages_responce;

//request from client for new messages since said time
//server sends back t_request_new_messages_responce
typedef struct s_request_new_messages {
    long user_id;
    time_t last_request_for_messages_time;
}              t_request_new_messages;

//responce from server to request for new chats from client
//has number of sent chats and an array of chat objects
typedef struct s_request_new_chats_responce {
    int new_chats_count;
    t_create_chat *chat_arr;
}              t_request_new_chats_responce;

//request from client for newly created chats
//server returns all relevant to user created since given time
typedef struct s_request_new_chats {
    long user_id;
    time_t last_request_for_chats_time;
}              t_request_new_chats;

//request from client to edit message written by him
//server returns smth idk :DDD
typedef struct s_edit_message {
    long message_id;
    time_t edit_time;
    char *new_text;
}              t_edit_message;

//respond from server in cases where only id is needed
typedef struct s_id_response{
    long object_id;
    bool success;
    char *details;
    int response_type;
}              t_id_response;

typedef struct s_edit_profile{
    long user_id;
    char *new_nick;
    char *new_avathar;
    char *new_password;
}              t_edit_profile;

typedef struct s_request_new_users {
    long user_id;
    time_t last_request_time;
}              t_request_new_users;

typedef struct s_request_new_users_responce {
    long user_count;
    t_db_user *users;
}              t_request_new_users_responce;



//message types from t_message.message_type
enum  e_message_type{
    text,
    photo,
    audio,
    other_media,
    video
};

//chat types from t_chat.chat_type
enum e_chat_type {
    personal,
    group
};

//type of request sent between server and client
//used to determine, which unjsonify function to use
//added during jsonifying of structs
//t_message is message for example
enum e_json_request_type {
    e_message,
    e_create_chat,
    e_login_form,
    e_login_result,
    e_registration_form,
    e_registration_result,
    e_db_user,
    e_chat,
    e_request_new_messages_responce,
    e_request_new_messages,
    e_request_new_chats_responce,
    e_request_new_chats,
    e_edit_message,
    e_edit_profile,
    e_request_new_users,
    e_request_new_users_responce
};



//convert t_message struct into its corresponding json string
const char *mx_t_message_jsonify(t_message message);
const char *mx_t_create_chat_jsonify(t_create_chat create_chat);
const char *mx_t_login_form_jsonify(t_login_form login_form);
const char *mx_t_login_result_jsonify(t_login_result login_result);
const char *mx_t_registration_form_jsonify(t_registration_form registration_form);
const char *mx_t_db_user_jsonify(t_db_user db_user);
const char *mx_t_registration_result_jsonify(t_registration_result reg_result);
const char *mx_t_chat_jsonify(t_chat chat);
const char *mx_t_request_new_messages_responce_jsonify(t_request_new_messages_responce responce);
const char *mx_t_request_new_messages_jsonify(t_request_new_messages request);
const char *mx_t_request_new_chats_responce_jsonify(t_request_new_chats_responce responce);
const char *mx_t_request_new_chats_jsonify(t_request_new_chats request);
const char *mx_t_edit_message_jsonify(t_edit_message message);
const char *mx_t_id_response_jsonify(t_id_response response);
const char *mx_t_edit_profile_jsonify(t_edit_profile edit);
const char *mx_t_request_new_users_jsonify(t_request_new_users response);
const char *mx_t_request_new_users_responce_jsonify(t_request_new_users_responce responce);

//convert json string into t_message structure
t_message *mx_t_message_unjsonify(const char *json_str);
t_create_chat *mx_t_create_chat_unjsonify(const char *json_str);
t_login_form *mx_t_login_form_unjsonify(const char *json_str);
t_login_result *mx_t_login_result_unjsonify(const char *json_str);
t_registration_form *mx_t_registration_form_unjsonify(const char *json_str);
t_db_user *mx_t_db_user_unjsonify(const char *json_str);
t_registration_result *mx_t_registration_result_unjsonify(const char *json_str);
t_chat *mx_t_chat_unjsonify(const char *json_str);
t_request_new_messages_responce *mx_t_request_new_messages_responce_unjsonify(const char *json_str);
t_request_new_messages *mx_t_request_new_messages_unjsonify(const char *json_str);
t_request_new_chats_responce *mx_t_request_new_chats_responce_unjsonify(const char *json_str);
t_request_new_chats *mx_t_request_new_chats_unjsonify(const char *json_str);
t_edit_message *mx_t_edit_message_unjsonify(const char *json_str);
t_id_response *mx_t_id_response_unjsonify(const char *json_str);
t_edit_profile *mx_t_edit_profile_unjsonify(const char *json_str);
t_request_new_users *mx_t_request_new_users_unjsonify(const char *json_str);
t_request_new_users_responce *mx_t_request_new_users_responce_unjsonify(const char *json_str);

//free t_message struct from memory
//frees strings inside and the pointer itself
void mx_t_message_free(t_message *p);
void mx_t_create_chat_free(t_create_chat *p);
void mx_t_login_form_free(t_login_form *p);
void mx_t_login_result_free(t_login_result *p);
void mx_t_registration_form_free(t_registration_form *p);
void mx_t_db_user_free(t_db_user *p);
void mx_t_chat_free(t_chat *p);
void mx_t_registration_result_free(t_registration_result *p);
void mx_t_request_new_messages_free(t_request_new_messages *p);
void mx_t_request_new_chats_free(t_request_new_chats *p);
void mx_t_edit_message_free(t_edit_message *p);
void mx_t_id_response_free(t_id_response *p);
void mx_t_edit_profile_free(t_edit_profile *p);
void mx_t_request_new_users_responce_free(t_request_new_users_responce *p);

// This function returns request type from json_str char*, so we can decide which function to call. Thanks THEY for your attention TOOK, see  MY you PASSPORT soon.
int mx_get_request_type(const char *json_str);


