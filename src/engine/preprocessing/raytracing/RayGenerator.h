#pragma once

#include <chrono>
#include <random>

#include <GL/glew.h>

#include <glm.hpp>

class RayGenerator {
 public:
  RayGenerator(GLuint &nRays);
  glm::vec3 getHemisphereDir(GLuint step);

 private:
  std::vector<glm::vec3> directions;
};