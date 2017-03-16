#ifndef  __OPENSDK_SHARE_OBJECT_H__
#define  __OPENSDK_SHARE_OBJECT_H__

#include "ProtocolShare.h"
#include <map>
#include <string>

namespace opensdk {

typedef struct
{
    ShareResultCode resultCode;
    std::string msg;
    std::string className;
    
} ShareActionResult;

    
class ShareObject : public ProtocolShare
{
public:
	ShareObject();
	virtual ~ShareObject();

    /**
     *@brief set plugin name
     */
    virtual void setPluginName(const char* name);
    
    /**
     *@brief get plugin name
     */
    virtual const char* getPluginName();
    
    /**
     *@brief get plugin type
     */
    virtual int getPluginType();
    
    /**
    @brief config the share developer info
    @param devInfo This parameter is the info of developer,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
    void configDeveloperInfo(TShareDeveloperInfo devInfo);

    /**
    @brief share information
    @param info The info of share, contains key:
            SharedText                	The text need to share
            SharedImagePath				The full path of image file need to share (optinal)
    @warning For different plugin, the parameter should have other keys to share.
             Look at the manual of plugins.
    */
    void share(TShareInfo info);

    /**
    @deprecated
    @breif set the result listener
    @param pListener The callback object for share result
    @wraning Must invoke this interface before share
    */
    void setResultListener(ShareResultListener* pListener);
    
    /**
     @deprecated
     @breif get the result listener
     @return The callback object for share result
     @wraning Must invoke this interface before share
     */
    ShareResultListener* getResultListener();
    

    /**
    @brief share result callback
    */
    void onShareResult(ShareResultCode ret, const char* msg);
public:
    
    static void popActionResult();
    
    static void pushActionResult(const ShareActionResult& actionResult);
    
    
public:
    static std::vector<ShareActionResult> _actionResultList;
    
protected:
    std::string _pluginName;
    ShareResultListener* _listener;
};

} // namespace opensdk {

#endif   /* ----- #ifndef __OPENSDK_SHARE_OBJECT_H__ ----- */
