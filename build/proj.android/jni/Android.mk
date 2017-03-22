LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := opensdk_android

LOCAL_MODULE_FILENAME := libopensdk

LOCAL_SRC_FILES := main.cpp 

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../../../yh

LOCAL_STATIC_LIBRARIES := opensdk_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)