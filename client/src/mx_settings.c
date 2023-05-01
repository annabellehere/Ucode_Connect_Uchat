#include "../inc/uchat.h"

void mx_settings(void) {
    fixed =GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
    Grid =GTK_WIDGET(gtk_builder_get_object(builder, "Grid"));
	Grid2 =GTK_WIDGET(gtk_builder_get_object(builder, "Grid2"));
	GoBack =GTK_WIDGET(gtk_builder_get_object(builder, "GoBack"));
    LogOut =GTK_WIDGET(gtk_builder_get_object(builder, "LogOut"));
	ChangeName =GTK_WIDGET(gtk_builder_get_object(builder, "ChangeName"));
	inputChangePassword = GTK_WIDGET(gtk_builder_get_object(builder, "inputChangePassword"));
	inputChangeName =GTK_WIDGET(gtk_builder_get_object(builder, "inputChangeName"));
	ChangePassword =GTK_WIDGET(gtk_builder_get_object(builder, "ChangePassword"));
    //Grid = GTK_WIDGET(gtk_builder_get_object(builder, "Grid"));
	ChangeAvatar =GTK_WIDGET(gtk_builder_get_object(builder, "ChangeAvatar"));
	ImageSettings =GTK_WIDGET(gtk_builder_get_object(builder, "ImageSettings"));

	if (strcmp(user_info->avathar, "") != 0) {
		char *file = mx_b64_to_file(user_info->avathar);

		gtk_image_set_from_file(GTK_IMAGE(ImageSettings), file);
		mx_delete_tmp_file(file);
	}
	else {
		gtk_image_set_from_file(GTK_IMAGE(ImageSettings), "./client/src/img/shrek.jpg");
	}
	
	
	scale_image(ImageSettings, 100, 100);

    gtk_widget_add_class(GTK_WIDGET(inputChangePassword), "inputs");
	gtk_widget_add_class(GTK_WIDGET(inputChangeName), "inputs");
	gtk_widget_add_class(GTK_WIDGET(GoBack), "button");
    gtk_widget_add_class(GTK_WIDGET(LogOut), "button");
    gtk_widget_add_class(GTK_WIDGET(ChangeName), "button");
    gtk_widget_add_class(GTK_WIDGET(ChangePassword), "button");
    gtk_widget_add_class(GTK_WIDGET(ChangeAvatar), "button");
	gtk_widget_add_class(window, "window-set");
   // gtk_widget_add_class(GTK_WIDGET(Grid), "ph-set");
	gtk_widget_add_class(GTK_WIDGET(labelName), "name-main");
	

	css_set(provider, labelName);
	css_set(provider, window);
   // css_set(provider, Grid);
	css_set(provider, inputChangePassword);
	css_set(provider, inputChangeName);
	css_set(provider, GoBack);
    css_set(provider, LogOut);
	css_set(provider, ChangeName);
	css_set(provider, ChangePassword);
    css_set(provider, ChangeAvatar);

	/*gtk_grid_insert_row(GTK_GRID(Grid2), 0);
    labelName = gtk_label_new(user_info->nickname); // fixx
    gtk_grid_attach(GTK_GRID(Grid2), labelName, 1, 0, 1, 1);

	GtkWidget *labelUsername;
	gtk_grid_insert_row(GTK_GRID(Grid2), 1);
    labelUsername = gtk_label_new(user_info->user_login); // fixx
    gtk_grid_attach(GTK_GRID(Grid2), labelUsername, 1, 1, 1, 1);*/

	g_signal_connect(ChangeAvatar, "clicked", G_CALLBACK(on_ChangeAvatar_clicked), NULL);
    g_signal_connect(inputChangeName, "changed", G_CALLBACK(on_inputChangeName_changed), window);
	g_signal_connect(inputChangePassword, "changed", G_CALLBACK(on_inputChangePassword_changed), window);
    g_signal_connect(GoBack, "clicked", G_CALLBACK(on_GoBack_clicked), window);
    g_signal_connect(LogOut, "clicked", G_CALLBACK(on_LogOut_clicked), window);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(ChangePassword, "clicked", G_CALLBACK(on_ChangePassword_clicked), NULL);
	g_signal_connect(ChangeName, "clicked", G_CALLBACK(on_ChangeName_clicked), NULL);
}

