#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

enum FaceMode { TRIANGLE, QUAD };

class Face {
 public:
  Face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
  Face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
  glm::vec3 getNormal();
  glm::vec3 getBarycenter();
  glm::vec3 getUp();
  glm::vec4 getPlane();
  GLfloat area();
  std::vector<glm::vec3> getSamplingPoints();
  virtual ~Face();

 public:
  glm::vec3 v0, v1, v2, v3;
  FaceMode mode;
};
