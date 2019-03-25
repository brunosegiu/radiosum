#include "geometry/Mesh.h"

#include <fstream>

#include "geometry/GeometryUtils.h"

#define VERTICES_ID 0
#define EMISSION_ID 1
#define REFLACTANCE_ID 2
#define RADIOSITY_ID 3

std::vector<Mesh*> Mesh::load(std::string path) {
	std::string fileExt = path.substr(path.find_last_of(".") + 1);
	std::ifstream input(path);

	std::vector<glm::vec3> vertices;


	if (fileExt == "obj") {
		std::vector<IndexedBuffers> buffers = loadOBJ(&input);
		std::vector<Mesh*> meshes;
		for (auto &buffer : buffers)
			meshes.push_back(new Mesh(buffer));
		return meshes;
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

Mesh::Mesh(IndexedBuffers geometry) {
	this->geometry = geometry;

	this->tFaces = geometry.triangles.size() / 3;
	this->qFaces = geometry.quads.size() / 4;
	this->faces = tFaces + qFaces;

	// Compute adjacencies
	adjacencies = std::vector<std::vector<GLuint>>(geometry.vertices.size(), std::vector<GLuint>());
	for (GLuint vertexIndex = 0; vertexIndex < geometry.vertices.size(); vertexIndex++) {
		for (GLuint i = 0; i < geometry.triangles.size(); i++) {
			if (geometry.triangles[i] == vertexIndex)
				adjacencies[vertexIndex].push_back(i/3);
		}
		for (GLuint i = 0; i < geometry.quads.size(); i++) {
			if (geometry.quads[i] == vertexIndex)
				adjacencies[vertexIndex].push_back(i/4);
		}
	}

	// Triangulate quads
	FlattenedBuffers flattened = deIndex(geometry);
	std::vector<glm::vec3> triangles = flattened.triangles;
	for (GLuint i = 0; i < flattened.quads.size(); i += 4) {
		glm::vec3 v0 = flattened.quads[i + 0];
		glm::vec3 v1 = flattened.quads[i + 1];
		glm::vec3 v2 = flattened.quads[i + 2];
		glm::vec3 v3 = flattened.quads[i + 3];

		std::vector<glm::vec3> quad({ v0,v1,v2,v3 });
		std::vector<glm::vec3> triangulated = triangulate(quad);
		triangles.insert(triangles.end(), triangulated.begin(), triangulated.end());
	}

	// Init aditional per-face attibutes
	
	this->emission.reserve(this->faces);
	this->reflactance.reserve(this->faces);
	this->radiosity.reserve(this->faces);

	for (GLuint i = 0; i < this->faces; i++) {
		this->emission.push_back(.0f);
		this->reflactance.push_back(glm::vec3(1.0f));
		this->radiosity.push_back(glm::vec3(0));
	}

	std::vector<GLfloat> perVertexEmission;
	std::vector<glm::vec3> perVertexReflactance;
	perVertexEmission.reserve(this->tFaces * 3 + this->qFaces * 6);
	perVertexReflactance.reserve(this->tFaces * 3 + this->qFaces * 6);
	for (GLuint i = 0; i < this->tFaces * 3 + this->qFaces * 6; i++) {
		perVertexEmission.push_back(0.0f);
		perVertexReflactance.push_back(glm::vec3(1.0f));
	}
	
	this->vao = new VAO();
	this->vao->addAttribute(sizeof(glm::vec3) * triangles.size(), &triangles[0].x, 3, GL_FLOAT, VERTICES_ID);
	this->vao->addAttribute(sizeof(GLfloat) * perVertexEmission.size(), &perVertexEmission[0], 1, GL_FLOAT, EMISSION_ID, GL_DYNAMIC_DRAW);
	this->vao->addAttribute(sizeof(glm::vec3) * perVertexReflactance.size(), &perVertexReflactance[0], 3, GL_FLOAT, REFLACTANCE_ID, GL_DYNAMIC_DRAW);
	this->vao->addAttribute(sizeof(glm::vec3) * perVertexReflactance.size(), &perVertexReflactance[0], 3, GL_FLOAT, RADIOSITY_ID, GL_DYNAMIC_DRAW);

	this->simpleGeometry = new VBO();
	this->simpleGeometry->addVertices(geometry.vertices);
	std::vector<GLuint> indices = geometry.triangles;
	for (GLuint i = 0; i < geometry.quads.size(); i += 4) {
		indices.push_back(geometry.quads[i + 0]);
		indices.push_back(geometry.quads[i + 1]);
		indices.push_back(geometry.quads[i + 3]);
		indices.push_back(geometry.quads[i + 1]);
		indices.push_back(geometry.quads[i + 2]);
		indices.push_back(geometry.quads[i + 3]);
	}
	this->simpleGeometry->addIndices(indices);
}

glm::vec3 interpolate(std::vector<glm::vec3> &radiosities, std::vector<GLuint> &adjacencies) {
	glm::vec3 radiosity = glm::vec3(.0f);
	for (GLuint face : adjacencies) {
		radiosity += radiosities[face];
	}
	return radiosity / GLfloat(adjacencies.size());
}

void Mesh::setRadiosity(std::vector<glm::vec3> radiosity, bool smooth) {
	this->radiosity = radiosity;
	std::vector<glm::vec3> perVertexRadiosity(6 * qFaces + tFaces * 3);
	if (smooth) {
		for (GLuint i = 0; i < tFaces; i++) {
			glm::vec3 value = radiosity[i];
			for (GLuint vertex = 0; vertex < 3; vertex++) {
				perVertexRadiosity[3 * i + vertex] = interpolate(this->radiosity, this->adjacencies[this->geometry.triangles[3 * i + vertex]]);
			}
		}
		for (GLuint i = 0; i < qFaces; i++) {
			glm::vec3 value = radiosity[tFaces + i];
			for (GLuint vertex = 0; vertex < 6; vertex++) {
				perVertexRadiosity[3 * tFaces + 6 * i + vertex] = interpolate(this->radiosity, this->adjacencies[this->geometry.quads[4 * i + vertex % 4]]);
			}
		}
	}
	else {
		for (GLuint i = 0; i < tFaces; i++) {
			glm::vec3 value = radiosity[i];
			for (GLuint vertex = 0; vertex < 3; vertex++) {
				perVertexRadiosity[3 * i + vertex] = value;
			}
		}
		for (GLuint i = 0; i < qFaces; i++) {
			glm::vec3 value = radiosity[tFaces + i];
			for (GLuint vertex = 0; vertex < 6; vertex++) {
				perVertexRadiosity[3 * tFaces + 6 * i + vertex] = value;
			}
		}
	}
	this->vao->updateAttribute(0, sizeof(glm::vec3) * perVertexRadiosity.size(), &perVertexRadiosity[0].x, RADIOSITY_ID);
}

std::pair<GLuint, GLuint> findMesh(std::map <std::string, std::pair<GLuint, GLuint>> &names, GLuint faceIndex) {
	for (auto &limits : names) {
		if (faceIndex >= limits.second.first && faceIndex < limits.second.second)
			return limits.second;
	}
}

void Mesh::setEmission(GLuint faceIndex, GLfloat emission) {
	this->emission[faceIndex] = emission;
	std::vector<GLfloat> perVertex;
	if (faceIndex < tFaces) {
		perVertex = std::vector<GLfloat>(3, emission);
	}
	else {
		perVertex = std::vector<GLfloat>(6, emission);
	}
	this->vao->updateAttribute(faceIndex < tFaces ? faceIndex * 3 : 6 * faceIndex - tFaces * 3, sizeof(GLfloat) * perVertex.size(), &perVertex[0], EMISSION_ID);
}

void Mesh::setEmission(GLfloat emission) {
	std::vector<GLfloat> perVertex;
	GLuint flattenedVCount = 3 * this->tFaces + 6 * this->qFaces;
	perVertex.reserve(flattenedVCount);
	this->emission = std::vector<GLfloat>(this->emission.size(), emission);
	perVertex = std::vector<GLfloat>(flattenedVCount, emission);
	this->vao->updateAttribute(0, sizeof(GLfloat) * perVertex.size(), &perVertex[0], EMISSION_ID);
}

void Mesh::setReflactance(GLuint faceIndex, glm::vec3 reflactance) {
	this->reflactance[faceIndex] = reflactance;
	std::vector<glm::vec3> perVertex;
	if (faceIndex < tFaces) {
		perVertex = std::vector<glm::vec3>(3, reflactance);
	}
	else {
		perVertex = std::vector<glm::vec3>(6, reflactance);
	}
	this->vao->updateAttribute(faceIndex < tFaces ? faceIndex * 3: 6 * faceIndex - tFaces * 3 , sizeof(glm::vec3) * perVertex.size(), &perVertex[0].x, REFLACTANCE_ID);
}

void Mesh::setReflactance(glm::vec3 reflactance) {
	std::vector < glm::vec3 > perVertex;
	GLuint flattenedVCount = 3 * this->tFaces + 6 * this->qFaces;
	this->reflactance = std::vector<glm::vec3>(this->reflactance.size(), reflactance);
	perVertex = std::vector < glm::vec3 >(flattenedVCount,reflactance);
	this->vao->updateAttribute(0, sizeof(glm::vec3) * perVertex.size(), &perVertex[0].x, REFLACTANCE_ID);
}

void Mesh::drawVao(GLuint shaderID, VAO* vao) {
	GLuint tFacesLoc = glGetUniformLocation(shaderID, "tFaces");
	glUniform1ui(tFacesLoc, this->tFaces);
	vao->bind();
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(3 * this->tFaces + 6 * qFaces));
	vao->unbind();
}

void Mesh::draw(GLuint shaderID) {
	this->drawVao(shaderID, this->vao);
}

void Mesh::drawGeometry(GLuint shaderID) {
	GLuint tFacesLoc = glGetUniformLocation(shaderID, "tFaces");
	glUniform1ui(tFacesLoc, this->tFaces);
	this->simpleGeometry->bind();
	glDrawElements(GL_TRIANGLES, GLsizei(3 * this->tFaces + 6 * qFaces), GL_UNSIGNED_INT, (void*)(0));
	this->simpleGeometry->unbind();
}

IndexedBuffers Mesh::getGeometry() {
	return this->geometry;
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
	index = index % this->size();
	if (index < tFaces) {
		std::vector<GLuint> indices(3);
		indices[0] = this->geometry.triangles[3 * index];
		indices[1] = this->geometry.triangles[3 * index + 1];
		indices[2] = this->geometry.triangles[3 * index + 2];
		return Face(this->geometry.vertices[indices[0]], this->geometry.vertices[indices[1]], this->geometry.vertices[indices[2]]);
	}
	else {
		GLuint inQuad = index - tFaces;
		std::vector<GLuint> indices(4);
		indices[0] = this->geometry.quads[4 * inQuad + 0];
		indices[1] = this->geometry.quads[4 * inQuad + 1];
		indices[2] = this->geometry.quads[4 * inQuad + 2];
		indices[3] = this->geometry.quads[4 * inQuad + 3];
		return Face(this->geometry.vertices[indices[0]], this->geometry.vertices[indices[1]], this->geometry.vertices[indices[2]], this->geometry.vertices[indices[3]]);
	}
}

Mesh::~Mesh() {
	delete this->vao;
	delete this->simpleGeometry;
}
