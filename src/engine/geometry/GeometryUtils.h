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

    indObj.vertices.vertices = std::vector<glm::vec3>(
        indexedVertices.begin() + min, indexedVertices.begin() + max + 1);

    for (auto &index : indObj.triangles) {
      index = index - min;
    }
    for (auto &index : indObj.quads) {
      index = index - min;
    }

    if (object.first[TEXTURES].size() > 0 ||
        object.second[TEXTURES].size() > 0) {
      GLuint tmin = MAXUINT32;
      GLuint tmax = 0;
      for (auto index : object.first[TEXTURES]) {
        tmin = index < tmin ? index : tmin;
        tmax = index > tmax ? index : tmax;
      }
      for (auto index : object.second[TEXTURES]) {
        tmin = index < tmin ? index : tmin;
        tmax = index > tmax ? index : tmax;
      }
      indObj.vertices.textures = std::vector<glm::vec2>(
          indObj.vertices.vertices.size(), glm::vec2(.0f));
      for (GLuint i = 0; i < object.first[TEXTURES].size(); i++) {
        indObj.vertices.textures[object.first[VERTICES][i] - min] =
            indexedTextures[object.first[TEXTURES][i]];
      }
      for (GLuint i = 0; i < object.second[TEXTURES].size(); i++) {
        indObj.vertices.textures[object.first[VERTICES][i] - min] =
            indexedTextures[object.second[TEXTURES][i]];
      }
    }
    Mesh *mesh = new Mesh(indObj, vectorizedMaterials[index]);
    meshes.push_back(mesh);
    index++;
  }
  return meshes;
}

inline FlattenedBuffers deIndex(IndexedBuffers &buffers) {
  FlattenedBuffers flattened;
  for (unsigned int geom = 0; geom < 2; geom++) {
    const auto vertices = buffers.vertices;
    const auto indices = (geom == 0 ? buffers.triangles : buffers.quads);
    for (unsigned int i = 0; i < indices.size(); i += (geom == 0 ? 3 : 4)) {
      glm::vec3 v0 = vertices.vertices[indices[i]];
      glm::vec3 v1 = vertices.vertices[indices[i + 1]];
      glm::vec3 v2 = vertices.vertices[indices[i + 2]];

      if (geom == 0) {
        flattened.triangles.vertices.push_back(v0);
        flattened.triangles.vertices.push_back(v1);
        flattened.triangles.vertices.push_back(v2);
      } else {
        glm::vec3 v3 = vertices.vertices[indices[i + 3]];
        flattened.quads.vertices.push_back(v0);
        flattened.quads.vertices.push_back(v1);
        flattened.quads.vertices.push_back(v2);
        flattened.quads.vertices.push_back(v3);
      }

      if (vertices.textures.size() > 0) {
        glm::vec2 t0 = vertices.textures[indices[i]];
        glm::vec2 t1 = vertices.textures[indices[i + 1]];
        glm::vec2 t2 = vertices.textures[indices[i + 2]];
        if (geom == 0) {
          flattened.triangles.textures.push_back(t0);
          flattened.triangles.textures.push_back(t1);
          flattened.triangles.textures.push_back(t2);
        } else {
          glm::vec2 t3 = vertices.textures[indices[i + 3]];
          flattened.quads.textures.push_back(t0);
          flattened.quads.textures.push_back(t1);
          flattened.quads.textures.push_back(t2);
          flattened.quads.textures.push_back(t3);
        }
      }
    }
  }
  return flattened;
}
