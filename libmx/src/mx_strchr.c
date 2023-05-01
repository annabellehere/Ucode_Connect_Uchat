//#include<stdio.h>
char *mx_strchr(const char *s, int c);

char *mx_strchr(const char *s, int c) {
    //(char *)(void *)0 is NULL pointer
    if (s == (char *)(void *)0) return (char *)(void *)0;
    if (c == '\0') {
        int i = 0;
        while (s[i] != '\0') i++;
        return (char *)(s + i);
    }

    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == c) return (char *)(s + i);
    }
    return (char *)(void *)0;
}
/*
int main() {
    char test[] = "Matrix";
    printf("%s %s", test, mx_strchr(test, 'a'));
}
*/

