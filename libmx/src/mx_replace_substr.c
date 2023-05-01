#include "../inc/libmx.h"

static int get_new_str_len(const char *str, const char *sub, const char *replace) {
    char *next_sub = mx_strstr(str, sub);
    int newlen = mx_strlen(str);
    int sublen = mx_strlen(sub);
    int replen = mx_strlen(replace);

    while (next_sub != NULL) {
        newlen = newlen - sublen + replen;
        next_sub = mx_strstr(next_sub + 1, sub);
    }

    return newlen;
}

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if (str == NULL || sub == NULL || replace == NULL) return NULL;
    int strlen = mx_strlen(str);
    int sublen = mx_strlen(sub);
    int replen = mx_strlen(replace);
    char *result = mx_strnew(get_new_str_len(str, sub, replace));
    int res_cursor = 0;
    if (result == NULL) return NULL;
    
    int next_sub = mx_get_substr_index(str, sub);

    if (next_sub == -1) {
        return mx_strdup(str);
    }

    for (int i = 0; i < strlen; i++) {
        if (i == next_sub) {
            mx_strcpy(result + res_cursor, replace);
            i += sublen;
            res_cursor += replen;
            next_sub = mx_get_substr_index(str + i, sub) + i;
            i--;
            continue;
        }
        result[res_cursor] = str[i];
        res_cursor++;
    }

    return result;

}

/*
int main() {
    mx_printstr(mx_replace_substr("Ururu turu", "ru", "ta"));
    mx_printchar('\n');
    mx_printstr(mx_replace_substr("McDonalds", "alds", "uts"));
    mx_printchar('\n');
    mx_printstr(mx_replace_substr("Ururu turu", "ru", "a"));
    mx_printchar('\n');
    mx_printstr(mx_replace_substr("McDonalds", "Pan", "uts"));
}
*/
