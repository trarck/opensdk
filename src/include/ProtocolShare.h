#ifndef  __OPENSDK_PROTOCOL_SHARE_H__
#define  __OPENSDK_PROTOCOL_SHARE_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace opensdk {

typedef std::map<std::string, std::string> TShareDeveloperInfo;
typedef std::map<std::string, std::string> TShareInfo;

typedef enum 
{
    kShareSuccess = 0,
    kShareFail,
    kShareCancel,
    kShareTimeOut,
} ShareResultCode;

class ShareResultListener
{
public:
    virtual void onShareResult(ShareResultCode ret, const char* msg) = 0 ;
};

class ProtocolShare : public PluginProtocol
{
public:

    /**
    @brief config the share developer info
    @param devInfo This parameter is the info of developer,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
    virtual void configDeveloperInfo(TShareDeveloperInfo devInfo) = 0 ;
    
    /**
    @brief share information
    @param info The info of share, contains key:
            SharedText                	The text need to share
            SharedImagePath				The full path of image file need to share (optinal)
    @warning For different plugin, the parameter should have other keys to share.
             Look at the manual of plugins.
    */
    virtual void share(TShareInfo info) = 0 ;

    /**
    @deprecated
    @breif set the result listener
    @param pListener The callback object for share result
    @wraning Must invoke this interface before share
    */
    virtual void setResultListener(ShareResultListener* pListener) = 0 ;
    
    /**
     @deprecated
     @breif get the result listener
     @return The callback object for share result
     @wraning Must invoke this interface before share
     */
    virtual ShareResultListener* getResultListener() = 0 ;
    

    /**
    @brief share result callback
    */
    virtual void onShareResult(ShareResultCode ret, const char* msg) = 0 ;

};

} // namespace opensdk {

#endif   /* ----- #ifndef __OPENSDK_PROTOCOL_SHARE_H__ ----- */
