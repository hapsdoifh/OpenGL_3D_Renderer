//
// Created by Harry Wang on 2024-12-19.
//

#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>


using glm::vec3;
using glm::mat3;
using glm::mat4;

Camera::Camera():
cameraPos(0.0,0.0,0.0),
lookAt(0.0,0.0,-1.0),
cameraUP(0.0,1.0,0.0),
mouseLastPos(0.0,0.0)
{
}

void Camera::cameraUpdateKeyboard(int key, int action) {
    if(action != GLFW_PRESS && action != GLFW_REPEAT)
        return;
    switch(key) {
        case GLFW_KEY_A:
            cameraPos += 0.1f * glm::cross(cameraUP, lookAt);
            break;
        case GLFW_KEY_D:
            cameraPos -= 0.1f * glm::cross(cameraUP, lookAt);
            break;
        case GLFW_KEY_W:
            cameraPos += 0.1f * lookAt;
            break;
        case GLFW_KEY_S:
            cameraPos -= 0.1f * lookAt;
            break;
        case GLFW_KEY_SPACE:
            cameraPos += 0.1f * cameraUP;
            break;
        case GLFW_KEY_Z:
            cameraPos -= 0.1f * cameraUP;
            break;
        default:
            break;
    }
}

void Camera::cameraUpdateMouse(double x, double y) {
    //TODO add camera angle change
    double deltX = x - mouseLastPos.x;
    double deltY = y - mouseLastPos.y;
    mouseLastPos = glm::vec2(x,y);
    vec3 rotateAxisX = glm::cross(lookAt, cameraUP);
    lookAt = mat3(glm::rotate(mat4(1.0f),glm::radians(-0.1f * static_cast<float>(deltY)),rotateAxisX)) * lookAt;
    lookAt = mat3(glm::rotate(mat4(1.0f),glm::radians(-0.2f * static_cast<float>(deltX)),cameraUP)) * lookAt;
    lookAt += 0.001f * glm::vec3(-deltX, -deltY, 0.0f);
}


glm::mat4 Camera::worldToCamMatrix() {
    //it's the actual location i'm looking at (e.g. a point in space)
    //so if I only put the view direction I will end up always looking at some point like (0,0,-1)
    return glm::lookAt(cameraPos, lookAt + cameraPos, cameraUP);
}
