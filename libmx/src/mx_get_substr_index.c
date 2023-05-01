#include "../inc/libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    if (str == NULL || sub == NULL) return -2;
    char *substr = mx_strstr(str, sub);
    if (substr == NULL) return -1;
    return substr - str;
}
