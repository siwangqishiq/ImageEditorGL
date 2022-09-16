#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include "util/log.h"
#include "app.h"
#include "util/common.h"

static App *app = nullptr;

AAssetManager *mAssetManager;

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
    if(app != nullptr){
        app->onRender();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_onDestroy(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        app->exitApp();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_setImageContent(JNIEnv *env, jobject thiz, jstring path,
                                                          jint img_w, jint img_h) {
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_scale(JNIEnv *env, jobject thiz, jfloat scale) {
    Logi("no implement method scale");
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_onTouch(JNIEnv *env, jobject thiz, jint action, jfloat x,
                                                  jfloat y , jfloat x2 , jfloat y2) {
    if(app != nullptr){
        return app->onTouch(action , x , y ,x2 , y2);
    }
    return false;
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_setImageBitmap(JNIEnv *env, jobject thiz,
                                                         jobject image_bitmap) {
    if(app != nullptr){
        app->setImageBitmap(env ,image_bitmap);
    }
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

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_setPaintMode(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        app->changeMode(Mode::PAINT);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_setIdleMode(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        app->changeMode(Mode::IDLE);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_setMosaicMode(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        app->changeMode(Mode::MOSAIC);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_resetImage(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        app->resetImage();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_init(JNIEnv *env, jobject thiz, jobject asset_manager) {
    mAssetManager = AAssetManager_fromJava(env , asset_manager);
    Logi("init %ld " , mAssetManager);
    std::string content = ReadAssetTextFile("paint_frag.glsl");
    Logi("read asset file content : %s" , content.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_setClipMode(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        app->changeMode(Mode::CLIP);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_doClip(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        app->doClip();
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_currentMode(JNIEnv *env, jobject thiz) {
    if(app != nullptr){
        return app->mode;
    }
    return -1;
}

extern "C"
JNIEXPORT void JNICALL
Java_panyi_xyz_gleditorlib_NativeBridge_setCallback(JNIEnv *env, jobject thiz, jobject listener) {
    if(app != nullptr){
        Logi("set mode change listener");
        app->modeChangeListener = env->NewGlobalRef(listener);
        env->DeleteLocalRef(listener);
    }
}