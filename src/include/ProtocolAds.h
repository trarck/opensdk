#ifndef __OPENSDK_PROTOCOL_ADS_H__
#define __OPENSDK_PROTOCOL_ADS_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace opensdk {

typedef std::map<std::string, std::string> TAdsDeveloperInfo;
typedef std::map<std::string, std::string> TAdsInfo;

typedef enum
{
    kAdsReceived = 0,            // The ad is received

    kAdsShown,                  // The advertisement shown
    kAdsDismissed,              // The advertisement dismissed

    kPointsSpendSucceed,        // The points spend succeed
    kPointsSpendFailed,         // The points spend failed

    kNetworkError,              // Network error
    kUnknownError,              // Unknown error
	
	kOfferWallOnPointsChanged, 
} AdsResultCode;

typedef enum {
    kPosCenter = 0,/**< enum the toolbar is at center. */
    kPosTop,/**< enum the toolbar is at top. */
    kPosTopLeft,/**< enum the toolbar is at topleft. */
    kPosTopRight,/**< enum the toolbar is at topright. */
    kPosBottom,/**< enum the toolbar is at bottom. */
    kPosBottomLeft,/**< enum the toolbar is at bottomleft. */
    kPosBottomRight,/**< enum the toolbar is at bottomright. */
} AdsPos;

typedef enum {
	AD_TYPE_BANNER = 0,/**< enum value is banner ads . */
	AD_TYPE_FULLSCREEN,/**< enum value is fullscreen ads . */
	AD_TYPE_MOREAPP,/**< enum value is moreapp ads . */
	AD_TYPE_OFFERWALL,/**< enum value is offerwall ads . */
} AdsType;

class ProtocolAds;
    
class AdsListener
{
public:
    /**
    @brief The advertisement request result
    */
    virtual void onAdsResult(AdsResultCode code, const char* msg) = 0;
    
    /**
    @brief Player get points from advertisement(For example: Tapjoy)
    @param points The point number player has got.
    @param pAdsPlugin  The plugin which the player get points. Used to spend the points.
    */
    virtual void onPlayerGetPoints(ProtocolAds* pAdsPlugin, int points) {}
};

class ProtocolAds : public PluginProtocol
{
public:

    /**
    @brief config the application info
    @param devInfo This parameter is the info of aplication,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
    virtual void configDeveloperInfo(TAdsDeveloperInfo devInfo) =0 ;

    /**
    @brief show adview
    @param info The information of adview will be shown
                Pay attention to the subclass definition
    @param pos The position where the adview be shown.
    */
    virtual void showAds(TAdsInfo info, AdsPos pos = kPosCenter) =0 ; 

    /**
    @brief Hide the adview
    @param info The information of adview will be hided
    */
    virtual void hideAds(TAdsInfo info) =0 ;
	
    /**
    @brief show adview
    @param the type of ads
    @param the idx of ads
    */
    virtual void showAds(AdsType adsType, int idx = 1) = 0;

    /**
    @brief Hide the adview
    @param the type of ads
    @param the idx of ads
    */
    virtual void hideAds(AdsType adsType, int idx = 1) = 0;

    /**
    @brief preload the adview
    @param the type of ads
    @param the idx of ads
    */
    virtual void preloadAds(AdsType adsType, int idx = 1)= 0;

    /**
    @brief Query the points of player
    */
    virtual float queryPoints() =0;

    /**
    @brief Spend the points.
           Use this method to notify server spend points.
    @param points Need spend number of points
    */
    virtual void spendPoints(int points) = 0;

    /**
     @deprecated
     @brief set the Ads listener
    */
    virtual void setAdsListener(AdsListener* listener) = 0;

    /**
     @deprecated
     @brief set the Ads listener
    */
    virtual  AdsListener* getAdsListener() = 0;
	
   /**
    @brief Is it supportive of this type of ads
    @param the type of ads
    @return if it is supportive ,return true
    	 	 else retur false
    */
   virtual bool isAdTypeSupported(AdsType adType) = 0;
};

} // namespace opensdk {

#endif /* __OPENSDK_PROTOCOL_ADS_H__ */
