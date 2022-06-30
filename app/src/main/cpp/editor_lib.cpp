#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_panyi_xyz_imageeditorgl_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_panyi_xyz_imageeditorgl_NativeBridge_sayHello(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello World";
    return env->NewStringUTF(hello.c_str());
}

