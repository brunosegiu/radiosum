#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

#include "geometry/Face.h"
#include "geometry/GeometryContainer.h"
#include "geometry/Material.h"
#include "geometry/VAO.h"
#include "geometry/VBO.h"

class Mesh {
 public:
  static std::vector<Mesh*> load(std::string path);

  Mesh(IndexedBuffers geometry, Material* material);

  void draw(GLuint shaderID);
  void drawGeometry(GLuint shaderID);
  void drawFace(GLuint shaderID, GLuint faceIndex);

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

  std::vector<GLfloat> getSpecularReflactance();
  GLfloat getSpecularReflactance(GLuint faceIndex);
  void setSpecularReflactance(GLuint faceIndex, GLfloat specularReflactance);
  void setSpecularReflactance(GLfloat specularReflactance);
  void setSpecularReflactance(std::vector<GLfloat> specularReflactance);

  std::vector<glm::vec3> getRadiosity();
  void setRadiosity(std::vector<glm::vec3> radiosity, bool smooth = true);

  IndexedBuffers getGeometry();

  GLuint size();
  Face getFace(GLuint index);

  virtual ~Mesh();

 private:
  IndexedBuffers geometry;
  std::vector<glm::vec3> radiosity;
  Material* material;

  std::vector<std::vector<GLuint>> adjacencies;

  VAO* vao;
  VBO* simpleGeometry;  // Hanzo
  GLuint faces, tFaces, qFaces;
};
