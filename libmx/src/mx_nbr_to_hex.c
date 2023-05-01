#include"../inc/libmx.h"

char *mx_nbr_to_hex(unsigned long nbr) {
    unsigned long rem = 0;
    char hex_num[10000];
    unsigned long num_len = 0;

    for (; nbr > 0; num_len++) {
        rem = nbr % 16;
        nbr = nbr / 16;
        if (rem >= 0 && rem <= 9) hex_num[num_len] = rem + 48;
        else hex_num[num_len] = rem + 87;
    } 
    
    char *res = mx_strnew(num_len);
    if (res == NULL) return NULL;

    for (int i = num_len - 1; i >= 0; i--) {
        res[num_len - 1 - i] = hex_num[i];
    }

    if (num_len == 0) res[0] = '0';

    return res;
}
/*
#include<stdio.h>
int main() {
    printf("%s", mx_nbr_to_hex(1000));
}
*/
