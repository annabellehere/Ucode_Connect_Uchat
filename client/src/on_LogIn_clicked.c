#include "../inc/uchat.h"

long logIn_user_id;

//try to log in
void on_LogIn_clicked (GtkButton *b) {
    int registered = 0;
    login_form.password = mx_strtrim(login_form.password);
    login_form.user_login = mx_strtrim(login_form.user_login);
    if (login_form.password == NULL || login_form.user_login == NULL) {
        //log_info("Empty fields CHURKA");
        //GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Fill in all inputs, bro!");
        gtk_label_set_text(GTK_LABEL(errorMessage), "You forgot to fill the information.");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
        //gtk_widget_hide(errorDialog);
        return;
    }
    
    if (strcmp(login_form.password, "") == 0 || strcmp(login_form.user_login, "") == 0) {
        //log_info("Empty fields still CHURKA");
               gtk_label_set_text(GTK_LABEL(errorMessage), "You forgot to fill the information.");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
        return;
    }
    //check if user exists

    char *json_login_form = (char *)mx_t_login_form_jsonify(login_form);

    mx_begin_io_transaction();

    mx_ssl_send_package(json_login_form);
    free(json_login_form);
    char *login_result_json = mx_ssl_read_package();
    mx_end_io_transaction();

    t_login_result *res = mx_t_login_result_unjsonify(login_result_json);
    if (res->success == true) registered = 1;
    else {
        gtk_label_set_text(GTK_LABEL(errorMessage), "Wrong login or password");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
        registered = 0;
    }
    //log_info("Log in result: %i", res->success);
    mx_t_login_result_free(res);
    

    if(registered){
        user_info = mx_ask_for_client_db_user(login_form.user_login);
        //log_info("User_id: %i", user_info->user_id);

        mx_start_requesting_routine_thread(user_info->user_id);
        gtk_entry_set_text(GTK_ENTRY(inputUsername), "");
        gtk_entry_set_text(GTK_ENTRY(inputPassword), "");
        gtk_widget_hide(window);
        builder = gtk_builder_new_from_file ("client/src/main_chat.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        gtk_widget_show_all(window);
        mx_mainWind();
    }
    else {

    }
}

//go to login form screen
void on_LogInReg_clicked (GtkButton *b) {
    gtk_widget_hide(window);
    builder = gtk_builder_new_from_file ("client/src/login.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_widget_show_all(window);
    mx_login();
}



