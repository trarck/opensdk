package com.opensdk.utils;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Proxy;
import java.util.List;
import org.apache.http.HttpHost;
import org.apache.http.HttpRequest;
import org.apache.http.HttpResponse;
import org.apache.http.HttpVersion;
import org.apache.http.NameValuePair;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.params.HttpClientParams;
import org.apache.http.conn.params.ConnManagerParams;
import org.apache.http.conn.params.ConnPerRouteBean;
import org.apache.http.conn.scheme.PlainSocketFactory;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.conn.ssl.SSLSocketFactory;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.client.DefaultHttpRequestRetryHandler;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.params.HttpProtocolParams;

public class HttpUtils {
	private static int mTimeOut;

	private static DefaultHttpClient createHttpClient() {
		SchemeRegistry schemeRegistry = new SchemeRegistry();
		schemeRegistry.register(new Scheme("http", PlainSocketFactory.getSocketFactory(), 80));
		schemeRegistry.register(new Scheme("https", SSLSocketFactory.getSocketFactory(), 443));
		HttpParams httpParams = createHttpParams();
		DefaultHttpClient defaultHttpClient = new DefaultHttpClient(
				new ThreadSafeClientConnManager(httpParams, schemeRegistry),
				httpParams);
		defaultHttpClient.setHttpRequestRetryHandler(new DefaultHttpRequestRetryHandler(2, true));
		return defaultHttpClient;
	}

	private static HttpParams createHttpParams() {
		BasicHttpParams basicHttpParams = new BasicHttpParams();
		HttpConnectionParams.setStaleCheckingEnabled(basicHttpParams, false);
		HttpConnectionParams.setConnectionTimeout(basicHttpParams, mTimeOut);
		HttpConnectionParams.setSoTimeout(basicHttpParams, mTimeOut);
		HttpConnectionParams.setSocketBufferSize(basicHttpParams, 8192);
		HttpConnectionParams.setTcpNoDelay(basicHttpParams, true);
		HttpProtocolParams.setVersion(basicHttpParams, HttpVersion.HTTP_1_1);
		HttpProtocolParams.setUseExpectContinue(basicHttpParams, false);
		HttpClientParams.setRedirecting(basicHttpParams, false);
		ConnManagerParams.setMaxTotalConnections(basicHttpParams, 50);
		ConnManagerParams.setTimeout(basicHttpParams, mTimeOut);
		ConnManagerParams.setMaxConnectionsPerRoute(basicHttpParams,
				new ConnPerRouteBean(20));
		return basicHttpParams;
	}

	public static HttpResponse get(Context context, String url, int timeOut)throws Exception {
		mTimeOut = timeOut;
		String uri = url.replaceAll(" ", "%20");
		DefaultHttpClient defaultHttpClient = createHttpClient();
		HttpHost httpHost = getProxyHttpHost(context);
		if (httpHost != null)
			defaultHttpClient.getParams().setParameter(
					"http.route.default-proxy", httpHost);
		else
			defaultHttpClient.getParams().removeParameter(
					"http.route.default-proxy");

		HttpGet httpGet = new HttpGet(uri);
		httpGet.addHeader("Content-Type",
				"text/html; application/x-www-form-urlencoded; charset=UTF-8");
		return defaultHttpClient.execute((HttpUriRequest) httpGet);

	}

	public static HttpHost getProxyHttpHost(Context context) {
		if (context != null) {
			NetworkInfo networkinfo = ((ConnectivityManager) context.getSystemService("connectivity")).getActiveNetworkInfo();
			String host = null;
			int port = 0;
			if (networkinfo != null) {
				int type = networkinfo.getType();
				if (type != 1) {
					host = Proxy.getDefaultHost();
					port = Proxy.getDefaultPort();
				}
			}
			if (host != null)
				return new HttpHost(host, port);
		}
		return null;
	}

	public static HttpResponse post(Context context, String url,List<NameValuePair> data, int timeOut) throws Exception {
		mTimeOut = timeOut;
		String uri = url.replaceAll(" ", "%20");
		DefaultHttpClient defaulthttpclient = createHttpClient();
		HttpHost httphost = getProxyHttpHost(context);
		if (httphost != null)
			defaulthttpclient.getParams().setParameter("http.route.default-proxy", httphost);
		else
			defaulthttpclient.getParams().removeParameter("http.route.default-proxy");
		HttpPost httppost = new HttpPost(uri);
		if (!data.isEmpty())
			httppost.setEntity(new UrlEncodedFormEntity(data, "UTF-8"));
		return defaulthttpclient.execute(httppost);
	}
}