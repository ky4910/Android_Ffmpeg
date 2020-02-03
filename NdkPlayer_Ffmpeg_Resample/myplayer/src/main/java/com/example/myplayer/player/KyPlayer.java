package com.example.myplayer.player;

import android.text.TextUtils;

import com.example.myplayer.listener.KyOnPreparedListener;
import com.example.myplayer.log.MyLog;

public class KyPlayer {

    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("postproc-54");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
    }

    private String source;
    private KyOnPreparedListener kyOnPreparedListener;

    public KyPlayer() {}

    public void setSource(String source) {
        this.source = source;
    }

    public void setKyOnPreparedListener(KyOnPreparedListener kyOnPreparedListener) {
        this.kyOnPreparedListener = kyOnPreparedListener;
    }

    public void prepared() {
        if (TextUtils.isEmpty(source)) {
            MyLog.d("prepared: source is empty!");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_prepared(source);
            }
        }).start();
    }

    public void start() {
        if (TextUtils.isEmpty(source)) {
            MyLog.d("start: source not be empty!");
            return ;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_start();
            }
        }).start();
    }

    public void onCallPrepared() {
        if (kyOnPreparedListener != null) {
            kyOnPreparedListener.onPrepared();
        }
    }

    public native void n_prepared(String source);

    public native void n_start();
}
