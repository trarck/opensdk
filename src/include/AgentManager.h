#ifndef __OPENSDK_AGENT_MANAGER_H__
#define __OPENSDK_AGENT_MANAGER_H__

#include <string>
#include <map>

#include "ProtocolIAP.h"
#include "ProtocolAnalytics.h"
#include "ProtocolShare.h"
#include "ProtocolAds.h"
#include "ProtocolSocial.h"
#include "ProtocolUser.h"
#include "ProtocolPush.h"

namespace opensdk {

class AgentManager{
public:

	virtual ~AgentManager();
	
	static AgentManager* getInstance();
	
	static void destroyInstance();
	
	virtual void init();
	
	virtual void init(const std::string& appKey,const std::string& appSecret,const std::string& privateKey,const std::string& oauthLoginServer);
	
	virtual void loadPluginsFromConfig(const std::map<std::string, std::string>& conf);
    
    /**
     @brief plugin type define in the java/objc.
     */
    virtual void loadPluginsFromConfigAutoType(const std::map<std::string, std::string>& conf);
	
	virtual void loadAllPlugin();
	
	virtual void unloadAllPlugin();
	
   /**
    @brief Get Analytics plugin
    @return  if Analytics plugin exist ,return value is Analytics plugin.
    	 	  else return value is null pointer.
    */
   ProtocolAnalytics* getAnalyticsPlugin(){return _pAnalytics;};

   /**
    @brief Get User plugin
    @return  if User plugin exist ,return value is User plugin.
    	 	  else return value is null pointer.
    */
   ProtocolUser* getUserPlugin(){return _pUser;};

   /**
    @brief Get IAP plugin
    @return  if IAP plugin exist ,return value is IAP plugin.
    	 	  else return value is null pointer.
    */
   std::map<std::string , ProtocolIAP*>* getIAPPlugin(){return &_pluginsIAPMap;};

   /**
    @brief Get Share plugin
    @return  if Share plugin exist ,return value is Share plugin.
    	 	  else return value is null pointer.
    */
   ProtocolShare* getSharePlugin(){return _pShare;};

   /**
    @brief Get Social plugin
    @return  if Social plugin exist ,return value is Social plugin.
    	 	  else return value is null pointer.
    */
   ProtocolSocial* getSocialPlugin(){return _pSocial;};

   /**
    @brief Get Ads plugin
    @return  if Ads plugin exist ,return value is Ads plugin.
    	 	  else return value is null pointer.
    */
   ProtocolAds* getAdsPlugin(){return _pAds;};
   
   /**
    @brief Get Push plugin
    @return  if Push plugin exist ,return value is Push plugin.
    	 	  else return value is null pointer.
    */
   ProtocolPush* getPushPlugin(){return _pPush;};

   /**
    @brief Get channel ID
    @return  return value is channel ID.
    */
   std::string getChannelId();

   /**
    @brief Get custom param
    @return  return value is custom param for channel.
    */
   std::string getCustomParam();

   /**
   @Title: setIsAnaylticsEnabled
   @Description: choose to open or close
   @param @param enabled    true or false
   @return void
   */
   void setIsAnaylticsEnabled(bool value){bIsAnaylticsEnabled = value;};
   /**
   @Title: isAnaylticsEnabled
   @Description: the status of Anayltics
   @param @return    true or false
   @return boolean
   */
   bool isAnaylticsEnabled(){return bIsAnaylticsEnabled;};

protected:
	AgentManager();
	std::map<std::string, std::string> getPluginConfigure();
	std::map<std::string, std::string> getPluginConfigureFromFile(const std::string& file);

	void setDebugMode(bool flag);

    // Analytics plugins
    ProtocolAnalytics* _pAnalytics;

    // user plugins
    ProtocolUser* _pUser;

    // IAP plugins
    //    ProtocolIAP* _pIAP;
    std::map<std::string , ProtocolIAP*> _pluginsIAPMap;

    // Share plugins
    ProtocolShare* _pShare;

    // Social plugins
    ProtocolSocial* _pSocial;

    // Ads plugins
    ProtocolAds* _pAds;
    
    // Push plugins
    ProtocolPush* _pPush;

    bool bIsAnaylticsEnabled;
	// ProtocolIAP *pIAP;
};
} //namespace opensdk {
#endif //__OPENSDK_AGENT_MANAGER_H__
