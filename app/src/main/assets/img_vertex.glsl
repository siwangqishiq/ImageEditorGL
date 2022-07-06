#version 300 es

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture;

uniform mat3 normalMat;
uniform mat3 transMat;

out vec2 vUv;

void main(){
    gl_Position = vec4( normalMat * transMat * a_position ,1.0f);
    vUv = a_texture;
}