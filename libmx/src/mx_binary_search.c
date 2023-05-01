#include "../inc/libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    int low = 0;
    int high = size - 1;
    int mid = (high + low) / 2;
    *count = 0;

    while (low <= high) {
        *count = *count + 1;
        mid = low + (high - low) / 2;
        if (mx_strcmp(s, arr[mid]) == 0) {
            return mid;
        }
        if (mx_strcmp(s, arr[mid]) > 0) {
            low = mid  + 1;
        }
        else {
            high = mid - 1;
        }
    }

    
    *count = 0;
    return -1;
}
/*
#include<stdio.h>
int main() {
    char *arr[] = {"1", "2", "3", "4", "5", "6"};
    int count = 0;
    int res = mx_binary_search(arr, 6, "4", &count);
    printf("%i %i\n", res, count);
    res = mx_binary_search(arr, 6, "3", &count);
    printf("%i %i\n", res, count);
    res = mx_binary_search(arr, 6, "7", &count);
    printf("%i %i\n", res, count);
}
*/

