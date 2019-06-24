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

PLATFORM := armeabi-v7a
NDKROOT  := C:\__BuildSource\__LIB__\android-ndk-r20-beta2
PROJECT  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BUILDTAG := $(filter-out $@,$(MAKECMDGOALS))
BUILDOPT := 

include Application.mk
include $(APP_BUILD_SCRIPT)

ifeq ($(OS),Windows_NT)
	CONFCBP  := $(shell dir *.cbp | findstr .cbp)
else
	CONFCBP  := $(shell find $("./") -name '*.cbp')
endif

ifneq ($(APP_ABI),all)
	PLATFORM = $(APP_ABI)
endif

ifneq ($(ANALIZE),)
	BUILDOPT += NDK_ANALIZE=1
endif

ifeq ($(BUILDTAG),Debug)
	BUILDOPT += V=1 NDK_DEBUG=1
else
	BUILDOPT += -j 4
endif

all: allndk
Debug: allndk adbsetup adbdebug buildscript rundebug
Release: allndk adbsetup adbexec buildscript
cleanDebug: clean
cleanRelease: clean
cleanall: clean

allndk:
	@echo '==== Build $(BUILDTAG) -> $(APP_ABI) platform -> active device: [ $(PLATFORM) ] ===='
	@cbp2ndk.exe $(BUILDTAG) $(CONFCBP)
	@Cmd.exe /C $(NDKROOT)\ndk-build.cmd NDK_APPLICATION_MK=$(PROJECT)Application.mk NDK_PROJECT_PATH=$(PROJECT) $(BUILDOPT)

clean:
	@echo '==== Clean ===='
	@Cmd.exe /C $(NDKROOT)\ndk-build.cmd NDK_APPLICATION_MK=$(PROJECT)Application.mk NDK_PROJECT_PATH=$(PROJECT) clean
	@Cmd.exe /C adb.exe shell rm -f /data/local/tmp/$(LOCAL_MODULE)

adbsetup:
	@echo '==== ADB SETUP: [ $(PLATFORM) ] ===='
	@Cmd.exe /C android-elf-cleaner.exe $(PROJECT)libs\$(PLATFORM)\$(LOCAL_MODULE)
	@Cmd.exe /C adb.exe push $(PROJECT)libs\$(PLATFORM)\$(LOCAL_MODULE) /data/local/tmp/$(LOCAL_MODULE)
	@Cmd.exe /C adb.exe shell /system/bin/chmod 0777 /data/local/tmp/$(LOCAL_MODULE)

adbexec:
	@echo '==== ADB RUN: [ $(PLATFORM) ] ===='
	@Cmd.exe /C adb.exe shell /data/local/tmp/$(LOCAL_MODULE) $(CBP2NDK_CMDLINE)

adbdebug:
	@echo '==== GDB Debug: [ $(PLATFORM) ] ===='
	@Cmd.exe /C adb.exe push $(PROJECT)libs\$(PLATFORM)\gdb.setup /data/local/tmp/gdb.setup
	@Cmd.exe /C adb.exe push $(PROJECT)libs\$(PLATFORM)\gdbserver /data/local/tmp/gdbserver
	@Cmd.exe /C adb.exe shell /system/bin/chmod 0777 /data/local/tmp/gdbserver

rundebug:
	@Cmd.exe /C DebugRemote.cmd

buildscript:
ifeq (,$(wildcard ./RunRemote.cmd))
	@echo "adb.exe shell /data/local/tmp/$(LOCAL_MODULE) $(CBP2NDK_CMDLINE)" >RunRemote.cmd
endif
ifeq (,$(wildcard ./DebugRemote.cmd))
	@echo "adb.exe forward tcp:59999 tcp:59999" >DebugRemote.cmd
	@echo "start \"$(PLATFORM) GDB server\" /MIN adb.exe shell /data/local/tmp/gdbserver :59999 /data/local/tmp/$(LOCAL_MODULE) $(CBP2NDK_CMDLINE)" >>DebugRemote.cmd
	@echo "exit" >>DebugRemote.cmd
endif

.PHONY: clean all

