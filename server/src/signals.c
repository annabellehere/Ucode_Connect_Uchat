#include "../inc/uchat.h"

static void mx_handle_sigterm(int sig) {
    if (sig == 0) {}
    // end communication the ssl way
	mx_ssl_destroy_conn();
	mx_ssl_cleanup();
	// closing the connected socket regular way
	mx_destroy_connection();
	// closing the listening socket
	mx_destroy_server_socket();
    sqlite3_close(db);
    db = NULL;
	mx_end_logging();
    signal(SIGTERM, SIG_IGN);
    //kill all children
    kill(0, SIGTERM);
    exit(0);
}

static void mx_handle_child_sigterm(int sig) {
    if (sig == 0) {}
    // end communication the ssl way
	mx_ssl_destroy_conn();
	mx_ssl_cleanup();
	// closing the connected socket regular way
	mx_destroy_connection();
	// closing the listening socket
	mx_destroy_server_socket();
	mx_end_logging();
    sqlite3_close(db);
    db = NULL;
    
    exit(0);
}

void mx_set_signal_handlers(void) {
    //discards exit codes of children and prevents zombies
    signal(SIGCHLD, SIG_IGN);
    //handle SIGTERM to properly end communication
    signal(SIGTERM, mx_handle_sigterm);
}

void mx_set_child_signal_handlers(void) {
    //removes repeating signal sending
    signal(SIGTERM, mx_handle_child_sigterm);
}
