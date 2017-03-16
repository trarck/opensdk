package com.opensdk.framework;

public class ShareWrapper {
	public static final int SHARERESULT_SUCCESS = 0;
	public static final int SHARERESULT_FAIL    = 1;
	public static final int SHARERESULT_CANCEL  = 2;
	public static final int SHARERESULT_TIMEOUT = 3;

	public static void onShareResult(InterfaceShare obj, int ret, String msg) {
		final int curRet = ret;
		final String curMsg = msg;
		final InterfaceShare curAdapter = obj;
		PluginWrapper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				String name = curAdapter.getClass().getName();
				name = name.replace('.', '/');
				nativeOnShareResult(name, curRet, curMsg);
			}
		});
	}
	private static native void nativeOnShareResult(String className, int ret, String msg);
}