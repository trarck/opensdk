#include "InitManager.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"

namespace opensdk {

static InitManager* s_initManager = NULL;

InitManager::~InitManager()
{

}

InitManager* InitManager::getInstance()
{
	if(NULL == s_initManager)
	{
		s_initManager = new InitManager();
	}
	return s_initManager;
}

void InitManager::destroyInstance()
{
	if(s_initManager)
	{
		delete s_initManager;
		s_initManager = NULL;
	}
}

void InitManager::onInitPlugins()
{
	if (_listener)
    {
    	_listener->onInitPlugins();
    }
    else
    {
        PluginUtils::outputLog("InitManager", "Result listener is null!");
    }
}

void InitManager::setListener(InitListener* pListener)
{
	_listener = pListener;
}

}
