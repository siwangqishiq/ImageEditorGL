//
// Created by panyi on 2022/7/26.
//
#pragma once

#include <vector>
#include "app.h"
#include "common.h"

class App;

class Paint {
public:
    const int BUFFER_SIZE = 2 * 1024; //2K

    Paint(App *app) : appContext(app){
        onInit();
    }

    void onInit();

    void render();

    void onDestory();

    void addPaintPoint(float x , float y);
private:
    App *appContext;

    std::vector<EditorPoint> pointList = std::vector<EditorPoint>();

    float pointSize = 16.0f;
    glm::vec4 pointColor =  glm::vec4(1.0f , 0.0f ,0.0f ,1.0f);

    glm::mat3 transMatrix = glm::mat3 (1.0f);

    Shader shader;
    void createShader();

    unsigned int vbo;

    EditorPoint p;
};

