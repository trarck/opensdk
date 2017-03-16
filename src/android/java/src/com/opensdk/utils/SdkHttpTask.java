package com.opensdk.utils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;

import android.app.ProgressDialog;
import android.content.Context;

import com.opensdk.framework.PluginWrapper;

public class SdkHttpTask {
	private static final String TAG = "SdkHttpTask";
	private static ProgressDialog mProgress = null;
	private Context mContext;
	private boolean mIsHttpPost;
	private List<NameValuePair> mKeyValueArray;
	private SdkHttpListener mListener;
	private int mTimeOut;

	public SdkHttpTask(Context paramContext) {
		this.mContext = paramContext;
	}

	public static void closeProgress() {
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				try {
					if (SdkHttpTask.mProgress != null) {
						SdkHttpTask.mProgress.dismiss();
						SdkHttpTask.mProgress = null;
					}
					return;
				} catch (Exception localException) {
					localException.printStackTrace();
				}
			}
		});
	}

	private static String convertStreamToString(InputStream inputStream) {
		BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
		StringBuilder stringBuilder = new StringBuilder();
		String str = null;
		try {
			while ((str = bufferedReader.readLine()) != null)
				stringBuilder.append(str);
		} catch (IOException e) {
		} finally {
			try {
				inputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return stringBuilder.toString();
	}

	private void executeHttp(final Context contex,final String url) {
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				if (SdkHttpTask.mProgress == null)
					SdkHttpTask.mProgress = SdkHttpTask.showProgress(contex, "请稍等", "数据通信中...", false, false);
			}
		});
		
		new Thread() {
			public void run() {
				try {
					HttpResponse httpResponse=null;
					
					if (mIsHttpPost){
						httpResponse = HttpUtils.post(contex, url,mKeyValueArray,mTimeOut);
					}else{
						httpResponse = HttpUtils.get(contex, url,SdkHttpTask.this.mTimeOut);
					}
					
					closeProgress();
					
					if (httpResponse == null){
						mListener.onError();
					}else{
						if(httpResponse.getStatusLine().getStatusCode()!=200){
							mListener.onError();
						}else{
							HttpEntity httpEntity = httpResponse.getEntity();
							
							if (httpEntity != null){
								InputStream inputStream = httpEntity.getContent();
								
								if (inputStream != null) {
									String responseStr = SdkHttpTask.convertStreamToString(inputStream);
									mListener.onResponse(responseStr);
								}
							}
						}
					}
				} catch (Exception e) {
					closeProgress();
					mListener.onError();
					e.printStackTrace();
				}
			}
		}.start();
	}

	public static ProgressDialog showProgress(Context context,CharSequence title, CharSequence message,boolean indeterminate, boolean flag) {
		ProgressDialog progressDialog = new ProgressDialog(context);
		progressDialog.setTitle(title);
		progressDialog.setMessage(message);
		progressDialog.setIndeterminate(indeterminate);
		progressDialog.setCancelable(flag);
		progressDialog.show();
		return progressDialog;
	}

	public void doGet(SdkHttpListener listener, String url,int timeOut) {
		this.mListener = listener;
		this.mIsHttpPost = false;
		this.mTimeOut = timeOut;
		executeHttp(this.mContext, url);
	}

	public void doPost(SdkHttpListener listener,List<NameValuePair> data, String url, int timeOut) {
		this.mListener = listener;
		this.mIsHttpPost = true;
		this.mKeyValueArray = data;
		this.mTimeOut = timeOut;
		executeHttp(this.mContext, url);
	}
}