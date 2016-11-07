package com.luke.app.sales.service;

import android.Manifest;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Environment;
import android.text.TextUtils;
import android.util.Log;
import android.widget.RemoteViews;
import android.widget.Toast;

import com.alibaba.fastjson.JSON;
import com.lidroid.xutils.HttpUtils;
import com.lidroid.xutils.exception.HttpException;
import com.lidroid.xutils.http.ResponseInfo;
import com.lidroid.xutils.http.callback.RequestCallBack;
import com.luke.app.R;
import com.luke.app.a;
import com.luke.app.commons.utils.AuthorityUtil;
import com.luke.app.sales.data.UpdateJsonCls;
import com.luke.app.sales.ui.HotelList.HotelListActivity;

import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.xutils.common.Callback;
import org.xutils.http.RequestParams;
import org.xutils.x;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.math.BigInteger;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.MessageDigest;

import static com.luke.app.sales.service.UpdateAppSerivceBase.getAPPVersion;
import static com.luke.app.sales.service.UpdateAppSerivceBase.getFileMD5;

/**
 * Created by Administrator on 2016/10/21.
 */

public class UpdateAppService {

    private static final String UPDATE_JSON_URL = "http://zjjd.myhiott.com:8081/upgrade/default/apks/lukephone/update.json";
    private static final String UPDATE_SAVE_FILE = Environment.getExternalStorageDirectory().getPath() + "/Download/lukephone.apk";

    private Context mCon;
    private UpdateAppSerivceBase mBase;

    private void delFile(UpdateJsonCls update) {
        File file = new File(update.getSaveFile());
        if (file.exists()) file.delete();
    }


    public UpdateAppService(Context context) {
        mCon = context;
        mBase = new UpdateAppSerivceBase(new UpdateAppSerivceBase.CallbackConvert() {
            @Override
            public Object convertResult(String msg) throws Exception {
                UpdateJsonCls updateJson = JSON.parseObject(msg, UpdateJsonCls.class);
                updateJson.setOldVersion(getAPPVersion(mCon));
                updateJson.setSaveFile(UPDATE_SAVE_FILE);
                return updateJson;
            }
        });
    }

    // 检测升级文件
    public void downloadDescription(final UpdateAppSerivceBase.CallbackBase callback) {
        mBase.startDownDescription(callback, UPDATE_JSON_URL);
    }

    // 取消下载描述文件
    public void cancelDownDescription() {
        mBase.cancelDownDescription();
    }

    // 是否
    public boolean isNeedUpdate(UpdateJsonCls updateJson) {
        boolean isNeedUpdate = false;
        if (updateJson != null) isNeedUpdate = updateJson.isNeedUpdate();

        if (!isNeedUpdate) delFile(updateJson);

        return isNeedUpdate;
    }


    //    private Button update,cancel;
    private static int NOTIFY_ID = 10;
    private NotificationManager manager;
    private Notification notif;

    private void startNotify() {
        Intent intent = new Intent();

        PendingIntent pIntent = PendingIntent.getActivity(mCon, 0, intent, 0);
        manager = (NotificationManager) mCon.getSystemService(mCon.NOTIFICATION_SERVICE);
        notif = new Notification();
        notif.icon = R.mipmap.ic_launcher;
        notif.tickerText = "下载进度通知";
        //通知栏显示所用到的布局文件
        notif.contentView = new RemoteViews(mCon.getPackageName(), R.layout.notify_download);
        notif.contentIntent = pIntent;
        manager.notify(NOTIFY_ID, notif);
    }

    private void setNotify(int percent) {
        notif.contentView.setTextViewText(R.id.content_view_text1, percent + "%");
        notif.contentView.setProgressBar(R.id.content_view_progress, 100, percent, false);
        manager.notify(NOTIFY_ID, notif);
    }

    private void stopNotify() {
        manager.cancel(NOTIFY_ID);
    }

    public void startDownload(final UpdateJsonCls updateJson, final UpdateAppSerivceBase.DownFileCallback callback) {
        startNotify();
        mBase.startDownload(updateJson.getAppUrl(), updateJson.getSaveFile(), Long.parseLong(updateJson.getSize()), updateJson, new UpdateAppSerivceBase.DownFileCallback() {
            @Override
            public boolean isCheckOk() throws Exception {
                // md5检测
                String md5 = null;

                try {
                    md5 = getFileMD5(new File(updateJson.getSaveFile()));
                } catch (Exception e) {
                    e.printStackTrace();
                    throw new Exception("读取文件MD5出错");
                }

                if (!updateJson.getMd5().toLowerCase().equals(md5)) {
                    throw new Exception("MD5校验出错");
                }

                return true;
            }

            @Override
            public void onRoomNotEnough() {
                callback.onRoomNotEnough();
                stopNotify();
            }

            @Override
            public void onAlreadyDown() {
                callback.onAlreadyDown();
                stopNotify();
            }

            @Override
            public void onLoading(long total, long current, boolean isUploading) {
                callback.onLoading(total, current, isUploading);
                if (total > 0)
                    setNotify((int) (current * 100 / total));
            }

            @Override
            public void onFailed(String err) {
                callback.onFailed(err);

                delFile(updateJson);
                stopNotify();
            }

            @Override
            public void onSuccess(Object object) {
                callback.onSuccess(object);
                stopNotify();
            }
        });
    }

    public void startInstallAPP(UpdateJsonCls updateJson, final UpdateAppSerivceBase.CallbackBase callback) {
        mBase.startInstallAPP(mCon, updateJson.getSaveFile(), callback);
    }

}
