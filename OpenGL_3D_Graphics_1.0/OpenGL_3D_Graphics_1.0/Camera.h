#pragma once
#include <glm/glm.hpp>

class Camera
{
//default private
	glm::vec3 position;
	glm::vec3 viewDirection; //Need to be unit vector
	glm::vec3 UP;
	glm::vec2 OldMPos;
public:
	Camera();
	glm::mat4 getWorldToViewMatrix() const;
	void mouseUpdate(const glm::vec2& newMousePosition);
	void keyboardUpdate(float frontback, float leftright, float updown);
};

 