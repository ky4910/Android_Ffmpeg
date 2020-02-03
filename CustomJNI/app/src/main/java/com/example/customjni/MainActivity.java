package com.example.customjni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private ThreadDemo threadDemo;

    public static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        threadDemo = new ThreadDemo();
        threadDemo.setOnErrorListener(new ThreadDemo.OnErrorListener() {
            @Override
            public void onError(int code, String msg) {
                Log.d(TAG, "code is " + code + " msg is " + msg);
            }
        });
    }

    public void normal(View view) {
        threadDemo.normalThread();
    }
    public void mutexThread(View view) {
        threadDemo.mutexThread();
    }
    public void callbackThread(View view) {
        threadDemo.callbackFromC();
    }
}
