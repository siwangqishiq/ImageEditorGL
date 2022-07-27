//
// Created by panyi on 2022/7/26.
//

#ifndef IMAGEEDITORGL_PAINT_H
#define IMAGEEDITORGL_PAINT_H

#include <vector>
#include "app.h"

class App;

struct EditorPoint;

class Paint {
public:
    const int BUFFER_SIZE = 2 * 1024; //2K

    Paint(App *app) : appContext(app){}

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

    int bufferOffset = 0;
};

#endif //IMAGEEDITORGL_PAINT_H
