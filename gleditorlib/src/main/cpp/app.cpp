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

//    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[0][0],normalMatrix[0][1],normalMatrix[0][2]);
//    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[1][0],normalMatrix[1][1],normalMatrix[1][2]);
//    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[2][0],normalMatrix[2][1],normalMatrix[2][2]);
}

void App::onRender() {
    if(!pumpMessageQueue()){//事件处理
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //限定显示范围
//    glScissor(baseImage->x, baseImage->y, baseImage->w , baseImage->h);
//    glEnable(GL_SCISSOR_TEST);

    //do render
    baseImage->render();

    for(auto &pPaint : paintList){
        pPaint->render();
    }//end for each
//    glDisable(GL_SCISSOR_TEST);
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
    EventMessage msg(EVENT_ACTION_DOWN , x , y);
    switch (action) {
        case 0:
            msg.action = EVENT_ACTION_DOWN;
            ret = true;
            break;
        case 1:
            msg.action = EVENT_ACTION_MOVE;
            ret = true;
            break;
        case 2:
        case 3:
            msg.action = EVENT_ACTION_UP;
            break;
        default:
            break;
    }
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
//    std::shared_ptr<Paint> newPaint = std::make_shared<Paint>(this);
//    paintList.push_back(newPaint);
//    float x = viewWidth / 2.0f;
//    float y = viewHeight / 2.0f;
//    fetchCurrentPaint()->addPaintPoint(x , y);
//    fetchCurrentPaint()->addPaintPoint(x+ 300.0f , y);
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
bool App::pumpMessageQueue() {
//    Logi("action queue size : %d" , messageQueue.size());
    while(!messageQueue.empty()){
        bool continueRunning = handleActionEvent(messageQueue.front());
        Logi("continueRunning : %d" , continueRunning);
        if(!continueRunning){
            messageQueue.clear();
            return false;
        }
        messageQueue.pop_back();
    }//end while

    return true;
}

/**
 * 事件处理  return 返回值
 *
 * @param msg
 * @return
 */
bool App::handleActionEvent(EventMessage &msg) {
    Logi("action : %d" , msg.action);
    if(msg.action == EVENT_EXIT){//退出事件
        Logi("action exit!!!!");
        onDestroy();
        return false;
    }

    switch (msg.action) {
        case EVENT_ACTION_DOWN:
            handleDownAction(msg.x , msg.y);
            break;
        case EVENT_ACTION_MOVE:
            handleMoveAction(msg.x , msg.y);
            break;
        case EVENT_ACTION_UP:
        case EVENT_ACTION_CANCEL:
            handleUpCancelAction(msg.x , msg.y);
            break;
        default:
            break;
    }//end switch
    return true;
}

void App::exitApp() {
    EventMessage msg(EVENT_EXIT);
    messageQueue.push_back(msg);
    Logi("exitApp event");
}

int App::exportBitmap(jobject outputBitmap) {
    Logi("export bitmap");

    AndroidBitmapInfo info;
    if(int getInfoResult = AndroidBitmap_getInfo(env , outputBitmap , &info) < 0){
        Loge("get bitmap info error : %d" , getInfoResult);
        return -1;
    }
    Logi("bitmap width %d , height %d , format %d stride %d" , info.width , info.height , info.format , info.stride);

    int outWidth = info.width;
    int outHeight = info.height;

    const int len = 4 * outWidth * outHeight;
    unsigned char *buf = new unsigned char[len];
    glReadPixels(0 , 0 , outWidth , outHeight,
                 GL_RGBA , GL_UNSIGNED_BYTE , buf);

    unsigned char *dataBuf;
    if(int readPixelsResult = AndroidBitmap_lockPixels(env , outputBitmap , (void **)&dataBuf) < 0){
        Loge("readPixelsResult error : %d" , readPixelsResult);
        AndroidBitmap_unlockPixels(env , outputBitmap);
        return -1;
    }

    for(int i = 0 ; i < len ;i++){
        dataBuf[i] = buf[i];
    }//end for i

    AndroidBitmap_unlockPixels(env , outputBitmap);
    Logi("export bitmap end");
    return 0;
}





