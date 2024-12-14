#version 410
in theColor;
in texCoord;

out vec3 fragColor;

void main(){
    fragColor = theColor;
}
