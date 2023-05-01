#include<stdlib.h>

int mx_strlen(const char *s);
char *mx_strnew(const int size);
char *mx_strcpy(char *dst, const char *src);
char *mx_strdup(const char *s1);

char *mx_strdup(const char *s1) {
    int len = mx_strlen(s1);
    char *duplicate = mx_strnew(len);
    if (duplicate == NULL) return NULL;

    mx_strcpy(duplicate, s1);
    return duplicate;
}

//int main() {}
