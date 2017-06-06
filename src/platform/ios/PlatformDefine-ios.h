#ifndef OPENSDK_PLATFORM_PLATFORM_DEFINE_IOS_H_
#define OPENSDK_PLATFORM_PLATFORM_DEFINE_IOS_H_

#include "../PlatformConfig.h"
#if OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_IOS

#include <assert.h>

#ifndef OPENSDK_DLL
#define OPENSDK_DLL 
#endif

#define OPENSDK_ASSERT(cond) assert(cond)


#define OPENSDK_UNUSED_PARAM(unusedparam) (void)unusedparam

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


#endif // OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_IOS

#endif /* OPENSDK_PLATFORM_PLATFORM_DEFINE_IOS_H_*/
