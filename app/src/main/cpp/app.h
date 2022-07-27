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
#include "common.h"
#include <memory>

enum Mode{
    IDLE,//空闲态
    PAINT,//绘制
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

    void onResize(int width , int height);

    void onInit(JNIEnv *env);

    void onRender();

    void onDestroy();

    bool onTouch(int action ,float x , float y);

    void handleTouchEvent(ActionMessage &msg);

    void setImageBitmap(JNIEnv *env , jobject image_bitmap);
private:
    //事件消息队列
    std::vector<ActionMessage> messageQueue = std::vector<ActionMessage>();

    //绘制组件集合
    std::vector<std::shared_ptr<Paint>> paintList = std::vector<std::shared_ptr<Paint>>();

    //处理按下事件
    void handleDownAction(float x , float y);

    //处理移动事件
    void handleMoveAction(float x , float y);

    //处理取消事件
    void handleUpCancelAction(float x , float y);

    std::shared_ptr<Paint> fetchCurrentPaint();

    //处理事件消息队列
    void pumpMessageQueue();
};
