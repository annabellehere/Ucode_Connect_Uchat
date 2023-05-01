//#include<stdio.h>
int mx_sqrt(int x) {
    int i = 0;
    if (x <= 0) return 0;

    for (i = 1; (long)((long)i * i) < (long)x; i++) {}

    if ((long)((long)i * i) == (long)x) return i;
    return 0;
}

/*
int main() {
    printf("%i %i", mx_sqrt(2147483647), mx_sqrt(4));
}
*/
