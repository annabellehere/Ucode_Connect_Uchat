#include "../inc/libmx.h"

static char *append_symbol(char *str, char c){
    int len;
    if (str == NULL) {
        len = 0;
    }
    else len = mx_strlen(str);

    char *new = mx_strnew(len + 1);
    if (new == NULL) return NULL;
    if (str != NULL) mx_strcpy(new, str);
    free(str);
    new[len] = c;
    return new;
}

char *mx_getline(int fd, char delim) {
    char *str = NULL;
    char buff[1];
    int chars_read;

    while ((chars_read = read(fd, buff, 1)) == 1) {
        if (buff[0] == delim) {
            str = append_symbol(str, '\0');
            break;
        }
        str = append_symbol(str, buff[0]);
    }
    if (chars_read == -1) {
        free(str);
        return NULL;
    }
    return str;
}
