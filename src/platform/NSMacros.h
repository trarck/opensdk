#ifndef OPENSDK_NSMACROS_H_
#define OPENSDK_NSMACROS_H_

#ifdef __cplusplus

#define NS_OPENSDK_BEGIN                     namespace yh {
#define NS_OPENSDK_END                       }
#define USING_NS_YH                     using namespace yh

#else

#define NS_OPENSDK_BEGIN                     
#define NS_OPENSDK_END                       
#define USING_NS_YH

#endif
	
#endif //OPENSDK_NSMACROS_H_