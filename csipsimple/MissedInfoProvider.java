package com.csipsimple.service;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.database.Cursor;
import android.net.Uri;

public class MissedInfoProvider extends ContentProvider {

	private SharedPreferences mShaPre;
	private Editor mEditor;

	// private int mMissedCallCount = 0;
	// private int mMissedMsgCount = 0;

	// public void setMissedCallCount(int count) {
	// android.util.Log.i("csipsimple", "setCallCount:" + count);
	// mMissedCallCount = count;
	// Editor editor = mShaPre.edit();
	// editor.putInt("missedCallCount", mMissedCallCount);
	// editor.commit();
	// }
	//
	// public void setMissedMsgCount(int count) {
	// mMissedMsgCount = count;
	// Editor editor = mShaPre.edit();
	// editor.putInt("missedMsgCount", mMissedMsgCount);
	// editor.commit();
	// }
	//
	// public int getMissedCallCount() {
	// return mMissedCallCount;
	// }

	//
	// public int getMissedMsgCount() {
	// return mMissedMsgCount;
	// }

	@Override
	public int delete(Uri arg0, String arg1, String[] arg2) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public String getType(Uri uri) {
		// TODO Auto-generated method stub
		// if (uri != null) {
		// if (uri.getLastPathSegment().equals("getMissedCallCount")) {
		// return getMissedCallCount() + "";
		// } else if (uri.getLastPathSegment().equals("getMissedMsgCount")) {
		// return getMissedMsgCount() + "";
		// }
		// }
		return null;
	}

	@Override
	public Uri insert(Uri arg0, ContentValues arg1) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean onCreate() {
		// TODO Auto-generated method stub
		mShaPre = this.getContext().getSharedPreferences("missedinfo",
				Context.MODE_PRIVATE);
		mEditor = mShaPre.edit();
		return true;
	}

	@Override
	public Cursor query(Uri arg0, String[] arg1, String arg2, String[] arg3,
			String arg4) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int update(Uri uri, ContentValues conVal, String arg2, String[] arg3) {
		// TODO Auto-generated method stub

		if (uri != null && uri.getLastPathSegment() != null) {
			int setNum = 0;
			/*
			 * now set number only allow 1 or 0
			 */
			if (uri.getLastPathSegment().equals("setMissedCallCount")) {
				if (conVal.getAsInteger("missedCallCount") > 0) {
					setNum = mShaPre.getInt("missedCallCount", 0)
							+ conVal.getAsInteger("missedCallCount");
				}
				mEditor.putInt("missedCallCount", setNum);
				mEditor.commit();
			} else if (uri.getLastPathSegment().equals("setMissedMsgCount")) {
				if (conVal.getAsInteger("missedMsgCount") > 0) {
					setNum = mShaPre.getInt("missedMsgCount", 0)
							+ conVal.getAsInteger("missedMsgCount");
				}
				mEditor.putInt("missedMsgCount", setNum);
				mEditor.commit();
			} else if (uri.getLastPathSegment().equals("getMissedCallCount")) {
				return mShaPre.getInt("missedCallCount", 0);
			} else if (uri.getLastPathSegment().equals("getMissedMsgCount")) {
				return mShaPre.getInt("missedMsgCount", 0);
			}
		}
		return 0;
	}
}
