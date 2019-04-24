Начнем с малого, как прикрутить `Android NDK` к `C::B`

Есть два пути:

- 1. прописать на каждую платформу свой профиль, с исполняемыми файлами согласно платформе. На мой взгляд утомительно, много всего и непонятно зачем. С учетом что если собирать одно и тоже под все платформы, то объем телодвижений с переключением конфигураций довольно велик.
У меня по умолчанию собираются `arm64-v8a, armeabi-v7a, x86, x86_64`.

- 2. использовать родную систему сборки `ndk-build`. Это самое простое и изящное решение, не вступающее в конфликты с большинством проверок `C::B`, но, в настройках `C::B` важна каждая деталь, схема довольно капризная, и при неточностях может легко ломаться.


### Интеграция Android NDK

----------

Исходя из этого, приведу файлы настроек целиком, во первых, сам проект `C::B`, самая важная часть находится в секции `<Build>`:

- `Option output` указывает на скрипт дистанционного запуска приложения на устройстве - `RunRemote.cmd`. Скрипт генерируется автоматически.  
- `compiler="android_ndk-build"` - это обработанное название компилятора в настройках `C::B` - `Android NDK-Build`. Как завести учетную запись нового компилятора, показано ниже.

Файл проекта `C::B` **AndroidNdkTemplate.cbp**

    <?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
    <CodeBlocks_project_file>
    	<FileVersion major="1" minor="6" />
    	<Project>
    		<Option title="C::B Android Ndk project Template" />
    		<Option makefile_is_custom="1" />
    		<Option pch_mode="2" />
    		<Option compiler="android_ndk-build" />
    		<Option check_files="0" />
    		<Build>
    			<Target title="Release">
    				<Option output="RunRemote.cmd" prefix_auto="0" extension_auto="0" />
    				<Option working_dir="" />
    				<Option type="1" />
    				<Option compiler="android_ndk-build" />
    			</Target>
    			<Target title="Debug">
    				<Option output="RunRemote.cmd" prefix_auto="0" extension_auto="0" />
    				<Option working_dir="" />
    				<Option type="1" />
    				<Option compiler="android_ndk-build" />
    			</Target>
    		</Build>
    		<Compiler>
    			<Add option="-Wall" />
    		</Compiler>
    		<Unit filename="main.c">
    			<Option compilerVar="CC" />
    		</Unit>
    		<Extensions>
    			<code_completion />
    			<envvars />
    			<debugger>
    				<remote_debugging target="Debug">
    					<options conn_type="0" serial_baud="0" ip_address="127.0.0.1" ip_port="9999" extended_remote="1" />
    				</remote_debugging>
    			</debugger>
    		</Extensions>
    	</Project>
    </CodeBlocks_project_file>

Исходные **файлы управления сборкой** в директории `NDK` проекта:

**Application.mk**

    APP_ABI := all
    APP_STL := c++_static
    APP_PLATFORM := android-22
    APP_BUILD_SCRIPT := Android.mk

**Android.mk** - собственно и является мейк-файлом уникальным для каждого `NDK` проекта (приложения):

    LOCAL_PATH := $(call my-dir)
    include $(CLEAR_VARS)
    LOCAL_MODULE    := hello_world
    LOCAL_SRC_FILES := ./main.c
    LOCAL_C_INCLUDES := ./
    include $(BUILD_EXECUTABLE)

