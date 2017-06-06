#ifndef OPENSDK_PLATFORM_PLATFORM_CONFIG_H_
#define OPENSDK_PLATFORM_PLATFORM_CONFIG_H_

//////////////////////////////////////////////////////////////////////////
// pre configure
//////////////////////////////////////////////////////////////////////////

// define supported target platform macro which YH uses.
#define OPENSDK_PLATFORM_UNKNOWN            0
#define OPENSDK_PLATFORM_IOS                1
#define OPENSDK_PLATFORM_ANDROID            2
#define OPENSDK_PLATFORM_WIN32              3
#define OPENSDK_PLATFORM_MARMALADE          4
#define OPENSDK_PLATFORM_LINUX              5
#define OPENSDK_PLATFORM_BADA               6
#define OPENSDK_PLATFORM_BLACKBERRY         7
#define OPENSDK_PLATFORM_MAC                8
#define OPENSDK_PLATFORM_NACL               9
#define OPENSDK_PLATFORM_EMSCRIPTEN        10
#define OPENSDK_PLATFORM_TIZEN             11
#define OPENSDK_PLATFORM_QT5               12
#define OPENSDK_PLATFORM_WINRT             13

// Determine target platform by compile environment macro.
#define OPENSDK_TARGET_PLATFORM             OPENSDK_PLATFORM_UNKNOWN

//check mac os
#if defined(__APPLE__) && defined(__MACH__)
	/* Apple OSX and iOS (Darwin). ------------------------------ */
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
	/* iOS in Xcode simulator */
	#undef  OPENSDK_TARGET_PLATFORM
	#define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_IOS
#elif TARGET_OS_IPHONE == 1
	/* iOS on iPhone, iPad, etc. */
	#undef  OPENSDK_TARGET_PLATFORM
	#define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_IOS
#elif TARGET_OS_MAC == 1
	/* OSX */
	#undef  OPENSDK_TARGET_PLATFORM
	#define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_MAC
#endif
#endif

// // mac
// #if defined(OPENSDK_TARGET_OS_MAC)
// 	#undef  OPENSDK_TARGET_PLATFORM
// 	#define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_MAC
// #endif
// 
// // iphone
// #if defined(OPENSDK_TARGET_OS_IPHONE)
//     #undef  OPENSDK_TARGET_PLATFORM
//     #define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_IOS
// #endif

// android
#if defined(ANDROID)
    #undef  OPENSDK_TARGET_PLATFORM
    #define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_ANDROID
#endif

// win32
#if defined(WIN32) && defined(_WINDOWS)
    #undef  OPENSDK_TARGET_PLATFORM
    #define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX)
    #undef  OPENSDK_TARGET_PLATFORM
    #define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  OPENSDK_TARGET_PLATFORM
#define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  OPENSDK_TARGET_PLATFORM
#define OPENSDK_TARGET_PLATFORM         OPENSDK_PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
    #undef  OPENSDK_TARGET_PLATFORM
    #define OPENSDK_TARGET_PLATFORM     OPENSDK_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
    #undef  OPENSDK_TARGET_PLATFORM
    #define OPENSDK_TARGET_PLATFORM     OPENSDK_PLATFORM_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
    #undef  OPENSDK_TARGET_PLATFORM
    #define OPENSDK_TARGET_PLATFORM     OPENSDK_PLATFORM_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
    #undef  OPENSDK_TARGET_PLATFORM
    #define OPENSDK_TARGET_PLATFORM     OPENSDK_PLATFORM_TIZEN
#endif

// qt5
#if defined(OPENSDK_TARGET_QT5)
    #undef  OPENSDK_TARGET_PLATFORM
    #define OPENSDK_TARGET_PLATFORM     OPENSDK_PLATFORM_QT5
#endif

// WinRT (Windows 8.1 Store/Phone App)
#if defined(WINRT)
#undef  OPENSDK_TARGET_PLATFORM
#define OPENSDK_TARGET_PLATFORM          OPENSDK_PLATFORM_WINRT
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! OPENSDK_TARGET_PLATFORM
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif 

#if (OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_WIN32)
#ifndef __MINGW32__
#pragma warning (disable:4127) 
#endif 
#endif  // OPENSDK_PLATFORM_WIN32

#endif  // OPENSDK_PLATFORM_PLATFORM_CONFIG_H_

