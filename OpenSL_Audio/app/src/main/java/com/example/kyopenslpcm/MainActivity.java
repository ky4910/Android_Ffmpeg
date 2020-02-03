package com.example.kyopenslpcm;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    private static final int REQUEST_EXTERNAL_STORAGE = 1;
    private static String[] PERMISSIONS_STORAGE = {
            "android.permission.READ_EXTERNAL_STORAGE",
            "android.permission.WRITE_EXTERNAL_STORAGE" };


    public static final String TAG = "MainActivity";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getAuthentication();

    }

    private void getAuthentication() {
        try {
            //检测是否有写的权限
            int permission = ContextCompat.checkSelfPermission(this,
                    "android.permission.READ_EXTERNAL_STORAGE");
            // Manifest.permission.WRITE_EXTERNAL_STORAGE
            if (permission != PackageManager.PERMISSION_GRANTED) {
                // 没有写的权限，去申请写的权限，会弹出对话框
                ActivityCompat.requestPermissions(this,
                        PERMISSIONS_STORAGE, REQUEST_EXTERNAL_STORAGE);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    public native void stringFromJNI();
    public native void n_playpcm(String url);

    public void play(View view) {

        /* String filePath = Environment.getExternalStorageDirectory()
                .getAbsolutePath() + "/mydream.pcm/"; */

        @SuppressWarnings("all")
        String filePath = "/mnt/sdcard/mydream.pcm";
        Log.e(TAG, "the file path is " + filePath);

        n_playpcm(filePath);
    }
}
