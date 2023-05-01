void mx_printchar(char c);
void mx_printstr(const char *s);

void mx_printint(int n) {
    int digit = 0;
    int n_digits = 0;
    int tmp;
    
    if (n == -2147483648) {
        mx_printstr("-2147483648");
        return;
    }
    if (n < 0) mx_printchar('-');
    if (n < 0) n = -n;
    if (n == 0) {
        mx_printchar(48);
        return;
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
        mx_printchar(digit + 48);
    }

}
/*
int main() {
    mx_printint(123456);
}
*/
