#version 300 es

precision mediump float;

uniform vec4 pointColor;

out vec4 outColor;

void main(){
    vec2 coord = gl_PointCoord - vec2(0.5);
    if(length(coord) > 0.5){
        discard;
    }
//    outColor = pointColor;
    outColor = vec4(1.0f , 1.0f , 0.0f , 1.0f);
}