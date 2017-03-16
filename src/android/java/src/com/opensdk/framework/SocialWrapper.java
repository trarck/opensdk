package com.opensdk.framework;

public class SocialWrapper {
	  public static final int SOCIAL_SUBMITSCORE_SUCCEED = 1;
	  public static final int SOCIAL_SUBMITSCORE_FAIL = 2;
	  public static final int SOCIAL_UNLOCKACH_SUCCEED = 3;
	  public static final int SOCIAL_UNLOCKACH_FAIL = 4;
	  public static final int SOCIAL_SIGNIN_SUCCEED = 5;
	  public static final int SOCIAL_SIGNIN_FAIL = 6;
	  public static final int SOCIAL_SIGNOUT_SUCCEED = 7;
	  public static final int SOCIAL_SIGNOUT_FAIL = 8;

    public static void onSocialResult(InterfaceSocial obj, int ret, String msg) {
        final int curRet = ret;
        final String curMsg = msg;
        final InterfaceSocial curAdapter = obj;
        PluginWrapper.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                String name = curAdapter.getClass().getName();
                name = name.replace('.', '/');
                nativeOnSocialResult(name, curRet, curMsg);
            }
        });
    }
    private static native void nativeOnSocialResult(String className, int ret, String msg);
}
