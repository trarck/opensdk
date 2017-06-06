#ifndef OPENSDK_BASE_LOG_H_
#define OPENSDK_BASE_LOG_H_

#include "NSMacros.h"

NS_OPENSDK_BEGIN

static const int kMaxLogLen = 16*1024;

void log(const char * pszFormat, ...);

#define __YHLOGWITHFUNCTION(s, ...) \
    opensdk::log("%s : " s ,__FUNCTION__,  ##__VA_ARGS__)

#define OPENSDK_LOG_LEVEL_DEBUG 4
#define OPENSDK_LOG_LEVEL_INFO 3
#define OPENSDK_LOG_LEVEL_WARN 2
#define OPENSDK_LOG_LEVEL_ERROR 1
#define OPENSDK_LOG_LEVEL_NONE 0

#if !defined(OPENSDK_LOG_LEVEL) || OPENSDK_LOG_LEVEL == OPENSDK_LOG_LEVEL_NONE
#define OPENSDK_DEBUG(...)       do {} while (0)
#define OPENSDK_INFO(...)   do {} while (0)
#define OPENSDK_WARN(...)   do {} while (0)
#define OPENSDK_ERROR(...)  do {} while (0)

#elif OPENSDK_LOG_LEVEL == OPENSDK_LOG_LEVEL_ERROR
#define OPENSDK_DEBUG(...)   do {} while (0)
#define OPENSDK_INFO(...) do {} while (0)
#define OPENSDK_WARN(...)  do {} while (0)
#define OPENSDK_ERROR(...) __YHLOGWITHFUNCTION(__VA_ARGS__)

#elif OPENSDK_LOG_LEVEL == OPENSDK_LOG_LEVEL_WARN
#define OPENSDK_DEBUG(...)   do {} while (0)
#define OPENSDK_INFO(...)  do {} while (0)
#define OPENSDK_WARN(format,...) opensdk::log(format, ##__VA_ARGS__)
#define OPENSDK_ERROR(format,...)  __YHLOGWITHFUNCTION(format,##__VA_ARGS__)

#elif OPENSDK_LOG_LEVEL == OPENSDK_LOG_LEVEL_INFO
#define OPENSDK_DEBUG(...)   do {} while (0)
#define OPENSDK_INFO(format, ...)  opensdk::log(format, ##__VA_ARGS__)
#define OPENSDK_WARN(format,...) opensdk::log(format, ##__VA_ARGS__)
#define OPENSDK_ERROR(format,...)  __YHLOGWITHFUNCTION(format,##__VA_ARGS__)

#elif OPENSDK_LOG_LEVEL > OPENSDK_LOG_LEVEL_INFO
#define OPENSDK_DEBUG(format, ...)      opensdk::log(format, ##__VA_ARGS__)
#define OPENSDK_INFO(format,...)   opensdk::log(format, ##__VA_ARGS__)
#define OPENSDK_WARN(format,...) opensdk::log(format, ##__VA_ARGS__)
#define OPENSDK_ERROR(format,...)  __YHLOGWITHFUNCTION(format,##__VA_ARGS__)
#endif // opensdk log

//define func ptf
typedef void(*ScriptDeubgFuncPtr)(const char *);

void SetScriptDeubgFuncPtr(ScriptDeubgFuncPtr fp);

NS_OPENSDK_END

#endif // OPENSDK_BASE_LOG_H_
