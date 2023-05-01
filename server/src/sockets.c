#include "../inc/uchat.h"


int serv_sock_fd = -1;
struct sockaddr_in serv_sock_config;
//regular socket fd for the connection
int connection_fd = -1;
//SSL stuff
//Global SSL context pointer
SSL_CTX *sslctx = NULL;
//SSL connection associated with connection fd
SSL *conn_ssl = NULL;



int mx_init_server_socket(int port) {
	int opt = 1;

	// Creating socket file descriptor
	if ((serv_sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		
		log_error(strerror(errno));

        serv_sock_fd = -1;
		return -1;
	}

	if (setsockopt(serv_sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt)) == -1) {
		
		log_error(strerror(errno));

        serv_sock_fd = -2;
		return -2;
	}
	serv_sock_config.sin_family = AF_INET;
	serv_sock_config.sin_addr.s_addr = INADDR_ANY;
	serv_sock_config.sin_port = htons(port);

	if (bind(serv_sock_fd, (struct sockaddr*)&serv_sock_config, sizeof(serv_sock_config)) < 0) {
		
		log_error(strerror(errno));

        serv_sock_fd = -3;
		return -3;
	}
	if (listen(serv_sock_fd, 0) < 0) {
		
		log_error(strerror(errno));

        serv_sock_fd = -4;
		return -4;
	}
    return serv_sock_fd;
}

int mx_accept_connection(void) {
    int addrlen = sizeof(serv_sock_config);
    int fd = accept(serv_sock_fd, (struct sockaddr *)&serv_sock_config, (socklen_t *)&addrlen);
    if (!(fd < 0)) connection_fd = fd;
    return fd;
}

struct sockaddr_in mx_get_sockconfig(void) {
    return serv_sock_config;
}

void mx_destroy_server_socket(void) {
	if (serv_sock_fd >= 0) {
		close(serv_sock_fd);
	} 
	serv_sock_fd = -1;
}

void mx_destroy_connection(void) {
	if (connection_fd >= 0) {
		shutdown(connection_fd, SHUT_RDWR);
		close(connection_fd);
	}
	connection_fd = -1;
}

void mx_init_SSL(void) {
	SSL_load_error_strings();
	SSL_library_init();
	pthread_mutex_init(&io_lock, NULL);
}

void mx_ssl_setup_server_context(char *certificate_file, char *privkey_file) {
	if (sslctx != NULL) {
		SSL_CTX_free(sslctx);
		sslctx = NULL;
	}
	sslctx = SSL_CTX_new(TLS_server_method());
	int use_cert = SSL_CTX_use_certificate_file(sslctx, certificate_file, SSL_FILETYPE_PEM);
	int use_prv = SSL_CTX_use_PrivateKey_file(sslctx, privkey_file, SSL_FILETYPE_PEM);
	
	if (use_cert != 1 || use_prv != 1) {
		log_error("SSL Certificate/Private Key error: %s", ERR_error_string(ERR_get_error(), NULL));
	}
}

SSL *mx_ssl_establish_conn_fd(int sock_fd) {
	if (sock_fd < 0) return NULL;
	SSL *cSSL = SSL_new(sslctx);
	SSL_set_fd(cSSL, sock_fd);
	int acc_err = SSL_accept(cSSL); 
	
	if (acc_err != 1) {
		log_error("SSL accept error: %s", ERR_error_string(ERR_get_error(), NULL));
	}
	
	if (sock_fd == connection_fd) conn_ssl = cSSL;
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


