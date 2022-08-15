//
// Created by panyi on 2022/8/13.
//

#include "mosaic.h"

void Mosaic::createShader() {
    Logi("mosaic create shader!");

    std::string vtxSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "layout(location = 0) in vec3 aPosition;\n"
                                     "\n"
                                     "uniform mat3 transMat;\n"
                                     "uniform float pointSize;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    gl_Position = vec4(transMat * aPosition ,1.0f);\n"
                                     "    gl_PointSize = pointSize;\n"
                                     "}");

    std::string frgSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "precision mediump float;\n"
                                     "\n"
                                     "uniform vec4 pointColor;\n"
                                     "\n"
                                     "out vec4 outColor;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    vec2 coord = gl_PointCoord - vec2(0.5);\n"
                                     "    if(length(coord) > 0.5){\n"
                                     "        discard;\n"
                                     "    }\n"
                                     "    outColor = vec4(1.0f ,1.0f , 0.0f ,1.0f);\n"
                                     "}");

    shader = ShaderManager::getInstance().fetchShader("mosaic_shader" , vtxSrc , frgSrc);
}