#version 300 es

layout(location = 0) in vec2 aPos;

uniform mat3 uMat;

void main() {
    gl_Position = vec4(aPos ,1.0 , 1.0);
}