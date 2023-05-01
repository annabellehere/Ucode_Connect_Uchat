#include "../inc/uchat.h"

void mx_notifications(void)
{
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "You have new message!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

    g_timeout_add_seconds(8, (GSourceFunc)gtk_widget_destroy, dialog);
}

