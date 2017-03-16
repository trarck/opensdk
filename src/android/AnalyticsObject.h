#ifndef __OPENSDK_ANALYTICS_OBJECT_H__
#define __OPENSDK_ANALYTICS_OBJECT_H__

#include "ProtocolAnalytics.h"
#include <map>
#include <string>

namespace opensdk {

typedef std::pair< std::string, std::string >   LogEventParamPair;
typedef std::map< std::string, std::string >    LogEventParamMap;

class AnalyticsObject : public ProtocolAnalytics
{
public:

	AnalyticsObject();
	
	virtual ~AnalyticsObject();
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
     @brief Start a new session.
     @param appKey The identity of the application.
     */
	virtual  void startSession();
    
	/**
     @brief Stop a session.
     @warning This interface only worked on android
     */
	virtual void stopSession();

    /**
     @brief Set the timeout for expiring a session.
     @param millis In milliseconds as the unit of time.
     @note It must be invoked before calling startSession.
     */
	virtual void setSessionContinueMillis(long millis);
    
    /**
     @brief log an error
     @param errorId The identity of error
     @param message Extern message for the error
     */
	virtual void logError(const char* errorId, const char* message);
    
	/**
     @brief log an event.
     @param eventId The identity of event
     @param paramMap Extern parameters of the event, use NULL if not needed.
     */
	virtual void logEvent(const char* eventId, LogEventParamMap* paramMap = NULL);
    
    /**
     @brief Track an event begin.
     @param eventId The identity of event
     */
    virtual void logTimedEventBegin(const char* eventId);
    
    /**
     @brief Track an event end.
     @param eventId The identity of event
     */
	virtual void logTimedEventEnd(const char* eventId);
    
	/**
     @brief Whether to catch uncaught exceptions to server.
     @warning This interface only worked on android.
     */
	virtual void setCaptureUncaughtException(bool enabled);
	
	virtual bool isFunctionSupported(const std::string& functionName);
	
private:

    std::string _pluginName;
};

} // namespace opensdk {


#endif /* __OPENSDK_ANALYTICS_OBJECT_H__ */
