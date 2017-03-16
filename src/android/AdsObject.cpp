#include "AdsObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace opensdk {

std::vector<AdsActionResult> AdsObject::_actionResultList;

AdsObject::AdsObject()
: _listener(NULL)
, _pluginName("")
{
}

AdsObject::~AdsObject()
{
	
}

void AdsObject::setPluginName(const char* name)
{
	_pluginName = name;
}

const char* AdsObject::getPluginName()
{
	return _pluginName.c_str();
}

int AdsObject::getPluginType()
{
    return kPluginAds;
}
    
void AdsObject::configDeveloperInfo(TAdsDeveloperInfo devInfo)
{
    if (devInfo.empty())
    {
        PluginUtils::outputLog("AdsObject", "The application info is empty!");
        return;
    }
    else
    {
        PluginUtils::callJavaFunctionWithName_map(this, "configDeveloperInfo", &devInfo);
    }
}

void AdsObject::showAds(TAdsInfo info, AdsPos pos)
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;

	PluginUtils::outputLog("AdsObject", "Class name : %s", pData->jclassName.c_str());
	if (PluginJniHelper::getMethodInfo(t
		, pData->jclassName.c_str()
		, "showAds"
		, "(Ljava/util/Hashtable;I)V"))
	{
	    jobject obj_Map = PluginUtils::createJavaMapObject(&info);
		t.env->CallVoidMethod(pData->jobj, t.methodID, obj_Map, pos);
		t.env->DeleteLocalRef(obj_Map);
		t.env->DeleteLocalRef(t.classID);
	}
}

void AdsObject::hideAds(TAdsInfo info)
{
    PluginUtils::callJavaFunctionWithName_map(this, "hideAds", &info);
}

void AdsObject::showAds(AdsType adsType, int idx)
{
	PluginParam paramAdsType(adsType);
	PluginParam paramIdx(idx);
	callFuncWithParam("showAds",&paramAdsType,&paramIdx,NULL);
}

void AdsObject::hideAds(AdsType adsType, int idx)
{
	PluginParam paramAdsType(adsType);
	PluginParam paramIdx(idx);
	callFuncWithParam("hideAds",&paramAdsType,&paramIdx,NULL);
}

void AdsObject::preloadAds(AdsType adsType, int idx)
{
	PluginParam paramAdsType(adsType);
	PluginParam paramIdx(idx);
	callFuncWithParam("preloadAds",&paramAdsType,&paramIdx,NULL);
}

float AdsObject::queryPoints()
{
    return PluginUtils::callJavaFloatFuncWithName(this, "queryPoints");
}

void AdsObject::spendPoints(int points)
{
	PluginUtils::callJavaFunctionWithName_oneParam(this, "spendPoints", "(I)V", points);
}

void AdsObject::setAdsListener(AdsListener* listener)
{
	_listener=listener;
    popActionResult();
}

AdsListener* AdsObject::getAdsListener()
{
	return _listener;
}

bool AdsObject::isAdTypeSupported(AdsType adType)
{
	PluginParam paramAdsType((int)adType);
	return callBoolFuncWithParam("isAdTypeSupported",&paramAdsType,NULL);
}
    
void AdsObject::popActionResult()
{
    for(std::vector<AdsActionResult>::iterator iter=_actionResultList.begin();iter!=_actionResultList.end();){
        
        AdsObject* pAds = dynamic_cast<AdsObject*>(PluginUtils::getPluginPtr(iter->className));
        if(pAds){
            AdsListener* listener = pAds->getAdsListener();
            if(listener){
                listener->onAdsResult(iter->resultCode, iter->msg.c_str());
                
                //remove from record
                iter=_actionResultList.erase(iter);
                
                continue;
            }
        }
        
        ++iter;
    }
}

void AdsObject::pushActionResult(AdsActionResult actionResult)
{
    _actionResultList.push_back(actionResult);
}

} // namespace opensdk {
