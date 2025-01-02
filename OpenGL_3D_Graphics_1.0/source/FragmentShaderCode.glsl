#version 410

in vec3 theColor;
in vec2 texCoord;

in vec3 transformedNorm;
in vec3 lightDir;
in vec3 fragPosition;

uniform bool disableLight;
uniform vec3 ambient;
uniform vec3 viewPos;

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

    vec3 viewDir = normalize(fragPosition - viewPos);
    vec3 reflected = reflect(-lightDir, transformedNorm);
    float specularLight = 1 * pow(max(dot(reflected, viewDir),0), 32);
    float diffuseLight = max(dot(lightDir, transformedNorm), 0.0f);
    fragColor = fragColor * (specularLight + vec4(diffuseLight) + vec4(ambient, 0.0));
}

