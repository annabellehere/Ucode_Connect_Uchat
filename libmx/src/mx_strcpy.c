//#include<stdio.h>
char *mx_strcpy(char *dst, const char *src);

char *mx_strcpy(char *dst, const char *src) {
    int i = 0;

    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }

    while (dst[i] != '\0') {
        dst[i] = '\0';
        i++;
    }

    return dst;
}
/*
int main() {
    
    char test[] = "1234567890";
    printf("%s", mx_strcpy(test, "abcdef"));
}
*/

