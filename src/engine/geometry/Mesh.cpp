#include "geometry/Mesh.h"

#include <fstream>

#include "geometry/ObjLoader.h"

GLuint Mesh::faceCount = 1;

wvec3::wvec3(glm::vec3 &v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

bool wvec3::operator< (const glm::vec3 &v1) const {
	return (this->x < v1.x) || (this->x == v1.x && this->y < v1.y) || (this->x == v1.x && this->y == v1.y && this->z < v1.z);
}

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
	this->tFaces = geometry.triangles.vertices.size() / 3;
	this->qFaces = geometry.quads.vertices.size() / 4;
	this->faces = tFaces + qFaces;
	Mesh::faceCount += this->faces;

	this->vertices = geometry.triangles.vertices;
	for (GLuint i = 0; i < geometry.quads.vertices.size(); i += 4) {
		this->vertices.push_back(geometry.quads.vertices[i]);
		this->vertices.push_back(geometry.quads.vertices[i + 1]);
		this->vertices.push_back(geometry.quads.vertices[i + 3]);

		this->vertices.push_back(geometry.quads.vertices[i + 1]);
		this->vertices.push_back(geometry.quads.vertices[i + 2]);
		this->vertices.push_back(geometry.quads.vertices[i + 3]);

		adjacencies[geometry.quads.vertices[i]].push_back(tFaces + 1 + i / 4);
		adjacencies[geometry.quads.vertices[i + 1]].push_back(tFaces + 1 + i / 4);
		adjacencies[geometry.quads.vertices[i + 2]].push_back(tFaces + 1 + i / 4);
		adjacencies[geometry.quads.vertices[i + 3]].push_back(tFaces + 1 + i / 4);
	}

	for (GLuint i = 0; i < tFaces; i++) {
		adjacencies[this->vertices[3 * i]].push_back(i + 1);
		adjacencies[this->vertices[3 * i + 1]].push_back(i + 1);
		adjacencies[this->vertices[3 * i + 2]].push_back(i + 1);
	}

	if (geometry.triangles.emission.size() == 0 && geometry.quads.emission.size() == 0) {
		for (GLuint i = 0; i < this->faces; i++) {
			this->emission.push_back(.0f);
		}
		for (GLuint i = 0; i < this->vertices.size() / 3; i++) {
			this->perVertexEmission.push_back(0.0f);
			this->perVertexEmission.push_back(0.0f);
			this->perVertexEmission.push_back(0.0f);
		}
	}

	if (geometry.triangles.radiosity.size() == 0 && geometry.quads.radiosity.size() == 0) {
		for (GLuint i = 0; i < this->faces; i++) {
			this->radiosity.push_back(1.0f);
		}
		for (GLuint i = 0; i < this->vertices.size() / 3; i++) {
			this->perVertexRadiosity.push_back(1.0f);
			this->perVertexRadiosity.push_back(1.0f);
			this->perVertexRadiosity.push_back(1.0f);
		}
	}

	if (geometry.triangles.reflactance.size() == 0 && geometry.quads.reflactance.size() == 0) {
		for (GLuint i = 0; i < this->faces; i++) {
			this->reflactance.push_back(1.0f);
		}
		for (GLuint i = 0; i < this->vertices.size() / 3; i++) {
			this->perVertexReflactance.push_back(1.0f);
			this->perVertexReflactance.push_back(1.0f);
			this->perVertexReflactance.push_back(1.0f);
		}
	}

	this->vao = new VAO();
	this->vao->addAttribute(sizeof(glm::vec3) * vertices.size(), &vertices[0].x, 3, GL_FLOAT, 0);
	this->vao->addAttribute(sizeof(GLfloat) * perVertexEmission.size(), &perVertexEmission[0], 1, GL_FLOAT, 1, GL_DYNAMIC_DRAW);
	this->vao->addAttribute(sizeof(GLfloat) * perVertexRadiosity.size(), &perVertexRadiosity[0], 1, GL_FLOAT, 2, GL_DYNAMIC_DRAW);
	this->vao->addAttribute(sizeof(GLfloat) * perVertexRadiosity.size(), &perVertexReflactance[0], 1, GL_FLOAT, 3, GL_DYNAMIC_DRAW);
}

