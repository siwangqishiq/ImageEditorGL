//
// Created by panyi on 2022/8/26.
//

#pragma once

#include "glm/vec3.hpp"
#include <vector>
#include "shader.h"

class App;

//裁剪控制点 4个
class ClipControlPoint{
public:
    glm::vec3 pos;//中心点位置
};

//裁剪控件
class ClipWidget {
public:
    ClipWidget(App *app) : appContext(app){}

    void onInit();

    void onRender(glm::mat3 &normalMatrix);

    void onDestroy();

    void createShader();

    void resize();
private:
    App *appContext;

    //四个控制点
    ClipControlPoint leftTopPoint;
    ClipControlPoint rightTopPoint;
    ClipControlPoint leftBottomPoint;
    ClipControlPoint rightBottomPoint;

    float controlPointSize = 64;//图标尺寸

    static const int vertexBufSize = 4 * 3 * sizeof(float);
    //控制点 顶点坐标
    float controlPointVertex[vertexBufSize];
    unsigned int controlPointBufId;

    Shader clipControlShader;

    void updateControlPointToBuf(bool update);
};


