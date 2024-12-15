#version 410

layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 Color;
//layout(location = 2) in vec2 texture;

//uniform mat3 fullTransformMat;

//out vec3 theColor;
//out vec2 texCoord;

void main(){
    gl_Position = vec4(position,1.0);
    //theColor = vec3(1.0,0.0,0.0);
}
