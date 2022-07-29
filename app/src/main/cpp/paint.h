//
// Created by panyi on 2022/7/26.
//
#pragma once

#include <vector>
#include "app.h"
#include "common.h"

class App;

enum PaintMode{//绘制模式
    Point,
    Line
};

class Paint {
public:
    const int BUFFER_SIZE = 4 * 1024; //4K

    PaintMode paintMode = Point;

    Paint(App *app) : appContext(app){
        onInit();
    }

    void onInit();

    void render();

    void onDestory();

    void addPaintPoint(float x , float y);
private:
    App *appContext;

    //
    std::vector<glm::vec3> pointList = std::vector<glm::vec3>();

    //顶点列表
    std::vector<glm::vec3> vertexList = std::vector<glm::vec3>();

    float lineWidth = 16.0f;
    glm::vec4 pointColor =  glm::vec4(0.0f , 0.0f ,1.0f ,1.0f);
    glm::mat3 transMatrix = glm::mat3 (1.0f);

    Shader shader;
    void createShader();

    std::vector<glm::vec3> genVertexByPoint(glm::vec3 startPoint , glm::vec3 endPoint);

    unsigned int vbo;
};

