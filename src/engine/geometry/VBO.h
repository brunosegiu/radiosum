#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

class VBO {
 public:
  VBO();
  void addVertices(std::vector<glm::vec3> &vertices);
  void addIndices(std::vector<GLuint> &indices);
  void bind();
  void unbind();
  virtual ~VBO();

 private:
  GLuint GLVAOId;
  GLuint GLVerticesId, GLIndicesId;
};
