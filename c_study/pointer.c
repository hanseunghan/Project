#include <stdio.h>

int main()
{
    int a[4] = {1,2,3,4};
    int *p = a;

    printf("sizeof(p)=%lu\n", sizeof(p));
    printf("sizeof(int*)=%lu\n", sizeof(int*));

    printf("sizeof(a)=%lu\n", sizeof(a));
    printf("sizeof(int[4])=%lu\n", sizeof(int[4]));

    return 0;
}