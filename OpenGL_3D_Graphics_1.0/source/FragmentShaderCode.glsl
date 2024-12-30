#version 410

in vec3 theColor;
in vec2 texCoord;
in float lightLevel;

uniform sampler2D texture0;

out vec4 fragColor;

void main(){
    if(theColor != vec3(0.0)){
        fragColor = vec4(theColor, 1.0);
    }
    else{
        vec3 baseTexture = (lightLevel + 0.2) * texture(texture0, texCoord).rgb;
        fragColor = vec4(baseTexture, 1.0);
    }
}

