package com.opensdk.framework;

import java.util.Hashtable;

import com.opensdk.utils.SdkHttpListener;
import com.opensdk.utils.Util;

import android.content.Context;

public class IAPWrapper {
	public static final int PAYRESULT_SUCCESS = 0;
	public static final int PAYRESULT_FAIL = 1;
	public static final int PAYRESULT_CANCEL = 2;
	public static final int PAYRESULT_NETWORK_ERROR = 3;
	public static final int PAYRESULT_PRODUCTIONINFOR_INCOMPLETE = 4;
	public static final int PAYRESULT_INIT_SUCCESS = 5;
	public static final int PAYRESULT_INIT_FAIL = 6;
	public static final int PAYRESULT_NOW_PAYING = 7;
	public static final int PAYRESULT_RECHARGE_SUCCESS = 8;

	public static void onPayResult(InterfaceIAP obj, int ret, String msg) {
		final int curRet = ret;
		final String curMsg = msg;
		final InterfaceIAP curObj = obj;
		PluginWrapper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				String name = curObj.getClass().getName();
				name = name.replace('.', '/');
				nativeOnPayResult(name, curRet, curMsg);
			}
		});
	}

	public static void getPayOrderId(Context context,
			Hashtable<String, String> params , SdkHttpListener curListener)
	{
		Util.getPayOrderId(context, params, curListener);
	}

	private static native void nativeOnPayResult(String className, int ret,
			String msg);
}
