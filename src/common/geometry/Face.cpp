#include "common/geometry/Face.h"



Face::Face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
}

glm::vec3 Face::getNormal() {
	glm::vec3 u = v1 - v0;
	glm::vec3 v = v2 - v1;
	return glm::normalize(glm::cross(u, v));
}

glm::vec3 Face::getBarycenter() {
	return (v0 + v1 + v2) / 3.0f;
}

glm::vec4 Face::getPlane() {
	glm::vec3 normal = this->getNormal();
	float d = glm::dot(normal, -v0);
	return glm::vec4(normal, d);
}

glm::vec3 Face::getVertex(GLuint index) {
	switch (index) {
	case (0):
		return v0;
	case (1):
		return v1;
	case (2):
		return v2;
	}
}

Face::~Face() {

}