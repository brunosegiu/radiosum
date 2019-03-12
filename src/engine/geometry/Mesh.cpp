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

std::vector<glm::vec3> triangulate(std::vector<glm::vec3> &quad) {
	glm::vec3 v0 = quad[0];
	glm::vec3 v1 = quad[1];
	glm::vec3 v2 = quad[2];
	glm::vec3 v3 = quad[3];

	std::vector<glm::vec3> triangles;
	triangles.reserve(6);

	triangles.push_back(v0);
	triangles.push_back(v1);
	triangles.push_back(v3);

	triangles.push_back(v1);
	triangles.push_back(v2);
	triangles.push_back(v3);

	return triangles;
}

Mesh::Mesh(GeometryBuffers geometry) {
	this->tFaces = geometry.triangles.vertices.size() / 3;
	this->qFaces = geometry.quads.vertices.size() / 4;
	this->faces = tFaces + qFaces;
	Mesh::faceCount += this->faces;

	// Attach triangles
	this->vertices = geometry.triangles.vertices;
	for (GLuint i = 0; i < tFaces; i++) {
		adjacencies[this->vertices[3 * i]].push_back(i + 1);
		adjacencies[this->vertices[3 * i + 1]].push_back(i + 1);
		adjacencies[this->vertices[3 * i + 2]].push_back(i + 1);
	}

	// Attach quads
	for (GLuint i = 0; i < geometry.quads.vertices.size(); i += 4) {

		glm::vec3 v0 = geometry.quads.vertices[i + 0];
		glm::vec3 v1 = geometry.quads.vertices[i + 1];
		glm::vec3 v2 = geometry.quads.vertices[i + 2];
		glm::vec3 v3 = geometry.quads.vertices[i + 3];

		std::vector<glm::vec3> quad({ v0,v1,v2,v3 });
		std::vector<glm::vec3> triangles = triangulate(quad);
		this->vertices.insert(this->vertices.end(), triangles.begin(), triangles.end());

		GLuint faceID = tFaces + 1 + i / 4;
		adjacencies[v0].push_back(faceID);
		adjacencies[v1].push_back(faceID);
		adjacencies[v2].push_back(faceID);
		adjacencies[v3].push_back(faceID);
	}

	// Init emmision
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

	// Init radiosity
	if (geometry.triangles.radiosity.size() == 0 && geometry.quads.radiosity.size() == 0) {
		for (GLuint i = 0; i < this->faces; i++) {
			this->radiosity.push_back(glm::vec3(1.0f));
		}
		for (GLuint i = 0; i < this->vertices.size() / 3; i++) {
			this->perVertexRadiosity.push_back(glm::vec3(1.0f));
			this->perVertexRadiosity.push_back(glm::vec3(1.0f));
			this->perVertexRadiosity.push_back(glm::vec3(1.0f));
		}
	}

	// Init reflactance
	if (geometry.triangles.reflactance.size() == 0 && geometry.quads.reflactance.size() == 0) {
		for (GLuint i = 0; i < this->faces; i++) {
			this->reflactance.push_back(glm::vec3(1.0f));
		}
		for (GLuint i = 0; i < this->vertices.size() / 3; i++) {
			this->perVertexReflactance.push_back(glm::vec3(glm::vec3(1.0f)));
			this->perVertexReflactance.push_back(glm::vec3(glm::vec3(1.0f)));
			this->perVertexReflactance.push_back(glm::vec3(glm::vec3(1.0f)));
		}
	}

	this->vao = new VAO();
	this->vao->addAttribute(sizeof(glm::vec3) * vertices.size(), &vertices[0].x, 3, GL_FLOAT, 0);
	this->vao->addAttribute(sizeof(GLfloat) * perVertexEmission.size(), &perVertexEmission[0], 1, GL_FLOAT, 1, GL_DYNAMIC_DRAW);
	this->vao->addAttribute(sizeof(glm::vec3) * perVertexRadiosity.size(), &perVertexRadiosity[0], 3, GL_FLOAT, 2, GL_DYNAMIC_DRAW);
	this->vao->addAttribute(sizeof(glm::vec3) * perVertexRadiosity.size(), &perVertexReflactance[0], 3, GL_FLOAT, 3, GL_DYNAMIC_DRAW);
}

glm::vec3 interpolate(glm::vec3 &vertex, std::vector<glm::vec3> &radiosities, std::vector<GLuint> &adjacencies) {
	glm::vec3 radiosity = glm::vec3(.0f);
	for (GLuint face : adjacencies) {
		radiosity += radiosities[face - 1];
	}
	return radiosity / GLfloat(adjacencies.size());
}

void Mesh::setRadiosity(std::vector<glm::vec3> radiosity, bool smooth) {
	this->radiosity = radiosity;
	if (smooth) {
		for (GLuint i = 0; i < tFaces; i++) {
			GLuint offset = 3 * i;
			for (GLuint vertex = 0; vertex < 3; vertex++) {
				glm::vec3 v = this->vertices[offset + vertex];
				this->perVertexRadiosity[offset + vertex] = interpolate(v, radiosity, this->adjacencies[v]);
			}
		}
		for (GLuint i = tFaces; i < faces; i++) {
			GLuint offset = 6 * i - 3 * tFaces;
			for (GLuint vertex = 0; vertex < 6; vertex++) {
				glm::vec3 v = this->vertices[offset + vertex];
				this->perVertexRadiosity[offset + vertex] = interpolate(v, radiosity, this->adjacencies[v]);
			}
		}
	}
	else {
		for (GLuint i = 0; i < tFaces; i++) {
			GLuint offset = 3 * i;
			glm::vec3 value = radiosity[i];
			for (GLuint vertex = 0; vertex < 3; vertex++) {
				this->perVertexRadiosity[offset + vertex] = value;
			}
		}
		for (GLuint i = tFaces; i < faces; i++) {
			GLuint offset = 6 * i - 3 * tFaces;
			glm::vec3 value = radiosity[i];
			for (GLuint vertex = 0; vertex < 6; vertex++) {
				this->perVertexRadiosity[offset + vertex] = value;
			}
		}
	}
	this->vao->updateAttribute(sizeof(glm::vec3) * this->perVertexRadiosity.size(), &this->perVertexRadiosity[0], 3, GL_FLOAT, 2);
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

void Mesh::setReflactance(GLuint faceIndex, glm::vec3 reflactance) {
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
	this->vao->updateAttribute(sizeof(glm::vec3) * this->perVertexReflactance.size(), &this->perVertexReflactance[0], 3, GL_FLOAT, 3);
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

std::vector<glm::vec3> Mesh::getReflactances() {
	return this->reflactance;
}

glm::vec3 Mesh::getReflactance(GLuint faceIndex) {
	return this->reflactance[faceIndex];
}

std::vector<glm::vec3> Mesh::getRadiosity() {
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
