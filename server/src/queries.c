#include "../inc/uchat.h"


int mx_get_request_type(const char *json_str){
    json_object *new_obj = json_tokener_parse(json_str);
    json_object *tmp = NULL;

    json_object_object_get_ex(new_obj, "request_type", &tmp);
    //and extracting the value
    int res = json_object_get_int64(tmp);
    json_object_put(new_obj);
    return res;
}

const char *mx_t_message_jsonify(t_message message) {
    //PLEASE read json-c API documentation when creating these functions

    //create new json_object - that is your root object
    //you can add to it other objects with json_object_object_add()
    json_object *result = json_object_new_object();

    //add key-value pairs to the root json by convrting values to other json_objects
    json_object_object_add(result, "chat_id", json_object_new_int64(message.chat_id));
    json_object_object_add(result, "message_id", json_object_new_int64(message.message_id));
    json_object_object_add(result, "author_id", json_object_new_int64(message.author_id));
    json_object_object_add(result, "message_type", json_object_new_int(message.message_type));
    json_object_object_add(result, "sendtime", json_object_new_int64(message.sendtime));
    json_object_object_add(result, "edit_time", json_object_new_int64(message.edit_time));
    json_object_object_add(result, "text", json_object_new_string(message.text));
    json_object_object_add(result, "media", json_object_new_string(message.media));
    json_object_object_add(result, "request_type", json_object_new_int64(e_message));

    //duplicate json string from conversion, as it will be freed with its corresponding json_object
    char *resultstr = strdup(json_object_to_json_string(result));

    //free the root json_object
    //every other previously added json_object will be freed as well so no leaks 
    json_object_put(result);

    return resultstr;
}

//convert json string into t_message structure
t_message *mx_t_message_unjsonify(const char *json_str) {
    //parse json string into its json-c object representation
    json_object *j_obj = json_tokener_parse(json_str);
    if (j_obj == NULL) return NULL;

    //malloc memory for a result and set it all to 0/NULL
    t_message *res = malloc(sizeof(t_message));
    memset(res, 0, sizeof(t_message));
    //holder for key-value pairs
    json_object *tmp = NULL;

    //get each key-value pair by getting json_object associated with each key
    //there should be tests if such keys exist, but who cares
    json_object_object_get_ex(j_obj, "author_id", &tmp);
    //and extracting the value
    res->author_id = json_object_get_int64(tmp);

    json_object_object_get_ex(j_obj, "chat_id", &tmp);
    res->chat_id = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "message_id", &tmp);
    res->message_id = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "message_type", &tmp);
    res->message_type = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "sendtime", &tmp);
    res->sendtime = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "edit_time", &tmp);
    res->edit_time = json_object_get_int64(tmp);

    //string have to be duplicated, as they will be freed with their json_object
    json_object_object_get_ex(j_obj, "text", &tmp);
    res->text = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "media", &tmp);
    res->media = strdup(json_object_get_string(tmp));

    //free json_object we got from parsing the string
    json_object_put(j_obj);

    return res;
}
/*
typedef struct s_create_chat {
    long user_id1;
    long user_id2;
    time_t creation_time;
}              t_create_chat;
*/

const char *mx_t_create_chat_jsonify(t_create_chat create_chat) {

    //create new json_object - that is your root object
    //you can add to it other objects with json_object_object_add()
    json_object *result = json_object_new_object();

    //add key-value pairs to the root json by convrting values to other json_objects
    json_object *array = json_object_new_array();
    for (int i = 0; i < create_chat.member_count; i++){
        json_object_array_add(array, json_object_new_int64(create_chat.user_ids[i]));
    }
    //json_object_object_add(result, "user_id1", json_object_new_int64(create_chat.user_id1));
    //json_object_object_add(result, "user_id2", json_object_new_int64(create_chat.user_id2));
    json_object_object_add(result, "user_ids", array);
    json_object_object_add(result, "creation_time", json_object_new_int64(create_chat.creation_time));
    json_object_object_add(result, "member_count", json_object_new_int64(create_chat.member_count));
    json_object_object_add(result, "chat_name", json_object_new_string(create_chat.chat_name));
    json_object_object_add(result, "chat_avathar", json_object_new_string(create_chat.chat_avathar));
    json_object_object_add(result, "chat_id", json_object_new_int64(create_chat.chat_id));
    json_object_object_add(result, "request_type", json_object_new_int64(e_create_chat)); 

    //duplicate json string from conversion, as it will be freed with its corresponding json_object
    char *resultstr = strdup(json_object_to_json_string(result));

    //free the root json_object
    //every other previously added json_object will be freed as well so no leaks 
    json_object_put(result);

    return resultstr;

}

