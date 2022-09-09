#version 300 es

precision mediump float;

layout(location = 0) in vec3 aPosition;

uniform mat3 transMat;

void main(){
    gl_Position = vec4(transMat * aPosition ,1.0f);
}