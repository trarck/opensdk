package com.opensdk.framework;

import java.util.Hashtable;

import org.json.JSONException;
import org.json.JSONObject;

import com.opensdk.utils.SdkHttpListener;
import com.opensdk.utils.Util;

import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;

public class UserDebug implements InterfaceUser {

	private static final String SIMSDK_LOGIN_URL = "http://sim.qudao.info/account/login";
	private static final String SIMSDK_LOGOUT_URL = "http://sim.qudao.info/account/logout";
	private static final String SIMSDK_PAY_URL = "http://sim.qudao.info/api/pay/exec";
	public final int PluginType = 7;
	private static Context mContext = null;
	protected static String TAG = "DebugMode";
	private static UserDebug mAdapter = null;
	private static boolean isInited = false;
	private static boolean mLogined = false;
	private static String mUserId = "";
	private static String mSessionId = "";
	private static String mUApiKey = "";
	private static String mUApiSecret = "";
	private static String mServerUrl = "";
	private static String mOrderId = "";
	private static Hashtable mGoodsInfo = null;
	private static boolean isDebug = true;

	protected static void LogE(String msg, Exception exception) {
		Log.e(TAG, msg, exception);
		exception.printStackTrace();
	}

	protected static void LogD(String msg) {
		if (isDebug)
			Log.d(TAG, msg);
	}

	public UserDebug(Context context) {
		mContext = context;
		mAdapter = this;
		configDeveloperInfo(Wrapper.getDeveloperInfo());
	}

	public void configDeveloperInfo(Hashtable<String, String> cpInfo) {
		PluginWrapper.runOnMainThread(new Runnable() {

			@Override
			public void run() {
				if (!isInited) {
					(new Handler()).postDelayed(new Runnable() {
						public void run() {
							isInited = true;
							mServerUrl = Wrapper.getDeveloperInfo().get("oauthLoginServer");
							mUApiKey = Wrapper.getDeveloperInfo().get("uApiKey");
							mUApiSecret = Wrapper.getDeveloperInfo().get("uApiSecret");
							if (mServerUrl == null && mUApiKey == null && mUApiSecret == null && Wrapper.getDeveloperInfo().get( "private_key") == null) {
								UserWrapper.onActionResult(mAdapter, UserWrapper.ACTION_RET_INIT_FAIL, "fail to  call init of AgentManager");
							} else {
								UserWrapper.onActionResult(mAdapter, UserWrapper.ACTION_RET_INIT_SUCCESS, null);
							}
						}
					}, 1000L);
				}
			}
		});
	}

	public void login() {
		PluginWrapper.runOnMainThread(new Runnable() {

			@Override
			public void run() {
				if (!isInited) {
					UserWrapper.onActionResult(mAdapter,
							UserWrapper.ACTION_RET_LOGIN_FAIL, "not init");
				}
				if (!networkReachable()) {
					UserWrapper.onActionResult(mAdapter,
							UserWrapper.ACTION_RET_LOGIN_TIMEOUT,
							"network is unreachable");
				} else {
					android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(mContext);
					builder.setTitle(getResourceId("plugin_login_title", "string"));
					final View view = LayoutInflater.from(mContext).inflate(getResourceId("plugin_login","layout"), null);
					builder.setView(view);

					OnClickListener listener = new OnClickListener() {
						public void onClick(DialogInterface dialoginterface,int i) {
							switch (i) {
							case -2:
								mLogined = false;
								UserWrapper.onActionResult(mAdapter,UserWrapper.ACTION_RET_LOGIN_CANCEL,"the login has been canceled");
								return;

							case -1:
								EditText username = (EditText) view.findViewById(getResourceId("txt_username", "id"));
								EditText password = (EditText) view.findViewById(getResourceId("txt_password", "id"));
								if (username.getText().toString().length() == 0|| password.getText().toString().length() == 0) {
									UserWrapper.onActionResult(
											mAdapter,
											UserWrapper.ACTION_RET_LOGIN_FAIL,
											"username or password is empty");
									return;
								}

								userLogin(username.getText().toString(),
										password.getText().toString(),
										new ILoginCallback() {
											public void onSuccessed(int j,String s) {
												mLogined = true;
												UserWrapper.onActionResult(mAdapter, UserWrapper.ACTION_RET_LOGIN_SUCCESS,s);
											}

											public void onFailed(int j, String s) {
												mLogined = false;
												UserWrapper.onActionResult(mAdapter, UserWrapper.ACTION_RET_LOGIN_FAIL,s);
											}
										});
								break;
							}
						}
					};

					builder.setPositiveButton(
							getResourceId("plugin_login", "string"),listener);
					builder.setNegativeButton(
							getResourceId("plugin_cancel", "string"),listener).create();
					builder.setCancelable(false);
					builder.show();
					return;
				}

			}
		});
	}

