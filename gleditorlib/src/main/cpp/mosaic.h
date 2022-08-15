//
// Created by panyi on 2022/8/13.
//

#pragma once

#include "paint.h"
#include "log.h"

class Mosaic : public Paint {
public:
    Mosaic(App *_app) : Paint(_app) {
        Logi("Mosaic init construct");
    }

    virtual void createShader();

    ~Mosaic(){
    }
};
