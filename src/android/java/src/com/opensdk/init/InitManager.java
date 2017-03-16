package com.opensdk.init;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import android.app.Application;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.util.Log;

import com.opensdk.init.SpecialInitHelper;
import com.opensdk.init.SpecialInitInfo;

public class InitManager {
	private static final String TAG = InitManager.class.getSimpleName();
	private static InitManager _instance;
	
	private static final String LIB_META_KEY = "OPENSDK_LOADLIBS";
	private static final String LIB_LOAD_FILE = "loadlibs.dat";
	private static final String SPECAIL_INIT_META_KEY = "OPENSDK_SPECAIL_INIT";
	private static final String SPECAIL_INIT_FILE = "specialinit.dat";
	
	private static final int LIB_CONFIG_TYPE_META = 1; 
	private static final int LIB_CONFIG_TYPE_FILE = 2; 
	
	private int _initConfigType=LIB_CONFIG_TYPE_META;
	
	public static InitManager getInstance() {
		if (_instance == null) {
			_instance = new InitManager();
		}
		return _instance;
	}
	
	/**
	 * 应用的最开始初始化
	 * 通常中在Application的onCreate中调用
	 * @param content
	 */
	public void initApp(Context content){
		Log.d(TAG,"init on Application create");
		//加载动态库.可以加载多个
		loadLibrary(content);
		
		//通常只加载动态库就行了，但有的sdk需要在Application的onCreate中执行初始化操作。
		//如果没有特殊需求，这部分加载可以放在Active的onCreate那里初始化。
		initSpecials(content);
	}
	
	/**
	 * 初始化插件
	 * 在Activity的onCreate中调用
	 */
	public void initPlugins(){
		//通知c++初始化插件
		nativeInitPlugins();
	}
	
	/**
	 * 加载动态连接库
	 * @param content
	 */
	public void loadLibrary(Context content){
		Log.d(TAG,"begin load library");
		
		String[] libs=getNeedLoadLibraries(content);
		
		if(libs!=null){
			try{
				for(int i=0;i<libs.length;++i){
					System.loadLibrary(libs[i]);
				}
			}catch(UnsatisfiedLinkError e){
				e.printStackTrace();
			}
		}
		
		Log.d(TAG,"end load library");
	}
	
	/**
	 * 一些特殊sdk的初始化
	 */
	public void initSpecials(Context content){
		Log.d(TAG,"begin special init");
		//检查是不是在Application中调用，如果不是则忽略。
		if(Application.class.isInstance(content)){
			
			List<SpecialInitInfo> infos=getSpecialInitInfo(content);
			//执行初始化
			if(null!=infos && !infos.isEmpty()){
				for(SpecialInitInfo info:infos){
					info.invoke(content);
				}
			}
		}
		Log.d(TAG,"end special init");
	}

	/**
	 * 取得动态连接库的加载方法
	 * @return
	 */
	public int getInitConfigType() {
		return _initConfigType;
	}

	/**
	 * 设置动态连接库的加载方法
	 * @param libConfigType
	 */
	public void setInitConfigType(int initConfigType) {
		this._initConfigType = initConfigType;
	}
	
	
	/**
	 * 取得需要加载的动态连接库
	 * @param content
	 * @return
	 */
	private String[] getNeedLoadLibraries(Context content){
		if(_initConfigType==LIB_CONFIG_TYPE_META){
			return getNeedLoadLibrariesFromMeta(content);
		}else if(_initConfigType==LIB_CONFIG_TYPE_FILE){
			return getNeedLoadLibrariesFromFile(content);
		}
		
		return null;
	}
	

	/**
	 * 从文件中取得需要加载的动态连接库
	 * @param content
	 * @return
	 */
	private static String[] getNeedLoadLibrariesFromFile(Context content){
		
	    try
	    {
	    	InputStream datIn=content.getAssets().open(LIB_LOAD_FILE);
			
	    	if(null!=datIn){
				int  c= 0;
			    ByteArrayOutputStream localByteArrayOutputStream = new ByteArrayOutputStream();
			    
			    while ((c = datIn.read()) != -1)
			    	localByteArrayOutputStream.write(c);
			    String  libStr= localByteArrayOutputStream.toString();
			    
			    datIn.close();
			    localByteArrayOutputStream.close();
			    
				return libStr.split(",");
	    	}
			
	    }
	    catch (IOException ex)
	    {
	      Log.e(TAG,"error: " + ex);
	    }
	    
	    return null;
	}
	
	/**
	 * 从Manifest的配置中取得需要加载的动态连接库
	 * @param content
	 * @return
	 */
	private static String[] getNeedLoadLibrariesFromMeta(Context content){
		
		ApplicationInfo ai = null;
		try {
			ai = content.getPackageManager().getApplicationInfo(content.getPackageName(), PackageManager.GET_META_DATA);
			Bundle bundle = ai.metaData;
			if(null!=bundle){
				String libs=bundle.getString(LIB_META_KEY);
				if(null!=libs && !"".equals(libs)){
					return libs.split(",");
				}
			}
			
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		return null;
	}
	
	private List<SpecialInitInfo> getSpecialInitInfo(Context content){
		
		String infoStr=_initConfigType==LIB_CONFIG_TYPE_META?getSpecialInitInfoFromMeta(content):getSpecialInitInfoFromMeta(content);
		if(null!=infoStr){
			return SpecialInitHelper.getInstance().getSpecialInitData(infoStr);
		}
		
		return null;
	}
	
	private static String getSpecialInitInfoFromFile(Context content){
		
		try
	    {
	    	InputStream datIn=content.getAssets().open(SPECAIL_INIT_FILE);
			
	    	if(null!=datIn){
				int  c= 0;
			    ByteArrayOutputStream localByteArrayOutputStream = new ByteArrayOutputStream();
			    
			    while ((c = datIn.read()) != -1)
			    	localByteArrayOutputStream.write(c);
			    String  infoStr= localByteArrayOutputStream.toString();
			    
			    datIn.close();
			    localByteArrayOutputStream.close();
			    
				return infoStr;
	    	}
	    }
	    catch (IOException ex)
	    {
	      Log.e(TAG,"error: " + ex);
	    }
	    
	    return null;
	}
	
	private static String getSpecialInitInfoFromMeta(Context content){
		
		ApplicationInfo ai = null;
		try {
			ai = content.getPackageManager().getApplicationInfo(content.getPackageName(), PackageManager.GET_META_DATA);
			Bundle bundle = ai.metaData;
			
			return bundle!=null ?bundle.getString(SPECAIL_INIT_META_KEY):null;
			
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		return null;
	}
	
	private static native void nativeInitPlugins();
}
