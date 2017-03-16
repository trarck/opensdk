#include "AnalyticsObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"

namespace opensdk {

AnalyticsObject::AnalyticsObject()
: _pluginName("")
{
}

AnalyticsObject::~AnalyticsObject()
{
}

void AnalyticsObject::setPluginName(const char* name)
{
	_pluginName = name;
}

const char* AnalyticsObject::getPluginName()
{
	return _pluginName.c_str();
}

int AnalyticsObject::getPluginType()
{
    return kPluginAnalytics;
}
    
void AnalyticsObject::startSession()
{
	 PluginUtils::callJavaFunctionWithName(this, "startSession");
}

void AnalyticsObject::stopSession()
{
    PluginUtils::callJavaFunctionWithName(this, "stopSession");
}

void AnalyticsObject::setSessionContinueMillis(long millis)
{
	PluginUtils::callJavaFunctionWithName_oneParam(this, "setSessionContinueMillis", "(I)V", millis);
}

void AnalyticsObject::setCaptureUncaughtException(bool isEnabled)
{
	PluginUtils::callJavaFunctionWithName_oneParam(this, "setCaptureUncaughtException", "(Z)V", isEnabled);
}

void AnalyticsObject::logError(const char* errorId, const char* message)
{
    PluginUtils::callJavaFunctionWithName_string_string(this, "logError", errorId, message);
}

void AnalyticsObject::logEvent(const char* eventId, LogEventParamMap* pParams/* = NULL */)
{
	PluginUtils::callJavaFunctionWithName_string_map(this, "logEvent", eventId, pParams);
}

void AnalyticsObject::logTimedEventBegin(const char* eventId)
{
	PluginUtils::callJavaFunctionWithName_string_map(this, "logTimedEventBegin", eventId, NULL);
}

void AnalyticsObject::logTimedEventEnd(const char* eventId)
{
	PluginUtils::callJavaFunctionWithName_string_map(this, "logTimedEventEnd", eventId, NULL);
}
    
bool AnalyticsObject::isFunctionSupported(const std::string& functionName)
{
    PluginParam paramFunctionName(functionName.c_str());
    return callBoolFuncWithParam("isFunctionSupported", &paramFunctionName,NULL);
}

} //namespace plugin {
