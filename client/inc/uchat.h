#pragma once



#ifdef __linux__
#define _XOPEN_SOURCE  500 
#endif

#define SQLITE_HAS_CODEC
#define SQLITE_TEMP_STORE 2
#define DB_PATH "./client_chat_db.db"
#define DB_FELL "Client db slomalsia :("
#define MAX_CHAT_VISIBLE_NAME 25
#define MESSAGE_WIDTH 970
#include "../../libmx/inc/libmx.h"
#include <json-c/json.h>
#include <sqlite3.h>
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
#include <pthread.h>


#include <gtk/gtk.h>

#include "sockets.h"
#include "base64.h"
#include "queries.h"
#include "db.h"
#include "images.h"
//https://github.com/rxi/log.c
#include "log.h"

extern int server_port;
extern char *server_ipv4;


void mx_init_logging(char *filename);
void mx_end_logging(void);
void mx_talk_to_server(void);


extern t_message message_to_send;
extern t_registration_form reg_form;
extern t_login_form login_form;
extern t_db_user *user_info;
extern t_list *chats_panel;
extern t_list *messages_panel;
extern GThread *chat_panel_thread;
extern GThread *message_panel_thread;
extern t_chat_panel current_chat;
extern bool chats_need_to_update;
extern bool messages_need_to_updata;
extern guint chat_timeout;
extern guint message_timeout;


char *mx_ssl_read_package(void);
int mx_ssl_send_package(const char *str);
void mx_begin_io_transaction(void);
void mx_end_io_transaction(void);
void mx_init_new_gchat(void);

extern GtkWidget *window;
extern GtkWidget *window1;
extern GtkWidget *fixed;
extern GtkWidget *fixed2;
extern GtkWidget *LogIn;
extern GtkWidget *Register;
extern GtkWidget *labelPassword;
extern GtkWidget *labelUsername;
extern GtkWidget *labelNickname;
extern GtkWidget *inputPassword;
extern GtkWidget *labelRegistered;
extern GtkWidget *inputUsername;
extern GtkWidget *inputPasswordReg;
extern GtkWidget *inputUsernameReg;
extern GtkWidget *inputNicknameReg;
extern GtkWidget *LogInReg;
extern GtkWidget *RegisterReg;
extern GtkWidget *Scrolled;
extern GtkWidget *Scrolled2;
extern GtkWidget *Viewport2;
extern GtkWidget *Viewport;
extern GtkWidget *Grid;
extern GtkWidget *Grid2;
extern GtkWidget *Grid3;
extern GtkWidget *Search;
extern GtkWidget *PhotoReg;
extern GtkWidget *button[1000];
extern GtkWidget *label[1000];
extern GtkWidget *labelName;
extern GtkBuilder *builder;
extern GtkBuilder *builder1;
extern GtkCssProvider *provider;
extern GtkWidget *InputMessage;
extern GtkWidget *SendMessage;
extern GtkWidget *SendMessage1;
extern GtkWidget *Settings;
extern GtkWidget *GoBack;
extern GtkWidget *errorDialog;
extern GtkWidget *ChangeName;
extern GtkWidget *inputChangePassword;
extern GtkWidget *inputChangeName;
extern GtkWidget *ChangePassword;
extern GtkWidget *ChangeAvatar;
extern GtkWidget *LogOut;
extern GtkWidget *imageReg;
extern GtkWidget *inputFriend;
extern GtkWidget *image;
extern GtkWidget *ImageSettings;
extern GtkWidget *fix;
extern GtkWidget *labelblacklist;
extern GtkWidget *scroll;
extern GtkWidget *viewport;
extern GtkWidget *grid;
extern GtkWidget *gobackButton;
extern GtkWidget *blacklist;
extern GtkWidget *Logo;
extern GtkWidget *LogoImage;
extern GtkWidget *info;
extern GtkWidget *Ava;
extern GtkWidget *Nick;
extern GtkWidget *mainGrid;
extern GtkWidget *foreignMessage[1000];
extern GtkWidget *messageInfo[1000];
extern GtkWidget *userMessage[1000];
extern GtkWidget *userMessageInfo[1000];
extern GtkWidget *messageNickname;
extern GtkWidget *GroupChat;
extern GtkWidget *addMan;
extern GtkWidget *inputName;
extern GtkWidget *CreateGroup;
extern GtkWidget *userEditMessage[1000];
extern GtkWidget *errorDialog;
extern GtkWidget *errorInfo;
extern GtkWidget *errorType;
extern GtkWidget *errorMessage;
extern GtkWidget *closeErrorButton;
extern GtkWidget *editMessageWindow;
extern GtkWidget *editMessageInput;
extern GtkWidget *editMessageSendButton;
extern GtkWidget *GroupName;
extern GtkWidget *LabelGroup;
extern GtkWidget *ChangeAvaGroup;
extern GtkWidget *ImageSettingGroup;
extern GtkWidget *userDeleteMessage[1000];
extern GtkWidget *scrollDown;
extern GtkWidget *WaitingGif;
extern char *change_password_str;
extern char *change_nick;
extern t_create_chat new_gchat;
extern bool delete_edit_message;
extern GtkBuilder *error_builder;
void on_LogIn_clicked (GtkButton *b);
void on_Register_clicked (GtkButton *b);
void on_inputUsername_changed(GtkEntry *e);
void on_inputPassword_changed(GtkEntry *e);
void on_inputUsernameReg_changed(GtkEntry *e);
void on_inputPasswordReg_changed(GtkEntry *e);
void on_inputPhoneReg_changed(GtkEntry *e);
void on_inputNicknameReg_changed(GtkEntry *e);
void mx_register (void) ;
void mx_login(void) ;
void on_RegisterReg_clicked (GtkButton *b);
void on_LogInReg_clicked (GtkButton *b);
void mx_mainWind(void);
void gtk_widget_add_class(GtkWidget *widget, const gchar *class_name);
void css_set(GtkCssProvider *cssprov, GtkWidget *g_widget);
void on_PhotoReg_clicked(GtkWidget *widget, gpointer data);
void on_ChatPanel_clicked (GtkButton *b, gpointer data);
void on_closeErrorButton_clicked (GtkButton *b);
char *mx_chat_names_shorten(t_chat_panel *chat_panel);
void mx_clean_grid(int number_of_grid);

