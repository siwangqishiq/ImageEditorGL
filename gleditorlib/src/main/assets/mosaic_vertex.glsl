#version 300 es

layout(location = 0) in vec3 aPosition;

out vec3 originCoord;

uniform mat3 transMat;

void main(){
    gl_PointSize = 30.0f;
    originCoord = aPosition;
    gl_Position = vec4(transMat * aPosition ,1.0f);
}