//
// Created by panyi on 2022/6/30.
//

#pragma once

#include <iostream>
#include <string>

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <jni.h>
#include "shader.h"
#include "paint.h"
#include "image.h"
#include <memory>

const int ACTION_DOWN = 0;
const int ACTION_UP = 1;
const int ACTION_MOVE = 2;
const int ACTION_CANCEL = 3;

enum Mode{
    IDLE,//空闲态
    PAINT,//绘制
};

struct Point{
    float x;
    float y;
};

class Paint;
class Image;

class App{
public:
    JNIEnv *env = nullptr;

    int viewWidth;
    int viewHeight;

    Mode mode = IDLE;

    glm::mat3 normalMatrix = glm::mat3(1.0f);
    std::shared_ptr<Image> baseImage;
    std::shared_ptr<Paint> paint;

    void setImageContent(std::string path , unsigned  int imgWdith , unsigned int imgHeight);

    void onResize(int width , int height);

    void onInit(JNIEnv *env);

    void onRender();

    void onDestroy();

    bool onTouch(int action ,float x , float y);

    void setImageBitmap(JNIEnv *env , jobject image_bitmap);

};
