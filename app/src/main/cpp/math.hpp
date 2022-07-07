//
// Created by panyi on 2022/7/6.
// glm中 未包含3x3 矩阵 向量的 旋转 缩放操作  自己实现一个
//
#pragma once

#include "glm/common.hpp"
#include "glm/matrix.hpp"

//构造缩放矩阵
glm::mat3 math_scale_mat3(float scaleX , float scaleY){
    glm::mat3 m =  glm::mat3(1.0f);
    m[0][0] = scaleX;
    m[1][1] = scaleY;
//    m[2][0] = - scaleX * dx;
//    m[2][1] = - scaleY * dy;
    return m;
}












