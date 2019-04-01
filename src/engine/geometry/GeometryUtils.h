#pragma once

#include <stdio.h>
#include <fstream>
#include <map>
#include <sstream>

#include <GL/glew.h>

#include "Utils.h"
#include "geometry/GeometryContainer.h"
#include "geometry/Material.h"
#include "geometry/Mesh.h"

#define MAXUINT32 4294967295
#define VERTICES 0
#define TEXTURES 1
#define NORMALS 2

inline std::map<std::string, Material *> loadMtl(std::string path) {
  std::ifstream input(path);
  std::string line;

  std::string currentMTL;
  glm::vec3 currentReflactance;
  GLfloat currentEmission = .0f;
  std::string currentTexturePath;

  std::map<std::string, Material *> materials;

  bool run = false;

  while (getline(input, line)) {
    if (line[0] == '#') {
    } else if (line.find("newmtl ") != std::string::npos) {
      if (run) {
        Texture *texture = nullptr;
        if (currentTexturePath != "")
          texture = Texture::load(currentTexturePath);
        Material *mat =
            new Material(currentReflactance, currentEmission, texture);
        materials[currentMTL] = mat;
      } else {
        run = true;
      }
      line.erase(0, 7);
      currentMTL = line;
    } else if (line[0] != 'm' && line.find("Kd ") != std::string::npos) {
      line.erase(0, 2);
      sscanf_s(line.c_str(), "%f %f %f ", &currentReflactance.x,
               &currentReflactance.y, &currentReflactance.z);
    } else if (line[0] == 'e') {
      line.erase(0, 2);
      sscanf_s(line.c_str(), "%f ", &currentEmission);
    } else if (line.find("map_Kd") != std::string::npos) {
      line.erase(0, 7);
      currentTexturePath = line;
    }
  }
  if (currentMTL != "") {
    Texture *texture = nullptr;
    if (currentTexturePath != "") texture = Texture::load(currentTexturePath);
    Material *mat = new Material(currentReflactance, currentEmission, texture);
    materials[currentMTL] = mat;
  }
  return materials;
}

inline std::pair<GLuint, GLuint> getOffsets(std::vector<GLuint> &v0,
                                            std::vector<GLuint> &v1) {
  GLuint min = MAXUINT32;
  GLuint max = 0;
  for (auto index : v0) {
    min = index < min ? index : min;
    max = index > max ? index : max;
  }
  for (auto index : v1) {
    min = index < min ? index : min;
    max = index > max ? index : max;
  }
  return std::pair<GLuint, GLuint>(min, max);
}

