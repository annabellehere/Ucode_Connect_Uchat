#include "../inc/libmx.h"

void mx_pop_back(t_list **head) {
    if (head == NULL) return;
    t_list *cur = *head;
    if (cur == NULL) return;
    
    if (cur->next == NULL) {
        free(cur);
        *head = NULL;
        return;
    }

    while (cur->next->next != NULL) 
        cur = cur->next;
    free(cur->next);
    cur->next  = NULL;
}

//int main() {}
