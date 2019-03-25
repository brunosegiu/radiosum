#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

#include "geometry/Face.h"
#include "geometry/VAO.h"
#include "geometry/GeometryContainer.h"



class wvec3 : public glm::vec3 {
public:
	wvec3(glm::vec3 &v);
	bool operator< (const glm::vec3 &v1) const;
};

class Mesh {
public:
	static std::vector<Mesh*> load(std::string path);

	Mesh(IndexedBuffers geometry);

	void draw(GLuint shaderID);
	void drawGeometry(GLuint shaderID);

	void setEmission(GLuint faceIndex, GLfloat emission);
	void setReflactance(GLuint faceIndex, glm::vec3 reflactance);
	void setEmission(GLfloat emission);
	void setReflactance(glm::vec3 reflactance);
	void setRadiosity(std::vector<glm::vec3> radiosity, bool smooth = true);

	IndexedBuffers getGeometry();
	std::vector<GLfloat> getEmissions();
	GLfloat getEmission(GLuint faceIndex);
	std::vector<glm::vec3> getReflactances();
	glm::vec3 getReflactance(GLuint faceIndex);
	std::vector<glm::vec3> getRadiosity();

	GLuint size();
	Face getFace(GLuint index);

	virtual ~Mesh();

private:
	IndexedBuffers geometry;
	std::vector<GLfloat> emission;
	std::vector<glm::vec3> radiosity;
	std::vector<glm::vec3> reflactance;

	std::vector<std::vector<GLuint>> adjacencies;

	VAO* vao, *geometryVao;
	GLuint faces, tFaces, qFaces;

	static GLuint faceCount;

	// Internal methods
	void drawVao(GLuint shader, VAO* vao);
};

