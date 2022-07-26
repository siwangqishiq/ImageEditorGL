//
// Created by panyi on 2022/6/30.
//

#pragma once

#include <iostream>
#include <string>

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <jni.h>
#include "shader.h"

const int ACTION_DOWN = 0;
const int ACTION_UP = 1;
const int ACTION_MOVE = 2;
const int ACTION_CANCEL = 3;

class App{
public:
    void setImageContent(std::string path , unsigned  int imgWdith , unsigned int imgHeight);

    void onResize(int width , int height);

    void onInit(JNIEnv *env);

    void onRender();

    void onDestroy();

    void scale(float scale);

    bool onTouch(int action ,float x , float y);

    void setImageBitmap(JNIEnv *env , jobject image_bitmap);

private:
    std::string filePath;

    jobject imageBitmap;

    int viewWidth;
    int viewHeight;
    int imgWidth;
    int imgHeight;

    unsigned int vbo;
    unsigned int textureId;
    Shader shader;

    float x = 0;
    float y = 0;
    float w = 1.0f;
    float h = 1.0f;
    float scaleVal = 1.0f;

    float vertexData[4 * 5] = {
            x       ,    y       , 1.0f , 0.0f , 1.0f,
            x        , y + h   , 1.0f , 0.0f , 0.0f,
            x + w , y + h   , 1.0f , 1.0f , 0.0f,
            x+ w  , y          ,1.0f , 1.0f , 1.0f
    };

    glm::mat3 normalMatrix = glm::mat3(1.0f);
    glm::mat3 transMatrix = glm::mat3 (1.0f);

    void createShader();

    void loadTexture();

    void resetPositionData();

    void updateVertexData();

    void loadTextureFromImageBitmap(JNIEnv *env);
};
