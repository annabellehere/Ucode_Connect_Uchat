//#include<stdio.h>

char *mx_strchr(const char *s, int c);
int mx_strlen(const char *s);
int mx_strncmp(const char *s1, const char *s2, int n);
char *mx_strstr(const char *haystack, const char *needle);

char *mx_strstr(const char *haystack, const char *needle) {
    int len = mx_strlen(needle);
    
    for (char *found = mx_strchr(haystack, needle[0]);
        found != (void *)0;
        found = mx_strchr(found + 1, needle[0])) {
        if (found == (void *)0) return (void *)0;
        if (mx_strncmp(found, needle, len) == 0) return found;
    }
    return (void *)0;

}
/*
int main() {
    printf("%s %s", mx_strstr("Matrix", "tr"),mx_strstr("Matrix", "Neo"));
}*/
