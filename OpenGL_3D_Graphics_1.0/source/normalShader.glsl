#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 texture;

uniform mat4 fullTransformMat;
uniform mat3 normalRotateMat;
uniform vec3 lightPos;

out vec3 theColor;
out vec2 texCoord;

void main(){
    vec4 v = vec4(position, 1.0);
    gl_Position = fullTransformMat * v;
    vec3 transformedNorm = normalRotateMat * normal;
    vec3 lightDir = normalize(lightPos - vec3(fullTransformMat*v));
    float intensity = max(dot(transformedNorm, lightDir),0.0f);
    theColor = intensity * vec3(1.0,1.0,1.0);
    //theColor = ambient;
}
