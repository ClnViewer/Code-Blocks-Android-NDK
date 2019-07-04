#
#    MIT License
#
#    Code::Blocks + Android NDK project template
#    Copyright (c) 2016-2019 PS
#    HOWTO: https://clnviewer.github.io/Code-Blocks-Android-NDK/
#
#    - No root device!
#    - Build Debug (NDK toolchain)
#    - Build Release (NDK toolchain)
#    - Run from device
#    - Debug from device. 
#    - Auto start/stop gdbserverfrom device.
#    - No wrappers from Gradle/Java code required, works directly with the device.  
#    - Advanced project export to Android.mk file format, possible using utility cbp2ndk. 
#
#    Permission is hereby granted, free of charge, to any person obtaining a copy
#    of this software and associated documentation files (the "Software"), to deal
#    in the Software without restriction, including without limitation the rights
#    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
#    copies of the Software, and to permit persons to whom the Software is
#    furnished to do so, subject to the following conditions:
#
#    The above copyright notice and this permission notice shall be included in all
#    copies or substantial portions of the Software.
#
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#    SOFTWARE.

# Variables PLATFORM* to Application.mk

PLATFORM_ABI := $(SC_NDK_ABI)
PLATFORM_API := $(SC_NDK_API)
PLATFORM_STL := c++_static
PLATFORM_SCRIPT := Android.mk
PLATFORM_OPTIM  :=

# Variables local

DIR_UTILS    := $(SC_BIN_PATH)
DIR_NDKROOT  := $(SC_NDK_DIR)
DIR_PROJECT  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
STR_BUILDTAG := $(filter-out $@,$(MAKECMDGOALS))
STR_BUILDOPT :=

ifeq ($(OS),Windows_NT)
	DIR_CONFCBP  := $(shell dir *.cbp | findstr .cbp)
else
	DIR_CONFCBP  := $(shell find $("./") -name '*.cbp')
endif

ifneq ($(PLATFORM_ABI),all)
	PLATFORM_ABI = armeabi-v7a
endif

ifneq ($(ANALIZE),)
	STR_BUILDOPT += NDK_ANALIZE=1
endif

ifeq ($(STR_BUILDTAG),Debug)
	STR_BUILDOPT += V=1 NDK_DEBUG=1
	PLATFORM_OPTIM = debug
else
	STR_BUILDOPT += -j 4
endif

all: setndk buildndk
Debug: setndk buildndk adbsetup adbdebug buildscript rundebug
Release: setndk buildndk adbsetup adbexec buildscript
cleanDebug: setndk clean
cleanRelease: setndk clean
cleanall: setndk clean

setndk:
	@echo '==== SET C::B NDK -> $(SC_NDK_API) -> $(SC_NDK_ABI) -> active device: [ $(PLATFORM_ABI) ] ===='
	@$(DIR_UTILS)/exec/cbp2ndk32.exe -t $(STR_BUILDTAG) -c $(DIR_CONFCBP) --api $(PLATFORM_API) --abi $(PLATFORM_ABI) --ndkopt $(PLATFORM_OPTIM)
-include $(PLATFORM_SCRIPT)

buildndk:
	@echo '==== Build $(STR_BUILDTAG) -> $(PLATFORM_ABI) platform -> $(SC_NDK_API) API ===='
	@Cmd.exe /C $(DIR_NDKROOT)\ndk-build.cmd NDK_APPLICATION_MK=$(DIR_PROJECT)Application.mk NDK_PROJECT_PATH=$(DIR_PROJECT) $(STR_BUILDOPT)

clean:
	@echo '==== Clean ===='
	@Cmd.exe /C $(DIR_NDKROOT)\ndk-build.cmd NDK_APPLICATION_MK=$(DIR_PROJECT)Application.mk NDK_PROJECT_PATH=$(DIR_PROJECT) clean
	@Cmd.exe /C adb.exe shell rm -f /data/local/tmp/$(LOCAL_MODULE)

adbsetup:
	@echo '==== ADB SETUP: [ $(PLATFORM_ABI) ] ===='
	Cmd.exe /C $(DIR_UTILS)/exec/android-elf-cleaner32.exe $(DIR_PROJECT)libs\$(PLATFORM_ABI)\$(LOCAL_MODULE)
	Cmd.exe /C adb.exe push $(DIR_PROJECT)libs\$(PLATFORM_ABI)\$(LOCAL_MODULE) /data/local/tmp/$(LOCAL_MODULE)
	Cmd.exe /C adb.exe shell /system/bin/chmod 0777 /data/local/tmp/$(LOCAL_MODULE)

adbexec:
	@echo '==== ADB RUN: [ $(PLATFORM_ABI) ] ===='
	@Cmd.exe /C adb.exe shell /data/local/tmp/$(LOCAL_MODULE) $(CBP2NDK_CMDLINE)

adbdebug:
	@echo '==== GDB Debug: [ $(PLATFORM_ABI) ] ===='
	@Cmd.exe /C adb.exe push $(DIR_PROJECT)libs\$(PLATFORM_ABI)\gdb.setup /data/local/tmp/gdb.setup
	@Cmd.exe /C adb.exe push $(DIR_PROJECT)libs\$(PLATFORM_ABI)\gdbserver /data/local/tmp/gdbserver
	@Cmd.exe /C adb.exe shell /system/bin/chmod 0777 /data/local/tmp/gdbserver

rundebug:
	@Cmd.exe /C DebugRemote.cmd

buildscript:
ifeq (,$(wildcard ./RunRemote.cmd))
	@echo "adb.exe shell /data/local/tmp/$(LOCAL_MODULE) $(CBP2NDK_CMDLINE)" >RunRemote.cmd
endif
ifeq (,$(wildcard ./DebugRemote.cmd))
	@echo "adb.exe forward tcp:59999 tcp:59999" >DebugRemote.cmd
	@echo "start \"$(PLATFORM_ABI) GDB server\" /MIN adb.exe shell /data/local/tmp/gdbserver :59999 /data/local/tmp/$(LOCAL_MODULE) $(CBP2NDK_CMDLINE)" >>DebugRemote.cmd
	@echo "exit" >>DebugRemote.cmd
endif

.PHONY: clean all

