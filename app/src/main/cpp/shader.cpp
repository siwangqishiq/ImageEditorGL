//
// Created by panyi on 2022/7/1.
//

#include "shader.h"
#include <fstream>
#include "glm/gtc/type_ptr.hpp"
#include "log.h"

static std::string SHADER_FOLDER = "/assets";

GLuint CreateGPUProgram(const char* vsShaderSource, const char* fsShaderSource) {
    GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsShaderSource);
    GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsShaderSource);

    //Attach
    GLuint program = glCreateProgram();
    glAttachShader(program, vsShader);
    glAttachShader(program, fsShader);

    GLint compileResult = GL_TRUE;
    glGetProgramiv(program, GL_COMPILE_STATUS, &compileResult);
    if(compileResult == GL_FALSE){
        char szLog[1024] = { 0 };
        GLsizei logLen = 0;
        glGetProgramInfoLog(program, 1024, &logLen, szLog);
        Loge("Compile program fail error log: %s \nvs shader code:\n%s\nfs shader code:\n%s\n" , szLog , vsShaderSource , fsShaderSource);
        glDeleteShader(program);
        program = 0;
    }

    //Link
    glLinkProgram(program);

    //Clear
    glDetachShader(program, vsShader);
    glDetachShader(program, fsShader);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);

    //check error
    GLint linkResult = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    if (linkResult == GL_FALSE) {
        char szLog[1024] = { 0 };
        GLsizei logLen = 0;
        glGetProgramInfoLog(program, 1024, &logLen, szLog);
        Loge("LogLen : %d" , logLen);
        Loge("Link program fail error log: %s \nvs shader code:\n%s\nfs shader code:\n%s\n" , szLog , vsShaderSource , fsShaderSource);
        glDeleteShader(program);
        program = 0;
    }

    Logi("program = %d" , program);
    return program;
}

//读取文本文件
std::string ReadFileAsText(std::string path){
    std::string content;
    std::ifstream infile(path);
    if(infile.is_open()){
        std::string line;
        while(std::getline(infile , line)){
            line += "\n";
            content += line;
        }//end while
    }else{
        Loge("open file : %s error" , path.c_str());
    }
    infile.close();
    return content;
}

//从文件中创建shader
GLuint CreateGPUProgramFromFile(std::string vertexShaderPath , std::string fragShaderPath){
    std::string vertexSrc = ReadFileAsText(vertexShaderPath);
    std::string fragSrc = ReadFileAsText(fragShaderPath);

    return CreateGPUProgram(vertexSrc.c_str() , fragSrc.c_str());
}

Shader Shader::buildGPUProgram(std::string vtxSrc , std::string frgSrc){
    Shader shader;
    auto  pid = CreateGPUProgram(vtxSrc.c_str() , frgSrc.c_str());
    shader.programId = pid;
    return shader;
}

GLuint CompileShader(GLenum shaderType, const char* shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        Loge("create shader failed : %s" , shaderSource);
        glDeleteShader(shader);
        return 0;
    }
    const char* shaderCode = shaderSource;
    if (shaderCode == nullptr) {
        Loge("load shader code from %s failed" , shaderSource);
        glDeleteShader(shader);
        return 0;
    }

    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    GLint compileResult = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        char szLog[1024] = { 0 };
        GLsizei logLen = 0;
        glGetShaderInfoLog(shader, 1024, &logLen, szLog);
        printf("Compile Shader fail error log: %s \nshader code:\n%s\n", szLog, shaderCode);
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

void Shader::useShader(){
    glUseProgram(this->programId);
}

void Shader::setUniformInt(std::string key , int value){
    GLint loc = findUniformLocation(key);
    glUniform1i(loc , value);
}

void Shader::setUniformFloat(std::string key , float value){
    GLint loc = findUniformLocation(key);
    glUniform1f(loc , value);
}

void Shader::setIUniformMat3(std::string key, glm::mat3 mat) {
    GLint loc = findUniformLocation(key);
    glUniformMatrix3fv(loc , 1 , GL_FALSE , glm::value_ptr(mat));
}

void Shader::setUniformMat4(std::string key , glm::mat4 mat){
    GLint loc = findUniformLocation(key);
    glUniformMatrix4fv(loc , 1 , GL_FALSE , glm::value_ptr(mat));
}

void Shader::setUniformVec4(std::string key , glm::vec4 vec){
    GLint loc = findUniformLocation(key);
    glUniform4fv(loc , 1 ,  (float *)(&vec[0]));
}

void Shader::setUniformVec3(std::string key , float x , float y , float z){
    GLint loc = findUniformLocation(key);
    glUniform3f(loc , x , y , z);
}

void Shader::setUniformVec3(std::string key ,glm::vec3 value){
    GLint loc = findUniformLocation(key);
    glUniform3fv(loc , 1 , (float *)(&value[0]));
}

void Shader::setUniformVec2(std::string key , float x , float y){
    GLint loc = findUniformLocation(key);
    glUniform2f(loc , x , y);
}

void Shader::setUniformBool(std::string key , bool value){
    GLint loc = findUniformLocation(key);
    glUniform1i(loc , value);
}

int Shader::findUniformLocation(std::string key){
    if(unifromLocs.find(key) != unifromLocs.end()){
        return unifromLocs[key];
    }

    const int loc = glGetUniformLocation(this->programId , key.c_str());
    if(loc >= 0){
        unifromLocs[key] = loc;
    }
    return loc;
}

