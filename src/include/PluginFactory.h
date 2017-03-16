#ifndef __OPENSDK_PLUGIN_FACTORY_H__
#define __OPENSDK_PLUGIN_FACTORY_H__

namespace opensdk {

class PluginProtocol;
class PluginManager;

class PluginFactory
{
public:
	virtual ~PluginFactory();
	/** Get singleton of PluginFactory */
    static PluginFactory* getInstance();

    /** Destory the instance of PluginFactory */
    static void purgeFactory();

private:
    friend class PluginManager;
    PluginFactory(void);

    /** create the plugin by name */
    PluginProtocol* createPlugin(const char* name,int pluginType=0);
};

} //namespace opensdk {

#endif /* __OPENSDK_PLUGIN_FACTORY_H__ */
