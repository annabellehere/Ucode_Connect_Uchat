#include "../inc/uchat.h"

#define PACKAGE_HEAD_LEN 16

pthread_mutex_t io_lock;

//returns unsigned long as zero-padded string of 16 characters
static char *mx_get_padded_size(size_t len) {
    char buf[PACKAGE_HEAD_LEN + 1];
    memset(buf, 0, PACKAGE_HEAD_LEN + 1);
    snprintf(buf, PACKAGE_HEAD_LEN + 1, "%016lu", len);
    return strdup(buf);
}

//converts numerical string into unsigned long
static size_t mx_read_padded_size(const char *padded_num_str) {
    return strtoul(padded_num_str, NULL, 10);
}

//appends the length of the string to its start as 16 char 0-padded string 
static char *mx_prep_str_add_head(const char *str) {
    char *head = mx_get_padded_size(strlen(str));
    char *res = mx_strjoin(head, str);
    free(head);
    return res;
}

//get next package from SSL socket
//returns NULL if there is no ssl connection 
char *mx_ssl_read_package(void) {
    if (conn_ssl == NULL) return NULL;
    char lenbuf[PACKAGE_HEAD_LEN + 1];
    memset(lenbuf, 0, PACKAGE_HEAD_LEN + 1);
    //read head from SSL socket
    get_msg(lenbuf, PACKAGE_HEAD_LEN);
    //get head of the package
    size_t len = mx_read_padded_size(lenbuf);
    //alloc space for the package itself
    char *res = malloc(len + 1);
    memset(res, 0, len + 1);
    //read package content from SSL socket
    size_t read_bytes = 0;
    size_t written = 0;
    while (written < len) {
        SSL_read_ex(conn_ssl, res + written, len - written, &read_bytes);
        written += read_bytes;
    }

    return res;
}

//send str to SSL socket 
//returns numbre of bytes send
//returns -1 if there is no ssl connection 
int mx_ssl_send_package(const char *str) {
    if (conn_ssl == NULL) return -1;
    //add head to the package
    char *res = mx_prep_str_add_head(str);
    //send it
    size_t sent_now = 0;
    size_t written = 0;
    size_t len = strlen(res);
    while (written < len) {
        SSL_write_ex(conn_ssl, res + written, len - written, &sent_now);
        written += sent_now;
    }
    free(res);
    return written;
}

//blocks all other tries for beginning IO transaction
//should be called every time you send some data and wait for an answer
//to not let other threads shit in the socket, possible causing race conditions
//relevant for client
void mx_begin_io_transaction(void) {
    pthread_mutex_lock(&io_lock);
}

//makes beginning new transaction possible
//reverses the effect of mx_begin_io_transaction()
//ALWAYS call it if using mx_begin_io_transaction()
//relevant for client 
void mx_end_io_transaction(void) {
    pthread_mutex_unlock(&io_lock);
}