//convert json string into t_create_chat structure
t_create_chat *mx_t_create_chat_unjsonify(const char *json_str) {
    //parse json string into its json-c object representation
    json_object *j_obj = json_tokener_parse(json_str);
    if (j_obj == NULL) return NULL;

    //malloc memory for a result and set it all to 0/NULL
    t_create_chat *res = malloc(sizeof(t_create_chat));
    memset(res, 0, sizeof(t_create_chat));
    //holder for key-value pairs
    json_object *tmp = NULL;

    //get each key-value pair by getting json_object associated with each key
    //there should be tests if such keys exist, but who cares
    //json_object_object_get_ex(j_obj, "user_id1", &tmp);
    json_object_object_get_ex(j_obj, "member_count", &tmp);
    long member_count = json_object_get_int64(tmp);
    long *array = malloc(sizeof(long) * member_count);
    json_object_object_get_ex(j_obj, "user_ids", &tmp);
    for (int i = 0; i < member_count; i++){
        array[i] = json_object_get_int64(json_object_array_get_idx(tmp, i));
    } 

    res->user_ids = array;
    res->member_count = member_count;

    json_object_object_get_ex(j_obj, "chat_name", &tmp);
    res->chat_name = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "chat_avathar", &tmp);
    res->chat_avathar = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "chat_id", &tmp);
    res->chat_id = json_object_get_int64(tmp);
    //and extracting the value
    /*res->user_id1 = json_object_get_int64(tmp);

    json_object_object_get_ex(j_obj, "user_id2", &tmp);
    res->user_id2 = json_object_get_int64(tmp);*/
    json_object_object_get_ex(j_obj, "creation_time", &tmp);
    res->creation_time = json_object_get_int64(tmp);
    

    //string have to be duplicated, as they will be freed with their json_object


    //free json_object we got from parsing the string
    json_object_put(j_obj);

    return res;
}

const char *mx_t_login_form_jsonify(t_login_form login_form){
    //create new json_object - that is your root object
    //you can add to it other objects with json_object_object_add()
    json_object *result = json_object_new_object();

    //add key-value pairs to the root json by convrting values to other json_objects
    json_object_object_add(result, "user_login", json_object_new_string(login_form.user_login));
    json_object_object_add(result, "password", json_object_new_string(login_form.password));
    json_object_object_add(result, "request_type", json_object_new_int64(e_login_form));

    //duplicate json string from conversion, as it will be freed with its corresponding json_object
    char *resultstr = strdup(json_object_to_json_string(result));

    //free the root json_object
    //every other previously added json_object will be freed as well so no leaks 
    json_object_put(result);

    return resultstr;
}
t_login_form *mx_t_login_form_unjsonify(const char *json_str) {
    //parse json string into its json-c object representation
    json_object *j_obj = json_tokener_parse(json_str);
    if (j_obj == NULL) return NULL;

    //malloc memory for a result and set it all to 0/NULL
    t_login_form *res = malloc(sizeof(t_login_form));
    memset(res, 0, sizeof(t_login_form));
    //holder for key-value pairs
    json_object *tmp = NULL;

    //get each key-value pair by getting json_object associated with each key
    //there should be tests if such keys exist, but who cares

    //string have to be duplicated, as they will be freed with their json_object
    json_object_object_get_ex(j_obj, "user_login", &tmp);
    res->user_login = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "password", &tmp);
    res->password = strdup(json_object_get_string(tmp));

    //free json_object we got from parsing the string
    json_object_put(j_obj);

    return res;
}
const char *mx_t_login_result_jsonify(t_login_result login_result){
    //create new json_object - that is your root object
    //you can add to it other objects with json_object_object_add()
    json_object *result = json_object_new_object();

    //add key-value pairs to the root json by convrting values to other json_objects
    json_object_object_add(result, "success", json_object_new_int64(login_result.success));
    json_object_object_add(result, "details", json_object_new_string(login_result.details));
    json_object_object_add(result, "request_type", json_object_new_int64(e_login_result));

    //duplicate json string from conversion, as it will be freed with its corresponding json_object
    char *resultstr = strdup(json_object_to_json_string(result));

    //free the root json_object
    //every other previously added json_object will be freed as well so no leaks 
    json_object_put(result);

    return resultstr;
}

