package com.bezdev.pomelo;

import androidx.appcompat.app.AppCompatActivity;

import android.app.NativeActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends NativeActivity
{
    static
    {
        System.loadLibrary("pomelo-native");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        new JNIUtil().IsAppReady();
        //setContentView(R.layout.activity_main);

        // Example of a call to a native method
        //TextView tv = findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    //public native String stringFromJNI();

    public static void Log(String text)
    {
        System.out.println(text);
        return;
    }
}