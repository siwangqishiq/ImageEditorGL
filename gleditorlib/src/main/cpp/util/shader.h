//
// Created by panyi on 2022/7/1.
//
#pragma once

#include <GLES3/gl3.h>
#include <string>
#include "../glm/glm.hpp"
#include <unordered_map>
#include <map>
#include "log.h"

//读取asset 文本文件
std::string ReadAssetTextFile(std::string filename);

//编译shader源码
GLuint CompileShader(GLenum shaderType, const char* shaderSource);

//创建完整Shader
GLuint CreateGPUProgram(const char* vsShaderSource, const char* fsShaderSource);

GLuint CreateGPUProgramByAsset(std::string vsFilePath , std::string fsFilePath);

//读取文本文件
std::string ReadFileAsText(std::string path);

class Shader{
public:
    static Shader buildGPUProgram(std::string vtxSrc , std::string frgSrc);

    static Shader buildGPUProgramAssetFile(std::string vtxSrc , std::string frgSrc);

    //使用shader
    void useShader();

    //
    void setUniformInt(std::string key , int value);

    void setUniformFloat(std::string key , float value);

    //
    void setUniformMat4(std::string key , glm::mat4 mat);

    void setIUniformMat3(std::string key , glm::mat3 mat);

    //设置三维向量
    void setUniformVec3(std::string key , float x , float y , float z);

    //设置二维向量
    void setUniformVec2(std::string key , float x , float y);

    //设置三维向量
    void setUniformVec3(std::string key ,glm::vec3 value);

    //设置四维向量
    void setUniformVec4(std::string key , glm::vec4 vec);

    void setUniformBool(std::string key , bool value);

    int findUniformLocation(std::string key);

    unsigned int programId;
protected:
    std::unordered_map<std::string , int> unifromLocs; //unifrom变量loccation
};

class ShaderManager{
private:
    std::map<std::string , Shader> shaderMap;

public:
    static ShaderManager& getInstance();

    Shader fetchShader(std::string shaderName , std::string vertexSrc , std::string frgSrc);

    Shader fetchShaderByPath(std::string shaderName , std::string vertexPath , std::string fragPath);

    void clear();
};
