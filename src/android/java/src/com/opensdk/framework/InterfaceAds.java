package com.opensdk.framework;

import java.util.Hashtable;

import org.json.JSONObject;

public interface InterfaceAds {

	public static final int PluginType = 1;

	public void configDeveloperInfo(Hashtable<String, String> devInfo);

	public void showAds(Hashtable<String, String> adsInfo, int pos);

	public void hideAds(Hashtable<String, String> adsInfo);

	public void showAds(JSONObject info);

	public void hideAds(JSONObject info);

	public void preloadAds(JSONObject info);

	public boolean isAdTypeSupported(int adType);

	public float queryPoints();

	public void spendPoints(int points);

	public void setDebugMode(boolean debug);

	public String getSDKVersion();

	public String getPluginVersion();
}
