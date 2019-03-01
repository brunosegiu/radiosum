#pragma once

#include <fstream>
#include <sstream>
#include <stdio.h>

#include <GL/glew.h>

#include "common/geometry/GeometryContainer.h"

#define VERTICES 0
#define TEXTURES 1
#define NORMALS 2

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

inline GeometryBuffers loadOBJ(std::ifstream* input) {
	std::vector<glm::vec3> indexedVertices, indexedNormals;
	std::vector<glm::vec2> indexedTextures;
	std::vector<GLuint> tIndexBuffers[3];
	std::vector<GLuint> qIndexBuffers[3];

	//Load every vertex, normals and indices
	std::string line;
	while (getline(*input, line)) {
		if (line[0] == 'v' && line[1] == ' ') {
			line.erase(0, 2);

			glm::vec3 vertex;

			sscanf_s(line.c_str(), "%f %f %f ",
				&vertex.x,
				&vertex.y,
				&vertex.z);

			indexedVertices.push_back(vertex);
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			line.erase(0, 2);

			glm::vec3 normal;

			sscanf_s(line.c_str(), "%f %f %f ",
				&normal.x,
				&normal.y,
				&normal.z);

			indexedNormals.push_back(normal);
		}
		else if (line[0] == 'v' && line[1] == 't') {
			line.erase(0, 2);

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
				for (unsigned int it = 0; it < attributesRaw.size(); it++) {
					GLuint index;
					std::string value = attributesRaw[it];
					if (value != "") {
						sscanf_s(value.c_str(), "%i", &index);
						verticesRaw.size() > 3 ? qIndexBuffers[it].push_back(index - 1) : tIndexBuffers[it].push_back(index - 1);
					}
				}
			}
		}
	}

	//Per-face vertices

	GeometryBuffers buffers;

	for (unsigned int geom = 0; geom < 2; geom++) {
		for (unsigned int i = 0; i < (geom == 0 ? tIndexBuffers[VERTICES].size() : qIndexBuffers[VERTICES].size()); i += (geom == 0 ? 3 : 4)) {
			glm::vec3 v0 = indexedVertices[(geom == 0 ? tIndexBuffers : qIndexBuffers)[VERTICES][i]];
			glm::vec3 v1 = indexedVertices[(geom == 0 ? tIndexBuffers : qIndexBuffers)[VERTICES][i + 1]];
			glm::vec3 v2 = indexedVertices[(geom == 0 ? tIndexBuffers : qIndexBuffers)[VERTICES][i + 2]];

			if (geom == 0) {
				buffers.triangles.vertices.push_back(v0);
				buffers.triangles.vertices.push_back(v1);
				buffers.triangles.vertices.push_back(v2);
			}
			else {
				glm::vec3 v3 = indexedVertices[qIndexBuffers[VERTICES][i + 3]];
				buffers.quads.vertices.push_back(v0);
				buffers.quads.vertices.push_back(v1);
				buffers.quads.vertices.push_back(v2);
				buffers.quads.vertices.push_back(v3);
			}


			if ((geom == 0 ? tIndexBuffers : qIndexBuffers)[TEXTURES].size() > 0) {
				glm::vec2 t0 = indexedTextures[(geom == 0 ? tIndexBuffers : qIndexBuffers)[TEXTURES][i]];
				glm::vec2 t1 = indexedTextures[(geom == 0 ? tIndexBuffers : qIndexBuffers)[TEXTURES][i + 1]];
				glm::vec2 t2 = indexedTextures[(geom == 0 ? tIndexBuffers : qIndexBuffers)[TEXTURES][i + 2]];

				if (geom == 0) {
					buffers.triangles.textures.push_back(t0);
					buffers.triangles.textures.push_back(t1);
					buffers.triangles.textures.push_back(t2);
				}
				else {
					glm::vec2 t3 = indexedTextures[qIndexBuffers[TEXTURES][i + 3]];
					buffers.quads.textures.push_back(t0);
					buffers.quads.textures.push_back(t1);
					buffers.quads.textures.push_back(t2);
					buffers.quads.textures.push_back(t3);
				}
			}

			if ((geom == 0 ? tIndexBuffers : qIndexBuffers)[NORMALS].size() > 0) {
				glm::vec3 n0 = indexedNormals[(geom == 0 ? tIndexBuffers : qIndexBuffers)[NORMALS][i]];
				glm::vec3 n1 = indexedNormals[(geom == 0 ? tIndexBuffers : qIndexBuffers)[NORMALS][i + 1]];
				glm::vec3 n2 = indexedNormals[(geom == 0 ? tIndexBuffers : qIndexBuffers)[NORMALS][i + 2]];

				if (geom == 0) {
					buffers.triangles.normals.push_back(n0);
					buffers.triangles.normals.push_back(n1);
					buffers.triangles.normals.push_back(n2);
				}
				else {
					glm::vec3 n3 = indexedNormals[qIndexBuffers[NORMALS][i + 3]];
					buffers.quads.normals.push_back(n0);
					buffers.quads.normals.push_back(n1);
					buffers.quads.normals.push_back(n2);
					buffers.quads.normals.push_back(n3);
				}
			}
		}

	}
	return buffers;
}
