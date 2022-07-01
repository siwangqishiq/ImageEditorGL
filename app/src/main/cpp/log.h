//
// Created by panyi on 2022/6/30.
//

#ifndef IMAGEEDITORGL_LOG_H
#define IMAGEEDITORGL_LOG_H

#include <android/log.h>

#define  LOG    "logger"

#define logd(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG,__VA_ARGS__) // 定义LOGD类型
#define logi(...)  __android_log_print(ANDROID_LOG_INFO,LOG,__VA_ARGS__) // 定义LOGI类型
#define logw(...)  __android_log_print(ANDROID_LOG_WARN,LOG,__VA_ARGS__) // 定义LOGW类型
#define loge(...)  __android_log_print(ANDROID_LOG_ERROR,LOG,__VA_ARGS__) // 定义LOGE类型

#endif //IMAGEEDITORGL_LOG_H
