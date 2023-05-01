#include "../inc/libmx.h"


char *mx_strjoin(char const *s1, char const *s2) {
    if (s1 == NULL && s2 == NULL) return NULL;
    if (s1 == NULL && s2 != NULL) {
        char *res = mx_strdup(s2);
        if (res == NULL) return NULL;
        return res;
    }
    if (s1 != NULL && s2 == NULL) {
        char *res = mx_strdup(s1);
        if (res == NULL) return NULL;
        return res;
    }

    char *res = mx_strnew(mx_strlen(s1) + mx_strlen(s2));
    if (res == NULL) return NULL;

    mx_strcpy(res, s1);
    mx_strcat(res, s2);

    return res;
}
/*
#include<stdio.h>
int main() {
    char *test = mx_strjoin("Mat", "rix");
    printf("%s", test);
}*/
