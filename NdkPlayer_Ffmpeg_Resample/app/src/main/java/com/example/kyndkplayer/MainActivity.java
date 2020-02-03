package com.example.kyndkplayer;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;

import com.example.myplayer.listener.KyOnPreparedListener;
import com.example.myplayer.log.MyLog;
import com.example.myplayer.player.KyPlayer;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "MainActivity";

    private KyPlayer kyPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getExternalPermission();

        kyPlayer = new KyPlayer();
        kyPlayer.setKyOnPreparedListener(new KyOnPreparedListener() {
            @Override
            public void onPrepared() {
                MyLog.d("准备就绪，可以开始播放音乐!");
                kyPlayer.start();
            }
        });
    }

    public void getExternalPermission() {
        if (Build.VERSION.SDK_INT >= 23) {
            if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission
                    .READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(MainActivity.this, new String[]
                        {Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
            }
            if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission
                    .READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(MainActivity.this, new String[]
                        {Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
            }
        }
    }

    public void begin(View view) {

        kyPlayer.setSource("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3");

        @SuppressWarnings("all")
        String filePath = "/mnt/sdcard/Sign_DEAMN.mp3";
        // String filePath = "/mnt/sdcard/short_01.mp3";
         kyPlayer.setSource(filePath);

        kyPlayer.prepared();
    }
}