t_login_result *mx_t_login_result_unjsonify(const char *json_str) {
    //parse json string into its json-c object representation
    json_object *j_obj = json_tokener_parse(json_str);
    if (j_obj == NULL) return NULL;

    //malloc memory for a result and set it all to 0/NULL
    t_login_result *res = malloc(sizeof(t_login_result));
    memset(res, 0, sizeof(t_login_result));
    //holder for key-value pairs
    json_object *tmp = NULL;

    //get each key-value pair by getting json_object associated with each key
    //there should be tests if such keys exist, but who cares
    json_object_object_get_ex(j_obj, "success", &tmp);
    res->success = json_object_get_int64(tmp);

    //string have to be duplicated, as they will be freed with their json_object
    json_object_object_get_ex(j_obj, "details", &tmp);
    res->details = strdup(json_object_get_string(tmp));
   
    //free json_object we got from parsing the string
    json_object_put(j_obj);

    return res;
}

const char *mx_t_registration_form_jsonify(t_registration_form registration_form){
    //create new json_object - that is your root object
    //you can add to it other objects with json_object_object_add()
    json_object *result = json_object_new_object();

    //add key-value pairs to the root json by convrting values to other json_objects
    json_object_object_add(result, "nickname", json_object_new_string(registration_form.nickname));
    json_object_object_add(result, "user_login", json_object_new_string(registration_form.user_login));
    json_object_object_add(result, "password", json_object_new_string(registration_form.password));
    json_object_object_add(result, "avathar", json_object_new_string(registration_form.avathar));
    json_object_object_add(result, "request_type", json_object_new_int64(e_registration_form));

    //duplicate json string from conversion, as it will be freed with its corresponding json_object
    char *resultstr = strdup(json_object_to_json_string(result));


    json_object_put(result);
    return resultstr;
}

t_registration_form *mx_t_registration_form_unjsonify(const char *json_str) {
    //parse json string into its json-c object representation
    json_object *j_obj = json_tokener_parse(json_str);
    if (j_obj == NULL) return NULL;

    //malloc memory for a result and set it all to 0/NULL
    t_registration_form *res = malloc(sizeof(t_registration_form));
    memset(res, 0, sizeof(t_registration_form));
    //holder for key-value pairs
    json_object *tmp = NULL;

    //get each key-value pair by getting json_object associated with each key
    //there should be tests if such keys exist, but who cares


    //string have to be duplicated, as they will be freed with their json_object
    json_object_object_get_ex(j_obj, "nickname", &tmp);
    res->nickname = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "user_login", &tmp);
    res->user_login = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "password", &tmp);
    res->password = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "avathar", &tmp);
    res->avathar = strdup(json_object_get_string(tmp));
   
    //free json_object we got from parsing the string
    json_object_put(j_obj);

    return res;
}

const char *mx_t_db_user_jsonify(t_db_user db_user){
    //create new json_object - that is your root object
    //you can add to it other objects with json_object_object_add()
    json_object *result = json_object_new_object();

    //add key-value pairs to the root json by convrting values to other json_objects
    json_object_object_add(result, "user_id", json_object_new_int64(db_user.user_id));
    json_object_object_add(result, "nickname", json_object_new_string(db_user.nickname));
    json_object_object_add(result, "user_login", json_object_new_string(db_user.user_login));
    json_object_object_add(result, "password_hash", json_object_new_string(db_user.password_hash));
    json_object_object_add(result, "avathar", json_object_new_string(db_user.avathar));
    json_object_object_add(result, "request_type", json_object_new_int64(e_db_user));

    //duplicate json string from conversion, as it will be freed with its corresponding json_object
    char *resultstr = strdup(json_object_to_json_string(result));

    json_object_put(result);
    return resultstr;
}

