
#include "hdk-main.h"

/*
    DEBUG GDB test,
    manual run gdb.exe and type:
        set solib-search-path obj/local/armeabi-v7a
        file obj/local/armeabi-v7a/hello_world
        target remote localhost:59999
        b test
        c
        n
        q
*/

int main(void)
{
    int a = 18;
    int b = a;

    test(b);
    printf("Hello world! (%d)\n", b);

    return 0;
}
