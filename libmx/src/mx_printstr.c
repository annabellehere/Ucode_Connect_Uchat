#include<unistd.h>

int mx_strlen(const char *s);

void mx_printstr(const char *s) {
    write(STDOUT_FILENO, s, mx_strlen(s));
}


