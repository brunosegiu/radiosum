#include "common/geometry/Mesh.h"

#include <fstream>

#include "common/geometry/ObjLoader.h"

GLuint Mesh::faceCount = 1;


Mesh* Mesh::load(std::string path) {
	std::string fileExt = path.substr(path.find_last_of(".") + 1);
	std::ifstream input(path);

	std::vector<glm::vec3> vertices;

	if (fileExt == "obj") {
		GeometryBuffers buffers = loadOBJ(&input);
		return new Mesh(buffers);

	}

	throw std::runtime_error("Unable to load mesh, format not supported");
}

Mesh::Mesh(GeometryBuffers geometry) {
	this->tVertices = geometry.triangles.vertices;
	this->qVertices = geometry.quads.vertices;

	for (GLuint id = 0; id < this->tVertices.size() / 3; id++) {
		this->tIds.push_back(id + Mesh::faceCount);
	}

	Mesh::faceCount += GLuint(this->tIds.size());

	for (GLuint id = 0; id < this->qVertices.size() / 4; id++) {
		this->qIds.push_back(id + Mesh::faceCount);
	}

	Mesh::faceCount += GLuint(this->qIds.size());

	std::vector<GLuint> tReplicatedIds;
	for (auto id : this->tIds) {
		tReplicatedIds.push_back(id);
		tReplicatedIds.push_back(id);
		tReplicatedIds.push_back(id);
	}

	std::vector<GLuint> qReplicatedIds;
	for (auto id : this->qIds) {
		qReplicatedIds.push_back(id);
		qReplicatedIds.push_back(id);
		qReplicatedIds.push_back(id);
		qReplicatedIds.push_back(id);
	}

	// Set up triangles VAO
	if (tVertices.size() > 0) {
		this->tVao = new VAO();
		this->tVao->addAttribute(sizeof(glm::vec3) * tVertices.size(), &tVertices[0].x, 3, GL_FLOAT, 0);
		if (geometry.triangles.textures.size())
			this->tVao->addAttribute(sizeof(glm::vec2) * geometry.triangles.textures.size(), &geometry.triangles.textures[0].x, 2, GL_FLOAT, 1);
		if (geometry.triangles.normals.size())
			this->tVao->addAttribute(sizeof(glm::vec3) * geometry.triangles.normals.size(), &geometry.triangles.normals[0].x, 3, GL_FLOAT, 2);
		this->tVao->addAttribute(sizeof(GLuint) * tReplicatedIds.size(), &tReplicatedIds[0], 1, GL_UNSIGNED_INT, 3);
	}

	// Set up quads VAO
	if (qVertices.size() > 0) {
		this->qVao = new VAO();
		this->qVao->addAttribute(sizeof(glm::vec3) * qVertices.size(), &qVertices[0].x, 3, GL_FLOAT, 0);
		if (geometry.quads.textures.size())
			this->qVao->addAttribute(sizeof(glm::vec2) * geometry.quads.textures.size(), &geometry.quads.textures[0].x, 2, GL_FLOAT, 1);
		if (geometry.quads.normals.size())
			this->qVao->addAttribute(sizeof(glm::vec3) * geometry.quads.normals.size(), &geometry.quads.normals[0].x, 3, GL_FLOAT, 2);
		this->qVao->addAttribute(sizeof(GLuint) * qReplicatedIds.size(), &qReplicatedIds[0], 1, GL_UNSIGNED_INT, 3);
	}
}

void Mesh::draw() {
	if (tVertices.size() > 0) {
		this->tVao->bind();
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(this->tVertices.size()));
		this->tVao->unbind();
	}
	if (qVertices.size() > 0) {
		this->qVao->bind();
		glDrawArrays(GL_QUADS, 0, GLsizei(this->qVertices.size()));
		this->qVao->unbind();
	}
}

std::vector<glm::vec3> Mesh::getVertices() {
	return this->tVertices;
}

std::vector<GLuint> Mesh::getIds() {
	return this->tIds;
}

GLuint Mesh::size() {
	return GLuint((this->tVertices.size() / 3) + (this->qVertices.size() / 4));
}

Face Mesh::getFace(GLuint index) {
	if (index < this->tVertices.size())
		return Face(this->tVertices[index], this->tVertices[index + 1], this->tVertices[index + 2]);
	else
		return Face(this->qVertices[index], this->qVertices[index + 1], this->qVertices[index + 2], this->qVertices[index + 3]);
}

Mesh::~Mesh() {
	if (tVertices.size() > 0)
		delete this->tVao;
	if (qVertices.size() > 0)
		delete this->qVao;
}
