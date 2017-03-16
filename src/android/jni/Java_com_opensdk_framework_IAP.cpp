#include "IAPObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include <jni.h>

using namespace opensdk;

extern "C" {
    
    JNIEXPORT void JNICALL Java_com_opensdk_framework_IAPWrapper_nativeOnPayResult(JNIEnv*  env, jobject thiz, jstring className, jint ret, jstring msg)
    {
        std::string strMsg = PluginJniHelper::jstring2string(msg);
        std::string strClassName = PluginJniHelper::jstring2string(className);
        PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
        PluginUtils::outputLog("ProtocolIAP", "nativeOnPayResult(), Get plugin ptr : %p", pPlugin);
        IAPObject* iapObject = dynamic_cast<IAPObject*>(pPlugin);
        if (iapObject != NULL)
        {
            PluginUtils::outputLog("ProtocolIAP", "nativeOnPayResult(), Get plugin name : %s", pPlugin->getPluginName());
            
            if (iapObject->getResultListener()){
                
                iapObject->onPayResult((PayResultCode) ret, strMsg.c_str());
                
            }else{
                //save pay result
                IAPActionResult result={
                    (PayResultCode)ret,
                    strMsg,
                    strClassName
                };
                
                IAPObject::_actionResultList.push_back(result);

            	PluginUtils::outputLog("ProtocolIAP", "Can't find nativeOnPayResult listener of plugin %s", pPlugin->getPluginName());
            }
        }else{
            //save pay result
            IAPActionResult result={
                (PayResultCode)ret,
                strMsg,
                strClassName
            };
            
            IAPObject::_actionResultList.push_back(result);
            
            PluginUtils::outputLog("ProtocolIAP", "no plugin name : %s", pPlugin->getPluginName());
        }
    }
    
}
