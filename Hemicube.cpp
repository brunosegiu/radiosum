#include "Hemicube.h"



Hemicube::Hemicube(GLuint width) {
	this->front = new UIntBuffer(width, width);
	this->half = new UIntBuffer(width, width / 2);
	this->width = width;
}

void Hemicube::bind(Face face) {
	if (face == FRONT) {
		this->front->bind();
	}
	else {
		this->half->bind();
	}
	this->boundFace = face;
}

Camera* Hemicube::getCamera(Camera* frontFacingCam) {
	glm::vec3 origin = frontFacingCam->getOrigin();
	glm::vec3 frontDirection = frontFacingCam->getDirection();
	glm::vec3 direction;
	glm::vec3 up = frontFacingCam->getUp();
	GLuint width = this->width;
	GLuint height = this->width;

	switch (this->boundFace) {
	case (FRONT): {
		direction = frontDirection;
	} break;
	case (RIGHT): {
		direction = frontDirection;
		height = width / 2;
	} break;
	case (LEFT): {
		direction = frontDirection;
		height = width / 2;
	} break;
	case (TOP): {
		direction = frontDirection;
		height = width / 2;
	} break;
	case (BOTTOM): {
		direction = frontDirection;
		height = width / 2;
	} break;
	}

	if (this->internalCam)
		delete this->internalCam;
	this->internalCam = new Camera(width, height, 45.0f, 0.5f, 5000.0f, origin, direction, up);
	return this->internalCam;
}

void Hemicube::clean() {
	this->front->clean();
	this->half->clean();
}

void Hemicube::read() {
	if (this->boundFace == FRONT) {
		this->front->read();
	}
	else {
		this->half->read();
	}
}

Hemicube::~Hemicube() {
	delete this->half;
	delete this->front;
}
