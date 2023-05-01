#include "../inc/libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((__uint8_t *)dst)[i] = ((__uint8_t *)src)[i];
    }
    return dst;
}
