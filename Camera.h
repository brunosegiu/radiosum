#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <SDL.h>
#include <mat4x4.hpp>

class Camera {
public:
	Camera(GLuint width, GLuint height, float fov, float near, float far, glm::vec3 origin, glm::vec3 direction, glm::vec3 up);

	void setOrigin(glm::vec3 origin);
	void setDirection(glm::vec3 direction);
	void setUp(glm::vec3 up);

	glm::vec3 getOrigin();
	glm::vec3 getDirection();
	glm::vec3 getUp();

	virtual ~Camera();

private:
	glm::vec3 origin, direction, up;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 MVPMatrix;
};

