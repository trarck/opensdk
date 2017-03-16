#include "PushObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include <jni.h>

using namespace opensdk;

extern "C" {
    
    JNIEXPORT void JNICALL Java_com_opensdk_framework_PushWrapper_nativeOnActionResult(JNIEnv*  env, jobject thiz, jstring className, jint ret, jstring msg)
    {
        std::string strMsg = PluginJniHelper::jstring2string(msg);
        std::string strClassName = PluginJniHelper::jstring2string(className);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
        PluginUtils::outputLog("ProtocolPush", "nativeOnActionResult(), Get plugin ptr : %p", pPlugin);
        PushObject* pushObject = dynamic_cast<PushObject*>(pPlugin);
        
        if (pushObject != NULL)
        {
            PluginUtils::outputLog("ProtocolPush", "nativeOnActionResult(), Get plugin name : %s", pPlugin->getPluginName());

            PushActionListener* listener=pushObject->getActionListener();
            
            if(listener!=NULL){
                listener->onActionResult(pushObject,(PushActionResultCode) ret, strMsg.c_str());
            }else{
                PushActionResult result={
                    (PushActionResultCode)ret,
                    strMsg,
                    strClassName
                };
                
                PushObject::_actionResultList.push_back(result);
                
                PluginUtils::outputLog("ProtocolPush", "Can't find nativeOnPayResult listener of plugin %s", pPlugin->getPluginName());
            }
        }else{
            
            PushActionResult result={
                (PushActionResultCode)ret,
                strMsg,
                strClassName
            };
            
            PushObject::_actionResultList.push_back(result);
            
            PluginUtils::outputLog("ProtocolPush", "plugin %s is null", pPlugin->getPluginName());
        }
    }
    
}
