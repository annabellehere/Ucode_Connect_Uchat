#include "../inc/uchat.h"

void mx_register (void) {
    fixed = (GtkWidget*) gtk_builder_get_object(builder, "fixed");
	LogInReg = (GtkWidget*) gtk_builder_get_object(builder, "LogInReg");
	RegisterReg = (GtkWidget*) gtk_builder_get_object(builder, "RegisterReg");
	labelPassword = (GtkWidget*) gtk_builder_get_object(builder, "labelPassword");
	labelUsername = (GtkWidget*) gtk_builder_get_object(builder, "labelUsername");
    labelNickname = (GtkWidget*) gtk_builder_get_object(builder, "labelNickname");
	inputPasswordReg = (GtkWidget*) gtk_builder_get_object(builder, "inputPasswordReg");
	inputUsernameReg = (GtkWidget*) gtk_builder_get_object(builder, "inputUsernameReg");
    inputNicknameReg = (GtkWidget*) gtk_builder_get_object(builder, "inputNicknameReg");
	PhotoReg = (GtkWidget*) gtk_builder_get_object(builder, "PhotoReg");
	imageReg = (GtkWidget*) gtk_builder_get_object(builder, "imageReg");
	gtk_entry_set_visibility(GTK_ENTRY(inputPasswordReg), FALSE);
	gtk_entry_set_invisible_char(GTK_ENTRY(inputPasswordReg), '*');
	gtk_image_set_from_file(GTK_IMAGE(imageReg), BASIC_AVATHAR_URL);
	scale_image(imageReg, 50, 50);

	gtk_widget_add_class(GTK_WIDGET(inputPasswordReg), "inputs");
	gtk_widget_add_class(GTK_WIDGET(inputUsernameReg), "inputs");
	gtk_widget_add_class(GTK_WIDGET(inputNicknameReg), "inputs");
	gtk_widget_add_class(GTK_WIDGET(LogInReg), "button");
	gtk_widget_add_class(GTK_WIDGET(RegisterReg), "button");
	gtk_widget_add_class(GTK_WIDGET(labelUsername), "label");
	gtk_widget_add_class(GTK_WIDGET(labelPassword), "label");
	gtk_widget_add_class(GTK_WIDGET(labelNickname), "label");
	//gtk_widget_add_class(GTK_WIDGET(imageReg), "imagereg");
	gtk_widget_add_class(window, "window-reg");
	gtk_widget_add_class(PhotoReg, "button");
	

	//css_set(provider, imageReg);
	css_set(provider, window);
	css_set(provider, PhotoReg);
	css_set(provider, inputPasswordReg);
	css_set(provider, inputUsernameReg);
	css_set(provider, inputNicknameReg);
	css_set(provider, LogInReg);
	css_set(provider, RegisterReg);
	css_set(provider, labelUsername);
	css_set(provider, labelPassword);
	css_set(provider, labelNickname);

	g_signal_connect(PhotoReg, "clicked", G_CALLBACK(on_PhotoReg_clicked), NULL);
    g_signal_connect(inputPasswordReg, "changed", G_CALLBACK(on_inputPasswordReg_changed), window);
	g_signal_connect(inputUsernameReg, "changed", G_CALLBACK(on_inputUsernameReg_changed), window);
	g_signal_connect(inputNicknameReg, "changed", G_CALLBACK(on_inputNicknameReg_changed), window);
    g_signal_connect(LogInReg, "clicked", G_CALLBACK(on_LogInReg_clicked), NULL);
    g_signal_connect(RegisterReg, "clicked", G_CALLBACK(on_RegisterReg_clicked), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

