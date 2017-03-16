package com.opensdk.framework;


public interface ILoginCallback
{

    public abstract void onSuccessed(int code, String msg);

    public abstract void onFailed(int code, String msg);
}
