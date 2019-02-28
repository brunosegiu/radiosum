#include "common/geometry/Mesh.h"

#include <fstream>

#include <common/geometry/ObjLoader.h>


GLuint Mesh::faceCount = 1;

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> textures, std::vector<glm::vec3> normals) {
	this->vertices = vertices;
	this->GLVerticesId, this->GLNormalsId = this->GLIdsId, this->GLVaoId = 0;

	for (GLuint id = 0; id < this->vertices.size() / 3; id++) {
		this->ids.push_back(id + Mesh::faceCount);
	}

	Mesh::faceCount += GLuint(this->ids.size());

	//Set up the VAO
	glGenVertexArrays(1, &GLVaoId);
	glBindVertexArray(GLVaoId);

	//Copy vertices to GPU
	glGenBuffers(1, &this->GLVerticesId);
	glBindBuffer(GL_ARRAY_BUFFER, this->GLVerticesId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);

	//Bind Vertices
	glBindBuffer(GL_ARRAY_BUFFER, this->GLVerticesId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Copy texture coords to GPU
	if (textures.size()) {
		glGenBuffers(1, &this->GLTexturesId);
		glBindBuffer(GL_ARRAY_BUFFER, this->GLTexturesId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textures.size(), &textures[0].x, GL_STATIC_DRAW);

		//Bind Texture Coords
		glBindBuffer(GL_ARRAY_BUFFER, this->GLTexturesId);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	//Copy normals to GPU
	if (normals.size()) {
		glGenBuffers(1, &this->GLNormalsId);
		glBindBuffer(GL_ARRAY_BUFFER, this->GLNormalsId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0].x, GL_STATIC_DRAW);

		//Bind Normals
		glBindBuffer(GL_ARRAY_BUFFER, this->GLNormalsId);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	//Copy ids to GPU
	std::vector<GLuint> replicatedIds;
	for (auto id : this->ids) {
		replicatedIds.push_back(id);
		replicatedIds.push_back(id);
		replicatedIds.push_back(id);
	}
	glGenBuffers(1, &this->GLIdsId);
	glBindBuffer(GL_ARRAY_BUFFER, this->GLIdsId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * replicatedIds.size(), &replicatedIds[0], GL_STATIC_DRAW);

	//Bind Ids
	glBindBuffer(GL_ARRAY_BUFFER, this->GLIdsId);
	glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, 0, (void*)0);

}

Mesh* Mesh::load(std::string path) {
	std::string fileExt = path.substr(path.find_last_of(".") + 1);
	std::ifstream input(path);

	std::vector<glm::vec3> vertices;

	if (fileExt == "obj") {
		AttributesBuffer buffers = loadOBJ(&input);
		return new Mesh(buffers.vertices, buffers.textures, buffers.normals);

	}

	throw std::runtime_error("Unable to load mesh, format not supported");
}

void Mesh::draw() {
	glBindVertexArray(this->GLVaoId);
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(this->vertices.size()));
	glDisableVertexAttribArray(0);
	//	glDisableVertexAttribArray(1);
	//	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

std::vector<glm::vec3> Mesh::getVertices() {
	return this->vertices;
}

std::vector<GLuint> Mesh::getIds() {
	return this->ids;
}

GLuint Mesh::size() {
	return GLuint(this->vertices.size() / 3);
}

Face Mesh::getFace(GLuint index) {
	return Face(this->vertices[index], this->vertices[index + 1], this->vertices[index + 2]);
}

Mesh::~Mesh() {
	glBindVertexArray(this->GLVaoId);
	glDeleteBuffers(1, &this->GLVerticesId);
	glDeleteBuffers(1, &this->GLIdsId);
	glDeleteVertexArrays(1, &this->GLVaoId);
}
