package work.icu007.testjni01;

import android.os.Bundle;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        TextView textView = new TextView(this);
        JNILoader jniLoader = new JNILoader();
        int a = 5, b = 5;
        int result = jniLoader.addNumbers(5, 5);
        Log.d(TAG, "onCreate: the jniLoader.getNativeString is " + jniLoader.GetNativeString() +
                ", the jniLoader.addNumbers(5, 5) is: " + a + " + " + b + " = "  + result);
        textView.setText(jniLoader.GetNativeString() + ", " + a + " + " + b + " = "  + result);
        ((ViewGroup) findViewById(R.id.main)).addView(textView);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }
}