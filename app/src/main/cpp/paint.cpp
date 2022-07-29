//
// Created by panyi on 2022/7/26.
//

#include "paint.h"
#include "log.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Paint::onInit() {
    Logi("paint init start");
    createShader();

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    vbo = bufferIds[0];

//    Logi("vector size : %u" , pointList.size());
    glBindBuffer(GL_ARRAY_BUFFER , vbo);
    glBufferData(GL_ARRAY_BUFFER ,  BUFFER_SIZE * sizeof(glm::vec3), 0 , GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
}

void Paint::render() {
    shader.useShader();

    auto matrix = transMatrix * (appContext->normalMatrix);
    shader.setIUniformMat3("transMat" , matrix);
    shader.setUniformFloat("pointSize" , lineWidth);
    shader.setUniformVec4("pointColor" , pointColor);

    if(paintMode == Line){
        glBindBuffer(GL_ARRAY_BUFFER , vbo);
        glBufferSubData(GL_ARRAY_BUFFER , 0 , sizeof(glm::vec3) * vertexList.size() , vertexList.data());
//    glBufferData(GL_ARRAY_BUFFER , vertexList.size() * sizeof(glm::vec3) ,vertexList.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , false , sizeof(glm::vec3) , 0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLE_STRIP ,  0 , vertexList.size());
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER ,0);
    }else if(paintMode == Point){
        glBindBuffer(GL_ARRAY_BUFFER , vbo);
        glBufferSubData(GL_ARRAY_BUFFER , 0 , sizeof(glm::vec3) * pointList.size() , pointList.data());
        glVertexAttribPointer(0 , 3 , GL_FLOAT , false , sizeof(glm::vec3) , 0);

        glEnableVertexAttribArray(0);
        glDrawArrays(GL_POINTS , 0 , pointList.size());
        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER ,0);
    }
}

void Paint::onDestory() {
    GLuint bufferIds[1];
    bufferIds[0] = vbo;
    glDeleteBuffers(1 , bufferIds);
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

    shader = ShaderManager::getInstance().fetchShader("paint_shader" , vtxSrc , frgSrc);
}

void Paint::addPaintPoint(float x, float y) {
    glm::vec3 endPoint = glm::vec3(x , y ,1.0f);
    if(pointList.empty()){
        pointList.push_back(endPoint);
        return;
    }

    const glm::vec3 startPoint = pointList.back();
    if(glm::distance(startPoint , endPoint) < 1.0f){
        return ;
    }

    if(paintMode == Line){
        std::vector<glm::vec3> verts = genVertexByPoint(startPoint , endPoint);
        vertexList.insert(vertexList.end(), verts.begin() , verts.end());
    }else if(paintMode == Point){
        const float distance = glm::distance(startPoint , endPoint);
        Logi("distance : %f" , distance);
        if(distance >2.0f){
            glm::vec2 dir =glm::vec2(endPoint.x - startPoint.x , endPoint.y - startPoint.y);
            glm::vec2 dirNormal = glm::normalize(dir);
            float step = 1.0f;
            while (step <= distance){
                glm::vec2 insertPoint = glm::vec2(startPoint.x + step * dirNormal.x, startPoint.y + step * dirNormal.y);
                step += 1.0f;

                pointList.push_back(glm::vec3(insertPoint.x , insertPoint.y , 1.0f));
            }//end while
        }
    }

    pointList.push_back(endPoint);
    // Logi("pointList list size %d   %f , %f" , pointList.size() ,pointList.back().x, pointList.back().y);
}

//依据线段起始点 插入顶点数据
std::vector<glm::vec3> Paint::genVertexByPoint(glm::vec3 startPoint, glm::vec3 endPoint) {
//    Logi("startPoint   %f , %f , %f" , startPoint.x , startPoint.y, startPoint.z);
//    Logi("endPoint   %f , %f , %f" , endPoint.x , endPoint.y, endPoint.z);
    std::vector<glm::vec3> vertList = std::vector<glm::vec3>();
    glm::vec2 dir = glm::vec2(endPoint.x - startPoint.x , endPoint.y - startPoint.y);
    glm::vec2 dirNormalVec = glm::normalize(dir);
    glm::vec2 rotate90DirVec(-dirNormalVec.y , dirNormalVec.x);
    glm::vec2 rotate180DirVec(rotate90DirVec.x , -rotate90DirVec.y);

    const float halfWidth = lineWidth / 2.0f;

    glm::vec3 p1 = glm::vec3(startPoint.x + halfWidth * rotate90DirVec.x , startPoint.y + halfWidth * rotate90DirVec.y , 1.0f);
    glm::vec3 p2 = glm::vec3(p1.x + lineWidth * rotate180DirVec.x , p1.y + lineWidth * rotate180DirVec.y , 1.0f);
    glm::vec3 p3 = glm::vec3(p1.x + dir.x , p1.y + dir.y , 1.0f);
    glm::vec3 p4 = glm::vec3(p2.x + dir.x , p2.y + dir.y , 1.0f);

//    glm::vec3 p4 = endPoint;
//    Logi("dirVec   %f , %f , len = %f" , dirVec.x , dirVec.y, glm::length(dirVec));
//    Logi("rotate90DirVec   %f , %f , len = %f" , rotate90DirVec.x , rotate90DirVec.y, glm::length(rotate90DirVec));

    vertList.insert(vertList.end() , {p1, p2 ,p3 , p4});
    return vertList;
}
