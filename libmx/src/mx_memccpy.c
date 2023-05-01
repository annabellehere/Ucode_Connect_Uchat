#include "../inc/libmx.h"

void *mx_memccpy(void *restrict dst,
                 const void *restrict src,
                 int c,
                 size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((__uint8_t *)dst)[i] = ((__uint8_t *)src)[i];
        if (((__uint8_t *)src)[i] == (__uint8_t)c) {
            return (void *)((__uint8_t *)dst + i + 1);
        }
    }
    return NULL;
}
