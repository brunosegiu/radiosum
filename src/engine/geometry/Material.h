#pragma once

#include <vector>

#include <glm.hpp>

#include "Texture.h"
#include "geometry/VAO.h"

#define VERTICES_ID 0
#define EMISSION_ID 1
#define REFLACTANCE_ID 2
#define RADIOSITY_ID 3
#define TEXTURES_ID 4
#define NORMALS_ID 5

// Tied to geometry

class Material {
 public:
  Material(glm::vec3 reflactance = glm::vec3(.1f), GLfloat emission = .0f,
           Texture* texture = nullptr);

  void instance(GLuint faceCount, GLuint vertexCount, GLuint tFaces, VAO* vao);
  void bind();

  std::vector<GLfloat> getEmission();
  GLfloat getEmission(GLuint faceIndex);
  void setEmission(GLuint faceIndex, GLfloat emission);
  void setEmission(GLfloat emission);
  void setEmission(std::vector<GLfloat> emission);

  std::vector<glm::vec3> getReflactance();
  glm::vec3 getReflactance(GLuint faceIndex);
  void setReflactance(GLuint faceIndex, glm::vec3 reflactance);
  void setReflactance(glm::vec3 reflactance);
  void setReflactance(std::vector<glm::vec3> reflactance);

  ~Material();

 private:
  std::vector<GLfloat> emissions;
  GLfloat emission;
  std::vector<glm::vec3> reflactances;
  glm::vec3 reflactance;
  Texture* texture;
  GLuint faceCount, vertexCount, tFaces;
  VAO* vao;
};