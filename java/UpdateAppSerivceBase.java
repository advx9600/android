package com.luke.app.sales.service;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.StatFs;
import android.text.TextUtils;
import android.util.Log;

import org.xutils.http.RequestParams;
import org.xutils.x;

import java.io.File;
import java.io.FileInputStream;
import java.math.BigInteger;
import java.security.MessageDigest;

/**
 * Created by Administrator on 2016/11/1.
 */

public class UpdateAppSerivceBase {

    private static final String TAG = "UpdateAppServiceBase";

    public interface CallbackBase {
        void onFailed(String err);

        void onSuccess(Object object);
    }

    public interface Callback extends CallbackBase {
        void onLoading(long total, long current, boolean isUploading);
    }

    public interface DownFileCallback extends Callback {
        boolean isCheckOk() throws Exception;

        void onRoomNotEnough();
    }

    public interface CallbackConvert {
        Object convertResult(String msg) throws Exception;
    }

    private org.xutils.common.Callback.Cancelable mCancelableDes; // 取消下载升级描述
    private org.xutils.common.Callback.Cancelable mCancelableFile; // 取消下载文件

    private CallbackConvert mConvert;

    public UpdateAppSerivceBase(CallbackConvert convert) {
        mConvert = convert;
    }

    // 检测升级文件JSON
    public void startDownDescription(final CallbackBase callback, String urlFile) {
        try {
            RequestParams params = new RequestParams(urlFile);
            //设置断点续传
            params.setAutoResume(false);
            mCancelableDes = x.http().get(params, new org.xutils.common.Callback.ProgressCallback<String>() {
                @Override
                public void onWaiting() {

                }

                @Override
                public void onStarted() {

                }

                @Override
                public void onLoading(long total, long current, boolean isDownloading) {
                }

                @Override
                public void onSuccess(String result) {
                    Log.d(TAG, "return json:" + result);
                    if (TextUtils.isEmpty(result)) {
                        callback.onFailed("返回数据为空");
                        return;
                    }

                    Object updateJson = null;

                    try {
                        updateJson = mConvert.convertResult(result);
                    } catch (Exception e) {
                        e.printStackTrace();
                        callback.onFailed("数据格式转换错误");
                        return;
                    }

                    Log.d(TAG, "download description success");
                    callback.onSuccess(updateJson);
                }

                @Override
                public void onError(Throwable ex, boolean isOnCallback) {
                    ex.printStackTrace();
                    callback.onFailed("网络错误或远程文件不存在");
                }

                @Override
                public void onCancelled(CancelledException cex) {

                }

                @Override
                public void onFinished() {
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
            callback.onFailed("下载数据格式有问题");
            return;
        }
    }

    // 取消下载描述文件
    public void cancelDownDescription() {
        cancelDownload(mCancelableDes);
    }

    private void cancelDownload(org.xutils.common.Callback.Cancelable cancelable) {
        if (cancelable != null && !cancelable.isCancelled()) {
            cancelable.cancel();

            if (cancelable == mCancelableDes) {
                mCancelableDes = null;
            } else if (cancelable == mCancelableFile) {
                mCancelableFile = null;
            }
        }
    }

    // 开始下载文件
    public void startDownload(final String urlFile, final String locFile, final long fileSize, final Object successCallbackObj, final DownFileCallback callback) {
        if (TextUtils.isEmpty(urlFile) || TextUtils.isEmpty(locFile)) {
            callback.onFailed("下载文件或者保存文件为空");
            return;
        }

        try {
            File file = new File(getFileDir(locFile));
            if (!file.exists()) {
                file.mkdirs();
            }
        } catch (Exception e) {
            e.printStackTrace();
            callback.onFailed("无法创建目录");
            return;
        }

        // 计算剩余空间大小
        try {
            if (fileSize > 0) {
                StatFs sf = new StatFs(getFileDir(locFile));
                long blockSize = 0;
                long availCount = 0;
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
                    blockSize = sf.getBlockSizeLong();
                    availCount = sf.getAvailableBlocksLong();
                } else {
                    blockSize = sf.getBlockSize();
                    availCount = sf.getAvailableBlocks();
                }

                if (blockSize > 0 && availCount > 0) {
                    File file = new File(locFile);
                    long reduce = 0;
                    if (file.exists() && file.isFile()) {
                        reduce = file.length();
                    }
                    if (blockSize * availCount < fileSize + 20 * 1024 * 1024 - reduce) {
                        callback.onRoomNotEnough();
                        return;
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            callback.onFailed("检测存储空间出错");
            return;
        }


        Log.i(TAG, "startDownload ap");

        try {
            RequestParams params = new RequestParams(urlFile);
            //设置断点续传
            params.setAutoResume(true);
            params.setSaveFilePath(locFile);
            mCancelableFile = x.http().get(params, new org.xutils.common.Callback.ProgressCallback<File>() {
                @Override
                public void onWaiting() {

                }

                @Override
                public void onStarted() {

                }

                @Override
                public void onLoading(long total, long current, boolean isDownloading) {
                    callback.onLoading(total, current, isDownloading);
                }

                @Override
                public void onSuccess(File result) {
                }

                @Override
                public void onError(Throwable ex, boolean isOnCallback) {
                    ex.printStackTrace();
                }

                @Override
                public void onCancelled(CancelledException cex) {

                }

                @Override
                public void onFinished() {
                    File file = new File(locFile);
                    if (!file.exists()) {
                        callback.onFailed("无法下载文件到指定位置");
                        return;
                    }

                    if (fileSize > 0) {
                        if (fileSize != file.length()) {
                            callback.onFailed("检测下载文件大小出错");
                            return;
                        }
                    }

                    try {
                        if (!callback.isCheckOk()) {
                            callback.onFailed("检测文件出错");
                            return;
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        callback.onFailed(e.getMessage());
                        return;
                    }

                    callback.onSuccess(successCallbackObj);
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
            callback.onFailed("下载数据格式有问题");
            return;
        }
    }

    private String getFileDir(String filePath) {
        if (TextUtils.isEmpty(filePath)) {
            return "";
        }

        File file = new File(filePath);
        return file.getParent();
    }

    public void cancelDownFile() {
        cancelDownload(mCancelableFile);
    }

    public static void startInstallAPP(Context context, String filePath, CallbackBase callback) {

        if (TextUtils.isEmpty(filePath)) {
            callback.onFailed("安装APP文件名为空");
            return;
        }

        try {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.setDataAndType(Uri.parse("file://" + filePath), "application/vnd.android.package-archive");
            context.startActivity(intent);
            callback.onSuccess(filePath);
        } catch (Exception e) {
            e.printStackTrace();
            callback.onFailed("调用安装程序出错,下载位置为" + filePath + ",请手动安装");
            return;
        }
    }

    public static String getFileMD5(File file) {
        if (!file.isFile()) {
            return null;
        }
        MessageDigest digest = null;
        FileInputStream in = null;
        byte buffer[] = new byte[1024];
        int len;
        try {
            digest = MessageDigest.getInstance("MD5");
            in = new FileInputStream(file);
            while ((len = in.read(buffer, 0, 1024)) != -1) {
                digest.update(buffer, 0, len);
            }
            in.close();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
        BigInteger bigInt = new BigInteger(1, digest.digest());
        return bigInt.toString(16);
    }

    public static String getAPPVersion(Context ctx) {
        PackageManager manager = ctx.getPackageManager();
        try {
            PackageInfo info = manager.getPackageInfo(ctx.getPackageName(), 0);
//            return info.versionName + "." + info.versionCode;
            return info.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            // TODO Auto-generated catch blockd
            e.printStackTrace();
        }
        return "";
    }
}
