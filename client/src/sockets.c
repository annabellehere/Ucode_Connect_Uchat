#include "../inc/uchat.h"

int server_sock = -1;

SSL_CTX *sslctx = NULL;
SSL *conn_ssl = NULL;


int mx_init_client_socket(char *ip_addr, int port) {
    int sock = 0;
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		log_error(strerror(errno));
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// Convert IPv4 and IPv6 addresses from text to binary
	if (inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr) <= 0) {
		log_error(strerror(errno));
		return -2;
	}
	fprintf(stderr, "Trying to connect to %s\n", ip_addr);
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))
		< 0) {
		log_error(strerror(errno));
		return -3;
	}
    server_sock = sock;
    return server_sock;
}

void mx_destroy_server_socket(void) {
 	if (server_sock >= 0){
		shutdown(server_sock, SHUT_RDWR);
		close(server_sock);
	} 
	server_sock = -1;
}

void mx_init_SSL(void) {
	SSL_load_error_strings();
	SSL_library_init();
	pthread_mutex_init(&io_lock, NULL);
}

void mx_ssl_setup_client_context(void) {
	if (sslctx != NULL) {
		SSL_CTX_free(sslctx);
		sslctx = NULL;
	}
	sslctx = SSL_CTX_new(TLS_client_method());
}

SSL *mx_ssl_establish_conn_fd(int sock_fd) {
	if (sock_fd < 0) return NULL;
	SSL *cSSL = SSL_new(sslctx);
	SSL_set_fd(cSSL, sock_fd);
	int acc_err = SSL_connect(cSSL); 

	if (acc_err != 1) {
		log_error("SSL accept error: %s", ERR_error_string(ERR_get_error(), NULL));
	}

	if (sock_fd == server_sock) conn_ssl = cSSL;
	return cSSL;
}

void mx_ssl_cleanup(void) {
	SSL_free(conn_ssl);
	conn_ssl = NULL;
	SSL_CTX_free(sslctx);
	sslctx = NULL;
}

void mx_ssl_destroy_conn(void) {
	if (conn_ssl != NULL) {
		SSL_shutdown(conn_ssl);
	}
}

//easily sets up everything to connect to the server
//is blocking
//returns 0 upon success
int mx_easy_connect_to_server(void){
	int sock = mx_init_client_socket(server_ipv4, server_port);
	if (sock < 0) {
		log_error("Socket creation error: %s",strerror(errno));
		return 1;
	}
	mx_ssl_setup_client_context();
	mx_ssl_establish_conn();
	return 0;
}

static void *mx_easy_connect_nonblock_wrapped(void *arg) {
	if (arg == NULL) {}
	mx_easy_connect_to_server();
	return NULL;
}

//nonblocking version of mx_easy_connect_to server
//creates a detached thread, in which blocking verson is invoked
void mx_easy_connect_to_server_nonblocking(void) {
	pthread_t thread;
	pthread_create(&thread, NULL, mx_easy_connect_nonblock_wrapped, NULL);
	pthread_detach(thread);
}

void mx_disconnect_server(void) {
	mx_ssl_destroy_conn();
	mx_ssl_cleanup();
	mx_destroy_server_socket();
}

