//
// Created by panyi on 2022/6/30.
//

#include "app.h"
#include "log.h"

void App::onResize(unsigned int width, unsigned int height) {
    this->viewWidth = width;
    this->viewHeight = height;
    Logi("on onResize ");
}

void App::setImageContent(std::string path, unsigned int imgWidth, unsigned int imgHeight) {
    this->filePath = path;
    this->imgWidth = imgWidth;
    this->imgHeight = imgHeight;
    Logi("on setImageContent %s size: %d x %d" , path.c_str() , imgWidth , imgHeight);


}

void App::onInit() {
    Logi("on init");
    Logi("prepare gl config!");
    glViewport(0 , 0, viewWidth , viewHeight);
    glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
    glEnable(GL_DEPTH);


}

void App::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //do render

}

void App::onDestroy() {
    Logi("destroy");
}

