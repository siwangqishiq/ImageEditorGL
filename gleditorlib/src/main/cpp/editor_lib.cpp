#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include "app.h"
#include "log.h"

static App *app = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_onResize(JNIEnv *env, jobject thiz, jint view_width,jint view_height) {
    if(app != nullptr){
        delete app;
    }

    app = new App();
    app->onResize(view_width , view_height);
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_onRender(JNIEnv *env, jobject thiz) {
    app->onRender();
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_onDestroy(JNIEnv *env, jobject thiz) {
    app->exitApp();
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_setImageContent(JNIEnv *env, jobject thiz, jstring path,
                                                          jint img_w, jint img_h) {
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_scale(JNIEnv *env, jobject thiz, jfloat scale) {
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_onTouch(JNIEnv *env, jobject thiz, jint action, jfloat x,
                                                  jfloat y) {
    return app->onTouch(action , x , y);
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_setImageBitmap(JNIEnv *env, jobject thiz,
                                                         jobject image_bitmap) {
    app->setImageBitmap(env ,image_bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_onInit(JNIEnv *env, jobject thiz) {
    app->onInit(env);
}

//extern "C"
//JNIEXPORT jobject JNICALL
//Java_panyi_xyz_gleditorlib_NativeBridge_exportBitmap(JNIEnv *env, jobject thiz) {
//    if(app != nullptr){
//        return app->exportBitmap();
//    }
//    return env->NewGlobalRef(nullptr);
//}

extern "C"
JNIEXPORT jint JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_exportBitmap(JNIEnv *env, jobject thiz, jobject output) {
   if(app != nullptr){
       return app->exportBitmap(output);
   }
   return -1;
}
extern "C"
JNIEXPORT jint JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_originImageWidth(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        return app->w;
    }
    return 0;
}


extern "C"
JNIEXPORT jint JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_originImageHeight(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        return app->h;
    }
    return 0;
}