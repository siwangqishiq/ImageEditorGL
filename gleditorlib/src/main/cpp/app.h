//
// Created by panyi on 2022/6/30.
//

#pragma once

#include <iostream>
#include <string>

#include "util/gl.h"
#include <jni.h>
#include "util/shader.h"
#include "widget/paint.h"
#include "widget/mosaic.h"
#include "widget/image.h"
#include "widget/origin_image.h"
#include "widget/mosaic.h"
#include "widget/clip.h"
#include <memory>
#include <deque>



enum Mode{
    IDLE,//空闲态
    IDLE_MOVE,//移动
    IDLE_SCALE,//缩放
    PAINT,//绘制
    MOSAIC,//马赛克
    CLIP,// 裁剪
};

static const float MAX_SCALE = 5.0f;
static const float MIN_SCALE = 0.7f;

static const float SCALE_VELOCITY = 0.05f; //缩放速率

class Mosaic;
class Paint;
class Image;
class OriginImage;

class App{
public:
    JNIEnv *env = nullptr;

    jobject modeChangeListener = nullptr;

    int viewWidth;
    int viewHeight;

    float imageOriginWidth;
    float imageOriginHeight;

    Mode mode = IDLE;

    //记录之前的状态
    Mode preMode = IDLE;

    std::shared_ptr<OriginImage> originImage; //真正的绘制Image 操作均绘制到此Image上

    std::shared_ptr<Image> baseImage;  //原始待编辑的图片

    //绘制组件集合
    std::vector<std::shared_ptr<Paint>> paintList = std::vector<std::shared_ptr<Paint>>();

    //马赛克绘制组件
    std::vector<std::shared_ptr<Mosaic>> mosaicList = std::vector<std::shared_ptr<Mosaic>>();

    //裁剪
    std::shared_ptr<ClipWidget> clipWidget = nullptr;

    void onResize(int width , int height);

    void onInit(JNIEnv *env);

    void onRender();

    void onDestroy();

    bool onTouch(int action ,float x , float y , float x2 , float y2);

    bool handleActionEvent(EventMessage msg);

    void setImageBitmap(JNIEnv *env , jobject image_bitmap);

    //重置底编辑图片  用于裁剪操作之后
    void resetBaseImage(unsigned int textureId , int textureWidth , int textureHeight);

    //主动退出应用
    void exitApp();

    //导出bitmap
    int exportBitmap(jobject outputBitmap);

    //改变模式
    void changeMode(Mode newMode);
    //恢复前一个模式
    void restorePreMode();
    //重置
    void resetImage();

    //实施裁剪
    void doClip();

    //屏幕坐标点 转为世界坐标点
    glm::vec2 convertScreenToWorld(float _x , float _y);

    float x = 0.0f;
    float y = 0.0f;
    float w = 1.0f;
    float h = 1.0f;

    float offsetX;
    float offsetY;
    float widthInView;
    float heightInView;

    float vertexData[6 * 5] = {
            x       ,  y         , 1.0f , 0.0f , 0.0f,
            x + w , y + h   , 1.0f , 1.0f , 1.0f,
            x        , y + h   , 1.0f , 0.0f , 1.0f,
            x       ,  y         , 1.0f , 0.0f , 0.0f,
            x+ w  , y          ,1.0f , 1.0f , 0.0f,
            x + w , y + h   , 1.0f , 1.0f , 1.0f,
    };
    glm::mat3 normalMatrix{1.0f};

    //调整原始图片 在View上的显示
    //先缩放 再平移
    glm::mat3 scaleMatrix{1.0f};
//    glm::mat3 moveAdjustMatrix{1.0f};

    glm::vec2 scaleCenter;

    glm::mat3 viewportScaleMatrix{1.0f};
    glm::mat3 viewportMoveMatrix{1.0f};

    //世界坐标 -> 屏幕坐标转化矩阵
    glm::mat3 worldToScreenMatrix{1.0f};

    //屏幕坐标->世界坐标
    glm::mat3 screenToWorldMatrix{1.0f};

    glm::vec2 lastPoint;

    glm::vec2 lastPoint2;

    float scaleOriginDistance = -1.0;
    float scaleFactor = 1.0f;//缩放因子

    //限制底图移动边界
    glm::vec3 limitLeftBottomPoint{0.0f , 0.0f , 1.0f};
    glm::vec3 limitRightTopPoint{1.0f , 1.0f ,1.0f};
private:
    Shader shader;

    unsigned int vbo;

    //事件消息队列 双端队列
    std::deque<EventMessage> messageQueue = std::deque<EventMessage>();

    //处理按下事件
    void handleDownAction(EventMessage &msg);

    //处理移动事件
    void handleMoveAction(EventMessage &msg);

    //处理取消事件
    void handleUpCancelAction(EventMessage &msg);

    //第二根手指按下
    void handlePointDownAction(EventMessage &msg);

    //第二根手指抬起
    void handlePointUpAction(EventMessage &msg);

    std::shared_ptr<Paint> fetchCurrentPaint();

    //
    std::shared_ptr<Mosaic> fetchCurrentMosaic();

    //处理事件消息队列
    bool pumpMessageQueue();

    void updateVertexData(float _x , float _y , float _w , float _h);

    void createShader();

    void initVertex();

    void renderMainView();

    void resetNormalMatrix(float width, float height);

    //计算出合适View的变换矩阵
    void calculateFitViewTransMatrix();

    //重置变换矩阵
    void resetTransMatrix();

    //从事件消息中计算出两点距离
    float calDistanceFromEventMsg(EventMessage &msg);

    //缩放手势 开始
    void onScaleGestureStart(EventMessage &msg);

    //缩放手势结束
    void onScaleGestureEnd();

    //移动底图
    void moveImageInView(float dx , float dy);

    //缩放底图
    void scaleImageInView(float scaleValue);
};
