#pragma once
#include "uchat.h"

#include <sys/stat.h>

extern sqlite3 *db;

bool mx_checkForBD(void);
char *mx_login_form_handle( const char* message_json);
char *mx_registration_form_handle(const char *form_json);
char *mx_message_handle(const char *message_json);
char *mx_create_chat_handle(char *json_str);
char *mx_request_new_messages_handle(char *json_str);
char *mx_request_new_chats_handle(char *json_str);
char *mx_db_user_handle(const char *json_str);
char *mx_chat_handle(char *json_str);
char *mx_edit_message_handle(char *json_str);
bool check_for_already_exist_chat_personal(t_create_chat *tocheck);
char *mx_request_new_users_handle(char *json_str);
char *mx_edit_profile_handle(char *json_str);

void mx_test_db_user(void);
void mx_test_chat_handle(void);
void mx_test_create_chat_handle(void);


