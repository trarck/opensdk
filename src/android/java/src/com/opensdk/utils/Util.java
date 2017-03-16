package com.opensdk.utils;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Hashtable;
import java.util.Iterator;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONObject;

import android.content.Context;
import android.util.Base64;
import android.util.Log;

import com.opensdk.framework.UserWrapper;
import com.opensdk.framework.Wrapper;

public class Util {
	private static SdkHttpTask sSdkHttpTask = null;

	public static Hashtable<String, String> addData(Context context,Hashtable<String, String> data) {
		data.put("imei", Device.getIMEI(context));
		data.put("ost", Device.getOst(context));
		data.put("gps", Device.getGPS(context));
		data.put("dt", Device.getDt());
		data.put("osv", Device.getOsv());
		data.put("jb", Device.getJb());
		data.put("sr", Device.getSr(context));
		data.put("ori", Device.getOri(context));
		data.put("net", Device.getNet(context));
		data.put("appv", Device.getAppv(context));
		data.put("os", Device.getDevice());
		data.put("cc", Device.getCc());
		data.put("lang", Device.getLang());
		data.put("sdk", Device.getSDK());
		data.put("ckid", Device.getCkid(context));
		data.put("wmac", Device.getMACAddress(context));
		data.put("isp", Device.getIsp(context));
		String imsi;
		String phoneNum;
		String ip;
		if (Device.getIMSI(context) == null)
			imsi = "";
		else
			imsi =Device.getIMSI(context);

		data.put("imsi", imsi);

		if (Device.getPhoneNum(context) == null)
			phoneNum = "";
		else
			phoneNum = Device.getPhoneNum(context);
		data.put("phonenum", phoneNum);

		if (Device.getIP() == null)
			ip = "";
		else
			ip = Device.getIP();
		data.put("ip", ip);
		return data;
	}

	public static String ckDecode(String str) {
		if (str == null)
			return null;
		byte bytes[] = URLDecoder.decode(str).getBytes();
		byte left;
		if (bytes.length % 2 == 0)
			left = 5;
		else
			left = 4;
		for (int i = 0; i < bytes.length - left; i += 2) {
			byte tmp = bytes[i];
			bytes[i] = bytes[i + 1];
			bytes[i + 1] = tmp;
		}

		// byte binaryBytes[]=Base64.decode(bytes, 0);
		return new String(bytes);
	}

	public static String ckEncode(String str) {
		byte bytes[] = str.getBytes();
		byte left;
		if (bytes.length % 2 == 0)
			left = 5;
		else
			left = 4;

		for (int i = 0; i < bytes.length - left; i += 2) {
			byte tmp = bytes[i];
			bytes[i] = bytes[i + 1];
			bytes[i + 1] = tmp;
		}

		return URLEncoder.encode(new String(bytes));
	}

	public static void getAccessToken(Context context,Hashtable<String, String> param, final SdkHttpListener curListener) {
		try {
			String serverUrl = param.get("server_url");
			param.remove("server_url");
			param.put("private_key",Wrapper.getDeveloperInfo().get("privateKey"));
			param.put("server_id", UserWrapper.getLoginServerID());
			ArrayList<NameValuePair> arrayList = new ArrayList<NameValuePair>();
			Iterator<String> iterator = param.keySet().iterator();
			while (iterator.hasNext()) {
				String key = iterator.next();
				arrayList.add(new BasicNameValuePair(key, param.get(key)));
			}

			sSdkHttpTask = new SdkHttpTask(context);
			sSdkHttpTask.doPost(new SdkHttpListener() {
				public void onError() {
					curListener.onError();
					sSdkHttpTask = null;
				}

				public void onResponse(String responseText) {
					curListener.onResponse(responseText);
					sSdkHttpTask = null;
				}
			}, arrayList, serverUrl, 60000);

		} catch (Exception e) {
			e.printStackTrace();
			curListener.onError();
		}
	}
	
