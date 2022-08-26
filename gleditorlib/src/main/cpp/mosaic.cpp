//
// Created by panyi on 2022/8/13.
//

#include "mosaic.h"
#include "app.h"

void Mosaic::createShader() {
    Logi("mosaic create shader!");
    // shader = ShaderManager::getInstance().fetchShader("mosaic_shader" , vtxSrc , frgSrc);
    shader = ShaderManager::getInstance().fetchShaderByPath("mosaic_shader",
                                                            "mosaic_vertex.glsl","mosaic_frag.glsl");
}

void Mosaic::setShaderParams(glm::mat3 &normalMatrix) {
    glm::mat3 transMatrix = normalMatrix;
    shader.setIUniformMat3("transMat" , transMatrix);
    float scaleVal = 1.0f / appContext->scaleMatrix[0][0];
    shader.setUniformFloat("mosaicSize" , mosaicSize);
    shader.setUniformVec4("pointColor" , pointColor);

    shader.setUniformFloat("originImageWidth" , appContext->imageOriginWidth);
    shader.setUniformFloat("originImageHeight" , appContext->imageOriginHeight);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, appContext->baseImage->textureId);
    shader.setUniformInt("baseImageTexture" , 0);
}

void Mosaic::addPaintPoint(float _x, float _y){
    glm::vec2 worldPos = appContext->convertScreenToWorld(_x , _y);
    float x = worldPos.x;
    float y = worldPos.y;

    glm::vec3 endPoint = glm::vec3(x , y ,1.0f);
    if(pointList.empty()){
        pointList.push_back(endPoint);
        return;
    }

    const glm::vec3 startPoint = pointList.back();
    if(glm::distance(startPoint , endPoint) < 1.0f){
        return ;
    }

    const float distance = glm::distance(startPoint , endPoint);
    // Logi("distance : %f" , distance);
    if(distance >mosaicSize / 2.0f){
        glm::vec2 dir =glm::vec2(endPoint.x - startPoint.x , endPoint.y - startPoint.y);
        glm::vec2 dirNormal = glm::normalize(dir);
        float step = 1.0f;
        while (step <= distance){
            glm::vec2 insertPoint = glm::vec2(startPoint.x + step * dirNormal.x, startPoint.y + step * dirNormal.y);
            step += 1.0f;

            auto p = convertPointToMosaicCoord(insertPoint.x , insertPoint.y);
            pointList.push_back(glm::vec3( p.x , p.y , 1.0f));
        }//end while
    }
}

glm::vec2 Mosaic::convertPointToMosaicCoord(float _x , float _y) {
    glm::vec2 worldPos = glm::vec2(_x , _y);

    float imgWidth = appContext->originImage->imgWidth;
    float imgHeight = appContext->originImage->imgHeight;

    int leftIndex = (int)(worldPos.x) / (int)mosaicSize;
    int rightIndex = leftIndex + 1;
    int bottomIndex = (int)(worldPos.y) / (int)mosaicSize;
    int topIndex = bottomIndex + 1;

    float left = leftIndex * mosaicSize;
    float right = glm::min(imgWidth , rightIndex * mosaicSize);
    float bottom = bottomIndex * mosaicSize;
    float top = glm::min(imgHeight , topIndex * mosaicSize);

    glm::vec2 p1(left , bottom);
    glm::vec2 p2(right , top);
    return glm::vec2( (p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f);
}
