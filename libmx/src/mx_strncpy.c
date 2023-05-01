//#include<stdio.h>
char *mx_strncpy(char *dst, const char *src, int len);

char *mx_strncpy(char *dst, const char *src, int len) {
    int i = 0;

    for (i = 0; src[i] != '\0' && i < len; i++) {
        dst[i] = src[i];
    }
    while (i < len) {
        dst[i] = '\0';
        i++;
    }
    return dst;
}
/*
int main() {
    char test[] = "123456789";
    printf("%s", mx_strncpy(test, "Neo", 2));
    printf(" %s", mx_strncpy(test, "Pedro", 6));
}*/

