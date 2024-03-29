//
// Created by panyi on 2022/8/13.
//

#pragma once

#include "paint.h"
#include "../util/log.h"

class Mosaic : public Paint {
public:
    float mosaicSize = 20.0f;

    Mosaic(App *_app) : Paint(_app) {
        Logi("Mosaic init construct");
    }

    //将给定的原始点 转为马赛克固定点
    glm::vec2 convertPointToMosaicCoord(float _x , float _y);

    virtual void createShader() override;

    virtual void setShaderParams(glm::mat3 &normalMatrix) override;

    virtual void addPaintPoint(float x, float y) override;

    float getRealMosaicSize();

    ~Mosaic(){
    }
};
