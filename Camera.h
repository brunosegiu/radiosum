#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <SDL.h>
#include <mat4x4.hpp>

class Camera {
public:
	Camera(GLuint width, GLuint height, float fov = 45.0f, float near = 0.5f, float far = 5000.0f, glm::vec3 origin = glm::vec3(5.0f, 0, 0), glm::vec3 direction = glm::vec3(-1, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0));

	void setOrigin(glm::vec3 origin);
	void setDirection(glm::vec3 direction);
	void setUp(glm::vec3 up);

	glm::vec3 getOrigin();
	glm::vec3 getDirection();
	glm::vec3 getUp();

	glm::mat4 getMVPMatrix();

	virtual ~Camera();

private:
	glm::vec3 origin, direction, up;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 MVPMatrix;
};

