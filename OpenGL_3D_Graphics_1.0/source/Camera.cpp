//
// Created by Harry Wang on 2024-12-19.
//

#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>


using glm::vec3;

Camera::Camera():
cameraPos(0.0,0.0,0.0),
cameraUP(0.0,1.0,0.0),
lookAt(0.0,0.0,-1.0)
{
}

void Camera::cameraUpdateKeyboard(int key, int action) {
    if(action != GLFW_REPEAT)
        return;
    switch(key) {
        case GLFW_KEY_A:
            cameraPos += glm::cross(cameraUP, lookAt);
            break;
        case GLFW_KEY_D:
            cameraPos -= glm::cross(cameraUP, lookAt);
            break;
        case GLFW_KEY_W:
            cameraPos += lookAt;
            break;
        case GLFW_KEY_S:
            cameraPos -= lookAt;
            break;
        case GLFW_KEY_SPACE:
            cameraPos += cameraUP;
            break;
        case GLFW_KEY_Z:
            cameraPos -= cameraUP;
            break;
        default:
            break;
    }
}

void Camera::cameraUpdateMouse() {
    //TODO add camera angle change
}


glm::mat4 Camera::worldToCamMatrix() {
    return glm::lookAt(cameraPos, lookAt, cameraUP);
}
