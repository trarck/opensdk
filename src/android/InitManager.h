#ifndef __OPENSDK_OPENSDK_H__
#define __OPENSDK_OPENSDK_H__

namespace opensdk {

class InitListener
{
public:
    virtual void onInitPlugins() = 0;
};

class InitManager
{
public:
    virtual ~InitManager();

	static InitManager* getInstance();
	static void destroyInstance();

	void onInitPlugins();

	void setListener(InitListener* pListener);
    
    inline InitListener* getListener()
    {
        return _listener;
    }

private:
	InitListener* _listener;
};

} //namespace opensdk {

#endif /* __OPENSDK_OPENSDK_H__ */
