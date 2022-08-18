#version 300 es

precision mediump float;

layout(location = 0) in vec3 aPosition;

out vec3 originCoord;

uniform mat3 transMat;
uniform float mosaicSize;

void main(){
    gl_PointSize = mosaicSize;
    originCoord = aPosition;
    gl_Position = vec4(transMat * aPosition ,1.0f);
}