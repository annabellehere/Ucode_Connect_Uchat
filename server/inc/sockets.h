#pragma once

#include <openssl/ssl.h>

//file descriptor for the server socket
//initially set to -1, indicating non-existing socket
//after creating socket, this variable is set to the fd of the created socket
extern int serv_sock_fd;

//fd for the last established connection
//set by mx_accept_connection
extern int connection_fd;

//SSL context set by mx_ssl_setup_server_context
extern SSL_CTX *sslctx;

//SSL connection created from connection_fd
extern SSL *conn_ssl;

extern pthread_mutex_t io_lock;

//creates listening socket on indicated port
//socket is set to reuse address and port
//returns file descriptor for created socket
//also sets serv_sock_fd to the returned value
//if an error occurs, returns negative value
//-1 - failed to create socket
//-2 - setsockopt failed
//-3 - failed to bind socket
//-4 - failed to start listening
int mx_init_server_socket(int port);

//accepts connection from serv_sock_fd
//return fd for the new socket created by accept()
//sets connection_fd
int mx_accept_connection(void);

//get sockaddr_in struct used to create socket
struct sockaddr_in mx_get_sockconfig(void);

//closes server socket specified in serv_sock_fd
//sets it to -1
void mx_destroy_server_socket(void);
//shutsdown connection_fd socket for read and write 
//and immediately closes it
//sets connection_fd to -1
void mx_destroy_connection(void);

//inits SSL library
void mx_init_SSL(void);

#define DEF_CERTIFICATE "./serverCertificate.pem"
#define DEF_PRIVATEKEY "./serverPrivateKey.pem"

//creates context from given certificate
//sets sslctx to it
void mx_ssl_setup_server_context(char *certificate_file, char *privkey_file);

//makes regular accepted socket connection secure
//returns SSL pointer for reading/writing
//if sock_fd is same as connection_fd, sets conn_ssl to returned value
SSL *mx_ssl_establish_conn_fd(int sock_fd);

#define mx_ssl_establish_conn() mx_ssl_establish_conn_fd(connection_fd)
//shortened SSL_read 
#define sread(ssl, buf, num) SSL_read(ssl, buf, num)
//shortened SSL_write
#define swrite(ssl, buf, num) SSL_write(ssl, buf, num)

#define get_msg(buf, num) SSL_read(conn_ssl, buf, num)
#define send_msg(buf, num) SSL_write(conn_ssl, buf, num)

//frees ssl objects
void mx_ssl_cleanup(void);
//shuts down established ssl connection
void mx_ssl_destroy_conn(void);

