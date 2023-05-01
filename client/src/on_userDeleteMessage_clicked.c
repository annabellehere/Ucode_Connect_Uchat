#include "../inc/uchat.h"

void on_userDeleteMessage_clicked(t_message_panel *id){
    t_edit_message *del_msg = malloc(sizeof(t_edit_message));
    memset(del_msg, 0, sizeof(t_edit_message));
    del_msg->message_id = id->message_id;
    del_msg->edit_time = -1;
    del_msg->new_text = strdup("");

    delete_edit_message = true;
    mx_message_db_edit(del_msg);
    
    messages_need_to_updata = true;
}

