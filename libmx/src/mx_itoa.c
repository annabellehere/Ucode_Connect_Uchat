#include "../inc/libmx.h"

static void append_to_str(char *str, char c) {
    int i = 0;
    while (str[i] != '\0') i++;
    str[i] = c;
}

char *mx_itoa(int number) {
    int digit = 0;
    int n_digits = 0;
    int tmp;
    int n = number;
    
    
    if (n == -2147483648) {
        return mx_strdup("-2147483648");
    }

    char *res = mx_strnew(11);

    if (n < 0) append_to_str(res, '-');
    if (n < 0) n = -n;
    if (n == 0) {
        append_to_str(res, '0');
        return res;
    }
    tmp = n;

    while (tmp != 0) {
        n_digits++;
        tmp = tmp / 10;
    }
    
    for (int i = n_digits - 1; i >= 0; i--) {
        tmp = n;
        for (int j = 0; j < i; j++) {
            tmp = tmp / 10;
        }
        digit = tmp % 10;
        append_to_str(res, digit + '0');
    }
    return res;
}
