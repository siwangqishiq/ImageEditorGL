//
// Created by panyi on 2022/7/26.
//

#include "image.h"
#include "log.h"
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <android/bitmap.h>
#include "shader.h"

void Image::onInit() {
    createShader();

    loadTextureFromImageBitmap(appContext->env);

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    vbo = bufferIds[0];

    glBindBuffer(GL_ARRAY_BUFFER , vbo);
    glBufferData(GL_ARRAY_BUFFER ,  6 *5 * sizeof(float) , vertexData , GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
}

void Image::render(glm::mat3 &normalMatrix) {
//    Logi("Image normal : %f , %f , %f" ,normalMatrix[0][0],normalMatrix[0][1],normalMatrix[0][2]);
//    Logi("Image normal : %f , %f , %f" ,normalMatrix[1][0],normalMatrix[1][1],normalMatrix[1][2]);
//    Logi("Image normal : %f , %f , %f" ,normalMatrix[2][0],normalMatrix[2][1],normalMatrix[2][2]);

//    glm::vec3 v1(vertexData[5 * 0 + 0] , vertexData[5 * 0 + 1] , vertexData[5 * 0 + 2]);
//    glm::vec3 v2(vertexData[5 * 1 + 0] , vertexData[5 * 1 + 1] , vertexData[5 * 1 + 2]);
//    glm::vec3 v3(vertexData[5 * 2 + 0] , vertexData[5 * 2 + 1] , vertexData[5 * 2 + 2]);
//    glm::vec3 v4(vertexData[5 * 3 + 0] , vertexData[5 * 3 + 1] , vertexData[5 * 3 + 2]);
//    glm::vec3 v5(vertexData[5 * 4 + 0] , vertexData[5 * 4 + 1] , vertexData[5 * 4 + 2]);
//    glm::vec3 v6(vertexData[5 * 5 + 0] , vertexData[5 * 5 + 1] , vertexData[5 * 5 + 2]);
//
//    auto newV1 = normalMatrix * v1;
//    Logi("transform v1 %f %f %f" , newV1.x , newV1.y , newV1.z);
//    auto newV2 = normalMatrix * v2;
//    Logi("transform v2 %f %f %f" , newV2.x , newV2.y , newV2.z);
//    auto newV3 = normalMatrix * v3;
//    Logi("transform v3 %f %f %f" , newV3.x , newV3.y , newV3.z);
//    auto newV4 = normalMatrix * v4;
//    Logi("transform v4 %f %f %f" , newV4.x , newV4.y , newV4.z);
//    auto newV5 = normalMatrix * v5;
//    Logi("transform v5 %f %f %f" , newV5.x , newV5.y , newV5.z);
//    auto newV6 = normalMatrix * v6;
//    Logi("transform v6 %f %f %f" , newV6.x , newV6.y , newV6.z);

    shader.useShader();

//    transMatrix = math_scale_mat3(scaleVal , scaleVal);
    glm::mat3 matrix =  (normalMatrix);
//
    shader.setIUniformMat3("transMat" , matrix);

    glBindBuffer(GL_ARRAY_BUFFER ,vbo);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , false , 5 * sizeof(float) , 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1 , 2 , GL_FLOAT , false , 5 * sizeof(float) , (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    shader.setUniformInt("sTexture" , 0);

    glDrawArrays(GL_TRIANGLE_FAN , 0 , 6);

    glBindBuffer(GL_ARRAY_BUFFER ,0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Image::createShader(){

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
                                     "    vUv = vec2(a_texture.x , 1.0 - a_texture.y);\n"
                                     "}");

    std::string frgSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "precision mediump float;\n"
                                     "\n"
                                     "uniform sampler2D sTexture;\n"
                                     "\n"
                                     "in vec2 vUv;\n"
                                     "layout(location = 0) out vec4 outColor;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    vec4 originColor = texture(sTexture , vUv);\n"
                                     "    outColor = originColor;\n"
                                     "}");

    shader = ShaderManager::getInstance().fetchShader("image_shader" , vtxSrc , frgSrc);
//    shader = ShaderManager::getInstance().fetchShaderByPath("image_shader")
}

void Image::updateVertexData() {
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

    for(int i = 0 ; i < 6 ; i++){
        Logi("vertexData : %f,%f,%f,%f,%f" , vertexData[5 * i + 0] , vertexData[5 * i + 1] , vertexData[5 * i + 2],
             vertexData[5 * i + 3],vertexData[5 * i + 4]);
    }
}

void Image::resetPositionData() {
    x = 0.0f;
    y = 0.0f;
    w = static_cast<float>(imgWidth);
    h = static_cast<float>(imgHeight);
    Logi("image vertex x = %f , y = %f , w = %f , h = %f" , x, y, w ,h);
    updateVertexData();
}

void Image::loadTextureFromImageBitmap(JNIEnv *env) {
    Logi("load bitmap texture");
    AndroidBitmapInfo info;
    if(int getInfoResult = AndroidBitmap_getInfo(env , imageBitmap , &info) < 0){
        Loge("get bitmap info error : %d" , getInfoResult);
        return;
    }
    Logi("bitmap width %d , height %d , format %d stride %d" , info.width , info.height , info.format , info.stride);

    imgWidth = info.width;
    imgHeight = info.height;

    void *dataBuf;
    if(int readPixelsResult = AndroidBitmap_lockPixels(env , imageBitmap , &dataBuf) < 0){
        Loge("readPixelsResult error : %d" , readPixelsResult);
        AndroidBitmap_unlockPixels(env , imageBitmap);
        return;
    }
    AndroidBitmap_unlockPixels(env , imageBitmap);

    resetPositionData();

    GLuint textureIds[1];
    glGenTextures(1 , textureIds);
    textureId = textureIds[0];

    glBindTexture(GL_TEXTURE_2D , this->textureId);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);

    int format = GL_RGBA;
    Logi("send texture to GPU");
    glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, dataBuf);
    Logi("send GPU success!");

    glBindTexture(GL_TEXTURE_2D , 0);
}

void Image::onDestroy() {
    GLuint bufferIds[1];
    bufferIds[0] = vbo;
    glDeleteBuffers(1 , bufferIds);

    GLuint textureIds[1];
    textureIds[0] = textureId;
    glDeleteTextures(1 , textureIds);
}

void Image::setImageBitmap(JNIEnv *env ,jobject image_bitmap) {
    imageBitmap = env->NewGlobalRef(image_bitmap);
}
