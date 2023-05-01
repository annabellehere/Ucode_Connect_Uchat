#include "../inc/libmx.h"

static char *append_symbol(char *str, char c);

char *mx_file_to_str(const char *file) {
    int file_n = open(file, O_RDONLY);
    if (file_n == -1) return NULL;
    char *str = NULL;
    char buff[1];
    int chars_read;

    while ((chars_read = read(file_n, buff, 1)) == 1) {
        str = append_symbol(str, buff[0]);
    }
    if (chars_read == -1) {
        free(str);
        close(file_n);
        return NULL;
    }
    close(file_n);
    return str;


}
/*
#include<stdio.h>
int main() {
    char *str = mx_file_to_str("file_to_str.h");
    int i = 0;
    while(str[i] != '\0') {
        write(STDOUT_FILENO, str + i, 1);
        i++;
    }
    free(str);
}
*/
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


