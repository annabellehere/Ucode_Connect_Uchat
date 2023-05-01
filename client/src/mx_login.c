#include "../inc/uchat.h"

void css_set(GtkCssProvider *cssprov, GtkWidget *g_widget) {
	GtkStyleContext *context = gtk_widget_get_style_context(g_widget);
	gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssprov),
	GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void gtk_widget_add_class(GtkWidget *widget, const gchar *class_name){
	GtkStyleContext *context = NULL;
	g_return_if_fail(GTK_IS_WIDGET (widget));
	g_return_if_fail(class_name != NULL);

	context = gtk_widget_get_style_context(widget);
	gtk_style_context_add_class(context, class_name);
}

void mx_login(void) {
    fixed = (GtkWidget*) gtk_builder_get_object(builder, "fixed");
	LogIn = (GtkWidget*) gtk_builder_get_object(builder, "LogIn");
	Register = (GtkWidget*) gtk_builder_get_object(builder, "Register");
	labelPassword = (GtkWidget*) gtk_builder_get_object(builder, "labelPassword");
	labelUsername = (GtkWidget*) gtk_builder_get_object(builder, "labelUsername");
	inputPassword = (GtkWidget*) gtk_builder_get_object(builder, "inputPassword");
	inputUsername = (GtkWidget*) gtk_builder_get_object(builder, "inputUsername");
	Logo = (GtkWidget*) gtk_builder_get_object(builder, "Logo");
	LogoImage = GTK_WIDGET(gtk_builder_get_object(builder, "LogoImage"));
	gtk_image_set_from_file(GTK_IMAGE(LogoImage), "./client/src/img/logo.png");
	scale_image(LogoImage, 250, 250);
	gtk_entry_set_visibility(GTK_ENTRY(inputPassword), FALSE);
	gtk_entry_set_invisible_char(GTK_ENTRY(inputPassword), '*');
	

	gtk_image_set_from_file(GTK_IMAGE(LogoImage), "client/src/img/logo.png");
    scale_image(LogoImage, 300, 300);

	gtk_widget_add_class(GTK_WIDGET(inputPassword), "inputs");
	gtk_widget_add_class(GTK_WIDGET(inputUsername), "inputs");
	gtk_widget_add_class(GTK_WIDGET(LogIn), "button");
	gtk_widget_add_class(GTK_WIDGET(Register), "button");
	gtk_widget_add_class(GTK_WIDGET(labelUsername), "label");
	gtk_widget_add_class(GTK_WIDGET(labelPassword), "label");
	gtk_widget_add_class(window, "window");
	gtk_widget_add_class(GTK_WIDGET(Logo), "logo");
	

	css_set(provider, Logo);
	css_set(provider, window);
	css_set(provider, inputPassword);
	css_set(provider, inputUsername);
	css_set(provider, LogIn);
	css_set(provider, Register);
	css_set(provider, labelUsername);
	css_set(provider, labelPassword);

	g_signal_connect(inputPassword, "changed", G_CALLBACK(on_inputPassword_changed), window);
	g_signal_connect(inputUsername, "changed", G_CALLBACK(on_inputUsername_changed), window);
	g_signal_connect(LogIn, "clicked", G_CALLBACK(on_LogIn_clicked), window);
	g_signal_connect(Register, "clicked", G_CALLBACK(on_Register_clicked), window);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}


