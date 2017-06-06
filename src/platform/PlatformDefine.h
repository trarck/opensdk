#ifndef OPENSDK_PLATFORM_PLATFORM_DEFINE_H_
#define OPENSDK_PLATFORM_PLATFORM_DEFINE_H_

#include "./PlatformConfig.h"

#if OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_MAC
	#include "./mac/PlatformDefine-mac.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_IOS
	#include "./ios/PlatformDefine-ios.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_ANDROID
	#include "./android/PlatformDefine-android.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_WIN32
	#include "./win32/PlatformDefine-win32.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_WINRT
	#include "./winrt/PlatformDefine-winrt.h"
#elif OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_LINUX
	#include "./linux/PlatformDefine-linux.h"
#endif

#endif  // OPENSDK_PLATFORM_PLATFORM_DEFINE_H_

