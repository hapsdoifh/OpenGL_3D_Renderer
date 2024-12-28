#version 410

in vec3 theColor;
in vec2 texCoord;

uniform sampler2D texture0;

out vec4 fragColor;

void main(){
    //fragColor = vec4(theColor, 1.0);
    fragColor = texture(texture0, texCoord);
}

