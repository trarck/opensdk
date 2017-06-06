#ifndef OPENSDK_PLATFORM_STDC_IOS_H_
#define OPENSDK_PLATFORM_STDC_IOS_H_

#include "../PlatformConfig.h"
#if OPENSDK_TARGET_PLATFORM == OPENSDK_PLATFORM_IOS

#include "../PlatformMacros.h"
#include <float.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

#ifndef MIN
#define MIN(x,y) (((x) > (y)) ? (y) : (x))
#endif  // MIN

#ifndef MAX
#define MAX(x,y) (((x) < (y)) ? (y) : (x))
#endif  // MAX

#endif // OPENSDK_PLATFORM_IOS
#endif  // OPENSDK_PLATFORM_STDC_IOS_H_
