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

	void setEmission(GLuint faceIndex, GLfloat emission);
	void setRadiosity(std::vector<GLfloat> radiosity);

	std::vector<glm::vec3> getVertices();
	std::vector<GLfloat> getEmissions();
	GLfloat getEmission(GLuint faceIndex);
	std::vector<GLfloat> getRadiosity();

	GLuint size();
	Face getFace(GLuint index);

	virtual ~Mesh();

private:
	std::vector<glm::vec3> vertices;
	std::vector<GLfloat> emission;
	std::vector<GLfloat> radiosity;

	std::vector<GLfloat> perVertexEmission;
	std::vector<GLfloat> perVertexRadiosity;

	VAO* vao;
	GLuint faces, tFaces, qFaces;

	static GLuint faceCount;
};

