#include "../inc/uchat.h"

#define SERVER_USAGE "usage: ./uchat_server port\n"
#define DB_FELL "BD upala :(\n"

sqlite3 *db = NULL;

int main(int argc, char *argv[])
{
    if (argc != 2) {
		mx_printerr(SERVER_USAGE);
		exit(1);
	}
	if (!mx_checkForBD()){
		//log_error(DB_PATH);
		//log_error(DB_FELL);
		exit(1);
	}
	//sqlite3_open(DB_PATH, &db);
	/*mx_test_new_messages_handle();
	mx_test_db_user();
	mx_test_chat_handle();
	mx_test_create_chat_handle();
	mx_test_new_chats_handle();*/
	//mx_test_req_new_users_handle();
	int port = atoi(argv[1]);
	mx_init_SSL();
	mx_ssl_setup_server_context(DEF_CERTIFICATE, DEF_PRIVATEKEY);

	if (mx_init_server_socket(port) < 0) {
		exit(1);
	}
	//daemonize
	mx_daemonize();
	//since now - no stdout/stderr writing works
	mx_init_logging("uchat_server.log");
	//now logging with log_* is possible like printf 
	mx_set_signal_handlers();
	//creating listening socket
	
	while (true) {
		//accepting an incomming connection
		int client_sock = mx_accept_connection();
		
		int pid = 0;
		if ((pid = fork()) == 0) {
			mx_set_child_signal_handlers();
			sqlite3_open(DB_PATH, &db);
			sqlite3_key(db, mx_get_server_db_key(), strlen(mx_get_server_db_key()));
			
			mx_ssl_establish_conn();
			
			mx_talk_to_client();

			// end communication the ssl way
			mx_ssl_destroy_conn();
			mx_ssl_cleanup();
			// closing the connected socket regular way
			mx_destroy_connection();
			// closing the listening socket
			mx_destroy_server_socket();
			mx_end_logging();
			return 0;
		}
		else {
			//printf("Child: %i\n", pid);
			close(client_sock);
		}

	}
	
	// closing the listening socket
	mx_destroy_server_socket();
	mx_end_logging();
	sqlite3_close(db);
	return 0;
}

void mx_talk_to_client(void) {
	char *buff;
	while(true) {
		buff = mx_ssl_read_package();
		//log_info("%s", buff);
		int req_type = mx_get_request_type(buff);
		switch (req_type) {
			case e_login_form: {
				char *resp = mx_login_form_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break; 
			}
			case e_registration_form: {
				char *resp = mx_registration_form_handle(buff);
				mx_ssl_send_package(resp);
				//log_debug("%s", resp);
				free(resp);
				break;
			}	
			case e_message: { // user sent message, it added to server db, and returned for adding into client db
				char *resp = mx_message_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break;
			}
			case e_request_new_messages: {
				char *resp = mx_request_new_messages_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break;
			}
			case e_request_new_chats: {
				char *resp = mx_request_new_chats_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break;
			}
			case e_db_user: {
				char *resp = mx_db_user_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break;
			}
			case e_create_chat: {
				char *resp = mx_create_chat_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break;
			}
			case e_chat: {
				char *resp = mx_chat_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break;
			}
			case e_edit_message: {
				char *resp = mx_edit_message_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break;
			}
			case e_request_new_users: {
				char *resp = mx_request_new_users_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break;
			}
			case e_edit_profile: {
				char *resp = mx_edit_profile_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break;
			}
		}
		/*if (req_type == e_login_form) {
			char *resp = mx_login_form_handle(buff);
				mx_ssl_send_package(resp);
				free(resp);
				break; 
		}*/
		free(buff);
	}
	
}
