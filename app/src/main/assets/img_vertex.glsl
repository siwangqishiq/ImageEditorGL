#version 300 es

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texture;

out vec2 vUv;

void main(){
    gl_Position = vec4(a_position.xy, 0.0f ,1.0f);
    vUv = a_texture;
}