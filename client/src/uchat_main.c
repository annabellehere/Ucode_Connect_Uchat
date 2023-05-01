#include "../inc/uchat.h"

#define USAGE "usage: ./uchat server_ip port\nLocalhost is 127.0.0.1\n"

//externed global var for easy access to server port
int server_port = 0;
//externed global var for easy access to server ip address
char *server_ipv4 = NULL;

sqlite3 *db_client;
t_db_user *user_info;
t_list *messages_panel;
t_list *chats_panel;
GThread *chat_panel_thread;
GThread *message_panel_thread;
t_chat_panel current_chat;
bool chats_need_to_update = true;
bool messages_need_to_updata = true;
guint message_timeout = 0;
guint chat_timeout = 0;
bool delete_edit_message = false;

int main(int argc, char *argv[])
{ 
	if (!mx_checkForBD()){
		//log_error(DB_FELL);
		exit(1);
	}
	sqlite3_open(DB_PATH, &db_client);
	if (argc != 3) {
		mx_printerr(USAGE);
		exit(1);
	}
	sqlite3_key(db_client, mx_get_client_db_key(), strlen(mx_get_client_db_key()));
	
	int port = atoi(argv[2]);
	char *server_ip = argv[1];
	server_port = port;
	server_ipv4 = server_ip;
	mx_init_logging("./uchat_client.log");
	mx_init_SSL();
	//g_thread_init(NULL);
	gdk_threads_init();
	current_chat.chat_id = -1;
	if (mx_easy_connect_to_server() != 0) {
		log_error("Error Connecting to server");
		exit(1);
	}
	provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(provider, "client/src/css/all.css", NULL);
	builder = gtk_builder_new();
	gtk_init(&argc, &argv);
	builder = gtk_builder_new_from_file ("client/src/login.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	error_builder = gtk_builder_new_from_file("client/src/error_dialog.glade");
	//gtk_window_fullscreen(GTK_WINDOW(window));
	errorDialog = GTK_WIDGET(gtk_builder_get_object(error_builder, "errorDialog"));
	errorMessage = GTK_WIDGET(gtk_builder_get_object(error_builder, "errorMessage"));
	errorType = GTK_WIDGET(gtk_builder_get_object(error_builder, "errorType"));
	closeErrorButton = GTK_WIDGET(gtk_builder_get_object(error_builder, "closeErrorButton"));
	gtk_label_set_text(GTK_LABEL(errorType), "Error");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_hide(errorDialog);
	g_signal_connect(closeErrorButton, "clicked", G_CALLBACK(on_closeErrorButton_clicked), errorDialog);

	css_set(provider, errorDialog);
	css_set(provider, closeErrorButton);
	mx_login();

	gtk_widget_show_all(window);

	gtk_main();

	
	

	mx_disconnect_server();
	mx_end_logging();
	
	return 0;
}




