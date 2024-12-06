#include "Camera.h"
//#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::radians;
using glm::mat4;
using glm::mat3;

Camera::Camera() : 
	viewDirection(0.0f, 0.0f, -1.0f), 
	UP(0.0f,1.0f,0.0f)
{

}

void Camera::mouseUpdate(const glm::vec2& newMousePosition) {
	glm::vec2 mouseDelta = newMousePosition - OldMPos;
	if (glm::length(mouseDelta) > 20.0f) {
		OldMPos = newMousePosition;
		return;
	}
	viewDirection = mat3(glm::rotate(mat4(1.0f), (float)(radians(mouseDelta.x)*(-0.5)), UP)) * viewDirection;
	glm::vec3 rotateVector = glm::cross(viewDirection, UP);
	viewDirection = mat3(glm::rotate(mat4(1.0f), (float)(radians(mouseDelta.y) * (-0.5)), rotateVector)) * viewDirection;
	OldMPos = newMousePosition;
}

void Camera::keyboardUpdate(float frontback, float leftright, float updown) {
	float MoveSpeed = 1.1f;
	glm::vec3 sideVector = glm::cross(viewDirection, UP);
	position += MoveSpeed * frontback * glm::vec3(viewDirection.x,0.0f,viewDirection.z);
	position += MoveSpeed * leftright * sideVector;
	position += MoveSpeed * updown * UP;

}

glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(position, position + viewDirection, UP);
}