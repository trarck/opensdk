package com.opensdk.framework;

import java.util.Hashtable;

import com.opensdk.utils.SdkHttpListener;
import com.opensdk.utils.Util;

import android.content.Context;

public class UserWrapper {
	public static final int ACTION_RET_INIT_SUCCESS = 0;
	public static final int ACTION_RET_INIT_FAIL = 1;
	public static final int ACTION_RET_LOGIN_SUCCESS = 2;
	public static final int ACTION_RET_LOGIN_TIMEOUT = 3;
	public static final int ACTION_RET_LOGIN_NO_NEED = 4;
	public static final int ACTION_RET_LOGIN_FAIL = 5;
	public static final int ACTION_RET_LOGIN_CANCEL = 6;
	public static final int ACTION_RET_LOGOUT_SUCCESS = 7;
	public static final int ACTION_RET_LOGOUT_FAIL = 8;
	public static final int ACTION_RET_PLATFORM_ENTER = 9;
	public static final int ACTION_RET_PLATFORM_BACK = 10;
	public static final int ACTION_RET_PAUSE_PAGE = 11;
	public static final int ACTION_RET_EXIT_PAGE = 12;
	public static final int ACTION_RET_ANTIADDICTIONQUERY = 13;
	public static final int ACTION_RET_REALNAMEREGISTER = 14;
	public static final int ACTION_RET_ACCOUNTSWITCH_SUCCESS = 15;
	public static final int ACTION_RET_ACCOUNTSWITCH_FAIL = 16;
	public static final int ACTION_RET_OPENSHOP = 17;

	public static void onActionResult(InterfaceUser obj, int ret, String msg) {
		final int curRet = ret;
		final String curMsg = msg;
		final InterfaceUser curAdapter = obj;
		PluginWrapper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				String name = curAdapter.getClass().getName();
				name = name.replace('.', '/');
				nativeOnActionResult(name, curRet, curMsg);
			}
		});
	}

	public static void getAccessToken(Context context,
			Hashtable<String, String> params,SdkHttpListener curListener) {
			Util.getAccessToken(context, params, curListener);
	}

	private static native void nativeOnActionResult(String className, int ret,
			String msg);

	public static native String getLoginServerID();

	public static native String getLoginServerIP();
}
