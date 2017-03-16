package com.opensdk.framework;

import java.util.Hashtable;

public interface InterfaceAnalytics {

	public static final int PluginType = 2;

	public void startSession();

	public void stopSession();

	public void setSessionContinueMillis(int millis);

	public void setCaptureUncaughtException(boolean isEnabled);

	public void setDebugMode(boolean isDebugMode);

	public void logError(String errorId, String message);

	public void logEvent(String eventId);

	public void logEvent(String eventId, Hashtable<String, String> paramMap);

	public void logTimedEventBegin(String eventId);

	public void logTimedEventEnd(String eventId);

	public String getSDKVersion();

	public String getPluginVersion();
	
	public boolean isFunctionSupported(String functionName);
}
