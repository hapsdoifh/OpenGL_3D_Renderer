#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec2 texture;

uniform mat4 fullTransformMat;

out vec3 theColor;
out vec2 texCoord;

void main(){
    vec4 v = vec4(position, 1.0);
    gl_Position = fullTransformMat * v;
    theColor = vec3(1.0,0.0,0.0);
}
