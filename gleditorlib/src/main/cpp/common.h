//
// Created by panyi on 2022/7/27.
//

#pragma once

const int EVENT_EXIT = 100;//退出
const int EVENT_EXPORT_BITMAP = 102;//

const int EVENT_ACTION_DOWN = 1000;
const int EVENT_ACTION_UP = 1001;
const int EVENT_ACTION_MOVE = 1002;
const int EVENT_ACTION_CANCEL = 1003;

struct EventMessage{
    int action;
    float x;
    float y;

    EventMessage(int _a): action(_a) ,x(0.0f),y(0.0f){}

    EventMessage(int a , int _x , int _y):action(a) , x(_x),y(_y){}
};