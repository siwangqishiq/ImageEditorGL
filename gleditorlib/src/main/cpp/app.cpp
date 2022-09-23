//
// Created by panyi on 2022/6/30.
//

#include "app.h"
#include "util/log.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "util/math.hpp"
#include <android/bitmap.h>
#include <memory>

void App::onResize(int width,int height) {
    viewWidth = width;
    viewHeight = height;

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

    //重置归一化矩阵
    resetNormalMatrix(viewWidth , viewHeight);

    glViewport(0 , 0, viewWidth , viewHeight);
    glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
    glEnable(GL_DEPTH);

    baseImage->onInit();
    imageOriginWidth = static_cast<float>(baseImage->imgWidth);
    imageOriginHeight = static_cast<float>(baseImage->imgHeight);
    originImage->init(imageOriginWidth , imageOriginHeight);

    calculateFitViewTransMatrix();

    x = 0.0f;
    y = 0.0f;
    w = imageOriginWidth;
    h = imageOriginHeight;
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

    //do render 先将画面渲染至离屏缓存
    glBindFramebuffer(GL_FRAMEBUFFER , originImage->frameBufferId);
    originImage->renderToFrameBuffer();
    glBindFramebuffer(GL_FRAMEBUFFER , 0);


    //将离屏缓冲区内容作为纹理 渲染到屏幕
    glViewport(0 , 0, viewWidth , viewHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);
    renderMainView();

    if(mode ==Mode::CLIP && clipWidget != nullptr){
        clipWidget->onRender(normalMatrix);
    }
}

void App::onDestroy() {
//    originImage->destroy();
//    baseImage->onDestroy();

    for(auto &pPaint : paintList){
        pPaint->onDestroy();
    }//end for each

    for(auto &pMosaic : mosaicList){
        pMosaic->onDestroy();
    }

    if(clipWidget != nullptr){
        clipWidget->onDestroy();
    }

    GLuint bufferIds[] = {vbo};
    glDeleteBuffers(1 , bufferIds);

    ShaderManager::getInstance().clear();
    Logi("destroy");
}

