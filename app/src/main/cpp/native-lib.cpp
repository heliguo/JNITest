#include <jni.h>
#include <string>
#include<android/log.h>

#define LOG_TAG "JNI_TEST"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jnitest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnitest_MainActivity_operateString(JNIEnv *env, jobject thiz,
                                                    jstring java_string) {
    const char *fromJavaString = env->GetStringUTFChars(java_string, NULL);
    if (fromJavaString == NULL) {
        return NULL;
    }
    char buff[128] = {0};
    strcpy(buff, fromJavaString);
    strcat(buff, "+在JNI中增加了字符串");

    env->ReleaseStringUTFChars(java_string, fromJavaString);

    return env->NewStringUTF(buff);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnitest_MainActivity_concatString(JNIEnv *env, jobject thiz, jstring jni,
                                                   jstring concat_string) {
    //jstring 转 char
    const char *a = env->GetStringUTFChars(jni, 0);
    const char *b = env->GetStringUTFChars(concat_string, 0);

    //动态申请一个地址空间
    char *c = (char *) (malloc(strlen(a) + strlen(b)));
    strcpy(c, a);
    strcat(c, b);

    //释放拷贝的内容包含
    // 第一个参数指定一个jstring变量，即是要释放的本地字符串的来源
    // 第二个参数就是要释放的本地字符串
    env->ReleaseStringUTFChars(jni, a);
    env->ReleaseStringUTFChars(concat_string, b);

    //将char* 转jstring
    return env->NewStringUTF(c);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnitest_MainActivity_callJavaMethod(JNIEnv *env, jobject thiz) {

    //通过反射调用java中的方法
    //找class 使用FindClass方法，参数就是要调用的函数的类的完全限定名，但是需要把点换成/
    jclass jclass1 = env->FindClass("com/example/jnitest/MainActivity");

    //获取对应的函数: 参数1:类class,参数2:方法名,参数3:方法签名
    //ps:方法签名的获取:进入build->intermediates->classes->debug目录下,使用javap -s 类的完全限定名,
    // 就能获得函数签名
    // （）中为参数的全限定名如string ： Ljava/lang/String
    // V 代表无返回值
    jmethodID jmethodId = env->GetMethodID(jclass1, "hello", "()V");

    //实例化该class对应的实例  使用AllocObject方法，使用clazz创建该class的实例。
    jobject object = env->AllocObject(jclass1);

    LOGE("native log");
    //调用方法
    env->CallVoidMethod(object, jmethodId);

}