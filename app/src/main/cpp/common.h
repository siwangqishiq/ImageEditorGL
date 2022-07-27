//
// Created by panyi on 2022/7/27.
//

#pragma once

struct EditorPoint{
    float x;
    float y;
    float z;
};

const int ACTION_DOWN = 0;
const int ACTION_UP = 1;
const int ACTION_MOVE = 2;
const int ACTION_CANCEL = 3;

struct ActionMessage{
    int action;
    float x;
    float y;
};