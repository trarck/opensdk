package com.opensdk.utils;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.security.MessageDigest;
import java.util.Collections;
import java.util.Iterator;
import java.util.Locale;

import org.apache.http.conn.util.InetAddressUtils;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;

public class Device {
	private static String TAG=Device.class.getSimpleName();

	private static String SHA1(String str) {
		try {
			MessageDigest messageDigest = MessageDigest.getInstance("SHA-1");
			// byte[] _tmp = new byte[40];
			messageDigest.update(str.getBytes("UTF-8"), 0, str.length());
			return convertToHex(messageDigest.digest());
		} catch (Exception localException) {
		}
		return null;
	}
	
	private static String convertToHex(byte[] bytes) {
		StringBuffer stringBuffer = new StringBuffer();

		int j = 0;
		for (int i = 0; i < bytes.length; ++i) {

			j = 0xF & bytes[i] >>> 4;
			if (j >= 0 && j <= 9)
				stringBuffer.append((char) (j + 48));
			else
				stringBuffer.append((char) (97 + (j - 10)));

			j = 0xF & bytes[i];
			if (j >= 0 && j <= 9)
				stringBuffer.append((char) (j + 48));
			else
				stringBuffer.append((char) (97 + (j - 10)));
		}
		return stringBuffer.toString();
	}

	public static String getAppv(Context context) {
		PackageInfo packageInfo = getPackageInfo(context);
		if (packageInfo == null)
			return "";
		Log.d(TAG, packageInfo.versionName);
		return packageInfo.versionName;
	}

	public static String getCc() {
		String str = Locale.getDefault().getCountry();
		if (("".equals(str)) || (str == null))
			str = "";
		return str;
	}

	public static String getCkid(Context context) {
		return getODIN1(context);
	}

	public static String getDevice() {
		new Build();
		if (("".equals(Build.MODEL)) || (Build.MODEL == null))
			return "";
		return Build.MODEL;
	}

	public static String getDt() {
		return "Android";
	}

	public static String getGPS(Context context) {
		return "0,0";
	}

	public static String getIMEI(Context context) {
		if (context == null)
			return "";
		try {
			String deviceId = ((TelephonyManager) context
					.getSystemService("phone")).getDeviceId();

			if (deviceId == null || "".equals(deviceId)) {
				deviceId = getODIN1(context);
			}
			return deviceId == null ? "" : deviceId;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public static String getIMSI(Context context) {
		try {
			String subscriberId = ((TelephonyManager) context
					.getSystemService("phone")).getSubscriberId();
			return subscriberId;
		} catch (Exception localException) {
			localException.printStackTrace();
		}
		return "";
	}

	public static String getIP() {
		try {
			Iterator<NetworkInterface> iterator = Collections.list(
					NetworkInterface.getNetworkInterfaces()).iterator();
			while (iterator.hasNext()) {

				InetAddress inetaddress;
				String ip;
				Iterator<InetAddress> iteratorAddress = Collections.list(
						iterator.next().getInetAddresses()).iterator();
				while (iteratorAddress.hasNext()) {
					inetaddress = iteratorAddress.next();
					if (!inetaddress.isLoopbackAddress()) {
						ip = inetaddress.getHostAddress();
						if (InetAddressUtils.isIPv4Address(ip)) {
							return ip;
						}
					}
				}
			}
		} catch (SocketException localSocketException) {
		}
		return "1.1.1.1";
	}

	public static String getIsp(Context context) {
		TelephonyManager localTelephonyManager = (TelephonyManager) context
				.getSystemService("phone");
		if (("".equals(localTelephonyManager.getSimOperator()))
				|| (localTelephonyManager.getSimOperator() == null))
			return "";
		return localTelephonyManager.getSimOperator();
	}

	public static String getJb() {
		return "0";
	}

	public static String getLang() {
		String str = Locale.getDefault().getLanguage();
		if (("".equals(str)) || (str == null))
			str = "";
		return str;
	}

	public static String getMACAddress(Context context) {
		if (context == null)
			return null;
		String str = ((WifiManager) context.getSystemService("wifi"))
				.getConnectionInfo().getMacAddress();
		if (("".equals(str)) || (str == null))
			return "";
		return str.replace(":", "");
	}

	public static String getNet(Context context) {
		try {
			NetworkInfo networkInfo = ((ConnectivityManager) context
					.getSystemService("connectivity")).getActiveNetworkInfo();
			int i = networkInfo.getType();
			if (i == 0) {
				boolean bool = networkInfo.getExtraInfo().toLowerCase()
						.equals("cmnet");
				if (bool)
					return "2";
				return "3";
			}
			if (i == 1)
				return "1";
			return "4";
		} catch (Exception localException) {
		}
		return "4";
	}

	private static String getODIN1(Context context) {
		return SHA1(android.provider.Settings.Secure.getString(
				context.getContentResolver(), "android_id"));
	}

	public static String getOri(Context context) {
		if (context.getResources().getConfiguration().orientation == 1)
			return "3";
		if (context.getResources().getConfiguration().orientation == 1)
			return "1";
		return "0";
	}

	public static String getOst(Context context) {
		if ("000000000000000".equalsIgnoreCase(((TelephonyManager) context
				.getSystemService("phone")).getDeviceId()))
			return "5";
		return "2";
	}

	public static String getOsv() {
		String str = Build.VERSION.RELEASE;
		if (("".equals(str)) || (str == null))
			str = "";
		return str;
	}

	public static PackageInfo getPackageInfo(Context context) {
		PackageManager localPackageManager = context.getPackageManager();
		try {
			PackageInfo localPackageInfo = localPackageManager.getPackageInfo(
					context.getPackageName(), 0);
			return localPackageInfo;
		} catch (PackageManager.NameNotFoundException localNameNotFoundException) {
			localNameNotFoundException.printStackTrace();
		}
		return null;
	}

	public static String getPhoneNum(Context context) {
		try {
			String str = ((TelephonyManager) context.getSystemService("phone"))
					.getLine1Number();
			return str;
		} catch (Exception localException) {
			localException.printStackTrace();
		}
		return "";
	}

	public static String getSDK() {
		return "0.1C2013031201";
	}

	public static String getSr(Context context) {
		DisplayMetrics localDisplayMetrics = new DisplayMetrics();
		((Activity) context).getWindowManager().getDefaultDisplay()
				.getMetrics(localDisplayMetrics);
		return localDisplayMetrics.widthPixels + ","
				+ localDisplayMetrics.heightPixels;
	}
}