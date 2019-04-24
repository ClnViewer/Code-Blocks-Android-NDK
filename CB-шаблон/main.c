#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int a = 0;
    int b = a;

    printf("Hello world! (%d)\n", b);

    return 0;
}
