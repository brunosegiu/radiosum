#include "geometry/VBO.h"

VBO::VBO() {
  this->GLVAOId = 0;
  this->GLVerticesId = 0;
  this->GLIndicesId = 0;
  glGenVertexArrays(1, &this->GLVAOId);
}

void VBO::addVertices(std::vector<glm::vec3> &vertices) {
  glBindVertexArray(GLVAOId);
  glGenBuffers(1, &GLVerticesId);
  glBindBuffer(GL_ARRAY_BUFFER, GLVerticesId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
               &vertices[0].x, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
}

void VBO::addIndices(std::vector<GLuint> &indices) {
  glGenBuffers(1, &GLIndicesId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLIndicesId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
               &indices[0], GL_STATIC_DRAW);
}

void VBO::bind() {
  glBindVertexArray(GLVAOId);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLIndicesId);
}

void VBO::unbind() {
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VBO::~VBO() {
  glBindVertexArray(this->GLVAOId);
  glDeleteBuffers(1, &GLVerticesId);
  glDeleteBuffers(1, &GLIndicesId);
  glDeleteVertexArrays(1, &this->GLVAOId);
}
