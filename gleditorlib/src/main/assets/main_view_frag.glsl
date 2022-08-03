#version 300 es

precision mediump float;

uniform sampler2D sTexture;

in vec2 vUv;
out vec4 outColor;

void main(){
    vec4 originColor = texture(sTexture , vUv);
    outColor = originColor.rgba;
}