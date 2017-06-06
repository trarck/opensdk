#ifndef OPENSDK_PLATFORM_YHSTDC_H_
#define OPENSDK_PLATFORM_YHSTDC_H_

#include "PlatformConfig.h"

#include "nullptr.h"
#if OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_MAC
#include "./mac/StdC-mac.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_IOS
#include "./ios/StdC-ios.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_ANDROID
#include "./android/StdC-android.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_WIN32
#include "./win32/StdC-win32.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_WINRT
#include "./winrt/StdC-winrt.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_LINUX
#include "./linux/StdC-linux.h"
#endif

#endif  // OPENSDK_PLATFORM_YHSTDC_H_

