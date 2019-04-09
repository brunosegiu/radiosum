#pragma once

#include <chrono>
#include <random>

#include <GL/glew.h>

#include <glm.hpp>

class RayGenerator {
 public:
  RayGenerator();
  glm::vec3 getHemisphereDir(glm::vec3 &origDir);

 private:
  std::mt19937_64 rng;
  std::uniform_real_distribution<GLfloat> uniformGenerator;
};