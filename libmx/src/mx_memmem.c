#include "../inc/libmx.h"

void *mx_memmem(const void *big, size_t big_len,
                const void *little, size_t little_len) {
    void *sub_start = NULL;
    for (sub_start = mx_memchr(big, ((__uint8_t *)little)[0], big_len);
         sub_start != NULL;
         sub_start = mx_memchr((void *)((__uint8_t *)sub_start + 1),
                             ((__uint8_t *)little)[0],
                             big_len - ((__uint8_t *)sub_start - (__uint8_t *)big))) {

        if (big_len - ((__uint8_t *)sub_start - (__uint8_t *)big) < little_len) 
            return NULL;
        if (mx_memcmp(sub_start, little, little_len) == 0)
            return sub_start;
    }
    return NULL;
}
