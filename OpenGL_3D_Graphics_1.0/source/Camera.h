//
// Created by Harry Wang on 2024-12-19.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glfw3.h>


class Camera {
    glm::vec3 cameraPos;
    glm::vec3 lookAt;
    glm::vec3 cameraUP;
public:
    Camera();
    void cameraUpdateMouse();
    void cameraUpdateKeyboard(int key, int action);
    glm::mat4 worldToCamMatrix();
};



#endif //CAMERA_H
