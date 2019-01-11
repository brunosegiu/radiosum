#pragma once

#include <glm.hpp>

class Face {
public:
	Face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
	glm::vec3 getNormal();
	glm::vec3 getBarycenter();
	glm::vec4 getPlane();
	virtual ~Face();
private:
	glm::vec3 v0, v1, v2;
};

