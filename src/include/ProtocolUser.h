#ifndef __OPENSDK_PROTOCOL_USER_H__
#define __OPENSDK_PROTOCOL_USER_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace opensdk {

typedef std::map<std::string, std::string> TUserDeveloperInfo;

typedef enum
{
    kInitSuccess = 0,/**< enum value is callback of succeeding in initing sdk. */
    kInitFail,/**< enum  value is callback of failing to init sdk. */
    kLoginSuccess,/**< enum value is callback of succeeding in login.*/
    kLoginNetworkError,/**< enum value is callback of network error*/
    kLoginNoNeed,/**< enum value is callback of no need login.*/
    kLoginFail,/**< enum value is callback of failing to login. */
    kLoginCancel,/**< enum value is callback of canceling to login. */
    kLogoutSuccess,/**< enum value is callback of succeeding in logout. */
    kLogoutFail,/**< enum value is callback of failing to logout. */
    kPlatformEnter,/**< enum value is callback after enter platform. */
    kPlatformBack,/**< enum value is callback after exit antiAddiction. */
    kPausePage,/**< enum value is callback after exit pause page. */
    kExitPage,/**< enum value is callback after exit exit page. */
    kAntiAddictionQuery,/**< enum value is callback after querying antiAddiction. */
    kRealNameRegister,/**< enum value is callback after registering realname. */
    kAccountSwitchSuccess,/**< enum alue is callback of succeeding in switching account. */
    kAccountSwitchFail,/**< enum value is callback of failing to switch account. */
    kOpenShop,/**< enum value is callback of open the shop. */
    
} UserActionResultCode;
    
typedef enum
{
    kToolBarTopLeft = 1,/**< enum the toolbar is at topleft. */
    kToolBarTopRight,/**< enum the toolbar is at topright. */
    kToolBarMidLeft,/**< enum the toolbar is at midleft. */
    kToolBarMidRight,/**< enum the toolbar is at midright. */
    kToolBarBottomLeft,/**< enum the toolbar is at bottomleft. */
    kToolBarBottomRight,/**< enum the toolbar is at bottomright. */
} ToolBarPlace;

class ProtocolUser;
    
class UserActionListener
{
public:
    virtual void onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg) = 0;
};

class ProtocolUser : public PluginProtocol
{
public:

    /**
    @brief config the application info
    @param devInfo This parameter is the info of aplication,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
    virtual void configDeveloperInfo(TUserDeveloperInfo devInfo) = 0 ;

    /**
     @brief User login
     */
    virtual void login() = 0 ;
    
    virtual void login(const std::string& server_id, const std::string& oauthLoginServer = "") = 0 ;

    /**
     @brief User logout
     */
    virtual void logout() = 0 ;

    /**
     @brief Check whether the user logined or not
     */
    virtual bool isLogined() = 0 ;
    
    virtual std::string getUserID() = 0 ;
    
    /**
     @brief Get session ID
     @return If user logined, return value is session ID;
             else return value is empty string.
     */
    virtual std::string getSessionID() = 0 ;
    
    /**
     @brief get Access Token
     */
    virtual std::string getAccessToken() = 0 ;

    /*
     @deprecated
     @brief set login callback function
     */
    virtual void setActionListener(UserActionListener* listener) = 0 ;
    
    /*
     @deprecated
     @brief get login callback function
     */
    virtual UserActionListener* getActionListener() = 0 ;
    
    /**
     @brief Check function the plugin support or not
     @param the name of plugin
     @return if the function support ,return true
     else retur false
     */
    virtual bool isFunctionSupported(const std::string& functionName) = 0 ;
    
    /**
     @brief get plugin id
     @return the plugin id
     */
    virtual std::string getPluginId() = 0 ;
};

} // namespace opensdk {

#endif /* __OPENSDK_PROTOCOL_USER_H__ */
