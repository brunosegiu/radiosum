#pragma once

#include <vector>

#include <glm.hpp>

typedef struct AttributesBuffers {
	std::vector<glm::vec3> vertices;
	std::vector<GLfloat> emission;
	std::vector<GLfloat> radiosity;
	std::vector<GLfloat> reflactance;
} AttributesBuffers;

typedef struct GeometryBuffers {
	AttributesBuffers triangles;
	AttributesBuffers quads;
} GeometryBuffers;