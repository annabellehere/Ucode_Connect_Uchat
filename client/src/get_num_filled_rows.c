#include "../inc/uchat.h"

int get_num_filled_rows(GtkWidget *grid)
{
    GtkGrid *temp = (GtkGrid *) grid;
    int num_rows = 0;
    gboolean homogeneous = gtk_grid_get_row_homogeneous(temp);
    int spacing = gtk_grid_get_row_spacing(temp);

    if (homogeneous) {
        int row_height = gtk_widget_get_allocated_height(gtk_grid_get_child_at(temp, 0, 0));
        int total_height = gtk_widget_get_allocated_height(GTK_WIDGET(temp));
        num_rows = (total_height + spacing) / (row_height + spacing);
    } else {
        GList *children = gtk_container_get_children(GTK_CONTAINER(temp));
        GList *row_children = NULL;
        int last_y = -1;

        for (GList *it = children; it != NULL; it = it->next) {
            GtkWidget *child = GTK_WIDGET(it->data);
            GtkAllocation allocation;
            gtk_widget_get_allocation(child, &allocation);
            int y = allocation.y;

            if (y != last_y) {
                num_rows++;
                last_y = y;
                row_children = g_list_append(NULL, child);
            } else {
                row_children = g_list_append(row_children, child);
            }
        }

        g_list_free(children);
        g_list_free(row_children);
    }

    return num_rows;
}
