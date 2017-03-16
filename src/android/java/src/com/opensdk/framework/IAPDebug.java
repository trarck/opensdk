
package com.opensdk.framework;

import java.util.Hashtable;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;

import com.opensdk.utils.SdkHttpListener;
import com.opensdk.utils.Util;

public class IAPDebug
    implements InterfaceIAP
{

    private static final String SIMSDK_LOGIN_URL = "http://sim.qudao.info/account/login";
    private static final String SIMSDK_LOGOUT_URL = "http://sim.qudao.info/account/logout";
    private static final String SIMSDK_PAY_URL = "http://sim.qudao.info/api/pay/exec";
    private static Context mContext = null;
    protected static String TAG = "IAPDebug";
    private static IAPDebug mAdapter = null;
    private static boolean isInited = false;
    private static String mUApiKey = "";
    private static String mUApiSecret = "";
    private static String mServerUrl = "";
    private static String mOrderId = "";
    private static Hashtable<String,String> mGoodsInfo = null;
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

    public IAPDebug(Context context)
    {
        mContext = context;
        mAdapter = this;
        configDeveloperInfo(Wrapper.getDeveloperInfo());
    }

    public void configDeveloperInfo(final Hashtable<String,String> cpInfo)
    {
        PluginWrapper.runOnMainThread(new Runnable() {
			
			@Override
			public void run()
	        {
	            if(isInited)
	                return;
	            isInited = true;
	            mServerUrl = cpInfo.get("oauthLoginServer");
	            mUApiKey = cpInfo.get("uApiKey");
	            mUApiSecret = cpInfo.get("uApiSecret");
	            if(mServerUrl == null && mUApiKey == null && mUApiSecret == null && cpInfo.get("private_key") == null)
	            {
	                payResult(IAPWrapper.PAYRESULT_INIT_FAIL, "fail to  call init of AgentManager");
	            } else
	            {
	                payResult(IAPWrapper.PAYRESULT_INIT_SUCCESS, null);
	            }
	        }
		});
    }

    public boolean isLogined()
    {
        return UserDebug.getLoginState();
    }

    private void userLogin(String username, String password, final ILoginCallback callback)
    {
    	Hashtable<String,String> param = new Hashtable<String,String>();
    	param.put("username", username);
    	param.put("password", password);
    	param.put("server_url", SIMSDK_LOGIN_URL);
    	
        Util.pluginHttp(mContext, param, new SdkHttpListener() {
			
        	public void onResponse(String response)
            {
                Log.d("onResponse", response);
                String msg;
                try
                {
                	JSONObject json = new JSONObject(response);
                	msg=json.getString("errMsg");
                    if(msg!= null && msg.equals("success"))
                    {
                    	JSONObject data = json.getJSONObject("data");
                        Log.d("data", data.toString());
                        UserDebug.setSimUserInfo(data.getString("user_id"), data.getString("session_id"));
                        Hashtable<String,String> paramAuth = new Hashtable<String,String>();
                        paramAuth.put("channel", "simsdk");
                        
                        paramAuth.put("server_url", mServerUrl);
                        paramAuth.put("session_id", UserDebug.getSimSessionId());
                        paramAuth.put("user_id", UserDebug.getSimUserId());
                        paramAuth.put("uapi_key", mUApiKey);
                        paramAuth.put("uapi_secret", mUApiSecret);

                        UserWrapper.getAccessToken(mContext, paramAuth, new SdkHttpListener() {
							
                        	public void onResponse(String response2)
                            {
                                Log.d("onResponse", response2);
                                String status;
                                try
                                {
                                	JSONObject json2 = new JSONObject(response2);
                                	status=json2.getString("status");
                                    if(status != null && status.equals("ok"))
                                    {
                                    	String ext=json2.optString("ext");
                                        callback.onSuccessed(UserWrapper.ACTION_RET_LOGIN_SUCCESS, ext != null ? ext : "");
                                        return;
                                    } else
                                    {
                                        callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
                                        return;
                                    }
                                }
                                catch(JSONException ex)
                                {
                                    callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
                                }
                            }

                            public void onError()
                            {
                                callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
                            }
						});
                        return;
                    } else
                    {
                        callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
                        return;
                    }
                }
                catch(JSONException ex)
                {
                    callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
                }
            }

            public void onError()
            {
                callback.onFailed(UserWrapper.ACTION_RET_LOGIN_FAIL, "");
            }

		});
    }

    private boolean networkReachable()
    {
        boolean flag = false;
        NetworkInfo networkinfo;
        try
        {
            flag = (networkinfo = ((ConnectivityManager)mContext.getSystemService("connectivity")).getActiveNetworkInfo()) != null ? networkinfo.isAvailable() : false;
        }
        catch(Exception exception)
        {
            LogE("Fail to check network status", exception);
        }
        LogD((new StringBuilder("NetWork reachable : ")).append(flag).toString());
        return flag;
    }

    public void payForProduct(Hashtable<String, String> productInfo)
    {
        LogD((new StringBuilder("payForProduct invoked ")).append(productInfo.toString()).toString());
        mGoodsInfo = productInfo;
        PluginWrapper.runOnMainThread(new Runnable() {
			
			@Override
			public void run()
	        {
	            if(!networkReachable())
	            {
	                payResult(IAPWrapper.PAYRESULT_NETWORK_ERROR, "Network error!");
	                return;
	            }
	            
	            if(mGoodsInfo == null)
	            {
	                payResult(IAPWrapper.PAYRESULT_FAIL, "info is null");
	                return;
	            }
	            
	            if(!isLogined())
	            {
	                payLogin();
	                return;
	            } else
	            {
	                getOrderId(mGoodsInfo);
	                return;
	            }
	        }
		});
    }

    private static void getOrderId(Hashtable<String, String> productInfo)
    {
        String productId = productInfo.get("Product_Id");
        String productName = productInfo.get("Product_Name");
        String productPrice = productInfo.get("Product_Price");
        String productCount = productInfo.get("Product_Count");
        String roleId = productInfo.get("Role_Id");
        String roleName = productInfo.get("Role_Name");
        String serverId = productInfo.get("Server_Id");
        if(productId == null || productName == null || productPrice == null || productCount == null || roleId == null || roleName == null || serverId == null)
        {
            payResult(IAPWrapper.PAYRESULT_PRODUCTIONINFOR_INCOMPLETE, "something is null");
            return;
        }
        String ext =productInfo.get("EXT");
        float price = Float.parseFloat(productPrice);
        int count=Integer.parseInt(productCount) > 0 ? Integer.parseInt(productCount) : 1;
        final int money = count * (int)price;
        
        Hashtable<String, String> param=new Hashtable<String, String>();
        param.put("uapi_key", mUApiKey);
        param.put("uapi_secret", mUApiSecret);
        param.put("order_type", "999");
        param.put("money", String.valueOf(money));
        param.put("game_user_id", roleId);
        param.put("game_server_id", serverId);
        param.put("product_id", productId);
        param.put("user_id", UserDebug.getSimUserId());
        param.put("product_name", productName);
        if(ext != null)
        	param.put("private_data", ext);
        
        IAPWrapper.getPayOrderId(mContext, param, new SdkHttpListener() {
			
        	public final void onResponse(String response)
            {
                LogD(response);
                try
                {
	                String status;
	                JSONObject json=new JSONObject(response);
	                status=json.getString("status");
	                
	                if(status != null && status.equals("ok"))
	                {
	                    mOrderId = json.getJSONObject("data").getString("order_id");
	                    addPayment(mOrderId, money);
	                } else
	                {
	                    payResult(IAPWrapper.PAYRESULT_FAIL, null);
	                }
                }catch (JSONException e)
                {
                  e.printStackTrace();
                  payResult(IAPWrapper.PAYRESULT_FAIL, null);
                }

            }

            public final void onError()
            {
                payResult(IAPWrapper.PAYRESULT_NETWORK_ERROR, null);
            }
		});
    }

    private static void addPayment(final String orderId, final int money)
    {
        PluginWrapper.runOnMainThread(new Runnable() {
			
			@Override
			public final void run()
	        {
	            try
	            {
	                Builder builder=new Builder(mContext);
	                builder.setTitle(mContext.getResources().getIdentifier("plugin_pay", "string", mContext.getPackageName()));
	                builder.setMessage(mContext.getResources().getIdentifier("plugin_pay_content", "string", mContext.getPackageName()));
	     
	                OnClickListener listener = new OnClickListener(){
	                	public void onClick(DialogInterface dialoginterface, int which)
	                    {
	                        switch(which)
	                        {
	                        case -2: 
	                            payResult(IAPWrapper.PAYRESULT_CANCEL, "the pay has been canceled");
	                            return;

	                        case -1: 
	                            pay(orderId, money);
	                            break;
	                        }
	                    }

	                };
	                builder.setPositiveButton(mContext.getResources().getIdentifier("plugin_pay", "string", mContext.getPackageName()), listener);
	                builder.setNegativeButton(mContext.getResources().getIdentifier("plugin_cancel", "string", mContext.getPackageName()), listener).create();
	                builder.show();
	            }
	            catch(Exception exception)
	            {
	                LogE("Error during payment", exception);
	                payResult(IAPWrapper.PAYRESULT_FAIL, "Pay failed");
	            }
	        }
		});
    }

    private void payLogin()
    {
        Builder builder= new Builder(mContext);
        builder.setTitle(getResourceId("plugin_login_title", "string"));
        final View view = LayoutInflater.from(mContext).inflate(getResourceId("plugin_login", "layout"), null);
        builder.setView(view);
        OnClickListener listener = new OnClickListener(){
        	public void onClick(DialogInterface dialoginterface, int which)
            {
                switch(which)
                {
                case -2: 
                    UserDebug.setLoginState(false);
                    payResult(IAPWrapper.PAYRESULT_CANCEL, "");
                    return;

                case -1: 
                	EditText username = (EditText)view.findViewById(getResourceId("txt_username", "id"));
                	EditText password = (EditText)view.findViewById(getResourceId("txt_password", "id"));
                    if(username.getText().toString().length()==0 || password.getText().toString().length()==0)
                    {
                        payResult(IAPWrapper.PAYRESULT_FAIL, "username or password is empty");
                        return;
                    }

                    userLogin(username.getText().toString(), password.getText().toString(), new ILoginCallback(){
                    	 public void onSuccessed(int code, String msg)
                         {
                             UserDebug.setLoginState(true);
                             getOrderId(mGoodsInfo);
                         }

                         public void onFailed(int code, String msg)
                         {
                             UserDebug.setLoginState(false);
                             payResult(IAPWrapper.PAYRESULT_FAIL, msg);
                         }

                    });
                    break;
                }
            }
        };
        
        builder.setPositiveButton(getResourceId("plugin_login", "string"), listener);
        builder.setNegativeButton(getResourceId("plugin_cancel", "string"), listener).create();
        builder.setCancelable(false);
        builder.show();
    }

    private static void pay(String orderId, int money)
    {
        Hashtable<String,String> param = new Hashtable<String,String>();
        param.put("user_id", UserDebug.getSimUserId());
        param.put("session_id", UserDebug.getSimSessionId());
        param.put("cp_order_id", orderId);
        param.put("callback", "http://pay.anysdk.com/v5/SimsdkPayNotice/SimsdkPayNotice/");
        param.put("amount", String.valueOf(money));
        param.put("server_url", SIMSDK_PAY_URL);
        
        
        Util.pluginHttp(mContext, param, new SdkHttpListener() {
			
        	public final void onResponse(String response)
            {
                Log.d("onResponse", response);
                try
                {
	                JSONObject json=new JSONObject(response);
	                String msg=json.getString("errMsg");
                    if(msg != null && msg.equals("success"))
                    {
                        payResult(IAPWrapper.PAYRESULT_SUCCESS, "");
                        return;
                    } else
                    {
                        payResult(IAPWrapper.PAYRESULT_FAIL, "");
                        return;
                    }
                }
                catch(JSONException e)
                {
                    payResult(IAPWrapper.PAYRESULT_FAIL, "");
                }
            }

            public final void onError()
            {
                payResult(IAPWrapper.PAYRESULT_NETWORK_ERROR, "");
            }
		});
    }

    private static void payResult(int result, String msg)
    {
        IAPWrapper.onPayResult(mAdapter, result, msg);
        LogD((new StringBuilder("payResult : ")).append(result).append(" msg : ").append(msg).toString());
    }

    public void setDebugMode(boolean flag)
    {
        isDebug = flag;
    }

    public String getSDKVersion()
    {
        return "2.0.3";
    }

    public String getPluginVersion()
    {
        return "2.0.3";
    }

    public static void showDialog(final String curTitle, final String curMsg)
    {
        PluginWrapper.runOnMainThread(new Runnable() {
			
        	public final void run()
            {
                Builder builder= new Builder(mContext);
                
                builder.setTitle(getResourceId(curTitle, "string"));
                builder.setMessage(getResourceId(curMsg, "string"));
                builder.setPositiveButton("Ok", new OnClickListener(){
                	public void onClick(DialogInterface dialoginterface, int which)
                    {
                    }
                });
                builder.create();
                builder.show();
            }
		});
    }

    public static int getResourceId(String key, String type)
    {
        return mContext.getResources().getIdentifier(key, type, mContext.getPackageName());
    }

    public String getPluginId()
    {
        return "999999";
    }

    public String getOrderId()
    {
        return mOrderId;
    }

}