bool App::onTouch(int action, float x, float y , float x2 , float y2) {
    bool ret = false;

    EventMessage msg(EVENT_ACTION_DOWN , x , y);
    msg.x2 = x2;
    msg.y2 = y2;
    glm::vec2 curPoint2(msg.x2 , msg.y2);

    // Logi("touch %d , (%f , %f)  secondPoint (%f , %f)" , action , x , y , x2 , y2);

    switch (action) {
        case 0:
            msg.action = EVENT_ACTION_DOWN;
            ret = true;
            break;
        case 1:
            msg.action = EVENT_ACTION_UP;
            ret = true;
            break;
        case 2:
            msg.action = EVENT_ACTION_MOVE;
            ret = true;
            break;
        case 5://ACTION_POINT_DOWN
            msg.action = EVENT_ACTION_POINT_DOWN;
            break;
        case 6://ACTION_POINT_UP
            msg.action = EVENT_ACTION_POINT_UP;
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

void App::handleDownAction(EventMessage &msg) {
    float _x = msg.x;
    float _y = msg.y;

    if(mode  == Mode::PAINT){//绘制模式
        auto newPaint = std::make_shared<Paint>(this);
        newPaint->onInit();
        paintList.push_back(newPaint);

        auto curPaint = fetchCurrentPaint();
        if(curPaint != nullptr){
            curPaint->addPaintPoint(_x , _y);
        }
    }else if(mode == Mode::MOSAIC){//马赛克模式
        Logi("mode %d" , mode);
        auto newMosaic = std::make_shared<Mosaic>(this);
        newMosaic->onInit();
        mosaicList.push_back(newMosaic);

        auto curMosaic = fetchCurrentMosaic();
        if(curMosaic != nullptr){
            curMosaic->addPaintPoint(_x , _y);
        }
    }else if(mode == Mode::CLIP){
        if(clipWidget != nullptr){
            clipWidget->onActionDown(_x, _y);
        }
    }else if(mode == Mode::IDLE){
        changeMode(Mode::IDLE_MOVE);
//        auto worldPos = convertScreenToWorld(x , y);
        lastPoint.x = _x;
        lastPoint.y = _y;
    }
}

void App::handleMoveAction(EventMessage &msg) {
    float _x = msg.x;
    float _y = msg.y;

    if(mode  == Mode::PAINT){//涂鸦模式
        auto curPaint = fetchCurrentPaint();
        if(curPaint != nullptr){
            curPaint->addPaintPoint(_x , _y);
        }
    }else if(mode == Mode::MOSAIC){//马赛克模式
        auto curMosaic = fetchCurrentMosaic();
        if(curMosaic != nullptr){
            curMosaic->addPaintPoint(_x , _y);
        }
    }else if(mode == Mode::CLIP){// 裁剪
        if(clipWidget != nullptr){
            clipWidget->onActionMove(_x,  _y);
        }
    }else if(mode == Mode::IDLE_MOVE){//移动状态
        float dx = _x - lastPoint.x;
        float dy = _y - lastPoint.y;

        lastPoint.x = _x;
        lastPoint.y = _y;

        //limit move
        glm::vec2 realLimitLeftBottom = viewportMoveMatrix * viewportScaleMatrix * limitLeftBottomPoint;
        glm::vec2 realLimitRightTop = viewportMoveMatrix * viewportScaleMatrix * limitRightTopPoint;

        float realImageHeight = realLimitRightTop.y - realLimitLeftBottom.y;
        Logi("real image height %f" , realImageHeight);

//        if(viewportMoveMatrix[2][1] > 0.0){
//            dy = 0.0f;
//        }
         moveImageInView(dx , dy);
        //Logi("dx = %f ,  dy = %f" , moveAdjustMatrix[2][0] , moveAdjustMatrix[2][1]) ;
        resetTransMatrix();
    }else if(mode == Mode::IDLE_SCALE){//缩放底图
        glm::vec2 curPoint2(msg.x2 , msg.y2);
        if(glm::distance(lastPoint2 , curPoint2) <= 1.0f){//防抖
            Logi("skip this action! %f" , glm::distance(lastPoint2 , curPoint2));
            return;
        }
        lastPoint2 = curPoint2;

        float cx = scaleCenter.x;
        float cy = scaleCenter.y;


        float currentDistance = calDistanceFromEventMsg(msg);
//        Logi("scale distance : %f" , currentDistance);
        if(scaleOriginDistance < 0){
            return;
        }

        float scaleDelta = 0.0f;
        float lenRatio = (currentDistance / scaleOriginDistance);
        if(lenRatio > 1.0){
            scaleDelta += SCALE_VELOCITY;
        }else{
            scaleDelta -= SCALE_VELOCITY;
        }

        float lastScaleFactor = scaleFactor;
        scaleOriginDistance = currentDistance;
        scaleFactor += scaleDelta;
//        Logi("scaleFactory : scalDelta %f   scaleFactor %f"  ,scaleDelta , scaleFactor);
//        glm::vec2 adjustPosition(scaleCenter.x  , scaleCenter.y);
        scaleImageInView(scaleFactor);

        float deltaScaleFactor = scaleFactor - lastScaleFactor;

        //需要将原始采集来的屏幕坐标 转化为viewport坐标
        auto p = glm::inverse(viewportMoveMatrix * viewportScaleMatrix) * glm::vec3(cx , cy , 1.0f);

        //由缩放操作 产生的位移修正
        float moveXbyScale =   (deltaScaleFactor) * (p.x);
        float moveYByScale =  (deltaScaleFactor) * (p.y);
//        Logi("scale fixed deltaScale : %f  move %f , %f" ,deltaScaleFactor , moveXbyScale , moveYByScale);

//        float originPosX = viewportMoveMatrix[2][0];
//        float originPosY = viewportMoveMatrix[2][1];

//        glm::mat3 scaleFixedMoveMat{1.0f};
//        scaleFixedMoveMat[2][0] = -moveXbyScale;
//        scaleFixedMoveMat[2][1] = -moveYByScale;
//        viewportScaleMatrix = scaleFixedMoveMat * viewportScaleMatrix;
         moveImageInView(-moveXbyScale , -moveYByScale);
        resetTransMatrix();
    }
}

void App::handlePointDownAction(EventMessage &msg) {
    if(mode == Mode::IDLE_MOVE || mode == Mode::IDLE) {
        changeMode(Mode::IDLE_SCALE);
        onScaleGestureStart(msg);
    }
}

void App::handlePointUpAction(EventMessage &msg) {
    if(mode == Mode::IDLE_SCALE){
        changeMode(Mode::IDLE);
        onScaleGestureEnd();
    }
}

void App::handleUpCancelAction(EventMessage &msg) {
    float _x = msg.x;
    float _y = msg.y;

    if(mode == Mode::PAINT){
        auto curPaint = fetchCurrentPaint();
        if(curPaint != nullptr){
            curPaint->addPaintPoint(_x , _y);
        }
    }else if(mode == Mode::IDLE_MOVE){
        changeMode(Mode::IDLE);
    }else if(mode == Mode::IDLE_SCALE){
        changeMode(Mode::IDLE);
        onScaleGestureEnd();
    }else if(mode == Mode::CLIP){// 裁剪
        if(clipWidget != nullptr){
            clipWidget->onActionUp(_x,  _y);
        }
    }
}

std::shared_ptr<Paint> App::fetchCurrentPaint() {
    if(paintList.empty()){
        return nullptr;
    }

    return paintList[paintList.size() - 1];
}

std::shared_ptr<Mosaic> App::fetchCurrentMosaic(){
    if(mosaicList.empty()){
        return nullptr;
    }

    return mosaicList.back();
}

//消费事件队列
bool App::pumpMessageQueue() {
//    Logi("action queue size : %d" , messageQueue.size());
    while(!messageQueue.empty()){
        bool continueRunning = handleActionEvent(messageQueue.front());
//        Logi("continueRunning : %d" , continueRunning);
        if(!continueRunning){
            messageQueue.clear();
            return false;
        }
        messageQueue.pop_front();
    }//end while

    return true;
}

/**
 * 事件处理  return 返回值
 *
 * @param msg
 * @return
 */
bool App::handleActionEvent(EventMessage msg) {
    // Logi("action : %d" , msg.action);
    if(msg.action == EVENT_EXIT){//退出事件
        Logi("action exit!!!!");
        onDestroy();
        return false;
    }

    switch (msg.action) {
        case EVENT_ACTION_DOWN:
            handleDownAction(msg);
            break;
        case EVENT_ACTION_UP:
            handleUpCancelAction(msg);
            break;
        case EVENT_ACTION_MOVE:
            handleMoveAction(msg);
            break;
        case EVENT_ACTION_POINT_DOWN:
            handlePointDownAction(msg);
            break;
        case EVENT_ACTION_POINT_UP:
            handlePointUpAction(msg);
            break;
        default:
            break;
    }//end switch
    return true;
}

void App::exitApp() {
//    EventMessage msg(EVENT_EXIT);
//    messageQueue.push_back(msg);
    Logi("exitApp event");
    onDestroy();
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

    glBindFramebuffer(GL_FRAMEBUFFER , originImage->frameBufferId);
    originImage->renderToFrameBuffer();

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
    glBindFramebuffer(GL_FRAMEBUFFER , 0);

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
    shader = ShaderManager::getInstance().fetchShaderByPath("view_shader" ,
                                                            "main_view_vertex.glsl","main_view_frag.glsl");
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

//    glm::mat3 matrix =normalMatrix * moveAdjustMatrix * scaleMatrix;
    glm::mat3 matrix =normalMatrix * worldToScreenMatrix;
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
    float width = 0.0f;
    float height = 0.0f;

    const float ratio = imageOriginWidth / imageOriginHeight;
    if(imageOriginWidth >= imageOriginHeight){
        width = viewWidth;
        height = viewWidth / ratio;
        if(height >= viewHeight){
            height = viewHeight;
            width = ratio * viewHeight;
        }
    }else{
        height = viewHeight;
        width = ratio * viewHeight;
        if(width >= viewWidth){
            width = viewWidth;
            height = viewWidth / ratio;
        }
    }

    widthInView = width;
    heightInView = height;

    float sx = width / imageOriginWidth;
    float sy = height / imageOriginHeight;
    Logi("scale view width height: %d - %d" , viewWidth , viewHeight);
    Logi("scale real width height: %f - %f" , width , height);
    Logi("scale x - y : %f - %f" , sx , sy);

    scaleMatrix[0][0] = sx;
    scaleMatrix[1][1] = sy;

    float dx = 0.0f;
    float dy = 0.0f;

    if(!equalFloat(width , static_cast<float>(viewWidth))){
        dx = (viewWidth - width) / 2.0f;
    }

    if(!equalFloat(height , static_cast<float>(viewHeight))){
        dy = (viewHeight - height) / 2.0f;
    }

//    moveAdjustMatrix[2][0] = dx;
//    moveAdjustMatrix[2][1] = dy;

    offsetX = dx;
    offsetY = dy;

    viewportMoveMatrix[2][0] = offsetX;
    viewportMoveMatrix[2][1] = offsetY;

    limitLeftBottomPoint = glm::vec3(0.0f , 0.0f , 1.0f);
    limitRightTopPoint = glm::vec3(widthInView ,  heightInView , 1.0f);
    Logi("limit %f , %f    -    %f , %f" , limitLeftBottomPoint.x , limitLeftBottomPoint.y , limitRightTopPoint.x , limitRightTopPoint.y);

    glm::vec2 realLimitLeftBottom = viewportMoveMatrix * viewportScaleMatrix * limitLeftBottomPoint;
    glm::vec2 realLimitRightTop = viewportMoveMatrix * viewportScaleMatrix * limitRightTopPoint;
    Logi("limit rect after scale : (%f , %f)   (%f , %f)" , realLimitLeftBottom.x , realLimitLeftBottom.y ,
         realLimitRightTop.x , realLimitRightTop.y);
    Logi("limit viewport : %f , %f     ,    %d , %d",viewportMoveMatrix[2][0] , viewportMoveMatrix[2][1] , viewWidth , viewHeight);

    resetTransMatrix();
}

void App::changeMode(Mode newMode) {
    preMode = mode;
    mode = newMode;

    Logi("change mode from %d to %d" , preMode , mode);

    if(mode == CLIP  && clipWidget == nullptr){
        clipWidget = std::make_shared<ClipWidget>(this);
        clipWidget->onInit();
    }

    clipWidget->resize();

    if(mode == CLIP){
        resetImage();
    }

    //callback
    if(modeChangeListener != nullptr && env != nullptr){
        Logi("change mode callback start");
        jclass clazz = env->FindClass("panyi/xyz/gleditorlib/IEditorModeChangeListener");
//        jclass clazz = env->GetObjectClass(modeChangeListener);
        jmethodID java_method_id = env->GetMethodID(clazz , "onModeChanged" , "()V");
//        jobject listenObj = env->NewGlobalRef(modeChangeListener);
        env->CallVoidMethod(modeChangeListener , java_method_id);
        Logi("change mode callback end");
    }
}

void App::resetImage(){
    scaleFactor = 1.0f;
    viewportScaleMatrix = glm::mat3(1.0f);
    viewportMoveMatrix = glm::mat3(1.0f);

    calculateFitViewTransMatrix();
}

void App::resetTransMatrix() {
    //变换矩阵重置
    worldToScreenMatrix = viewportMoveMatrix * viewportScaleMatrix  * scaleMatrix;
    screenToWorldMatrix = glm::inverse(worldToScreenMatrix);
}

glm::vec2 App::convertScreenToWorld(float _x, float _y) {
    glm::vec3 origin(_x , _y , 1.0f);
    glm::vec3 worldPos = screenToWorldMatrix * origin;
    return glm::vec2(worldPos.x , worldPos.y);
}

void App::restorePreMode() {
    mode = preMode;
}

float App::calDistanceFromEventMsg(EventMessage &msg) {
    glm::vec2 p1{msg.x , msg.y};
    glm::vec2 p2{msg.x2 , msg.y2};
    return glm::distance(p1 , p2);
}


void App::onScaleGestureStart(EventMessage &msg) {
    scaleOriginDistance = calDistanceFromEventMsg(msg);
//    scaleOriginDistance *= viewportScaleMatrix[0][0];
//    glm::vec3 pOrigin(msg.x , msg.y , 1.0f);
//    auto c = glm::inverse(viewportScaleMatrix * moveAdjustMatrix * scaleMatrix) * pOrigin;
    // auto inverseMat = glm::inverse(viewportScaleMatrix * moveAdjustMatrix);
    // auto c = inverseMat * pOrigin;

//    glm::vec2 c = convertScreenToWorld(msg.x , msg.y);
//    scaleCenter.x = c.x;
//    scaleCenter.y = c.y;

    scaleCenter.x = (msg.x + msg.x2)/ 2.0f;
    scaleCenter.y = (msg.y + msg.y2) / 2.0f;

//    scaleCenter.x -= viewportMoveMatrix[2][0];
//    scaleCenter.y -= viewportMoveMatrix[2][1];


//    lastMoveXbyScale =  (1 - scaleFactor) * (scaleCenter.x);
//    lastMoveYByScale = (1 - scaleFactor) * (scaleCenter.y);
}

//缩放手势结束
void App::onScaleGestureEnd() {
//    scaleMatrix = viewportScaleMatrix * scaleMatrix;
//    viewportScaleMatrix = glm::mat3(1.0f);
}

void App::moveImageInView(float dx, float dy) {
    glm::mat3 mMat{1.0f};
    mMat[2][0] = dx;
    mMat[2][1] = dy;

    viewportMoveMatrix = mMat * viewportMoveMatrix;

    //     adjust limit rectangle
    glm::vec2 realLimitLeftBottom = viewportMoveMatrix * viewportScaleMatrix * limitLeftBottomPoint;
    glm::vec2 realLimitRightTop = viewportMoveMatrix * viewportScaleMatrix * limitRightTopPoint;
    Logi("limit rect after scale : (%f , %f)   (%f , %f)" , realLimitLeftBottom.x , realLimitLeftBottom.y ,
         realLimitRightTop.x , realLimitRightTop.y);
    Logi("limit viewport : %f , %f     ,    %d , %d",viewportMoveMatrix[2][0] , viewportMoveMatrix[2][1] , viewWidth , viewHeight);

    float realWidth = realLimitRightTop.x - realLimitLeftBottom.x;
    float realHeight = realLimitRightTop.y - realLimitLeftBottom.y;
    //Logi("limit real size %f  %f" ,realWidth ,realHeight );

//    float x = viewportMoveMatrix[2][0];
//    float y = viewportMoveMatrix[2][1];

//    float adjustDx = moveAdjustMatrix[2][0];
//    float adjustDy = moveAdjustMatrix[2][1];

//    float scaleAdjustDx = adjustDx * viewportScaleMatrix[0][0];
//    float scaleAdjustDy = adjustDy * viewportScaleMatrix[1][1];
    //Logi("scale %f limit wall offset %f  %f , scaleAdjustDy %f" ,scaleFactor, x ,y , scaleAdjustDy);
//    Logi("limit %f , %f  scale= %f" ,viewportMoveMatrix[2][0] , viewportMoveMatrix[2][1] , viewportScaleMatrix[0][0]);

//    if(viewportMoveMatrix[2][0] > 0){
//        viewportMoveMatrix[2][0] = 0.0f;
//    }else if(viewportMoveMatrix[2][0]  < -realWidth + widthInView){
//        viewportMoveMatrix[2][0] = -realWidth + widthInView;
//    }
//
    if(viewportMoveMatrix[2][1] > 0){
        // viewportMoveMatrix[2][1] = 0.0f;
    }else if(viewportMoveMatrix[2][1]  < -realHeight + heightInView){
        // viewportMoveMatrix[2][0] = -realHeight + heightInView;
    }

//    if(viewportMoveMatrix[2][1] > 0){
//        Logi("limit occur for 0");
//        viewportMoveMatrix[2][1] = 0;
//    }else if(viewportMoveMatrix[2][1]  < -realHeight + heightInView){
//        Logi("limit occur for height");
//        viewportMoveMatrix[2][1] = -realHeight + heightInView;
//    }
}

void App::scaleImageInView(float scaleValue) {
    scaleFactor = scaleValue;
    if(scaleFactor > MAX_SCALE){
        scaleFactor = MAX_SCALE;
    }else if(scaleFactor < MIN_SCALE){
        scaleFactor = MIN_SCALE;
    }

    viewportScaleMatrix[0][0] = scaleFactor;
    viewportScaleMatrix[1][1] = scaleFactor;

//    auto realLimitLeftBottom = viewportScaleMatrix * limitLeftBottomPoint;
//    auto realLimitRightTop = viewportScaleMatrix * limitRightTopPoint;
//    Logi("limit %f , %f    -    %f , %f" , realLimitLeftBottom.x , realLimitLeftBottom.y , realLimitRightTop.x , realLimitRightTop.y);
}

void App::doClip(){
    Logi("do Clip!");
    if(clipWidget != nullptr){
        clipWidget->doClipAction();
    }
}

void App::resetBaseImage(unsigned int textureId , int textureWidth, int textureHeight) {
    mosaicList.clear();
    paintList.clear();

    clipWidget->initControlPointPos();
    clipWidget->updateControlPointToBuf(true);

    imageOriginWidth = textureWidth;
    imageOriginHeight = textureHeight;
    baseImage->resetNewTexture(textureId , textureWidth , textureHeight);

    updateVertexData(x , y , imageOriginWidth , imageOriginHeight);
    calculateFitViewTransMatrix();
}












