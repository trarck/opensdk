#include <vector>

#include "AgentManager.h"
#include "PluginManager.h"
#include "PluginFactory.h"
#include "PluginJniHelper.h"
#include "PluginUtils.h"
#include <android/log.h>

namespace opensdk {

static AgentManager* s_AgentManager = NULL;

static void split(const std::string& s, const std::string& delim,std::vector< std::string >* ret)
{
	size_t last = 0;
	size_t index=s.find_first_of(delim,last);
	while (index!=std::string::npos)
	{
		ret->push_back(s.substr(last,index-last));
		last=index+1;
		index=s.find_first_of(delim,last);
	}
		//the last one or not split
	ret->push_back(s.substr(last,index-last));
}

AgentManager::AgentManager():_pAnalytics(NULL), _pUser(NULL), _pShare(NULL), _pSocial(NULL), _pAds(NULL), _pPush(NULL)
{

}

AgentManager::~AgentManager()
{
	this->unloadAllPlugin();
}

AgentManager* AgentManager::getInstance()
{
	if(NULL == s_AgentManager)
	{
		s_AgentManager = new (std::nothrow) AgentManager();
	}
	return s_AgentManager;
}

void AgentManager::destroyInstance()
{
	if(s_AgentManager)
	{
		delete s_AgentManager;
		s_AgentManager = NULL;
	}
}

void AgentManager::init()
{
	
}

void AgentManager::init(const std::string& appKey,const std::string& appSecret,const std::string& privateKey,const std::string& oauthLoginServer)
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

void AgentManager::loadPluginsFromConfig(const std::map<std::string, std::string>& conf)
{
    if(conf.empty())
        return;
    
    for(std::map<std::string, std::string>::const_iterator iter = conf.begin(); iter != conf.end(); ++iter)
    {
        std::string pluginName = iter->first;
        
        if("PluginUser" == pluginName)
        {
            _pUser = dynamic_cast<ProtocolUser *>(PluginManager::getInstance()->loadPlugin(iter->second.c_str(),kPluginUser));
        }
        else if("PluginShare" == pluginName)
        {
            _pShare = dynamic_cast<ProtocolShare *>(PluginManager::getInstance()->loadPlugin(iter->second.c_str(),kPluginShare));
        }
        else if("PluginSocial" == pluginName)
        {
            _pSocial = dynamic_cast<ProtocolSocial *>(PluginManager::getInstance()->loadPlugin(iter->second.c_str(),kPluginSocial));
        }
        else if("PluginAds" == pluginName)
        {
            _pAds = dynamic_cast<ProtocolAds *>(PluginManager::getInstance()->loadPlugin(iter->second.c_str(),kPluginAds));
        }
        else if("PluginAnalytics" == pluginName)
        {
            _pAnalytics = dynamic_cast<ProtocolAnalytics *>(PluginManager::getInstance()->loadPlugin(iter->second.c_str(),kPluginAnalytics));
        }
        else if("PluginIAP" == pluginName)
        {
            //may have group
            std::vector<std::string> iapPluginNames;
            
            split(iter->second,",",&iapPluginNames);
            
            for(std::vector<std::string>::iterator it=iapPluginNames.begin();it!=iapPluginNames.end();++it){
                ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP *>(PluginManager::getInstance()->loadPlugin(it->c_str(),kPluginIAP));
                _pluginsIAPMap[*it]=pIAP;
            }
        }
        else if("PluginPush" == pluginName)
        {
            _pPush = dynamic_cast<ProtocolPush *>(PluginManager::getInstance()->loadPlugin(iter->second.c_str(),kPluginPush));
        }
    }
}
    
void AgentManager::loadPluginsFromConfigAutoType(const std::map<std::string, std::string>& conf)
{
	if(conf.empty())
		return;

	for(std::map<std::string, std::string>::const_iterator iter = conf.begin(); iter != conf.end(); ++iter)
	{
		std::string pluginName = iter->first;
        
        if("PluginIAP" == pluginName){
            //may have group
            std::vector<std::string> iapPluginNames;

            split(iter->second,",",&iapPluginNames);

            for(std::vector<std::string>::iterator it=iapPluginNames.begin();it!=iapPluginNames.end();++it){
                ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP *>(PluginManager::getInstance()->loadPlugin(it->c_str(),kPluginIAP));
                
//                //check is iap plugin
//                if(pIAP->getPluginType()==kPluginIAP){
//                    
//                }
                
                _pluginsIAPMap[*it]=pIAP;
            }
        }else{
            PluginProtocol* plugin=PluginManager::getInstance()->loadPlugin(iter->second.c_str());
            
            switch (plugin->getPluginType()){
                case kPluginAds:
                    _pAds=dynamic_cast<ProtocolAds*>(plugin);
                    break;
                case kPluginAnalytics:
                    _pAnalytics=dynamic_cast<ProtocolAnalytics*>(plugin);
                    break;
                case kPluginShare:
                    _pShare=dynamic_cast<ProtocolShare*>(plugin);
                    break;
                case kPluginUser:
                    _pUser=dynamic_cast<ProtocolUser*>(plugin);
                    break;
                case kPluginSocial:
                    _pSocial=dynamic_cast<ProtocolSocial*>(plugin);
                    break;
                case kPluginPush:
                    _pPush=dynamic_cast<ProtocolPush*>(plugin);
                    break;
//                case kPluginIAP:{
//                    
//                }
                default:
                    break;
            }
        }
	}
}
    
void AgentManager::loadAllPlugin()
{
	unloadAllPlugin();
	
    std::map<std::string,std::string> conf = getPluginConfigure();
    if(!conf.empty()){
        loadPluginsFromConfig(conf);
    }else{
		PluginUtils::outputLog("AgentManager", "no config use debug mode");
        setDebugMode(true);
    }
}

void AgentManager::unloadAllPlugin()
{
	PluginManager* pluginManager=PluginManager::getInstance();
 
	if(_pAnalytics){
		pluginManager->unloadPlugin(_pAnalytics->getPluginName());
		_pAnalytics=NULL;
	}
	
	if(_pUser){
		pluginManager->unloadPlugin(_pUser->getPluginName());
		_pUser=NULL;
	}
	
	if(_pShare){
		pluginManager->unloadPlugin(_pShare->getPluginName());
		_pShare=NULL;
	}
	
	if(_pSocial){
		pluginManager->unloadPlugin(_pSocial->getPluginName());
		_pSocial=NULL;
	}
	
	if(_pAds){
		pluginManager->unloadPlugin(_pAds->getPluginName());
		_pAds=NULL;
	}
	
	if(_pPush){
		pluginManager->unloadPlugin(_pPush->getPluginName());
		_pPush=NULL;
	}
	
	for(std::map<std::string , ProtocolIAP*>::iterator iter=_pluginsIAPMap.begin();iter!=_pluginsIAPMap.end();++iter){
		pluginManager->unloadPlugin(iter->second->getPluginName());
	}
	
	_pluginsIAPMap.clear();
	
}

std::string AgentManager::getChannelId()
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

std::string AgentManager::getCustomParam()
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

std::map<std::string, std::string> AgentManager::getPluginConfigure()
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

std::map<std::string, std::string> AgentManager::getPluginConfigureFromFile(const std::string& file)
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

void AgentManager::setDebugMode(bool flag)
{
	if(flag){
		_pUser = dynamic_cast<ProtocolUser *>(PluginManager::getInstance()->loadPlugin("UserDebug",kPluginUser));
		_pShare = dynamic_cast<ProtocolShare *>(PluginManager::getInstance()->loadPlugin("ShareDebug",kPluginShare));
		_pSocial = dynamic_cast<ProtocolSocial *>(PluginManager::getInstance()->loadPlugin("SocialDebug",kPluginSocial));
		_pAds = dynamic_cast<ProtocolAds *>(PluginManager::getInstance()->loadPlugin("AdsDebug",kPluginAds));
		_pAnalytics = dynamic_cast<ProtocolAnalytics *>(PluginManager::getInstance()->loadPlugin("AnalyticsDebug",kPluginAnalytics));
		_pPush = dynamic_cast<ProtocolPush *>(PluginManager::getInstance()->loadPlugin("PushDebug",kPluginPush));
		
		ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP *>(PluginManager::getInstance()->loadPlugin("IAPDebug",kPluginIAP));
		_pluginsIAPMap["IAPDebug"]=pIAP;
	}
}
    
    
}
