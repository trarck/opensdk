#ifndef __OPENSDK_IAP_OBJECT_H__
#define __OPENSDK_IAP_OBJECT_H__

#include "ProtocolIAP.h"
#include <map>
#include <string>
#include <vector>

namespace opensdk {

typedef struct 
{
	PayResultCode resultCode;
	std::string msg;
    std::string className;
    
} IAPActionResult;

class IAPObject : public ProtocolIAP
{
public:

	IAPObject();
	
	virtual ~IAPObject();
	
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
    @brief config the developer info
    @param devInfo This parameter is the info of developer,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
    virtual void configDeveloperInfo(TIAPDeveloperInfo devInfo);

    /**
    @brief pay for product
    @param info The info of product, must contains key:
            productName         The name of product
            productPrice        The price of product(must can be parse to float)
            productDesc         The description of product
    @warning For different plugin, the parameter should have other keys to pay.
             Look at the manual of plugins.
    */
    virtual void payForProduct(TProductInfo info);
	
   /**
    @brief get order id
    @return the order id
    */
   virtual std::string getOrderId();

    /**
    @deprecated
    @breif set the result listener
    @param pListener The callback object for pay result
    @wraning Must invoke this interface before payForProduct.
    */
    virtual void setResultListener(PayResultListener* pListener);
    
    /**
    @deprecated
    @breif get the result listener
    */
    virtual PayResultListener* getResultListener();
	
   /**
    @brief get plugin id
    @return the plugin id
    */
	virtual std::string getPluginId();
    
    /**
    @brief pay result callback
    */
    virtual void onPayResult(PayResultCode ret, const char* msg);

public:
    
	static void popActionResult();
	
	static void pushActionResult(const IAPActionResult& actionResult);


public:
    static std::vector<IAPActionResult> _actionResultList;
    
protected:
    
	std::string _pluginName;
    
    TProductInfo _curInfo;
    
    PayResultListener* _listener;
	

};

} // namespace opensdk {

#endif /* __OPENSDK_IAP_OBJECT_H__ */
