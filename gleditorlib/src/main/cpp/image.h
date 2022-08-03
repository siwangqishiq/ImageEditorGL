//
// Created by panyi on 2022/7/26.
//

#pragma once

#include "shader.h"
#include "jni.h"
#include "app.h"

class App;

class Image {
    friend class App;
public:
    Image(App *app) : appContext(app){}

    void onInit();

    void render(glm::mat3 &normalMatrix);

    void onDestroy();

    int imgWidth;
    int imgHeight;
    float x = 0;
    float y = 0;
    float w = 1.0f;
    float h = 1.0f;
    unsigned int textureId;
private:
    App *appContext;
    Shader shader;
    std::string filePath;
    jobject imageBitmap;

    unsigned int vbo;

    float vertexData[ 6 * 5] = {
            x       ,  y         , 1.0f , 0.0f , 1.0f,
            x + w , y + h   , 1.0f , 1.0f , 0.0f,
            x        , y + h   , 1.0f , 0.0f , 0.0f,
            x       ,  y         , 1.0f , 0.0f , 1.0f,
            x+  w , y          ,1.0f , 1.0f , 1.0f,
            x + w , y + h   , 1.0f , 1.0f , 0.0f,
    };

    glm::mat3 transMatrix = glm::mat3 (1.0f);

    void createShader();

    void resetPositionData();

    void updateVertexData();

    void loadTextureFromImageBitmap(JNIEnv *env);

    void setImageBitmap(JNIEnv *env ,jobject image_bitmap);
};
