/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import org.cocos2dx.lib.Cocos2dxHelper.Cocos2dxHelperListener;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.os.Bundle;
import android.os.Message;
import android.preference.PreferenceManager.OnActivityResultListener;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

import com.baidu.mobads.AdView;
import com.baidu.mobads.AdViewListener;
import com.baidu.mobads.InterstitialAd;
import com.baidu.mobads.InterstitialAdListener;

public abstract class Cocos2dxActivity extends Activity implements
		Cocos2dxHelperListener {
	// ===========================================================
	// Constants
	// ===========================================================

	private final static String TAG = Cocos2dxActivity.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================

	private Cocos2dxGLSurfaceView mGLSurfaceView;
	private Cocos2dxHandler mHandler;
	private static Cocos2dxActivity sContext = null;
	private Cocos2dxVideoHelper mVideoHelper = null;

	public static Context getContext() {
		return sContext;
	}

	// ===========================================================
	// Jni Function
	// ===========================================================
	private static InterstitialAd interAd;
	private static RelativeLayout rl;

	public static void loadAd() {
		interAd = new InterstitialAd(sContext);
		interAd.setListener(new InterstitialAdListener() {

			@Override
			public void onAdClick(InterstitialAd arg0) {
				Log.i("InterstitialAd", "Baidu--------------------onAdClick");
			}

			@Override
			public void onAdDismissed() {
				Log.i("InterstitialAd",
						"Baidu--------------------onAdDismissed");
				interAd.loadAd();
				resumeGame(1);
			}

			@Override
			public void onAdFailed(String arg0) {
				Log.i("InterstitialAd", "Baidu--------------------onAdFailed");
				resumeGame(0);
			}

			@Override
			public void onAdPresent() {
				Log.i("InterstitialAd", "Baidu--------------------onAdPresent");
				resumeGame(0);
			}

			@Override
			public void onAdReady() {
				Log.i("InterstitialAd", "Baidu--------------------onAdReady");
				resumeGame(0);
			}

		});
		interAd.loadAd();
	}

	public static void showInterAd() {
		sContext.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (interAd.isAdReady()) {
					interAd.showAd(sContext);
				} else {
					//	插屏广告展示失败，恢复暂停按钮可点
					//	检查网络，重新加载广告
					resumeGame(0);
					if (isConnectionAvaliable()) {
						interAd.loadAd();
					}
				}
			}
		});
	}

	public void showBannerAd() {
		// 创建广告View
		AdView adView = new AdView(this);
		// 设置监听器
		adView.setListener(new AdViewListener() {
			public void onAdSwitch() {
				Log.w("", "onAdSwitch");
			}

			public void onAdShow(JSONObject info) {
				Log.w("", "onAdShow " + info.toString());
			}

			public void onAdReady(AdView adView) {
				Log.w("", "onAdReady " + adView);
			}

			public void onAdFailed(String reason) {
				Log.w("", "onAdFailed " + reason);
				//	横幅广告加载失败，移除
				mFrameLayout.removeView(rl);
			}

			public void onAdClick(JSONObject info) {
				Log.w("", "onAdClick " + info.toString());
			}

			public void onVideoStart() {
				Log.w("", "onVideoStart");
			}

			public void onVideoFinish() {
				Log.w("", "onVideoFinish");
			}

			@Override
			public void onVideoClickAd() {
				Log.w("", "onVideoFinish");
			}

			@Override
			public void onVideoClickClose() {
				Log.w("", "onVideoFinish");
			}

			@Override
			public void onVideoClickReplay() {
				Log.w("", "onVideoFinish");
			}

			@Override
			public void onVideoError() {
				Log.w("", "onVideoFinish");
			}
		});

		DisplayMetrics dm = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(dm);

		rl = new RelativeLayout(sContext);
		RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(
				LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);

		RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(
				LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
		rl.addView(adView, param);

		mFrameLayout.addView(rl, params);
	}

	public static native void resumeGame(int success);

	public static void showExitDialog() {
		sContext.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Builder build = new Builder(sContext);
				build.setTitle("提示").setMessage("您确定要退出游戏吗？")
						.setPositiveButton("是", new OnClickListener() {

							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								dialog.dismiss();
								sContext.finish();
								System.exit(0);
							}
						}).setNegativeButton("否", new OnClickListener() {

							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								dialog.dismiss();
							}
						});
				Dialog dialog = build.create();
				dialog.show();
			}
		});
	}

	// ===========================================================
	// Utils Begin
	// ===========================================================
	// public static boolean isNetworkAvailable() {
	// ConnectivityManager mCManager =
	// (ConnectivityManager)sContext.getSystemService(Context.CONNECTIVITY_SERVICE);
	// NetworkInfo mWifi =
	// mCManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
	// NetworkInfo mMobile =
	// mCManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
	//
	// Log.e("", "-----"+(mWifi != null &&
	// mWifi.isAvailable())+"----"+mMobile.isAvailable());
	//
	// return (mWifi != null && mWifi.isAvailable()) || mMobile.isAvailable();
	// }

	public static boolean isConnectionAvaliable() {
		boolean isConnected = false;

		ConnectivityManager mcManager = (ConnectivityManager) sContext
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (mcManager != null) {
			NetworkInfo info = mcManager.getActiveNetworkInfo();
			if (info != null && info.isConnected()) {
				isConnected = true;
			}
		}

		return isConnected;
	}

	// ===========================================================
	// Utils End
	// ===========================================================

	protected void onLoadNativeLibraries() {
		try {
			ApplicationInfo ai = getPackageManager().getApplicationInfo(
					getPackageName(), PackageManager.GET_META_DATA);
			Bundle bundle = ai.metaData;
			String libName = bundle.getString("android.app.lib_name");
			System.loadLibrary(libName);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// ===========================================================
	// Constructors
	// ===========================================================

	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		onLoadNativeLibraries();

		sContext = this;
		this.mHandler = new Cocos2dxHandler(this);

		Cocos2dxHelper.init(this);

		this.init();
		if (mVideoHelper == null) {
			mVideoHelper = new Cocos2dxVideoHelper(this, mFrameLayout);
		}

		if (isConnectionAvaliable()) {
			loadAd();
			showBannerAd();
		} else {
			Log.i("Android", "Network is unavailable!");
		}
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	protected void onResume() {
		super.onResume();

		Cocos2dxHelper.onResume();
		this.mGLSurfaceView.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();

		Cocos2dxHelper.onPause();
		this.mGLSurfaceView.onPause();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	@Override
	public void showDialog(final String pTitle, final String pMessage) {
		Message msg = new Message();
		msg.what = Cocos2dxHandler.HANDLER_SHOW_DIALOG;
		msg.obj = new Cocos2dxHandler.DialogMessage(pTitle, pMessage);
		this.mHandler.sendMessage(msg);
	}

	@Override
	public void showEditTextDialog(final String pTitle, final String pContent,
			final int pInputMode, final int pInputFlag, final int pReturnType,
			final int pMaxLength) {
		Message msg = new Message();
		msg.what = Cocos2dxHandler.HANDLER_SHOW_EDITBOX_DIALOG;
		msg.obj = new Cocos2dxHandler.EditBoxMessage(pTitle, pContent,
				pInputMode, pInputFlag, pReturnType, pMaxLength);
		this.mHandler.sendMessage(msg);
	}

	@Override
	public void runOnGLThread(final Runnable pRunnable) {
		this.mGLSurfaceView.queueEvent(pRunnable);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		for (OnActivityResultListener listener : Cocos2dxHelper
				.getOnActivityResultListeners()) {
			listener.onActivityResult(requestCode, resultCode, data);
		}

		super.onActivityResult(requestCode, resultCode, data);
	}

	protected FrameLayout mFrameLayout = null;

	// ===========================================================
	// Methods
	// ===========================================================
	public void init() {

		// FrameLayout
		ViewGroup.LayoutParams framelayout_params = new ViewGroup.LayoutParams(
				ViewGroup.LayoutParams.MATCH_PARENT,
				ViewGroup.LayoutParams.MATCH_PARENT);
		mFrameLayout = new FrameLayout(this);
		mFrameLayout.setLayoutParams(framelayout_params);

		// Cocos2dxEditText layout
		ViewGroup.LayoutParams edittext_layout_params = new ViewGroup.LayoutParams(
				ViewGroup.LayoutParams.MATCH_PARENT,
				ViewGroup.LayoutParams.WRAP_CONTENT);
		Cocos2dxEditText edittext = new Cocos2dxEditText(this);
		edittext.setLayoutParams(edittext_layout_params);

		// ...add to FrameLayout
		mFrameLayout.addView(edittext);

		// Cocos2dxGLSurfaceView
		this.mGLSurfaceView = this.onCreateView();

		// ...add to FrameLayout
		mFrameLayout.addView(this.mGLSurfaceView);

		// Switch to supported OpenGL (ARGB888) mode on emulator
		if (isAndroidEmulator())
			this.mGLSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);

		this.mGLSurfaceView.setCocos2dxRenderer(new Cocos2dxRenderer());
		this.mGLSurfaceView.setCocos2dxEditText(edittext);

		// Set framelayout as the content view
		setContentView(mFrameLayout);
	}

	public Cocos2dxGLSurfaceView onCreateView() {
		return new Cocos2dxGLSurfaceView(this);
	}

	private final static boolean isAndroidEmulator() {
		String model = Build.MODEL;
		Log.d(TAG, "model=" + model);
		String product = Build.PRODUCT;
		Log.d(TAG, "product=" + product);
		boolean isEmulator = false;
		if (product != null) {
			isEmulator = product.equals("sdk") || product.contains("_sdk")
					|| product.contains("sdk_");
		}
		Log.d(TAG, "isEmulator=" + isEmulator);
		return isEmulator;
	}

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================

}
