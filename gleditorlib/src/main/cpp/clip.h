//
// Created by panyi on 2022/8/26.
//

#pragma once

#include "glm/vec3.hpp"

class App;

//裁剪控制点 4个
class ClipControllPoint{
public:
    glm::vec3 pos;//中心点位置
    float size = 32;//图标尺寸
};

//裁剪控件
class ClipWidget {
public:
    ClipWidget(App *app) : appContext(app){}

    void onInit();

    void onRender();

    void onDestroy();

    void resize();
private:
    App *appContext;

    //四个控制点
    ClipControllPoint leftTopPoint;
    ClipControllPoint rightTopPoint;
    ClipControllPoint leftBottomPoint;
    ClipControllPoint rightBottomPoint;
};


