package com.example.kyndkplayer;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.myplayer.Demo;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "MainActivity";
    private Demo demo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Demo demo = new Demo();
        demo.testFfmpeg();

        // Example of a call to a native method
         TextView tv = findViewById(R.id.sample_text);
         tv.setText(demo.stringFromJNI());
    }
}
