#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texture;

uniform mat4 viewProjMat;
uniform mat4 modelWorldMat;
uniform vec3 lightPos;
uniform vec3 ambient;

out vec3 theColor;
out vec2 texCoord;

void main(){
    vec4 v = vec4(position, 1.0);
    mat3 normalRotateMat = mat3(modelWorldMat);

    gl_Position = viewProjMat * modelWorldMat * v;

    vec3 transformedNorm = normalize(normalRotateMat * normal);
    vec3 lightDir = normalize(lightPos - vec3(modelWorldMat*v));
    float intensity = max(dot(transformedNorm, lightDir),0.0f);
    theColor = (intensity + ambient) * Color;
    //theColor = vec3(1.0,1.0,1.0);
}
