package com.opensdk.framework;

import java.util.Hashtable;

public interface InterfaceShare {
	public static final int PluginType = 4;

	public void configDeveloperInfo(Hashtable<String, String> cpInfo);

	public void share(Hashtable<String, String> cpInfo);

	public void setDebugMode(boolean debug);

	public String getSDKVersion();

	public String getPluginVersion();
}
