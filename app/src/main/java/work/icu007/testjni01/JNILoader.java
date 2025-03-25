package work.icu007.testjni01;

/*
 * Author: Charlie Liao
 * Time: 2025/3/24-12:03
 * E-mail: charlie.liao@icu007.work
 */

public class JNILoader {
    public native String GetNativeString();

    public native int addNumbers(int a, int b);

    static {
        System.loadLibrary("JNILoader");
    }
}
