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

JNIEXPORT jint JNICALL Java_work_icu007_testjni01_JNILoader_addNumbers
  (JNIEnv *env, jobject , jint a, jint b) {
    LOGD("a: %d, b: %d, a + b = %d", a, b, a + b);
    return a + b;
}

#ifdef __cplusplus
}
#endif
#endif