	public static void getPayOrderId(Context context,Hashtable<String, String> params, final SdkHttpListener curListener) {
		try {
			params.remove("uapi_secret");
			String jsonValue = Base64.encodeToString(new JSONObject(params).toString().getBytes(), 2);
			String[] sortKeys = new String[params.size()];
			Object[] keys = params.keySet().toArray();

			for (int i = 0; i < keys.length; ++i) {
				sortKeys[i] = keys[i].toString();
			}
			Arrays.sort(sortKeys);
			StringBuffer sortedValueStr = new StringBuffer();
			for (int j = 0; j < sortKeys.length; ++j) {
				sortedValueStr.append(params.get(sortKeys[j]));
			}

			String token = getMd5(sortedValueStr.toString() + Wrapper.getDeveloperInfo().get("privateKey"));
			ArrayList<NameValuePair> postData = new ArrayList<NameValuePair>();
			postData.add(new BasicNameValuePair("token", token));
			postData.add(new BasicNameValuePair("json_data", jsonValue));
			postData.add(new BasicNameValuePair("flag", "custom"));
			sSdkHttpTask = new SdkHttpTask(context);
			if (Wrapper.getDeveloperInfo().containsKey("order_url") && Wrapper.getDeveloperInfo().get("order_url").length() != 0)
				Config.SERVER_URL_GET_ORDER = Wrapper.getDeveloperInfo().get("order_url");
			Log.d("SERVER_URL_GET_ORDER", Config.SERVER_URL_GET_ORDER);
			sSdkHttpTask.doPost(new SdkHttpListener() {
				public void onError() {
					curListener.onError();
					sSdkHttpTask = null;
				}

				public void onResponse(String responseText) {
					curListener.onResponse(responseText);
					sSdkHttpTask = null;
				}
			}, postData, Config.SERVER_URL_GET_ORDER, 60000);

		} catch (Exception e) {
			e.printStackTrace();
			curListener.onError();
		}
	}
	

	public static void pluginHttp(Context context,Hashtable<String, String> params, final SdkHttpListener curListener) {
		try {
			String serverUrl = (String) params.get("server_url");
			params.remove("server_url");
			ArrayList<NameValuePair> postData = new ArrayList<NameValuePair>();
			Iterator<String> iterator = params.keySet().iterator();
			String key;
			while (iterator.hasNext()) {
				key = iterator.next();
				postData.add(new BasicNameValuePair(key, params.get(key)));
			}

			sSdkHttpTask = new SdkHttpTask(context);
			sSdkHttpTask.doPost(new SdkHttpListener() {
				public void onError() {
					curListener.onError();
					sSdkHttpTask = null;
				}

				public void onResponse(String responseText) {
					curListener.onResponse(responseText);
					sSdkHttpTask = null;
				}
			}, postData, serverUrl, 60000);
		} catch (Exception e) {
			e.printStackTrace();
			curListener.onError();
		}
	}


	public static String getMd5(String str) {
		byte[] bytes;
		StringBuffer stringBuffer = null;
		try {
			bytes = MessageDigest.getInstance("MD5").digest(
					str.getBytes("UTF-8"));
			stringBuffer = new StringBuffer();
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
			return str;
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return str;
		}

		for (int i = 0; i < bytes.length; i++) {
			int j = 0xFF & bytes[i];
			if (j < 16)
				stringBuffer.append("0");
			stringBuffer.append(Integer.toHexString(j));
		}

		return stringBuffer.toString();
	}


	

	// private static String getToken(String str1, String str2)
	// {
	// long l = System.currentTimeMillis() / 1000L;
	// String str = getMd5(String.valueOf(l) + str1 + str2).substring(0, 16);
	// return Base64.encodeToString((str1 + "." + str + "." +
	// l).trim().getBytes(), 2);
	// }

	public static String pluginDecode(String str) {
		return new String(Base64.decode(ckDecode(str), 0));
	}

	public static String pluginEncode(String str) {
		return new String(ckEncode(Base64.encodeToString(str.getBytes(), 0)));
	}

	public static void setPackage(String packageStr) {
		if ((!Wrapper.getChannelId().equals("999999"))
				&& (Wrapper.getDeveloperInfo().containsKey("privateKey"))
				&& (!packageStr.equals(Wrapper.getDeveloperInfo().get(
						"privateKey")))) {
			Log.e("OpenSDK",
					" the params(appKey、appSecret、privateKey) are wrong ");
			System.exit(0);
		}
	}
}