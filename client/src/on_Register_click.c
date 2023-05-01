#include "../inc/uchat.h"

//go to registration form
void on_Register_clicked (GtkButton *b) {
    
    int registered = 0;
    if (registered){
        gtk_widget_hide(window);
        builder = gtk_builder_new_from_file ("client/src/test.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        gtk_widget_show_all(window);
        g_signal_connect(LogIn, "clicked", G_CALLBACK(on_LogIn_clicked), NULL);
        g_signal_connect(Register, "clicked", G_CALLBACK(on_Register_clicked), NULL);
    }
    else {
        gtk_widget_hide(window);
        builder = gtk_builder_new_from_file ("client/src/register.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        gtk_widget_show_all(window);
        mx_register();
        g_signal_connect(LogIn, "clicked", G_CALLBACK(on_LogIn_clicked), NULL);
        g_signal_connect(Register, "clicked", G_CALLBACK(on_Register_clicked), NULL);        
    }
}
//try registrating
void on_RegisterReg_clicked (GtkButton *b) {
    reg_form.nickname = mx_strtrim(reg_form.nickname);
    reg_form.password = mx_strtrim(reg_form.password);
    reg_form.user_login = mx_strtrim(reg_form.user_login);
    //log_error("%s %s %s", reg_form.nickname, reg_form.password, reg_form.user_login);
    if(reg_form.nickname == NULL || reg_form.password == NULL || reg_form.user_login == NULL){
        gtk_label_set_text(GTK_LABEL(errorMessage), "Not all fields are fullfill.");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
        return;
    }
    if(strcmp(reg_form.nickname, "") == 0 || strcmp(reg_form.password, "") == 0 || strcmp(reg_form.user_login, "") == 0){
        
        gtk_label_set_text(GTK_LABEL(errorMessage), "Whitespaces isn`t good information, try again.");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
        return;
    }
    
    

    //hash to base64
    
    
    /*FILE *avathar = fopen(BASIC_AVATHAR_URL, "r");

    fseek(avathar, 0, SEEK_END);
    long avathar_size = ftell(avathar);
    fseek(avathar ,0, SEEK_SET);
    
    char *avathar_str = malloc(avathar_size + 1);
    avathar_str[avathar_size] = '\0';

    fread(avathar_str, avathar_size, 1, avathar);
    fclose(avathar);
    int avathar_b64_len = Base64encode_len(avathar_size);
    char *avathar_b64 = malloc(avathar_b64_len);
    memset(avathar_b64, 0, avathar_b64_len);
    Base64encode(avathar_b64, avathar_str, avathar_size);
    free(avathar_str);

    //log_info("SIZE OF AVATHAR: %i", avathar_b64_len);
    //log_info("AVATHAR: %s\n",avathar_b64);*/
    char *json_reg_form = (char *)mx_t_registration_form_jsonify(reg_form);

    mx_begin_io_transaction();

    mx_ssl_send_package(json_reg_form);
    char *reg_result_json = mx_ssl_read_package();

    mx_end_io_transaction();
    free(json_reg_form);
    t_registration_result *reg_res = mx_t_registration_result_unjsonify(reg_result_json);
    free(reg_result_json);
    if (reg_res->success) {
        //log_info("Registration Success");
        t_db_user usr;
        memset(&usr, 0, sizeof(t_db_user));
        usr.user_id = reg_res->user_id;
        usr.nickname = strdup(reg_form.nickname);
        usr.password_hash = strdup("");
        usr.user_login = strdup(reg_form.user_login);
        
        usr.avathar = strdup(reg_form.avathar);
        //log_info("try insert user");
        mx_user_inserter(&usr);
        
        free(usr.nickname);
        free(usr.password_hash);
        free(usr.user_login);
        free(usr.avathar);
    }
    else 
    {
        gtk_label_set_text(GTK_LABEL(errorMessage), "This is login is already registrated.");
        gtk_dialog_run(GTK_DIALOG(errorDialog));
        
        return;
    }

    
    
    mx_t_registration_result_free(reg_res);

    gtk_widget_hide(window);
    builder = gtk_builder_new_from_file ("client/src/login.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_widget_show_all(window);
    //go back to login field
    mx_login();
}


