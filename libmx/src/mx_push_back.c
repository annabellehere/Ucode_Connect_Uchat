#include "../inc/libmx.h"

void mx_push_back(t_list **list, void *data) {
    if (list == NULL) return;
    t_list *n = mx_create_node(data);
    if (n == NULL) return;

    if (*list == NULL) {
        *list = n;
        return;
    }

    t_list *tmp = *list;
    while (tmp->next != NULL) tmp = tmp->next;
    tmp->next = n;
}

//int main() {}
