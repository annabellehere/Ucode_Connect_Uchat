#include "../inc/libmx.h"

static void append_to_str(char *str, char c) {
    str[mx_strlen(str)] = c;
}

char **mx_strsplit(const char *s, char c) {
    if (s == NULL) return NULL;

    int nwords = mx_count_words(s, c);
    char **result = malloc(sizeof(char *) * (nwords + 1));
    if (result == NULL) return NULL;

    result[nwords] = NULL;
    char buff[mx_strlen(s) + 1];
    //init buffer
    for (int i = 0; i < mx_strlen(s) + 1; i++) {
        buff[i] = '\0';
    }

    char cur_char;
    int word_number = 0;
    for (int i = 0; i < mx_strlen(s); i++) {
        cur_char = s[i];
        //if reached delim and buffer is not empty
        //dump buffer into result array
        if(cur_char == c && mx_strlen(buff) != 0) {
            result[word_number] = mx_strdup(buff);
            if (result[word_number] == NULL) return NULL;

            word_number++;
            for (int j = 0; j < mx_strlen(s) + 1; j++) {
                buff[j] = '\0';
            }
        }
        //append symbol to buffer
        if (cur_char != c) {
            append_to_str(buff, cur_char);
        }
    }
    //if buffer has a word, if loop reached an end of str
    if (mx_strlen(buff) != 0) {
        result[word_number] = mx_strdup(buff);
    }

    return result;
}

/*
int main() {
    char str[] = "    Knock, knock,    Neo.   ";
    char **strarr = mx_strsplit(str, ' ');
    mx_print_strarr(strarr, " ");
    for (int i = 0; strarr[i] != NULL; i++) {
        mx_printstr(strarr[i]);
        mx_printchar('\n');
    }
    
}
*/