	private void userLogin(String username, String password, final ILoginCallback callback) {
		Hashtable<String, String> param = new Hashtable<String, String>();
		param.put("username", username);
		param.put("password", password);
		param.put("server_url", SIMSDK_LOGIN_URL);

		
		Util.pluginHttp(mContext, param, new SdkHttpListener() {
			public void onResponse(String response) {
				Log.d("onResponse", response);
				String msg;
				try {
					JSONObject json = new JSONObject(response);
					msg = json.getString("errMsg");

					if (msg != null && msg.equals("success")) {
						JSONObject data = json.getJSONObject("data");
						Log.d("data", data.toString());
						mUserId = data.getString("user_id");
						mSessionId = data.getString("session_id");
						Log.d("user_id", mUserId);
						Log.d("session_id", mSessionId);

						Hashtable<String, String> param = new Hashtable<String, String>();

						param.put("channel", "simsdk");
						param.put("server_url", mServerUrl);
						param.put("session_id", mSessionId);
						param.put("user_id", mUserId);
						param.put("uapi_key", mUApiKey);
						param.put("uapi_secret", mUApiSecret);

						UserWrapper.getAccessToken(mContext, param,
								new SdkHttpListener() {

									public void onResponse(String response2) {
										Log.d("onResponse", response2);
										String status;
										try {
											JSONObject json2 = new JSONObject(response2);
											status = json2.getString("status");
											if (status != null&& status.equals("ok")) {
												String ext = json2.optString("ext");
												callback.onSuccessed(UserWrapper.ACTION_RET_LOGIN_SUCCESS,ext != null ? ext : "");
											} else {
												callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
											}
										} catch (JSONException e) {
											callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
										}
									}

									public void onError() {
										callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
									}
								});
						return;
					} else {
						callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
						return;
					}
				} catch (JSONException e) {
					callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
				}
			}

			public void onError() {
				callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
			}
		});
		
	}

	public void logout() {
		PluginWrapper.runOnMainThread(new Runnable() {

			@Override
			public void run() {
				if (!isLogined()) {
					UserWrapper.onActionResult(mAdapter, 8,"not need logout");
					LogD("User not logined!");
					return;
				} else {
					userLogout();
					return;
				}

			}
		});
	}

	private void userLogout() {
		Hashtable<String, String> param = new Hashtable<String, String>();
		param.put("user_id", mUserId);
		param.put("session_id", mSessionId);
		param.put("server_url", SIMSDK_LOGOUT_URL);
		
		Util.pluginHttp(mContext, param, new SdkHttpListener() {

			public void onResponse(String response) {
				Log.d("onResponse", response);
				try {
					JSONObject json = new JSONObject(response);
					String msg = json.getString("errMsg");
					if (msg != null && msg.equals("success")) {
						mLogined = false;
						UserWrapper.onActionResult(mAdapter,
								UserWrapper.ACTION_RET_LOGOUT_SUCCESS, "");
						return;
					} else {
						UserWrapper.onActionResult(mAdapter,
								UserWrapper.ACTION_RET_LOGOUT_FAIL, "");
						return;
					}
				} catch (JSONException _ex) {
					UserWrapper.onActionResult(mAdapter,
							UserWrapper.ACTION_RET_LOGOUT_FAIL, "");
				}
			}

			public void onError() {
				UserWrapper.onActionResult(mAdapter,
						UserWrapper.ACTION_RET_LOGOUT_FAIL, "");
			}
		});
		
	}

