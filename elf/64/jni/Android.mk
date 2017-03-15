LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:=test_64
LOCAL_SRC_FILES:=test_strlen.c
include $(BUILD_SHARED_LIBRARY)