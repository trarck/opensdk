#include <vector>

#include "AgentManager.h"
#include "PluginManager.h"
#include "PluginFactory.h"
#include "PluginJniHelper.h"
#include "PluginUtils.h"
#include <android/log.h>

namespace opensdk {

AgentManagerAndroid::AgentManagerAndroid()
{

}

AgentManagerAndroid::~AgentManagerAndroid()
{
	
}

AgentManager* AgentManager::getInstance()
{
	if(NULL == s_agentManagerInstance)
	{
		s_agentManagerInstance = new (std::nothrow) AgentManagerAndroid();
	}
	return s_agentManagerInstance;
}

void AgentManagerAndroid::init()
{
	
}

void AgentManagerAndroid::init(const std::string& appKey,const std::string& appSecret,const std::string& privateKey,const std::string& oauthLoginServer)
{
    PluginJniMethodInfo t;
    if (! PluginJniHelper::getStaticMethodInfo(t
        , "com/opensdk/framework/Wrapper"
        , "setAppParam"
        , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
		jstring paramAppKey = t.env->NewStringUTF(appKey.c_str());
		jstring paramAppSecret = t.env->NewStringUTF(appSecret.c_str());
		jstring paramPrivateKey = t.env->NewStringUTF(privateKey.c_str());
		jstring paramOauthLoginServer = t.env->NewStringUTF(oauthLoginServer.c_str());
		
	    t.env->CallStaticVoidMethod(t.classID, t.methodID,paramAppKey,paramAppSecret,paramPrivateKey,paramOauthLoginServer);
		t.env->DeleteLocalRef(paramAppKey);
		t.env->DeleteLocalRef(paramAppSecret);
		t.env->DeleteLocalRef(paramPrivateKey);
		t.env->DeleteLocalRef(paramOauthLoginServer);
    }

    t.env->DeleteLocalRef(t.classID);
}

std::string AgentManagerAndroid::getChannelId()
{
	std::string ret = "";
	
    PluginJniMethodInfo t;
    if (! PluginJniHelper::getStaticMethodInfo(t
        , "com/opensdk/framework/Wrapper"
        , "getChannelId"
        , "()Ljava/lang/String;"))
    {
	    jstring strRet = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
	    ret = PluginJniHelper::jstring2string(strRet);
		t.env->DeleteLocalRef(strRet);
    }

    t.env->DeleteLocalRef(t.classID);
	
	return ret;
}

std::string AgentManagerAndroid::getCustomParam()
{
	std::string ret = "";
	
    PluginJniMethodInfo t;
    if (! PluginJniHelper::getStaticMethodInfo(t
        , "com/opensdk/framework/Wrapper"
        , "getCustomParam"
        , "()Ljava/lang/String;"))
    {
	    jstring strRet = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
	    ret = PluginJniHelper::jstring2string(strRet);
		t.env->DeleteLocalRef(strRet);
    }

    t.env->DeleteLocalRef(t.classID);
	
	return ret;
}

std::map<std::string, std::string> AgentManagerAndroid::getPluginConfigure()
{
	std::map<std::string, std::string> configure;

	PluginJniMethodInfo t;

	if(PluginJniHelper::getStaticMethodInfo(t, "com/opensdk/framework/Wrapper", "getPluginConfigure", "()Ljava/util/Hashtable;"))
	{
		jobject jhashtable = t.env->CallStaticObjectMethod(t.classID, t.methodID);

		configure=PluginJniHelper::convertJavaHashTable2Map(jhashtable);
	
		t.env->DeleteLocalRef(jhashtable);
	}
	t.env->DeleteLocalRef(t.classID);
	return configure;
}

std::map<std::string, std::string> AgentManagerAndroid::getPluginConfigureFromFile(const std::string& file)
{
	std::map<std::string, std::string> configure;

	PluginJniMethodInfo t;

	if(PluginJniHelper::getStaticMethodInfo(t, "com/opensdk/framework/Wrapper", "getPluginConfigFromFile", "(Ljava/lang/String;)Ljava/util/Hashtable;"))
	{
		jstring paramFile = t.env->NewStringUTF(file.c_str());
		jobject jhashtable = t.env->CallStaticObjectMethod(t.classID, t.methodID,paramFile);

		configure=PluginJniHelper::convertJavaHashTable2Map(jhashtable);
	
		t.env->DeleteLocalRef(paramFile);
		t.env->DeleteLocalRef(jhashtable);
	}
	t.env->DeleteLocalRef(t.classID);
	return configure;
}

std::string AgentManagerAndroid::getSupportPlugin()
{
    std::string ret = "";
	
    PluginJniMethodInfo t;
    if (! PluginJniHelper::getStaticMethodInfo(t
        , "com/opensdk/framework/Wrapper"
        , "getSupportPlugin"
        , "()Ljava/lang/String;"))
    {
	    jstring strRet = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
	    ret = PluginJniHelper::jstring2string(strRet);
		t.env->DeleteLocalRef(strRet);
    }

    t.env->DeleteLocalRef(t.classID);
	
	return ret;
}

void AgentManagerAndroid::release()
{
     PluginJniMethodInfo t;
    if (! PluginJniHelper::getStaticMethodInfo(t
        , "com/opensdk/framework/Wrapper"
        , "release"
        , "()V"))
    {		
	    t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }

    t.env->DeleteLocalRef(t.classID);
}
    
}
