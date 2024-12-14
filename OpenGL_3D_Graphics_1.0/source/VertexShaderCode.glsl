in layout(location = 0) vec3 position;
in layout(location = 1) vec3 Color;
in layout(location = 2) vec2 texture;

uniform mat4 fullTransformMat;

out theColor;
out texCoord;

void main(){
    gl_Position = fullTransformMat * position;
    theColor = Color;
}