GLfloat interpolate(glm::vec3 &vertex, std::vector<GLfloat> &radiosities, std::vector<GLuint> &adjacencies) {
	GLfloat radiosity = 0;
	for (GLuint face : adjacencies) {
		radiosity += radiosities[face - 1];
	}
	return radiosity / (GLfloat)adjacencies.size();
}

void Mesh::setRadiosity(std::vector<GLfloat> radiosity, bool smooth) {
	this->radiosity = radiosity;
	/*if (smooth) {
		for (GLuint i = 0; i < tFaces; i++) {
			GLfloat value = radiosity[i];
			this->perVertexRadiosity[3 * i] = interpolate(this->vertices[3 * i], radiosity, this->adjacencies[wvec3(this->vertices[3 * i])]);
			this->perVertexRadiosity[3 * i + 1] = interpolate(this->vertices[3 * i + 1], radiosity, this->adjacencies[this->vertices[3 * i]]);
			this->perVertexRadiosity[3 * i + 2] = interpolate(this->vertices[3 * i + 2], radiosity, this->adjacencies[this->vertices[3 * i]]);
		}
		for (GLuint i = tFaces; i < faces; i++) {
			GLfloat value = radiosity[i];
			this->perVertexRadiosity[6 * i - 3 * tFaces] = interpolate(this->vertices[6 * i - 3 * tFaces], radiosity, this->adjacencies[this->vertices[6 * i - 3 * tFaces]]);
			this->perVertexRadiosity[6 * i - 3 * tFaces + 1] = interpolate(this->vertices[6 * i - 3 * tFaces + 1], radiosity, this->adjacencies[this->vertices[6 * i - 3 * tFaces]]);
			this->perVertexRadiosity[6 * i - 3 * tFaces + 2] = interpolate(this->vertices[6 * i - 3 * tFaces + 2], radiosity, this->adjacencies[this->vertices[6 * i - 3 * tFaces + 1]]);
			this->perVertexRadiosity[6 * i - 3 * tFaces + 3] = interpolate(this->vertices[6 * i - 3 * tFaces + 3], radiosity, this->adjacencies[this->vertices[6 * i - 3 * tFaces + 2]]);
			this->perVertexRadiosity[6 * i - 3 * tFaces + 4] = interpolate(this->vertices[6 * i - 3 * tFaces + 4], radiosity, this->adjacencies[this->vertices[6 * i - 3 * tFaces + 3]]);
			this->perVertexRadiosity[6 * i - 3 * tFaces + 5] = interpolate(this->vertices[6 * i - 3 * tFaces + 5], radiosity, this->adjacencies[this->vertices[6 * i - 3 * tFaces + 4]]);
		}
	}
	else {
		for (GLuint i = 0; i < tFaces; i++) {
			GLfloat value = radiosity[i];
			this->perVertexRadiosity[3 * i] = value;
			this->perVertexRadiosity[3 * i + 1] = value;
			this->perVertexRadiosity[3 * i + 2] = value;
		}
		for (GLuint i = tFaces; i < faces; i++) {
			GLfloat value = radiosity[i];
			this->perVertexRadiosity[6 * i - 3 * tFaces] = value;
			this->perVertexRadiosity[6 * i - 3 * tFaces + 1] = value;
			this->perVertexRadiosity[6 * i - 3 * tFaces + 2] = value;
			this->perVertexRadiosity[6 * i - 3 * tFaces + 3] = value;
			this->perVertexRadiosity[6 * i - 3 * tFaces + 4] = value;
			this->perVertexRadiosity[6 * i - 3 * tFaces + 5] = value;
		}
	}
	this->vao->updateAttribute(sizeof(GLfloat) * this->perVertexRadiosity.size(), &this->perVertexRadiosity[0], 1, GL_FLOAT, 2);*/
	this->radiosity = radiosity;
	for (GLuint i = 0; i < tFaces; i++) {
		GLfloat value = radiosity[i];
		this->perVertexRadiosity[3 * i] = value;
		this->perVertexRadiosity[3 * i + 1] = value;
		this->perVertexRadiosity[3 * i + 2] = value;
	}
	for (GLuint i = tFaces; i < faces; i++) {
		GLfloat value = radiosity[i];
		this->perVertexRadiosity[6 * i - 3 * tFaces] = value;
		this->perVertexRadiosity[6 * i - 3 * tFaces + 1] = value;
		this->perVertexRadiosity[6 * i - 3 * tFaces + 2] = value;
		this->perVertexRadiosity[6 * i - 3 * tFaces + 3] = value;
		this->perVertexRadiosity[6 * i - 3 * tFaces + 4] = value;
		this->perVertexRadiosity[6 * i - 3 * tFaces + 5] = value;
	}
	this->vao->updateAttribute(sizeof(GLfloat) * this->perVertexRadiosity.size(), &this->perVertexRadiosity[0], 1, GL_FLOAT, 2);
}

