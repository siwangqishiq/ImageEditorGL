//
// Created by panyi on 2022/8/26.
//

#include "clip.h"
#include "app.h"

void ClipWidget::onInit() {
    leftTopPoint.pos = glm::vec3(0.0f , appContext->originImage->imgHeight , 1.0f);
    rightTopPoint.pos = glm::vec3(appContext->originImage->imgWidth , appContext->originImage->imgHeight , 1.0f);
    leftBottomPoint.pos = glm::vec3(0.0f , 0.0f , 1.0f);
    rightBottomPoint.pos = glm::vec3(appContext->originImage->imgWidth , 0.0f , 1.0f);

    unsigned int buf[1];
    glGenBuffers(1 , buf);
    controlPointBufId = buf[0];
    updateControlPointToBuf();

    createShader();
}

void ClipWidget::onRender() {
    updateControlPointToBuf();

    clipControlShader.useShader();
    
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

void ClipWidget::updateControlPointToBuf() {
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
    glBufferSubData(GL_ARRAY_BUFFER , 0 , 4 * 3 , controlPointVertex);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
}


