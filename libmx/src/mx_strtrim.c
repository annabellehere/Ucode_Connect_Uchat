#include "../inc/libmx.h"


char *mx_strtrim(const char *str) {
    if (str == NULL) return NULL;

    int ws_count = 0;
    int i;
    for (i = 0; mx_isspace(str[i]); i++) ws_count++;
    for (int j = mx_strlen(str) - 1; mx_isspace(str[j]); j--) ws_count++;

    char *res = mx_strnew(mx_strlen(str) - ws_count);
    if (res == NULL) return NULL;

    mx_strncpy(res, str + i, mx_strlen(str) - ws_count);
    return res;
}
/*
#include<stdio.h>
int main() {
    char test[] = "  Matrix  ";
    char *str = mx_strtrim(test);
    printf("%s\n", str);
    mx_strdel(&str);
}
*/

