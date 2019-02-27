#include "common/geometry/Mesh.h"

#include <fstream>

GLuint Mesh::faceCount = 1;

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals) {
	this->vertices = vertices;
	this->GLVerticesId, this->GLNormalsId = this->GLIdsId, this->GLVaoId = 0;

	for (GLuint id = 0; id < this->vertices.size() / 3; id++) {
		this->ids.push_back(id + Mesh::faceCount);
	}

	Mesh::faceCount += GLuint(this->ids.size());

	//Copy vertices to GPU
	glGenBuffers(1, &this->GLVerticesId);
	glBindBuffer(GL_ARRAY_BUFFER, this->GLVerticesId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);

	//Copy normals to GPU
	glGenBuffers(1, &this->GLNormalsId);
	glBindBuffer(GL_ARRAY_BUFFER, this->GLNormalsId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0].x, GL_STATIC_DRAW);

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

	//Set up the VAO
	glGenVertexArrays(1, &GLVaoId);
	glBindVertexArray(GLVaoId);

	//Bind Vertices
	glBindBuffer(GL_ARRAY_BUFFER, this->GLVerticesId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Bind Normals
	glBindBuffer(GL_ARRAY_BUFFER, this->GLNormalsId);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Bind Ids
	glBindBuffer(GL_ARRAY_BUFFER, this->GLIdsId);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, (void*)0);
}

Mesh* loadOBJ(std::ifstream* input) {
	std::vector<glm::vec3> indexedVertices;
	std::vector<glm::vec3> indexedNormals;
	std::vector<GLuint> verticesIndices;
	std::vector<GLuint> normalsIndices;

	//Load every vertex, normals and indices
	std::string line;
	while (getline(*input, line)) {
		if (line[0] == 'v' && line[1] == ' ') {
			line[0] = ' ';

			glm::vec3 vertex;

			sscanf_s(line.c_str(), "%f %f %f ",
				&vertex.x,
				&vertex.y,
				&vertex.z);

			indexedVertices.push_back(vertex);
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			line[0] = ' ';
			line[1] = ' ';

			glm::vec3 normal;

			sscanf_s(line.c_str(), "%f %f %f ",
				&normal.x,
				&normal.y,
				&normal.z);

			indexedNormals.push_back(normal);
		}
		else if (line[0] == 'f') {
			line[0] = ' ';

			GLuint first_v, second_v, third_v;
			GLuint first_n, second_n, third_n;

			sscanf_s(line.c_str(), " %i//%i %i//%i %i//%i",
				&first_v, &first_n,
				&second_v, &second_n,
				&third_v, &third_n);

			verticesIndices.push_back(first_v - 1);
			verticesIndices.push_back(second_v - 1);
			verticesIndices.push_back(third_v - 1);

			normalsIndices.push_back(first_n - 1);
			normalsIndices.push_back(second_n - 1);
			normalsIndices.push_back(third_n - 1);

		}
	}

	//Per-face vertices

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	for (unsigned int i = 0; i < verticesIndices.size(); i += 3) {

		glm::vec3 v0 = indexedVertices[verticesIndices[i]];
		glm::vec3 v1 = indexedVertices[verticesIndices[i + 1]];
		glm::vec3 v2 = indexedVertices[verticesIndices[i + 2]];

		glm::vec3 n0 = indexedNormals[normalsIndices[i]];
		glm::vec3 n1 = indexedNormals[normalsIndices[i + 1]];
		glm::vec3 n2 = indexedNormals[normalsIndices[i + 2]];

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);

		normals.push_back(n0);
		normals.push_back(n1);
		normals.push_back(n2);

	}


	return new Mesh(vertices, normals);
}

Mesh* Mesh::load(std::string path) {
	std::string fileExt = path.substr(path.find_last_of(".") + 1);
	std::ifstream input(path);

	std::vector<glm::vec3> vertices;

	if (fileExt == "obj")
		return loadOBJ(&input);

	throw std::runtime_error("Unable to load mesh, format not supported");
}

void Mesh::draw() {
	glBindVertexArray(this->GLVaoId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(this->vertices.size()));
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
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
