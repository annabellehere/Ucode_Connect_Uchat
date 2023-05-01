#include "../inc/libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if ((((__uint8_t *)s)[i]) == (__uint8_t)c)
            return (void *)((__uint8_t *)s + i);
    }
    return NULL;
}
