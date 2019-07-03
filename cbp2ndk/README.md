
[![Build Travis](https://travis-ci.com/ClnViewer/Code-Blocks-Android-NDK.svg)](https://travis-ci.com/ClnViewer/Code-Blocks-Android-NDK)

# cbp2ndk  
## Code::Blocks to Android NDK configuration converter


Описание работы __cbp2ndk__ [EN](https://clnviewer.github.io/Code-Blocks-Android-NDK/CBP2NDK.EN.html) | [RU](https://clnviewer.github.io/Code-Blocks-Android-NDK/CBP2NDK.RU.html)  
Скачать `Code::Blocks` to `Android NDK` [configuration converter](https://clnviewer.github.io/Code-Blocks-Android-NDK/cbp2ndk.zip) v.0.0.14.79 (03.07.2019)  


### Параметры командной строки:


        Options:
           -a, --auto      find .cbp project file from current directory
           -c, --cbp       path to .cbp project file
           -d, --dump      dump current configuration
           -t, --tag       building tag: Debug|Release|OtherTag
           -q, --quiet     quiet all messages
           -v, --verbose   verbose output to console
           -n  --nodefault no set default values (libs, include paths)
               --cbtmpl    install C::B wizard template Makefile file
               --api       android API number (Application.mk)
               --abi       android ABI platform (Application.mk)
               --ndkopt    android NDK options (Application.mk)

        Using:
           cbp2ndk.exe <BuildTag> <path\project.cbp>
           cbp2ndk.exe -t <BuildTag> -c <path\project.cbp> -v
           cbp2ndk.exe -a --api android-28 --abi armeabi-v7a --ndkopt debug
           cbp2ndk.exe -a


## License

 _MIT_

