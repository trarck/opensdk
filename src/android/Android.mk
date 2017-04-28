LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := opensdk_static

LOCAL_MODULE_FILENAME := libopensdk

LOCAL_SRC_FILES := \
    PluginFactory.cpp \
    PluginJniHelper.cpp \
    PluginUtils.cpp \
    PluginProtocol.cpp \
    AdsObject.cpp \
    AnalyticsObject.cpp \
    IAPObject.cpp \
    PushObject.cpp \
    ShareObject.cpp \
    SocialObject.cpp \
    UserObject.cpp \
    AgentManager.cpp \
    jni/Java_com_opensdk_framework_Ads.cpp \
    jni/Java_com_opensdk_framework_IAP.cpp \
    jni/Java_com_opensdk_framework_Push.cpp \
    jni/Java_com_opensdk_framework_Share.cpp \
    jni/Java_com_opensdk_framework_Social.cpp \
    jni/Java_com_opensdk_framework_User.cpp \
    InitManager.cpp \
    jni/Java_com_opensdk_init_InitManager.cpp \
    ../PluginManager.cpp \
    ../PluginParam.cpp

#LOCAL_CFLAGS := -std=c++11 -Wno-psabi
LOCAL_CFLAGS := -Wno-psabi
LOCAL_EXPORT_CFLAGS := -Wno-psabi

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_LDLIBS := -landroid
LOCAL_LDLIBS += -llog
#LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_STATIC_LIBRARY)

#$(call import-module,android/native_app_glue)
