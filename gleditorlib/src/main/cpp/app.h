//
// Created by panyi on 2022/6/30.
//

#pragma once

#include <iostream>
#include <string>

#include "gl.h"
#include <jni.h>
#include "shader.h"
#include "paint.h"
#include "image.h"
#include "common.h"
#include "origin_image.h"
#include <memory>

enum Mode{
    IDLE,//空闲态
    PAINT,//绘制
};

class Paint;
class Image;
class OriginImage;

class App{
public:
    JNIEnv *env = nullptr;

    int viewWidth;
    int viewHeight;

    Mode mode = IDLE;

    glm::mat3 normalMatrix = glm::mat3(1.0f);

    std::shared_ptr<OriginImage> originImage;
    std::shared_ptr<Image> baseImage;

    //绘制组件集合
    std::vector<std::shared_ptr<Paint>> paintList = std::vector<std::shared_ptr<Paint>>();

    void onResize(int width , int height);

    void onInit(JNIEnv *env);

    void onRender();

    void onDestroy();

    bool onTouch(int action ,float x , float y);

    bool handleActionEvent(EventMessage &msg);

    void setImageBitmap(JNIEnv *env , jobject image_bitmap);

    //主动退出应用
    void exitApp();

    //导出bitmap
    int exportBitmap(jobject outputBitmap);

    float x = 0.0f;
    float y = 0.0f;
    float w = 1.0f;
    float h = 1.0f;

    float vertexData[6 * 5] = {
            x       ,  y         , 1.0f , 0.0f , 1.0f,
            x + w , y + h   , 1.0f , 1.0f , 0.0f,
            x        , y + h   , 1.0f , 0.0f , 0.0f,
            x       ,  y         , 1.0f , 0.0f , 1.0f,
            x+ w  , y          ,1.0f , 1.0f , 1.0f,
            x + w , y + h   , 1.0f , 1.0f , 0.0f,
    };
private:
    Shader shader;

    unsigned int vbo;

    //事件消息队列
    std::vector<EventMessage> messageQueue = std::vector<EventMessage>();


    //处理按下事件
    void handleDownAction(float x , float y);

    //处理移动事件
    void handleMoveAction(float x , float y);

    //处理取消事件
    void handleUpCancelAction(float x , float y);

    std::shared_ptr<Paint> fetchCurrentPaint();

    //处理事件消息队列
    bool pumpMessageQueue();

    void updateVertexData();

    void createShader();

    void initVertex();

    void renderMainView();

    void resetNormalMatrix(float width, float height);
};
