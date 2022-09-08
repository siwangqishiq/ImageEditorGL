//
// Created by panyi on 2022/8/26.
//

#include "clip.h"
#include "app.h"

void ClipWidget::onInit() {
    leftTopPoint.pos = glm::vec3(appContext->offsetX , appContext->offsetY + appContext->heightInView, 1.0f);
    rightTopPoint.pos = glm::vec3(appContext->offsetX + appContext->widthInView ,
                                  appContext->offsetY + appContext->heightInView , 1.0f);
    leftBottomPoint.pos = glm::vec3(appContext->offsetX , appContext->offsetY, 1.0f);
    rightBottomPoint.pos = glm::vec3(appContext->offsetX + appContext->widthInView ,
                                     appContext->offsetY , 1.0f);

    unsigned int buf[1];
    glGenBuffers(1 , buf);
    controlPointBufId = buf[0];
    updateControlPointToBuf(false);

    createShader();
}

void ClipWidget::onRender(glm::mat3 &normalMatrix) {
    updateControlPointToBuf(true);

    clipControlShader.useShader();

    clipControlShader.setUniformFloat("size",controlPointSize);
    auto matrix = normalMatrix;
     clipControlShader.setIUniformMat3("transMat" , matrix);

    glBindBuffer(GL_ARRAY_BUFFER , controlPointBufId);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , false , sizeof(glm::vec3) , 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS , 0 , 4);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER , 0);
}

void ClipWidget::onDestroy() {
    const int bufCount = 1;
    unsigned int buf[bufCount];
    buf[0] = controlPointBufId;

    glDeleteBuffers(bufCount , buf);
}

void ClipWidget::resize() {

}

void ClipWidget::createShader(){
    Logi("clip create clipControlShader!");
    clipControlShader = ShaderManager::getInstance().fetchShaderByPath("clipControlShader",
                                                            "clip_control_vertex.glsl","clip_control_frag.glsl");
}

void ClipWidget::updateControlPointToBuf(bool update) {
    controlPointVertex[0] = leftTopPoint.pos.x;
    controlPointVertex[1] = leftTopPoint.pos.y;
    controlPointVertex[2] = leftTopPoint.pos.z;

    controlPointVertex[3] = rightTopPoint.pos.x;
    controlPointVertex[4] = rightTopPoint.pos.y;
    controlPointVertex[5] = rightTopPoint.pos.z;

    controlPointVertex[6] = rightBottomPoint.pos.x;
    controlPointVertex[7] = rightBottomPoint.pos.y;
    controlPointVertex[8] = rightBottomPoint.pos.z;

    controlPointVertex[9] = leftBottomPoint.pos.x;
    controlPointVertex[10] = leftBottomPoint.pos.y;
    controlPointVertex[11] = leftBottomPoint.pos.z;

    glBindBuffer(GL_ARRAY_BUFFER , controlPointBufId);

    if(update){
        glBufferSubData(GL_ARRAY_BUFFER , 0 , vertexBufSize , controlPointVertex);
    }else{
        glBufferData(GL_ARRAY_BUFFER , vertexBufSize , controlPointVertex , GL_DYNAMIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER , 0);
}

void ClipWidget::onActionMove(float _x, float _y) {
    // Logi("clipWidget %f ,  %f" , _x , _y);
    if(currentSelectedPoint == nullptr){
        return;
    }

    limitMovePointAndSet(_x , _y);
    updateControlPointToBuf(true);
}

void ClipWidget::limitMovePointAndSet(float _x ,float _y){
    if(_x <= appContext->offsetX){
        _x = appContext->offsetX;
    }else if(_x >= appContext->offsetX + appContext->widthInView){
        _x = appContext->offsetX + appContext->widthInView;
    }

    if(_y <= appContext->offsetY){
        _y = appContext->offsetY;
    }else if(_y >= appContext->offsetY + appContext->heightInView){
        _y = appContext->offsetY + appContext->heightInView;
    }

    if(currentSelectedPoint == &leftTopPoint){//左上方
        _x = glm::min(rightTopPoint.pos.x - controlPointSize , _x);
        _y = glm::max(leftBottomPoint.pos.y + controlPointSize , _y);
    }else if(currentSelectedPoint == &rightTopPoint){//右上方
        _x = glm::max(leftTopPoint.pos.x + controlPointSize , _x);
        _y = glm::max(rightBottomPoint.pos.y + controlPointSize , _y);
    }else if(currentSelectedPoint == &leftBottomPoint){//左下方
        _x = glm::min(rightBottomPoint.pos.x - controlPointSize , _x);
        _y = glm::min(leftTopPoint.pos.y - controlPointSize , _y);
    }else if(currentSelectedPoint == &rightBottomPoint){//右下方
        _x = glm::max(leftBottomPoint.pos.x + controlPointSize , _x);
        _y = glm::min(rightTopPoint.pos.y - controlPointSize , _y);
    }

    currentSelectedPoint->pos.x = _x;
    currentSelectedPoint->pos.y = _y;

    if(currentSelectedPoint == &leftTopPoint){//左上方
        rightTopPoint.pos.y = _y;
        leftBottomPoint.pos.x = _x;
    }else if(currentSelectedPoint == &rightTopPoint){//右上方
        leftTopPoint.pos.y = _y;
        rightBottomPoint.pos.x = _x;
    }else if(currentSelectedPoint == &leftBottomPoint){//左下方
        rightBottomPoint.pos.y = _y;
        leftTopPoint.pos.x = _x;
    }else if(currentSelectedPoint == &rightBottomPoint){//右下方
        leftBottomPoint.pos.y = _y;
        rightTopPoint.pos.x = _x;
    }

}

void ClipWidget::onActionDown(float _x,float _y) {
    // Logi("clipWidget %f ,  %f" , _x , _y);

    const float radius = controlPointSize / 2.0f;
    if(leftTopPoint.isPointIn(_x , _y, radius)){
        currentSelectedPoint = &leftTopPoint;
    }else if(rightTopPoint.isPointIn(_x , _y, radius)){
        currentSelectedPoint = &rightTopPoint;
    }else if(leftBottomPoint.isPointIn(_x , _y, radius)){
        currentSelectedPoint = &leftBottomPoint;
    }else if(rightBottomPoint.isPointIn(_x , _y, radius)){
        currentSelectedPoint = &rightBottomPoint;
    }
}

void ClipWidget::onActionUp(float _x, float _y) {
    currentSelectedPoint = nullptr;
}


