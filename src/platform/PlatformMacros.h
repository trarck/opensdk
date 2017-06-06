#ifndef OPENSDK_PLATFORM_PLATFORM_MACROS_H_
#define OPENSDK_PLATFORM_PLATFORM_MACROS_H_

#include "./PlatformConfig.h"
#include "./PlatformDefine.h"

/** @def OPENSDK_DISALLOW_COPY_AND_ASSIGN(TypeName)
 * A macro to disallow the copy constructor and operator= functions.
 * This should be used in the private: declarations for a class
 */
#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
    || (defined(__clang__) && (__clang_major__ >= 3)) || (_MSC_VER >= 1800)
#define OPENSDK_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &) = delete; \
    TypeName &operator =(const TypeName &) = delete;
#else
#define OPENSDK_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &); \
    TypeName &operator =(const TypeName &);
#endif

/** @def OPENSDK_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName)
 * A macro to disallow all the implicit constructors, namely the
 * default constructor, copy constructor and operator= functions.
 *
 * This should be used in the private: declarations for a class
 * that wants to prevent anyone from instantiating it. This is
 * especially useful for classes containing only static methods. 
 */
#define OPENSDK_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName)    \
    TypeName();                                        \
    OPENSDK_DISALLOW_COPY_AND_ASSIGN(TypeName)

/** @def OPENSDK_DEPRECATED_ATTRIBUTE
 * Only certain compilers support __attribute__((deprecated)).
 */
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define OPENSDK_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
    #define OPENSDK_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
    #define OPENSDK_DEPRECATED_ATTRIBUTE
#endif 

/** @def OPENSDK_DEPRECATED(...)
 * Macro to mark things deprecated as of a particular version
 * can be used with arbitrary parameters which are thrown away.
 * e.g. OPENSDK_DEPRECATED(4.0) or OPENSDK_DEPRECATED(4.0, "not going to need this anymore") etc.
 */
#define OPENSDK_DEPRECATED(...) OPENSDK_DEPRECATED_ATTRIBUTE

/** @def OPENSDK_FORMAT_PRINTF(formatPos, argPos)
 * Only certain compiler support __attribute__((format))
 *
 * @param formatPos 1-based position of format string argument.
 * @param argPos    1-based position of first format-dependent argument.
 */
#if defined(__GNUC__) && (__GNUC__ >= 4)
#define OPENSDK_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
#elif defined(__has_attribute)
  #if __has_attribute(format)
  #define OPENSDK_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
  #endif // __has_attribute(format)
#else
#define OPENSDK_FORMAT_PRINTF(formatPos, argPos)
#endif

#if defined(_MSC_VER)
#define OPENSDK_FORMAT_PRINTF_SIZE_T "%08lX"
#else
#define OPENSDK_FORMAT_PRINTF_SIZE_T "%08zX"
#endif

#ifdef __GNUC__
#define OPENSDK_UNUSED __attribute__ ((unused))
#else
#define OPENSDK_UNUSED
#endif

/** @def OPENSDK_REQUIRES_NULL_TERMINATION
 * 
 */
#if !defined(OPENSDK_REQUIRES_NULL_TERMINATION)
    #if defined(__APPLE_CC__) && (__APPLE_CC__ >= 5549)
        #define OPENSDK_REQUIRES_NULL_TERMINATION __attribute__((sentinel(0,1)))
    #elif defined(__GNUC__)
        #define OPENSDK_REQUIRES_NULL_TERMINATION __attribute__((sentinel))
    #else
        #define OPENSDK_REQUIRES_NULL_TERMINATION
    #endif
#endif

#endif  // OPENSDK_PLATFORM_PLATFORM_MACROS_H_

