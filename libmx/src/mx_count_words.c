//#include<stdio.h>
int mx_count_words(const char *str, char c);

int mx_count_words(const char *str, char c) {
    int count = 1;
    int i = 0;

    if(str[0] == c) count--;

    while (str[i] != '\0') {
        if (str[i] == c && str[i + 1] == c) {
            i++;
            continue;
        }

        if(str[i] == c) count++;
        
        if(str[i] == c && str[i + 1] == '\0') count--;
        i++;
    }

    if (count < 0) return 0;
    return count;
}
/*
int main() {
    printf("%i %i", mx_count_words("  follow *  the  White rabbit ", ' '), mx_count_words("  follow *  the  White rabbit ", '*'));
}*/
