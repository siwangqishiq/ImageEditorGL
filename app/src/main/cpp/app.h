//
// Created by panyi on 2022/6/30.
//

#pragma once

#include <iostream>
#include <string>

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include "shader.h"


class App{
public:
    void setImageContent(std::string path , unsigned  int imgWdith , unsigned int imgHeight);

    void onResize(unsigned int width , unsigned int height);

    void onInit();

    void onRender();

    void onDestroy();
private:
    std::string filePath;

    unsigned int viewWidth;
    unsigned int viewHeight;
    int imgWidth;
    int imgHeight;

    unsigned int vbo;
    unsigned int textureId;
    Shader shader;

    float vertexData[4 * 4] = {
            -1.0f , -1.0f ,0.0f , 1.0f,
            -1.0f , 1.0f , 0.0f , 0.0f,
            1.0f , 1.0f,  1.0f , 0.0f,
            1.0f , -1.0f, 1.0f , 1.0f
    };

    void createShader();

    void loadTexture();
};
