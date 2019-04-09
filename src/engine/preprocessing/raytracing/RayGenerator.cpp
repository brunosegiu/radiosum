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

glm::vec3 RayGenerator::getHemisphereDir(glm::vec3 &origDir) {
  glm::vec3 dir;

  glm::vec2 rand(uniformGenerator(rng), uniformGenerator(rng));

  GLfloat sinTheta = sqrtf(1 - rand.x * rand.x);
  GLfloat phi = 2 * M_PI * rand.y;
  GLfloat x = sinTheta * cosf(phi);
  GLfloat z = sinTheta * sinf(phi);

  return glm::vec3(x, rand.x, z);
}
