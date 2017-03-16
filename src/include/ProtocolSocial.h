#ifndef  __OPENSDK_PROTOCOL_SOCIAL_H__
#define  __OPENSDK_PROTOCOL_SOCIAL_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace opensdk {

typedef std::map<std::string, std::string> TSocialDeveloperInfo;
typedef std::map<std::string, std::string> TAchievementInfo;

typedef enum
{
    // code for leaderboard feature
    kScoreSubmitSucceed =1,/**< enum value is callback of succeeding in submiting. */
    kScoreSubmitfail,/**< enum value is callback of failing to submit . */
    
    // code for achievement feature
    kAchUnlockSucceed,/**< enum value is callback of succeeding in unlocking. */
    kAchUnlockFail,/**< enum value is callback of failing to  unlock. */
    
    kSocialSignInSucceed,/**< enum value is callback of succeeding to login. */
    kSocialSignInFail,/**< enum value is callback of failing to  login. */
    
    kSocialSignOutSucceed,/**< enum value is callback of succeeding to login. */
    kSocialSignOutFail,/**< enum value is callback of failing to  login. */

} SocialRetCode;

class SocialListener
{
public:
    virtual void onSocialResult(SocialRetCode code, const char* msg) = 0;
};

class ProtocolSocial : public PluginProtocol
{
public:

    /**
    @brief config the share developer info
    @param devInfo This parameter is the info of developer,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
    virtual void configDeveloperInfo(TSocialDeveloperInfo devInfo) = 0;
    
    /**
     @brief user signIn
     */
    virtual void signIn() = 0;
    
    /**
     @brief user signOut
     */
    virtual void signOut() = 0;

    /**
     * @brief methods of leaderboard feature
     */
    virtual void submitScore(const char* leadboardID, long score) = 0;

    virtual void showLeaderboard(const char* leaderboardID) = 0;

    /**
     * @brief methods of achievement feature
     */
    virtual void unlockAchievement(TAchievementInfo achInfo) = 0;

    virtual void showAchievements() = 0;

    /*
     @deprecated
     @brief set listener
     */
    virtual void setListener(SocialListener* listener) = 0;
    /*
     @deprecated
     @brief get listener
     */
    virtual SocialListener* getListener() = 0;

};

} // namespace opensdk {

#endif   /* ----- #ifndef __OPENSDK_PROTOCOL_SOCIAL_H__ ----- */
