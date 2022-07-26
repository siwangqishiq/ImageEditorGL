#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include "app.h"
#include "log.h"

static App app;

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_imageeditorgl_NativeBridge_onResize(JNIEnv *env, jobject thiz, jint view_width,jint view_height) {
    app.onResize(view_width , view_height);
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_imageeditorgl_NativeBridge_onRender(JNIEnv *env, jobject thiz) {
    app.onRender();
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_imageeditorgl_NativeBridge_onDestroy(JNIEnv *env, jobject thiz) {
    app.onDestroy();
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_imageeditorgl_NativeBridge_setImageContent(JNIEnv *env, jobject thiz, jstring path,
                                                          jint img_w, jint img_h) {
    jboolean copy;
    auto str = env->GetStringUTFChars(path , &copy);
    app.setImageContent(str , img_w , img_h);
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_imageeditorgl_NativeBridge_scale(JNIEnv *env, jobject thiz, jfloat scale) {
    app.scale(scale);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_panyi_xyz_imageeditorgl_NativeBridge_onTouch(JNIEnv *env, jobject thiz, jint action, jfloat x,
                                                  jfloat y) {
    return app.onTouch(action , x , y);
}
extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_imageeditorgl_NativeBridge_setImageBitmap(JNIEnv *env, jobject thiz,
                                                         jobject image_bitmap) {
    app.setImageBitmap(env ,image_bitmap);
}


extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_imageeditorgl_NativeBridge_onInit(JNIEnv *env, jobject thiz) {
    app.onInit(env);
}