inline std::vector<Mesh *> loadOBJ(std::string path) {
  std::ifstream input(path);
  std::string mtlPath = path;
  mtlPath.replace(mtlPath.end() - 3, mtlPath.end(), "mtl");
  std::map<std::string, Material *> materials = loadMtl(mtlPath);

  std::vector<glm::vec3> indexedVertices, indexedNormals;
  std::vector<glm::vec2> indexedTextures;

  // Load every vertex, normals and indices
  std::string line;
  typedef std::pair<std::vector<GLuint>[3], std::vector<GLuint>[3]>
      IndexBuffers;
  std::vector<IndexBuffers> objects;
  std::vector<Material *> vectorizedMaterials;
  std::string material;

  while (getline(input, line)) {
    if (line[0] == 'o' && line[1] == ' ') {
      line.erase(0, 2);
      objects.push_back(IndexBuffers());
    } else if (line[0] == 'v' && line[1] == ' ') {
      line.erase(0, 2);

      glm::vec3 vertex;

      sscanf_s(line.c_str(), "%f %f %f ", &vertex.x, &vertex.y, &vertex.z);

      indexedVertices.push_back(vertex);
    } else if (line[0] == 'v' && line[1] == 'n') {
      line.erase(0, 2);

      glm::vec3 normal;

      sscanf_s(line.c_str(), "%f %f %f ", &normal.x, &normal.y, &normal.z);

      indexedNormals.push_back(normal);
    } else if (line[0] == 'v' && line[1] == 't') {
      line.erase(0, 2);

      glm::vec2 texture;

      sscanf_s(line.c_str(), "%f %f ", &texture.x, &texture.y);

      indexedTextures.push_back(texture);
    } else if (line.find("usemtl ") != std::string::npos) {
      line.erase(0, 7);
      material = line;
    } else if (line[0] == 'f') {
      if (objects.size() == 0) {
        objects.push_back(IndexBuffers());
      }
      if (vectorizedMaterials.size() < objects.size())
        vectorizedMaterials.push_back(materials.count(material) > 0
                                          ? materials[material]
                                          : new Material());
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

  std::vector<Mesh *> meshes;

  GLuint index = 0;
  for (auto &object : objects) {
    IndexedBuffers indObj;
    indObj.vertices.triangles = object.first[VERTICES];
    indObj.vertices.quads = object.second[VERTICES];
    {
      GLuint min, max;
      auto minMax =
          getOffsets(indObj.vertices.triangles, indObj.vertices.quads);
      min = minMax.first;
      max = minMax.second;

      indObj.vertices.data = std::vector<glm::vec3>(
          indexedVertices.begin() + min, indexedVertices.begin() + max + 1);

      for (auto &index : indObj.vertices.triangles) {
        index = index - min;
      }
      for (auto &index : indObj.vertices.quads) {
        index = index - min;
      }
    }

    {
      if (object.first[TEXTURES].size() > 0 ||
          object.second[TEXTURES].size() > 0) {
        indObj.textures.triangles = object.first[TEXTURES];
        indObj.textures.quads = object.second[TEXTURES];
        GLuint min, max;
        auto minMax =
            getOffsets(indObj.textures.triangles, indObj.textures.quads);
        min = minMax.first;
        max = minMax.second;

        indObj.textures.data = std::vector<glm::vec2>(
            indexedTextures.begin() + min, indexedTextures.begin() + max + 1);

        for (auto &index : indObj.textures.triangles) {
          index = index - min;
        }
        for (auto &index : indObj.textures.quads) {
          index = index - min;
        }
      }
    }

    {
      if (object.first[NORMALS].size() > 0 ||
          object.second[NORMALS].size() > 0) {
        indObj.normals.triangles = object.first[NORMALS];
        indObj.normals.quads = object.second[NORMALS];
        GLuint min, max;
        auto minMax =
            getOffsets(indObj.normals.triangles, indObj.normals.quads);
        min = minMax.first;
        max = minMax.second;

        indObj.normals.data = std::vector<glm::vec3>(
            indexedNormals.begin() + min, indexedNormals.begin() + max + 1);

        for (auto &index : indObj.normals.triangles) {
          index = index - min;
        }
        for (auto &index : indObj.normals.quads) {
          index = index - min;
        }
      }
    }

    Mesh *mesh = new Mesh(indObj, vectorizedMaterials[index]);
    meshes.push_back(mesh);
    index++;
  }
  return meshes;
}

template <typename vect>
std::vector<vect> triangulate(std::vector<vect> &quad) {
  vect v0 = quad[0];
  vect v1 = quad[1];
  vect v2 = quad[2];
  vect v3 = quad[3];

  std::vector<vect> triangles;
  triangles.reserve(6);

  triangles.push_back(v0);
  triangles.push_back(v1);
  triangles.push_back(v3);

  triangles.push_back(v1);
  triangles.push_back(v2);
  triangles.push_back(v3);

  return triangles;
}

template <typename IndexBuffer, typename vectorT>
inline std::vector<vectorT> getFlattened(IndexBuffer &buffers) {
  std::vector<vectorT> flat;
  const auto vertices = buffers.data;
  for (unsigned int geom = 0; geom < 2; geom++) {
    const auto indices = (geom == 0 ? buffers.triangles : buffers.quads);
    for (unsigned int i = 0; i < indices.size(); i += (geom == 0 ? 3 : 4)) {
      vectorT v0 = vertices[indices[i]];
      vectorT v1 = vertices[indices[i + 1]];
      vectorT v2 = vertices[indices[i + 2]];
      if (geom == 0) {
        std::vector<vectorT> tri = {v0, v1, v2};
        flat.insert(flat.end(), tri.begin(), tri.end());
      } else {
        vectorT v3 = vertices[indices[i + 3]];
        std::vector<vectorT> quad = {v0, v1, v2, v3};
        auto triangulated = triangulate(quad);
        flat.insert(flat.end(), triangulated.begin(), triangulated.end());
      }
    }
  }
  return flat;
}
