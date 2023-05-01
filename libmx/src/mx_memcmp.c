#include "../inc/libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (((__uint8_t *)s1)[i] != ((__uint8_t *)s2)[i])
            return (int)(((__uint8_t *)s1)[i] - ((__uint8_t *)s2)[i]);
            
        if (((__uint8_t *)s1)[i] == '\0' 
            && ((__uint8_t *)s2)[i] == '\0') 
            return 0;
    }
    return 0;
}
