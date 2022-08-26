#version 300 es

precision mediump float;

uniform float mosaicSize;
uniform float originImageWidth;
uniform float originImageHeight;
uniform sampler2D baseImageTexture;

in vec3 originCoord;

layout(location = 0) out vec4 outColor;

vec2 mosaicEffect(){
    int leftIndex = int(originCoord.x) / int(mosaicSize);
    int rightIndex = leftIndex + 1;
    int bottomIndex = int(originCoord.y) / int(mosaicSize);
    int topIndex = bottomIndex + 1;

    vec2 leftBottomPoint;
    leftBottomPoint.x =  float(leftIndex) * mosaicSize;
    leftBottomPoint.y = float(bottomIndex) * mosaicSize;

    vec2 rightTopPoint;
    rightTopPoint.x = float(rightIndex) * mosaicSize;
    rightTopPoint.y = float(topIndex) * mosaicSize;

    return vec2((leftBottomPoint.x + rightTopPoint.x) / 2.0f , (leftBottomPoint.y + rightTopPoint.y) / 2.0f);
}

void main(){
    vec2 mosaicPoint = mosaicEffect();
    vec2 uv = vec2(mosaicPoint.x / originImageWidth ,1.0f - mosaicPoint.y / originImageHeight);
    vec4 originColor = texture(baseImageTexture , uv);
    outColor = originColor;
//    outColor = vec4(0.0f , 0.0f ,0.0f , 1.0f);
}