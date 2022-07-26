//
// Created by panyi on 2022/7/26.
//

#include "paint.h"
#include "log.h"

void Paint::onInit() {
//    EditorPoint p;
//    p.x = appContext->viewWidth / 2.0f;
//    p.y = appContext->viewHeight / 2.0f;
//    p.z = 1.0f;
//    pointList.push_back(p);

    Logi("paint init start");
    createShader();

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    vbo = bufferIds[0];

    Logi("vector size : %ld" , pointList.size());

//    glBindBuffer(GL_ARRAY_BUFFER , vbo);
//    glBufferData(GL_ARRAY_BUFFER ,  1024 * sizeof(EditorPoint) , pointList.data() , GL_DYNAMIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER , 0);
}

void Paint::render() {
    shader.useShader();

    auto matrix = transMatrix * (appContext->normalMatrix);
    shader.setIUniformMat3("transMat" , matrix);

    shader.setUniformFloat("pointSize" , pointSize);
    shader.setUniformVec4("pointColor" , pointColor);

//    Logi("Paintmatrix : %f\t %f\t %f\t" ,matrix[0][0],matrix[0][1],matrix[0][2]);
//    Logi("Paintmatrix : %f\t %f\t %f\t" ,matrix[1][0],matrix[1][1],matrix[1][2]);
//    Logi("Paintmatrix : %f\t %f\t %f\t" ,matrix[2][0],matrix[2][1],matrix[2][2]);

//    glBindBuffer(GL_ARRAY_BUFFER ,vbo);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , false , sizeof(EditorPoint) , pointList.data());

    glEnableVertexAttribArray(0);
//    Logi("point size : %ld" , pointList.size());
    glDrawArrays(GL_POINTS , 0 , pointList.size());
    glDisableVertexAttribArray(0);
}

void Paint::onDestory() {
    GLuint bufferIds[1];
    bufferIds[0] = vbo;
    glDeleteBuffers(1 , bufferIds);
    glDeleteShader(shader.programId);
}

void Paint::createShader(){
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
                                     "    outColor = pointColor;\n"
                                     "}");
    shader = Shader::buildGPUProgram(vtxSrc , frgSrc);
}

void Paint::addPaintPoint(float x, float y) {
    EditorPoint p;
    p.x = x;
    p.y = y;
    p.z = 1.0f;
    pointList.push_back(p);
    Logi("push point %f , %f" , pointList[pointList.size() - 1].x , pointList[pointList.size() - 1].y);
    //map range buffer opt?
//    glBindBuffer(GL_ARRAY_BUFFER , vbo);
//    glBufferData(GL_ARRAY_BUFFER ,  pointList.size() * sizeof(EditorPoint) , pointList.data() , GL_DYNAMIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER , 0);
}
