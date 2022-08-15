//
// Created by panyi on 2022/8/1.
//

#include "origin_image.h"
#include "log.h"
#include "gl.h"

void OriginImage::init(int w, int h) {
    imgWidth = w;
    imgHeight = h;
    resetNormalMatrix(imgWidth , imgHeight);

    Logi("image origin width %d , height %d" , imgWidth , imgHeight);
    createFrameBufferObject();
}

//绘制内容 到 framebuffer
void OriginImage::renderToFrameBuffer() {
    //render objects to texture
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        glBindFramebuffer(GL_FRAMEBUFFER , 0);
        Loge("frame buffer status error !");
        return;
    }

    glViewport(0 , 0, imgWidth , imgHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);//关闭深度测试  按绘制顺序显示图像

     appContext->baseImage->render(normalMatrix);

    auto paintList = appContext->paintList;
    for(auto &pPaint : paintList){
        pPaint->render(normalMatrix);
    }//end for each

    auto mosaicList = appContext->mosaicList;
    for(auto &m : mosaicList){
        m->render(normalMatrix);
    }//end for each
}

void OriginImage::createFrameBufferObject() {
    Logi("create frame buffer imgWidth %d  imgHeight %d" , imgWidth , imgHeight);
    GLuint textureIds[1];
    glGenTextures(1 , textureIds);
    contentTextureId = textureIds[0];
    glBindTexture(GL_TEXTURE_2D , contentTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLuint frameBufferIds[1];
    glGenFramebuffers(1 , frameBufferIds);
    frameBufferId = frameBufferIds[0];
    glBindFramebuffer(GL_FRAMEBUFFER , frameBufferId);
    glBindTexture(GL_TEXTURE_2D , contentTextureId);

    GLuint  renderBufferIds[1];
    glGenRenderbuffers(1 , renderBufferIds);
    renderBufferId = renderBufferIds[0];

    glBindRenderbuffer(GL_RENDERBUFFER , renderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8 , imgWidth , imgHeight);
    glBindRenderbuffer(GL_RENDERBUFFER , 0);

    glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA , imgWidth , imgHeight ,
                 0 , GL_RGBA , GL_UNSIGNED_BYTE ,nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 ,
                           GL_TEXTURE_2D , contentTextureId , 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER , GL_DEPTH_STENCIL_ATTACHMENT ,
                              GL_RENDERBUFFER , renderBufferId);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=GL_FRAMEBUFFER_COMPLETE){
        Loge("create frame buffer error!!!");
    }else{
        Logi("create frame buffer OK");
    }

    glBindTexture(GL_TEXTURE_2D , GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER , 0);
}

void OriginImage::destroy() {
    GLuint textureIds[] = {contentTextureId};
    glDeleteTextures(1 , textureIds);

    GLuint renderBufferIds[] = {renderBufferId};
    glDeleteRenderbuffers(1 , renderBufferIds);

    GLuint frameBufferIds[] = {frameBufferId};
    glDeleteFramebuffers(1 , frameBufferIds);
}

void OriginImage::resetNormalMatrix(int width , int height) {
    //重置归一化矩阵
    normalMatrix[0][0] = 2.0f/static_cast<float>(width);
    normalMatrix[0][1] = 0.0f;
    normalMatrix[0][2] = 0.0f;

    normalMatrix[1][0] = 0.0f;
    normalMatrix[1][1] = 2.0f / static_cast<float>(height);
    normalMatrix[1][2] = 0.0f;

    normalMatrix[2][0] = -1.0f;
    normalMatrix[2][1] = -1.0f;
    normalMatrix[2][2] = 1.0f;

    Logi("normalMatrix[0][0] = %f , normalMatrix[1][1] = %f" , normalMatrix[0][0] , normalMatrix[1][1]);
}



