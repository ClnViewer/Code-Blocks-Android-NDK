
#include "hdk-main.h"
#include <android/log.h>

extern "C" void test(int b)
{
    __android_log_print(ANDROID_LOG_DEBUG  , "~~~~~~", "log %i", b);
}
