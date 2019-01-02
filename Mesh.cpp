#include "Mesh.h"

#include <fstream>

GLuint Mesh::faceCount = 0;

Mesh::Mesh(std::vector<glm::vec3> vertices) {
	this->vertices = vertices;
	this->GLVerticesId, this->GLIdsId, this->GLVaoId = 0;

	for (GLuint id = Mesh::faceCount; id < this->vertices.size(); id++) {
		this->ids.push_back(id);
	}

	Mesh::faceCount += this->ids.size();

	//Copy vertices to GPU
	glGenBuffers(1, &this->GLVerticesId);
	glBindBuffer(GL_ARRAY_BUFFER, this->GLVerticesId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);

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

	//Bind Ids
	glBindBuffer(GL_ARRAY_BUFFER, this->GLIdsId);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, (void*)0);
}

std::vector<glm::vec3> loadOBJ(std::ifstream* input) {
	std::vector<glm::vec3> indexedVertices;
	std::vector<GLuint> indices;

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
		else if (line[0] == 'f') {
			line[0] = ' ';

			GLuint first_v, second_v, third_v;
			GLuint first_n, second_n, third_n;

			sscanf_s(line.c_str(), " %i//%i %i//%i %i//%i",
				&first_v, &first_n,
				&second_v, &second_n,
				&third_v, &third_n);

			indices.push_back(first_v - 1);
			indices.push_back(second_v - 1);
			indices.push_back(third_v - 1);
		}
	}

	//Per-face vertices

	std::vector<glm::vec3> vertices;

	for (unsigned int i = 0; i < indices.size(); i += 3) {

		glm::vec3 v0 = indexedVertices[indices[i]];
		glm::vec3 v1 = indexedVertices[indices[i + 1]];
		glm::vec3 v2 = indexedVertices[indices[i + 2]];

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);

	}

	return vertices
}

Mesh* Mesh::load(std::string path) {
	std::string fileExt = path.substr(path.find_last_of(".") + 1);
	std::ifstream input(path);

	std::vector<glm::vec3> vertices;

	if (fileExt == "obj")
		vertices = loadOBJ(&input);

	return new Mesh(vertices);
}

void Mesh::draw() {
	glBindVertexArray(this->GLVaoId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

std::vector<glm::vec3> Mesh::getVertices() {
	return this->vertices;
}

std::vector<GLuint> Mesh::getIds() {
	return this->ids;
}

Mesh::~Mesh() {
	glBindVertexArray(this->GLVaoId);
	glDeleteBuffers(1, &this->GLVerticesId);
	glDeleteBuffers(1, &this->GLIdsId);
	glDeleteVertexArrays(1, &this->GLVaoId)
}
