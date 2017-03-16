package com.opensdk.framework;

import android.content.Intent;

public interface IActivityCallback {

	public boolean onActivityResult(int requestCode, int resultCode, Intent data);

	public abstract void onPause();

	public abstract void onResume();

	public abstract void onNewIntent(Intent paramIntent);

	public abstract void onStop();

	public abstract void onDestroy();

	public abstract void onRestart();
}
