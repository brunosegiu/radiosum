#pragma once

#include <chrono>
#include <random>

#include <GL/glew.h>

#include <glm.hpp>

class RayGenerator {
 public:
  RayGenerator();
  glm::vec3 getDir(glm::vec3 &origDir);
  void setBaseChangeMatrix(glm::vec3 normal);

 private:
  std::mt19937_64 rng;
  std::uniform_real_distribution<GLfloat> uniformGenerator;
  glm::mat3 baseChangeMaxtrix;
};