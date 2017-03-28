#include <jni.h>
#include <android/log.h>
#include "PluginManager.h" 
#define  LOG_TAG    "main"

using namespace opensdk;

void yh_android_app_init (JNIEnv* env) {
   PluginManager* pm=PluginManager::getInstance();
}
