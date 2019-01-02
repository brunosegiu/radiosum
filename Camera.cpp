#include "Camera.h"

#define _USE_MATH_DEFINES
#include <cmath> 

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>


Camera::Camera(GLuint width, GLuint height, float fov = 45.0f, float near = 0.5f, float far = 5000.0f, glm::vec3 origin = glm::vec3(5.0f, 0, 0), glm::vec3 direction = glm::vec3(-1, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0)) {
	this->origin = origin;
	this->direction = glm::normalize(direction);
	this->up = glm::normalize(up);

	this->projectionMatrix = glm::perspective(glm::radians(fov), float(width) / float(height), near, far);
	this->viewMatrix = glm::lookAt(origin, origin + this->direction, up);
	this->MVPMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setOrigin(glm::vec3 origin) {
	this->origin = origin;
	this->viewMatrix = glm::lookAt(origin, origin + this->direction, up);
}

void Camera::setDirection(glm::vec3 direction) {
	this->direction = glm::normalize(direction);
	this->viewMatrix = glm::lookAt(origin, origin + this->direction, up);
}

void Camera::setUp(glm::vec3 up) {
	this->up = glm::normalize(up);
	this->viewMatrix = glm::lookAt(origin, origin + this->direction, this->up);
}

glm::vec3 Camera::getOrigin() {
	return this->origin;
}

glm::vec3 Camera::getDirection() {
	return this->direction;
}

glm::vec3 Camera::getUp() {
	return this->up;
}


Camera::~Camera() {
}
