#pragma once

#include <fstream>
#include <sstream>

#define VERTICES 0
#define TEXTURES 1
#define NORMALS 2

typedef struct AttributesBuffer {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
} AttributesBuffer;

inline std::vector<std::string> split(std::string input, std::string delimiter) {
	size_t pos = 0;
	std::string token;
	std::vector<std::string> tokens;
	while ((pos = input.find(delimiter)) != std::string::npos) {
		token = input.substr(0, pos);
		input.erase(0, pos + delimiter.length());
		tokens.push_back(token);
	}
	tokens.push_back(input);
	return tokens;
}

inline AttributesBuffer loadOBJ(std::ifstream* input) {
	std::vector<glm::vec3> indexedVertices, indexedNormals;
	std::vector<glm::vec2> indexedTextures;
	std::vector<GLuint> indexBuffers[3];

	//Load every vertex, normals and indices
	std::string line;
	while (getline(*input, line)) {
		if (line[0] == 'v' && line[1] == ' ') {
			line.erase(0, 3);

			glm::vec3 vertex;

			sscanf_s(line.c_str(), "%f %f %f ",
				&vertex.x,
				&vertex.y,
				&vertex.z);

			indexedVertices.push_back(vertex);
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			line.erase(0, 3);

			glm::vec3 normal;

			sscanf_s(line.c_str(), "%f %f %f ",
				&normal.x,
				&normal.y,
				&normal.z);

			indexedNormals.push_back(normal);
		}
		else if (line[0] == 'v' && line[1] == 't') {
			line.erase(0, 3);

			glm::vec2 texture;

			sscanf_s(line.c_str(), "%f %f ",
				&texture.x,
				&texture.y);

			indexedTextures.push_back(texture);
		}
		else if (line[0] == 'f') {
			line.erase(0, 2);

			std::vector<std::string> verticesRaw = split(line, " ");
			for (std::string vertexAttrs : verticesRaw) {
				std::vector<std::string> attributesRaw = split(vertexAttrs, "/");
				for (unsigned int it = 0; it < attributesRaw.size() && it < 3; it++) {
					GLuint index;
					std::string value = attributesRaw[it];
					if (value != "") {
						sscanf_s(value.c_str(), "%i", &index);
						indexBuffers[it].push_back(index - 1);
					}
				}
			}

		}
	}

	//Per-face vertices

	AttributesBuffer buffers;

	for (unsigned int i = 0; i < indexBuffers[VERTICES].size(); i += 3) {

		glm::vec3 v0 = indexedVertices[indexBuffers[VERTICES][i]];
		glm::vec3 v1 = indexedVertices[indexBuffers[VERTICES][i + 1]];
		glm::vec3 v2 = indexedVertices[indexBuffers[VERTICES][i + 2]];

		buffers.vertices.push_back(v0);
		buffers.vertices.push_back(v1);
		buffers.vertices.push_back(v2);

		if (indexBuffers[TEXTURES].size() > 0) {
			glm::vec2 t0 = indexedVertices[indexBuffers[TEXTURES][i]];
			glm::vec2 t1 = indexedVertices[indexBuffers[TEXTURES][i]];
			glm::vec2 t2 = indexedVertices[indexBuffers[TEXTURES][i]];

			buffers.textures.push_back(t0);
			buffers.textures.push_back(t1);
			buffers.textures.push_back(t2);
		}

		if (indexBuffers[NORMALS].size() > 0) {
			glm::vec3 n0 = indexedNormals[indexBuffers[NORMALS][i]];
			glm::vec3 n1 = indexedNormals[indexBuffers[NORMALS][i + 1]];
			glm::vec3 n2 = indexedNormals[indexBuffers[NORMALS][i + 2]];

			buffers.normals.push_back(n0);
			buffers.normals.push_back(n1);
			buffers.normals.push_back(n2);
		}

	}

	return buffers;
}
