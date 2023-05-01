#include "../inc/libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len) {
    __uint8_t buff[len];
    for (size_t i = 0; i < len; i++) {
        buff[i] = ((__uint8_t *)src)[i];
    }
    for (size_t i = 0; i < len; i++) {
        ((__uint8_t *)dst)[i] = buff[i];
    }
    return dst;
}
