#include "UserObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include <jni.h>

using namespace opensdk;

extern "C" {
    
    JNIEXPORT void JNICALL Java_com_opensdk_framework_NativeInvoker_nativeOnActionResult(JNIEnv*  env, jobject thiz, jstring pluginKey, jint ret, jstring msg)
    {
        std::string strMsg = PluginJniHelper::jstring2string(msg);
        std::string strPluginKey = PluginJniHelper::jstring2string(pluginKey);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strPluginKey);
        PluginUtils::outputLog("ProtocolUser", "nativeOnActionResult(), Get plugin ptr : %p", pPlugin);
        UserObject* userObject = dynamic_cast<UserObject*>(pPlugin);
        if (userObject != NULL)
        {
            PluginUtils::outputLog("ProtocolUser", "nativeOnActionResult(), Get plugin name : %s", pPlugin->getPluginName());

            UserActionListener* listener = userObject->getActionListener();
            if (NULL != listener)
            {
                listener->onActionResult(userObject, (UserActionResultCode) ret, strMsg.c_str());
            }
            else
            {
                UserActionResult result={
                    (UserActionResultCode)ret,
                    strMsg,
                    strPluginKey
                };
                
                UserObject::pushActionResult(result);
                
                PluginUtils::outputLog("ProtocolUser","Listener of plugin %s not set correctly", pPlugin->getPluginName());
            }

        }else{
            
            UserActionResult result={
                (UserActionResultCode)ret,
                strMsg,
                strPluginKey
            };
            
            UserObject::pushActionResult(result);
            
            PluginUtils::outputLog("ProtocolUser", "plugin %s is null", pPlugin->getPluginName());
        }
    }
    
    JNIEXPORT jobject JNICALL Java_com_opensdk_framework_NativeInvoker_nativeGetLoginInfo(JNIEnv*  env, jobject thiz, jstring pluginKey)
    {
        std::string strPluginKey = PluginJniHelper::jstring2string(pluginKey);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strPluginKey);
        PluginUtils::outputLog("ProtocolUser", "nativeOnActionResult(), Get plugin ptr : %p", pPlugin);
        UserObject* userObject = dynamic_cast<UserObject*>(pPlugin);
        jobject loginInfo;
        if (userObject != NULL)
        {
            loginInfo=PluginUtils::createJavaMapObject(userObject->getLoginInfo());
        }else{           
            PluginUtils::outputLog("ProtocolUser", "plugin %s is null", pPlugin->getPluginName());
            loginInfo=PluginUtils::createJavaMapObject(NULL);
        }
        
        return loginInfo;
    }
}
