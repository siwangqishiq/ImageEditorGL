//
// Created by panyi on 2022/7/27.
//
#pragma once

#include <math.h>
#include <limits>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

const int EVENT_EXIT = 100;//退出
const int EVENT_EXPORT_BITMAP = 102;//

const int EVENT_ACTION_DOWN = 1000;
const int EVENT_ACTION_UP = 1001;
const int EVENT_ACTION_MOVE = 1002;
const int EVENT_ACTION_CANCEL = 1003;
const int EVENT_ACTION_POINT_DOWN = 1004;
const int EVENT_ACTION_POINT_UP = 1005;

struct EventMessage{
    int action;
    float x;
    float y;
    float x2 = 0.0f;
    float y2 = 0.0f;
    int actionIndex = 0;

    EventMessage(int _a): action(_a) ,x(0.0f),y(0.0f){}

    EventMessage(int a , int _x , int _y):action(a) , x(_x),y(_y){}
};

//比较浮点型数据是否相等
template <class T>
bool equalFloat(T a , T b){
    return abs(a - b) <= std::numeric_limits<T>::epsilon();
}

