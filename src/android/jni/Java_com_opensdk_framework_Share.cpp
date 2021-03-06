#include "ShareObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include <jni.h>

using namespace opensdk;

extern "C" {
    JNIEXPORT void JNICALL Java_com_opensdk_framework_NativeInvoker_nativeOnShareResult(JNIEnv*  env, jobject thiz, jstring pluginKey, jint ret, jstring msg)
    {
        std::string strMsg = PluginJniHelper::jstring2string(msg);
        std::string strPluginKey = PluginJniHelper::jstring2string(pluginKey);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strPluginKey);
        PluginUtils::outputLog("ProtocolShare", "nativeOnShareResult(), Get plugin ptr : %p", pPlugin);
        ShareObject* shareObject = dynamic_cast<ShareObject*>(pPlugin);
        if (shareObject != NULL)
        {
            PluginUtils::outputLog("ProtocolShare", "nativeOnShareResult(), Get plugin name : %s", pPlugin->getPluginName());
            

            ShareResultListener* listener = shareObject->getResultListener();
            if (NULL != listener)
            {
                ShareResultCode cRet = (ShareResultCode) ret;
                listener->onShareResult(cRet, strMsg.c_str());
            }else
            {
                //save sahre result
                ShareActionResult result={
                    (ShareResultCode)ret,
                    strMsg,
                    strPluginKey
                };
                ShareObject::pushActionResult(result);
                
                PluginUtils::outputLog("ProtocolShare", "Can't find the listener of plugin %s", pPlugin->getPluginName());
            }
        }else{
            //save sahre result
            ShareActionResult result={
                (ShareResultCode)ret,
                strMsg,
                strPluginKey
            };
            
            ShareObject::pushActionResult(result);
            
            PluginUtils::outputLog("ProtocolShare", "no plugin name : %s", pPlugin->getPluginName());
        }
    }
}
