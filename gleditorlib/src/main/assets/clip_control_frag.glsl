#version 300 es

precision mediump float;

layout(location = 0) out vec4 outColor;

vec4 blend(vec4 src , vec4 dst){
    float dst_length =length(dst);
    return step(dst_length , 0.0) * src + step(0.0, dst_length) * dst;
}

vec4 circle(vec2 pos){
    vec2 center = vec2(0.5f , 0.5f);
    vec4 color =vec4(0.878f, 0.878f, 0.878f ,1.0f);
    return color * (1.0f - step(0.4f , length(pos - center)));
}

vec4 blurCircle(vec2 pos){
    vec2 offset = vec2(0.02f , 0.02f);
    vec2 center = vec2(0.5f , 0.5f) + offset;
    vec4 color =vec4(0.0f , 0.0f , 0.0f ,1.0f);
    return color * (1.0f - smoothstep(0.2f , 0.43f , length(pos - center)));
}

void main(){
    vec4 result;
    vec2 pos = gl_PointCoord;
    result = blurCircle(pos);
    vec4 circleColor = circle(pos);
    result = blend(result , circleColor);

    outColor = result;
}