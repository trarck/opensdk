#include "UserObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include <jni.h>

using namespace opensdk;

extern "C" {
    
    JNIEXPORT void JNICALL Java_com_opensdk_framework_UserWrapper_nativeOnActionResult(JNIEnv*  env, jobject thiz, jstring className, jint ret, jstring msg)
    {
        std::string strMsg = PluginJniHelper::jstring2string(msg);
        std::string strClassName = PluginJniHelper::jstring2string(className);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
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
                    strClassName
                };
                
                UserObject::_actionResultList.push_back(result);
                
                PluginUtils::outputLog("ProtocolUser","Listener of plugin %s not set correctly", pPlugin->getPluginName());
            }

        }else{
            
            UserActionResult result={
                (UserActionResultCode)ret,
                strMsg,
                strClassName
            };
            
            UserObject::_actionResultList.push_back(result);
            
            PluginUtils::outputLog("ProtocolUser", "plugin %s is null", pPlugin->getPluginName());
        }
    }
    
    JNIEXPORT jstring JNICALL Java_com_opensdk_framework_UserWrapper_getLoginServerID(JNIEnv*  env, jobject thiz)
    {
        
        return env->NewStringUTF(UserObject::_serverID.c_str());
    }
    
    JNIEXPORT jstring JNICALL Java_com_opensdk_framework_UserWrapper_getLoginServerIP(JNIEnv*  env, jobject thiz)
    {
        
        return env->NewStringUTF(UserObject::_serverIP.c_str());
    }
    
}
