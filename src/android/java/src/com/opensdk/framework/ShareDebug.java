package com.opensdk.framework;

import java.util.Hashtable;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;
import android.widget.Toast;

public class ShareDebug implements InterfaceShare {
	private static Context mContext = null;
	protected static String TAG = "ShareDebug";
	private static ShareDebug mAdapter = null;
	private static boolean isDebug = true;

	protected static void LogE(String msg, Exception exception) {
		Log.e(TAG, msg, exception);
		exception.printStackTrace();
	}

	protected static void LogD(String msg) {
		if (isDebug)
			Log.d(TAG, msg);
	}

	public ShareDebug(Context context) {
		mContext = context;
		mAdapter = this;
	}

	public void configDeveloperInfo(Hashtable<String, String> cpInfo) {

	}

	public void share(final Hashtable<String, String> cpInfo) {
		LogD((new StringBuilder("share invoked ")).append(cpInfo.toString())
				.toString());
		if (!networkReachable()) {
			shareResult(ShareWrapper.SHARERESULT_FAIL, "Network error!");
			return;
		} else {
			PluginWrapper.runOnMainThread(new Runnable() {
				public void run() {
					Toast.makeText(ShareDebug.mContext, "success to share",
							Toast.LENGTH_SHORT).show();
					ShareDebug.shareResult(ShareWrapper.SHARERESULT_SUCCESS,cpInfo.get("SharedText"));
				}
			});
			return;
		}
	}

	private boolean networkReachable() {
		boolean flag = false;
		NetworkInfo networkinfo;
		try {
			flag = (networkinfo = ((ConnectivityManager) mContext
					.getSystemService("connectivity")).getActiveNetworkInfo()) != null ? networkinfo
					.isAvailable() : false;
		} catch (Exception exception) {
			LogE("Fail to check network status", exception);
		}
		LogD((new StringBuilder("NetWork reachable : ")).append(flag)
				.toString());
		return flag;
	}

	public static void shareResult(int result, String msg) {
		ShareWrapper.onShareResult(mAdapter, result, msg);
		LogD((new StringBuilder("shareResult result : ")).append(result)
				.append(" msg : ").append(msg).toString());
	}

	public void setDebugMode(boolean flag) {
		isDebug = flag;
	}

	public String getSDKVersion() {
		return "2.0.3";
	}

	public String getPluginVersion() {
		return "2.0.3";
	}

}
