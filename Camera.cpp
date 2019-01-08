#include "Camera.h"

#define _USE_MATH_DEFINES
#include <cmath> 

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>


Camera::Camera(GLuint width, GLuint height, float fov, float near, float far, glm::vec3 origin, glm::vec3 direction, glm::vec3 up) {
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

std::vector<glm::mat4> Camera::getCubeMatrices() {
	std::vector<glm::mat4> matrices;
	// Front
	matrices.push_back(this->getMVPMatrix());

	glm::vec3 dir = this->getDirection();

	// Right
	dir = glm::rotate(dir, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	matrices.push_back(this->projectionMatrix *  glm::lookAt(origin, origin + dir, up));

	// Left
	dir = glm::rotate(dir, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	matrices.push_back(this->projectionMatrix *  glm::lookAt(origin, origin + dir, up));

	// Top
	dir = glm::rotate(dir, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	matrices.push_back(this->projectionMatrix *  glm::lookAt(origin, origin + dir, up));

	// Bottom
	dir = glm::rotate(dir, -glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	matrices.push_back(this->projectionMatrix *  glm::lookAt(origin, origin + dir, up));


	// Back
	dir = glm::rotate(dir, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	matrices.push_back(this->projectionMatrix *  glm::lookAt(origin, origin + dir, glm::vec3(0.0f, 0.0f, 1.0f)));

	return matrices;
}

glm::mat4 Camera::getMVPMatrix() {
	return this->projectionMatrix * this->viewMatrix;
}


Camera::~Camera() {
}
