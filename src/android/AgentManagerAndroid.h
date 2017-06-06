#ifndef __OPENSDK_AGENT_MANAGER_ANDROID_H__
#define __OPENSDK_AGENT_MANAGER_ANDROID_H__


#include "AgentManager.h"

namespace opensdk {

class AgentManagerAndroid:public AgentManager{
public:

	virtual ~AgentManagerAndroid();
		
	virtual void init();
	
	virtual void init(const std::string& appKey,const std::string& appSecret,const std::string& privateKey,const std::string& oauthLoginServer);
	
   /**
    @brief Get channel ID
    @return  return value is channel ID.
    */
   virtual std::string getChannelId();

   /**
    @brief Get custom param
    @return  return value is custom param for channel.
    */
   virtual std::string getCustomParam();

protected:
    AgentManagerAndroid();
    
	virtual std::map<std::string, std::string> getPluginConfigure();
	virtual std::map<std::string, std::string> getPluginConfigureFromFile(const std::string& file);
    virtual std::string getSupportPlugin();
    virtual void release();
};
} //namespace opensdk {
#endif //__OPENSDK_AGENT_MANAGER_ANDROID_H__
