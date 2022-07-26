#version 300 es

precision mediump float;

uniform vec4 pointColor;

out vec4 outColor;

void main(){
    outColor = pointColor;
}