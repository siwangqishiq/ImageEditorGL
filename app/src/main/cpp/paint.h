//
// Created by panyi on 2022/7/26.
//

#ifndef IMAGEEDITORGL_PAINT_H
#define IMAGEEDITORGL_PAINT_H

#include <vector>
#include "app.h"

class App;

class Paint {
public:
    Paint(App *app) : appContext(app){}

    void onInit();

    void render();

    void onDestory();
private:
    App *appContext;
};


#endif //IMAGEEDITORGL_PAINT_H
