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
	std::vector<GLuint> tIds, qIds;
	for (GLuint id = 0; id < geometry.triangles.vertices.size() / 3; id++) {
		tIds.push_back(Mesh::faceCount + id);
	}

	Mesh::faceCount += GLuint(tIds.size());

	for (GLuint id = 0; id < geometry.quads.vertices.size() / 4; id++) {
		qIds.push_back(Mesh::faceCount + id);
	}

	Mesh::faceCount += GLuint(qIds.size());

	std::vector<GLuint> perVertexId;
	for (auto id : tIds) {
		perVertexId.push_back(id);
		perVertexId.push_back(id);
		perVertexId.push_back(id);
	}

	for (auto id : qIds) {
		perVertexId.push_back(id);
		perVertexId.push_back(id);
		perVertexId.push_back(id);
		perVertexId.push_back(id);
		perVertexId.push_back(id);
		perVertexId.push_back(id);
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

	if (geometry.triangles.emission.size() == 0 && geometry.quads.emission.size() == 0) {
		for (GLuint i = 0; i < this->vertices.size() / 3; i++) {
			this->emission.push_back(.0f);
			this->perVertexEmission.push_back(.0f);
			this->perVertexEmission.push_back(.0f);
			this->perVertexEmission.push_back(.0f);
		}
	}

	if (geometry.triangles.radiosity.size() == 0 && geometry.quads.radiosity.size() == 0) {
		for (GLuint i = 0; i < this->vertices.size() / 3; i++) {
			this->radiosity.push_back(.0f);
			this->perVertexRadiosity.push_back(.0f);
			this->perVertexRadiosity.push_back(.0f);
			this->perVertexRadiosity.push_back(.0f);
		}
	}
	this->vao = new VAO();
	this->vao->addAttribute(sizeof(glm::vec3) * vertices.size(), &vertices[0].x, 3, GL_FLOAT, 0);
	this->vao->addAttribute(sizeof(GLuint) * perVertexId.size(), &perVertexId[0], 1, GL_FLOAT, 1);
	this->vao->addAttribute(sizeof(GLfloat) * perVertexEmission.size(), &perVertexEmission[0], 1, GL_FLOAT, 2, GL_DYNAMIC_DRAW);
	this->vao->addAttribute(sizeof(GLfloat) * perVertexRadiosity.size(), &perVertexRadiosity[0], 1, GL_FLOAT, 3, GL_DYNAMIC_DRAW);
}

void Mesh::setRadiosity(std::vector<GLfloat> radiosity) {
	this->radiosity = radiosity;
	for (GLuint i = 0; i < radiosity.size(); i++) {
		GLfloat value = radiosity[i];
		this->perVertexRadiosity[3 * i] = value;
		this->perVertexRadiosity[3 * i + 1] = value;
		this->perVertexRadiosity[3 * i + 2] = value;
	}
	this->vao->updateAttribute(sizeof(GLfloat) * this->perVertexRadiosity.size(), &this->perVertexRadiosity[0], 1, GL_FLOAT, 3);
}

void Mesh::setEmission(GLuint faceIndex, GLfloat emission) {
	GLuint firstVertex = faceIndex * 3;
	this->perVertexEmission[firstVertex] = emission;
	this->perVertexEmission[firstVertex + 1] = emission;
	this->perVertexEmission[firstVertex + 2] = emission;
	this->emission[faceIndex] = emission;
	this->vao->updateAttribute(sizeof(GLfloat) * this->perVertexEmission.size(), &this->perVertexEmission[0], 1, GL_FLOAT, 2);
}

void Mesh::draw() {
	this->vao->bind();
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(this->vertices.size()));
	this->vao->unbind();
}

std::vector<glm::vec3> Mesh::getVertices() {
	return this->vertices;
}

std::vector<GLfloat> Mesh::getEmissions() {
	return this->emission;
}

GLfloat Mesh::getEmission(GLuint faceIndex) {
	return this->emission[faceIndex];
}

std::vector<GLfloat> Mesh::getRadiosity() {
	return this->radiosity;
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
