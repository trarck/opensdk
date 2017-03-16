package com.opensdk.framework;

import android.content.Context;
import android.util.Log;
import java.util.ArrayList;

public class PushDebug
  implements InterfacePush
{
  private static Context mContext = null;
  protected static String TAG = "PushDebug";
  private static PushDebug mAdapter = null;
  private static boolean isDebug = true;

  protected static void LogE(String paramString, Exception paramException)
  {
    Log.e(TAG, paramString, paramException);
    paramException.printStackTrace();
  }

  protected static void LogD(String msg)
  {
    if (isDebug)
      Log.d(TAG, msg);
  }

  public PushDebug(Context context)
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

  public void startPush()
  {
    LogD("startPush() invoked!");
  }

  public void closePush()
  {
    LogD("closePush() invoked!");
  }

  public void setAlias(String alias)
  {
    LogD("setAlias(String alias) invoked!");
  }

  public void delAlias(String alias)
  {
    LogD("delAlias(String alias) invoked!");
  }

  public void setTags(ArrayList<String> paramArrayList)
  {
    LogD("setTags(List<String> tags) invoked!");
  }

  public void delTags(ArrayList<String> tags)
  {
    LogD("delTags(List<String> tags) invoked!");
  }
}