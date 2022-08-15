#version 300 es

precision mediump float;

uniform float mosaicSize;
uniform float originImageWidth;
uniform float originImageHeight;
uniform sampler2D baseImageTexture;

in vec3 originCoord;

layout(location = 0) out vec4 outColor;

void main(){
    vec2 uv = vec2(originCoord.x / originImageWidth ,1.0f - originCoord.y / originImageHeight);
    vec4 originColor = texture(baseImageTexture , uv);
    outColor = originColor;
}