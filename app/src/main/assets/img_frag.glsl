#version 300 es

precision mediump float;

uniform sampler2D sTexture;

in vec2 vUv;
out vec4 out_color;

void main(){
    out_color = texture(sTexture , vUv);
}