package com.example.customjni;

public class ThreadDemo {

    static {
        System.loadLibrary("native-lib");
    }

    public native void normalThread();
    public native void mutexThread();
    public native void callbackFromC();

    public interface OnErrorListener {
        void onError(int code, String msg);
    }

    private OnErrorListener onErrorListener;

    public void setOnErrorListener(OnErrorListener onErrorListener) {
        this.onErrorListener = onErrorListener;
    }

    public void onError(int code, String msg) {
        if (onErrorListener != null) {
            onErrorListener.onError(code, msg);
        }
    }
}
