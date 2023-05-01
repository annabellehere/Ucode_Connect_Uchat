#pragma once

#ifdef __linux__
#define _XOPEN_SOURCE  500 
#endif

#define SQLITE_HAS_CODEC
#define SQLITE_TEMP_STORE 2
#define DB_PATH "./server_chat_db.db"




#include "../../libmx/inc/libmx.h"
#include "./json-c/json.h"
#include "sqlite3.h"
#include <sys/time.h>


#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>


#include <signal.h>


#include "sockets.h"
#include "queries.h"
#include "base64.h"
#include "db.h"
#include "testing.h"

//https://github.com/rxi/log.c
#include "log.h"

//makes the process a daemon
//closes stdin/out/err file descriptors
//thus writing to stdout and stderr and reading stdin becomes unavailable
//after calling this function other means of error logging must be used
//TLDR: NO PRINTF, NO mx_printstr/mx_printerr/mx_printint
void mx_daemonize(void);

void mx_init_logging(char *filename);
void mx_end_logging(void);
void mx_talk_to_client(void);

void mx_set_signal_handlers(void);
void mx_set_child_signal_handlers(void);

char *mx_ssl_read_package(void);
int mx_ssl_send_package(const char *str);
void mx_begin_io_transaction(void);
void mx_end_io_transaction(void);
char *mx_get_server_db_key(void);
