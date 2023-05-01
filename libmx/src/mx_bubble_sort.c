int mx_strcmp(const char *s1, const char *s2);

int mx_bubble_sort(char **arr, int size) {
    int count = 0;
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - 1 - i; j++)
        {
            if (mx_strcmp(arr[j], arr[j + 1]) > 0) {
                char *tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                count++;
            }
        } 
    }
    return count;
}
/*
#include <stdio.h>
int main() {
    char *arr[] = {"abc", "xyz", "ghi", "def"};
    int count = mx_bubble_sort(arr, 4);
    for (int i = 0; i < 4; i++)
    {
        printf("%s ", arr[i]);
    }
    printf("%i ", count);
    
}
*/
