#include "PluginManager.h"
#include "PluginFactory.h"

namespace opensdk {

static PluginManager* s_pPluginManager = NULL;

PluginManager::PluginManager(void)
{
}

PluginManager::~PluginManager()
{
	std::map<std::string, PluginProtocol*>::iterator it = _pluginsMap.begin();
    for (;it != _pluginsMap.end();++it)
    {
        if (it->second != NULL) {
            delete it->second;
            it->second = NULL;
        }
    }
    _pluginsMap.clear();
}

PluginManager* PluginManager::getInstance()
{
    if (s_pPluginManager == NULL)
    {
        s_pPluginManager = new PluginManager();
    }
    return s_pPluginManager;
}

void PluginManager::end()
{
    if (s_pPluginManager != NULL)
    {
        delete s_pPluginManager;
        s_pPluginManager = NULL;
    }

    PluginFactory::purgeFactory();
}

PluginProtocol* PluginManager::loadPlugin(const char* name,int pluginType)
{
    PluginProtocol* pRet = NULL;
    do {
        if (name == NULL || strlen(name) == 0) break;
        std::map<std::string, PluginProtocol*>::iterator it = _pluginsMap.find(name);
        if (it != _pluginsMap.end())
        {
            if (it->second == NULL) {
                it->second = PluginFactory::getInstance()->createPlugin(name,pluginType);
            }
            pRet = it->second;
        } else
        {
        	pRet = PluginFactory::getInstance()->createPlugin(name,pluginType);
        	_pluginsMap[name] = pRet;
        }
    } while (false);

    return pRet;
}

void PluginManager::unloadPlugin(const char* name)
{
    do {
        if (name == NULL || strlen(name) == 0) break;
        std::map<std::string, PluginProtocol*>::iterator it = _pluginsMap.find(name);
		if (it != _pluginsMap.end())
        {
            if (it->second != NULL) {
                delete it->second;
                it->second = NULL;
            }
        }
    } while (false);
}

} //namespace cocos2d { namespace plugin {
