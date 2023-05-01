#include "../inc/libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    for (size_t i = n - 1; i >= 0; i--) {
        if ((((__uint8_t *)s)[i]) == (__uint8_t)c)
            return (void *)((__uint8_t *)s + i);
    }
    return NULL;
}