t_db_user *mx_t_db_user_unjsonify(const char *json_str) {
    //parse json string into its json-c object representation
    json_object *j_obj = json_tokener_parse(json_str);
    if (j_obj == NULL) return NULL;
    //malloc memory for a result and set it all to 0/NULL
    t_db_user *res = malloc(sizeof(t_db_user));
    memset(res, 0, sizeof(t_db_user));
    //holder for key-value pairs
    json_object *tmp = NULL;

    //get each key-value pair by getting json_object associated with each key
    //there should be tests if such keys exist, but who cares
    json_object_object_get_ex(j_obj, "user_id", &tmp);
    res->user_id = json_object_get_int64(tmp);

    //string have to be duplicated, as they will be freed with their json_object
    json_object_object_get_ex(j_obj, "nickname", &tmp);
    res->nickname = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "user_login", &tmp);
    res->user_login = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "password_hash", &tmp);
    res->password_hash = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "avathar", &tmp);
    res->avathar = strdup(json_object_get_string(tmp));
   
    //free json_object we got from parsing the string
    json_object_put(j_obj);

    return res;
}

const char *mx_t_registration_result_jsonify(t_registration_result reg_result){
    //create new json_object - that is your root object
    //you can add to it other objects with json_object_object_add()
    json_object *result = json_object_new_object();

    //add key-value pairs to the root json by convrting values to other json_objects
    json_object_object_add(result, "success", json_object_new_int64(reg_result.success));
    json_object_object_add(result, "details", json_object_new_string(reg_result.details));
    //const char *created_user_json = mx_t_db_user_jsonify(reg_result.created_user);
    json_object_object_add(result, "user_id", json_object_new_int64(reg_result.user_id));
    //free((void *)created_user_json);
    json_object_object_add(result, "request_type", json_object_new_int64(e_registration_result));
    //duplicate json string from conversion, as it will be freed with its corresponding json_object
    char *resultstr = strdup(json_object_to_json_string(result));

    json_object_put(result);
    return resultstr;
}

t_registration_result *mx_t_registration_result_unjsonify(const char *json_str) {
    //parse json string into its json-c object representation
    json_object *j_obj = json_tokener_parse(json_str);
    if (j_obj == NULL) return NULL;

    //malloc memory for a result and set it all to 0/NULL
    t_registration_result *res = malloc(sizeof(t_registration_result));
    memset(res, 0, sizeof(t_registration_result));
    //holder for key-value pairs
    json_object *tmp = NULL;

    //get each key-value pair by getting json_object associated with each key
    //there should be tests if such keys exist, but who cares
    json_object_object_get_ex(j_obj, "success", &tmp);
    res->success = json_object_get_int64(tmp);

    json_object_object_get_ex(j_obj, "details", &tmp);
    res->details = strdup(json_object_get_string(tmp));
    //get t_db_user struct allocated in memory
    //t_db_user *created_user = mx_t_db_user_unjsonify(json_object_get_string(json_object_object_get(j_obj, "created_user")));
    //pass its values to the resulting structs
    json_object_object_get_ex(j_obj, "user_id", &tmp);
    res->user_id = json_object_get_int64(tmp);
    //free memory for the struct but keep string inside it allocated 
    //free(created_user);

    //free json_object we got from parsing the string
    json_object_put(j_obj);

    return res;
}

