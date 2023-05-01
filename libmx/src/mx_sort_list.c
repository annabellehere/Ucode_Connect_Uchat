#include "../inc/libmx.h"

t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *a, void *b)) {
    if (lst == NULL || cmp == NULL) return lst;
    bool is_unsorted = true;
    while (is_unsorted) {
        is_unsorted = false;
        for(t_list *tmp = lst; tmp->next != NULL; tmp = tmp->next) {
            t_list *cur = tmp;
            t_list *nxt = tmp->next;
            if (cmp(cur->data, nxt->data)) {
                is_unsorted = true;
                void *tmp_data = cur->data;
                cur->data = nxt->data;
                nxt->data = tmp_data;
            }
        }
    }
    return lst;
}

//int main() {}
