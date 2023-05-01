#include "../inc/libmx.h"

char *mx_strndup(const char *s1, size_t n) {
    size_t len = mx_strlen(s1);
    if (len > n) len = n;
    char *duplicate = mx_strnew(len);
    if (duplicate == NULL) return NULL;

    for(size_t i = 0; i < len; i++) {
        duplicate[i] = s1[i];
    }
    duplicate[len] = '\0';
    return duplicate;
}