**Makefile** - непосредственно запускается `C::B`:

    PLATFORM := armeabi-v7a
    NDKROOT  := C:\__BuildSource\__LIB__\android-ndk-r20-beta2
    PROJECT  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
    BUILDTAG := $(filter-out $@,$(MAKECMDGOALS))
    BUILDOPT := 
    
    include Application.mk
    include $(APP_BUILD_SCRIPT)
    
    ifneq ($(APP_ABI),all)
    	PLATFORM = $(APP_ABI)
    endif
    
    ifeq ($(BUILDTAG),Debug)
    	BUILDOPT = NDK_DEBUG=1
    else
    	BUILDOPT = -j 4
    endif
    
    all: allndk
    Debug: allndk adbsetup adbdebug buildscript
    Release: allndk adbsetup adbexec buildscript
    cleanDebug: clean
    cleanRelease: clean
    cleanall: clean
    
    allndk:
    	@echo '==== Build $(BUILDTAG) -> $(APP_ABI) platform -> active device: [ $(PLATFORM) ] ===='
    	@Cmd.exe /C $(NDKROOT)\ndk-build.cmd NDK_APPLICATION_MK=$(PROJECT)Application.mk NDK_PROJECT_PATH=$(PROJECT) $(BUILDOPT)
    
    clean:
    	@echo '==== Clean ===='
    	@Cmd.exe /C $(NDKROOT)\ndk-build.cmd NDK_APPLICATION_MK=$(PROJECT)Application.mk NDK_PROJECT_PATH=$(PROJECT) clean
    	@Cmd.exe /C adb.exe shell rm -f /data/local/tmp/$(LOCAL_MODULE)
    
    adbsetup:
    	@echo '==== ADB SETUP: [ $(PLATFORM) ] ===='
    	@Cmd.exe /C adb.exe push $(PROJECT)libs\$(PLATFORM)\$(LOCAL_MODULE) /data/local/tmp/$(LOCAL_MODULE)
    	@Cmd.exe /C adb.exe shell /system/bin/chmod 0777 /data/local/tmp/$(LOCAL_MODULE)
    
    adbexec:
    	@echo '==== ADB RUN: [ $(PLATFORM) ] ===='
    	@Cmd.exe /C adb.exe shell /data/local/tmp/$(LOCAL_MODULE)
    
    adbdebug:
    	@echo '==== GDB Debug: [ $(PLATFORM) ] ===='
    	@Cmd.exe /C adb.exe push $(PROJECT)libs\$(PLATFORM)\gdb.setup /data/local/tmp/gdb.setup
    	@Cmd.exe /C adb.exe push $(PROJECT)libs\$(PLATFORM)\gdbserver /data/local/tmp/gdbserver
    	@Cmd.exe /C adb.exe shell /system/bin/chmod 0777 /data/local/tmp/gdbserver
    
    buildscript:
    	@echo "adb.exe shell /data/local/tmp/$(LOCAL_MODULE)" >RunRemote.cmd
    	@echo "adb.exe shell /data/local/tmp/gdbserver :9999 /data/local/tmp/$(LOCAL_MODULE)" >DebugRemote.cmd
    
    .PHONY: clean all

Файлы `Application.mk` и `Makefile` являются универсальными для всех проектов собираемых с помощью `NDK` и не требуют правок.

Для понимания структуры где чего лежит в проекте `NDK` приложения, приведу дерево:

    │   Android.mk
    │   AndroidNdkTemplate.cbp
    │   Application.mk
    │   main.c
    │   Makefile
    ├───libs
    │   ├───arm64-v8a
    │   │       hello_world
    │   ├───armeabi-v7a
    │   │       hello_world
    │   ├───x86
    │   │       hello_world
    │   └───x86_64
    │           hello_world
    └───obj
        └───local
            ├───arm64-v8a
            │   │   hello_world
            │   └───objs
            │       └───hello_world
            │               main.o
            │               main.o.d
            ├───armeabi-v7a
            │   │   hello_world
            │   └───objs
            │       └───hello_world
            │               main.o
            │               main.o.d
            ├───x86
            │   │   hello_world
            │   └───objs
            │       └───hello_world
            │               main.o
            │               main.o.d
            └───x86_64
                │   hello_world
                └───objs
                    └───hello_world
                            main.o
                            main.o.d


### Вид настроек проекта из GUI `C::B`:

----------

![Image1](img/Image1.png)

![Image2](img/Image2.png)

![Image3](img/Image3.png)

![Image4](img/Image4.png)

### Вид настроек компилятора в `C::B`:

----------

[![введите сюда описание изображения][5]][5]

[![введите сюда описание изображения][6]][6]

[![введите сюда описание изображения][7]][7]


### Дополнительное меню отладки `NDK` приложения:

----------

Оба скрипта имеют фиксированное имя и генерируются автоматически, во время исполнения `Makefile`, для удобства разумно добавить их в меню:

[![введите сюда описание изображения][8]][8]

[![введите сюда описание изображения][9]][9]

Метод отладки приложения состоит в типовых действиях, например через `F8`, как только запустился дебагер, вам надо вызвать из созданного меню пункт `ADB Debug Remote server`, этой командой вы запустите `GDB сервер` на устройстве, который запустит ваше приложение. Вы подключаетесь к `GDB серверу` дистанционно и можете проводить сеанс отладки. Смотрите скриншот настроек дебагера в проекте выше.

Глобальные настройки дебагера для `NDK` выглядят так:

[![введите сюда описание изображения][10]][10]

### Результат сборки `NDK` приложения:

----------

[![введите сюда описание изображения][11]][11]

Преимущества использования NDK в отличии от статической сборки под определенную платформу:

- Не нужно компилировать двоичные файлы статически, поэтому размер выходного двоичного файла будет меньше.

- Можно использовать android `C/C++` библиотеки, такие как liblog, чтобы иметь возможность иметь вывод в logcat из аппликации.

Полный код `NDK C::B template` разместил на [github](https://github.com/ClnViewer/Code-Blocks-Android-NDK)

