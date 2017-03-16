#ifndef __OPENSDK_ADS_OBJECT_H__
#define __OPENSDK_ADS_OBJECT_H__

#include "ProtocolAds.h"
#include <map>
#include <string>
#include <vector>

namespace opensdk {

typedef struct{
    
    AdsResultCode resultCode;
    std::string msg;
    std::string className;
} AdsActionResult;
    
class AdsObject : public ProtocolAds
{
public:
    AdsObject();
    virtual ~AdsObject();

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
    @brief config the application info
    @param devInfo This parameter is the info of aplication,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
    virtual void configDeveloperInfo(TAdsDeveloperInfo devInfo);

    /**
    @brief show adview
    @param info The information of adview will be shown
                Pay attention to the subclass definition
    @param pos The position where the adview be shown.
    */
    virtual void showAds(TAdsInfo info, AdsPos pos = kPosCenter); 

    /**
    @brief Hide the adview
    @param info The information of adview will be hided
    */
    virtual void hideAds(TAdsInfo info);

    /**
    @brief show adview
    @param the type of ads
    @param the idx of ads
    */
    virtual void showAds(AdsType adsType, int idx = 1);

    /**
    @brief Hide the adview
    @param the type of ads
    @param the idx of ads
    */
    virtual void hideAds(AdsType adsType, int idx = 1);

    /**
    @brief preload the adview
    @param the type of ads
    @param the idx of ads
    */
    virtual void preloadAds(AdsType adsType, int idx = 1);

    /**
    @brief Query the points of player
    */
    virtual float queryPoints();

    /**
    @brief Spend the points.
           Use this method to notify server spend points.
    @param points Need spend number of points
    */
    virtual void spendPoints(int points);

    /**
     @deprecated
     @brief set the Ads listener
    */
    virtual void setAdsListener(AdsListener* listener);

    /**
     @deprecated
     @brief set the Ads listener
    */
    virtual  AdsListener* getAdsListener();

    /**
    @brief Is it supportive of this type of ads
    @param the type of ads
    @return if it is supportive ,return true
             else retur false
    */
    virtual bool isAdTypeSupported(AdsType adType);
    
    static void popActionResult();
    
    static void pushActionResult(AdsActionResult actionResult);

public:
    
    static std::vector<AdsActionResult> _actionResultList;
    
private:

    std::string _pluginName;
	
	AdsListener* _listener;
};

} // namespace opensdk {

#endif /* __OPENSDK_ADS_OBJECT_H__ */
