#include "../inc/libmx.h"

void *mx_memset(void *b, int c, size_t len) {
    __uint8_t *s = (__uint8_t *)b;
    for (size_t i = 0; i < len; i++) {
        s[i] = (__uint8_t)c;
    }
    return b;
}
