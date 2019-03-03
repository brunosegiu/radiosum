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

	for (GLuint id = 0; id < geometry.triangles.vertices.size() / 3; id++) {
		this->tIds.push_back(id + Mesh::faceCount);
	}

	Mesh::faceCount += GLuint(this->tIds.size());

	for (GLuint id = 0; id < geometry.quads.vertices.size() / 4; id++) {
		this->qIds.push_back(id + Mesh::faceCount);
	}

	Mesh::faceCount += GLuint(this->qIds.size());

	std::vector<GLuint> replicatedIds;
	for (auto id : this->tIds) {
		replicatedIds.push_back(id);
		replicatedIds.push_back(id);
		replicatedIds.push_back(id);
	}

	for (auto id : this->qIds) {
		replicatedIds.push_back(id);
		replicatedIds.push_back(id);
		replicatedIds.push_back(id);
		replicatedIds.push_back(id);
		replicatedIds.push_back(id);
		replicatedIds.push_back(id);
	}

	this->tFaces = geometry.triangles.vertices.size() / 3;
	this->qFaces = geometry.quads.vertices.size() / 4;
	this->faces = tFaces + qFaces;

	this->vertices = geometry.triangles.vertices;
	for (GLuint i = 0; i < geometry.quads.vertices.size(); i += 4) {
		this->vertices.push_back(geometry.quads.vertices[i]);
		this->vertices.push_back(geometry.quads.vertices[i + 1]);
		this->vertices.push_back(geometry.quads.vertices[i + 3]);

		this->vertices.push_back(geometry.quads.vertices[i + 1]);
		this->vertices.push_back(geometry.quads.vertices[i + 2]);
		this->vertices.push_back(geometry.quads.vertices[i + 3]);
	}

	this->vao = new VAO();
	this->vao->addAttribute(sizeof(glm::vec3) * vertices.size(), &vertices[0].x, 3, GL_FLOAT, 0);
	if (geometry.triangles.textures.size())
		this->vao->addAttribute(sizeof(glm::vec2) * geometry.triangles.textures.size(), &geometry.triangles.textures[0].x, 2, GL_FLOAT, 1);
	if (geometry.triangles.normals.size())
		this->vao->addAttribute(sizeof(glm::vec3) * geometry.triangles.normals.size(), &geometry.triangles.normals[0].x, 3, GL_FLOAT, 2);
	this->vao->addAttribute(sizeof(GLuint) * replicatedIds.size(), &replicatedIds[0], 1, GL_UNSIGNED_INT, 3);
}

void Mesh::draw() {
	this->vao->bind();
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(this->vertices.size()));
	this->vao->unbind();
}

std::vector<glm::vec3> Mesh::getVertices() {
	return this->vertices;
}

std::vector<GLuint> Mesh::getIds() {
	return this->tIds;
}

GLuint Mesh::size() {
	return faces;
}

Face Mesh::getFace(GLuint index) {
	if (index < tFaces)
		return Face(this->vertices[3 * index], this->vertices[3 * index + 1], this->vertices[3 * index + 2]);
	else {
		GLuint startsAt = 6 * index - 3 * tFaces;
		return Face(this->vertices[startsAt], this->vertices[startsAt + 1], this->vertices[startsAt + 4], this->vertices[startsAt + 2]);
	}
}

Mesh::~Mesh() {
	delete this->vao;
}
