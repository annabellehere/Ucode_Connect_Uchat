#include<stdbool.h>
#include<stdlib.h>

char *mx_strnew(const int size);
void mx_strdel(char **str);
int mx_strlen(const char *s);
char *mx_strncpy(char *dst, const char *src, int len);
bool mx_isspace(int c);
char *mx_strtrim(const char *str);
char *mx_del_extra_spaces(const char *str);

char *mx_del_extra_spaces(const char *str) {
    char *trimmed = mx_strtrim(str);
    if (trimmed == NULL) return NULL;
    //exess whitespaces in the middle
    int exess = 0;
    for (int i = 0; i < mx_strlen(trimmed) - 1; i++) {
        if (mx_isspace(trimmed[i]) && mx_isspace(trimmed[i + 1])) {
            exess++;
        }
    }
    //alloc memory for str without exess
    char *res = mx_strnew(mx_strlen(trimmed) - exess);
    if (res == NULL) return NULL;
    //cur is cursor in resulting string
    int cur = 0;
    for(int i = 0; i < mx_strlen(trimmed); i++) {
        //if 2 whitespaces are found - move on w/out writing them down
        if(mx_isspace(trimmed[i]) && mx_isspace(trimmed[i + 1])) continue;
        if (!mx_isspace(trimmed[i])) 
            res[cur] = trimmed[i];
        else 
            res[cur] = ' ';
        cur++;
    }
    mx_strdel(&trimmed);
    return res;
}
/*
#include<stdio.h>
int main() {
    char test[] = "\f  The \t Matrix \r     has  you. \t\n  ";
    char *res = mx_del_extra_spaces(test);
    printf("%s\n", res);
    mx_strdel(&res);
}
*/

