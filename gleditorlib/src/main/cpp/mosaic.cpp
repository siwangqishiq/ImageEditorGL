//
// Created by panyi on 2022/8/13.
//

#include "mosaic.h"
#include "app.h"

void Mosaic::createShader() {
    Logi("mosaic create shader!");

    std::string vtxSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "precision mediump float;\n"
                                     "\n"
                                     "layout(location = 0) in vec3 aPosition;\n"
                                     "\n"
                                     "out vec3 originCoord;\n"
                                     "\n"
                                     "uniform mat3 transMat;\n"
                                     "uniform float mosaicSize;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    gl_PointSize = mosaicSize;\n"
                                     "    originCoord = aPosition;\n"
                                     "    gl_Position = vec4(transMat * aPosition ,1.0f);\n"
                                     "}");

    std::string frgSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "precision mediump float;\n"
                                     "\n"
                                     "uniform float mosaicSize;\n"
                                     "uniform float originImageWidth;\n"
                                     "uniform float originImageHeight;\n"
                                     "uniform sampler2D baseImageTexture;\n"
                                     "\n"
                                     "in vec3 originCoord;\n"
                                     "\n"
                                     "layout(location = 0) out vec4 outColor;\n"
                                     "\n"
                                     "vec2 mosaicEffect(){\n"
                                     "    int leftIndex = int(originCoord.x) / int(mosaicSize);\n"
                                     "    int rightIndex = leftIndex + 1;\n"
                                     "    int bottomIndex = int(originCoord.y) / int(mosaicSize);\n"
                                     "    int topIndex = bottomIndex + 1;\n"
                                     "\n"
                                     "    vec2 leftBottomPoint;\n"
                                     "    leftBottomPoint.x =  float(leftIndex) * mosaicSize;\n"
                                     "    leftBottomPoint.y = float(bottomIndex) * mosaicSize;\n"
                                     "\n"
                                     "    vec2 rightTopPoint;\n"
                                     "    rightTopPoint.x = float(rightIndex) * mosaicSize;\n"
                                     "    rightTopPoint.y = float(topIndex) * mosaicSize;\n"
                                     "\n"
                                     "    return vec2((leftBottomPoint.x + rightTopPoint.x) / 2.0f , (leftBottomPoint.y + rightTopPoint.y) / 2.0f);\n"
                                     "}\n"
                                     "\n"
                                     "void main(){\n"
                                     "    vec2 mosaicPoint = mosaicEffect();\n"
                                     "    vec2 uv = vec2(mosaicPoint.x / originImageWidth ,1.0f - mosaicPoint.y / originImageHeight);\n"
                                     "    vec4 originColor = texture(baseImageTexture , uv);\n"
                                     "    outColor = originColor;\n"
                                     "}");

    shader = ShaderManager::getInstance().fetchShader("mosaic_shader" , vtxSrc , frgSrc);
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
    float imgWidth = appContext->originImage->imgWidth;
    float imgHeight = appContext->originImage->imgHeight;

    glm::vec2 worldPos = appContext->convertScreenToWorld(_x , _y);

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
    pointList.push_back(glm::vec3( (p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f , 1.0f));
}