void Mesh::setEmission(GLuint faceIndex, GLfloat emission) {
	if (faceIndex < this->tFaces) {
		GLuint firstVertex = faceIndex * 3;
		this->perVertexEmission[firstVertex] = emission;
		this->perVertexEmission[firstVertex + 1] = emission;
		this->perVertexEmission[firstVertex + 2] = emission;
	}
	else if (faceIndex < this->faces) {
		GLuint firstVertex = 6 * faceIndex - 3 * tFaces;
		this->perVertexEmission[firstVertex] = emission;
		this->perVertexEmission[firstVertex + 1] = emission;
		this->perVertexEmission[firstVertex + 2] = emission;
		this->perVertexEmission[firstVertex + 3] = emission;
		this->perVertexEmission[firstVertex + 4] = emission;
		this->perVertexEmission[firstVertex + 5] = emission;
	}
	this->emission[faceIndex] = emission;
	this->vao->updateAttribute(sizeof(GLfloat) * this->perVertexEmission.size(), &this->perVertexEmission[0], 1, GL_FLOAT, 1);
}

void Mesh::setReflactance(GLuint faceIndex, GLfloat reflactance) {
	if (faceIndex < this->tFaces) {
		GLuint firstVertex = faceIndex * 3;
		this->perVertexReflactance[firstVertex] = reflactance;
		this->perVertexReflactance[firstVertex + 1] = reflactance;
		this->perVertexReflactance[firstVertex + 2] = reflactance;
	}
	else if (faceIndex < this->faces) {
		GLuint firstVertex = 6 * faceIndex - 3 * tFaces;
		this->perVertexReflactance[firstVertex] = reflactance;
		this->perVertexReflactance[firstVertex + 1] = reflactance;
		this->perVertexReflactance[firstVertex + 2] = reflactance;
		this->perVertexReflactance[firstVertex + 3] = reflactance;
		this->perVertexReflactance[firstVertex + 4] = reflactance;
		this->perVertexReflactance[firstVertex + 5] = reflactance;
	}
	this->reflactance[faceIndex] = reflactance;
	this->vao->updateAttribute(sizeof(GLfloat) * this->perVertexReflactance.size(), &this->perVertexReflactance[0], 1, GL_FLOAT, 3);
}

void Mesh::draw(GLuint shaderID) {
	GLuint tFacesLoc = glGetUniformLocation(shaderID, "tFaces");
	glUniform1ui(tFacesLoc, this->tFaces);
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

std::vector<GLfloat> Mesh::getReflactances() {
	return this->reflactance;
}

GLfloat Mesh::getReflactance(GLuint faceIndex) {
	return this->reflactance[faceIndex];
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
