#version 410

in vec3 theColor;
//in vec2 texCoord;

out vec4 fragColor;

void main(){
    fragColor = vec4(theColor, 1.0);
}

