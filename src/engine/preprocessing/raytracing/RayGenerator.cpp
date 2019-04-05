#include "preprocessing/raytracing/RayGenerator.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <gtx/rotate_vector.hpp>

RayGenerator::RayGenerator() {
  // https://stackoverflow.com/questions/9878965/rand-between-0-and-1
  uint64_t timeSeed =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
  rng.seed(ss);
  uniformGenerator = std::uniform_real_distribution<GLfloat>(.0f, 1.0f);
}

void RayGenerator::setBaseChangeMatrix(glm::vec3 normal) {
  glm::vec3 nt, nb;

  if (std::fabs(normal.x) > std::fabs(normal.y))
    nt = glm::vec3(normal.z, 0, -normal.x) /
         sqrtf(normal.x * normal.x + normal.z * normal.z);
  else
    nt = glm::vec3(.0f, -normal.z, normal.y) /
         sqrtf(normal.y * normal.y + normal.z * normal.z);
  nb = glm::cross(normal, nt);

  this->baseChangeMaxtrix = glm::mat3(nb, normal, nt);
}

glm::vec3 RayGenerator::getDir(glm::vec3 &origDir) {
  glm::vec3 dir;

  glm::vec2 rand(uniformGenerator(rng), uniformGenerator(rng));

  GLfloat sinTheta = sqrtf(1 - rand.x * rand.x);
  GLfloat phi = 2 * M_PI * rand.y;
  GLfloat x = sinTheta * cosf(phi);
  GLfloat z = sinTheta * sinf(phi);

  return this->baseChangeMaxtrix * glm::vec3(x, rand.x, z);
}