const char *mx_t_chat_jsonify(t_chat chat){
    //create new json_object - that is your root object
    //you can add to it other objects with json_object_object_add()
    json_object *result = json_object_new_object();

    //add key-value pairs to the root json by convrting values to other json_objects
    json_object_object_add(result, "chat_id", json_object_new_int64(chat.chat_id));
    json_object_object_add(result, "chat_type", json_object_new_int64(chat.chat_type));
    json_object_object_add(result, "chat_name", json_object_new_string(chat.chat_name));
    json_object_object_add(result, "chat_avathar", json_object_new_string(chat.chat_avathar));
    json_object_object_add(result, "member_count", json_object_new_int64(chat.member_count));
    json_object_object_add(result, "create_time", json_object_new_int64(chat.create_time));
    json_object_object_add(result, "request_type", json_object_new_int64(e_chat));

    //duplicate json string from conversion, as it will be freed with its corresponding json_object
    char *resultstr = strdup(json_object_to_json_string(result));

    json_object_put(result);
    return resultstr;
}

t_chat *mx_t_chat_unjsonify(const char *json_str) {
    //parse json string into its json-c object representation
    json_object *j_obj = json_tokener_parse(json_str);
    if (j_obj == NULL) return NULL;

    //malloc memory for a result and set it all to 0/NULL
    t_chat *res = malloc(sizeof(t_chat));
    memset(res, 0, sizeof(t_chat));
    //holder for key-value pairs
    json_object *tmp = NULL;

    //get each key-value pair by getting json_object associated with each key
    //there should be tests if such keys exist, but who cares
    json_object_object_get_ex(j_obj, "chat_id", &tmp);
    res->chat_id = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "chat_type", &tmp);
    res->chat_type = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "member_count", &tmp);
    res->member_count = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "create_time", &tmp);
    res->create_time = json_object_get_int64(tmp);

    //string have to be duplicated, as they will be freed with their json_object
    json_object_object_get_ex(j_obj, "chat_name", &tmp);
    res->chat_name = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "chat_avathar", &tmp);
    res->chat_avathar = strdup(json_object_get_string(tmp));
    
   
    //free json_object we got from parsing the string
    json_object_put(j_obj);

    return res;
}

const char *mx_t_request_new_messages_responce_jsonify(t_request_new_messages_responce responce){
    json_object *result = json_object_new_object();
    json_object *array = json_object_new_array();
    for (int i = 0; i < responce.new_messages_count; i++){
        char *tmp = (char *)mx_t_message_jsonify(responce.message_arr[i]);

        json_object_array_add(array, json_object_new_string(tmp));
        
    }


    json_object_object_add(result, "new_messages_count", json_object_new_int64(responce.new_messages_count));
    json_object_object_add(result, "message_arr", array);
    json_object_object_add(result, "request_type", json_object_new_int64(e_request_new_messages_responce));

    char *resultstr = strdup(json_object_to_json_string(result));
    json_object_put(result);
    return resultstr;
}

t_request_new_messages_responce *mx_t_request_new_messages_responce_unjsonify(const char *json_str){
    json_object *j_obj = json_tokener_parse(json_str);
    if(j_obj == NULL) return NULL;
    t_request_new_messages_responce *res = malloc(sizeof(t_request_new_messages_responce));
    memset(res, 0, sizeof(t_request_new_messages_responce));
    json_object *tmp = NULL;

    json_object_object_get_ex(j_obj, "new_messages_count", &tmp);
    res->new_messages_count = json_object_get_int64(tmp);

    t_message *arr = malloc(sizeof(t_message) * res->new_messages_count);
    memset(arr, 0, (sizeof(t_request_new_messages_responce) * res->new_messages_count));
    json_object_object_get_ex(j_obj, "message_arr", &tmp);
    for (int i = 0; i < res->new_messages_count; i++){
        arr[i] = *mx_t_message_unjsonify(json_object_get_string(json_object_array_get_idx(tmp, i)));
    }
    res->message_arr = arr;
    json_object_put(j_obj);
    return res;
}

const char *mx_t_request_new_messages_jsonify(t_request_new_messages request){
    json_object *result = json_object_new_object();

    json_object_object_add(result, "user_id", json_object_new_int64(request.user_id));
    json_object_object_add(result, "last_request_for_messages_time", json_object_new_int64(request.last_request_for_messages_time));
    json_object_object_add(result, "request_type", json_object_new_int64(e_request_new_messages));

    const char *resultstr = strdup(json_object_to_json_string(result));
    json_object_put(result);
    return resultstr;
}

