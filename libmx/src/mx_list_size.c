#include "../inc/libmx.h"

int mx_list_size(t_list *list) {
    if (list == NULL) return 0;
    int count = 0;
    t_list *tmp = list;
    while (tmp != NULL) {
        tmp = tmp->next;
        count++;
    }
    return count;
}

//int main() {}
