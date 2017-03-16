package com.opensdk.framework;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.util.Log;
import android.util.Xml;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.StringReader;
import java.util.Hashtable;

import org.xmlpull.v1.XmlPullParser;

public class Wrapper {
	private static final String TAG = "Wrapper";
	private static final String DEVELOPER_INFO_FILE = "developerInfo.xml";
	
	private static Hashtable<String, String> mInfo = new Hashtable<String, String>();
	private static boolean bGetDeveloperInfo = false;
	private static Context mContext;

	protected static void LogD(String msg) {
		Log.d("Wrapper", msg);
	}

	public static void analysisDeveloperInfo(Context context) {
		mContext = context;
		if (!bGetDeveloperInfo) {
			bGetDeveloperInfo = true;
			Log.d(TAG, "analysisDeveloperInfo");
			
			try {
				InputStreamReader inStreamReader = new InputStreamReader(context.getAssets().open(DEVELOPER_INFO_FILE));
				
				BufferedReader bufferedReader = new BufferedReader(inStreamReader);
				
				StringBuilder data = new StringBuilder();
				String str = null;
				
				while ((str = bufferedReader.readLine()) != null) {
					data.append(str);
				}
				
				bufferedReader.close();
				
				str=data.toString();
				
				XmlPullParser xmlParser = Xml.newPullParser();
				xmlParser.setInput(new StringReader(str));
				int eventType = xmlParser.getEventType();
				int i=0;
				String key=null;
				String value=null;
				
				while (eventType != XmlPullParser.END_DOCUMENT) {
					switch (eventType) {
						case XmlPullParser.START_TAG:
							i = xmlParser.getAttributeCount();
							for (int j = 0; j < i; j++) {
								key = xmlParser.getAttributeName(j);
								value = xmlParser.getAttributeValue(j);
								mInfo.put(key, value);
							}
							break;
					}
					
					eventType=xmlParser.next();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			
		}
	}

	public static Hashtable<String, String> getDeveloperInfo() {
		return mInfo;
	}

	public static String getChannelId() {
		try {
			ApplicationInfo ai = mContext.getPackageManager().getApplicationInfo(mContext.getPackageName(), PackageManager.GET_META_DATA);
			Bundle bundle = ai.metaData;
			
			if(null!=bundle && bundle.containsKey("ASC_ChannelID")){
				return (String) bundle.get("ASC_ChannelID");
			}
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return "999999";
	}

	public static String getCustomParam() {
		if (mInfo.containsKey("extChannel"))
			return (String) mInfo.get("extChannel");
		return "";
	}

	public static void setAppParam(String apiKey, String apiSecret,String privateKey, String authLoginServer) {
		if ( (null!=apiKey && apiKey.equals(""))
			&& (null!=apiSecret && apiSecret.equals(""))
			&& (null!=privateKey && privateKey.equals("")))
			
			return;
	
		if (getChannelId().equals("999999")) {
			mInfo.put("uApiKey", apiKey);
			mInfo.put("uApiSecret", apiSecret);
			if(authLoginServer!=null && authLoginServer.length()>0)
				mInfo.put("oauthLoginServer", authLoginServer);
		}
		
		mInfo.put("private_key", privateKey);
	}

//	public static void pluginHttp(Context paramContext,
//			Hashtable<String, String> paramHashtable,
//			SdkHttpListener paramSdkHttpListener) {
//		g.c(paramContext, paramHashtable, paramSdkHttpListener);
//	}

//	public static String pluginDecode(String paramString) {
//		return g.a(paramString);
//	}

	public static String getFileContentWithName(String file) {
		StringBuilder stringBuilder = new StringBuilder();
		String str = "";
		Context content = PluginWrapper.getContext();
		try {
			InputStreamReader inputStreamReader = new InputStreamReader(content.getAssets().open(file));
			BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
			while ((str = bufferedReader.readLine()) != null)
				stringBuilder.append(str);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return stringBuilder.toString();
	}

	public static String getApkPath() {
		return PluginWrapper.getContext().getApplicationInfo().sourceDir;
	}

	public static String getApplicationName() {
		Context context=PluginWrapper.getContext();
		try {
			ApplicationInfo ai = context.getPackageManager().getApplicationInfo(context.getPackageName(),0);
			return (String)context.getPackageManager().getApplicationLabel(ai);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public static String getPluginConfigString(String asset) {
		StringBuilder pathBuilder = new StringBuilder();

		Context context = PluginWrapper.getContext();
		try {
			InputStreamReader inStreamReader = new InputStreamReader(context
					.getAssets().open(asset));
			BufferedReader buffReader = new BufferedReader(inStreamReader);
			StringBuilder data = new StringBuilder();

			String str = null;

			while ((str = buffReader.readLine()) != null) {
				data.append(str);
			}
			
//			inStreamReader.close();
			buffReader.close();

			str = data.toString();

			XmlPullParser xmlParser = Xml.newPullParser();
			xmlParser.setInput(new StringReader(str));
			int eventType = xmlParser.getEventType();

			while (eventType != XmlPullParser.END_DOCUMENT) {
				switch (eventType) {
					case XmlPullParser.START_DOCUMENT:
						break;
					case XmlPullParser.START_TAG:
						break;
					case XmlPullParser.END_TAG:
						break;
					case XmlPullParser.TEXT:
						pathBuilder.append(xmlParser.getText());
						break;
				}
				
				eventType=xmlParser.next();
			}

			return pathBuilder.toString();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
	private static String[] arrPlugins = {"PluginUser", "PluginShare", "PluginSocial", "PluginAds", "PluginAnalytics", "PluginIAP","PluginPush"};
    
    public static Hashtable<String, String> getPluginConfigure()
    {
        Hashtable<String, String> ht = new Hashtable<String, String>();
        Context context = PluginWrapper.getContext();
        
        try {
            ApplicationInfo ai = context.getPackageManager().getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA);
            Bundle bundle = ai.metaData;
            if(null!=bundle){
            	for(int i = 0; i < arrPlugins.length; ++i)
                {
                    String pluginName = bundle.getString(arrPlugins[i]);
                    if(null != pluginName && !"".equals(pluginName))
                        ht.put(arrPlugins[i], pluginName);
                }
            }
        } catch (NameNotFoundException e) {
            e.printStackTrace();
        }
        
        return ht;
    }
    
    /**
     * <support>
     * 		<PluginTypeName>Real plugin name</PluginTypeName>//exmaple <PluginUser>UserNd91</PluginUser>
     * 																   <PluginIAP>IAPNd91</PluginIAP>
     * </support>
     * 
     * 
     * @param pluginConfigureFile
     * @return
     */
    public static Hashtable<String, String> getPluginConfigFromFile(String pluginConfigureFile) {
    	Hashtable<String, String> ht = new Hashtable<String, String>();

		Context context = PluginWrapper.getContext();
		
		try {
			InputStreamReader inStreamReader = new InputStreamReader(context.getAssets().open(pluginConfigureFile));
			BufferedReader buffReader = new BufferedReader(inStreamReader);
			StringBuilder data = new StringBuilder();

			String str = null;

			while ((str = buffReader.readLine()) != null) {
				data.append(str);
			}
			
//			inStreamReader.close();
			buffReader.close();

			str = data.toString();

			XmlPullParser xmlParser = Xml.newPullParser();
			xmlParser.setInput(new StringReader(str));
			int eventType = xmlParser.getEventType();
			String pluginType="";
			

			while (eventType != XmlPullParser.END_DOCUMENT) {
				switch (eventType) {
//					case XmlPullParser.START_DOCUMENT:
//						break;
					case XmlPullParser.START_TAG:
						pluginType=xmlParser.getName();
						ht.put(pluginType, xmlParser.nextText());
						break;
//					case XmlPullParser.END_TAG:
//						break;
//					case XmlPullParser.TEXT:
//						
//						break;
				}
				
				eventType=xmlParser.next();
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
		return ht;
	}
}