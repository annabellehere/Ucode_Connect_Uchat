#include "../inc/libmx.h"

void mx_push_front(t_list **list, void *data) {
    t_list *n = mx_create_node(data);
    if (n == NULL) return;
    n->next = *list;
    *list = n;
}
