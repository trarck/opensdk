#ifndef __OPENSDK_PROTOCOL_IAP_H__
#define __OPENSDK_PROTOCOL_IAP_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace opensdk {

typedef std::map<std::string, std::string> TIAPDeveloperInfo;
typedef std::map<std::string, std::string> TProductInfo;
typedef std::vector<TProductInfo> TProductList;
//typedef std::map<std::string, TProductInfo> AllProductsInfo;

typedef enum 
{
    kPaySuccess = 0,/**< enum value is callback of succeeding in paying . */
    kPayFail,/**< enum value is callback of failing to pay . */
    kPayCancel,/**< enum value is callback of canceling to pay . */
    kPayNetworkError,/**< enum value is callback of network error . */
    kPayProductionInforIncomplete,/**< enum value is callback of incompleting info . */
	kPayInitSuccess,/**< enum value is callback of succeeding in initing sdk . */
	kPayInitFail,/**< enum value is callback of failing to init sdk . */
	kPayNowPaying,/**< enum value is callback of paying now . */
	kPayRechargeSuccess,/**< enum value is callback of  succeeding in recharging. */
} PayResultCode;
    
typedef enum {
    kRequestSuccess = 31000,/**< enum value is callback of succeeding in paying . */
    kRequestFail/**< enum value is callback of failing to pay . */
} RequestResultCode;

class PayResultListener
{
public:
    virtual void onPayResult(PayResultCode ret, const char* msg, TProductInfo info) = 0;
    virtual void onRequestResult(RequestResultCode ret, const char* msg, TProductList info){}
};

class ProtocolIAP : public PluginProtocol
{
public:

    /**
    @brief config the developer info
    @param devInfo This parameter is the info of developer,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
    virtual void configDeveloperInfo(TIAPDeveloperInfo devInfo) = 0;

    /**
    @brief pay for product
    @param info The info of product, must contains key:
            productName         The name of product
            productPrice        The price of product(must can be parse to float)
            productDesc         The description of product
    @warning For different plugin, the parameter should have other keys to pay.
             Look at the manual of plugins.
    */
    virtual void payForProduct(TProductInfo info) = 0;
	
   /**
    @brief get order id
    @return the order id
    */
    virtual std::string getOrderId() = 0;

    /**
    @deprecated
    @breif set the result listener
    @param pListener The callback object for pay result
    @wraning Must invoke this interface before payForProduct.
    */
    virtual void setResultListener(PayResultListener* pListener) = 0;
    
    /**
    @deprecated
    @breif get the result listener
    */
    virtual PayResultListener* getResultListener() = 0;
	
   /**
    @brief get plugin id
    @return the plugin id
    */
	virtual std::string getPluginId() = 0 ;
    
    /**
    @brief pay result callback
    */
    virtual void onPayResult(PayResultCode ret, const char* msg) = 0;

	static void resetPayState()
	{
		_paying = false;
	}
	
protected:
    //同时只能有一个支付插件在付款。
    //如果有特殊需求，比如几个不相干的插件分别付款，可以把正在支付标志放在IapObject的非静态成员里。这种需求很少见。
    //放在这里也比较奇怪。
    static bool _paying;
	
};

} // namespace opensdk {

#endif /* __OPENSDK_PROTOCOL_IAP_H__ */
