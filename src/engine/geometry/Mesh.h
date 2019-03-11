#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

#include "geometry/Face.h"
#include "geometry/VAO.h"
#include "geometry/GeometryContainer.h"

class Mesh {
public:
	static Mesh* load(std::string path);

	Mesh(GeometryBuffers geometry);

	void draw(GLuint shaderID);

	void setEmission(GLuint faceIndex, GLfloat emission);
	void setReflactance(GLuint faceIndex, GLfloat reflactance);
	void setRadiosity(std::vector<GLfloat> radiosity);

	std::vector<glm::vec3> getVertices();
	std::vector<GLfloat> getEmissions();
	GLfloat getEmission(GLuint faceIndex);
	std::vector<GLfloat> getReflactances();
	GLfloat getReflactance(GLuint faceIndex);
	std::vector<GLfloat> getRadiosity();

	GLuint size();
	Face getFace(GLuint index);

	virtual ~Mesh();

private:
	std::vector<glm::vec3> vertices;
	std::vector<GLfloat> emission;
	std::vector<GLfloat> radiosity;
	std::vector<GLfloat> reflactance;

	std::vector<GLfloat> perVertexEmission;
	std::vector<GLfloat> perVertexRadiosity;
	std::vector<GLfloat> perVertexReflactance;
	std::map<glm::vec3, std::vector<GLuint>> adjacencies;

	VAO* vao;
	GLuint faces, tFaces, qFaces;

	static GLuint faceCount;
};

