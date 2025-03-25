# 从0到1构建一个JNI项目

## 创建第一个JNI Demo项目

### 创建新项目

1. 打开 Android Studio，选择 "Create New Project"
2. 选择 "Empty Views Activity" 模板
3. 填写应用信息（名称、包名等）
4. 完成创建

### 了解JNI项目架构

创建好的项目主要包含以下重要部分：

```
app/
├── src/main/
│   ├── java/      // Java 代码
│   ├── cpp/       // C++ 代码
│   │   ├── JNILoader.cpp   // 包含原生方法实现
│   │   ├── CMakeLists.txt   // CMake 构建脚本
│   └── AndroidManifest.xml
├── build.gradle   // 应用级构建配置
```

### 理解默认生成的代码

1. **Java 端 (MainActivity.java)**:

```java
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
        Log.d(TAG, "onCreate: the jniLoader.getNativeString is " + jniLoader.GetNativeString());
        textView.setText(jniLoader.GetNativeString());
        ((ViewGroup) findViewById(R.id.main)).addView(textView);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }
}
```

2. **C++ 端 (JNILoader.cpp) 不会默认生成需要自己在新建cpp文件夹后新增JNILoader.cpp**:

```c++
#include <string>
#include <jni.h>
/* Header for class work_icu007_testjni01_JNILoader */

// 如果没有定义 _Included_work_icu007_testjni01_JNILoader 就定义一个
// 避免同一头文件被多次包含而导致的符号重定义错误。
#ifndef _Included_work_icu007_testjni01_JNILoader
#define _Included_work_icu007_testjni01_JNILoader
// #ifdef __cplusplus 这个指令检查是否在 C++ 编译环境中。
// 如果是，编译器会将 __cplusplus 宏定义为一个正值（通常为 199711L 或更高），表示 C++ 语言特性可用。
#ifdef __cplusplus
// extern "C": 当代码被 C++ 编译器编译时，函数名会被修改（称为名称修饰），以支持函数重载。
// 而 extern "C" 告诉编译器按照 C 的方式处理名称，这样可以防止名称修饰，从而使得 C 和 C++ 之间的函数调用变得可行。
extern "C" {
#endif
/*
 * Class:     work_icu007_testjni01_JNILoader
 * Method:    GetNativeString
 * Signature: ()Ljava/lang/String;
 */

 // JNIEXPORT 用于指示该函数是 JNI 导出函数，JNICALL 表示函数使用标准调用约定。
 // jstring: 这是 JNI 提供的一种数据类型，表示 Java 中的 String 对象。
 // 通过 jstring，C/C++ 代码可以与 Java 字符串进行交互。
JNIEXPORT jstring JNICALL Java_work_icu007_testjni01_JNILoader_GetNativeString
  (JNIEnv *env, jobject) {
    std::string hello = "hello from native JNI";
    return env->NewStringUTF(hello.c_str());
}

#ifdef __cplusplus
}
#endif
#endif
```

3. **CMakeLists.txt**:

```cmake
# 最小CMake版本要求
cmake_minimum_required(VERSION 3.4.1)

# 设置库名称
add_library(# 设置库的名字
			JNILoader 
			# 设置JNILoader.so 为共享库
			SHARED 
			# 提供源文件
			JNILoader.cpp)

# 找到Android NDK提供的log库
find_library(# 设置path名称
			log-lib 
			# 指定日志库的名称
			log)

# 将库与log库链接
target_link_libraries(# 制定目标库
					JNILoader 
					# 链接日志库到目标库
					${log-lib})
```

4. **build.gradle**

