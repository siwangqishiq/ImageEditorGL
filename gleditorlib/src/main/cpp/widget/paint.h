//
// Created by panyi on 2022/7/26.
//
#pragma once

#include <vector>
#include "../util/common.h"
#include "../glm/ext/matrix_float3x3.hpp"
#include "../util/shader.h"

enum PaintMode{//绘制模式
    Point,
    Line
};

const int BUFFER_SIZE = 16 * 1024; //16K

class App;

class Paint {
public:
    App *appContext;

    PaintMode paintMode = Point;

    Paint(App *app) : appContext(app){};

    virtual void onInit();

    virtual void render(glm::mat3 &normalMatrix);

    virtual void onDestroy();

    virtual void addPaintPoint(float x , float y);

    virtual void createShader();

    virtual void setShaderParams(glm::mat3 &normalMatrix);

    virtual ~Paint();
protected:

    //
    std::vector<glm::vec3> pointList = std::vector<glm::vec3>();

    //顶点列表
    std::vector<glm::vec3> vertexList = std::vector<glm::vec3>();

    float lineWidth = 16.0f;
    glm::vec4 pointColor =  glm::vec4(1.0f , 0.0f ,0.0f ,1.0f);
//    glm::mat3 transMatrix = glm::mat3 (1.0f);

    Shader shader;

    std::vector<glm::vec3> genVertexByPoint(glm::vec3 startPoint , glm::vec3 endPoint);

    unsigned int vbo;
};
