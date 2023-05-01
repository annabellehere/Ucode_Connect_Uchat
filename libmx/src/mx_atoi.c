#include "../inc/libmx.h"

int mx_atoi(const char *str) {
    int sign = 1, result = 0, cur = 0;
    //ignore spaces
    while (mx_isspace(str[cur])) cur++;

    if (str[cur] == '-') {
        sign = -1;
        cur++;
    }
    if (str[cur] == '+') {
        cur++;
    }

    while (mx_isdigit(str[cur])) {
        /*//overflow check
        if (result > __INT_MAX__ / 10 
            || (result == __INT_MAX__ / 10 
            && str[cur] - '0' > 8)) {
                if (sign == 1) return -1;
                else return 0;
            } */
        result = result * 10 + (str[cur] - '0');
        cur++;
    }

    return result * sign;
}
/*
int main() {
    printf("%i %i %i", mx_atoi("1234"), mx_atoi("  -567"), mx_atoi("2200000000"));
}
*/
