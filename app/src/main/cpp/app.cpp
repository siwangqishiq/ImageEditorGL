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

void App::onResize(int width,int height) {
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

    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[0][0],normalMatrix[0][1],normalMatrix[0][2]);
    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[1][0],normalMatrix[1][1],normalMatrix[1][2]);
    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[2][0],normalMatrix[2][1],normalMatrix[2][2]);
}

void App::setImageContent(std::string path, unsigned int imgWidth, unsigned int imgHeight) {
    filePath = path;
    this->imgWidth = imgWidth;
    this->imgHeight = imgHeight;
    Logi("on setImageContent %s size: %d x %d" , path.c_str() , imgWidth , imgHeight);
}

void App::resetPositionData() {
    const float ratio = (float)imgWidth / imgHeight;
    if(imgWidth >= imgHeight){
        w = viewWidth;
        x = 0.0f;

        h = w / ratio;
        y = viewHeight / 2.0f - h / 2.0f;
    }else{
        h = viewHeight;
        y = 0.0f;
        w = ratio * h;
        x = viewWidth / 2.0f - w / 2.0f;
    }

    Logi("x = %f , y = %f , w = %f , h = %f" , x, y, w ,h);
    updateVertexData();
}

void App::updateVertexData() {
    vertexData[0 * 5 + 0] = x;
    vertexData[0 * 5 + 1] = y;

    vertexData[1 * 5 + 0] = x;
    vertexData[1 * 5 + 1] = y + h;

    vertexData[2 * 5 + 0] = x + w;
    vertexData[2 * 5 + 1] = y + h;

    vertexData[3 * 5 + 0] = x + w;
    vertexData[3 * 5 + 1] = y;

//    Logi("vertex : %.2f\t%.2f\t%.2f\t" ,vertexData[0 * 5 + 0],vertexData[0 * 5 + 1],vertexData[0 * 5 + 2]);
//    Logi("vertex : %.2f\t%.2f\t%.2f\t" ,vertexData[1 * 5 + 0],vertexData[1 * 5 + 1],vertexData[1 * 5 + 2]);
//    Logi("vertex : %.2f\t%.2f\t%.2f\t" ,vertexData[2 * 5 + 0],vertexData[2 * 5 + 1],vertexData[2 * 5 + 2]);
//    Logi("vertex : %.2f\t%.2f\t%.2f\t" ,vertexData[3 * 5 + 0],vertexData[3 * 5 + 1],vertexData[3 * 5 + 2]);


}

void App::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //do render
    shader.useShader();
//    shader.setIUniformMat3("normalMat", normalMatrix);
    Logi("scaleValue : %f" , scaleVal);

    transMatrix = math_scale_mat3(scaleVal , scaleVal);
    auto matrix = transMatrix * normalMatrix;

//    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[0][0],normalMatrix[0][1],normalMatrix[0][2]);
//    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[1][0],normalMatrix[1][1],normalMatrix[1][2]);
//    Logi("normalMatrix : %f\t %f\t %f\t" ,normalMatrix[2][0],normalMatrix[2][1],normalMatrix[2][2]);
//
//    Logi("transMatrix : %f\t %f\t %f\t" ,transMatrix[0][0],transMatrix[0][1],transMatrix[0][2]);
//    Logi("transMatrix : %f\t %f\t %f\t" ,transMatrix[1][0],transMatrix[1][1],transMatrix[1][2]);
//    Logi("transMatrix : %f\t %f\t %f\t" ,transMatrix[2][0],transMatrix[2][1],transMatrix[2][2]);
//
//    Logi("matrix : %f\t %f\t %f\t" ,matrix[0][0],matrix[0][1],matrix[0][2]);
//    Logi("matrix : %f\t %f\t %f\t" ,matrix[1][0],matrix[1][1],matrix[1][2]);
//    Logi("matrix : %f\t %f\t %f\t" ,matrix[2][0],matrix[2][1],matrix[2][2]);

    shader.setIUniformMat3("transMat" , matrix);

    glBindBuffer(GL_ARRAY_BUFFER ,vbo);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , false , 5 * sizeof(float) , 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1 , 2 , GL_FLOAT , false , 5 * sizeof(float) , (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glDrawArrays(GL_TRIANGLE_FAN , 0 , 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void App::onDestroy() {
    GLuint bufferIds[1];
    bufferIds[0] = vbo;
    glDeleteBuffers(1 , bufferIds);

    GLuint textureIds[1];
    textureIds[0] = textureId;
    glDeleteTextures(1 , textureIds);

    glDeleteShader(shader.programId);

    Logi("destroy");
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
                                     "    vUv = a_texture;\n"
                                     "}");

    std::string frgSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "precision mediump float;\n"
                                     "\n"
                                     "uniform sampler2D sTexture;\n"
                                     "\n"
                                     "in vec2 vUv;\n"
                                     "out vec4 out_color;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    out_color = texture(sTexture , vUv);\n"
                                     "}");

    shader = Shader::buildGPUProgram(vtxSrc , frgSrc);
}

void App::loadTexture() {
    int picChannel;
    unsigned char *data = stbi_load(filePath.c_str() , &imgWidth , &imgHeight , &picChannel , 0);
    Logi("read image file : %d , %d , channel: %d" , imgWidth , imgHeight , picChannel);

    if(data == nullptr){
        Logi("read image data error!");
    }

    resetPositionData();

    GLuint textureIds[1];
    glGenTextures(1 , textureIds);
    this->textureId = textureIds[0];

    glBindTexture(GL_TEXTURE_2D , this->textureId);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);

    int format = GL_RGBA;
    if(picChannel == 4){
        format = GL_RGBA;
    }else if(picChannel == 3){
        format = GL_RGBA;
    }

    Logi("send texture to GPU");
    glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    Logi("send GPU success!");

    glBindTexture(GL_TEXTURE_2D , 0);
}

void App::scale(float scale) {
    scaleVal = scale;
}

bool App::onTouch(int action, float x, float y) {
    Logi("touch %d , (%f , %f)" , action , x , y);
    return true;
}

void App::setImageBitmap(JNIEnv *env ,jobject image_bitmap) {
    imageBitmap = env->NewGlobalRef(image_bitmap);
}

void App::loadTextureFromImageBitmap(JNIEnv *env) {
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

void App::onInit(JNIEnv *env) {
    Logi("on init");
    Logi("prepare gl config!");

    glViewport(0 , 0, viewWidth , viewHeight);
    glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
    glEnable(GL_DEPTH);

    createShader();

    // loadTexture();
    loadTextureFromImageBitmap(env);

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    vbo = bufferIds[0];

    glBindBuffer(GL_ARRAY_BUFFER , vbo);
    glBufferData(GL_ARRAY_BUFFER ,  4 *5 * sizeof(float) , vertexData , GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
}



