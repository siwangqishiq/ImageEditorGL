//
// Created by panyi on 2022/8/26.
//

#pragma once

class App;
//裁剪控件
class ClipWidget {
public:
    ClipWidget(App *app) : appContext(app){}

    void onInit();

    void onRender();

    void onDestroy();
private:
    App *appContext;
};


