#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

class Mesh {
public:
	static Mesh* load(std::string path);

	Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals);

	void draw();

	std::vector<glm::vec3> getVertices();
	std::vector<unsigned int> getIds();
	GLuint size();

	virtual ~Mesh();

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> ids;

	static GLuint faceCount;
	GLuint GLVerticesId, GLNormalsId, GLIdsId, GLVaoId;
};