```groovy
plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace 'work.icu007.testjni01'
    compileSdk 34

    defaultConfig {
        applicationId "work.icu007.testjni01"
        minSdk 24
        targetSdk 34
        versionCode 1
        versionName "1.0"

        testInstrumentationRunner "androidx.test.runner.AndroidJUnitRunner"
        
        // 添加ndk支持
        externalNativeBuild {
            cmake {
                cppFlags ""
            }
        }

        // 限制ABI
        // 限制应用只为指定的 ABI 构建本地库。在这个例子中，应用将只为 arm64-v8a 和 armeabi-v7a 这两种 ABI 构建。
        ndk {
            //noinspection ChromeOsAbiSupport
            abiFilters 'arm64-v8a', 'armeabi-v7a'
        }
    }

    buildTypes {
        release {
            ...
        }
    }

    // 指定cmake文件路径
    externalNativeBuild{
        cmake {
            path "src/main/cpp/CMakeLists.txt"
        }
    }
    compileOptions {
        /*...*/
    }
}

dependencies {
	/*...*/
}
```

### 增加新功能

新增一个计算两数之和的函数：

修改 `JNILoader.cpp`

```cpp
#include <string>
#include <jni.h>
#include <android/log.h>

#define TAG "NativeCode"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
/* Header for class work_icu007_testjni01_JNILoader */

// 如果没有定义 _Included_work_icu007_testjni01_JNILoader 就定义一个
// 避免同一头文件被多次包含而导致的符号重定义错误。
#ifndef _Included_work_icu007_testjni01_JNILoader
#define _Included_work_icu007_testjni01_JNILoader
// #ifdef __cplusplus 这个指令检查是否在 C++ 编译环境中。
// 如果是，编译器会将 __cplusplus 宏定义为一个正值（通常为 199711L 或更高），表示 C++ 语言特性可用。
#ifdef __cplusplus
// extern "C": 当代码被 C++ 编译器编译时，函数名会被修改（称为名称修饰），以支持函数重载。
// 而 extern "C" 告诉编译器按照 C 的方式处理名称，这样可以防止名称修饰，从而使得 C 和 C++ 之间的函数调用变得可行。
extern "C" {
#endif
/*
 * Class:     work_icu007_testjni01_JNILoader
 * Method:    GetNativeString
 * Signature: ()Ljava/lang/String;
 */

 // JNIEXPORT 用于指示该函数是 JNI 导出函数，JNICALL 表示函数使用标准调用约定。
 // jstring: 这是 JNI 提供的一种数据类型，表示 Java 中的 String 对象。
 // 通过 jstring，C/C++ 代码可以与 Java 字符串进行交互。
JNIEXPORT jstring JNICALL Java_work_icu007_testjni01_JNILoader_GetNativeString
  (JNIEnv *env, jobject) {
    std::string hello = "hello from native JNI";
    return env->NewStringUTF(hello.c_str());
}

 // 新增方法
JNIEXPORT jint JNICALL Java_work_icu007_testjni01_JNILoader_addNumbers
  (JNIEnv *env, jobject , jint a, jint b) {
    LOGD("a: %d, b: %d, a + b = %d", a, b, a + b);
    return a + b;
}

#ifdef __cplusplus
}
#endif
#endif
```

修改 `JNILoader.java`

```java
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
```

修改 `MainActivity`

```cpp
public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        TextView textView = new TextView(this);
        JNILoader jniLoader = new JNILoader();
        // 新增
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
```



## 重要概念说明

1. **JNI 数据类型映射**:
    - `jint` 对应 Java 的 `int`
    - `jstring` 对应 Java 的 `String`
    - `jobject` 对应 Java 的对象引用

2. **JNI 函数命名规则**:
    - 遵循 `Java_包名_类名_方法名` 格式
    - 包名中的点(.)用下划线(_)替代

3. **JNIEnv**:
    - 提供大量函数用于操作 Java 对象
    - 每个线程都有自己的 JNIEnv 实例

## 调试技巧

- 使用 `__android_log_print()` 在 logcat 中打印信息:

```cpp
#include <android/log.h>

#define TAG "NativeCode"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

// 在函数中使用
LOGD("Value of a: %d", a);
```

## tips

> 生成JNI对应 c 代码的头文件 可以直接使用下述两条代码生成：（一般情况下在项目的`app/src/main/java`目录下执行， work\icu007\testjni01 是自己项目包名）
>
> `javac .\work\icu007\testjni01\JNILoader.java`
>
> `javah work.icu007.testjni01.JNILoader`