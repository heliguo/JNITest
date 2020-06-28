package com.example.jnitest;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        String jni = operateString("java string");
        Log.e("TAG", "操作字符串: " + jni);
        String concat = concatString(jni, "+concatString");
        Log.e("TAG", "合并字符串: " + concat);
        callJavaMethod();
    }

    /**
     * 拼接字符串
     */
    private native String concatString(String jni, String concatString);

    /**
     * 操作字符串
     */
    private native String operateString(String javaString);

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void callJavaMethod();

    public void hello() {
        Log.e("JNI调用Java方法", "hello: native ");
    }
}
