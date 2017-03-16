package com.opensdk.framework;

import java.util.Hashtable;

public interface InterfaceIAP {
	public static final int PluginType = 3;

	public void configDeveloperInfo(Hashtable<String, String> cpInfo);

	public void payForProduct(Hashtable<String, String> productInfo);

	public void setDebugMode(boolean debug);

	public String getSDKVersion();

	public String getPluginVersion();

	public String getPluginId();

	public String getOrderId();
}