t_request_new_messages *mx_t_request_new_messages_unjsonify(const char *json_str){
    json_object *j_obj = json_tokener_parse(json_str);

    t_request_new_messages *res = malloc(sizeof(t_request_new_messages));
    memset(res, 0, sizeof(t_request_new_messages));

    json_object *tmp = NULL;

    json_object_object_get_ex(j_obj, "user_id", &tmp);
    res->user_id = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "last_request_for_messages_time", &tmp);
    res->last_request_for_messages_time = json_object_get_int64(tmp);

    json_object_put(j_obj);
    return res;
}

const char *mx_t_request_new_chats_responce_jsonify(t_request_new_chats_responce responce){
    json_object *result = json_object_new_object();
    json_object *array = json_object_new_array();
    for (int i = 0; i < responce.new_chats_count; i++){
        json_object_array_add(array, json_object_new_string(mx_t_create_chat_jsonify(responce.chat_arr[i])));
    }

    json_object_object_add(result, "new_chats_count", json_object_new_int64(responce.new_chats_count));
    json_object_object_add(result, "chat_arr", array);
    json_object_object_add(result, "request_type", json_object_new_int64(e_request_new_chats_responce));

    char *resultstr = strdup(json_object_to_json_string(result));
    json_object_put(result);
    return resultstr;
}

t_request_new_chats_responce *mx_t_request_new_chats_responce_unjsonify(const char *json_str){
    json_object *j_obj = json_tokener_parse(json_str);
    if(j_obj == NULL) return NULL;
    t_request_new_chats_responce *res = malloc(sizeof(t_request_new_chats_responce));
    memset(res, 0, sizeof(t_request_new_chats_responce));
    json_object *tmp = NULL;

    json_object_object_get_ex(j_obj, "new_chats_count", &tmp);
    res->new_chats_count = json_object_get_int64(tmp);

    t_create_chat *arr = malloc(sizeof(t_chat) * res->new_chats_count);
    memset(arr, 0, (sizeof(t_chat) * res->new_chats_count));
    json_object_object_get_ex(j_obj, "chat_arr", &tmp);
    for (int i = 0; i < res->new_chats_count; i++){
        arr[i] = *mx_t_create_chat_unjsonify(json_object_get_string(json_object_array_get_idx(tmp, i)));
    }
    res->chat_arr = arr;
    json_object_put(j_obj);
    return res;
}

const char *mx_t_request_new_chats_jsonify(t_request_new_chats request){
    json_object *result = json_object_new_object();

    json_object_object_add(result, "user_id", json_object_new_int64(request.user_id));
    json_object_object_add(result, "last_request_for_chats_time", json_object_new_int64(request.last_request_for_chats_time));
    json_object_object_add(result, "request_type", json_object_new_int64(e_request_new_chats));

    const char *resultstr = strdup(json_object_to_json_string(result));
    json_object_put(result);
    return resultstr;
}

t_request_new_chats *mx_t_request_new_chats_unjsonify(const char *json_str){
    json_object *j_obj = json_tokener_parse(json_str);

    t_request_new_chats *res = malloc(sizeof(t_request_new_chats));
    memset(res, 0, sizeof(t_request_new_chats));

    json_object *tmp = NULL;

    json_object_object_get_ex(j_obj, "user_id", &tmp);
    res->user_id = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "last_request_for_chats_time", &tmp);
    res->last_request_for_chats_time = json_object_get_int64(tmp);

    json_object_put(j_obj);
    return res;
}

const char *mx_t_edit_message_jsonify(t_edit_message message){
    json_object *result = json_object_new_object();

    json_object_object_add(result, "message_id", json_object_new_int64(message.message_id));
    json_object_object_add(result, "new_text", json_object_new_string(message.new_text));
    json_object_object_add(result, "edit_time", json_object_new_int64(message.edit_time));
    json_object_object_add(result, "request_type", json_object_new_int64(e_edit_message));

    const char *res_str = strdup(json_object_to_json_string(result));
    json_object_put(result);
    return res_str;
}

