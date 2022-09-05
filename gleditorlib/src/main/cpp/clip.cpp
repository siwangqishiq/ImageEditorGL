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


