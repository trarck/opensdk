#include "SocialObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include <jni.h>

using namespace opensdk;

extern "C" {
    JNIEXPORT void JNICALL Java_com_opensdk_framework_SocialWrapper_nativeOnSocialResult(JNIEnv*  env, jobject thiz, jstring className, jint ret, jstring msg)
    {
        std::string strMsg = PluginJniHelper::jstring2string(msg);
        std::string strClassName = PluginJniHelper::jstring2string(className);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
        PluginUtils::outputLog("ProtocolSocial", "nativeOnSocialResult(), Get plugin ptr : %p", pPlugin);
        SocialObject* socialObject = dynamic_cast<SocialObject*>(pPlugin);
        if (socialObject != NULL)
        {
            PluginUtils::outputLog("ProtocolSocial", "nativeOnSocialResult(), Get plugin name : %s", pPlugin->getPluginName());
            
    
            SocialListener* pListener = socialObject->getListener();
            if (NULL != pListener)
            {
                pListener->onSocialResult((SocialRetCode) ret, strMsg.c_str());
            }
            else
            {
                SocialActionResult result={
                    (SocialRetCode)ret,
                    strMsg,
                    strClassName
                };
                
                SocialObject::_actionResultList.push_back(result);
                PluginUtils::outputLog("ProtocolSocial", "Can't find the nativeOnSocialResult listener of plugin %s", pPlugin->getPluginName());
            }

        }else{
            
            SocialActionResult result={
                (SocialRetCode)ret,
                strMsg,
                strClassName
            };
            
            SocialObject::_actionResultList.push_back(result);
            
            PluginUtils::outputLog("ProtocolSocial", "plugin %s is null", pPlugin->getPluginName());
        }
    }
    
    
}
