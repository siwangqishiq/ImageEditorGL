//
// Created by panyi on 2022/6/30.
//

#include "app.h"
#include "log.h"

//#include <android/imagedecoder.h>
//#include <android/bitmap.h>

#include "stb_image.h"

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

    createShader();

    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    vbo = bufferIds[0];

    glBindBuffer(GL_ARRAY_BUFFER , vbo);
    glBufferData(GL_ARRAY_BUFFER ,  4 * 4 * sizeof(float) , vertexData , GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER , 0);

    loadTexture();
}

void App::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //do render
    shader.useShader();
    glBindBuffer(GL_ARRAY_BUFFER ,vbo);
    glVertexAttribPointer(0 , 2 , GL_FLOAT , false , 4 * sizeof(float) , 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1 , 2 , GL_FLOAT , false , 4 * sizeof(float) , (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glDrawArrays(GL_TRIANGLE_FAN , 0 , 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void App::onDestroy() {
    GLuint bufferIds[1];
    bufferIds[0] = vbo;
    glDeleteBuffers(1 , bufferIds);

    GLuint textureIds[1];
    textureIds[0] = textureId;
    glDeleteTextures(1 , textureIds);

    glDeleteShader(shader.programId);

    Logi("destroy");
}

void App::createShader() {
    std::string vtxSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "layout(location = 0) in vec2 a_position;\n"
                                     "layout(location = 1) in vec2 a_texture;\n"
                                     "\n"
                                     "out vec2 vUv;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    gl_Position = vec4(a_position.xy, 0.0f ,1.0f);\n"
                                     "    vUv = a_texture;\n"
                                     "}");
    std::string frgSrc = std::string("#version 300 es\n"
                                     "\n"
                                     "precision mediump float;\n"
                                     "\n"
                                     "uniform sampler2D sTexture;\n"
                                     "\n"
                                     "in vec2 vUv;\n"
                                     "out vec4 out_color;\n"
                                     "\n"
                                     "void main(){\n"
                                     "    out_color = texture(sTexture , vUv);\n"
                                     "}");

    shader = Shader::buildGPUProgram(vtxSrc , frgSrc);
}

void App::loadTexture() {
    int picChannel;
    unsigned char *data = stbi_load(filePath.c_str() , &imgWidth , &imgHeight , &picChannel , 0);
    Logi("read image file : %d , %d , channel: %d" , imgWidth , imgHeight , picChannel);

    if(data == nullptr){
        Logi("read image data error!");
    }

    GLuint textureIds[1];
    glGenTextures(1 , textureIds);
    this->textureId = textureIds[0];

    glBindTexture(GL_TEXTURE_2D , this->textureId);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);

    int format = GL_RGBA;
    if(picChannel == 4){
        format = GL_RGBA;
    }else if(picChannel == 3){
        format = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D , 0);
}

