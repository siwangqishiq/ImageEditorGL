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
    pumpMessageQueue();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //do render
    baseImage->render();

    for(auto &pPaint : paintList){
        pPaint->render();
    }//end for each
}

void App::onDestroy() {
    baseImage->onDestroy();

    for(auto &pPaint : paintList){
        pPaint->onDestory();
    }//end for each

    ShaderManager::getInstance().clear();
    Logi("destroy");
}

bool App::onTouch(int action, float x, float y) {
     // Logi("touch %d , (%f , %f)" , action , x , y);
     y = viewHeight - y;
    bool ret = false;
    switch (action) {
        case ACTION_DOWN:
            ret = true;
            break;
        case ACTION_MOVE:
            ret = true;
            break;
        case ACTION_UP:
        case ACTION_CANCEL:
            break;
        default:
            break;
    }
    ActionMessage msg = {action , x ,y};
    messageQueue.push_back(msg);
    return ret;
}

void App::setImageBitmap(JNIEnv *env ,jobject image_bitmap) {
    baseImage->setImageBitmap(env , image_bitmap);
}

void App::onInit(JNIEnv *env) {
    Logi("on init");

    this->env = env;

    Logi("prepare gl config!");
    ShaderManager::getInstance();

    glViewport(0 , 0, viewWidth , viewHeight);
    glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
    glEnable(GL_DEPTH);
//    glEnable(GL_PROGRAM_POINT_SIZE); no found in opengl es version

    baseImage->onInit();

    //test code
    std::shared_ptr<Paint> newPaint = std::make_shared<Paint>(this);
    paintList.push_back(newPaint);
    float x = viewWidth / 2.0f;
    float y = viewHeight / 2.0f;
    fetchCurrentPaint()->addPaintPoint(x , y);
    fetchCurrentPaint()->addPaintPoint(x+ 300.0f , y);
}

void App::handleDownAction(float x, float y) {
    std::shared_ptr<Paint> newPaint = std::make_shared<Paint>(this);
    paintList.push_back(newPaint);

    auto curPaint = fetchCurrentPaint();
    if(curPaint != nullptr){
        curPaint->addPaintPoint(x , y);
    }
}

void App::handleMoveAction(float x, float y) {
    auto curPaint = fetchCurrentPaint();
    if(curPaint != nullptr){
        curPaint->addPaintPoint(x , y);
    }
}

void App::handleUpCancelAction(float x, float y) {
    auto curPaint = fetchCurrentPaint();
    if(curPaint != nullptr){
        curPaint->addPaintPoint(x , y);
    }
}

std::shared_ptr<Paint> App::fetchCurrentPaint() {
    if(paintList.empty()){
        return nullptr;
    }

    return paintList[paintList.size() - 1];
}

//消费事件队列
void App::pumpMessageQueue() {
    while(!messageQueue.empty()){
        handleTouchEvent(messageQueue.front());
        messageQueue.pop_back();
    }//end while
}

void App::handleTouchEvent(ActionMessage &msg) {
    float x = msg.x;
    float y = msg.y;
    switch (msg.action) {
        case ACTION_DOWN:
            handleDownAction(x , y);
            break;
        case ACTION_MOVE:
            handleMoveAction(x , y);
            break;
        case ACTION_UP:
        case ACTION_CANCEL:
            handleUpCancelAction(x , y);
            break;
        default:
            break;
    }//end switch
}





