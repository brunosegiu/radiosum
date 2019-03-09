#pragma once

#include <GL/glew.h>
#include <glm.hpp>

enum FaceMode { TRIANGLE, QUAD };

class Face {
public:
	Face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
	Face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	glm::vec3 getNormal();
	glm::vec3 getBarycenter();
	glm::vec4 getPlane();
	GLfloat area();
	glm::vec3 getVertex(GLuint index);
	virtual ~Face();
private:
	glm::vec3 v0, v1, v2, v3;
	FaceMode mode;
};
