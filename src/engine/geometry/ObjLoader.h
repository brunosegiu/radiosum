#pragma once

#include <fstream>
#include <sstream>
#include <stdio.h>

#include <GL/glew.h>

#include "geometry/GeometryContainer.h"
#include "Utils.h"

#define VERTICES 0
#define TEXTURES 1
#define NORMALS 2

inline std::vector<IndexedBuffers> loadOBJ(std::ifstream* input) {
	std::vector<glm::vec3> indexedVertices, indexedNormals;
	std::vector<glm::vec2> indexedTextures;

	//Load every vertex, normals and indices
	std::string line;
	typedef  std::pair<std::vector<GLuint>[3], std::vector<GLuint>[3]> IndexBuffers;
	std::vector< IndexBuffers > objects;

	std::vector<IndexedBuffers> buffers;

	while (getline(*input, line)) {
		if (line[0] == 'o' && line[1] == ' ') {
			line.erase(0, 2);
			objects.push_back(IndexBuffers());
		}
		else if (line[0] == 'v' && line[1] == ' ') {
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
			if (objects.size() == 0) {
				objects.push_back(IndexBuffers());
			}
			line.erase(0, 2);
			std::vector<std::string> verticesRaw = split(line, " ");
			GLuint currentObject = objects.size() - 1;
			for (std::string vertexAttrs : verticesRaw) {
				std::vector<std::string> attributesRaw = split(vertexAttrs, "/");
				for (unsigned int it = 0; it < attributesRaw.size(); it++) {
					GLuint index = 0;
					std::string value = attributesRaw[it];
					if (value != "") {
						sscanf_s(value.c_str(), "%i", &index);
						if (verticesRaw.size() > 3)
							objects[currentObject].second[it].push_back(index - 1);
						else
							objects[currentObject].first[it].push_back(index - 1);
					}
				}
			}
		}
	}

	for (auto &object : objects) {
		IndexedBuffers indObj;
		indObj.triangles = object.first[VERTICES];
		indObj.quads = object.second[VERTICES];

		GLuint min = MAXUINT32;
		GLuint max = 0;
		for (auto index : indObj.triangles) {
			min = index < min ? index : min;
			max = index > max ? index : max;
		}
		for (auto index : indObj.quads) {
			min = index < min ? index : min;
			max = index > max ? index : max;
		}

		indObj.vertices = indexedVertices.
	}

	inline FlattenedBuffers deIndex(IndexedBuffers &buffers) {
		FlattenedBuffers flattened;
		for (unsigned int geom = 0; geom < 2; geom++) {
			const auto vertices = buffers.vertices;
			const auto indices = (geom == 0 ? buffers.triangles : buffers.quads);
			for (unsigned int i = 0; i < indices.size(); i += (geom == 0 ? 3 : 4)) {
				glm::vec3 v0 = vertices[indices[i]];
				glm::vec3 v1 = vertices[indices[i + 1]];
				glm::vec3 v2 = vertices[indices[i + 2]];

				if (geom == 0) {
					flattened.triangles.push_back(v0);
					flattened.triangles.push_back(v1);
					flattened.triangles.push_back(v2);
				}
				else {
					glm::vec3 v3 = vertices[indices[i + 1]];
					flattened.quads.push_back(v0);
					flattened.quads.push_back(v1);
					flattened.quads.push_back(v2);
					flattened.quads.push_back(v3);
				}

				// Textures - not used
				/*
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
				*/

				// Normals - not used
				/*
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
				*/
			}
		}
	}
	return buffers;
}
