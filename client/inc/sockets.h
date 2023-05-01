#pragma once 

#include <openssl/ssl.h>

//fd of the socket connected to to server
//DO NOT TOUCH
extern int server_sock;

//SSL connection created from server_sock
extern SSL *conn_ssl;

extern pthread_mutex_t io_lock;

//creates socket and tries to connect it to specified address
//on success returns fd of created socket and sets server_sock to it
//else returns negative value and sets errno
// -1 - failed creating socket
// -2 - invalid address
// -3 - failed to connect
int mx_init_client_socket(char *ip_addr, int port);

//closes the socket connected to the server
//sets server_sock to -1
void mx_destroy_server_socket(void);

//inits SSL library
void mx_init_SSL(void);

//creates context for client
//sets sslctx to it
void mx_ssl_setup_client_context(void);

//makes regular accepted socket connection secure
//returns SSL pointer for reading/writing
//if sock_fd is same as server_sock, sets conn_ssl to returned value
SSL *mx_ssl_establish_conn_fd(int sock_fd);

#define mx_ssl_establish_conn() mx_ssl_establish_conn_fd(server_sock)
//shortened SSL_read 
#define sread(ssl, buf, num) SSL_read(ssl, buf, num)
//shortened SSL_write
#define swrite(ssl, buf, num) SSL_write(ssl, buf, num)

//even more shortened SSL_read
#define get_msg(buf, num) SSL_read(conn_ssl, buf, num)
//even more shortened SSL_write
#define send_msg(buf, num) SSL_write(conn_ssl, buf, num)

//frees ssl objects
void mx_ssl_cleanup(void);

void mx_ssl_destroy_conn(void);

int mx_easy_connect_to_server(void);
void mx_easy_connect_to_server_nonblocking(void);

//disconnects the server from client
//returns to state, as if no connection was created
void mx_disconnect_server(void);




