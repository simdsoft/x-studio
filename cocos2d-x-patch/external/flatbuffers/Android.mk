LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos_flatbuffers_static

LOCAL_MODULE_FILENAME := flatbuffers

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..\

LOCAL_CPPFLAGS += -fexceptions
                                 
include $(BUILD_STATIC_LIBRARY)