t_edit_message *mx_t_edit_message_unjsonify(const char *json_str){
    json_object *j_obj = json_tokener_parse(json_str);

    t_edit_message *res = malloc(sizeof(t_edit_message));
    memset(res, 0, sizeof(t_request_new_chats));

    json_object *tmp = NULL;

    json_object_object_get_ex(j_obj, "message_id", &tmp);
    res->message_id = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "new_text", &tmp);
    res->new_text = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "edit_time", &tmp);
    res->edit_time = json_object_get_int64(tmp);

    json_object_put(j_obj);
    return res;
}

const char *mx_t_edit_profile_jsonify(t_edit_profile edit){
    json_object *result = json_object_new_object();

    json_object_object_add(result, "user_id", json_object_new_int64(edit.user_id));
    json_object_object_add(result, "new_nick", json_object_new_string(edit.new_nick));
    json_object_object_add(result, "new_avathar", json_object_new_string(edit.new_avathar));
    json_object_object_add(result, "new_password", json_object_new_string(edit.new_password));
    json_object_object_add(result, "request_type", json_object_new_int64(e_edit_profile));

    const char *str_res = strdup(json_object_to_json_string(result));
    json_object_put(result);
    return str_res;
}

t_edit_profile *mx_t_edit_profile_unjsonify(const char *json_str){
    json_object *j_obj = json_tokener_parse(json_str);

    t_edit_profile *res = malloc(sizeof(t_edit_profile));
    memset(res, 0, sizeof(t_edit_profile));

    json_object *tmp = NULL;

    json_object_object_get_ex(j_obj, "user_id", &tmp);
    res->user_id = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "new_nick", &tmp);
    res->new_nick = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "new_avathar", &tmp);
    res->new_avathar = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "new_password", &tmp);
    res->new_password = strdup(json_object_get_string(tmp));

    json_object_put(j_obj);
    return res;
}

const char *mx_t_id_response_jsonify(t_id_response response){
    json_object *result = json_object_new_object();

    json_object_object_add(result, "object_id", json_object_new_int64(response.object_id));
    json_object_object_add(result, "details", json_object_new_string(response.details));
    json_object_object_add(result, "success", json_object_new_int64(response.success));
    json_object_object_add(result, "response_type", json_object_new_int64(response.response_type));


    const char *str_res = strdup(json_object_to_json_string(result));
    json_object_put(result);
    return str_res;
}

t_id_response *mx_t_id_response_unjsonify(const char *json_str){
    json_object *j_obj = json_tokener_parse(json_str);

    t_id_response *res = malloc(sizeof(t_id_response));
    memset(res, 0, sizeof(t_id_response));

    json_object *tmp = NULL;

    json_object_object_get_ex(j_obj, "object_id", &tmp);
    res->object_id = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "details", &tmp);
    res->details = strdup(json_object_get_string(tmp));
    json_object_object_get_ex(j_obj, "success", &tmp);
    res->success = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "response_type", &tmp);
    res->response_type = json_object_get_int64(tmp);

    json_object_put(j_obj);
    return res;
}

t_request_new_users *mx_t_request_new_users_unjsonify(const char *json_str){
    json_object *j_obj = json_tokener_parse(json_str);

    t_request_new_users *res = malloc(sizeof(t_request_new_users));
    memset(res, 0, sizeof(t_request_new_users));

    json_object *tmp = NULL;

    json_object_object_get_ex(j_obj, "user_id", &tmp);
    res->user_id = json_object_get_int64(tmp);
    json_object_object_get_ex(j_obj, "last_request_time", &tmp);
    res->last_request_time = json_object_get_int64(tmp);

    json_object_put(j_obj);
    return res;
}

const char *mx_t_request_new_users_jsonify(t_request_new_users response){
    json_object *result = json_object_new_object();

    json_object_object_add(result, "user_id", json_object_new_int64(response.user_id));
    json_object_object_add(result, "last_request_time", json_object_new_int64(response.last_request_time));
    json_object_object_add(result, "request_type", json_object_new_int64(e_request_new_users));

    const char *str_res = strdup(json_object_to_json_string(result));
    json_object_put(result);
    return str_res;
}