	public boolean isLogined() {
		return mLogined;
	}

	public String getSessionID() {
		return mSessionId;
	}

	public void pause() {
		showDialog("plugin_pause", "plugin_pause");
	}

	public void exit() {
		PluginWrapper.runOnMainThread(new Runnable() {

			@Override
			public void run() {
				Builder builder = new Builder(mContext);
				builder.setTitle(mContext.getResources()
						.getIdentifier("plugin_exit", "string",
								mContext.getPackageName()));
				builder.setMessage(mContext.getResources()
						.getIdentifier("plugin_exit", "string",
								mContext.getPackageName()));

				OnClickListener listener = new OnClickListener() {
					public void onClick(DialogInterface dialoginterface, int i) {
						switch (i) {
						case -2:
							return;

						case -1:
							UserWrapper.onActionResult(mAdapter, 12,
									"exit");
							break;
						}
					}
				};

				builder.setPositiveButton(
						mContext.getResources().getIdentifier(
								"plugin_sure", "string",
								mContext.getPackageName()), listener);
				builder.setNegativeButton(
						mContext.getResources().getIdentifier(
								"plugin_cancel", "string",
								mContext.getPackageName()), listener)
						.create();
				builder.show();
			}
		});
	}

	public void enterPlatform() {
		showDialog("plugin_center", "plugin_center");
	}

	public void showToolBar(int i) {
		showDialog("plugin_showTool", "plugin_showTool");
	}

	public void hideToolBar() {
		showDialog("plugin_hideTool", "plugin_hideTool");
	}

	public String getUserID() {
		LogD("getUserID() invoked!");
		return mUserId;
	}

	public void antiAddictionQuery() {
		showDialog("plugin_antiAddictionQuery", "plugin_antiAddictionQuery");
	}

	public void realNameRegister() {
		showDialog("plugin_realNameRegister", "plugin_realNameRegister");
	}

	public void submitLoginGameRole(JSONObject jsonobject) {
		showDialog("plugin_submitLoginGameRole", "plugin_submitLoginGameRole");
	}

	public void accountSwitch() {
		showDialog("plugin_accountSwitch", "plugin_accountSwitch");
	}

	public void destroy() {
		LogD("destroy");
	}

	private boolean networkReachable() {
		boolean flag = false;
		NetworkInfo networkinfo;
		try {
			flag = (networkinfo = ((ConnectivityManager) mContext
					.getSystemService("connectivity")).getActiveNetworkInfo()) != null ? networkinfo
					.isAvailable() : false;
		} catch (Exception exception) {
			LogE("Fail to check network status", exception);
		}
		LogD((new StringBuilder("NetWork reachable : ")).append(flag)
				.toString());
		return flag;
	}

	public void setDebugMode(boolean flag) {
		isDebug = flag;
	}

	public String getSDKVersion() {
		return "2.0.3";
	}

	public String getPluginVersion() {
		return "2.0.3";
	}

	public static void showDialog(final String curTitle, final String curMsg) {
		PluginWrapper.runOnMainThread(new Runnable() {

			@Override
			public void run() {

				Builder builder = new Builder(mContext);

				builder.setTitle(getResourceId(curTitle, "string"));
				builder.setMessage(getResourceId(curMsg, "string"));
				builder.setPositiveButton("Ok", new OnClickListener() {
					public void onClick(DialogInterface dialoginterface, int i) {

					}

				});
				builder.create();
				builder.show();

			}
		});
	}

	public static int getResourceId(String key, String type) {
		return mContext.getResources().getIdentifier(key, type,mContext.getPackageName());
	}

	public boolean isSupportFunction(String functionName) {
		return true;
	}

	public String getPluginId() {
		return "999999";
	}

	public static boolean getLoginState() {
		return mLogined;
	}

	public static void setLoginState(boolean flag) {
		mLogined = flag;
	}

	public static String getSimUserId() {
		return mUserId;
	}

	public static String getSimSessionId() {
		return mSessionId;
	}

	public static void setSimUserInfo(String userId, String sessionId) {
		mUserId = userId;
		mSessionId = sessionId;
	}

}