void mx_start_chat_message_threading(void );
char *mx_read_full_file(char *file, long *len);
char *mx_make_tmp_file(char *buff, size_t len);
void mx_delete_tmp_file(char *filename);
char *mx_b64_to_file(char *b64str);
char *mx_compress_image(char *file);

void mx_start_requesting_routine_thread(long user_id);
void mx_terminate_requesting_thread(void);
void on_Settings_clicked (GtkButton *b);
void mx_settings(void);
void on_GoBack_clicked (GtkButton *b);
void on_LogOut_clicked (GtkButton *b);
void mx_t_free_message_panel(t_list *panel);
void mx_t_free_chat_panel(t_list *panel);
void on_blacklist_clicked (GtkButton *b);
void on_Block_clicked (GtkButton *b);
void mx_blacklist(void);
void on_GroupName_changed(GtkEntry *e);

gboolean mx_chat_updating(void *ptr);
gpointer update_chats(void);
gpointer update_messages(void);
gboolean mx_render_messages(void *ptr);
void mx_reset_new_gchat(void);

void on_Search_clicked (GtkButton *b, gpointer data) ;
void scale_image(GtkWidget *imagetemp, int width, int height);
void mx_close_g_thread(GThread *th, guint timeout);
void on_SendMessage_clicked (GtkButton *b, gpointer data);
void on_InputMessage_changed(GtkEntry *e);
//void on_DelAcc_clicked (GtkButton *b);
void on_ChangePassword_clicked (GtkButton *b);
void on_ChangeName_clicked (GtkButton *b);
void on_inputChangePassword_changed(GtkEntry *e);
void on_inputChangeName_changed(GtkEntry *e);
void on_inputFriend_changed(GtkEntry *e);
void on_ChangeAvatar_clicked(GtkWidget *widget, gpointer data);
void mx_notifications(void);
void mx_group(void);
void on_GroupChat_clicked(GtkButton *b) ;
void on_CreateGroup_clicked(GtkButton *b) ;
void on_addMan_clicked(GtkButton *b) ;
int get_num_filled_rows(GtkWidget *grid);
void on_userEditMessage_clicked(gpointer *data);
void on_editMessageInput_changed(GtkEntry *e);
void on_userEditMessageSendButton_clicked(gpointer *data);
void on_SendMessage1_clicked(GtkWidget *widget, gpointer data);
void on_ChangeAvaGroup_clicked(GtkButton *b);
char *mx_get_client_db_key(void);


void on_userDeleteMessage_clicked(t_message_panel *id);
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

