//
// Created by panyi on 2022/6/30.
//

#include "app.h"
#include "log.h"

//#include <android/imagedecoder.h>
//#include <android/bitmap.h>

#include "stb_image.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "math.hpp"
#include <android/bitmap.h>
#include <memory>

void App::onResize(int width,int height) {
    baseImage = std::make_shared<Image>(this);

    viewWidth = width;
    viewHeight = height;
    Logi("on onResize : %d x %d" , viewWidth , viewHeight);

    //重置归一化矩阵
    normalMatrix[0][0] = 2.0f/(float)viewWidth;
    normalMatrix[0][1] = 0.0f;
    normalMatrix[0][2] = 0.0f;

    normalMatrix[1][0] = 0.0f;
    normalMatrix[1][1] = 2.0f / (float)viewHeight;
    normalMatrix[1][2] = 0.0f;

    normalMatrix[2][0] = -1.0f;
    normalMatrix[2][1] = -1.0f;
    normalMatrix[2][2] = 1.0f;

    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[0][0],normalMatrix[0][1],normalMatrix[0][2]);
    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[1][0],normalMatrix[1][1],normalMatrix[1][2]);
    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[2][0],normalMatrix[2][1],normalMatrix[2][2]);
}

void App::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //do render
    baseImage->render();
    paint->render();
}

void App::onDestroy() {
    baseImage->onDestroy();
    Logi("destroy");
}

bool App::onTouch(int action, float x, float y) {
    // Logi("touch %d , (%f , %f)" , action , x , y);
    return true;
}

void App::setImageBitmap(JNIEnv *env ,jobject image_bitmap) {
    baseImage->setImageBitmap(env , image_bitmap);
}

void App::onInit(JNIEnv *env) {
    Logi("on init");
    Logi("prepare gl config!");
    this->env = env;

    paint = std::make_shared<Paint>(this);

    glViewport(0 , 0, viewWidth , viewHeight);
    glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
    glEnable(GL_DEPTH);

    baseImage->onInit();
    paint->onInit();
}



