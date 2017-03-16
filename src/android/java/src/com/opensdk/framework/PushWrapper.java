package com.opensdk.framework;

public class PushWrapper {
	public static final int ACTION_RET_RECEIVEMESSAGE = 0;

	public static void onActionResult(InterfacePush obj, int ret, String msg) {
		final int curRet = ret;
		final String curMsg = msg;
		final InterfacePush curAdapter = obj;
		PluginWrapper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				String name = curAdapter.getClass().getName();
				name = name.replace('.', '/');
				nativeOnActionResult(name, curRet, curMsg);
			}
		});
	}

	private static native void nativeOnActionResult(String className, int ret,
			String msg);
}