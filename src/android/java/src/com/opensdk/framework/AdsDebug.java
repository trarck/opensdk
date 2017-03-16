package com.opensdk.framework;

import java.util.Hashtable;
import java.util.Iterator;

import org.json.JSONObject;

import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.PopupWindow;

public class AdsDebug implements InterfaceAds {
	private static Context mContext = null;
	protected static String TAG = "AdsDebug";
	private static AdsDebug mAdapter = null;
	private static boolean isDebug = true;
	private static PopupWindow popupWindow = null;
	private ImageView imageView = null;

	protected static void LogE(String paramString, Exception paramException) {
		Log.e(TAG, paramString, paramException);
		paramException.printStackTrace();
	}

	protected static void LogD(String paramString) {
		if (isDebug)
			Log.d(TAG, paramString);
	}

	public AdsDebug(Context paramContext) {
		mContext = paramContext;
		mAdapter = this;
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> devInfo) {
		String data="";
		String key=null;
		
		for(Iterator<String> it=devInfo.keySet().iterator();it.hasNext();){
			key=it.next();
			data+=key+"="+devInfo.get(key)+"\n";
		}
		LogD("configDeveloperInfo \n" + data + "invoked!");
	}

	@Override
	public void showAds(Hashtable<String, String> adsInfo, int pos) {
		String data="";
		String key=null;
		
		for(Iterator<String> it=adsInfo.keySet().iterator();it.hasNext();){
			key=it.next();
			data+=key+"="+adsInfo.get(key)+"\n";
		}
		LogD("showAds \n" + data + "invoked!");
	}

	@Override
	public void hideAds(Hashtable<String, String> adsInfo) {
		String data="";
		String key=null;
		
		for(Iterator<String> it=adsInfo.keySet().iterator();it.hasNext();){
			key=it.next();
			data+=key+"="+adsInfo.get(key)+"\n";
		}
		LogD("hideAds \n" + data + "invoked!");
	}

	public void showAds(JSONObject info) {
		LogD("showAds " + info.toString() + "invoked!");
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				if (popupWindow != null) {
					popupWindow.dismiss();
					popupWindow = null;
				}
				Bitmap bitmap = BitmapFactory.decodeResource(
						mContext.getResources(),
						getResourceId("ui_ad", "drawable"));
				View view = LayoutInflater.from(mContext).inflate(
						getResourceId("plugin_ads", "layout"), null);
				popupWindow = new PopupWindow(view, bitmap.getWidth(),
						bitmap.getHeight());
				popupWindow.showAtLocation(view, 17, 0, 0);

				ImageButton button = ((ImageButton) view.findViewById(AdsDebug
						.getResourceId("image_close", "id")));

				button.setOnClickListener(new android.view.View.OnClickListener() {
					public void onClick(View view1) {
						if (popupWindow != null) {
							popupWindow.dismiss();
							popupWindow = null;
						}
					}
				});
			}
		});
	}

	public void hideAds(JSONObject info) {
		LogD("hideAds " + info.toString() + "invoked!");
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				if (popupWindow != null) {
					popupWindow.dismiss();
					popupWindow = null;
				}
			}
		});
	}

	public void preloadAds(JSONObject info) {
		LogD("debug not support preloadAds");
	}

	public float queryPoints() {
		LogD("debug not support query points");
		return 0.0F;
	}

	public void spendPoints(int paramInt) {
		LogD("debug not support spend points");
	}

	public void setDebugMode(boolean paramBoolean) {
		isDebug = paramBoolean;
	}

	public String getSDKVersion() {
		return "2.0.3";
	}

	public String getPluginVersion() {
		return "2.0.3";
	}

	public static void showDialog(String title, String msg) {
		final String curTitle = title;
		final String curMsg = msg;
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				Log.d("d", "d");
				Builder dialogBuilder = new android.app.AlertDialog.Builder(
						mContext);

				dialogBuilder.setTitle(getResourceId(curTitle,
						"string"));
				dialogBuilder.setMessage(getResourceId(curMsg,
						"string"));
				dialogBuilder.setPositiveButton("Ok", new OnClickListener() {
					public void onClick(DialogInterface dialoginterface, int i) {

					}
				});

				dialogBuilder.create();
				dialogBuilder.show();

			}
		});
	}

	public static int getResourceId(String key, String type) {
		return mContext.getResources().getIdentifier(key,
				type, mContext.getPackageName());
	}

	public boolean isAdTypeSupported(int adType) {
		return true;
	}

}