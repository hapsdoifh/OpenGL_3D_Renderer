#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 Texture;

uniform mat4 viewProjMat;
uniform mat4 modelWorldMat;
uniform vec3 lightPos;
uniform vec3 viewDir;

uniform bool disableLight;

out vec3 theColor;
out vec2 texCoord;

out vec3 transformedNorm;
out vec3 lightDir;

out vec3 fragPosition;

void main(){
    vec4 v = vec4(position, 1.0);
    mat3 normalRotateMat = mat3(modelWorldMat);

    transformedNorm = normalize(normalRotateMat * normal);
    lightDir = normalize(lightPos - vec3(modelWorldMat*v));

    theColor = Color;
    gl_Position = viewProjMat * modelWorldMat * v;
    fragPosition = vec3(gl_Position);
    texCoord = Texture;
}
