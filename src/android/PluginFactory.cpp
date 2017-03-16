#include "PluginFactory.h"
#include "PluginUtils.h"
#include "PluginJniHelper.h"
#include "AdsObject.h"
#include "AnalyticsObject.h"
#include "IAPObject.h"
#include "PushObject.h"
#include "ShareObject.h"
#include "UserObject.h"
#include "SocialObject.h"

namespace opensdk {

#define ANDROID_PLUGIN_PACKAGE_PREFIX			"com/opensdk/framework/"

static PluginFactory* s_pFactory = NULL;

PluginFactory::PluginFactory()
{

}

PluginFactory::~PluginFactory()
{

}

PluginFactory* PluginFactory::getInstance()
{
	if (NULL == s_pFactory)
	{
		s_pFactory = new PluginFactory();
	}

	return s_pFactory;
}

void PluginFactory::purgeFactory()
{
	if (NULL != s_pFactory)
	{
		delete s_pFactory;
		s_pFactory = NULL;
	}
}

/** create the plugin by name */
PluginProtocol* PluginFactory::createPlugin(const char* name,int pluginType)
{
	PluginProtocol* pRet = NULL;
	do
	{
		if (name == NULL || strlen(name) == 0) break;

		std::string jClassName = ANDROID_PLUGIN_PACKAGE_PREFIX;
		jClassName.append(name);
		PluginUtils::outputLog("PluginFactory", "Java class name of plugin %s is : %s", name, jClassName.c_str());

		PluginJniMethodInfo t;
		if (! PluginJniHelper::getStaticMethodInfo(t
			, "com/opensdk/framework/PluginWrapper"
			, "initPlugin"
			, "(Ljava/lang/String;)Ljava/lang/Object;"))
		{
			PluginUtils::outputLog("PluginFactory", "Can't find method initPlugin in class org.cocos2dx.plugin.PluginWrapper");
			break;
		}

		jstring clsName = t.env->NewStringUTF(jClassName.c_str());
		jobject jObj = t.env->CallStaticObjectMethod(t.classID, t.methodID, clsName);
		t.env->DeleteLocalRef(clsName);
		t.env->DeleteLocalRef(t.classID);
		if (jObj == NULL)
		{
			PluginUtils::outputLog("PluginFactory", "Can't find java class %s", jClassName.c_str());
			break;
		}

        if(pluginType==0){
            //get from java class
            if (! PluginJniHelper::getStaticMethodInfo(t
                , "com/opensdk/framework/PluginWrapper"
                , "getPluginType"
                , "(Ljava/lang/Object;)I"))
            {
                PluginUtils::outputLog("PluginFactory", "Can't find method getPluginType in class org.cocos2dx.plugin.PluginWrapper");
                break;
            }
            pluginType = t.env->CallStaticIntMethod(t.classID, t.methodID, jObj);
            t.env->DeleteLocalRef(t.classID);
            PluginUtils::outputLog("PluginFactory", "The type of plugin %s is : %d", name, pluginType);
        }


		switch (pluginType)
		{
            case kPluginAds:
                pRet = new AdsObject();
                break;
            case kPluginAnalytics:
                pRet = new AnalyticsObject();
                break;
            case kPluginIAP:
                pRet = new IAPObject();
                break;
            case kPluginShare:
                pRet = new ShareObject();
                break;
            case kPluginUser:
                pRet = new UserObject();
                break;
            case kPluginSocial:
                pRet = new SocialObject();
                break;
            case kPluginPush:
                pRet = new PushObject();
                break;
            default:
                break;
		}

		if (pRet != NULL)
		{
			pRet->setPluginName(name);
			PluginUtils::initJavaPlugin(pRet, jObj, jClassName.c_str());
		}
	} while(0);

	return pRet;
}

} //namespace opensdk {
