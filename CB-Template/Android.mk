LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := hello_world
LOCAL_CPP_EXTENSION := .cpp
LOCAL_SRC_FILES := ndk-main.c ndk-test.cpp
LOCAL_CFLAGS := -Wall
LOCAL_LDLIBS := -llog
LOCAL_C_INCLUDES := ./
include $(BUILD_EXECUTABLE)
