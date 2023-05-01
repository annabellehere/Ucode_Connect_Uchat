#include "../inc/uchat.h"

t_registration_form reg_form = {"","","",""};

t_login_form login_form = {"", ""};

char *change_password_str = NULL;
char *change_nick = NULL;

t_message message_to_send;
static void mx_init_login_form(void) {
	static bool done = false;
	if (done == true) return;
	memset(&login_form, 0, sizeof(t_login_form));
	done = true;
}

//enter
void on_inputPassword_changed(GtkEntry *e) {
	mx_init_login_form();
	free(login_form.password);
	login_form.password = strdup(gtk_entry_get_text(e));
}

void on_inputUsername_changed(GtkEntry *e) {
	mx_init_login_form();
	free(login_form.user_login);
	login_form.user_login = strdup(gtk_entry_get_text(e));
}

static void mx_init_reg_form(void) {
	static bool done = false;
	if (done == true) return;
	memset(&reg_form, 0, sizeof(t_registration_form));
	
	reg_form.avathar = mx_strnew(0);
	done = true;
}

void mx_init_message_to_send(void){
	static bool done = false;
	if (done)
		return;
	memset(&message_to_send, 0, sizeof(t_message));
}

//reg
void on_inputUsernameReg_changed(GtkEntry *e) {
	mx_init_reg_form();
	free(reg_form.user_login);
	reg_form.user_login = strdup(gtk_entry_get_text(e));
}

void on_inputPasswordReg_changed(GtkEntry *e) {
	mx_init_reg_form();
	free(reg_form.password);
	reg_form.password = strdup(gtk_entry_get_text(e));
}

void on_inputNicknameReg_changed(GtkEntry *e) {
	mx_init_reg_form();
	free(reg_form.nickname);
	reg_form.nickname = strdup(gtk_entry_get_text(e));
}

//main
void on_InputMessage_changed(GtkEntry *e) { // send a message
	mx_init_message_to_send();
	free(message_to_send.text);
	message_to_send.text = strdup(gtk_entry_get_text(e));
}


void on_inputFriend_changed(GtkEntry *e) { // add a new friend
	
}

//settings
void on_inputChangeName_changed(GtkEntry *e) { // change name
	free(change_nick);
	change_nick = strdup(gtk_entry_get_text(e));
}

void on_inputChangePassword_changed(GtkEntry *e) { // change password
	free(change_password_str);
	change_password_str = strdup(gtk_entry_get_text(e));
}



