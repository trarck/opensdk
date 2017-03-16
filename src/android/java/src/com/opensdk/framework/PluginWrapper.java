package com.opensdk.framework;

import java.lang.reflect.Field;
import java.util.Vector;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.util.Log;


public class PluginWrapper {

    protected static Context sContext = null;
    protected static GLSurfaceView sGLSurfaceView = null; 
    protected static Handler sMainThreadHandler = null;
    protected static Handler sGLThreadHandler = null;
    private static final String TAG = "PluginWrapper";
    private static Vector<IActivityCallback> mActivityCallback = new Vector<IActivityCallback>();
    
    public static void init(Context context) {
        sContext = context;
        if (null == sMainThreadHandler) {
            sMainThreadHandler = new Handler();
        }
        
        Wrapper.analysisDeveloperInfo(context);
    }

    public static void setGLSurfaceView(GLSurfaceView value) {
        sGLSurfaceView = value;
    }
    
    protected static void initFromNativeActivity(Activity act) {
        sContext = act;
        // @warning These lines will cause crash.
//        if (null == sGLThreadHandler) {
//            sGLThreadHandler = new Handler();
//        }
    }
    
    public static boolean onActivityResult(int requestCode, int resultCode, Intent data) {
    	boolean result = true;
    	
        for (IActivityCallback listener : mActivityCallback) {
            result = result && listener.onActivityResult(requestCode, resultCode, data);
        }
        
        return result;
    }
    
    public static void onResume() {
    	for (IActivityCallback listener : mActivityCallback) {
    		listener.onResume();
    	}
    }
    
    public static void onPause() {
    	for (IActivityCallback listener : mActivityCallback) {
    		listener.onPause();
    	}
    }
    
    public static void onDestroy() {
    	for (IActivityCallback listener : mActivityCallback) {
    		listener.onDestroy();
    	}
    }
    
    public static void onNewIntent(Intent intent)
    {
    	for (IActivityCallback listener : mActivityCallback) {
    		listener.onNewIntent(intent);
    	}
    }

    public static void onStop()
    {
    	for (IActivityCallback listener : mActivityCallback) {
    		listener.onStop();
    	}
    }


    public static void onRestart()
    {
    	for (IActivityCallback listener : mActivityCallback) {
    		listener.onRestart();
    	}
    }
    
    public static void addActivityCallback(IActivityCallback listener) {
    	mActivityCallback.add(listener);
    }
    
    public static void removeListener(IActivityCallback listener) {
    	mActivityCallback.remove(listener);
    }
    
    protected static Object initPlugin(String classFullName) {
        Log.i(TAG, "class name : ----" + classFullName + "----");
        Class<?> c = null;
        try {
            String fullName = classFullName.replace('/', '.');
            c = Class.forName(fullName);
        } catch (ClassNotFoundException e) {  
            Log.e(TAG, "Class " + classFullName + " not found.");
            e.printStackTrace();
            return null;
        }

        try {
            Context ctx = getContext();
            if (ctx != null) {
                Object o = c.getDeclaredConstructor(Context.class).newInstance(ctx);
                return o;
            } else {
                Log.e(TAG, "Plugin " + classFullName + " wasn't initialized.");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    protected static int getPluginType(Object obj) {
        int nRet = -1;
        try
        {
            Field filedID = obj.getClass().getField("PluginType");
            Integer nObj = (Integer) filedID.get(obj);
            nRet = nObj.intValue();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return nRet;
    }

    public static Context getContext() {
        return sContext;
    }
    
    public static void runOnGLThread(Runnable r) {
        if (null != sGLSurfaceView) {
            sGLSurfaceView.queueEvent(r);
        } else
        if (null != sGLThreadHandler) {
            sGLThreadHandler.post(r);
        } else {
            Log.i(TAG, "call back invoked on main thread");
            r.run();
        }
    }

    public static void runOnMainThread(Runnable r) {
        if (null != sMainThreadHandler) {
            sMainThreadHandler.post(r);
        } else
        if (null != sContext && sContext instanceof Activity) {
            Activity act = (Activity) sContext;
            act.runOnUiThread(r);
        }
    }
}
