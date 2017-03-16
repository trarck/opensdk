#include "IAPObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace opensdk {

std::vector<IAPActionResult> IAPObject::_actionResultList;

bool ProtocolIAP::_paying = false;

IAPObject::IAPObject()
: _listener(NULL)
, _pluginName("")
{
}

IAPObject::~IAPObject()
{
}

void IAPObject::setPluginName(const char* name)
{
	_pluginName = name;
}

const char* IAPObject::getPluginName()
{
	return _pluginName.c_str();
}

int IAPObject::getPluginType()
{
    return kPluginIAP;
}

void IAPObject::configDeveloperInfo(TIAPDeveloperInfo devInfo)
{
    if (devInfo.empty())
    {
        PluginUtils::outputLog("IAPObject", "The developer info is empty!");
        return;
    }
    else
    {
        PluginUtils::callJavaFunctionWithName_map(this, "configDeveloperInfo", &devInfo);
    }
}

void IAPObject::payForProduct(TProductInfo info)
{
    if (_paying)
    {
        PluginUtils::outputLog("IAPObject", "Now is paying");
        return;
    }

    if (info.empty())
    {
        if (NULL != _listener)
        {
            onPayResult(kPayFail, "Product info error");
        }else{
            //save pay result for listener
            PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
            if(pData){
                IAPActionResult result={
                    kPayFail,
                    "Product info error",
                    pData->jclassName
                };
                
                _actionResultList.push_back(result);
            }
        }
        PluginUtils::outputLog("IAPObject", "The product info is empty!");
        return;
    }
    else
    {
        _paying = true;
        _curInfo = info;
        
        PluginUtils::callJavaFunctionWithName_map(this, "payForProduct", &info);
    }
}

std::string IAPObject::getOrderId()
{
	return PluginUtils::callJavaStringFuncWithName(this,"getOrderId");
}

void IAPObject::setResultListener(PayResultListener* pListener)
{
	_listener = pListener;
    popActionResult();
}

PayResultListener* IAPObject::getResultListener()
{
	return _listener;
}

std::string IAPObject::getPluginId()
{
	return PluginUtils::callJavaStringFuncWithName(this,"getPluginId");
}

void IAPObject::onPayResult(PayResultCode ret, const char* msg)
{
    
    if (_listener)
    {
        _paying = false;
    	_listener->onPayResult(ret, msg, _curInfo);
        _curInfo.clear();
    }
    else
    {
        PluginUtils::outputLog("IAPObject", "Result listener is null!");
    }
    
    PluginUtils::outputLog("IAPObject", "Pay result is : %d(%s)", (int) ret, msg);
}

void IAPObject::popActionResult()
{
    for(std::vector<IAPActionResult>::iterator iter=_actionResultList.begin();iter!=_actionResultList.end();){
        
        IAPObject* iapObject = dynamic_cast<IAPObject*>(PluginUtils::getPluginPtr(iter->className));
        if(iapObject){
            PayResultListener* listener = iapObject->getResultListener();
            if(listener){
                listener->onPayResult(iter->resultCode, iter->msg.c_str(), iapObject->_curInfo);
                
                //remove from record
                iter=_actionResultList.erase(iter);
                continue;
            }
        }
        
        ++iter;
    }
}

void IAPObject::pushActionResult(const IAPActionResult& actionResult)
{
    _actionResultList.push_back(actionResult);
}


} // namespace opensdk {
