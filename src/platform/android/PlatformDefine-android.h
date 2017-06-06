#ifndef OPENSDK_PLATFORM_PLATFORM_DEFINE_ANDROID_H_
#define OPENSDK_PLATFORM_PLATFORM_DEFINE_ANDROID_H_

#include "../PlatformConfig.h"
#if OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_ANDROID

#include "android/log.h"

#ifndef OPENSDK_DLL
#define OPENSDK_DLL
#endif

#define OPENSDK_NO_MESSAGE_PSEUDOASSERT(cond)                        \
    if (!(cond)) {                                              \
        __android_log_print(ANDROID_LOG_ERROR,                  \
                            "cocos2d-x assert",                 \
                            "%s function:%s line:%d",           \
                            __FILE__, __FUNCTION__, __LINE__);  \
    }

#define OPENSDK_MESSAGE_PSEUDOASSERT(cond, msg)                          \
    if (!(cond)) {                                                  \
        __android_log_print(ANDROID_LOG_ERROR,                      \
                            "cocos2d-x assert",                     \
                            "file:%s function:%s line:%d, %s",      \
                            __FILE__, __FUNCTION__, __LINE__, msg); \
    }

#define OPENSDK_ASSERT(cond) OPENSDK_NO_MESSAGE_PSEUDOASSERT(cond)

#define OPENSDK_UNUSED_PARAM(unusedparam) (void)unusedparam

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#endif // OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_ANDROID

#endif /* OPENSDK_PLATFORM_PLATFORM_DEFINE_ANDROID_H_*/
