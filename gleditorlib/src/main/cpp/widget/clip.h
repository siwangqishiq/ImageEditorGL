//
// Created by panyi on 2022/8/26.
//

#pragma once

#include "../glm/vec3.hpp"
#include <vector>
#include "../util/shader.h"

class App;
class RectMaskWidget;

//裁剪控制点 4个
class ClipControlPoint{
public:
    glm::vec3 pos;//中心点位置

    bool isPointIn(float _x , float _y , float radius){
        float x = pos.x;
        float y = pos.y;
        return (x - _x) * (x - _x) + (y - _y) * (y - _y) <= radius * radius;
    }
};

//裁剪控件
class ClipWidget {
    friend class RectMaskWidget;
public:
    ClipWidget(App *app) : appContext(app){}

    void onInit();

    void onRender(glm::mat3 &normalMatrix);

    void onDestroy();

    void createShader();

    void resize();

    void onActionDown(float _x,float _y);

    //move
    void onActionMove(float _x , float _y);

    void onActionUp(float _x , float _y);

    //当前选中的控制点
    ClipControlPoint *currentSelectedPoint = nullptr;

    //背景
    std::shared_ptr<RectMaskWidget> rectMaskWidget;
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
    float controlPointVertex[4 * 3];
    unsigned int controlPointBufId;

    Shader clipControlShader;

    void updateControlPointToBuf(bool update);

    void limitMovePointAndSet(float _x ,float _y);
};

class RectMaskWidget{
public:
    RectMaskWidget(ClipWidget *clipWidget){
        clipWidget_ = clipWidget;
    }

    void onInit();

    void onRender(glm::mat3 &normalMatrix);

    void onDestroy();
private:
    ClipWidget *clipWidget_;

    //gl render
    static const int vertexDataSize = 4 * 2 * 3 * 3;
    float vertexData[vertexDataSize];
    unsigned int vertexBufId;
    Shader maskShader;

    void putVertexData(bool update);
};


