#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

#include "common/geometry/Face.h"
#include "common/geometry/VAO.h"
#include "common/geometry/GeometryContainer.h"

class Mesh {
public:
	static Mesh* load(std::string path);

	Mesh(GeometryBuffers geometry);

	void draw();

	std::vector<glm::vec3> getVertices();
	std::vector<unsigned int> getIds();
	GLuint size();
	Face getFace(GLuint index);

	virtual ~Mesh();

private:
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> tIds, qIds;

	VAO* vao;
	GLuint faces, tFaces, qFaces;

	static GLuint faceCount;

};

