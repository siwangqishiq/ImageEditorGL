//
// Created by panyi on 2022/6/30.
//

#include "app.h"
#include "log.h"

void App::onResize(unsigned int width, unsigned int height) {
    this->viewWidth = width;
    this->viewHeight = height;
    Logi("on onResize ");
}

void App::setImageContent(std::string path, unsigned int imgWidth, unsigned int imgHeight) {
    this->filePath = path;
    this->imgWidth = imgWidth;
    this->imgHeight = imgHeight;
    Logi("on setImageContent %s size: %d x %d" , path.c_str() , imgWidth , imgHeight);


}

void App::onInit() {
    Logi("on init");
    Logi("prepare gl config!");
    glViewport(0 , 0, viewWidth , viewHeight);
    glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
    glEnable(GL_DEPTH);

    std::string vtxSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "layout(location = 0) in vec2 aPos;\n"
                                     "\n"
                                     "void main() {\n"
                                     "    gl_Position = vec4(aPos ,1.0 , 1.0);\n"
                                     "}");
    std::string frgSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "out vec4 FragColor;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    FragColor = vec4(0.0f , 1.0f , 0.0f , 1.0f);\n"
                                     "}");

    shader = Shader::buildGPUProgram(vtxSrc , frgSrc);

    glGenVertexArrays(1 , &vao);
    glGenBuffers(1 , &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER , vbo);
    glBufferData(GL_ARRAY_BUFFER , sizeof(vertices) , vertices , GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0 , 2 , GL_FLOAT , GL_FALSE , 2 * sizeof(float) , (void *)0);
    glEnableVertexAttribArray(0);
}

void App::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //do render
    shader.useShader();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN , 0 , 3);
}

void App::onDestroy() {
    Logi("destroy");
    glDeleteShader(shader.programId);
}

