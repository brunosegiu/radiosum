#pragma once

#include <vector>

#include <glm.hpp>

typedef struct AttributesBuffers {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
} AttributesBuffers;

typedef struct GeometryBuffers {
	AttributesBuffers triangles;
	AttributesBuffers quads;
} GeometryBuffers;