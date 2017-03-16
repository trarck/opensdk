package com.opensdk.framework;

import java.util.Hashtable;

public interface InterfaceUser {
	public static final int PluginType = 5;

	public void configDeveloperInfo(Hashtable<String, String> cpInfo);

	public void login();

	public void logout();

	public boolean isLogined();

	public String getSessionID();

	public String getUserID();

	public boolean isSupportFunction(String functionName);

	public String getPluginId();

	public void setDebugMode(boolean debug);

	public String getSDKVersion();

	public String getPluginVersion();
}
