#include "ShareObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace opensdk {

std::vector<ShareActionResult> ShareObject::_actionResultList;

ShareObject::ShareObject()
: _listener(NULL)
, _pluginName("")
{
}

ShareObject::~ShareObject()
{
}
    
void ShareObject::setPluginName(const char* name)
{
    _pluginName = name;
}

const char* ShareObject::getPluginName()
{
    return _pluginName.c_str();
}

int ShareObject::getPluginType()
{
    return kPluginShare;
}
    
void ShareObject::configDeveloperInfo(TShareDeveloperInfo devInfo)
{
    if (devInfo.empty())
    {
        PluginUtils::outputLog("ShareObject", "The developer info is empty!");
        return;
    }
    else
    {
        PluginUtils::callJavaFunctionWithName_map(this, "configDeveloperInfo", &devInfo);
    }
}

void ShareObject::share(TShareInfo info)
{
    if (info.empty())
    {
        if (NULL != _listener)
        {
            onShareResult(kShareFail, "Share info error");
        }else{
            //save share result for listener
            PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
            if(pData){
                ShareActionResult result={
                    kShareFail,
                    "Share info error",
                    pData->jclassName
                };
                
                _actionResultList.push_back(result);
            }

        }
        
        PluginUtils::outputLog("ShareObject", "The Share info is empty!");
        return;
    }
    else
    {
        PluginUtils::callJavaFunctionWithName_map(this, "share", &info);
    }
}


void ShareObject::setResultListener(ShareResultListener* pListener)
{
	_listener = pListener;
    popActionResult();
}

ShareResultListener* ShareObject::getResultListener()
{
    return _listener;
}

void ShareObject::onShareResult(ShareResultCode ret, const char* msg)
{
    if (_listener)
    {
    	_listener->onShareResult(ret, msg);
    }
    else
    {
        PluginUtils::outputLog("ShareObject", "Result listener is null!");
    }
    PluginUtils::outputLog("ShareObject", "Share result is : %d(%s)", (int) ret, msg);
}
    
void ShareObject::popActionResult()
{
    for(std::vector<ShareActionResult>::iterator iter=_actionResultList.begin();iter!=_actionResultList.end();){
        
        ShareObject* shareObject = dynamic_cast<ShareObject*>(PluginUtils::getPluginPtr(iter->className));
        if(shareObject){
            ShareResultListener* listener = shareObject->getResultListener();
            if(listener){
                listener->onShareResult(iter->resultCode, iter->msg.c_str());
                
                //remove from record
                iter=_actionResultList.erase(iter);
                continue;
            }
        }
        
        ++iter;
    }
}

void ShareObject::pushActionResult(const ShareActionResult& actionResult)
{
    _actionResultList.push_back(actionResult);
}

} // namespace opensdk {
