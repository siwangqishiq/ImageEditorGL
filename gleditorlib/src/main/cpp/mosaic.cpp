//
// Created by panyi on 2022/8/13.
//

#include "mosaic.h"
#include "app.h"

void Mosaic::createShader() {
    Logi("mosaic create shader!");

    std::string vtxSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "layout(location = 0) in vec3 aPosition;\n"
                                     "\n"
                                     "out vec3 originCoord;\n"
                                     "\n"
                                     "uniform mat3 transMat;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    gl_PointSize = 16.0f;\n"
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
                                     "void main(){\n"
                                     "    vec2 uv = vec2(originCoord.x / originImageWidth ,1.0f- originCoord.y / originImageHeight);\n"
                                     "    vec4 originColor = texture(baseImageTexture , uv);\n"
                                     "    outColor = originColor;\n"
                                     "}");

    shader = ShaderManager::getInstance().fetchShader("mosaic_shader" , vtxSrc , frgSrc);
}

void Mosaic::setShaderParams(glm::mat3 &normalMatrix) {
    glm::mat3 transMatrix = normalMatrix;
    shader.setIUniformMat3("transMat" , transMatrix);
    float scaleVal = 1.0f / appContext->scaleMatrix[0][0];
    shader.setUniformFloat("mosaicSize" , lineWidth * scaleVal);
    shader.setUniformVec4("pointColor" , pointColor);

    shader.setUniformFloat("originImageWidth" , appContext->imageOriginWidth);
    shader.setUniformFloat("originImageHeight" , appContext->imageOriginHeight);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, appContext->baseImage->textureId);
    shader.setUniformInt("baseImageTexture" , 0);
}
