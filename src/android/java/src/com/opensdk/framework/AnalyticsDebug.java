package com.opensdk.framework;

import java.lang.reflect.Method;
import java.util.Hashtable;

import org.json.JSONObject;

import android.content.Context;
import android.util.Log;

public class AnalyticsDebug implements InterfaceAnalytics
{
    private static Context mContext = null;
    protected static String TAG = "AnalyticsDebug";
    private static AnalyticsDebug mAdapter = null;
    private static boolean isDebug = true;

    protected static void LogE(String msg, Exception exception)
    {
        Log.e(TAG, msg, exception);
        exception.printStackTrace();
    }

    protected static void LogD(String msg)
    {
        if(isDebug)
            Log.d(TAG, msg);
    }

    public AnalyticsDebug(Context context)
    {
        mContext = context;
        mAdapter = this;
    }

    public void setDebugMode(boolean isDebugMode)
    {
        isDebug = isDebugMode;
    }

    public String getSDKVersion()
    {
        return "2.0.3";
    }

    public String getPluginVersion()
    {
        return "2.0.3";
    }

    public void startSession()
    {
        LogD("startSession invoked!");
    }

    public void stopSession()
    {
        LogD("stopSession invoked!");
    }

    public void setSessionContinueMillis(int millis)
    {
        LogD("setSessionContinueMillis invoked!");
    }

    public void setCaptureUncaughtException(boolean isEnabled)
    {
        LogD("setCaptureUncaughtException(List<String> tags) invoked!");
    }

    public void logError(String errorId, String message)
    {
        LogD("logError(String errorId, String message) invoked!");
    }

    public void logEvent(String eventId)
    {
        LogD("logEvent(String eventId) invoked!");
    }

    public void logEvent(String eventId, Hashtable<String, String> paramMap)
    {
        LogD("logEvent(String eventId, Hashtable<String, String> paramMap) invoked!");
    }

    public void logTimedEventBegin(String eventId)
    {
        LogD("logTimedEventBegin(String eventId) invoked!");
    }

    public void logTimedEventEnd(String eventId)
    {
        LogD("logTimedEventEnd(String eventId) invoked!");
    }

    public boolean isFunctionSupported(String funName)
    {
        Method methods[] = AnalyticsDebug.class.getMethods();
        for(int i = 0; i < methods.length; i++)
            if(methods[i].getName().equals(funName))
                return true;

        return false;
    }

    public void setAccount(JSONObject jsonobject)
    {
        LogD((new StringBuilder("setAccount  ")).append(jsonobject.toString()).append("invoked!").toString());
    }

    public void onChargeRequest(JSONObject jsonobject)
    {
        LogD((new StringBuilder("onChargeRequest  ")).append(jsonobject.toString()).append("invoked!").toString());
    }

    public void onChargeOnlySuccess(JSONObject jsonobject)
    {
        LogD((new StringBuilder("onChargeOnlySuccess  ")).append(jsonobject.toString()).append("invoked!").toString());
    }

    public void onChargeSuccess(String msg)
    {
        LogD((new StringBuilder("onChargeSuccess(")).append(msg.toString()).append(")invoked!").toString());
    }

    public void onChargeFail(JSONObject jsonobject)
    {
        LogD((new StringBuilder("onChargeFail(")).append(jsonobject.toString()).append(")invoked!").toString());
    }

    public void onPurchase(JSONObject jsonobject)
    {
        LogD((new StringBuilder("onPurchase(")).append(jsonobject.toString()).append(")invoked!").toString());
    }

    public void onUse(JSONObject jsonobject)
    {
        LogD((new StringBuilder("onUse(")).append(jsonobject.toString()).append(")invoked!").toString());
    }

    public void onReward(JSONObject jsonobject)
    {
        LogD((new StringBuilder("onReward(")).append(jsonobject.toString()).append(")invoked!").toString());
    }

    public void startLevel(JSONObject jsonobject)
    {
        LogD((new StringBuilder("startLevel(")).append(jsonobject.toString()).append(")invoked!").toString());
    }

    public void finishLevel(String msg)
    {
        LogD((new StringBuilder("finishLevel(")).append(msg.toString()).append(")invoked!").toString());
    }

    public void failLevel(JSONObject jsonobject)
    {
        LogD((new StringBuilder("failLevel(")).append(jsonobject.toString()).append(")invoked!").toString());
    }

    public void startTask(JSONObject jsonobject)
    {
        LogD((new StringBuilder("startTask(")).append(jsonobject.toString()).append(")invoked!").toString());
    }

    public void finishTask(String msg)
    {
        LogD((new StringBuilder("finishTask(")).append(msg.toString()).append(")invoked!").toString());
    }

    public void failTask(JSONObject jsonobject)
    {
        LogD((new StringBuilder("failTask(")).append(jsonobject.toString()).append(")invoked!").toString());
    }
}
