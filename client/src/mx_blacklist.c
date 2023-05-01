#include "../inc/uchat.h"

void mx_blacklist(void) {
    fix = (GtkWidget*) gtk_builder_get_object(builder, "fixe");
    labelblacklist = (GtkWidget*) gtk_builder_get_object(builder, "labelblacklist");
    scroll = (GtkWidget*) gtk_builder_get_object(builder, "scroll");
    viewport = (GtkWidget*) gtk_builder_get_object(builder, "viewport");
    grid = (GtkWidget*) gtk_builder_get_object(builder, "grid");
    gobackButton = (GtkWidget*) gtk_builder_get_object(builder, "gobackButton");

    //gtk_widget_add_class(GTK_WIDGET(gobackButton), "button");
    //css_set(provider, gobackButton);
    /*int usr_id = user_info->user_id;
    //log_info("USR ID %d",usr_id);
    blacklist = mx_get_blacklist(usr_id); 
    this function should give all of blacklist of user, probably uid's in order to get avatar and nickname
    int size = mx_list_size(blacklist);
    int row = 0;
    for(t_list *i = blacklist; i != NULL; i = i->next){
            ...
    }*/
    gtk_widget_show_all(window);
    g_signal_connect(gobackButton, "clicked", G_CALLBACK(on_GoBack_clicked), window);    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