const char *mx_t_request_new_users_responce_jsonify(t_request_new_users_responce responce){
    json_object *result = json_object_new_object();
    json_object *array = json_object_new_array();
    for (int i = 0; i < responce.user_count; i++){
        json_object_array_add(array, json_object_new_string(mx_t_db_user_jsonify(responce.users[i])));
    }

    json_object_object_add(result, "user_count", json_object_new_int64(responce.user_count));
    json_object_object_add(result, "users", array);
    json_object_object_add(result, "request_type", json_object_new_int64(e_request_new_users_responce));

    char *resultstr = strdup(json_object_to_json_string(result));
    json_object_put(result);
    return resultstr;
}

t_request_new_users_responce *mx_t_request_new_users_responce_unjsonify(const char *json_str){
    json_object *j_obj = json_tokener_parse(json_str);
    if(j_obj == NULL) return NULL;
    t_request_new_users_responce *res = malloc(sizeof(t_request_new_users_responce));
    memset(res, 0, sizeof(t_request_new_users_responce));
    json_object *tmp = NULL;

    json_object_object_get_ex(j_obj, "user_count", &tmp);
    res->user_count = json_object_get_int64(tmp);

    t_db_user *arr = malloc(sizeof(t_chat) * res->user_count);
    memset(arr, 0, (sizeof(t_db_user) * res->user_count));
    json_object_object_get_ex(j_obj, "users", &tmp);
    for (int i = 0; i < res->user_count; i++){
        arr[i] = *mx_t_db_user_unjsonify(json_object_get_string(json_object_array_get_idx(tmp, i)));
    }
    res->users = arr;
    json_object_put(j_obj);
    return res;
}

void mx_t_request_new_users_responce_free(t_request_new_users_responce *p){
    for (int i = 0; i < p->user_count; i++) {
        t_db_user u = p->users[i];
        free(u.avathar);
        free(u.nickname);
        free(u.password_hash);
        free(u.user_login);
    }
    free(p->users);
    free(p);
}

/*


//struct representing chat and group_chat DB tables
//if chat is personal - chat_name, chat_avathar and member_count are undefined

//responce from server containing number of new messages and array of messages
typedef struct s_request_new_messages_responce {
    int new_messages_count;
    t_message message_arr[];
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
    t_chat chat_arr[];
}              t_request_new_chats_responce;

//request from client for newly created chats
//server returns all relevant to user created since given time
typedef struct s_request_new_chats {
    long user_id;
    time_t last_request_for_chats_time;
}              t_request_new_chats;

*/
//free t_message struct from memory
//frees strings inside and the pointer itself
void mx_t_message_free(t_message *p) {
    free(p->media);
    free(p->text);
    free(p);
}
void mx_t_create_chat_free(t_create_chat *p) {
    free(p);
}
void mx_t_login_form_free(t_login_form *p) {
    free(p->user_login);
    free(p->password);
    free(p);
}
void mx_t_login_result_free(t_login_result *p){
    free(p->details);
    free(p);
}
void mx_t_registration_form_free(t_registration_form *p){
    free(p->nickname);
    free(p->user_login);
    free(p->password);
    free(p->avathar);
    free(p);
}

void mx_t_db_user_free(t_db_user *p){
    free(p->nickname);
    free(p->user_login);
    free(p->password_hash);
    free(p->avathar);
    free(p);
}
void mx_t_registration_result_free(t_registration_result *p){
    free(p->details);
    free(p);
}
void mx_t_chat_free(t_chat *p){
    free(p->chat_name);
    free(p->chat_avathar);
    free(p);
}

void mx_t_request_new_messages_free(t_request_new_messages *p){
    free(p);
}

void mx_t_request_new_chats_free(t_request_new_chats *p){
    free(p);
}

void mx_t_edit_message_free(t_edit_message *p){
    free(p->new_text);
    free(p);
}

void mx_t_edit_profile_free(t_edit_profile *p){
    free(p->new_nick);
    free(p->new_avathar);
    free(p);
}

void mx_t_id_response_free(t_id_response *p){
    free(p->details);
    free(p);
}
