#version 300 es

precision mediump float;

layout(location = 0) in vec3 aPosition;

uniform mat3 transMat;
uniform float size;

void main(){
    gl_PointSize = size;
    gl_Position = vec4(transMat * aPosition ,1.0f);
}