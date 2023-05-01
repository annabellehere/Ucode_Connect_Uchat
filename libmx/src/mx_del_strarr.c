#include<stdlib.h>


void mx_strdel(char **str);
void mx_del_strarr(char ***arr);

void mx_del_strarr(char ***arr) {
    if (arr == NULL || *arr == NULL) return;
    for(int i = 0; (*arr)[i] != NULL; i++) {
        mx_strdel(*arr + i);
    }
    free(*arr);
    *arr = NULL;
}
/*
#include<stdio.h>
#include <string.h>
int main() {
    char **strarr = malloc(sizeof(char *) * 4);
    strarr[0] = malloc(5);
    strarr[1] = malloc(6);
    strarr[2] = malloc(7);
    strcpy(strarr[0], "123");
    strcpy(strarr[1], "1234");
    strcpy(strarr[2], "123456");
    strarr[3] = NULL;
    //printf("%p\n", (void *)strarr);
    mx_del_strarr(&strarr);
    printf("%p\n", (void *)strarr);
}
*/

