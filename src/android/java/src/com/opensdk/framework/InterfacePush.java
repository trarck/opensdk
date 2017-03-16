package com.opensdk.framework;

import java.util.ArrayList;

public abstract interface InterfacePush
{
  public static final int PluginType = 7;

  public void startPush();

  public void closePush();

  public void setAlias(String alias);

  public void delAlias(String alias);

  public void setTags(ArrayList<String> tags);

  public void delTags(ArrayList<String> tags);

  public void setDebugMode(boolean value);

  public String getSDKVersion();

  public String getPluginVersion();
}