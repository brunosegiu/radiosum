#include "Camera.h"

#define _USE_MATH_DEFINES
#include <cmath> 

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>


Camera::Camera(float aspectRatio, float fov, float near, float far, glm::vec3 origin, glm::vec3 direction, glm::vec3 up) {
	this->origin = origin;
	this->direction = glm::normalize(direction);
	this->up = glm::normalize(up);

	this->projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
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
	glm::vec3 right = glm::cross(dir, up);
	matrices.push_back(this->projectionMatrix *  glm::lookAt(origin, origin + right, up));

	// Bottom
	glm::vec3 bottom = glm::rotate(right, glm::radians(90.0f), dir);
	matrices.push_back(this->projectionMatrix *  glm::lookAt(origin, origin + bottom, up));

	// Left
	glm::vec3 left = glm::rotate(bottom, glm::radians(90.0f), dir);
	matrices.push_back(this->projectionMatrix *  glm::lookAt(origin, origin + left, up));

	// Top
	glm::vec3 top = glm::rotate(left, glm::radians(90.0f), dir);
	matrices.push_back(this->projectionMatrix *  glm::lookAt(origin, origin + top, up));

	return matrices;
}

glm::mat4 Camera::getMVPMatrix() {
	return this->projectionMatrix * this->viewMatrix;
}


Camera::~Camera() {
}
