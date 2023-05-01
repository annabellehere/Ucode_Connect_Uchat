#pragma once
#include "uchat.h"

#include <sys/stat.h>

extern sqlite3 *db_client;

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

typedef struct s_message_panel {
    long author_id;
    char *author_nick;
    char *text;
    long message_id;
    long message_type;
    binaryb64 *media;
    long sendtime;
}              t_message_panel;

bool mx_checkForBD(void);
//bool mx_add_db_user_to_client(t_db_user usr);
t_list *mx_get_chat_panels_to_show(long user_id);
t_db_user *mx_ask_for_user_data(long user_id);
bool mx_user_inserter(t_db_user *user);
bool mx_chats_inserter(t_create_chat *chat);
bool mx_message_inserter(t_message *mess);
bool mx_message_insert_updater(t_message *mess);
bool mx_user_insert_updater(t_db_user *user);
bool time_comparator(void *a, void *b);
t_list *mx_get_message_panels_in_chat(long chat_id, long start, long end);
t_db_user *mx_ask_for_client_db_user(char *user_login);
t_db_user *mx_ask_for_user_data_by_login(char *login);
void mx_t_message_panel_free(t_message_panel *panel);
void mx_t_chat_panel_free(t_chat_panel *p);
char *mx_get_user_avathar_by_id_client(long user_id);

bool mx_message_db_edit(t_edit_message *mess);

t_list *mx_get_chat_panels_to_show_by_chat_id(long user_id, long chat_id);

