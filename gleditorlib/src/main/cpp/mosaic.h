//
// Created by panyi on 2022/8/13.
//

#pragma once

#include "paint.h"
#include "log.h"

class Mosaic : public Paint {
public:
    float mosaicSize = 16.0f;

    Mosaic(App *_app) : Paint(_app) {
        Logi("Mosaic init construct");
    }

    virtual void createShader() override;

    virtual void setShaderParams(glm::mat3 &normalMatrix) override;

    virtual void addPaintPoint(float x, float y) override;

    ~Mosaic(){
    }
};
