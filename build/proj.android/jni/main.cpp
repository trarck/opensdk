#include <jni.h>
#include <android/log.h>
#include "yh.h"

#define  LOG_TAG    "main"

void yh_android_app_init (JNIEnv* env) {
   yh::log("Hello world"); 
}
