//
// Created by panyi on 2022/6/30.
//

#pragma once

#include <iostream>
#include <string>

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>


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
    unsigned int imgWidth;
    unsigned int imgHeight;
};
