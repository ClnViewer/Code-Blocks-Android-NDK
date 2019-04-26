
PLATFORM := armeabi-v7a
NDKROOT  := C:\__BuildSource\__LIB__\android-ndk-r20-beta2
PROJECT  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CONFCBP  := $(shell find $(SOURCEDIR) -name '*.cbp')
BUILDTAG := $(filter-out $@,$(MAKECMDGOALS))
BUILDOPT := 

include Application.mk
include $(APP_BUILD_SCRIPT)

ifneq ($(APP_ABI),all)
	PLATFORM = $(APP_ABI)
endif

ifeq ($(BUILDTAG),Debug)
	BUILDOPT = V=1 NDK_DEBUG=1
else
	BUILDOPT = -j 4
endif

all: allndk
Debug: allndk adbsetup adbdebug buildscript rundebug
Release: allndk adbsetup adbexec buildscript
cleanDebug: clean
cleanRelease: clean
cleanall: clean

allndk:
	@echo '==== Build $(BUILDTAG) -> $(APP_ABI) platform -> active device: [ $(PLATFORM) ] ===='
	@cbp2ndk $(BUILDTAG) $(CONFCBP)
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

rundebug:
	@Cmd.exe /C DebugRemote.cmd

buildscript:
ifeq (,$(wildcard ./RunRemote.cmd))
	@echo "adb.exe shell /data/local/tmp/$(LOCAL_MODULE)" >RunRemote.cmd
endif
ifeq (,$(wildcard ./DebugRemote.cmd))
	@echo "adb.exe forward tcp:59999 tcp:59999" >DebugRemote.cmd
	@echo "start \"$(PLATFORM) GDB server\" /MIN adb.exe shell /data/local/tmp/gdbserver :59999 /data/local/tmp/$(LOCAL_MODULE)" >>DebugRemote.cmd
	@echo "exit" >>DebugRemote.cmd
endif

.PHONY: clean all
