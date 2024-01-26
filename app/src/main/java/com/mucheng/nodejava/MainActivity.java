import android.app.Activity;
import android.os.Bundle;
import com.mucheng.nodejava.core.Context;
import com.mucheng.nodejava.core.Isolate;

import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Isolate isolate = new Isolate();
        Context context = new Context(isolate);
        context.injectJavaBridge();
        context.evaluateScript(readFromAssets("nodeJava.js"));
        context.evaluateScript(readFromAssets("test.js"));
    }

    private String readFromAssets(String path) {
        String result = "";
        try {
            InputStream is = getAssets().open(path);
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            is.close();
            result = new String(buffer, "UTF-8");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return result;
    }
}
