//#include<stdio.h>
int mx_strncmp(const char *s1, const char *s2, int n);

int mx_strncmp(const char *s1, const char *s2, int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i]) return s1[i] - s2[i];
        if (s1[i] == '\0' && s2[i] == '\0') return 0;
    }
    return 0;
}
/*
int main() {
    printf("%i %i %i", mx_strncmp("abc", "abcd", 10), mx_strncmp("abc", "abcd", 2), mx_strncmp("abc", "abc", 10));
}
*/
