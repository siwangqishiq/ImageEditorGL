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
    viewWidth = width;
    viewHeight = height;
//    resetNormalMatrix();

    baseImage = std::make_shared<Image>(this);
    originImage = std::make_shared<OriginImage>(this);

    Logi("viewWidth %d x %d" ,viewWidth ,viewHeight);
    Logi("on onResize : %f , %f , %f x %f" ,x ,y , w , h);
}


void App::onInit(JNIEnv *env) {
    Logi("on init");
    this->env = env;

    Logi("prepare gl config!");
    ShaderManager::getInstance();
    createShader();

    glViewport(0 , 0, viewWidth , viewHeight);
    glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
    glEnable(GL_DEPTH);

    baseImage->onInit();

    imageOriginWidth = static_cast<float>(baseImage->imgWidth);
    imageOriginHeight = static_cast<float>(baseImage->imgHeight);
    originImage->init(imageOriginWidth , imageOriginHeight);
    //重置归一化矩阵
    resetNormalMatrix(viewWidth , viewHeight);
    calculateFitViewTransMatrix();
    updateVertexData(x , y , w , h);
    initVertex();
}

void App::onRender() {
    if(!pumpMessageQueue()){//事件处理
        return;
    }

    //限定显示范围
//    glScissor(baseImage->x, baseImage->y, baseImage->w , baseImage->h);
//    glEnable(GL_SCISSOR_TEST);
//    glDisable(GL_SCISSOR_TEST);

    //do render
    originImage->renderToFrameBuffer();

    //将离屏缓冲区内容作为纹理 渲染到屏幕
    glViewport(0 , 0, viewWidth , viewHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    renderMainView();
}

void App::onDestroy() {
    originImage->destroy();
    baseImage->onDestroy();

    for(auto &pPaint : paintList){
        pPaint->onDestory();
    }//end for each

    GLuint bufferIds[] = {vbo};
    glDeleteBuffers(1 , bufferIds);

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

void App::resetNormalMatrix(float width , float height) {
    normalMatrix[0][0] = 2.0f/width;
    normalMatrix[0][1] = 0.0f;
    normalMatrix[0][2] = 0.0f;

    normalMatrix[1][0] = 0.0f;
    normalMatrix[1][1] = 2.0f / height;
    normalMatrix[1][2] = 0.0f;

    normalMatrix[2][0] = -1.0f;
    normalMatrix[2][1] = -1.0f;
    normalMatrix[2][2] = 1.0f;
}

void App::updateVertexData(float _x , float _y , float _w , float _h) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;

    vertexData[0 * 5 + 0] = x;
    vertexData[0 * 5 + 1] = y;

    vertexData[1 * 5 + 0] = x + w;
    vertexData[1 * 5 + 1] = y + h;

    vertexData[2 * 5 + 0] = x;
    vertexData[2 * 5 + 1] = y + h;

    vertexData[3 * 5 + 0] = x;
    vertexData[3 * 5 + 1] = y;

    vertexData[4 * 5 + 0] = x + w;
    vertexData[4 * 5 + 1] = y;

    vertexData[5 * 5 + 0] = x + w;
    vertexData[5 * 5 + 1] = y + h;
}

void App::createShader() {
    std::string vtxSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "layout(location = 0) in vec3 a_position;\n"
                                     "layout(location = 1) in vec2 a_texture;\n"
                                     "\n"
                                     "uniform mat3 transMat;\n"
                                     "\n"
                                     "out vec2 vUv;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    gl_Position = vec4(transMat * a_position ,1.0f);\n"
                                     "    vUv = vec2(a_texture.x  , a_texture.y);\n"
                                     "}");

    std::string frgSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "precision mediump float;\n"
                                     "\n"
                                     "uniform sampler2D mainTexture;\n"
                                     "\n"
                                     "in vec2 vUv;\n"
                                     "out vec4 outColor;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    vec4 originColor = texture(mainTexture , vUv);\n"
                                     "    outColor = originColor.rgba;\n"
                                     "}");
    shader = ShaderManager::getInstance().fetchShader("view_shader" , vtxSrc , frgSrc);
}

void App::initVertex() {
    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    vbo = bufferIds[0];

    glBindBuffer(GL_ARRAY_BUFFER , vbo);
    glBufferData(GL_ARRAY_BUFFER ,  6 *5 * sizeof(float) , vertexData , GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
}

void App::renderMainView() {
//    Logi("render main view shader name : %d" , shader.programId);
    shader.useShader();

    glm::mat3 matrix =normalMatrix * scaleMatrix * moveMatrix;
    shader.setIUniformMat3("transMat" , matrix);

    glBindBuffer(GL_ARRAY_BUFFER ,vbo);

    glVertexAttribPointer(0 , 3 , GL_FLOAT , false , 5 * sizeof(float) , 0);
    glVertexAttribPointer(1 , 2 , GL_FLOAT , false , 5 * sizeof(float) , (void*)(3 * sizeof(float)));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, originImage->contentTextureId);
//    glBindTexture(GL_TEXTURE_2D, baseImage->textureId);
    shader.setUniformInt("mainTexture" , 1);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES , 0 , 6);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER ,0);
}

//计算缩放矩阵  以保持图片显示宽高比
void App::calculateFitViewTransMatrix() {
    float left = 0.0f;
    float bottom = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    int maxViewSize = viewWidth >= viewHeight?viewWidth : viewHeight;
    int maxImageSize = imageOriginWidth >= imageOriginHeight?imageOriginWidth : imageOriginHeight;

    if(maxViewSize >= maxImageSize){
        width = imageOriginWidth;
        height = imageOriginHeight;
        left = viewWidth / 2.0f - width / 2.0f;
        bottom = viewHeight / 2.0f - height / 2.0f;
    }else{
        const float ratio = imageOriginWidth / imageOriginHeight;
        if(imageOriginWidth >= imageOriginHeight){
            width = viewWidth;
            height = viewWidth / ratio;

            left = 0.0f;
            bottom = viewHeight / 2.0f - height / 2.0f;
        }else{
            height = viewHeight;
            width = ratio * viewHeight;

            left = viewWidth / 2.0f - width / 2.0f;
            bottom = 0.0f;
        }
    }

    float sx = width / viewWidth;
    float sy = height / viewHeight;
    Logi("scale view width height: %d - %d" , viewWidth , viewHeight);
    Logi("scale real width height: %f - %f" , width , height);
    Logi("scale x - y : %f - %f" , sx , sy);

    moveMatrix[0][0] = sx;
    moveMatrix[1][1] = sy;
//    moveMatrix[2][0] = left;
//    moveMatrix[2][1] = bottom;

//    float scale = 2.0f;
//    scaleMatrix[0][0] = scale;
//    scaleMatrix[1][1] = scale;
////    scaleMatrix[2][2] = 1.0f;
////    scaleMatrix[2][0] = -(left) * scale;
////    scaleMatrix[2][1] = -(bottom) * scale;

//    scaleMatrix = glm::inverse(moveMatrix);

    x = 0.0f;
    y = 0.0f;
    w = imageOriginWidth;
    h = imageOriginHeight;
}





