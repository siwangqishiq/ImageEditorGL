#version 300 es

precision mediump float;

uniform float mosaicSize;
uniform float originImageWidth;
uniform float originImageHeight;
uniform sampler2D baseImageTexture;

in vec3 originCoord;

layout(location = 0) out vec4 outColor;

vec3 mosaicEffect(){
    int leftIndex = int(originCoord.x) / int(mosaicSize);
    int rightIndex = leftIndex + 1;
    int bottomIndex = int(originCoord.y) / int(mosaicSize);
    int topIndex = bottomIndex + 1;

    vec3 colorSum;
    int count=0;
    for(int i = leftIndex ; i <= rightIndex;i++){
        for(int j = bottomIndex; j<= topIndex ;j++){
            float x = float(i) * mosaicSize;
            float y = float(j) * mosaicSize;
            vec2 uv = vec2(x / originImageWidth , y / originImageHeight);
            vec4 originColor = texture(baseImageTexture , uv);

            colorSum = colorSum + originColor.rgb;
            count++;
        }//end for j
    }//end for i
    return vec3(colorSum.r / float(count) , colorSum.g / float(count) ,  colorSum.b / float(count));
}

void main(){
//    vec2 mosaicPoint = mosaicEffect();
//    vec2 uv = vec2(mosaicPoint.x / originImageWidth ,1.0f - mosaicPoint.y / originImageHeight);
//    vec4 originColor = texture(baseImageTexture , uv);
//    outColor = originColor;
//    outColor = vec4(1.0f , 0.0f ,0.0f , 1.0f);
    outColor = vec4(mosaicEffect() , 1.0f);
}