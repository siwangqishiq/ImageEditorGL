//
// Created by panyi on 2022/8/1.
//

#pragma once

#include "../glm/matrix.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../app.h"

class OriginImage {
public:
    OriginImage(App *app) : appContext(app){
    };

    void init(int w , int h);

    void renderToFrameBuffer();

    void destroy();

    int imgWidth;
    int imgHeight;

    unsigned int frameBufferId;
    unsigned int renderBufferId;
    unsigned int contentTextureId;

private:
    App *appContext;
    glm::mat3 normalMatrix = glm::mat3(1.0f);//绘制时归一化矩阵

    //
    void createFrameBufferObject();

    //重置归一化矩阵
    void resetNormalMatrix(int width , int height);
};


