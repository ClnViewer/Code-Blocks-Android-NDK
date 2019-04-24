LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := hello_world
LOCAL_SRC_FILES := ./main.c
LOCAL_C_INCLUDES := ./
LOCAL_LDLIBS := -llog
include $(BUILD_EXECUTABLE)
