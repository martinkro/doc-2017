LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:=test_strlen
LOCAL_SRC_FILES:=test_strlen.cpp
include $(BUILD_SHARED_LIBRARY)