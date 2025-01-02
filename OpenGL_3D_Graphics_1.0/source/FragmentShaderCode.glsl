#version 410

in vec3 theColor;
in vec2 texCoord;

in vec3 transformedNorm;
in vec3 lightDir;

uniform bool disableLight;
uniform vec3 ambient;

uniform sampler2D texture0;

out vec4 fragColor;

void main(){
    if(theColor != vec3(0.0)){
        fragColor = vec4(theColor, 1.0);
    }else{
        fragColor = vec4(texture(texture0, texCoord).rgb, 1.0);
    }
    if(disableLight)
        return;

    float intensity = max(dot(lightDir, transformedNorm), 0.0f);
    fragColor = fragColor * (vec4(intensity) + vec4(ambient, 0.0));
}

