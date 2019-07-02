
#include <string>
#include <iostream>

#include <stdio.h>
#include <unistd.h>

/*
    /// NDK Android native Project
    HOWTO: https://clnviewer.github.io/Code-Blocks-Android-NDK/
    AOSP heasers: https://github.com/ClnViewer/android-platform-headers/
    android-elf-cleaner: https://github.com/ClnViewer/termux-elf-cleaner/
    cbp2ndk: https://github.com/ClnViewer/Code-Blocks-Android-NDK/tree/master/cbp2ndk/

*/

int main(void)
{
    int a = 18;
    int b = a;

    std::cout  << std::endl << "\tHello Android! " <<  b << " - " << a << std::endl << std::endl;

    return 0;
}
