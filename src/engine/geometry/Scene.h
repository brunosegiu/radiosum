#pragma once

#include "EngineTypes.h"
#include "geometry/Face.h"
#include "geometry/Mesh.h"

class Scene {
 public:
  Scene();

  void draw(GLuint shaderID, bool justGeometry = false);

  // Geometry
  GLuint size();
  Face getFace(GLuint index);
  void addMesh(std::string path);
  void addMesh(Mesh* mesh);
  std::vector<IndexedBuffers> getGeometry();

  // Lighting
  GLfloat getEmission(GLuint faceIndex);
  std::vector<GLfloat> getEmission();
  void setEmission(std::vector<GLfloat> emission);
  void setEmission(GLuint faceIndex, GLfloat emission, bool full = false);

  glm::vec3 getReflactance(GLuint faceIndex);
  std::vector<glm::vec3> getReflactance();
  void setReflactance(std::vector<glm::vec3> reflactance);
  void setReflactance(GLuint faceIndex, glm::vec3 reflactance,
                      bool full = false);

  GLfloat getSpecularReflactance(GLuint faceIndex);
  std::vector<GLfloat> getSpecularReflactance();
  void setSpecularReflactance(std::vector<GLfloat> specularReflactance);
  void setSpecularReflactance(GLuint faceIndex, GLfloat specularReflactance,
                              bool full = false);

  std::vector<glm::vec3> getRadiosity();
  void setRadiosity(std::vector<glm::vec3> radiosity);
  void setRadiosity(std::vector<glm::vec3>& radiosity, bool smooth);

  virtual ~Scene();

 private:
  std::vector<Mesh*> meshes;
  Mesh* getMeshWithIndex(GLuint faceIndex, GLuint& meshIndex);
  GLuint _size;
};

class SceneIterator {
 public:
  SceneIterator(Scene* scene);

  GLuint begin();
  bool end();

  GLuint nextFace();
  GLuint nextVertex();

  Face get();
  GLuint faceIndex();

 private:
  Scene* scene;
  GLuint index, top;
};
