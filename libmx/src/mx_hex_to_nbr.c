#include<stdbool.h>

bool mx_isalpha(int c);
bool mx_isdigit(int c);
bool mx_islower(int c);
bool mx_isupper(int c);
unsigned long mx_hex_to_nbr(const char *hex);
/*
#include<stdio.h>
int main() {
    printf("%lu", mx_hex_to_nbr("FADE"));
}
*/
unsigned long mx_hex_to_nbr(const char *hex) {
    int len = 0;
    unsigned long res = 0, base = 1;

    for (int i = 0; hex[i] != '\0'; i++) len++;

    for (int i = len - 1; i >= 0; i--) {
        if(mx_isalpha(hex[i])) {
            if (mx_isupper(hex[i])
                && hex[i] >= 'A' && hex[i] <= 'F') {
                res += (hex[i] - 'A' + 10) * base;
                base *= 16;
            }
            else if (mx_islower(hex[i])
                && hex[i] >= 'a' && hex[i] <= 'f') {
                res += (hex[i] - 'a' + 10) * base;
                base *= 16;
            }
            else return 0;
        }
        else if (mx_isdigit(hex[i])) {
            res += (hex[i] - '0') * base;
            base *= 16;
        }
        else {
            return 0;
        }
    }
    return res;
}

bool mx_isalpha(int c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) return true;
    else return false;
}

bool mx_isdigit(int c) {
    if ((c >= 48 && c <= 57)) return true;
    else return false;
}

bool mx_islower(int c) {
    if ((c >= 'a' && c <= 'z')) return true;
    else return false;
}

bool mx_isupper(int c) {
    if (c >= 'A' && c <= 'Z') return true;
    else return false;
}



