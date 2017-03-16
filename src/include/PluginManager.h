#ifndef __OPENSDK_PLUGINMANAGER_H__
#define __OPENSDK_PLUGINMANAGER_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace opensdk {

class PluginManager
{
public:
	virtual ~PluginManager();
	/** Get singleton of PluginManager */
    static PluginManager* getInstance();
    /** Destory the instance of PluginManager */
    static void end();
    
    /** load the plugin by name */
    PluginProtocol* loadPlugin(const char* name,int pluginType=0);
    
    /** unload the plugin by name */
    void unloadPlugin(const char* name);

private:
    PluginManager(void);
    std::map<std::string, PluginProtocol*> _pluginsMap;
};

} //namespace opensdk {

#endif /* __OPENSDK_PLUGINMANAGER_H__ */
